/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "message_transport_uart.h"
#include "message_transport_pal.h"
#include "fsl_lpuart.h"
#if SDK_OS_BAREMETAL
#include "swisr.h"
#elif SDK_OS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include "comm_command_proc_host.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NUMBER_OF_MESSAGE_TO_PROCESS 2
/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint8_t uartRxPortStatus;
volatile uint8_t uartTxPortStatus;
uint16_t uartRxBufIndex, uartTxBufIndex;
uint16_t uartBytesToTx;
uint8_t *pUartTxBuffer;

static serial_buffer_t msg_to_process[NUMBER_OF_MESSAGE_TO_PROCESS];
static uint8_t msg_read_idx, msg_write_idx;

/*******************************************************************************
 * Prototype
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void Comm_Process(void)
{
    e_meter_message_crc_t *s_msg_crc = NULL;

    while ((msg_to_process[msg_read_idx].buffer != NULL))
    {
        MessageTransport_RcvCallback(&msg_to_process[msg_read_idx], &s_msg_crc);

        uint32_t primask                    = DisableGlobalIRQ();
        msg_to_process[msg_read_idx].buffer = NULL;
        msg_to_process[msg_read_idx].size   = 0;
        msg_read_idx                        = (msg_read_idx + 1) % NUMBER_OF_MESSAGE_TO_PROCESS;
        EnableGlobalIRQ(primask);

        if (s_msg_crc != NULL)
        {
            MessageTransport_RcvProcess(s_msg_crc);
        }

        s_msg_crc = NULL;
    }
}

message_transport_status_t UART_MessageTransportSend(const serial_buffer_t *buffer)
{
    while (uartTxPortStatus == UART_TXING)
    {
        //	__asm__("BKPT");
        vTaskDelay(1);
    }

    UART_Transmit(UART_CONTROL_INDEX, buffer->buffer, buffer->size);
    return MessageTransport_NonBlocking;
}

message_transport_status_t UART_MessageTransportReceive(uint8_t *buffer, size_t *length)
{
    return MessageTransport_Ok;
}


/*!
 * brief Initializes the UART instances which are required for communication
 * with host clients.
 */
void Uart_ModuleInit(void)
{
    const serial_handler_message_handler_t serial_handler = {
        .send = UART_MessageTransportSend,
        .rcv  = UART_MessageTransportReceive,
    };

    MessageTransport_Init(&serial_handler);

    lpuart_config_t uartConfig;

    /* Host UART  */
    /*
     * uartConfig.baudRate_Bps = 115200;
     * uartConfig.parityMode = kUART_ParityDisabled;
     * uartConfig.stopBitCount = kUART_OneStopBit;
     * uartConfig.txFifoWatermark = 0;
     * uartConfig.rxFifoWatermark = 1;
     * uartConfig.enableTx = false;
     * uartConfig.enableRx = false;
     */
    LPUART_GetDefaultConfig(&uartConfig);
    uartConfig.baudRate_Bps = 115200U;
    uartConfig.enableTx     = true;
    uartConfig.enableRx     = true;

    LPUART_Init(SIGBRD_LPUART, &uartConfig, SIGBRD_LPUART_CLK_FREQ);
    /* Enable RX interrupt. */
    LPUART_EnableInterrupts(SIGBRD_LPUART, kLPUART_RxDataRegFullInterruptEnable | kLPUART_RxOverrunInterruptEnable);
    NVIC_SetPriority(SIGBRD_LPUART_IRQn, SIGBRD_LPUART_INT_PRIORITY);
    EnableIRQ(SIGBRD_LPUART_IRQn);

    uartRxPortStatus = uartTxPortStatus = UART_IDLE;
    uartRxBufIndex = uartTxBufIndex = 0;
}

#if (ENABLE_SIGBOARD && (SIGBRD == EM_HPGP))
/*!
 * @brief Callback function being invoked from the ISR of the UART instances.
 * - Rx operation: copies data bytes from UART DATA/FIFO to system RAM buffer.
 * Indicates to application tasks about the current state of Rx process.
 * - Tx operation: writes UART DATA register from system RAM buffer.
 * Indicates to application tasks about the current state of Tx process.
 *
 * param uart_index Index of the UART Port.
 */
void SIGBRD_LPUART_IRQHandler(void)
{
    volatile uint32_t uart_status;

    uart_status = LPUART_GetStatusFlags(SIGBRD_LPUART);

    if ((kLPUART_RxOverrunFlag)&LPUART_GetStatusFlags(SIGBRD_LPUART))
    {
        /* clear OR */
        (void)LPUART_ClearStatusFlags(SIGBRD_LPUART, kLPUART_RxOverrunFlag);
    }

    /* If new data arrived. */
    if (kLPUART_RxDataRegFullFlag & uart_status)
    {
        uint8_t rxByte;
        static serial_buffer_t *uartRxBuffer = NULL;

        rxByte = LPUART_ReadByte(SIGBRD_LPUART);

        /* Check if its the 1st byte of a receive sequence */
        if (uartRxPortStatus == UART_IDLE)
        {
            if (uartRxBuffer == NULL)
            {
                uartRxBuffer = MessageTransport_AcquireRcvBuffer();
            }

            if (uartRxBuffer != NULL)
            {
                /* Move to RXING only when a free buffer is available. If we lost part of the message
                 * the CRC will signal us. Up for the user to ask for retransmission */
                uartRxPortStatus = UART_RXING;
            }
        }

        if ((uartRxPortStatus == UART_RXING) && uartRxBuffer)
        {
            if (rxByte == EOL)
            {
                uartRxPortStatus   = UART_PROCESSING;
                uartRxBuffer->size = uartRxBufIndex;
            }
            else if (uartRxBuffer->buffer != NULL && uartRxBufIndex < uartRxBuffer->size)
            {
                uartRxBuffer->buffer[uartRxBufIndex++] = rxByte;
                uartRxBufIndex %= uartRxBuffer->size;
            }
        }

        if ((uartRxPortStatus == UART_PROCESSING) && uartRxBuffer)
        {
            uint32_t primask = DisableGlobalIRQ();
            if (msg_to_process[msg_write_idx].buffer == NULL)
            {
                msg_to_process[msg_write_idx].buffer = uartRxBuffer->buffer;
                msg_to_process[msg_write_idx].size   = uartRxBuffer->size;
                msg_write_idx                        = (msg_write_idx + 1) % NUMBER_OF_MESSAGE_TO_PROCESS;

#if SDK_OS_BAREMETAL
                NVIC_CallIsr(SWISR_VECTOR_0);
#elif defined(SDK_OS_FREE_RTOS)
                EVSE_UARTComm_SetEvent(EVSE_UART_MESSAGE_RECEIVED);
#endif
            }
            EnableGlobalIRQ(primask);
            uartRxBuffer     = NULL;
            uartRxPortStatus = UART_IDLE;
            uartRxBufIndex   = 0;
        }
    }
    else if (kLPUART_TransmissionCompleteFlag & uart_status)
    {
        if (uartBytesToTx == 0)
        {
            MessageTransport_SendCallback(pUartTxBuffer);
            uartTxBufIndex   = 0;
            uartTxPortStatus = UART_IDLE;
            /* Disable transmit */
            LPUART_DisableInterrupts(SIGBRD_LPUART, kLPUART_TransmissionCompleteInterruptEnable);
        }
        else
        {
            LPUART_WriteByte(SIGBRD_LPUART, *(pUartTxBuffer + uartTxBufIndex++));
            uartBytesToTx--;
        }
    }

    SDK_ISR_EXIT_BARRIER;
}
#endif /* ENABLE_SIGBOARD && (SIGBRD == EM_HPGP) */

/*!
 * @brief Initiates a character buffer data transmission through UART port.
 * Only the 1st byte is written to UART DATA register while the rest of the buffer
 * data are sent from the ISR callback.
 *
 * @param uart_index index of the uart channel.
 * @param txBuff     pointer of the transmission buffer.
 * @param txLength   length of the packet to be transmitted.
 */
void UART_Transmit(UART_INDEX uart_index, uint8_t *txBuff, uint16_t txLength)
{
    pUartTxBuffer = txBuff;
    uartBytesToTx = txLength;
    //	uartRxPortStatus = UART_IDLE;
    uartTxPortStatus = UART_TXING;
    uartTxBufIndex   = 0;
    /* Enable transmit */
    LPUART_EnableInterrupts(SIGBRD_LPUART, kLPUART_TransmissionCompleteInterruptEnable);
}

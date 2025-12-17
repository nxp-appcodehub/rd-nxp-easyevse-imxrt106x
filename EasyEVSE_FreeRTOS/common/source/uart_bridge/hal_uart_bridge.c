/*
 *
 *
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * --------------------------------------------------------------------
 *
 *        MODULE:       HAL UART BRIDGE driver
 *
 *        PURPOSE:      Hardware abstraction layer for the communication
 *        between host controller and signal board
 *
 *        COMMENTS:     -
 *
 * ------------------------------------------------------------------------*/

/* standard includes */

#include "hal_uart_bridge.h"
#include <stdio.h>
#include "fsl_lpuart.h"
#include <sys/time.h>

#if EASYEVSE
#include "EVSE_config.h"
#include "EVSE_Utils.h"
#include "EVSE_Metering.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#endif /* EasyEVSE */

#define UART_THREAD_PRIORITY (5)

#define SIGBRD_RESP_TIMEOUT 0x10

#define SIGBRD_EVSE_COMMAND_BUFFER_SIZE 8
#define SIGBRD_EV_COMMAND_BUFFER_SIZE   4
#define SIGBRD_EVSE_MAX_TRIES_WAIT      3

#define SIGBRD_QUEUE_SIZE           2
#define SIGBRD_END_OF_MSG           '\r'
#define SIGBRD_COMMAND_START_STRING "["
#define SIGBRD_COMMAND_START        '['
#define SIGBRD_COMMAND_START_SIZE   1
#define SIGBRD_COMMAND_END          ']'
#define SIGBRD_COMMAND_END_SIZE     1
#define SIGBRD_COMMAND_SIZE         1

#if EASYEVSE
#define DBGPRINTF_DEBUG(x) configPRINTF(x)
#define DBGPRINT_INFO(x)   configPRINTF(x)
#define perror(x)          configPRINTF((error(x)))
#define DBGPRINT_ERROR(x)  configPRINTF(x)
#else
#define DBGPRINTF_DEBUG(x)
#define DBGPRINT_INFO(x)
#define perror(x)
#define DBGPRINT_ERROR(x)
#endif /* EASYEVSE */

typedef enum _lpuart_state
{
    LPUART_STATE_IDLE = 0,
    LPUART_STATE_SENDING,
    LPUART_STATE_WAITING_RESPONSE,
    LPUART_STATE_MESSAGE_SEND,
} lpuart_state_t;

typedef struct _lpuart_message_format
{
    uint32_t size;
    char *buffer_addr;
} lpuart_message_format_t;

static bool response_received      = false;
static lpuart_state_t lpuart_state = LPUART_STATE_IDLE;
static char sigbrd_ring_buffer[SIGBRD_RING_BUFFER_NUMBERS][SIGBRD_RING_BUFFER_SIZE];
static volatile uint16_t
    rx_index_sigbrd[SIGBRD_RING_BUFFER_NUMBERS]; /* Index of the memory to save new arrived data. */
static uint8_t rx_write_index_buffer, rx_read_index_buffer;
static COMMAND_UART_CODE expected_reply_code;

#if EASYEVSE
static QueueHandle_t response_receive_queue, response_receive_manager_queue;
#else
uint32_t gdwTenMilliSecTimeout;
#endif /* EASYEVSE */

struct sigbrd_state_t SigbrdRawState = {
    kPPNotDetected, kCPStateF, false, 0, 0, 0, 0x3030, 0, {'\0'}, 0.0, 0.0, 0.0,
};

static uint32_t adc_sample_size        = 0;
static uint32_t iso15118_tick_delay_ms = 0;
static bool s_init = false;

static void reset_sigbrd_state_machine()
{
    expected_reply_code = CMD_ERR;
    lpuart_state        = LPUART_STATE_IDLE;
}

static void GetADCVal(char *sigBrdRingBuffer, uint32_t *adcValPtr)
{
    static uint8_t adcBuffIndex = 0;
    int ret;

    /* Set pointer to start of receiver array */
    ret = (int)strtod(&sigBrdRingBuffer[0], NULL); /* point ptr after the decimals */

    if (adcBuffIndex >= adc_sample_size)
    {
        adcBuffIndex = 0;
    }

    if (ret == 0)
    {
        if ((sigBrdRingBuffer[0] == '0') && (sigBrdRingBuffer[1] == '0') && (sigBrdRingBuffer[2] == '0') &&
            (sigBrdRingBuffer[3] == '0') && (sigBrdRingBuffer[4] == '0'))
        {
            *(adcValPtr + adcBuffIndex) = SigbrdRawState.ADCCPValue = 0;
        }
        else
        {
            DBGPRINT_ERROR(("GetADCVal() - string to decimal error"));
        }
    }
    else if (ret > 0)
    {
        *(adcValPtr + adcBuffIndex) = SigbrdRawState.ADCCPValue = ret / 16; /* ADC value */
    }

    adcBuffIndex++;
}
/**
 * @brief Write data over UART
 *
 * @param buf buffer from where to read the data
 * @param size size of the data that needs to be written
 * @param timeout timeout for the write procedure
 * @return uint32_t number of bytes written
 */
static uint32_t nblk_write_uart(const char *const buf, uint32_t size, const struct timeval *timeout)
{
    response_received = false;

    for (int i = 0; i < size; i++)
    {
        LPUART_WriteByte(SIGBRD_LPUART, buf[i]);
        if (buf[i] == SIGBRD_END_OF_MSG)
        {
            lpuart_state = LPUART_STATE_WAITING_RESPONSE;
        }

        /* Wait for UART Outputs to complete */
        while (!(LPUART_GetStatusFlags(SIGBRD_LPUART) & kLPUART_TxDataRegEmptyFlag))
        {
        }
    }

    /* TODO Might be worth adding wait for ACK */
    return size;
}

/**
 * @brief Read the UART
 *
 * @param buf buffer in which to read the data
 * @param size Size of the buffer
 * @param timeout timeout
 * @return uint32_t number of byte read
 */
static uint32_t nblk_read_uart(void *buf, uint32_t size, struct timeval *timeout)
{
    /* Wait for event sigbrd which is set by the SIGBRD_LPUART_IRQHandler only when full packet received from Meter */
    uint32_t timeoutms              = (timeout->tv_sec * 1000000 + timeout->tv_usec) / 1000;
    lpuart_message_format_t message = {0};
    uint32_t bytes_rcv              = 0;
#if EASYEVSE
    xQueueReceive(response_receive_queue, &message, pdMS_TO_TICKS(timeoutms));
#else
    gdwTenMilliSecTimeout = timeoutms / 10; // 10*10 = 100 msec timeout
    while ((!response_received) && (gdwTenMilliSecTimeout))
    {
    }
    if (response_received)
    {
        message.buffer_addr = sigbrd_ring_buffer[rx_read_index_buffer];
        message.size        = rx_index_sigbrd[rx_read_index_buffer];
        rx_read_index_buffer++;
    }
#endif /* EASYEVSE */

    if (response_received == true)
    {
        bytes_rcv = message.size;
        if (size < bytes_rcv)
        {
            DBGPRINT_ERROR(("%s Too many bytes rcv %d", __FUNCTION__, bytes_rcv));
            bytes_rcv = size;
        }

        memcpy(buf, message.buffer_addr, bytes_rcv);
    }

    return bytes_rcv;
}

/**
 * @brief Get the reply code object
 *
 * @param message_buffer message reply from sigboard
 * @return COMMAND_UART_CODE
 */
static COMMAND_UART_CODE get_reply_code(char *message_buffer)
{
    char cmd      = CMD_ERR;
    uint16_t size = 0;
    /*
     * Some commands in previous versions of the SIGBRD LPC55
     * firmware replied with a null byte instead of ASCII '0'.
     * Protect parser by forcing ASCII.
     */
    if (message_buffer[0] == '\0')
    {
        message_buffer[0] = '0';
    }

    size = strlen(message_buffer);

    for (uint32_t i = 0; i < size; i++)
    {
        if (message_buffer[i] == SIGBRD_COMMAND_START)
        {
            if (((i < (size - SIGBRD_COMMAND_SIZE - SIGBRD_COMMAND_START_SIZE)) &&
                 (message_buffer[i + SIGBRD_COMMAND_START_SIZE + SIGBRD_COMMAND_SIZE] == SIGBRD_COMMAND_END)))
            {
                if ((cmd >= METER_ALL) && (cmd <= METER_LAST))
                {
                    /* we found another cmd related to Meter this means METER_ALL*/
                    cmd = METER_ALL;
                    break;
                }
                cmd = message_buffer[i + SIGBRD_COMMAND_START_SIZE];
            }
        }
    }

    return cmd;
}

/**
 * @brief Parse the reply in order to get the values from the message_buffer
 *
 * @param message_buffer message reply from sigboard
 * @param value_returned where to store the parse value.
 * @return COMMAND_UART_CODE
 */
static COMMAND_UART_CODE parse_reply(char *message_buffer, uint32_t *value_returned)
{
    char sigboardRing[SIGBRD_RING_BUFFER_SIZE];
    char *value = NULL;
    char cmd    = CMD_ERR;
    int i_resp  = -1;

    cmd = get_reply_code(message_buffer);

    /* strtok() is destructive, back up the read buffer */
    memcpy(sigboardRing, message_buffer, SIGBRD_RING_BUFFER_SIZE);

    value = strtok(sigboardRing, SIGBRD_COMMAND_START_STRING); /*Looking for the value before [*/

    if (value == NULL)
    {
        return CMD_ERR;
    }

    switch (cmd)
    {
        case ALL_STATES:
        case PP_STATE:
        case CP_STATE:
        case GFCI_STATE:
        case ADC_CP_VALUE:
        case ADC_PP_VALUE:
        case GET_PWM_DUTY:
        case CP_RESISTOR_VALUE:
        case PWM_DUTY_PER_MILLI:
        case CLOSE_RELAY:
        case OPEN_RELAY:
        case SET_CP:
            i_resp = atoi(value);
            break;
        case HWVERSION:
            i_resp = *value;
            break;
        case VERSION:
            strcpy(SigbrdRawState.version, value);
            break;
#if ((EASYEVSE == 1) && (ENABLE_METER == 1))
        case METER_ALL:
        case METER_CURRENT:
        case METER_VOLTAGE:
        case METER_POWER:
        case METER_STATE:
        {
            meter_data_t parsed_meter_data = {0};
            uint32_t found_fields          = 0;

            EVSE_Meter_ParseMeterReply(message_buffer, strlen(message_buffer) - 1, &parsed_meter_data, &found_fields);
            EVSE_Meter_SetMeterData(&parsed_meter_data, found_fields);
            if (found_fields & (1 << METER_FIELD_IRMS))
            {
                SigbrdRawState.current = parsed_meter_data.irms;
            }
            if (found_fields & (1 << METER_FIELD_VRMS))
            {
                SigbrdRawState.voltage = parsed_meter_data.vrms;
            }
            if (found_fields & (1 << METER_FIELD_WH))
            {
                SigbrdRawState.power = parsed_meter_data.wh;
            }
        }
        break;
#endif /* EASYEVSE */
    }

    switch (cmd)
    {
        case HWVERSION:
            if (value_returned != NULL)
            {
                *value_returned = i_resp;
            }
            break;
        case PP_STATE:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - The Proximity Pilot State is %d \r\n", i_resp));
            SigbrdRawState.PPState = (kPPState)i_resp;

            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.PPState;
            }
            break;
        case CP_STATE:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - The Control Pilot State is %d \r\n", i_resp));
            SigbrdRawState.CPState = (kCPState)i_resp;
            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.CPState;
            }
            break;
        case GFCI_STATE:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - The GFCI State is %d \r\n", i_resp));
            SigbrdRawState.GFCIState = (uint16_t)i_resp;
            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.GFCIState;
            }
            break;
        case ADC_CP_VALUE:
            if (value_returned != NULL)
            {
                *value_returned = i_resp;
                // DBGPRINTF_DEBUG (("[UART_BRIDGE] - The ADC Value of Control Pilot is %d \n", i_resp));
                GetADCVal(&sigboardRing[0], value_returned);
            }
            else
            {
                DBGPRINTF_DEBUG(("Response code 0x%x \r\n", cmd));
            }
            break;
        case ADC_PP_VALUE:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - The ADC Value of Proximity Pilot is %d \r\n", i_resp));
            SigbrdRawState.ADCPPValue = (uint32_t)i_resp;
            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.ADCPPValue;
            }
            break;
        case GET_PWM_DUTY:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - Get PWM duty cycle is %d \r\n", i_resp));
            SigbrdRawState.PWMDutyCycle = (uint32_t)i_resp;
            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.PWMDutyCycle;
            }
            break;
        case CP_RESISTOR_VALUE:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - The CP_RESISTOR_VALUE is %d \r\n", i_resp));
            if (value_returned != NULL)
            {
                *value_returned = i_resp;
            }
            break;
        case PWM_DUTY_PER_MILLI:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - PWM_DUTY_PER_MILLI is: %s 0x%x \r\n", value, cmd));
            break;
        case CLOSE_RELAY:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - Close Relay %d \r\n", i_resp));
            break;
        case OPEN_RELAY:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - Open Relay %d \r\n", i_resp));
            break;
        case SET_CP:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - SET_CP is: %d \r\n", i_resp));
            break;
        case VERSION:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - Version %s \r\n", value));
            break;
#if ((EASYEVSE == 1) && (ENABLE_METER == 1))
            /* TODO */
        case METER_ALL:
            /* Do nothing */
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - METER_ALL \r\n"));
            break;
        case METER_CURRENT:
            //   DBGPRINTF_DEBUG (("[UART_BRIDGE] - METER_CURRENT \r\n"));
            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.current;
            }

            break;
        case METER_VOLTAGE:
            //    DBGPRINTF_DEBUG (("[UART_BRIDGE] - METER_VOLTAGE \r\n"));
            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.voltage;
            }
            break;
        case METER_POWER:
            //    DBGPRINTF_DEBUG (("[UART_BRIDGE] - METER_POWER \r\n"));
            if (value_returned != NULL)
            {
                *value_returned = SigbrdRawState.power;
            }
            break;
        case METER_STATE:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - METER_State \r\n"));
            break;
#endif /* EASYEVSE */
        case CMD_UNKNOWN:
            DBGPRINTF_DEBUG(("[UART_BRIDGE] - WARNING: error response received \r\n"));
            break;
        default:
            // Log event
            DBGPRINTF_DEBUG(("[UART_BRIDGE] Unexpected reply: \"%s\" \r\n", sigboardRing));
            break;
    }
    /*
     * TODO: How to handle ALL_STATE and METER_ALL commands from
     * both the parsing perspective (easy) and the value returned
     * perspective (hard)?
     */

    return cmd;
}

static void Init_SigBrd_Uart(void)
{
    lpuart_config_t uart_config;

    /* Set up Meter UART
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&uart_config);
    uart_config.baudRate_Bps = 115200U;
    uart_config.enableTx     = true;
    uart_config.enableRx     = true;
    LPUART_Init(SIGBRD_LPUART, &uart_config, SIGBRD_LPUART_CLK_FREQ);
    /* Enable on RX interrupt. */
    LPUART_EnableInterrupts(SIGBRD_LPUART, kLPUART_RxDataRegFullInterruptEnable | kLPUART_RxOverrunInterruptEnable);
    NVIC_SetPriority(SIGBRD_LPUART_IRQn, 7);
    NVIC_ClearPendingIRQ(SIGBRD_LPUART_IRQn);
    EnableIRQ(SIGBRD_LPUART_IRQn);
}

#if (ENABLE_SIGBOARD == 1)
void SIGBRD_LPUART_IRQHandler(void)
{
    volatile uint32_t uart_status;

    char *readBuffer = sigbrd_ring_buffer[rx_write_index_buffer];

    uart_status = LPUART_GetStatusFlags(SIGBRD_LPUART);

    if ((kLPUART_RxOverrunFlag)&uart_status)
    {
        /* clear OR */
        LPUART_ClearStatusFlags(SIGBRD_LPUART, kLPUART_RxOverrunFlag);
    }

    /* Check if new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&uart_status)
    {
        char data = LPUART_ReadByte(SIGBRD_LPUART);
        if (rx_index_sigbrd[rx_write_index_buffer] < SIGBRD_RING_BUFFER_SIZE)
        {
            readBuffer[rx_index_sigbrd[rx_write_index_buffer]] = data;

            rx_index_sigbrd[rx_write_index_buffer]++;
            if (rx_index_sigbrd[rx_write_index_buffer] == SIGBRD_RING_BUFFER_SIZE)
            {
                /* restrict sequence of rxIndex to 0..SIGBRD_RING_BUFFER_SIZE */
                rx_index_sigbrd[rx_write_index_buffer] = 0;
            }
            else if (data == SIGBRD_END_OF_MSG)
            {
                /* set NULL character at end of data for str function handling*/
                readBuffer[rx_index_sigbrd[rx_write_index_buffer]] = 0;

                if (lpuart_state == LPUART_STATE_WAITING_RESPONSE)
                {
                    /* Set event flag 0 to wakeup meter_refresh thread.  */
                    response_received    = true;
                    rx_read_index_buffer = rx_write_index_buffer;
                }

#if EASYEVSE
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;
                lpuart_message_format_t message     = {.buffer_addr = sigbrd_ring_buffer[rx_write_index_buffer],
                                                       .size        = rx_index_sigbrd[rx_write_index_buffer]};

                xQueueSendFromISR(response_receive_manager_queue, &message, &xHigherPriorityTaskWoken);
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
#endif /* EASYEVSE */

                rx_write_index_buffer = (rx_write_index_buffer + 1) % SIGBRD_RING_BUFFER_NUMBERS;
                if (rx_index_sigbrd[rx_write_index_buffer])
                {
                    rx_index_sigbrd[rx_write_index_buffer] = 0;
                }
            }
        }
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif /* (ENABLE_SIGBOARD == 0) */

void SIGBRD_GetPPState(uint16_t *pp_state)
{
    if (SIGBRD_EVSE_UARTCommsProcess(PP_STATE, 0, NULL))
        return;

    if (pp_state != NULL)
    {
        *pp_state = SigbrdRawState.PPState;
    }
}

void SIGBRD_GetCPState(uint16_t *cp_state)
{
    if (SIGBRD_EVSE_UARTCommsProcess(CP_STATE, 0, NULL))
        return;

    if (cp_state != NULL)
    {
        *cp_state = SigbrdRawState.CPState;
    }
}

void SIGBRD_GetGFCIState(uint16_t *gfci_state)
{
    if (SIGBRD_EVSE_UARTCommsProcess(GFCI_STATE, 0, NULL))
        return;

    if (gfci_state != NULL)
    {
        *gfci_state = (uint16_t)SigbrdRawState.GFCIState;
    }
}

void SIGBRD_CloseRelay(void)
{
    uint32_t value;
    SIGBRD_EVSE_UARTCommsProcess(CLOSE_RELAY, 0, &value);
}

void SIGBRD_OpenRelay(void)
{
    uint32_t value;
    SIGBRD_EVSE_UARTCommsProcess(OPEN_RELAY, 0, &value);
}

/*
 * Public meter accessor functions.
 */
void SIGBRD_GetMeterCurrent(double *current)
{
    if (SIGBRD_EVSE_UARTCommsProcess(METER_CURRENT, 0, NULL))
        return;

    if (current != NULL)
    {
        *current = SigbrdRawState.current;
    }
}

void SIGBRD_GetMeterVoltage(double *voltage)
{
    if (SIGBRD_EVSE_UARTCommsProcess(METER_VOLTAGE, 0, NULL))
        return;

    if (voltage != NULL)
    {
        *voltage = SigbrdRawState.voltage;
    }
}

void SIGBRD_GetMeterPower(double *power)
{
    if (SIGBRD_EVSE_UARTCommsProcess(METER_POWER, 0, NULL))
        return;

    if (power != NULL)
    {
        *power = SigbrdRawState.power;
    }
}

void SIGBRD_GetADCVal(uint32_t *adcValPtr, uint32_t ulSize)
{
    adc_sample_size = ulSize;
    if (SIGBRD_EVSE_UARTCommsProcess(ADC_CP_VALUE, 0, adcValPtr))
        return;

#if DEBUG
    static uint32_t debug_display = 1;
    if (debug_display == 0)
    {
        DBGPRINTF_DEBUG(("Print ADC last value: %d", SigbrdRawState.ADCCPValue * 16));
        if (CP_READ_TIMEOUT > iso15118_tick_delay_ms)
        {
            debug_display = 1000 / CP_READ_TIMEOUT;
        }
        else
        {
            debug_display = 1000 / iso15118_tick_delay_ms;
        }
    }

    debug_display--;
#endif /* DEBUG */
}

void SIGBRD_SetPWMDutyInPercent(uint16_t dutyCycle)
{
    SIGBRD_SetPWMDutyInPerMilli(dutyCycle * 10);
}

void SIGBRD_SetPWMDutyInPerMilli(uint16_t dutyCycle)
{
    static uint16_t set_dutyVal_last = 0;

    DBGPRINT_ERROR(("SIGBRD_SetPWMDutyInPerMilli %d", dutyCycle));
    if (dutyCycle == set_dutyVal_last)
    {
        return;
    }
    else
    {
        set_dutyVal_last = dutyCycle;
    }

    SIGBRD_EVSE_UARTCommsProcess(PWM_DUTY_PER_MILLI, dutyCycle, NULL);
}

void SIGBRD_SetCPResistorValue(kCPResistor gpioNumber, kCPResistorState gpioState)
{
    uint16_t value_resistor = 0;

    if (gpioNumber == kCPEVResistor_270R)
    {
    }
    else if (gpioNumber == kCPEVResistor_1K3)
    {
        value_resistor = 0x3130 | gpioState;
    }

    SIGBRD_EV_UARTCommsProcess(SET_CP, value_resistor, NULL);
}

/*******************************************************************************
 * SIGBRD_EVSE_UARTCommsProcess(char command_code, uint32_t value_transmitted,
 *                             uint32_t *value_returned)
 * DESCRIPTION:
 * char command_code = UART Code for the sigboard and meter command.
 * uint32_t value_transmitted = Value to be transmited code+value
 * uint32_t *value_returned = Response data from the sigboard
 * RETURNS:
 * None
 ******************************************************************************/
int SIGBRD_EVSE_UARTCommsProcess(char command_code, uint16_t value_transmitted, uint32_t *value_returned)
{
    int status             = 0;
    struct timeval timeout = {0, 20000};
    int len                = 0;
    char message_rcv[SIGBRD_RING_BUFFER_SIZE];
    char command[SIGBRD_EVSE_COMMAND_BUFFER_SIZE] = {' ', ' ',' ',' ', ' ', ' ', ' ', SIGBRD_END_OF_MSG};
    COMMAND_UART_CODE reply_code                  = CMD_ERR;

    uint8_t bSize_command = 0;
    uint8_t bWait_tries   = 0;
 
    if (s_init == false)
    {
        return -1;
    }

    memset(message_rcv, 0, SIGBRD_RING_BUFFER_SIZE);

    command[bSize_command++] = command_code;
    switch (command_code)
    {
        case CLOSE_RELAY:
        case OPEN_RELAY:
            /* Closing and opening relay is a costing operation */
            timeout.tv_usec = 40000;
        case ALL_STATES:
        case PP_STATE:
        case CP_STATE:
        case GFCI_STATE:
        case ADC_CP_VALUE:
        case ADC_PP_VALUE:
        case CP_RESISTOR_VALUE:
        case METER_ALL:
        case METER_CURRENT:
        case METER_VOLTAGE:
        case METER_POWER:
        case METER_STATE:
        case METER_Q:
        case METER_S:
        case HWVERSION:
        case VERSION:
            /* Get parameters*/
            command[bSize_command++] = SIGBRD_END_OF_MSG;
            break;
        case PWM_DUTY_PER_MILLI:
            /* Set PWM duty cycle Value */
            bSize_command += sprintf(&command[1], "%05d%c", value_transmitted, SIGBRD_END_OF_MSG);
            SigbrdRawState.PWMDutyPerMilli = value_transmitted;
            break;
        default:
            // Log event
            DBGPRINT_ERROR(("SIGBRD - Command error \n\r"));
            return -2;
            break;
    }

#if EASYEVSE
    EVSE_LPUART_GetMutex(EVSE_LPUART_GetInstance(SIGBRD_LPUART));
#endif /* EASYEVSE */

    /* send command to SIG-BRDX */

    lpuart_state        = LPUART_STATE_SENDING;
    expected_reply_code = command_code;

    len = nblk_write_uart(command, bSize_command, &timeout);

    if (len < bSize_command)
    {
        /* Failed to write */
        DBGPRINT_ERROR(("SIGBRD_EVSE_UARTCommsProcess() - failed to write %d", len));
        status = -1;
        goto exit_uart_send;
    }

    while ((reply_code != command_code) && (bWait_tries < SIGBRD_EVSE_MAX_TRIES_WAIT))
    {
        len = nblk_read_uart(message_rcv, sizeof(message_rcv), &timeout);

        if (len <= 0)
        {
            DBGPRINT_ERROR(("SIGBRD_EVSE_UARTCommsProcess() - empty read or select timeout"));
            bWait_tries++;
        }
        else
        {
            reply_code = parse_reply(message_rcv, value_returned);
        }
    }
    if (bWait_tries == SIGBRD_EVSE_MAX_TRIES_WAIT)
    {
        status = -1;
        DBGPRINT_ERROR(
            ("SIGBRD_EVSE_UARTCommsProcess() - failed to receive message [command code: %c]", expected_reply_code));
    }
    else
    {
        while (lpuart_state != LPUART_STATE_MESSAGE_SEND)
        {
            vTaskDelay(1);
        }
    }
exit_uart_send:
    reset_sigbrd_state_machine();
#if EASYEVSE
    EVSE_LPUART_PostMutex(EVSE_LPUART_GetInstance(SIGBRD_LPUART));
#endif /* EASYEVSE */
    return status;
}

void SIGBRD_GetPWMDutyInPercent(uint16_t *dutyVal)
{
    uint16_t dutyVal_Milli = 0;

    if (dutyVal == NULL)
    {
        return;
    }

    SIGBRD_GetPWMDutyInPerMilli(&dutyVal_Milli);
    *dutyVal = dutyVal_Milli / 10;
}

void SIGBRD_GetPWMDutyInPerMilli(uint16_t *dutyVal)
{
    static uint8_t get_dutyVal_count = 0;

    if (dutyVal == NULL)
    {
        return;
    }

    if (get_dutyVal_count == 0)
    {
        if (SIGBRD_EV_UARTCommsProcess(GET_PWM_DUTY, 0, &SigbrdRawState.PWMDutyCycle))
            return;
    }

    get_dutyVal_count++;

    if (SigbrdRawState.PWMDutyCycle < 0xFFFF)
    {
        *dutyVal = SigbrdRawState.PWMDutyCycle;
    }
    else
    {
        *dutyVal = 0xFFFF;
    }

    if (get_dutyVal_count == 10)
    {
        get_dutyVal_count = 0;
    }
}

void SIGBRD_GetSWVersion(uint32_t *sw_version_major, uint32_t *sw_version_minor, uint32_t *sw_version_hotfix)
{
    if ((sw_version_major == NULL) || (sw_version_minor == NULL) || (sw_version_hotfix == NULL))
    {
        return;
    }

#ifndef EASYEVSE_EV
    SIGBRD_EVSE_UARTCommsProcess(VERSION, 0, NULL);
#else
    SIGBRD_EV_UARTCommsProcess(VERSION, 0, NULL);
#endif

    *sw_version_major  = -1;
    *sw_version_minor  = -1;
    *sw_version_hotfix = -1;

    uint8_t last_parsing_element_index = 0;
    for (uint8_t i = 0; i < SIGBRD_RING_BUFFER_SIZE; i++)
    {
        if (SigbrdRawState.version[i] == '.')
        {
            SigbrdRawState.version[i] = '\0';

            uint32_t version           = atoi(&SigbrdRawState.version[last_parsing_element_index]);
            last_parsing_element_index = i + 1;

            if (*sw_version_major == -1)
            {
                *sw_version_major = version;
            }
            else if (*sw_version_minor == -1)
            {
                *sw_version_minor = version;
            }
        }
        else if (SigbrdRawState.version[i] == '\0')
        {
            uint32_t version = atoi(&SigbrdRawState.version[last_parsing_element_index]);
            if (*sw_version_hotfix == -1)
            {
                *sw_version_hotfix = version;
            }
            break;
        }
    }
}

void SIGBRD_GetGetHWVersion(uint32_t *hw_version)
{
    if (hw_version == NULL)
        return;

    *hw_version = INVALID_SIGBOARD_HW_VERSION;

#ifndef EASYEVSE_EV
    SIGBRD_EVSE_UARTCommsProcess(HWVERSION, 0, hw_version);
#else
    SIGBRD_EV_UARTCommsProcess(HWVERSION, 0, hw_version);
#endif
}

void SIGBRD_GetCPResistorValue(kCPResistor gpioNumber, kCPResistorState *gpioState)
{
    char resistor_number = '0';

    if (gpioState == NULL)
    {
        return;
    }

    if (gpioNumber == kCPEVResistor_270R)
    {
        resistor_number = '0'; /*  270 Ohm */
        *gpioState      = 0;
    }
    else if (gpioNumber == kCPEVResistor_1K3)
    {
        resistor_number = '1'; /* 1.3k Ohm */
        if (SIGBRD_EV_UARTCommsProcess(CP_RESISTOR_VALUE, (uint16_t)resistor_number, (uint32_t *)gpioState))
            return;
    }

    if (resistor_number == '0')
    {
        SigbrdRawState.CPResistor = (*gpioState << 8)                       /* 270 Ohm read from Sigboard */
                                    | (SigbrdRawState.CPResistor & 0x30ff); /* 1.3k Ohm cached value       */
    }
    else if (resistor_number == '1')
    {
        SigbrdRawState.CPResistor = (SigbrdRawState.CPResistor & 0xff30) /* 270 Ohm cached value       */
                                    | *gpioState;                        /* 1.3k Ohm read from Sigboard */
    }
}

/*******************************************************************************
 * SIGBRD_EV_UARTCommsProcess(char command_code, uint16_t value_resistor,
 * uint32_t *value_resp)
 * DESCRIPTION:
 * char command_code = UART Code for the EV Sigboard and meter command.
 * uint16_t value_resistor = Resistor 270 Ohms = 0 or 1.3K Ohms = 1
 * uint32_t *value_resp = Resistor State 1 is ON and 0 is OFF
 * RETURNS:
 * status of UART transmission
 ******************************************************************************/
int SIGBRD_EV_UARTCommsProcess(char command_code, uint16_t value_transmitted, uint32_t *value_returned)
{
    int status = 0;
    char message_rcv[SIGBRD_RING_BUFFER_SIZE];
    char command[SIGBRD_EV_COMMAND_BUFFER_SIZE] = {' ', ' ', ' ', SIGBRD_END_OF_MSG};
    struct timeval timeout                      = {0, 20000};
    int len                                     = 0;

    COMMAND_UART_CODE reply_code = CMD_ERR;

    uint8_t bSize_command = 0;
    uint8_t bWait_tries   = 0;

    if (s_init == false)
    {
        return -1;
    }
    memset(message_rcv, 0, SIGBRD_RING_BUFFER_SIZE); /*clear message_buffer before read*/

    switch (command_code)
    {
        case ALL_STATES:
        case PP_STATE:
        case GET_PWM_DUTY:
        case HWVERSION:
        case VERSION:
            /* Get parameters*/
            command[1]    = SIGBRD_END_OF_MSG;
            bSize_command = 2;
            break;
        case CP_RESISTOR_VALUE:
            /* Get parameters*/
            command[1]    = (char)value_transmitted;
            command[2]    = SIGBRD_END_OF_MSG;
            bSize_command = 3;
            break;
        case SET_CP:
            /* Set Control Pilot Resistor */
            command[1]                = (char)(value_transmitted >> 8);
            command[2]                = (char)(value_transmitted & 0x00FF);
            command[3]                = SIGBRD_END_OF_MSG;
            bSize_command             = 4;
            SigbrdRawState.CPResistor = value_transmitted;
            break;
        default:
            // Log event
            return -2;
            break;
    }

    command[0] = command_code;

#if EASYEVSE
    EVSE_LPUART_GetMutex(EVSE_LPUART_GetInstance(SIGBRD_LPUART));
#endif /* EASYEVSE */

    lpuart_state        = LPUART_STATE_SENDING;
    expected_reply_code = command_code;

    len = nblk_write_uart(command, bSize_command, &timeout);

    lpuart_state = LPUART_STATE_WAITING_RESPONSE;

    if (len < bSize_command)
    {
        /* Failed to write */
        DBGPRINT_ERROR(("SIGBRD_EV_UARTCommsProcess() - failed to write %d", len));
        status = -1;
        goto exit_uart_send;
    }

    while ((reply_code != command_code) && (bWait_tries < SIGBRD_EVSE_MAX_TRIES_WAIT))
    {
        len = nblk_read_uart(message_rcv, sizeof(message_rcv), &timeout);
        if (len <= 0)
        {
            DBGPRINT_ERROR(("SIGBRD_EV_UARTCommsProcess() - empty read or select timeout"));
            bWait_tries++;
        }
        else
        {
            reply_code = parse_reply(message_rcv, value_returned);
        }
    }

    if (bWait_tries == SIGBRD_EVSE_MAX_TRIES_WAIT)
    {
        status = -1;
        DBGPRINT_ERROR(
            ("SIGBRD_EV_UARTCommsProcess() - failed to receive message [command code: %c]", expected_reply_code));
    }
    else
    {
        while (lpuart_state != LPUART_STATE_MESSAGE_SEND)
        {
            vTaskDelay(1);
        }
    }

exit_uart_send:
    reset_sigbrd_state_machine();
#if EASYEVSE
    EVSE_LPUART_PostMutex(EVSE_LPUART_GetInstance(SIGBRD_LPUART));
#endif /* EASYEVSE */
    return status;
}

#if EASYEVSE
static void bridge_manager_task(void *pvParameters)
{
    lpuart_message_format_t message = {0};

    while (1)
    {
        xQueueReceive(response_receive_manager_queue, &message, portMAX_DELAY);

        COMMAND_UART_CODE reply_code = get_reply_code(message.buffer_addr);

        if ((lpuart_state == LPUART_STATE_WAITING_RESPONSE) && (reply_code == expected_reply_code))
        {
            if (xQueueSend(response_receive_queue, &message, 0) == pdPASS)
            {
                lpuart_state = LPUART_STATE_MESSAGE_SEND;
            }
            else
            {
                DBGPRINT_ERROR(("Failed to put message in the RCV queue"));
            }
        }
        else
        {
            /* Async */
            parse_reply(message.buffer_addr, NULL);
        }
    }
}
#endif /* EASYEVSE */

void SIGBRD_SetChargingProtocol_TickDelayMs(uint32_t delay)
{
    iso15118_tick_delay_ms = delay;
}

void SIGBRD_UART_BridgeEntry(void)
{

    if (s_init == true)
    {
        return;
    }

    reset_sigbrd_state_machine();
#if EASYEVSE
    EVSE_LPUART_InitMutex(EVSE_LPUART_GetInstance(SIGBRD_LPUART));
    /* Attempt to create a semaphore. */
    response_receive_queue         = xQueueCreate(SIGBRD_QUEUE_SIZE, sizeof(lpuart_message_format_t));
    response_receive_manager_queue = xQueueCreate(SIGBRD_QUEUE_SIZE, sizeof(lpuart_message_format_t));

    if ((response_receive_queue == NULL) || (response_receive_manager_queue == NULL))
    {
        while (1)
            ;
    }

    if (xTaskCreate(bridge_manager_task, "Bridge_Manager", APP_BRIDGE_MANAGER_STACK_SIZE, NULL,
                    APP_BRIDGE_MANAGER_PRIORITY, NULL) != pdPASS)
    {
        while (1)
            ;
    }
#endif /* EASYEVSE */

    Init_SigBrd_Uart();
    s_init = true;
}

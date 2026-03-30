/*
 * Copyright 2022-2024, 2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef MESSAGE_TRANSPORT_UART_H
#define MESSAGE_TRANSPORT_UART_H

#if defined(__cplusplus)
extern "C" {
#endif

#include "fsl_common.h"
#include "board.h"
/*!
 * @addtogroup comm_port_driver
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* CommPortDriver specific macros */

/* Host controller port specific */
#define HOST_UART_CONTROL_INIT       LPUART_Init
#define HOST_UART_BASE_PTR_CONTROL   LPUART4
#define HOST_UART_CONTROL_GETCHAR    LPUART_ReadByte(LPUART4)
#define HOST_UART_CONTROL_PUTCHAR(c) LPUART_WriteByte(LPUART4, c)
#define HOST_UART_CONTROL_CLOCK      BOARD_DEBUG_UART_CLK_FREQ
#define HOST_UART_INTERRUPT_PRIORITY 0

#define EOL         0x00
#define zUARTBUFFER 128

#define STX        0x02
#define ETX        0x03
#define EOT        0x04
#define ACK        0x06
#define END_OF_MSG '\r'

/*! @brief CommPort_Driver Indexes */
typedef enum
{
    UART_CONTROL_INDEX,
    NUM_UART_CHANNEL
} UART_INDEX;

enum
{
    UART_TX,
    UART_RX,
};

/*! @brief CommPort_Driver Modes*/
typedef enum
{
    UART_IDLE,
    UART_RXING,
    UART_PROCESSING,
    UART_TXING
} UART_STATUS;

/*! @name CommPort_Driver Configuration */
/*@{*/

/*!
 * @brief Initializes the UART ports for host communication.
 */
void Uart_ModuleInit(void);

void UART_Transmit(UART_INDEX uart_index, uint8_t *txBuff, uint16_t txLength);

/*!
 * @brief Communication process/task.
 */
void Comm_Process(void);

/*@}*/

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /*MESSAGE_TRANSPORT_UART_H*/

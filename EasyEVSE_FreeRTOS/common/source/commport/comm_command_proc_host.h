/*
 * Copyright 2025-2026 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef COMMUNICATION_API_H
#define COMMUNICATION_API_H

#include "board.h"
#include <stdint.h>
#include "types.pb.h"
#include "message_handler.h"

#include "EVSE_Metering.h"

#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_SIGBOARD_HW_VERSION 0xFFFFFFFF
#define INVALID_SIGBOARD_SW_VERSION 0xFFFFFFFF

#define CP_READ_TIMEOUT 20U

typedef enum _req_flags
{
    CP_STATE_REQ_MASK    =   1 << 0,
    PP_STATE_REQ_MASK    =   1 << 1,
    EV_PLUGGED_REQ_MASK  =   1 << 2,
    EV_REMOVED_REQ_MASK  =   1 << 3,
    EV_PWR_REQ_MASK      =   1 << 4,
}req_flags_t;

typedef enum _lpuart_state
{
    LPUART_STATE_IDLE = 0,
    LPUART_STATE_SENDING,
    LPUART_STATE_WAITING_RESPONSE,
    LPUART_STATE_MESSAGE_SEND,
} lpuart_state_t;

typedef enum _uart_comm_events
{
    EVSE_UART_MESSAGE_RECEIVED = 1 << 1,
    EVSE_UART_MESSAGE_SENT     = 1 << 2,
    EVSE_UART_ALL_EVENTS       = 0xFFFFFF,
} uart_comm_events_t;

typedef enum _emeter_comm_events
{
    EVSE_EMETER_RESPONSE_RECEIVED = 1 << 1,
    EVSE_EMETER_ALL_EVENTS        = 0xFFFFFF,
} emeter_comm_events_t;

/**
 * @brief Initializes the communication module.
 *
 * This sets up the message handler callbacks and initializes the UART module.
 */
void Host_Communication_Init(void);

void EVSE_UARTComm_SetEvent(uart_comm_events_t event);

void EVSE_EMETERComm_SetEvent(emeter_comm_events_t event);

/**
 * @brief Sends an asynchronous host command to the e-meter.
 *
 * This function builds and sends a message based on the specified command ID.
 *
 * @param cmd_id The command to send (e.g., COMMAND_ID_GET_METROLOGY_ALL).
 * @return MessageStatus indicating success or failure of the operation.
 */
MessageStatus Host_AsyncSend(command_id_t cmd_id);

bool SIGBRD_PushNotificationReceived(uint8_t mask, uint16_t *value);

void SIGBRD_GetSWVersion(uint32_t *sw_version_major, uint32_t *sw_version_minor, uint32_t *sw_version_hotfix);

void SIGBRD_GetHWVersion(uint32_t *hw_version);

void SIGBRD_SetPWMDutyInPercent(uint16_t dutyCycle);
void SIGBRD_SetPWMDutyInMilli(uint16_t dutyCycle);

void SIGBRD_GetPWMDutyInPercent(uint16_t *dutyVal);
void SIGBRD_GetPWMDutyInMilli(uint16_t *dutyVal);

void SIGBRD_GetPPState(uint16_t *pp_state);
void SIGBRD_GetCPState(uint16_t *cp_state);
void SIGBRD_GetGFCIState(uint16_t *gfci_state);

void SIGBRD_CloseRelay(void);
void SIGBRD_OpenRelay(void);

void SIGBRD_GetADCVal(uint32_t *adcValPtr); // this is a value for a 16 bit ADC

void SIGBRD_GetMeterCurrent(double *currentPh1, double *currentPh2, double *currentPh3);
void SIGBRD_GetMeterVoltage(double *voltagePh1, double *voltagePh2, double *voltagePh3);
void SIGBRD_GetMeterActivePower(double *PPh1, double *PPh2, double *PPh3);
void SIGBRD_GetMeterReactivePower(double *QPh1, double *QPh2, double *QPh3);
void SIGBRD_GetMeterApparentPower(double *SPh1, double *SPh2, double *SPh3);
void SIGBRD_GetMetrologyAll(meter_data_t *meter_data);

void SIGBRD_GetRelayState(uint8_t *relay_state);

void SIGBRD_SetChargingProtocol_TickDelayMs();

void SIGBRD_UART_BridgeEntry(void);

#ifdef __cplusplus
}
#endif

#endif // COMMUNICATION_API_H

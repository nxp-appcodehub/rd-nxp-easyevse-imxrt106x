/*
 * Copyright 2023-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _STX_HAL_UART_BRIDGE_H_
#define _STX_HAL_UART_BRIDGE_H_

#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INVALID_SIGBOARD_HW_VERSION 0xFFFFFFFF
#define SIGBRD_RING_BUFFER_SIZE     64
#define SIGBRD_RING_BUFFER_NUMBERS  4

#define CP_READ_TIMEOUT 20U

/*#define HAL_UART_BRIDGE_DEBUG   1

#ifndef HAL_UART_BRIDGE_DEBUG
#define HAL_UART_BRIDGE_DEBUG   0
#endif*/

/* Control debug out */
/*#if ((HAL_UART_BRIDGE_DEBUG == 1) && (DEBUG_STX == 1))
    #include "debug_stx.h"
    #define HALUARTBRIDGEPRINT(a)                   DBGPRINTLN ("HALUARTBRIDGE",a)
    #define HALUARTBRIDGEPRINT1(a,b)                DBGPRINTLN1("HALUARTBRIDGE",a,b)
#else
    #define HALUARTBRIDGEPRINT(a)
    #define HALUARTBRIDGEPRINT1(a,b)
#endif*/
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* GPIOPin can be set in two states */
typedef enum
{
    CMD_ERR            = -1,
    ALL_STATES         = 'a',
    PP_STATE           = 'b',
    CP_STATE           = 'c',
    GFCI_STATE         = 'd',
    ADC_CP_VALUE       = 'e',
    ADC_PP_VALUE       = 'f',
    GET_PWM_DUTY       = 'g',
    CP_RESISTOR_VALUE  = 'h',
    PWM_DUTY_PER_MILLI = 'i',
    CLOSE_RELAY        = 'j',
    OPEN_RELAY         = 'k',
    SET_CP             = 's',
    VERSION            = 'v',
    HWVERSION          = 'w',
    METER_ALL          = '0',
    METER_CURRENT      = '1',
    METER_VOLTAGE      = '2',
    METER_POWER        = '3',
    METER_STATE        = '4',
    METER_Q            = '5',
    METER_S            = '6',
    METER_LAST         = '6',
    CMD_UNKNOWN        = 'n',
} COMMAND_UART_CODE;

typedef enum
{
    kCPStateOff,
    kCPStateOn,
    kCPStateLast = 0xFFFFFFFF
} kCPResistorState;

typedef enum
{
    kCPEVResistor_270R,
    kCPEVResistor_1K3,
    kCPEVResistor_Last = 0xFFFFFFFF
} kCPResistor;

/*! @brief List of Control Pilot states */
typedef enum
{
    kCPStateA, /*!< Pilot Control line voltage at +12V. */
    kCPStateB, /*!< Pilot Control line voltage at +9V. */
    kCPStateC, /*!< Pilot Control line voltage at +6V. */
    kCPStateD, /*!< Pilot Control line voltage at +3V. */
    kCPStateE, /*!< Pilot Control line voltage error. */
    kCPStateF, /*!< Pilot Control line voltage indicate EVSE is offline. */
} kCPState;

typedef enum
{
    kPPNotDetected,
    kPP150RDetected,
    kPP480RDetected,
    kPPDetectedMaxVal,
} kPPState;

struct sigbrd_state_t
{
    kPPState PPState;
    kCPState CPState;
    uint32_t ADCCPValue;
    uint32_t ADCPPValue;
    uint32_t PWMDutyCycle;
    uint16_t CPResistor;
    uint32_t PWMDutyPerMilli;
    uint16_t GFCIState;
    char version[SIGBRD_RING_BUFFER_SIZE];
    float current;
    float voltage;
    float power;
};

void SIGBRD_GetSWVersion(uint32_t *sw_version_major, uint32_t *sw_version_minor, uint32_t *sw_version_hotfix);

void SIGBRD_GetGetHWVersion(uint32_t *hw_version);

void SIGBRD_SetPWMDutyInPercent(uint16_t dutyCycle);
void SIGBRD_SetPWMDutyInPerMilli(uint16_t dutyCycle);

void SIGBRD_GetPWMDutyInPercent(uint16_t *dutyVal);
void SIGBRD_GetPWMDutyInPerMilli(uint16_t *dutyVal);

void SIGBRD_SetCPResistorValue(kCPResistor gpioNumber, kCPResistorState gpioState);
void SIGBRD_GetCPResistorValue(kCPResistor gpioNumber, kCPResistorState *gpioState);

void SIGBRD_GetPPState(uint16_t *pp_state);
void SIGBRD_GetCPState(uint16_t *cp_state);
void SIGBRD_GetGFCIState(uint16_t *gfci_state);

void SIGBRD_CloseRelay(void);
void SIGBRD_OpenRelay(void);

void SIGBRD_GetADCVal(uint32_t *adcValPtr, uint32_t ulSize);

void SIGBRD_GetMeterCurrent(double *current);
void SIGBRD_GetMeterVoltage(double *voltage);
void SIGBRD_GetMeterPower(double *power);

int SIGBRD_EVSE_UARTCommsProcess(char command_code, uint16_t value_transmitted, uint32_t *value_returned);
int SIGBRD_EV_UARTCommsProcess(char command_code, uint16_t value_transmitted, uint32_t *value_returned);

void SIGBRD_SetChargingProtocol_TickDelayMs();

void SIGBRD_UART_BridgeEntry(void);
#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* _STX_HAL_UART_BRIDGE_H_ */

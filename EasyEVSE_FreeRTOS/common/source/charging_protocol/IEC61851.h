/*
 * Copyright 2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef J1772_IEC61851_H_
#define J1772_IEC61851_H_

#include "stdint.h"
#include <stdbool.h>
#include "EVSE_ChargingProtocol.h"

/* The ADC measures from 0-3.3V 16bits */
/*
    12V => 3.1V => 61563
    9V => 2.794V => 55486
    6V => 2.488V => 49409
    3V => 2.181V => 43312
    0V => 1.874V => 32215
    -12V => 0.65V => 12908
*/
#define BATTERY_CAPACITY       1000
#define BATTERY_STARTING_LEVEL 20
#define BATTERY_DESIRED_LEVEL  100

#define BC_MEASURE_INTERVAL_MS 100 /* MS */
#define PWM100_MIN_VALUE       980
#define PWM100_MAX_VALUE       1000

#define PWM5_MIN_VALUE                45
#define PWM5_MAX_VALUE                55
#define PWM5_VALUE                    50

#define STATEB2_TRANSITION_B1_MIN_TIME  3000
#define STATEA_MIN_LEVEL                58000
#define STATEA_MAX_LEVEL                62000
#define STATEB_MIN_LEVEL                52000
#define STATEB_MAX_LEVEL                54000
#define STATEC_MIN_LEVEL                46000
#define STATEC_MAX_LEVEL                49000
#define STATED_MIN_LEVEL                42000
#define STATED_MAX_LEVEL                44000

#define STATEE_MIN_LEVEL                32000
#define STATEE_MAX_LEVEL                33000

#define STATEF_MIN_LEVEL                12000   /* -12V state value */
#define STATEF_MAX_LEVEL                14000   /* -12V low state value */

#define MIN_CURRENT                     (6u)
#define MAX_CURRENT                     (81u)

/* if error state either stay in state F or try to see if an EV is connected */
#define ERROR_STATE_DELAY               (300u)

/* Fastest response time is 100 ms. Set the delay to 10u */
#define J1772_TICK_DELAY                (10u)

/* The allowed adjustment time before changing the amps again */
#define T10T9DELAY                      (5000u)

typedef enum _j1772_state{
    STATE_A = 0,
    STATE_A2,
    STATE_B,
    STATE_B2,
    STATE_C,
    STATE_C2,
    STATE_D,
    STATE_D2,
    STATE_E,
    STATE_F,
    J1772_LAST_STATE,
} j1772_state_t;

typedef enum _j1772_status{

    J1772_Succes,
    J1772_Error,
    J1772_CP_Req_Digital_mode,
} j1772_status_t;

typedef enum _proximity_state {
    PP_NotConnected,
    PP_ButtonPressed,
    PP_Connected
} proximity_state_t;


/**
 * @brief Get the Amps from a known duty cycle
 *
 * @param dutyCycle value of the duty cycle
 * @param amps pointer where to store the amps
 * @return j1772_status_t J1772_Error if wrong duty cycle provided
 */
j1772_status_t EVSE_J1772_GetAmpsFromDutyCycle(uint8_t dutyCycle, uint8_t *amps);

/**
 * @brief Get the duty cycle from a known charge rate
 *
 * @param amps the wanted amps
 * @param dutyCycle the duty cycle that matches that provided amps
 * @return j1772_status_t
 */
j1772_status_t EVSE_J1772_GetDutyCycleFromAmps(uint8_t amps, uint8_t *dutyCycle);

/**
 * @brief Set the CP pwm based on the state provided and amps
 *
 * @param j1772_state j1772 state
 * @param amps amps
 */
j1772_status_t EVSE_J1772_SetCPFromState(j1772_state_t j1772_state, uint16_t amps);

/**
 * @brief Get the CP state from a CP value. The cp values are based on the ADC used.
 * Adjust the STATEX_MIN_LEVEL and STATEX_MAX_LEVEL before
 *
 * @param cp_value
 * @return j1772_state_t
 */
j1772_state_t EVSE_J1772_GetStateFromCPValue(uint32_t cp_value);

/**
 * @brief Get the state.
 *
 * @return j1772_state_t
 */
j1772_state_t EVSE_J1772_GetState();

/**
 * @brief Get a printable version of a J1772 state
 *
 * @param state
 * @return const char*
 */
const char* EVSE_J1772_GetStateString(j1772_state_t j1772_state);

/* EV*/
proximity_state_t EVSE_J1772_GetPPStatefromPPValue(uint16_t pp_value);
proximity_state_t EVSE_J1772_GetPPState();

/**
 * @brief 
 * 
 * @param pwm 
 * @return j1772_status_t 
 */
j1772_status_t EVSE_J1772_SetCPFromPWM(uint8_t pwm);

/**
 * @brief Set CP Resistor according to the EV state
 *
 * @param eEvState
 * @return j1772_status_t
 */
j1772_status_t EVSE_J1772_SetCPResistor (j1772_state_t eEvState);

/**
 * @brief Get the CP State in a Char Version 
 * 
 * @return char 'A','B','C','D','E','F'
 */
char EVSE_J1772_GetCpStateString();

/**
 * @brief Get the CP value.
 * 
 * @param cp_value 
 * @return j1772_status_t 
 */
j1772_status_t EVSE_J1772_GetCPValue(uint32_t *cp_value);

/**
 * @brief Disable power delivery
 * 
 * @return j1772_status_t 
 */
j1772_status_t EVSE_J1772_DisablePower();

/**
 * @brief Enable power delivery
 * 
 * @return j1772_status_t 
 */
j1772_status_t EVSE_J1772_EnablePower();

/**
 * @brief Set the max current of the EVSE or EV
 * 
 * @param max_current 
 */
void EVSE_J1772_SetMaxCurrent(uint32_t max_current);

/**
 * @brief Get current charging state of the system
 * 
 * @param bCharging 
 * @return true if the car is charging
 * @return false if the car is not charging
 */
void EVSE_J1772_isCharging(bool *bCharging);

/**
 * @brief Stop the charging session from outside the stack
 *
 * @param bCharging true if the charge should be stopped
 */
void EVSE_ISO15118_StopCharging(bool bCharging);

/**
 * @brief Get vehicle data as described in the charging protocol header.
 * if the values are not valid for J1772 they will be left untouched
 * 
 * @param vehicle_data 
 */
void  EVSE_J1772_GetVehicleData(vehicle_data_t *vehicle_data);

/**
 * @brief Init the J1772 stack
 * 
 */
void EVSE_J1772_Init(void);

/**
 * @brief 
 * 
 * @param stopCharging 
 */
void EVSE_J1772_Loop(bool *stopCharging);
#endif /* J1772_IEC61851_H_ */

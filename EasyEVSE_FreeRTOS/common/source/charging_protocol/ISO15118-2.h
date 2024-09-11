/*
 * Copyright 2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef CHARGING_PROTOCOL_ISO15118_2_H_
#define CHARGING_PROTOCOL_ISO15118_2_H_

#include "stdint.h"
#include <stdbool.h>

#include "EVSE_ChargingProtocol.h"
typedef enum _evse_iso15118_state
{
    EVSE_ISO15118_NotEnable,
	EVSE_ISO15118_InitStack,
	EVSE_ISO15118_FWDownload,
	EVSE_ISO15118_WaitConnect,
	EVSE_ISO15118_SLACStart,
	EVSE_ISO15118_SLACSucces,
	EVSE_ISO15118_SLACFailed,
	EVSE_ISO15118_AuthWait,
	EVSE_ISO15118_AuthFailed,
	EVSE_ISO15118_V2GRunnig,
	EVSE_ISO15118_LAST
}evse_iso15118_state_t;

typedef enum _vehicle_auth_methods
{
   VehicleAuth_EIM,
   VehicleAuth_PnC,
   LAST_AUTH_METHOD
} vehicle_auth_methods_t;

typedef enum _V2G_status
{
   NOT_CONNECTED,
   DEMO,
   LAST_V2G_STATUS
} V2G_status_t;

/**
 * @brief Get a string version from a status 
 * 
 * @param state the state from which to get the string
 * @return const char* return NULL if wrong state
 */
const char* EVSE_ISO15118_GetStringFromState(evse_iso15118_state_t state);

/**
 * @brief Set the state 
 * 
 * @param state new value of the state
 */
void EVSE_ISO15118_SetState(evse_iso15118_state_t state);
/**
 * @brief Get the state
 * 
 * @return evse_iso15118_state_t return the state of ISO15118 stack
 */
evse_iso15118_state_t EVSE_ISO15118_GetState();

/**
 * @brief Init the ISO15118 stack
 * 
 */
void EVSE_ISO15118_Init();

/**
 * @brief The loop operations that need to be executed periodically
 * 
 * @param stopCharging 
 */
void EVSE_ISO15118_Loop(bool *stopCharging);

/**
 * @brief Set Vehicle AUTH mode
 * 
 * @param _auth_method 
 */
void EVSE_ISO15118_SetVehicleAuthMethod(vehicle_auth_methods_t _auth_method);

/**
 * @brief Get Vehicle Auth Mode EIM - external identification method or PNC plug and charge
 * 
 * @return vehicle_auth_methods_t 
 */
vehicle_auth_methods_t EVSE_ISO15118_GetVehicleAuthMethod(void);
/**
 * @brief Set the power current counter.
 * 
 * @param cntr 
 */
void EVSE_ISO15118_SetPowerReqCntr(uint32_t cntr);

/**
 * @brief Get the current Power Request counter
 * 
 * @return uint32_t 
 */
uint32_t EVSE_ISO15118_GetPowerRequestsCntr(void);
/**
 * @brief Increment current power req counter
 * 
 */
void EVSE_ISO15118_IncrementPowerReqCntr(void);

/**
 * @brief Set V2G demo status
 * 
 * @param status 
 */
void EVSE_ISO15118_SetV2GStatus(V2G_status_t status);

/**
 * @brief Get V2G demo status
 * 
 * @return V2G_status_t 
 */
V2G_status_t EVSE_ISO15118_GetV2GStatus();


/**
 * @brief Set the max current for both the EV and EVSE
 * 
 * @param max_current 
 */
void EVSE_ISO15118_SetMaxCurrent(uint32_t max_current);

/**
 * @brief Get the vehicle data as describe in the Charging Protocol structure.
 * if some attributes are not supported by the stack the value will not be updated.
 * 
 * @param vehicle_data 
 */
void  EVSE_ISO15118_GetVehicleData(vehicle_data_t *vehicle_data);
/**
 * @brief Get current charging state
 * 
 * @param bCharging 
 * @return true if the car is charging
 * @return false if the car is not charging
 */
void EVSE_ISO15118_isCharging(bool *bCharging);

/**
 * @brief Get the CP State in a Char Version
 *
 * @return char 'A','B','C','D','E','F'
 */
char EVSE_ISO15118_GetCpStateString();

/**
 * @brief Stop the charging session from outside the stack
 * 
 * @param bCharging true if the charge should be stopped
 */
void EVSE_ISO15118_StopCharging(bool bCharging);

/**
 * @brief Init the ISo15118 stack
 * 
 */
void EVSE_ISO15118_Init(void);

/**
 * @brief Main part of the stack.
 * 
 * @param stopCharging 
 */
void EVSE_ISO15118_Loop(bool *stopCharging);

#endif /* CHARGING_PROTOCOL_ISO15118_2_H_ */

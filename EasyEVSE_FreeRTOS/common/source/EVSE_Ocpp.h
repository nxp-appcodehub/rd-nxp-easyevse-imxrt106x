/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/**
 * @brief Defines an interface to be used when interacting with ocpp.
 */

#ifndef EVSE_OCPP_H_
#define EVSE_OCPP_H_

#include <stdbool.h>
#include <stdint.h>
#include "EVSE_ConnectivityConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef enum _ocpp_events
{
	EV_CONNECTED_EVENT                       = 1 << 1,
	EV_AUTH_NEEDED_EVENT                     = 1 << 2,
	EV_AUTHORIZED_EVENT                      = 1 << 3,
	EVSE_CHARGING_STARTED_EVENT              = 1 << 4,
	EVSE_CHARGING_STOPPED_EVENT              = 1 << 5,
	EVSE_TELEMETRY_READY_EVENT               = 1 << 6,
	EV_DISCONNECTED_EVENT                    = 1 << 7,
	EVSE_CHARGING_START_REMOTE_EVENT         = 1 << 8,
	EVSE_CHARGING_STOP_REMOTE_EVENT          = 1 << 9,
	EVSE_NETWORK_DOWN_EVENT                  = 1 << 10,
	EVSE_RECONNECT_EVENT                     = 1 << 11,
	EVSE_OCPP_ALL_EVENTS                     = 0xFFFFFF,
} ocpp_events_t;

typedef enum _ocppConnectionState
{
    EVSE_Ocpp_NotConnected = 0,
    EVSE_Ocpp_Connected,
	EVSE_Ocpp_Error,
    EVSE_Ocpp_Last
} ocppConnectionState_t;

typedef enum _ocppMeterValueIdx
{
    ENERGY_WH_IMPORT_TOTAL = 0, // Imported energy (from grid)
	ENERGY_WH_I_L1,
	ENERGY_WH_I_L2,
	ENERGY_WH_I_L3,
	ENERGY_WH_EXPORT_TOTAL,    // Exported energy (to grid)
	ENERGY_WH_E_L1,
	ENERGY_WH_E_L2,
	ENERGY_WH_E_L3,
	POWER_W,                   // Instantaneous power
	VOLTAGE_V,                 // Voltage
	VAR,                       // Reactive power
	CURRENT_A,                 // Current
	FREQUENCY_HZ,              // Grid frequency
    OCPP_METER_VAUES_IDX_LAST
} ocppMeterValueIdx_t;

/**
 * @brief Convert the OCPP connection state into a readable string
 * @param ocppState OCPP connection state for which to get the string
 * @return return a constant string
 */
const char *EVSE_OCPP_GetStringFromState(ocppConnectionState_t ocppState);

/**
 * @brief Get the current OCPP connection state
 * @return the current OCPP connection state
 */
ocppConnectionState_t EVSE_OCPP_GetConnectionState(void);

/**
 *  @brief Set an event for the OCPP task
 */
void EVSE_OCPP_SetEvent(ocpp_events_t event);

/**
 * @brief Creates the OCPP connectivity Task, initializes internal components.
 */
void EVSE_OCPP_Init(void);

/**
 * @brief Get the number of telemetry packets sent to the OCPP server.
 */
const uint32_t EVSE_OCPP_GetTelemetryCount(void);

/**
 * @brief Set current metering data
 * @param values Pointer to the values buffer
 * @param phase_rot_error true for 3 phase rotation error (ccw)
 */
void EVSE_OCPP_SetCurrentPowerData(const float* values, bool phase_rot_error);

/*
 * @brief Get the authorization response from the ocpp server
 * @param timeout_ms Timeout in ms to wait for server response, 0 in non-blocking mode
 * @param auth_response The actual authorization response from the ocpp server
 * @returns true if the operation finished, false otherwise
 */
bool EVSE_OCPP_GetAuthResponse(uint32_t timeout_ms, bool* auth_response);

/*
 * @brief Get the message to display from the ocpp server
 */
const char* EVSE_OCPP_GetDisplayMessageString(void);

#ifdef __cplusplus
}
#endif

#endif /* EVSE_OCPP_H_ */

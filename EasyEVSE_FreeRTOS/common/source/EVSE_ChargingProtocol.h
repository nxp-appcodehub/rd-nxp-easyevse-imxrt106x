/*
 * Copyright 2024-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_ISO15118_H_
#define EVSE_ISO15118_H_

#include "EVSE_NFC.h"
#include "EVSE_Utils.h"

#define CHARGINGPROTOCOL_NOTICK_DELAY     (1U)
#define CHARGINGPROTOCOL_SHORTTICK_DELAY  (2U)
#define CHARGINGPROTOCOL_LONGTICK_DELAY   (100U)
#define CHARGINGPROTOCOL_MEDIUMTICK_DELAY (50U)
#define CHARGINGPROTOCOL_TICK_DELAY       (10U)
#define CHARGINGPROTOCOL_MAX_DELAY        (6000U)

#define MAX_PROTOCOL_LENGTH (20U)

typedef enum _charging_priority
{
    EVSE_Charging_EVSEMaxPriority,
    EVSE_Charging_EVSENormalPriority
} charging_priority_t;

typedef enum _charging_events
{
    EVSE_Charging_EVSERefreshData     = 1 << 1,
    EVSE_Charging_EVSEStopCharging    = 1 << 2,
    EVSE_Charging_EVSESuspendCharging = 1 << 3,
    /* Internal use only */
    EVSE_Charging_ALL_EVENTS = 0xFFFFFF,
} charging_events_t;

typedef enum _charging_directions
{
    EVSE_G2V,
    EVSE_V2G,
    EVSE_NoChargingDirection,
} charging_directions_t;

typedef enum _evse_charging_states
{
    EVSE_ChargingStarted,
    EVSE_ChargingNone,
    EVSE_ChargingResumed,
    EVSE_ChargingPaused,
} charging_states_t;

typedef enum _evse_charging_protocol
{
    EVSE_BasicCharging_J1772,
    EVSE_HighLevelCharging_ISO15118,
    EVSE_HighLevelCharging_ISO15118_20,
    EVSE_LastChargingProtocol,
} evse_charging_protocol_t;

typedef enum _evse_auth_methods
{
    VehicleAuth_EIM,
    VehicleAuth_PnC,
    VehicleAuth_None,
    LAST_AUTH_METHOD
} evse_auth_methods_t;

typedef enum _battery_levels
{
    EMPTY_BATTERY,
    FULL_BATTERY,
}battery_levels_t;

typedef struct _vehicle_data
{
    char vehicleID[CARD_MAX_SIZE_UID];
    char protocol[MAX_PROTOCOL_LENGTH];
    uint32_t batteryLevel;        /* Current vehicle battery level as % */
    uint32_t chargedBatteryUnits; /* Battery units increased during EV charging*/
    uint32_t batteryCapacity;     /* Vehicle battery capacity read from tag in kWh */
    uint32_t elapsedTime;         /* Time elapse in seconds since start charging */
    uint32_t requestedEnergy;     /* Energy requested by the EV */
    uint32_t deliveredEnergy;     /* Energy delivered by the EVSE from EV connection until EV disconnection */
    uint32_t deliveredEnergyHLC;  /* Energy delivered by the EVSE during high-level charging */
    uint8_t energyDeliveryStatus; /* How much of the requested energy is actually delivered */
    uint32_t timeToCharge;        /* Time remaining in seconds until battery is fully charged */
    float chargeCurrent;          /* EV charging current */
#if EASYEVSE_EV
    float chargeVoltage;   /* EV charging voltage */
    float chargePower;     /* EV charging power */
    float fBatteryLevel;   /* EV float battery level (for a user friendlier UI) */
#endif /* EASYEVSE_EV */
    bool vehicleAuth;             /* Vehicle authentication status */
    evse_charging_protocol_t charging_protocol;

} vehicle_data_t;

/**
 * @brief Function to init the ISO15118 stack.
 *
 */
void EVSE_ChargingProtocol_Init(void);

/**
 * @brief Get a string of the charging protocol present in the charging parameter
 *
 * @param charging_protocol the protocol for which there is a charging request
 * @return const char*
 */
const char *EVSE_ChargingProtocol_GetStringFromProtocol(evse_charging_protocol_t charging_protocol);

/**
 * @brief Start charging from EVSE side
 */
void EVSE_ChargingProtocol_StartCharging(void);

/**
 * @brief Stop charging from EVSE side
 */
void EVSE_ChargingProtocol_StopCharging(void);

/**
 * @brief Set the priority of the charging task. Use it with caution
 * EVSE_Charging_EVSEMaxPriority can starve other tasks
 *
 * @param priority_mode
 */
void EVSE_ChargingProtocol_SetTaskPriority(charging_priority_t priority_mode);

/**
 * @brief Set event in the Charging task
 *
 * @param event
 */
void EVSE_ChargingProtocol_SetEvent(charging_events_t event);

/**
 * @brief Set the delay between loops in the charging protocol
 * @param new_stack_delay must be between 1 and CHARGINGPROTOCOL_LONGTICK_DELAY
 */
void EVSE_ChargingProtocol_SetTaskDelay(uint16_t new_stack_delay);

/**
 * @brief Get a string of the current charging protocol used
 *
 * @return const char*
 */
const char *EVSE_ChargingProtocol_GetStringFromCurrentProtocol(void);

/**
 * @brief Set the current charging protocol
 *
 * @param charging_protocol new charging protocol
 */
void EVSE_ChargingProtocol_SetProtocol(evse_charging_protocol_t charging_protocol);

/**
 * @brief Get current charging protocol
 *
 * @return evse_charging_protocol_t
 */
evse_charging_protocol_t EVSE_ChargingProtocol_GetProtocol(void);

/**
 * @brief Get current charging protocol string. If it is high level charging, string will contain auth method
 *
 * @return evse charging string
 */
const char *EVSE_ChargingProtocol_GetProtocolString(void);

/**
 * @brief Get if the car is charging
 *
 * @return true
 * @return false
 */
bool EVSE_ChargingProtocol_isCharging(void);

/**
 * @brief Get charging direction
 *
 * @return charging direction (Grid to EV/ EV to Grid/ None)
 */
charging_directions_t EVSE_ChargingProtocol_ChargingDirection();

/**
 * @brief Get charging direction
 *
 * @param charging direction
 * @return charging direction "Grid to Vehicle" "Vehicle to Grid"  "No charging direction";
 */
const char *EVSE_ChargingProtocol_GetStringFromDirection(charging_directions_t charging_direction);

/**
 * @brief Returns current charging state
 *
 * @return charging_states_t current charging state
 */
charging_states_t EVSE_ChargingProtocol_GetChargingState();

/**
 * @brief Sets current charging state
 *
 * param charging_states_t charging state to be set
 */
void EVSE_ChargingProtocol_SetChargingState(charging_states_t charging_state);

/**
 * @brief Get the current charging state
 *
 * @return YES if it is charging
 * @return NO if it is not charging
 */
const char *EVSE_ChargingProtocol_isChargingString(void);

/**
 * @brief Get Vehicle data
 *
 * @return const vehicle_data_t*
 */
const vehicle_data_t *EVSE_ChargingProtocol_GetVehicleData(void);

/**
 * @brief Set the current max current for an EV or EVSE
 *
 * @param max_current max current. No limit
 */
void EVSE_ChargingProtocol_SetMaxCurrent(uint32_t max_current);

/**
 * @brief Get the CP State in a Char Version
 *
 * @return char 'A','B','C','D','E','F'
 */
char *EVSE_ChargingProtocol_GetCPStateString();

/**
 * @brief Set NFC authentication from EVSE side
 */
void EVSE_ChargingProtocol_SetNFCAuthentication(uint8_t *cardUID, uint8_t size);

/**
 * @brief Set payment method for vehicle authentication
 *
 * @param method The authentication method to be set
 */
void EVSE_ChargingProtocol_SetPaymentMethod(evse_auth_methods_t method);

#if EASYEVSE_EV
/**
 * @brief Get Vehicle data
 *
 * @return const vehicle_data_t*
 */
const vehicle_data_t *EV_ChargingProtocol_GetVehicleData(void);

/**
 * @brief Get if the car is charging
 *
 * @return true
 * @return false
 */
bool EV_ChargingProtocol_isCharging(void);

/**
 * @brief Change energy direction (V2G or G2V)
 *
 */
void EV_ChargingProtocol_ChangeChargingDirection(void);

/**
 * @brief Reset battery level (to full or empty)
 *
 */
evse_return_status_t EV_ChargingProtocol_ResetBatteryLevel(battery_levels_t battery_level);

/**
 * @brief Start charging from EV side
 */
evse_return_status_t EV_ChargingProtocol_StartCharging(void);

/**
 * @brief Stop charging from EV side
 */
evse_return_status_t EV_ChargingProtocol_StopCharging(void);

/**
 * @brief Set charging protocol from EV side
 */
evse_return_status_t EV_ChargingProtocol_SetProtocol(evse_charging_protocol_t protocol);

/**
 * @brief Get charging protocol on EV side
 */
evse_charging_protocol_t EV_ChargingProtocol_GetProtocol(void);

/**
 * @brief Ev set Auth method
 */
void EV_ChargingProtocol_SetAuthMethod(evse_auth_methods_t auth_method);

#endif /* EASYEVSE_EV */
#endif /* EVSE_ISO15118_H_ */

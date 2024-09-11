/*
 * Copyright 2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_ISO15118_H_
#define EVSE_ISO15118_H_

#include "EVSE_NFC.h"

#define CHARGINGPROTOCOL_SHORTTICK_DELAY  (2)
#define CHARGINGPROTOCOL_LONGTICK_DELAY   (100)
#define CHARGINGPROTOCOL_MEDIUMTICK_DELAY (50)
#define CHARGINGPROTOCOL_TICK_DELAY       (10)
#define CHARGINGPROTOCOL_MAX_DELAY        (6000)

#define MAX_PROTOCOL_LENGTH               (20)

typedef enum _charging_events
{
    EVSE_Charging_EVSERefreshData         = 1 << 1,
    EVSE_Charging_EVSEStopCharging        = 1 << 2,
    /* Internal use only */
    EVSE_Charging_ALL_EVENTS              = 0xFFFFFF,
} charging_events_t;

typedef enum _evse_charging_protocol
{
    EVSE_BasicCharging_J1772,
    EVSE_HighLevelCharging_ISO15118,
    EVSE_LastChargingProtocol,
}evse_charging_protocol_t;

typedef struct _vehicle_data
{
    char vehicleID[CARD_MAX_SIZE_UID];
    char protocol[MAX_PROTOCOL_LENGTH];
    evse_charging_protocol_t charging_protocol;
    uint32_t batteryLevel;          /* Current vehicle battery level as % */
    uint32_t chargedBatteryUnits;   /* Battery units increased during EV charging*/
    uint32_t batteryCapacity;       /* Vehicle battery capacity read from tag in kWh */
    float chargeCurrent;
    uint32_t timeToCharge;          /* Time remaining in seconds until battery is fully charged */
    uint32_t elapsedTime;           /* Time elapse in seconds since start charging */
    uint32_t requestedEnergy;       /* Energy requested by the EV */
    uint32_t deliveredEnergy;       /* Energy delivered by the EVSE from EV connection until EV disconnection */
    uint32_t deliveredEnergyHLC;    /* Energy delivered by the EVSE during high-level charging */
    uint8_t energyDeliveryStatus;   /* How much of the requested energy is actually delivered */
    bool vehicleAuth;               /* Vehicle authentication status */
} vehicle_data_t;

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
 * @brief Function to init the ISO15118 stack.
 * 
 */
void EVSE_ChargingProtocol_Init(void);

/**
 * @brief Get a string of the current charging protocol used
 * 
 * @return const char* 
 */
const char* EVSE_ChargingProtocol_GetStringFromCurrentProtocol(void);

/**
 * @brief Get a string of the charging protocol present in the charging parameter
 * 
 * @param charging_protocol the protocol for which there is a charging request
 * @return const char* 
 */
const char* EVSE_ChargingProtocol_GetStringFromProtocol(evse_charging_protocol_t charging_protocol);

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
 * @brief Get if the car is charging
 * 
 * @return true 
 * @return false 
 */
bool EVSE_ChargingProtocol_isCharging(void);

/**
 * @brief Get the current charging state
 *
 * @return YES if it is charging
 * @return NO if it is not charging
 */
const char* EVSE_ChargingProtocol_isChargingString(void);

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

#endif /* EVSE_ISO15118_H_ */

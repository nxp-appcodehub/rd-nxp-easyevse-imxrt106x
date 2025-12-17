/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_METERING_H_
#define EVSE_METERING_H_

#include "EVSE_ChargingProtocol.h"

#define MAX_BATTERY_LEVEL    100
#define EVSE_ID_MAX_SIZE     10
#define METHOD_NAME_MAX_SIZE 24

#define MAX_IRMS 800
#define MAX_VRMS 600

typedef enum
{
    /* Set by the METER_LPUART_IRQHandler only when full packet received from Meter. */
    METER_FLAG = (1 << 0),
    /* Set by the METER_LPUART_IRQHandler in case of an RX overrun. */
    METER_OVERRUN_FLAG = (1 << 1),
    /* Start a read request */
    METER_READ = (1 << 2),
    /* Set to force to recalculate cost based on new findings */
    METER_REFRESH_DATA = (1 << 3),
} meter_events_t;

typedef enum _meter_data_fields
{
    METER_FIELD_IRMS = 0,
    METER_FIELD_VRMS,
    METER_FIELD_WH,
    METER_FIELD_STATE,
    METER_FIELD_Q,
    METER_FIELD_S,
} meter_data_fields_t;

/*! @brief Metering data structure. */
typedef struct _meter_data
{
    float irms; /* meter measured consumption current */
    float vrms; /* meter measured voltage */
    float wh;   /* meter measured power */

    double Q;   /* meter measured  Re-Active Power  */
    double S;   /* meter measured  Apparent Power   */

    uint8_t meterState;
    const char *EVSE_ChargeState;      /* Charge state moves from A to D for charging and E..F for errors */
    const char *EVSE_ChargingProtocol; /* BASIC or ISO15118 */
} meter_data_t;

/*! @brief Evse data structure. */
typedef struct evse_data_t
{
    uint32_t EVSE_Temperature; /* Current EVSE local temperature */
    uint32_t EVSE_GridPowerLimit;
    uint32_t EVSE_Rating;

    uint32_t EVSE_Uptime;
    float EVSE_ChargeCost; /* Charge cost */
    bool EVSE_IsCharging;  /* True if car is charging */
    charging_directions_t EVSE_ChargingDirection;
    uint8_t SIGBRD_HW_version;
    uint8_t SIGBRD_SW_version_major;
    uint8_t SIGBRD_SW_version_minor;
    uint8_t SIGBRD_SW_version_bugfix;
    char EVSE_Id[EVSE_ID_MAX_SIZE]; /* EVSE ID */
    /* Charging station specifics */
    double latitude;           /* Evse location latitude 51.50263*/
    double longitude;          /* Evse location longitude -0.15087*/
    double altitude;           /* Evse location altitude */
    uint32_t stationFirmwareV; /* RT1064 firmware version number */
} evse_data_t;

typedef enum
{
    /** Operation was successful */
    kMeterStatus_Success = 0x0,
    /** Operation failed */
    kMeterStatus_Fail = 0x1,
} meter_status_t;

/*
 * EVSE Metering entry point.
 * Handles communication with the metering board.
 */
void EVSE_Meter_Init(void);

/**
 * @brief Set an event in the meter task
 *
 * @param event the event to be set
 */
void EVSE_Set_MeterEvent(meter_events_t event);

/**
 * @brief  Checks if meter values are valid according to J1772 standard
 *
 * @param  None
 * @return True if values are valid, False otherwise
 */
const bool EVSE_Meter_CheckValidMeterData(void);

/*
 * @brief Retrieves meter data
 * @return pointer to meter information
 */
const meter_data_t *EVSE_Meter_GetMeterData(void);

/**
 * @brief Get the EVSE information
 * @return pointer to EVSE information
 */
const evse_data_t *EVSE_Meter_GetEVSEData(void);

/**
 * @brief Get the firmware version running on the EVSE
 *
 * @return EVSE firmware version in format MMmmFF, where M = FIRMWARE_VERSION_MAJOR, m = FIRMWARE_VERSION_MINOR, F =
 * FIRMWARE_VERSION_HOTFIX.
 */
uint32_t EVSE_Meter_GetFirmwareVersion(void);

/**
 * @brief Parse e reply from a Meter device to obtain the I R P Q S
 *
 * @param meter_reply buffer that contains raw data from the meter device
 * @param reply_size the size of the buffer
 * @param meter_data_t *parsed_meter_data
 * @param found_fileds filed which were found in the raw data stream
 */
void EVSE_Meter_ParseMeterReply(uint8_t *meter_reply,
                                uint32_t reply_size,
                                meter_data_t *parsed_meter_data,
                                uint32_t *found_fields);

/**
 * @brief Update the meter data from an external source. This will force parameter calculation and UI update
 *
 * @param new_meter_data pointer of the new meter data
 * @param fields_to_update fields data are valid in the new meter_data
 */
void EVSE_Meter_SetMeterData(const meter_data_t *new_meter_data, uint32_t fields_to_update);

#endif /* EVSE_METERING_H_ */

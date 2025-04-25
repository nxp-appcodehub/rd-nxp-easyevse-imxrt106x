/*

 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CUSTOM_EVSE_H_
#define __CUSTOM_EVSE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"
#include "EVSE_Metering.h"
#include "EVSE_Connectivity.h"
#include "EVSE_ChargingProtocol.h"
#include <ISO15118.h>
#include "EVSE_UI.h"
#include "EVSE_Utils.h"

#define NOT_CHARGING        0
#define MAX_EVSE_SW_VERSION 20

#define RED_LED   0xfc0702
#define GREEN_LED 0x40d440
#define GRAY_LED  0xdadada
#define BLUE_LED  0x0000e0

#define CHARGING_IMAGES_NUMBER 4

#define RESUME_TEXT "Resume"
#define PAUSE_TEXT  "Pause"

typedef enum _evse_screens
{
    Screen_Main,
    Screen_NetworkDebug,
    Screen_CarSimulation,
    Screen_EVSE,
    Screen_Meter,
    Screen_NFC,
    Screen_ISODebug,
    LAST_SCREEN,
} evse_screens_t;

/**
 * Get the screen active on the display
 */
evse_screens_t ui_getCurentScreen(void);

/**
 * Update the Vehicle Id in NFC screen
 * @param vehicleID string that points to a vehicle ID
 */
void UI_Update_NFC_VehicleID(const char *vehicleID);

/**
 * Set the telemetry counter in Debug screen
 * @param telemetryCnt telemetry counter
 */
void UI_Update_Debug_TelemetryCnt(uint32_t telemetryCnt);

/**
 * Set the Cloud Status prompt text inside Debug screen
 * @param CloudMessage text to be displayed
 */
void UI_Update_Debug_CloudMessage(const char *CloudMessage);

/**
 * Set the Network Status prompt text inside Debug screen
 * @param NetworkMessage text to be displayed
 */
void UI_Update_Debug_NetworkMessage(const char *NetworkMessage);

/**
 * Set all the values in Debug screen
 * @param NetworkMessage text to be displayed in the network status prompt
 * @param CloudMessage text to be displayed in the cloud status prompt
 * @param telemetryCnt telemetry counter
 */
void UI_Update_Debug_Values(const char *NetworkMessage, const char *CloudMessage, uint32_t telemetryCnt);

/**
 * Update the meter values in Meter screen
 *
 * @param meter structure that includes all the metered parameters
 */
void UI_Update_Meter_Values(const meter_data_t *meter);

/**
 * Update the widgets in Car screen
 * @param chargingMessage The charging prompt message
 * @param battery_level the battery level
 */
void UI_Update_Car_Values(const vehicle_data_t *vehicle_data);

/**
 * Update the widgets from the EVSE screen
 * @param evse_data Evse data
 */
void UI_Update_EVSE_Values(const evse_data_t *evse_data);

/**
 * Update the widgets in the Main screen
 * @param header value
 */
void UI_Update_Main_Values(const char *header);

/**
 * Update EVSE uptime (time passed since the application started)
 */
void UI_Update_EVSE_Uptime(void);

/**
 * Update data displayed on ISO15118 Debug screen
 */
void UI_Update_ISO15118_Debug_Values(const evse_iso15118_state_t status,
                                     const V2G_status_t V2G_status,
                                     const uint32_t Telemetry_cntr);

/**
 * Update Power Request Cntr displayed on ISO15118 Debug screen
 */
void UI_Update_ISO15118_Debug_PwrReqCntr(const uint32_t power_req_cntr);

/**
 * Update V2G Demo displayed on ISO15118 Debug screen
 */
void UI_Update_Debug_V2GStatus(const V2G_status_t status);

/**
 * Update ISO15118 Status displayed on ISO15118 Debug screen
 */
void UI_Update_Debug_ISO15118Status(const evse_iso15118_state_t status);

/**
 * Update digital clocks from every screen
 */
void UI_Update_Localtime(void);

/**
 * Update Vehicle ID displayed on Car screen
 */
void UI_Update_Car_VehicleID(const char *vehicleID);

/**
 * Update charging direction for Grid to Vehicle scenario
 */
void GridtoVehicle_animation();

/**
 * Update charging direction for Vehicle to Grid scenario
 */
void VehicletoGrid_animation(void);

/**
 * Clean the charging animation.
 */
void CleanCharging_animation(void);
#ifdef __cplusplus
}
#endif
#endif /* __CUSTOM_EVSE_H_ */

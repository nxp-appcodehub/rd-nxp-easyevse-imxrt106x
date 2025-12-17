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
    Screen_Linking,
    Screen_Charging,
    LAST_SCREEN,
} evse_screens_t;

/**
 * Get the screen active on the display
 */
evse_screens_t ui_getCurentScreen(void);

/**
 * Update Vehicle ID
 * @param vehicleID string that points to a vehicle ID
 */
void UI_Update_NFC_ID(const char *vehicleID);

/**
 * Update card type
 */
void UI_Update_NFC_Card_Type(const char *cardType);

/**
 * Update card security status
 */
void UI_Update_NFC_Card_Security(const char *cardSecurityStatus);

/**
 * Update all NFC data
 */
void UI_Update_NFC_Values(const char *vehicleID, const char *cardType, const char *cardSecurityStatus);

/**
 * Update metering data
 */
void UI_Update_Metering_Values(const meter_data_t *meter_data);

/**
 * Set the telemetry counter
 * @param telemetryCnt telemetry counter
 */
void UI_Update_Connectivity_Telemetry_Cntr(uint32_t telemetryCnt);

/**
 * Set the OCPP Status prompt text
 * @param OCPPMessage text to be displayed
 */
void UI_Update_Connectivity_OCPP_Status(const char *OCPPMessage);

/**
 * Update SSID header
 */
void UI_Update_Connectivity_SSID(const char *SSID);

/**
 * Update all connectivity data
 */
void UI_Update_Connectivity_Values(const char *OCPPMessage, uint32_t telemetryCnt, const char *SSID);

/**
 * Update vehicle data
 */
void UI_Update_Vehicle_Values(const vehicle_data_t *vehicle_data);

/**
 * Update EVSE data
 */
void UI_Update_EVSE_Values(const evse_data_t *evse_data);

/**
 * Update power request counter
 *  */
void UI_Update_PwrReqCntr(const uint32_t power_req_cntr);

/**
 * Update ISO15118 status
 */
void UI_Update_ISO15118Status(const evse_iso15118_state_t status);

/**
 * Update digital clocks from every screen
 */
void UI_Update_Localtime(void);

/**
 * Update NFC activation status icon
 */
void UI_Update_NFC_Activation_Status(void);

#ifdef __cplusplus
}
#endif
#endif /* __CUSTOM_EVSE_H_ */

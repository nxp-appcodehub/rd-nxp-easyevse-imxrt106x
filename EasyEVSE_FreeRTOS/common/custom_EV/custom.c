/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include "lvgl.h"
#include "custom.h"
#include "gui_guider.h"
#include "custom_ev.h"
#include "FreeRTOSConfig.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/
static const vehicle_data_t *sp_lastVehicleData    = NULL;

extern const char *iso15118AuthStateToString[LAST_AUTH_METHOD + 1];
/**
 * Create a demo application
 */

void custom_init(lv_ui *ui)
{
    /* Add your codes here */
}

void UI_Update_ChargingParameters(const vehicle_data_t *vehicle_data)
{
    if (guider_ui.Main_screen == NULL)
    {
        return;
    }

    if(vehicle_data == NULL)
    {
        return;
    }

    sp_lastVehicleData = vehicle_data;
    char data_string[MAX_PROTOCOL_LENGTH] = {0};
    const char *label_text = NULL;

    int32_t bar_value = lv_bar_get_value(guider_ui.Main_screen_battery_indicator);
    if((float)bar_value != sp_lastVehicleData->fBatteryLevel)
    {
        lv_bar_set_value(guider_ui.Main_screen_battery_indicator, sp_lastVehicleData->fBatteryLevel, LV_ANIM_OFF);
        memset(data_string, '\0', sizeof(data_string));
        snprintf(data_string, sizeof(data_string), "%.2f%%", sp_lastVehicleData->fBatteryLevel);
        lv_label_set_text(guider_ui.Main_screen_battery_value, data_string);
    }

    label_text = lv_label_get_text(guider_ui.Main_screen_protocol_value);
    memset(data_string, '\0', sizeof(data_string));
    snprintf(data_string, sizeof(data_string), "%s", EVSE_ChargingProtocol_GetStringFromProtocol(sp_lastVehicleData->charging_protocol));
    if(memcmp(data_string, label_text, strlen(data_string)) != 0)
    {
        lv_label_set_text(guider_ui.Main_screen_protocol_value, data_string);
        label_text = lv_label_get_text(guider_ui.Main_screen_auth_value);
        memset(data_string, '\0', sizeof(data_string));
#if ENABLE_ISO15118
        if ((sp_lastVehicleData->charging_protocol == EVSE_HighLevelCharging_ISO15118) ||
                (sp_lastVehicleData->charging_protocol == EVSE_HighLevelCharging_ISO15118_20))
        {
            snprintf(data_string, sizeof(data_string), "%s", iso15118AuthStateToString[EV_GetAuthMethod()]);
            if(memcmp(label_text, data_string, strlen(data_string)) != 0)
            {
                lv_label_set_text(guider_ui.Main_screen_auth_value, data_string);
            }
        }
        else
        {
            snprintf(data_string, sizeof(data_string), iso15118AuthStateToString[VehicleAuth_None]);
            if(memcmp(label_text, data_string, strlen(data_string)) != 0)
            {
                lv_label_set_text(guider_ui.Main_screen_auth_value, data_string);
            }
        }
#else
        snprintf(data_string, sizeof(data_string), iso15118AuthStateToString[VehicleAuth_None]);
        if(memcmp(label_text, data_string, strlen(data_string)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_auth_value, data_string);
        }
#endif /* ENABLE_ISO15118 */
    }

    if (EV_ChargingProtocol_isCharging())
    {
        label_text = lv_label_get_text(guider_ui.Main_screen_time_of_charge_value);
        const char* time_to_charge = NULL;
        if (sp_lastVehicleData->timeToCharge != UINT32_MAX)
        {
            time_to_charge = convertSecToFormat(sp_lastVehicleData->timeToCharge, HH_MM_SS);
        }
        else
        {
            time_to_charge = INFINITE_TIME;
        }

        if (memcmp(label_text, time_to_charge, strlen(time_to_charge)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_time_of_charge_value, time_to_charge);
        }

        label_text = lv_label_get_text(guider_ui.Main_screen_current_value);
        memset(data_string, '\0', sizeof(data_string));
        snprintf(data_string, sizeof(data_string), "%.2f", sp_lastVehicleData->chargeCurrent);
        if(memcmp(label_text, data_string, strlen(data_string)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_current_value, data_string);
        }

        label_text = lv_label_get_text(guider_ui.Main_screen_voltage_value);
        memset(data_string, '\0', sizeof(data_string));
        snprintf(data_string, sizeof(data_string), "%.2f", sp_lastVehicleData->chargeVoltage);
        if(memcmp(label_text, data_string, strlen(data_string)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_voltage_value, data_string);
        }

        label_text = lv_label_get_text(guider_ui.Main_screen_power_value);
        memset(data_string, '\0', sizeof(data_string));
        snprintf(data_string, sizeof(data_string), "%.2f", sp_lastVehicleData->chargePower);
        if(memcmp(label_text, data_string, strlen(data_string)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_power_value, data_string);
        }

        label_text = lv_label_get_text(guider_ui.Main_screen_protocol_value);
        memset(data_string, '\0', sizeof(data_string));
        snprintf(data_string, sizeof(data_string), "%s", EVSE_ChargingProtocol_GetStringFromProtocol(sp_lastVehicleData->charging_protocol));
        if(memcmp(label_text, data_string, strlen(data_string)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_protocol_value, data_string);
        }

#if ENABLE_ISO15118
        label_text = lv_label_get_text(guider_ui.Main_screen_time_of_charge_label);
        if (EVSE_ISO15118_GetEnergyDirection() == EVSE_G2V)
        {
            if (memcmp(label_text, TIME_TO_DISCHARGE_STRING, strlen(TIME_TO_DISCHARGE_STRING)) == 0)
            {
                lv_label_set_text(guider_ui.Main_screen_time_of_charge_label, TIME_TO_CHARGE_STRING);
            }
            lv_obj_set_style_bg_color(guider_ui.Main_screen_battery_indicator, lv_color_hex(GREEN_LED), LV_PART_INDICATOR|LV_STATE_DEFAULT);
        }
        else if (EVSE_ISO15118_GetEnergyDirection() == EVSE_V2G)
        {
            if (memcmp(label_text, TIME_TO_CHARGE_STRING, strlen(TIME_TO_CHARGE_STRING)) == 0)
            {
                lv_label_set_text(guider_ui.Main_screen_time_of_charge_label, TIME_TO_DISCHARGE_STRING);
            }
            lv_obj_set_style_bg_color(guider_ui.Main_screen_battery_indicator, lv_color_hex(RED_LED), LV_PART_INDICATOR|LV_STATE_DEFAULT);
        }
#endif /* ENABLE_ISO15118 */
    }
    else
    {
        label_text = lv_label_get_text(guider_ui.Main_screen_protocol_value);
        if(memcmp(label_text, NO_CHARGING_PROTOCOL, strlen(NO_CHARGING_PROTOCOL)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_protocol_value, NO_CHARGING_PROTOCOL);
        }

        label_text = lv_label_get_text(guider_ui.Main_screen_time_of_charge_value);
        char* not_charging_time = convertSecToFormat(NOT_CHARGING, HH_MM);
        if(memcmp(label_text, not_charging_time, strlen(not_charging_time)) != 0)
        {
            lv_label_set_text(guider_ui.Main_screen_time_of_charge_value, not_charging_time);
        }
        lv_obj_set_style_bg_color(guider_ui.Main_screen_battery_indicator, lv_color_hex(BLUE_LED), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
}

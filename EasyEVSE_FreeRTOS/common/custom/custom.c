/*
 * Copyright 2023-2025 NXP
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
#include <stdbool.h>
#include "lvgl.h"
#include "custom.h"
#include "gui_guider.h"
#include "custom_evse.h"
#include "FreeRTOSConfig.h"
#include "time.h"

/*********************
 *      DEFINES
 *********************/

#define MAX_UINT16_NUMBERS 8
#define MAX_UINT32_NUMBERS 12
#define MAX_CLOCK_HMFORMAT 6

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

static const char *s_screenNames[LAST_SCREEN + 1] = {[Screen_Main]          = "MAIN SCREEN",
                                                     [Screen_Linking]       = "LINKING SCREEN",
                                                     [Screen_Charging]      = "CHARGING SCREEN",
                                                     [LAST_SCREEN]          = "INVALID SCREEN"};

static uint8_t previous_image_v2g[CHARGING_IMAGES_NUMBER] = {2, 3, 4, 1};
static uint8_t previous_image_g2v[CHARGING_IMAGES_NUMBER] = {4, 1, 2, 3};

#if (BOARD_SIGBOARD_ETH == 1)
static const char SIGBOARD_connection[] = "ETH connection";
#elif (BOARD_SIGBOARD_ARDUINO_HEADER == 1)
static const char SIGBOARD_connection[] = "Arduino header";
#elif (BOARD_SIGBOARD_ARDUINO_HEADER == 0)
static const char SIGBOARD_connection[] = "Camera interface";
#endif /* BOARD_SIGBOARD_ETH */

/**********************
 *  STATIC VARIABLES
 **********************/

static const char *sp_vehicle_id         = NULL;
static const char *sp_card_type          = NULL;
static const char *sp_card_security      = NULL;
static const meter_data_t *sp_meter_data = NULL;

static const char *sp_lastNetworkMessage           = NULL;
static const char *sp_lastOCPPMessage             = NULL;
static const evse_data_t *sp_lastEvseData          = NULL;
static const vehicle_data_t *sp_lastVehicleData    = NULL;
static evse_iso15118_state_t sp_lastISO15118Status = EVSE_ISO15118_NotEnable;
static V2G_status_t sp_lastV2GStatus               = NOT_CONNECTED;

static uint32_t s_lastPowerReqCnt  = 0;
static uint32_t s_lastTelemetryCnt = 0;

static const char *auth_methods[LAST_AUTH_METHOD + 1]   = {"EIM", "PnC", "INVALID AUTH METHOD"};
static const char *V2G_status_list[LAST_V2G_STATUS + 1] = {"Not connected", "Demo", "INVALID V2G STATUS"};

static evse_screens_t current_screen = LAST_SCREEN;

extern const char *EVSE_PROTOCOL_strings[EVSE_LastChargingProtocol + 1];

int Screen_digital_clock_hour_value;
int Screen_digital_clock_min_value;
int Screen_digital_clock_sec_value;

static bool UI_isScreenDeleted(evse_screens_t screen)
{
    if(screen == Screen_Main)
    {
        return guider_ui.Main_Screen_del;
    }
    else if(screen == Screen_Linking)
    {
        return guider_ui.Linking_Screen_del;
    }
    else if(screen == Screen_Charging)
    {
        return guider_ui.Charging_Screen_del;
    }

    return true;
}

 /**
 * @brief           Calculate the digital clock by the 24-hours mode
 * @param obj       pointer to the values for hour/minute/seconds
 */
static void clock_count_24(int * hour, int * minute, int * seconds)
{
    (*seconds)++;

    if(*seconds == 60) {
        *seconds = 0;
        (*minute)++;
    }

    if(*minute == 60) {
        *minute = 0;
        (*hour)++;
    }

    if(*hour == 24) {
        *hour = 0;
    }
}

static void ClockTimer_Set(int Screen_digital_clock_hour_value, int Screen_digital_clock_min_value)
{
    uint8_t clockHMformat[MAX_CLOCK_HMFORMAT] = {0};

    snprintf(clockHMformat, sizeof(clockHMformat), "%d:%02d", Screen_digital_clock_hour_value,
             Screen_digital_clock_min_value);
    if ((ui_getCurentScreen() == Screen_Main) && guider_ui.Main_Screen_clock)
    {
        lv_label_set_text(guider_ui.Main_Screen_clock, clockHMformat);
    }
    if ((ui_getCurentScreen() == Screen_Charging) && guider_ui.Charging_Screen_clock)
    {
        lv_label_set_text(guider_ui.Charging_Screen_clock, clockHMformat);
    }
    if ((ui_getCurentScreen() == Screen_Linking) && guider_ui.Linking_Screen_clock)
    {
        lv_label_set_text(guider_ui.Linking_Screen_clock, clockHMformat);
    }
}

static void ClockTimer_callback(lv_timer_t *timer)
{
    clock_count_24(&Screen_digital_clock_hour_value, &Screen_digital_clock_min_value, &Screen_digital_clock_sec_value);
    ClockTimer_Set(Screen_digital_clock_hour_value, Screen_digital_clock_min_value);
}

/*
 * @brief Sets the address of inactive screen to NULL so it can be reused by active screen
 */
static void UI_clearScreenReference(const evse_screens_t screen)
{
    if(screen == Screen_Main)
    {
        guider_ui.Main_Screen = NULL;
    }
    else if(screen == Screen_Charging)
    {
        guider_ui.Charging_Screen = NULL;
    }
    else if(screen == Screen_Linking)
    {
        guider_ui.Linking_Screen = NULL;
    }
}

/**
 * Associate a type from evse_screens_t structure to each screen
 * @param screenObj lv_obj_t that points to a screen
 */
static evse_screens_t ScreenObjToScreenType(lv_obj_t *screenObj)
{
    evse_screens_t screenType;

    if (screenObj == guider_ui.Main_Screen)
    {
        screenType = Screen_Main;
    }
    else if (screenObj == guider_ui.Charging_Screen)
    {
        screenType = Screen_Charging;
    }
    else if (screenObj == guider_ui.Linking_Screen)
    {
        screenType = Screen_Linking;
    }
    else
    {
        screenType = LAST_SCREEN;
    }

    return screenType;
}

void custom_init(lv_ui *ui)
{
    /* Add your code here */
    lv_timer_t *timer        = lv_timer_create(ClockTimer_callback, 1000, NULL);
}

void update_meter_data()
{
    configPRINTF(("Request meter data button pressed"));
    EVSE_Set_MeterEvent(METER_READ);
}

void EVSE_Pause_Resume(void)
{
    charging_states_t charging_state = EVSE_ChargingProtocol_GetChargingState();
    if (charging_state == EVSE_ChargingPaused)
    {
        // call Resume charging session callback TODO
        EVSE_ChargingProtocol_SetChargingState(EVSE_ChargingResumed);
    }
    else if (charging_state == EVSE_ChargingResumed || charging_state == EVSE_ChargingStarted)
    {
        // call Pause charging session callback TODO
        EVSE_ChargingProtocol_SetChargingState(EVSE_ChargingPaused);
    }
}

/**
 * Get active screen
 */
evse_screens_t ui_getCurentScreen(void)
{
    return current_screen;
}

#if 0
static void UI_Update_PauseResumeBtn()
{
    if (UI_isScreenDeleted(Screen_Charging) && (ui_getCurentScreen() != Screen_Charging))
    {
        return;
    }

    if (guider_ui.Charging_Screen_btn_stop_label == NULL || guider_ui.Charging_Screen_btn_stop == NULL)
    {
        return;
    }

    charging_states_t charging_state = EVSE_ChargingProtocol_GetChargingState();

    if (charging_state == EVSE_ChargingStarted) // if charging session started for the first time
    {
        lv_obj_clear_flag(guider_ui.Charging_Screen_btn_stop, LV_OBJ_FLAG_HIDDEN);
        lv_label_set_text(guider_ui.Charging_Screen_btn_stop_label, PAUSE_TEXT);
    }
    else if (charging_state == EVSE_ChargingPaused)
    {
        lv_label_set_text(guider_ui.Charging_Screen_btn_stop_label, RESUME_TEXT);
    }
    else if (charging_state == EVSE_ChargingResumed)
    {
        lv_label_set_text(guider_ui.Charging_Screen_btn_stop_label, PAUSE_TEXT);
    }
    else if (charging_state == EVSE_ChargingNone)
    {
        lv_obj_add_flag(guider_ui.Charging_Screen_btn_stop, LV_OBJ_FLAG_HIDDEN);
    }
}
#endif

void ui_ChangeScreen(lv_obj_t *new_screen)
{
    evse_screens_t old_screen             = ScreenObjToScreenType(lv_scr_act());
    current_screen                        = ScreenObjToScreenType(new_screen);
    char data_string[MAX_EVSE_SW_VERSION] = {0};

    if((old_screen != current_screen) && (UI_isScreenDeleted(old_screen)))
    {
        UI_clearScreenReference(old_screen);
    }
    ClockTimer_Set(Screen_digital_clock_hour_value, Screen_digital_clock_min_value);

    UI_Update_NFC_Activation_Status();
    /* If we get this function called the screens are already created */
    if (current_screen == Screen_Main)
    {
        connectionModule_t connectionModule = EVSE_Connectivity_GetConnectionModule();
        if (connectionModule == EVSE_ConnectionModule_ETH)
        {
            lv_label_set_text(guider_ui.Main_Screen_type_of_connection_label, "ETH");
        }
        else if (connectionModule == EVSE_ConnectionModule_WiFi)
        {
            lv_label_set_text(guider_ui.Main_Screen_type_of_connection_label, "WiFi");
        }
        else
        {
            lv_label_set_text(guider_ui.Main_Screen_type_of_connection_label, "None");
        }
        snprintf(data_string, sizeof(data_string), "EVSE SW v%d.%d.%d", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR,
                 FIRMWARE_VERSION_HOTFIX);
        lv_label_set_text(guider_ui.Main_Screen_EVSE_software_version, data_string);
        EVSE_UI_SetEvent(EVSE_UI_NetworkStatus);
        EVSE_UI_SetEvent(EVSE_UI_OcppStatus);
        EVSE_UI_SetEvent(EVSE_UI_TelemetryData);
    }
    if (current_screen == Screen_Linking)
    {
        UI_Update_ISO15118Status(sp_lastISO15118Status);
        UI_Update_Metering_Values(sp_meter_data);
        EVSE_UI_SetEvent(EVSE_UI_NFC);
    }
    else if (current_screen == Screen_Charging)
    {
        UI_Update_Vehicle_Values(sp_lastVehicleData);
        UI_Update_Metering_Values (sp_meter_data);
        UI_Update_EVSE_Values(sp_lastEvseData);
    }

    configPRINTF(
        ("Changed screen from %s. New screen is %s", s_screenNames[old_screen], s_screenNames[current_screen]));
}

void UI_Update_Localtime(void)
{
    time_t seconds_today = (time_t)EVSE_Connectivity_GetUnixTime();
    struct tm *local_time;
    local_time                      = localtime(&seconds_today);
    Screen_digital_clock_hour_value = local_time->tm_hour;
    Screen_digital_clock_min_value  = local_time->tm_min;
    Screen_digital_clock_sec_value  = local_time->tm_sec;
    ClockTimer_Set(Screen_digital_clock_hour_value, Screen_digital_clock_min_value);
}

void UI_Update_PwrReqCntr(const uint32_t power_req_cntr)
{
    s_lastPowerReqCnt = power_req_cntr;
    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }

        if (guider_ui.Charging_Screen_power_request_cntr_value != NULL)
        {
            const char *current_power_req_cntr_str = lv_label_get_text(guider_ui.Charging_Screen_power_request_cntr_value);
            if (current_power_req_cntr_str == NULL)
            {
                return;
            }
            char power_req_cnt_str[MAX_UINT32_NUMBERS] = {0};
            snprintf(power_req_cnt_str, sizeof(power_req_cnt_str), "%d", s_lastPowerReqCnt);
            if (strcmp(power_req_cnt_str, current_power_req_cntr_str) != 0)
            {
                lv_label_set_text(guider_ui.Charging_Screen_power_request_cntr_value, power_req_cnt_str);
            }
        }
    }
}

void UI_Update_ISO15118Status(const evse_iso15118_state_t status)
{
    if (status >= EVSE_ISO15118_LAST)
    {
        return;
    }

    sp_lastISO15118Status = status;
    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }

        if (guider_ui.Linking_Screen_ISO15118_status_value != NULL)
        {
            const char *iso15118_state_string   = EVSE_ISO15118_GetStringFromState(status);
            const char *current_ISO15118_status = lv_label_get_text(guider_ui.Linking_Screen_ISO15118_status_value);

            if (iso15118_state_string == NULL)
            {
                return;
            }

            if ((current_ISO15118_status == NULL) || (strcmp(current_ISO15118_status, iso15118_state_string) != 0))
            {
                lv_label_set_text(guider_ui.Linking_Screen_ISO15118_status_value, iso15118_state_string);
            }
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }
    }
}

void UI_Update_Metering_Values(const meter_data_t *meter_data)
{
    if (meter_data == NULL)
    {
        return;
    }

    sp_meter_data = meter_data;

    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }

        lv_label_set_text(guider_ui.Linking_Screen_mode_value, sp_meter_data->EVSE_ChargeState);

        EVSE_UI_SetEvent(EVSE_UI_ISO15118_Stack_Status);

        if(guider_ui.Linking_Screen_cont_linked == NULL)
        {
            return;
        }

        lv_label_set_text(guider_ui.Linking_Screen_SIGBOARD_connection_value, SIGBOARD_connection);
        EVSE_UI_SetEvent(EVSE_UI_NFC);
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }

        char my_string[20];

        /* convert Power float to string to display in LVGL prompt */
        memset(my_string, '\0', sizeof(my_string));
        snprintf(my_string, sizeof(my_string), "%.4f", sp_meter_data->wh / 1000);
        lv_label_set_text(guider_ui.Charging_Screen_kWh_value, my_string);

        /* convert current float to string to display in LVGL prompt */
        memset(my_string, '\0', sizeof(my_string));
        snprintf(my_string, sizeof(my_string), "%.2f", sp_meter_data->irms);
        lv_label_set_text(guider_ui.Charging_Screen_current_value, my_string);

        /* convert voltage float to string to display in LVGL prompt */
        memset(my_string, '\0', sizeof(my_string));
        snprintf(my_string, sizeof(my_string), "%.2f", sp_meter_data->vrms);
        lv_label_set_text(guider_ui.Charging_Screen_voltage_value, my_string);

        lv_label_set_text(guider_ui.Charging_Screen_mode_value, sp_meter_data->EVSE_ChargeState);
    }
}

void UI_Update_Vehicle_Values(const vehicle_data_t *vehicle_data)
{
    if (vehicle_data == NULL)
    {
        return;
    }

    sp_lastVehicleData = vehicle_data;

    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }

        char data_string[MAX_PROTOCOL_LENGTH] = {0};
        char my_string[20];

#if 0
        UI_Update_PauseResumeBtn();
#endif

        lv_label_set_text(guider_ui.Charging_Screen_elapsed_time_value,
                          convertSecToFormat(sp_lastVehicleData->elapsedTime, HH_MM_SS));

        UI_Update_NFC_ID(sp_lastVehicleData->vehicleID);

        if(guider_ui.Charging_Screen_cont_details == NULL)
        {
            return;
        }

        if (EVSE_ChargingProtocol_isCharging())
        {
            lv_label_set_text(guider_ui.Charging_Screen_time_to_charge_value,
                              convertSecToFormat(sp_lastVehicleData->timeToCharge, HH_MM_SS));
        }
        else
        {
            lv_label_set_text(guider_ui.Charging_Screen_time_to_charge_value, convertSecToFormat(NOT_CHARGING, HH_MM));
        }

        if ((sp_lastVehicleData->charging_protocol == EVSE_HighLevelCharging_ISO15118) ||
            (sp_lastVehicleData->charging_protocol == EVSE_HighLevelCharging_ISO15118_20))
        {
            snprintf(data_string, sizeof(data_string), "%s - %s", EVSE_ChargingProtocol_GetProtocolString(),
                     EVSE_ISO15118_GetVehicleAuthMethodString());
            lv_label_set_text(guider_ui.Charging_Screen_protocol_value, data_string);

            lv_bar_set_value(guider_ui.Charging_Screen_bar_charging, sp_lastVehicleData->energyDeliveryStatus, LV_ANIM_OFF);
            snprintf(data_string, sizeof(data_string), "%d", sp_lastVehicleData->energyDeliveryStatus);
            lv_obj_remove_flag(guider_ui.Charging_Screen_bar_charging, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(guider_ui.Charging_Screen_battery_level, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(guider_ui.Charging_Screen_label_percent, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(guider_ui.Charging_Screen_battery_level, data_string);
        }
        else
        {
            lv_obj_add_flag(guider_ui.Charging_Screen_bar_charging, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.Charging_Screen_battery_level, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(guider_ui.Charging_Screen_label_percent, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(guider_ui.Charging_Screen_protocol_value, EVSE_ChargingProtocol_GetProtocolString());
        }

        UI_Update_PwrReqCntr(s_lastPowerReqCnt);
    }
}

void UI_Update_EVSE_Values(const evse_data_t *evse_data)
{
    if (evse_data == NULL)
    {
        return;
    }

    sp_lastEvseData = evse_data;

    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }

        char *direction_string = EVSE_ChargingProtocol_GetStringFromDirection(evse_data->EVSE_ChargingDirection);
        lv_label_set_text(guider_ui.Charging_Screen_charging_direction_value, direction_string);
        if(evse_data->EVSE_ChargingDirection == EVSE_G2V)
        {
            lv_label_set_text(guider_ui.Charging_Screen_label_charged1, "Charging Time (G2V)");
        }
        else if(evse_data->EVSE_ChargingDirection == EVSE_V2G)
        {
            lv_label_set_text(guider_ui.Charging_Screen_label_charged1, "Discharging Time (V2G)");
        }
        if(sp_lastEvseData->EVSE_Id != NULL)
        {
            lv_label_set_text(guider_ui.Charging_Screen_EVSE_ID_value, sp_lastEvseData->EVSE_Id);
        }
    }
}

void UI_Update_NFC_ID(const char *vehicleID)
{
    if(vehicleID == NULL)
    {
        return;
    }

    sp_vehicle_id = vehicleID;
    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }

        const char *label_text = lv_label_get_text(guider_ui.Linking_Screen_card_UID_value);

        if (guider_ui.Linking_Screen_card_UID_value != NULL)
        {
            if(memcmp(label_text, sp_vehicle_id, strlen(sp_vehicle_id)) != 0)
            {
                lv_label_set_text(guider_ui.Linking_Screen_card_UID_value, sp_vehicle_id);
            }
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }

        const char *label_text = lv_label_get_text(guider_ui.Charging_Screen_vehicle_id_value);

        if (guider_ui.Charging_Screen_vehicle_id_value != NULL)
        {
            if(memcmp(label_text, sp_vehicle_id, strlen(sp_vehicle_id)) != 0)
            {
                lv_label_set_text(guider_ui.Charging_Screen_vehicle_id_value, sp_vehicle_id);
            }
        }
    }
}

void UI_Update_NFC_Card_Type(const char *cardType)
{
    if(cardType == NULL)
    {
        return;
    }

    sp_card_type = cardType;
    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }

        char *label_text = lv_label_get_text(guider_ui.Linking_Screen_card_type_value);
        if (guider_ui.Linking_Screen_card_type_value != NULL)
        {
            if(memcmp(label_text, sp_card_type, strlen(sp_card_type)) != 0)
            {
                lv_label_set_text(guider_ui.Linking_Screen_card_type_value, sp_card_type);
            }
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }
    }
}

void UI_Update_NFC_Card_Security(const char *cardSecurityStatus)
{
    if(cardSecurityStatus == NULL)
    {
        return;
    }

    sp_card_security = cardSecurityStatus;
    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }

        char *label_text = lv_label_get_text(guider_ui.Linking_Screen_card_security_value);
        if (guider_ui.Linking_Screen_card_security_value != NULL)
        {
            if(memcmp(label_text, sp_card_security, strlen(sp_card_security)) != 0)
            {
                lv_label_set_text(guider_ui.Linking_Screen_card_security_value, sp_card_security);
            }
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }
    }
}

void UI_Update_NFC_Values(const char *vehicleID, const char *cardType, const char *cardSecurityStatus)
{
    UI_Update_NFC_ID(vehicleID);
    UI_Update_NFC_Card_Type(cardType);
    UI_Update_NFC_Card_Security(cardSecurityStatus);
}

void UI_Update_Connectivity_OCPP_Status(const char *OCPPMessage)
{
    if (OCPPMessage == NULL)
    {
       return;
    }

    sp_lastOCPPMessage = OCPPMessage;
    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }

        if (guider_ui.Main_Screen_OCPP_status_value)
        {
            /* Update OCPP Status prompt on Main Screen*/
            lv_label_set_text(guider_ui.Main_Screen_OCPP_status_value, sp_lastOCPPMessage);
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }
    }
}

void UI_Update_Connectivity_Telemetry_Cntr(uint32_t telemetryCnt)
{
    s_lastTelemetryCnt = telemetryCnt;
    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }

        if (guider_ui.Main_Screen_telemetry_value)
        {
            const char *current_telemetry_cnt_str      = lv_label_get_text(guider_ui.Main_Screen_telemetry_value);
            char telemetry_cnt_str[MAX_UINT32_NUMBERS] = {0};
            snprintf(telemetry_cnt_str, sizeof(telemetry_cnt_str), "%d", s_lastTelemetryCnt);

            if ((current_telemetry_cnt_str == NULL) || (strcmp(telemetry_cnt_str, current_telemetry_cnt_str) != 0))
            {
                /* Update telemetry counter if new data was sent */
                lv_label_set_text(guider_ui.Main_Screen_telemetry_value, telemetry_cnt_str);
            }
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }
    }
}

void UI_Update_Connectivity_SSID(const char *SSID)
{
    if (SSID == NULL)
    {
       return;
    }

    evse_screens_t current_screen = ui_getCurentScreen();

    if(current_screen == Screen_Main)
    {
        if(guider_ui.Main_Screen == NULL)
        {
            return;
        }

        if (guider_ui.Main_Screen_SSID_prompt != NULL)
        {
            const char *current_SSID = lv_label_get_text(guider_ui.Main_Screen_SSID_prompt);

            if ((current_SSID == NULL) || (strcmp(current_SSID, SSID)))
            {
                lv_label_set_text(guider_ui.Main_Screen_SSID_prompt, SSID);
            }
        }
    }
    else if(current_screen == Screen_Linking)
    {
        if(guider_ui.Linking_Screen == NULL)
        {
            return;
        }
    }
    else if(current_screen == Screen_Charging)
    {
        if(guider_ui.Charging_Screen == NULL)
        {
            return;
        }
    }
}

void UI_Update_Connectivity_Values(const char *OCPPMessage, uint32_t telemetryCnt, const char *SSID)
{
    UI_Update_Connectivity_OCPP_Status(OCPPMessage);
    UI_Update_Connectivity_Telemetry_Cntr(telemetryCnt);
    UI_Update_Connectivity_SSID(SSID);
}

void UI_Update_NFC_Activation_Status(void)
{
    evse_screens_t current_screen = ui_getCurentScreen();
    if(current_screen == Screen_Main)
    {
#if ENABLE_CLEV663_NFC
        if(EVSE_NFC_getNFCActivationStatus())
        {
            lv_obj_set_style_text_color(guider_ui.Main_Screen_NFC_activation_status, lv_color_hex(0x34c181), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_text_color(guider_ui.Main_Screen_NFC_activation_status, lv_color_hex(0xc51f1f), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
#endif
    }
    else if(current_screen == Screen_Linking)
    {
#if ENABLE_CLEV663_NFC
        if(EVSE_NFC_getNFCActivationStatus())
        {
            lv_obj_set_style_text_color(guider_ui.Linking_Screen_NFC_activation_status, lv_color_hex(0x34c181), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_text_color(guider_ui.Linking_Screen_NFC_activation_status, lv_color_hex(0xc51f1f), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
#endif
    }
    else if(current_screen == Screen_Charging)
    {
#if ENABLE_CLEV663_NFC
        if(EVSE_NFC_getNFCActivationStatus())
        {
            lv_obj_set_style_text_color(guider_ui.Charging_Screen_NFC_activation_status, lv_color_hex(0x34c181), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
        else
        {
            lv_obj_set_style_text_color(guider_ui.Charging_Screen_NFC_activation_status, lv_color_hex(0xc51f1f), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
#endif
    }
}

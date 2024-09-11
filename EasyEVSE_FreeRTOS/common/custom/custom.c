/*
 * Copyright 2023-2024 NXP
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
                                                     [Screen_NetworkDebug]  = "DEBUG NETWORK",
                                                     [Screen_CarSimulation] = "CAR CHARGING",
                                                     [Screen_EVSE]          = "EVSE",
                                                     [Screen_Meter]         = "METER",
                                                     [Screen_NFC]           = "NFC",
                                                     [Screen_ISODebug]      = "DEBUG ISO15118",
                                                     [LAST_SCREEN]          = "INVALID SCREEN"};

#if (BOARD_SIGBOARD_ARDUINO_HEADER == 1)
    static const char SIGBOARD_connection[] = "Arduino header";
#else
    static const char SIGBOARD_connection[] = "Camera interface";
#endif

/**********************
 *  STATIC VARIABLES
 **********************/

static const char *sp_vehicle_id         = NULL;
static const meter_data_t *sp_meter_data = NULL;

static const char *sp_lastNetworkMessage           = NULL;
static const char *sp_lastCloudMessage             = NULL;
static const evse_data_t *sp_lastEvseData          = NULL;
static const vehicle_data_t *sp_lastVehicleData    = NULL;
static evse_iso15118_state_t sp_lastISO15118Status = EVSE_ISO15118_NotEnable;
static V2G_status_t sp_lastV2GStatus               = NOT_CONNECTED;

static uint32_t s_lastPowerReqCnt          = 0;
static uint32_t s_lastTelemetryCnt         = 0;

static const char *auth_methods[LAST_AUTH_METHOD + 1]   = {"EIM", "PnC", "INVALID AUTH METHOD"};
static const char *V2G_status_list[LAST_V2G_STATUS + 1] = {"Not connected", "Demo", "INVALID V2G STATUS"};

static evse_screens_t current_screen = LAST_SCREEN;

extern const char *EVSE_PROTOCOL_strings[EVSE_LastChargingProtocol + 1];

int Screen_digital_clock_hour_value;
int Screen_digital_clock_min_value;
int Screen_digital_clock_sec_value;

static void ClockTimer_Set(int Screen_digital_clock_hour_value, int Screen_digital_clock_min_value)
{
    uint8_t clockHMformat[MAX_CLOCK_HMFORMAT] = {0};

    snprintf(clockHMformat, sizeof(clockHMformat), "%d:%02d", Screen_digital_clock_hour_value, Screen_digital_clock_min_value);
    if (guider_ui.Main_Screen_clock)
    {
        lv_label_set_text(guider_ui.Main_Screen_clock, clockHMformat);
    }
    if (guider_ui.Debug_Conn_Screen_clock)
    {
        lv_label_set_text(guider_ui.Debug_Conn_Screen_clock, clockHMformat);
    }
    if (guider_ui.Car_Screen_clock)
    {
        lv_label_set_text(guider_ui.Car_Screen_clock, clockHMformat);
    }
    if (guider_ui.EVSE_Screen_clock)
    {
        lv_label_set_text(guider_ui.EVSE_Screen_clock, clockHMformat);
    }
    if (guider_ui.Meter_Screen_clock)
    {
        lv_label_set_text(guider_ui.Meter_Screen_clock, clockHMformat);
    }
    if (guider_ui.NFC_Screen_clock)
    {
        lv_label_set_text(guider_ui.NFC_Screen_clock, clockHMformat);
    }
    if (guider_ui.Debug_ISO_Screen_clock)
    {
        lv_label_set_text(guider_ui.Debug_ISO_Screen_clock, clockHMformat);
    }
}

static void ClockTimer_callback(lv_timer_t *timer)
{
    clock_count_24(&Screen_digital_clock_hour_value, &Screen_digital_clock_min_value, &Screen_digital_clock_sec_value);
    ClockTimer_Set(Screen_digital_clock_hour_value, Screen_digital_clock_min_value);
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
    else if (screenObj == guider_ui.Debug_Conn_Screen)
    {
        screenType = Screen_NetworkDebug;
    }
    else if (screenObj == guider_ui.Car_Screen)
    {
        screenType = Screen_CarSimulation;
    }
    else if (screenObj == guider_ui.EVSE_Screen)
    {
        screenType = Screen_EVSE;
    }
    else if (screenObj == guider_ui.Meter_Screen)
    {
        screenType = Screen_Meter;
    }
    else if (screenObj == guider_ui.NFC_Screen)
    {
        screenType = Screen_NFC;
    }
    else if (screenObj == guider_ui.Debug_ISO_Screen)
    {
        screenType = Screen_ISODebug;
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
    lv_timer_t *timer = lv_timer_create(ClockTimer_callback, 1000, NULL);
}

void update_meter_data()
{
    configPRINTF(("Request meter data button pressed"));
    EVSE_Set_MeterEvent(METER_READ);
}

void read_NFC_UID()
{
    /* TODO */
}

/**
 * Get active screen
 */
evse_screens_t ui_getCurentScreen(void)
{
    return current_screen;
}

static void updateChargingSettings(evse_charging_protocol_t protocol)
{
    char requestedEnergy_string[MAX_UINT32_NUMBERS] = {0};
    char deliveredEnergy_string[MAX_UINT32_NUMBERS] = {0};

    if (protocol == EVSE_BasicCharging_J1772)
    {
        lv_obj_add_flag(guider_ui.Car_Screen_requested_energy_value, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.Car_Screen_delivered_energy_value, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.Car_Screen_energy_progress_bar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.Car_Screen_requested_energy_prompt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.Car_Screen_delivered_energy_prompt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(guider_ui.Car_Screen_energy_progress_prompt, LV_OBJ_FLAG_HIDDEN);
    }
    else if (protocol == EVSE_HighLevelCharging_ISO15118)
    {
        lv_obj_clear_flag(guider_ui.Car_Screen_requested_energy_value, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Car_Screen_delivered_energy_value, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Car_Screen_energy_progress_bar, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Car_Screen_requested_energy_prompt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Car_Screen_delivered_energy_prompt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Car_Screen_energy_progress_prompt, LV_OBJ_FLAG_HIDDEN);
        memset(requestedEnergy_string, '\0', sizeof(requestedEnergy_string));
        snprintf(requestedEnergy_string, sizeof(requestedEnergy_string), "%d", sp_lastVehicleData->requestedEnergy);
        lv_label_set_text(guider_ui.Car_Screen_requested_energy_value, requestedEnergy_string);
        memset(deliveredEnergy_string, '\0', sizeof(deliveredEnergy_string));
        snprintf(deliveredEnergy_string, sizeof(deliveredEnergy_string), "%d", sp_lastVehicleData->deliveredEnergyHLC);
        lv_label_set_text(guider_ui.Car_Screen_delivered_energy_value, deliveredEnergy_string);
        lv_bar_set_value(guider_ui.Car_Screen_energy_progress_bar, sp_lastVehicleData->energyDeliveryStatus,
                         LV_ANIM_OFF);
    }
}

void UI_Update_Car_VehicleID(const char *vehicleID)
{
    if(vehicleID == NULL)
    {
        return;
    }
    else if (guider_ui.Car_Screen && guider_ui.Car_Screen_vehicle_id_value)
    {
        lv_label_set_text(guider_ui.Car_Screen_vehicle_id_value, vehicleID);
    }
}

/**
 * Update vehicle ID in NFC Screen
 */
void UI_Update_NFC_VehicleID(const char *vehicleID)
{
    sp_vehicle_id = vehicleID;

    if (sp_vehicle_id == NULL)
    {
        return;
    }

    if (guider_ui.NFC_Screen_card_UID_value != NULL)
    {
        /* Update card UID prompt on NFC Screen*/
        lv_label_set_text(guider_ui.NFC_Screen_card_UID_value, sp_vehicle_id);
    }
}

/**
 * Update cloud status message in Debug Screen
 */
void UI_Update_Debug_CloudMessage(const char *CloudMessage)
{
    if (CloudMessage == NULL)
    {
        return;
    }
    sp_lastCloudMessage = CloudMessage;

    if ((sp_lastCloudMessage != NULL) && (guider_ui.Debug_Conn_Screen_cloud_status_value))
    {
        /* Update Cloud Status prompt on Debug Screen*/
        lv_label_set_text(guider_ui.Debug_Conn_Screen_cloud_status_value, sp_lastCloudMessage);
    }
}

/**
 * Update network message in Debug Screen
 */
void UI_Update_Debug_NetworkMessage(const char *NetworkMessage)
{
    if (NetworkMessage == NULL)
    {
        return;
    }

    sp_lastNetworkMessage = NetworkMessage;

    if ((sp_lastNetworkMessage != NULL) && (guider_ui.Debug_Conn_Screen_network_status_value))
    {
        /* Update Network Status prompt on Debug Screen*/
        lv_label_set_text(guider_ui.Debug_Conn_Screen_network_status_value, sp_lastNetworkMessage);
    }
}

/**
 * Update telemetry counter in Debug Screen
 */
void UI_Update_Debug_TelemetryCnt(uint32_t telemetryCnt)
{
    s_lastTelemetryCnt = telemetryCnt;

    if (guider_ui.Debug_Conn_Screen_telemetry_value)
    {
        const char *current_telemetry_cnt_str = lv_label_get_text(guider_ui.Debug_Conn_Screen_telemetry_value);
        char telemetry_cnt_str[MAX_UINT32_NUMBERS] = {0};
        snprintf(telemetry_cnt_str, sizeof(telemetry_cnt_str), "%d", s_lastTelemetryCnt);

        if ((current_telemetry_cnt_str == NULL) || (strcmp(telemetry_cnt_str, current_telemetry_cnt_str) != 0))
        {
            /* Update telemetry counter if new data was sent */
            lv_label_set_text(guider_ui.Debug_Conn_Screen_telemetry_value, telemetry_cnt_str);
        }
    }
}

/**
 * Update data in Debug Screen
 */
void UI_Update_Debug_Values(const char *NetworkMessage, const char *CloudMessage, uint32_t telemetryCnt)
{
    UI_Update_Debug_NetworkMessage(NetworkMessage);
    UI_Update_Debug_CloudMessage(CloudMessage);
    UI_Update_Debug_TelemetryCnt(telemetryCnt);
}

/**
 * Update data in Car Simulation Screen
 */
void UI_Update_Car_Values(const vehicle_data_t *vehicle_data)
{
    if (guider_ui.Car_Screen == NULL)
    {
        return;
    }

    if (vehicle_data == NULL)
    {
        return;
    }

    sp_lastVehicleData   = vehicle_data;
    char data_string[MAX_PROTOCOL_LENGTH] = {0};

    if (EVSE_ChargingProtocol_isCharging())
    {
        lv_label_set_text(guider_ui.Car_Screen_time_to_charge_value,
                          convertSecToFormat(sp_lastVehicleData->timeToCharge, HH_MM_SS));
    }
    else
    {
        lv_label_set_text(guider_ui.Car_Screen_time_to_charge_value, convertSecToFormat(NOT_CHARGING, HH_MM));
    }

    lv_label_set_text(guider_ui.Car_Screen_elapsed_time_value,
                      convertSecToFormat(sp_lastVehicleData->elapsedTime, HH_MM_SS));

    updateChargingSettings(sp_lastVehicleData->charging_protocol);
    UI_Update_Car_VehicleID(sp_lastVehicleData->vehicleID);

    if (sp_lastVehicleData->charging_protocol == EVSE_HighLevelCharging_ISO15118)
    {
        snprintf(data_string, sizeof(data_string), "%s - %s", EVSE_PROTOCOL_strings[EVSE_HighLevelCharging_ISO15118], auth_methods[EVSE_ISO15118_GetVehicleAuthMethod()]);
        lv_label_set_text(guider_ui.Car_Screen_protocol_value, data_string);
    }
    else
    {
        lv_label_set_text(guider_ui.Car_Screen_protocol_value, EVSE_PROTOCOL_strings[EVSE_BasicCharging_J1772]);
    }

    snprintf(data_string, sizeof(data_string), "%.2f", sp_lastVehicleData->chargeCurrent);
    lv_label_set_text(guider_ui.Car_Screen_charge_current_value, data_string);
}

/**
 * Update data in EVSE Screen
 *
 * @param meter structure that includes all EVSE parameters
 */
void UI_Update_EVSE_Values(const evse_data_t *evse_data)
{
    char my_string[20];

    if (guider_ui.EVSE_Screen == NULL)
    {
        return;
    }

    if (evse_data == NULL)
    {
        lv_led_set_color(guider_ui.EVSE_Screen_charge_state_led, lv_color_hex(RED_LED));
        return;
    }

    sp_lastEvseData = evse_data;

    /* convert current charge cost float to string to display in LVGL prompt */
    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%.2f", sp_lastEvseData->EVSE_ChargeCost);
    lv_label_set_text(guider_ui.EVSE_Screen_charge_cost_value, my_string);

    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%d", sp_lastEvseData->EVSE_Rating);
    lv_label_set_text(guider_ui.EVSE_Screen_power_rate_value, my_string);

    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%d", sp_lastEvseData->EVSE_Temperature);
    lv_label_set_text(guider_ui.EVSE_Screen_temperature_value, my_string);

    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%d", sp_lastEvseData->EVSE_GridPowerLimit);
    lv_label_set_text(guider_ui.EVSE_Screen_grid_limit_value, my_string);

    lv_label_set_text(guider_ui.EVSE_Screen_EVSE_ID_value, sp_lastEvseData->EVSE_Id);

    if (sp_lastEvseData->EVSE_IsCharging == true)
    {
        lv_led_set_color(guider_ui.EVSE_Screen_charge_state_led, lv_color_hex(GREEEN_LED));
    }
    else
    {
        lv_led_set_color(guider_ui.EVSE_Screen_charge_state_led, lv_color_hex(RED_LED));
    }

    snprintf(my_string, sizeof(my_string), "HW v%d\nSW v%d.%d.%d", sp_lastEvseData->SIGBRD_HW_version,
            sp_lastEvseData->SIGBRD_SW_version_major, sp_lastEvseData->SIGBRD_SW_version_minor,
            sp_lastEvseData->SIGBRD_SW_version_bugfix);
    lv_label_set_text(guider_ui.EVSE_Screen_sigboard_versions_value, my_string);
}

/**
 * Update data in Meter Screen
 *
 * @param meter structure that includes all EVSE parameters
 */
void UI_Update_Meter_Values(const meter_data_t *meter)
{
    char my_string[20];

    sp_meter_data = meter;

    if (sp_meter_data == NULL)
    {
        return;
    }

    if (guider_ui.Meter_Screen == NULL)
    {
        return;
    }

    /* convert current float to string to display in LVGL prompt */
    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%.2f", sp_meter_data->irms);
    lv_label_set_text(guider_ui.Meter_Screen_I_RMS_value, my_string);

    /* convert voltage float to string to display in LVGL prompt */
    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%.2f", sp_meter_data->vrms);
    lv_label_set_text(guider_ui.Meter_Screen_V_RMS_value, my_string);

    /* convert Power float to string to display in LVGL prompt */
    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%.4f", sp_meter_data->wh / 1000);
    lv_label_set_text(guider_ui.Meter_Screen_kWh_value, my_string);
    lv_label_set_text(guider_ui.Meter_Screen_active_value, my_string);

    /* convert reactive value float to string to display in LVGL prompt */
    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%.2f", sp_meter_data->Q);
    lv_label_set_text(guider_ui.Meter_Screen_reactive_value, my_string);

    /* convert apparent value float to string to display in LVGL prompt */
    memset(my_string, '\0', sizeof(my_string));
    snprintf(my_string, sizeof(my_string), "%.2f", sp_meter_data->S);
    lv_label_set_text(guider_ui.Meter_Screen_aparent_value, my_string);

    /* Print state */
    lv_label_set_text(guider_ui.Meter_Screen_mode_value, sp_meter_data->EVSE_ChargeState);
}

/**
 * Update data in Main Screen
 *
 * @param meter structure that includes all EVSE parameters
 */
void UI_Update_Main_Values(const char *header)
{
    if(header == NULL)
    {
        return;
    }

    if (guider_ui.Main_Screen_SSID_prompt == NULL)
    {
        return;
    }

    const char *current_header_text = lv_label_get_text(guider_ui.Main_Screen_SSID_prompt);

    if ((current_header_text == NULL) || (strcmp(current_header_text, header)))
    {
        lv_label_set_text(guider_ui.Main_Screen_SSID_prompt, header);
    }
}

void ui_ChangeScreen(lv_obj_t *new_screen)
{
    evse_screens_t old_screen             = ScreenObjToScreenType(lv_scr_act());
    current_screen                        = ScreenObjToScreenType(new_screen);
    char data_string[MAX_EVSE_SW_VERSION] = {0};

    ClockTimer_Set(Screen_digital_clock_hour_value, Screen_digital_clock_min_value);

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
        snprintf(data_string, sizeof(data_string), "EVSE SW v%d.%d.%d", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR, FIRMWARE_VERSION_HOTFIX);
        lv_label_set_text(guider_ui.Main_Screen_EVSE_software_version, data_string);
    }
    if (current_screen == Screen_Meter)
    {
        UI_Update_Meter_Values(sp_meter_data);
    }
    else if (current_screen == Screen_EVSE)
    {
        sp_lastEvseData = EVSE_Meter_GetEVSEData();
        UI_Update_EVSE_Values(sp_lastEvseData);
    }
    else if (current_screen == Screen_NFC)
    {
        UI_Update_NFC_VehicleID(sp_vehicle_id);
    }
    else if (current_screen == Screen_CarSimulation)
    {
        UI_Update_Car_Values(sp_lastVehicleData);
    }
    else if (current_screen == Screen_NetworkDebug)
    {
        UI_Update_Debug_Values(sp_lastNetworkMessage, sp_lastCloudMessage, s_lastTelemetryCnt);
    }
    else if (current_screen == Screen_ISODebug)
    {
        UI_Update_ISO15118_Debug_Values(sp_lastISO15118Status, sp_lastV2GStatus, s_lastPowerReqCnt);
    }

    configPRINTF(
        ("Changed screen from %s. New screen is %s", s_screenNames[old_screen], s_screenNames[current_screen]));
}

void UI_Update_EVSE_Uptime()
{
    if (guider_ui.EVSE_Screen && guider_ui.EVSE_Screen_up_time_value)
    {
        lv_label_set_text(guider_ui.EVSE_Screen_up_time_value,
                          convertSecToFormat(EVSE_GetSecondsSinceBoot(), HH_MM_SS));
    }
}

void UI_Update_ISO15118_Debug_Values(const evse_iso15118_state_t ISO15118_status,
                                     const V2G_status_t V2G_status,
                                     const uint32_t Telemetry_cntr)
{
    if (guider_ui.Debug_ISO_Screen == NULL)
    {
        return;
    }
    UI_Update_Debug_ISO15118Status(ISO15118_status);
    UI_Update_Debug_V2GStatus(V2G_status);
    UI_Update_ISO15118_Debug_PwrReqCntr(Telemetry_cntr);
    lv_label_set_text(guider_ui.Debug_ISO_Screen_SIGBOARD_connection_value, SIGBOARD_connection);
}

void UI_Update_ISO15118_Debug_PwrReqCntr(const uint32_t power_req_cntr)
{
    s_lastPowerReqCnt = power_req_cntr;
    if (guider_ui.Debug_ISO_Screen_power_request_cntr_value != NULL)
    {
        const char *current_power_req_cntr_str = lv_label_get_text(guider_ui.Debug_ISO_Screen_power_request_cntr_value);
        if (current_power_req_cntr_str == NULL)
        {
            return;
        }
        char power_req_cnt_str[MAX_UINT32_NUMBERS] = {0};
        snprintf(power_req_cnt_str, sizeof(power_req_cnt_str), "%d", s_lastPowerReqCnt);
        if (strcmp(power_req_cnt_str, current_power_req_cntr_str) != 0)
        {
            lv_label_set_text(guider_ui.Debug_ISO_Screen_power_request_cntr_value, power_req_cnt_str);
        }
    }
}

void UI_Update_Debug_ISO15118Status(const evse_iso15118_state_t status)
{
    if (status >= EVSE_ISO15118_LAST)
    {
        return;
    }

    if (guider_ui.Debug_ISO_Screen_ISO15118_status_value != NULL)
    {
        const char *iso15118_state_string   = EVSE_ISO15118_GetStringFromState(status);
        const char *current_ISO15118_status = lv_label_get_text(guider_ui.Debug_ISO_Screen_ISO15118_status_value);

        if (iso15118_state_string == NULL)
        {
            return;
        }

        if ((current_ISO15118_status == NULL) || (strcmp(current_ISO15118_status, iso15118_state_string) != 0))
        {
            lv_label_set_text(guider_ui.Debug_ISO_Screen_ISO15118_status_value, iso15118_state_string);
        }
    }

    if ((status != EVSE_ISO15118_NotEnable) && (guider_ui.Debug_ISO_Screen_V2G_demo_prompt != NULL) &&
        (lv_obj_has_flag(guider_ui.Debug_ISO_Screen_V2G_demo_prompt, LV_OBJ_FLAG_HIDDEN)))
    {
        /* Assume all have the flag */
        lv_obj_clear_flag(guider_ui.Debug_ISO_Screen_V2G_demo_value, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Debug_ISO_Screen_power_request_cntr_value, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Debug_ISO_Screen_V2G_demo_prompt, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(guider_ui.Debug_ISO_Screen_power_request_cntr_prompt, LV_OBJ_FLAG_HIDDEN);
    }

    sp_lastISO15118Status = status;
}

void UI_Update_Debug_V2GStatus(const V2G_status_t status)
{
    if (status >= LAST_V2G_STATUS)
    {
        return;
    }
    sp_lastV2GStatus = status;
    if (guider_ui.Debug_ISO_Screen_V2G_demo_value != NULL)
    {
        const char *current_V2G_status = lv_label_get_text(guider_ui.Debug_ISO_Screen_V2G_demo_value);
        if ((current_V2G_status == NULL) || strcmp(current_V2G_status, V2G_status_list[status]))
        {
            lv_label_set_text(guider_ui.Debug_ISO_Screen_V2G_demo_value, V2G_status_list[status]);
        }
    }
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

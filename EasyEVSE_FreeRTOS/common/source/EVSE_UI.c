/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "event_groups.h"

#include "board.h"
#include "pin_mux.h"
#include "lvgl_support.h"

/* LVGL includes */
#include "custom.h"
#include "custom_evse.h"
#include "gui_guider.h"
#include "events_init.h"

#include "EVSE_UI.h"
#include "EVSE_Metering.h"
#include "EVSE_Connectivity.h"
#include "EVSE_Cloud_Connectivity.h"
#if (CLEV663_ENABLE == 1)
#include "EVSE_NFC.h"
#endif /* (CLEV663_ENABLE == 1) */
#if ENABLE_ISO15118
#include <ISO15118.h>
#endif /* ENABLE_ISO15118 */
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define REFRESH_SCREEN_PERIOD_MS 10
#define UPDATE_UPTIME_PERIOD_MS  1000
#define UPDATE_VEHICLE_TIME_MS   1000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static bool s_ui_initialized;
lv_ui guider_ui;

static EventGroupHandle_t s_evse_ui_event = NULL;
static TimerHandle_t s_refreshScreenTimer = NULL;
static TimerHandle_t s_refreshUptimeTimer = NULL;
static TimerHandle_t s_refreshVehicleData = NULL;

vehicle_data_t *vehicle_data = NULL;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void prvEVSE_UI_Init()
{
    lv_port_pre_init();
    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();

    s_ui_initialized = true;

    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);
}

static void prvTimer_Callback(TimerHandle_t xTimerHandle)
{
    void *pTimerHandle = pvTimerGetTimerID(xTimerHandle);
    if (pTimerHandle == (void *)&s_refreshScreenTimer)
    {
        EVSE_UI_SetEvent(EVSE_UI_RefreshScreen);
    }
    else if (pTimerHandle == (void *)&s_refreshUptimeTimer)
    {
        EVSE_UI_SetEvent(EVSE_UI_UpdateUptime);
    }
    else if (pTimerHandle == (void *)&s_refreshVehicleData)
    {
        EVSE_UI_SetEvent(EVSE_UI_ChargingSession);
    }
}

/*!
 * @brief FreeRTOS tick hook.
 */
void vApplicationTickHookUI(void)
{
    if (s_ui_initialized)
    {
        lv_tick_inc(portTICK_PERIOD_MS);
    }
}

void EVSE_UI_SetEvent(ui_events_t event)
{
    if (s_evse_ui_event)
    {
        xEventGroupSetBits(s_evse_ui_event, event);
    }
}

void EVSE_UI_Task(void *param)
{
    BaseType_t xResult = pdFAIL;

    prvEVSE_UI_Init();

    xResult = xTimerStart(s_refreshScreenTimer, 0);
    if (xResult == pdFAIL)
    {
        configPRINTF((error("[UI] Failed to start \"s_refreshScreen\" timer.")));
        while (1)
            ;
    }

    xResult = xTimerStart(s_refreshUptimeTimer, 0);
    if (xResult == pdFAIL)
    {
        configPRINTF((error("[UI] Failed to start \"s_refreshUptime\" timer.")));
        while (1)
            ;
    }

    xResult = xTimerStart(s_refreshVehicleData, 0);
    if (xResult == pdFAIL)
    {
        configPRINTF((error("[UI] Failed to start \"s_refreshVehicleData\" timer.")));
        while (1)
            ;
    }
    while (1)
    {
        EventBits_t uxBits = 0;
        uxBits             = xEventGroupWaitBits(s_evse_ui_event,    /* The event group being tested. */
                                                 EVSE_UI_ALL_EVENTS, /* The bits within the event group to wait for. */
                                                 pdTRUE, pdFALSE,    /* Don't wait for all bits, either bit will do. */
                                                 portMAX_DELAY);     /* Wait until event */

        if ((uxBits & EVSE_UI_TelemetryData) == EVSE_UI_TelemetryData)
        {
            /* Update UI data */
            const meter_data_t *meter_data = EVSE_Meter_GetMeterData();
            UI_Update_Meter_Values(meter_data);
            const uint32_t telemetry_cnt = EVSE_Cloud_GetTelemetryCount();
            UI_Update_Debug_TelemetryCnt(telemetry_cnt);
            const evse_data_t *evse_data = EVSE_Meter_GetEVSEData();
            UI_Update_EVSE_Values(evse_data);
        }

        if ((uxBits & EVSE_UI_NFC) == EVSE_UI_NFC)
        {
            /* TODO Update the NFC data */
            const char *vehicleID = NULL;
#if (CLEV663_ENABLE == 1)
            vehicleID = EVSE_NFC_Get_VehicleID();
#endif /* #if (CLEV663_ENABLE == 1) */
            UI_Update_NFC_VehicleID(vehicleID);
            UI_Update_Car_VehicleID(vehicleID);
        }

        if ((uxBits & EVSE_UI_RefreshScreen) == EVSE_UI_RefreshScreen)
        {
            /* We get here every 10ms and refresh current screen */
            lv_task_handler();
            xTimerStart(s_refreshScreenTimer, 0);
        }

        if ((uxBits & EVSE_UI_UpdateUptime) == EVSE_UI_UpdateUptime)
        {
            UI_Update_EVSE_Uptime();
        }

        if ((uxBits & EVSE_UI_UpdateLocalTime) == EVSE_UI_UpdateLocalTime)
        {
            UI_Update_Localtime();
        }

        if ((uxBits & EVSE_UI_ChargingSession) == EVSE_UI_ChargingSession)
        {
            const vehicle_data_t *vehicle_data = EVSE_ChargingProtocol_GetVehicleData();
            UI_Update_Car_Values(vehicle_data);
        }
#if ENABLE_ISO15118
        if ((uxBits & EVSE_UI_ISO15118_Stack_Status) == EVSE_UI_ISO15118_Stack_Status)
        {
            evse_iso15118_state_t status = EVSE_ISO15118_GetState();
            UI_Update_Debug_ISO15118Status(status);
        }

        if ((uxBits & EVSE_UI_ISO15118_V2G_status) == EVSE_UI_ISO15118_V2G_status)
        {
            V2G_status_t status = EVSE_ISO15118_GetV2GStatus();
            UI_Update_Debug_V2GStatus(status);
        }

        if ((uxBits & EVSE_UI_RequestedPower) == EVSE_UI_RequestedPower)
        {
            uint32_t power_requests = EVSE_ISO15118_GetPowerRequestsCntr();
            UI_Update_ISO15118_Debug_PwrReqCntr(power_requests);
        }
#endif /* ENABLE_ISO15118 */
        if ((uxBits & EVSE_UI_CloudStatus) == EVSE_UI_CloudStatus)
        {
            cloudConnectionState_t cloudConnectionState = EVSE_Cloud_GetConnectionState();
            UI_Update_Debug_CloudMessage(EVSE_Cloud_GetStringFromState(cloudConnectionState));
        }

        if ((uxBits & EVSE_UI_NetworkStatus) == EVSE_UI_NetworkStatus)
        {
            const char *headerStr             = NULL;
            connectionState_t connectionState = EVSE_Connectivity_GetConnectionState();
            UI_Update_Debug_NetworkMessage(EVSE_Connectivity_GetStringFromState(connectionState));

#if (ENABLE_WIFI == 1)
            headerStr = EVSE_Connectivity_GetAPName();
#else
            headerStr = EVSE_Connectivity_GetIPv4();
#endif /* ENABLE_WIFI */
            if (headerStr == NULL)
            {
                headerStr = EVSE_Connectivity_GetStringFromState(connectionState);
            }

            UI_Update_Main_Values(headerStr);
        }
    }

    /* Shouldn't reach this point */
    vTaskDelete(NULL);
}

void EVSE_UI_Init()
{
    s_evse_ui_event = xEventGroupCreate();

    if (s_evse_ui_event == NULL)
    {
        configPRINTF((error("%s Failed to create event"), __FUNCTION__));
        while (1)
            ;
    }

    /* create the timer that handles the refresh */
    s_refreshScreenTimer = xTimerCreate("RefreshScreen", (TickType_t)pdMS_TO_TICKS(REFRESH_SCREEN_PERIOD_MS), pdFALSE,
                                        (void *)&s_refreshScreenTimer, (TimerCallbackFunction_t)prvTimer_Callback);

    if (s_refreshScreenTimer == NULL)
    {
        configPRINTF((error("[UI] Failed to start \"RefreshScreen\" timer.")));
        while (1)
            ;
    }

    s_refreshUptimeTimer = xTimerCreate("UpdateUptime", (TickType_t)pdMS_TO_TICKS(UPDATE_UPTIME_PERIOD_MS), pdTRUE,
                                        (void *)&s_refreshUptimeTimer, (TimerCallbackFunction_t)prvTimer_Callback);

    if (s_refreshUptimeTimer == NULL)
    {
        configPRINTF((error("[UI] Failed to start \"RefreshUptime\" timer.")));
        while (1)
            ;
    }

    s_refreshVehicleData = xTimerCreate("VehicleTimer", (TickType_t)pdMS_TO_TICKS(UPDATE_VEHICLE_TIME_MS), pdTRUE,
                                        (void *)&s_refreshVehicleData, (TimerCallbackFunction_t)prvTimer_Callback);

    if (s_refreshVehicleData == NULL)
    {
        configPRINTF((error("[UI] Failed to start \"RefreshUptime\" timer.")));
        while (1)
            ;
    }

    /* Init LVGL */
    if (xTaskCreate(EVSE_UI_Task, "UI", APP_UI_STACK_SIZE, NULL, APP_UI_PRIORITY - 1, NULL) != pdPASS)
    {
        while (1)
            ;
    }
}

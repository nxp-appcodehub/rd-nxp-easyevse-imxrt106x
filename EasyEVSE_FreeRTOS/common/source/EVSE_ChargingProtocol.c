/*
 * Copyright 2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include "EVSE_config.h"

#include "hal_uart_bridge.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#if (ENABLE_ISO15118 == 1)
#include "appl-main.h"
#endif /* (ENABLE_ISO15118 == 1)*/
#include "EVSE_ChargingProtocol.h"
#include "EVSE_UI.h"
#if (CLEV663_ENABLE == 1)
#include "EVSE_NFC.h"
#endif /* (CLEV663_ENABLE == 1) */
#include "EVSE_Metering.h"

#include "ISO15118-2.h"
#include "IEC61851.h"

uint16_t stack_delay = 0;

static vehicle_data_t vehicle_data = {0};
static evse_charging_protocol_t s_curentChargingProtocol = EVSE_BasicCharging_J1772;
static EventGroupHandle_t s_evse_charging_event = NULL;

const char *EVSE_PROTOCOL_strings[EVSE_LastChargingProtocol + 1] = {
        [EVSE_BasicCharging_J1772]          = "BASIC",
        [EVSE_HighLevelCharging_ISO15118]   = "ISO15118",
        [EVSE_LastChargingProtocol]         = "LAST"
    };

static void EVSE_ChargingProtocol_StopCharging()
{
#if ENABLE_ISO15118
    EVSE_ISO15118_StopCharging(true);
#elif ENABLE_J1772
    EVSE_J1772_StopCharging(true);
#endif /* ENABLE_ISO15118 */
}

static void EVSE_ChargingWaitEvent()
{
    EventBits_t uxBits = 0;
    uxBits             = xEventGroupWaitBits(s_evse_charging_event,    /* The event group being tested. */
                                             EVSE_Charging_ALL_EVENTS, /* The bits within the event group to wait for. */
                                             pdTRUE, pdFALSE,    /* Don't wait for all bits, either bit will do. */
                                             pdMS_TO_TICKS(stack_delay));     /* Wait until event */

    if ((uxBits & EVSE_Charging_EVSERefreshData) == EVSE_Charging_EVSERefreshData)
    {
#ifndef EASYEVSE_EV
        const evse_data_t *evse_data = EVSE_Meter_GetEVSEData();
        EVSE_ChargingProtocol_SetMaxCurrent(MIN(evse_data->EVSE_GridPowerLimit, evse_data->EVSE_Rating));
#endif
    }
    else if ((uxBits & EVSE_Charging_EVSEStopCharging) == EVSE_Charging_EVSEStopCharging)
    {
        configPRINTF(("Stop charge request"));
        EVSE_ChargingProtocol_StopCharging();
    }

}

static void EVSE_WaitSigboardReady_Blocking()
{
    bool sigbrdReady =  false;

    /* Set the delay to 1 until we finished load the firmware */
    EVSE_ChargingProtocol_SetTaskDelay(CHARGINGPROTOCOL_LONGTICK_DELAY);

    while (sigbrdReady == false)
    {
        uint32_t hwversion = INVALID_SIGBOARD_HW_VERSION;

        SIGBRD_GetGetHWVersion(&hwversion);

        if (hwversion != INVALID_SIGBOARD_HW_VERSION)
        {
            sigbrdReady = true;
            configPRINTF(("SIGBOARD detected, HW Version %d", hwversion));
        }

        vTaskDelay(pdMS_TO_TICKS(stack_delay));

        if (stack_delay < CHARGINGPROTOCOL_MAX_DELAY)
        {
            stack_delay =  stack_delay * 2;
        }
    }
}

static void chargingprotocol_task(void *pvParameters)
{
    bool stopCharging = false;
    (void)pvParameters;

    EVSE_WaitSigboardReady_Blocking();

    EVSE_ChargingProtocol_SetTaskDelay(CHARGINGPROTOCOL_SHORTTICK_DELAY);

#if ENABLE_ISO15118
    EVSE_ISO15118_Init();
#else
    EVSE_J1772_Init();
#endif /* ENABLE_ISO15118 */
    while (1)
    {
        if (stopCharging == false)
        {
#if ENABLE_ISO15118
            EVSE_ISO15118_Loop(&stopCharging);
#else
            EVSE_J1772_Loop(&stopCharging);
#endif /* ENABLE_ISO15118 */
        }
        EVSE_ChargingWaitEvent();
    }
}

void EVSE_ChargingProtocol_SetEvent(charging_events_t event)
{
    if (s_evse_charging_event)
    {
        xEventGroupSetBits(s_evse_charging_event, event);
    }
}

void EVSE_ChargingProtocol_SetTaskDelay(uint16_t new_stack_delay)
{
    if ((new_stack_delay >= 1) && (new_stack_delay <= CHARGINGPROTOCOL_LONGTICK_DELAY))
    {
        stack_delay = new_stack_delay;
        SIGBRD_SetChargingProtocol_TickDelayMs(stack_delay);
    }
}

void EVSE_ChargingProtocol_SetMaxCurrent(uint32_t max_current)
{

#if ENABLE_ISO15118
    EVSE_ISO15118_SetMaxCurrent(max_current);
#elif ENABLE_J1772
    EVSE_J1772_SetMaxCurrent(max_current);
#endif /* ENABLE_ISO15118 */
}

const char* EVSE_ChargingProtocol_GetStringFromCurrentProtocol()
{
    if (s_curentChargingProtocol < EVSE_LastChargingProtocol)
    {
        return EVSE_PROTOCOL_strings[s_curentChargingProtocol];
    }
    else
    {
        return NULL;
    }
}

const char* EVSE_ChargingProtocol_GetStringFromProtocol(evse_charging_protocol_t charging_protocol)
{
    if (charging_protocol < EVSE_LastChargingProtocol)
    {
        return EVSE_PROTOCOL_strings[charging_protocol];
    }
    else
    {
        return NULL;
    }
}

void EVSE_ChargingProtocol_SetProtocol(evse_charging_protocol_t charging_protocol)
{
     if (s_curentChargingProtocol != charging_protocol)
     {
         s_curentChargingProtocol = charging_protocol;
         EVSE_UI_SetEvent(EVSE_UI_ChargingSession);
     }
}

evse_charging_protocol_t EVSE_ChargingProtocol_GetProtocol()
{
    return s_curentChargingProtocol;
}

bool EVSE_ChargingProtocol_isCharging()
{
    bool bCharging;
#if ENABLE_ISO15118
    EVSE_ISO15118_isCharging(&bCharging);
#elif ENABLE_J1772
    EVSE_J1772_isCharging(&bCharging);
#endif /* ENABLE_ISO15118 */

    return bCharging;
}

/* Check if EV is charging */
const char* EVSE_ChargingProtocol_isChargingString(void)
{
    if (EVSE_ChargingProtocol_isCharging())
    {
        return "YES";
    }
    else
    {
        return "NO";
    }
}

const vehicle_data_t *EVSE_ChargingProtocol_GetVehicleData(void)
{

#if ENABLE_ISO15118
    EVSE_ISO15118_GetVehicleData(&vehicle_data);
#elif ENABLE_J1772
    EVSE_J1772_GetVehicleData(&vehicle_data);
#endif /* ENABLE_ISO15118 */

    vehicle_data.charging_protocol    = EVSE_ChargingProtocol_GetProtocol();
    strcpy(vehicle_data.protocol, EVSE_ChargingProtocol_GetStringFromCurrentProtocol());
#if (CLEV663_ENABLE == 1)
    strcpy(vehicle_data.vehicleID, EVSE_NFC_Get_VehicleID());
#endif  /* (CLEV663_ENABLE == 1) */
    return &vehicle_data;
}

char *EVSE_ChargingProtocol_GetCPStateString ()
{
    static char cpStateString[2] = "A";
#if ENABLE_ISO15118
    cpStateString[0] = EVSE_ISO15118_GetCpStateString();
#elif ENABLE_J1772
    cpStateString[0] = EVSE_J1772_GetCpStateString();
#endif /* ENABLE_ISO15118 */

    return cpStateString;
}

static void EVSE_ChargingProtocol_DataInit()
{
    strcpy(vehicle_data.vehicleID, "N/A");
    strcpy(vehicle_data.protocol, EVSE_ChargingProtocol_GetStringFromCurrentProtocol());
    vehicle_data.chargeCurrent        = 0;
    vehicle_data.timeToCharge         = 0;
    vehicle_data.elapsedTime          = 0;
    vehicle_data.requestedEnergy      = 0;
    vehicle_data.deliveredEnergy      = 0;
    vehicle_data.energyDeliveryStatus = 0;
    vehicle_data.batteryCapacity      = 0;
    vehicle_data.batteryLevel         = 0;
    vehicle_data.charging_protocol    = EVSE_ChargingProtocol_GetProtocol();
}

void EVSE_ChargingProtocol_Init(void)
{

    stack_delay = CHARGINGPROTOCOL_TICK_DELAY;
    SIGBRD_SetChargingProtocol_TickDelayMs(stack_delay);
    SIGBRD_UART_BridgeEntry();
    EVSE_ChargingProtocol_DataInit();

    s_evse_charging_event = xEventGroupCreate();

    if (s_evse_charging_event == NULL)
    {
        configPRINTF((error("%s Failed to create event"), __FUNCTION__));
        while (1)
            ;
    }

    if (xTaskCreate(chargingprotocol_task, "Charging Task", APP_ISO15118_STACK_SIZE, NULL, APP_ISO15118_PRIORITY,
                    NULL) != pdPASS)
    {
        configPRINTF(("Failed to create Charging Task!\r\n"));
        while (1)
            ;
    }
}

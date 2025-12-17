/*
 * Copyright 2024-2025 NXP
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
#include "fsl_adapter_gpio.h"

#if (ENABLE_ISO15118 == 1)
#include "appl-main.h"
#endif /* (ENABLE_ISO15118 == 1)*/
#include "EVSE_ChargingProtocol.h"
#include "EVSE_UI.h"
#if (ENABLE_CLEV663_NFC == 1)
#include "EVSE_NFC.h"
#endif /* (ENABLE_CLEV663_NFC == 1) */
#include "EVSE_Metering.h"

#include "ISO15118.h"
#include "IEC61851.h"

#define MASK_INTERRUPT_TIME_MS 1000U

GPIO_HANDLE_DEFINE(PushBtnGpioHandle);

uint16_t stack_delay = 0;

static vehicle_data_t vehicle_data                       = {0};
static evse_charging_protocol_t s_curentChargingProtocol = EVSE_BasicCharging_J1772;
static EventGroupHandle_t s_evse_charging_event          = NULL;
static charging_states_t s_charging_state                = EVSE_ChargingNone;

const char *EVSE_PROTOCOL_strings[EVSE_LastChargingProtocol + 1] = {
    [EVSE_BasicCharging_J1772]           = "BASIC",
    [EVSE_HighLevelCharging_ISO15118]    = "ISO15118-2",
    [EVSE_HighLevelCharging_ISO15118_20] = "ISO15118-20",
    [EVSE_LastChargingProtocol]          = "LAST"};

volatile static bool pause_resume_button_pressed;

static void EVSE_ChargingProtocol_PauseResumeBtn_callback()
{
    pause_resume_button_pressed = true;
}

static void EVSE_ChargingProtocol_ChangeDirection_callback()
{
    static uint32_t last_callback = 0;
    uint32_t current_time         = EVSE_GetMsSinceBoot();

    if ((current_time - last_callback) > MASK_INTERRUPT_TIME_MS)
    {
#if ENABLE_ISO15118
        EVSE_ISO15118_ChangeEnergyDirection();
#endif /* ENABLE_ISO15118 */
        last_callback = current_time;
    }
}

void EVSE_ChargingProtocol_StartCharging()
{
#if ENABLE_ISO15118
    EVSE_ISO15118_StartCharging();
#endif
}

void EVSE_ChargingProtocol_StopCharging()
{
#if ENABLE_ISO15118
    EVSE_ISO15118_StopCharging(true);
#elif ENABLE_J1772
    EVSE_J1772_StopCharging(true);
#endif /* ENABLE_ISO15118 */
}

static void EVSE_ChargingProtocol_PauseCharging()
{
#if ENABLE_ISO15118
    // TODO check iso15118_20 and pause charging session
#endif /* ENABLE_ISO15118 */
}
#if EASYEVSE_EV
static void EV_ChargingProtocol_SetDefaults()
{
#if ENABLE_ISO15118
    EV_ISO15118_SetDefault();
#endif /* ENABLE_ISO15118 */

}
#endif /* EASYEVSE_EV */

static void EVSE_ChargingWaitEvent()
{
    EventBits_t uxBits = 0;
    uxBits             = xEventGroupWaitBits(s_evse_charging_event,    /* The event group being tested. */
                                             EVSE_Charging_ALL_EVENTS, /* The bits within the event group to wait for. */
                                             pdTRUE, pdFALSE, /* Don't wait for all bits, either bit will do. */
                                             pdMS_TO_TICKS(stack_delay)); /* Wait until event */

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
    else if ((uxBits & EVSE_Charging_EVSESuspendCharging) == EVSE_Charging_EVSESuspendCharging)
    {
        configPRINTF(("Suspend charge request"));
        EVSE_ChargingProtocol_PauseCharging();
    }
}

static void EVSE_WaitSigboardReady_Blocking()
{
    bool sigbrdReady = false;

    /* Set the delay to 1 until we finished load the firmware */
    EVSE_ChargingProtocol_SetTaskDelay(CHARGINGPROTOCOL_LONGTICK_DELAY);

    while (sigbrdReady == false)
    {
        uint32_t hwversion = INVALID_SIGBOARD_HW_VERSION;
        uint32_t swversion_major = INVALID_SIGBOARD_SW_VERSION;
        uint32_t swversion_minor = INVALID_SIGBOARD_SW_VERSION;
        uint32_t swversion_hot_fix = INVALID_SIGBOARD_SW_VERSION;

        SIGBRD_GetGetHWVersion(&hwversion);

        if (hwversion != INVALID_SIGBOARD_HW_VERSION)
        {
            configPRINTF(("SIGBOARD detected, HW Version %d", hwversion));
        }

        SIGBRD_GetSWVersion(&swversion_major, &swversion_minor, &swversion_hot_fix);
        if ((swversion_major != INVALID_SIGBOARD_SW_VERSION) && (swversion_minor != INVALID_SIGBOARD_SW_VERSION) && (swversion_hot_fix != INVALID_SIGBOARD_SW_VERSION))
        {
            sigbrdReady = true;
            configPRINTF(("SIGBOARD detected, SW Version %d.%d.%d", swversion_major, swversion_minor, swversion_hot_fix));
        }

        vTaskDelay(pdMS_TO_TICKS(stack_delay));

        if (stack_delay < CHARGINGPROTOCOL_MAX_DELAY)
        {
            stack_delay = stack_delay * 2;
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

void EVSE_ChargingProtocol_SetTaskPriority(charging_priority_t priority_mode)
{
    if (priority_mode == EVSE_Charging_EVSEMaxPriority)
    {
        vTaskPrioritySet(NULL, configMAX_PRIORITIES - 1);
    }
    else
    {
        vTaskPrioritySet(NULL, APP_ISO15118_PRIORITY);
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

const char *EVSE_ChargingProtocol_GetStringFromCurrentProtocol()
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

const char *EVSE_ChargingProtocol_GetStringFromProtocol(evse_charging_protocol_t charging_protocol)
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

const char *EVSE_ChargingProtocol_GetProtocolString(void)
{
    return EVSE_PROTOCOL_strings[s_curentChargingProtocol];
}

bool EVSE_ChargingProtocol_isCharging()
{
    bool bCharging = false;
#if ENABLE_ISO15118
    EVSE_ISO15118_isCharging(&bCharging);
#elif ENABLE_J1772
    EVSE_J1772_isCharging(&bCharging);
#endif /* ENABLE_ISO15118 */

    return bCharging;
}

charging_directions_t EVSE_ChargingProtocol_ChargingDirection()
{
    charging_directions_t charging_direction = EVSE_NoChargingDirection;

    if (EVSE_ChargingProtocol_isCharging())
    {
        if (s_curentChargingProtocol == EVSE_BasicCharging_J1772)
        {
            charging_direction = EVSE_G2V;
        }
#if ENABLE_ISO15118
        else if ((s_curentChargingProtocol == EVSE_HighLevelCharging_ISO15118) ||
                 (s_curentChargingProtocol == EVSE_HighLevelCharging_ISO15118_20))
        {
            charging_direction = EVSE_ISO15118_GetEnergyDirection();
        }
#endif /* ENABLE_ISO15118 */
    }

    return charging_direction;
}

const char *EVSE_ChargingProtocol_GetStringFromDirection(charging_directions_t charging_direction)
{
    if(charging_direction == EVSE_G2V)
    {
        return "Grid to Vehicle";
    }
    else if(charging_direction == EVSE_V2G)
    {
        return "Vehicle to Grid";
    }
    else
    {
        return "No charging direction";
    }
}

charging_states_t EVSE_ChargingProtocol_GetChargingState()
{
    return s_charging_state;
}

void EVSE_ChargingProtocol_SetChargingState(charging_states_t charging_state)
{
    s_charging_state = charging_state;
}

/* Check if EV is charging */
const char *EVSE_ChargingProtocol_isChargingString(void)
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

    vehicle_data.charging_protocol = EVSE_ChargingProtocol_GetProtocol();
    strcpy(vehicle_data.protocol, EVSE_ChargingProtocol_GetStringFromCurrentProtocol());
#if (ENABLE_CLEV663_NFC == 1)
    strcpy(vehicle_data.vehicleID, EVSE_NFC_Get_VehicleID());
#endif /* (ENABLE_CLEV663_NFC == 1) */
    return &vehicle_data;
}

char *EVSE_ChargingProtocol_GetCPStateString()
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
#if EASYEVSE_EV
    vehicle_data.chargeVoltage = 0.0;
    vehicle_data.chargePower   = 0.0;
    vehicle_data.fBatteryLevel = 0.0;
#endif /* EASYEVSE_EV */
}

void EVSE_ChargingProtocol_Init(void)
{
    stack_delay = CHARGINGPROTOCOL_TICK_DELAY;
    SIGBRD_SetChargingProtocol_TickDelayMs(stack_delay);
    SIGBRD_UART_BridgeEntry();
    EVSE_ChargingProtocol_DataInit();
#if EASYEVSE_EV
    EV_ChargingProtocol_SetDefaults();
#endif /* EASYEVSE_EV */

    hal_gpio_pin_config_t user_btn_config = {
        kHAL_GpioDirectionIn,
        0,
        BOARD_USER_BUTTON_GPIO_PORT,
        BOARD_USER_BUTTON_GPIO_PIN,
    };
    HAL_GpioInit(PushBtnGpioHandle, &user_btn_config);
    HAL_GpioSetTriggerMode(PushBtnGpioHandle, kHAL_GpioInterruptFallingEdge);
    HAL_GpioInstallCallback(PushBtnGpioHandle, EVSE_ChargingProtocol_ChangeDirection_callback, NULL);

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

void EVSE_ChargingProtocol_SetPaymentMethod(evse_auth_methods_t method)
{
#if ENABLE_ISO15118
    EVSE_ISO15118_SetPaymentMethod(method);
#endif
}

void EVSE_ChargingProtocol_SetNFCAuthentication(uint8_t *cardUID, uint8_t size)
{
#if ENABLE_ISO15118
    EVSE_ISO15118_SetNFCAuthentication(cardUID, size);
#elif ENABLE_J1772
    EVSE_J1772_SetNFCAuthentication(cardUID, size);
#endif
}

#if EASYEVSE_EV
const vehicle_data_t *EV_ChargingProtocol_GetVehicleData(void)
{
#if ENABLE_ISO15118
    EV_ISO15118_GetVehicleData(&vehicle_data);
#elif ENABLE_J1772
    EV_J1772_GetVehicleData(&vehicle_data);
#endif /* ENABLE_ISO15118 */

    return &vehicle_data;
}

bool EV_ChargingProtocol_isCharging()
{
    bool bCharging = false;
#if ENABLE_ISO15118
    EV_ISO15118_isCharging(&bCharging);
#elif ENABLE_J1772
    EV_J1772_isCharging(&bCharging);
#endif /* ENABLE_ISO15118 */

    return bCharging;
}

void EV_ChargingProtocol_ChangeChargingDirection()
{
#if ENABLE_ISO15118
    EV_ISO15118_ChangeChargingDirection();
#endif
}

evse_return_status_t EV_ChargingProtocol_ResetBatteryLevel(battery_levels_t battery_level)
{
#if ENABLE_ISO15118
    EV_ISO15118_ResetBatteryLevel(battery_level);
    return SUCCESS;
#elif ENABLE_J1772
    EV_J1772_ResetBatteryLevel(battery_level);
    return SUCCESS;
#endif
}

evse_return_status_t EV_ChargingProtocol_StartCharging()
{
#if ENABLE_ISO15118
    EV_ISO15118_StartCharging();
    return SUCCESS;
#elif ENABLE_J1772
    EV_J1772_StartCharging();
    return NOT_IMPLEMENTED;
#endif
}

evse_return_status_t EV_ChargingProtocol_StopCharging()
{
#if ENABLE_ISO15118
    EV_ISO15118_StopCharging();
    return SUCCESS;
#elif ENABLE_J1772
    EV_J1772_StopCharging();
    return NOT_IMPLEMENTED;
#endif
}

evse_return_status_t EV_ChargingProtocol_SetProtocol(evse_charging_protocol_t protocol)
{
#if ENABLE_ISO15118
    EV_ISO15118_SetProtocol(protocol);
    return SUCCESS;
#elif ENABLE_J1772
    return NOT_IMPLEMENTED;
#endif
}

void EV_ChargingProtocol_SetAuthMethod(evse_auth_methods_t auth_method)
{
#if ENABLE_ISO15118
    EV_ISO15118_SetAuthMethod(auth_method);
#endif
}

evse_charging_protocol_t EV_ChargingProtocol_GetProtocol()
{
#if ENABLE_ISO15118
    EV_ISO15118_GetProtocol();
#elif ENABLE_J1772
    return EVSE_BasicCharging_J1772;
#endif
}

#endif /* EASYEVSE_EV */

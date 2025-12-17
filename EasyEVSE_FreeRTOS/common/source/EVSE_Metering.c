/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_lpuart.h"
#include "math.h"

#include "EVSE_Metering.h"
#include "EVSE_UI.h"
#include "EVSE_Utils.h"
#include "EVSE_ChargingProtocol.h"

#if ((ENABLE_SIGBOARD == 1) && (METER_SIGBOARD_LPUART_SHARED == 1))
#include "hal_uart_bridge.h"
#endif

#if ENABLE_OCPP
#include "EVSE_Ocpp.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#if PRINT_TELEMETRY_ENABLE
#define LOG_METERING(x) configPRINTF(x)
#else
#define LOG_METERING(x)
#endif

/* Ring buffer size (Unit: Byte). */
#define METER_RING_BUFFER_SIZE 256

/* Ms time between two metering update requests. */
#define METER_REQUEST_INTERVAL_MS 10000

/* Only support labels from [0] to [9] */
#define METER_LABEL_STRING_SIZE 3
#define METER_VALUE_STRING_SIZE 7

#define DOUBLE_DECIMAL_PLACE_DIGITS (2)

#define MAX_EVSE_CURRENT (16)

#define GRID_UPDATE_BIT        0x1
#define TARIFF_RATE_UPDATE_BIT 0x2
#define TARIFF_COST_UPDATE_BIT 0x4

#define MAJOR_VERSION_MULTIPLIER 10000U
#define MINOR_VERSION_MULTIPLIER 100U

/*! @brief Properties data structure. */
typedef struct _twin_property_data
{
    uint32_t GridPowerLimit; /* Grid controlled max power to deliver */
    uint32_t TariffRate;     /* Current energy tariff in use ie 0 : 1 : 2 */
    float TariffCost;        /* Energy Cost per KWh */
    uint8_t updated;         /* Indicates latest update:  bit 0: GridPowerLimit updated,
                              * bit 1: TariffRate updated, bit 2: TariffCost updated */
} twin_property_data_t;

/*******************************************************
 * Variables
 *******************************************************/
/* Buffer to store raw data received from KM */
char meterRingBuffer[METER_RING_BUFFER_SIZE];
/* Index of the memory to save new arrived data. */
volatile uint16_t rxIndex;

SemaphoreHandle_t xMutexUart;
static EventGroupHandle_t meter_event_flags = NULL;

static const char *EVSE_LABEL_strings[6] = {
    [METER_FIELD_IRMS] = "[1]",  [METER_FIELD_VRMS] = "[2]", [METER_FIELD_WH] = "[3]",
    [METER_FIELD_STATE] = "[4]", [METER_FIELD_Q] = "[5]",    [METER_FIELD_S] = "[6]"};
static const char *EVSE_CHARGE_STATE_strings[7] = {"Z", "A", "B", "C", "D", "E", "F"};

/* Device data */
static uint32_t evseRating   = 16; /* Current rating of charging point in Amps*/
static uint32_t chargingRate = 0;  /* Actual kw in use */
#if (ENABLE_SIGBOARD == 1)
static uint32_t SIGBRD_HW_version        = INVALID_SIGBOARD_HW_VERSION;
static uint32_t SIGBRD_SW_version_major  = 0;
static uint32_t SIGBRD_SW_version_minor  = 0;
static uint32_t SIGBRD_SW_version_bugfix = 0;
#else
static const uint8_t SIGBRD_HW_version        = 0;
static const uint8_t SIGBRD_SW_version_major  = 0;
static const uint8_t SIGBRD_SW_version_minor  = 0;
static const uint8_t SIGBRD_SW_version_bugfix = 0;
#endif /* ENABLE_SIGBOARD */

/* Various auxiliary variables */
static char timeBuff[12]    = "00H:00M:00S"; /* Time remaining in iso8601 format */
static char chargeStatus[2] = "D";           /* Charge state moves from A to D for charging and E..F for errors */
// static char isCharging[4]   = "NO";            /* 'YES', if the station is charging, 'NO' if it doesn't */
static char oldMeterState;

static meter_data_t meter_data;
static twin_property_data_t property_data;
static evse_data_t evse_data;

static bool request_data = false;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/**
 * Sends an update request to the metering board to get the latest measurement
 * every METER_REQUEST_INTERVAL_MS ms.
 */
static void meter_request_task(void *pvParameters);

/**
 * Parses the data received from the metering board and updates the
 * meter_data struct.
 */
static void meter_refresh_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/

static void EVSE_Meter_Data_Init(void)
{
    strcpy(evse_data.EVSE_Id, "10071856");                        /* Charging station SN as Hex string */
    evse_data.latitude         = 51.50263;                        /* Evse location latitude 51.50263*/
    evse_data.longitude        = -0.15087;                        /* Evse location longitude -0.15087*/
    evse_data.altitude         = 0.0;                             /* Evse location altitude */
    evse_data.stationFirmwareV = EVSE_Meter_GetFirmwareVersion(); /* RT1064 firmware version number */

    evse_data.EVSE_Temperature    = 25;
    evse_data.EVSE_GridPowerLimit = 32;
    evse_data.EVSE_Rating         = 16;
    evse_data.EVSE_Uptime         = 0;
    evse_data.EVSE_ChargeCost     = 0;     /* Charge cost */
    evse_data.EVSE_IsCharging     = false; /* True if car is charging */

    meter_data.irms             = 2.0;
    meter_data.vrms             = 230.0;
    meter_data.wh               = 890.0;
    meter_data.Q                = 890.0;
    meter_data.S                = 890.0;
    meter_data.EVSE_ChargeState = NULL;

    property_data.GridPowerLimit = 32;
    property_data.TariffRate     = 0;
    property_data.TariffCost     = 0.10;
    property_data.updated        = 0x0;

    EVSE_ChargingProtocol_SetMaxCurrent(MIN(evse_data.EVSE_Rating, evse_data.EVSE_GridPowerLimit));
}

void EVSE_Meter_Init(void)
{
#if ((ENABLE_SIGBOARD == 0) || (METER_SIGBOARD_LPUART_SHARED == 0))

    lpuart_config_t uart_config;
    /* Set up Meter UART
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kLPUART_ParityDisabled;
     * config.stopBitCount = kLPUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 0;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    LPUART_GetDefaultConfig(&uart_config);
    uart_config.baudRate_Bps = 115200U;
    uart_config.enableTx     = true;
    uart_config.enableRx     = true;
    LPUART_Init(METER_LPUART, &uart_config, METER_LPUART_CLK_FREQ);

    rxIndex = 0;

    /* Enable on RX interrupt. */
    LPUART_EnableInterrupts(METER_LPUART, kLPUART_RxDataRegFullInterruptEnable | kLPUART_RxOverrunInterruptEnable);
    EnableIRQWithPriority(METER_LPUART_IRQn, 3);

    /* Create MuteX to handle UART access between Host and Meter */
    xMutexUart = xSemaphoreCreateMutex();
    if (xMutexUart == NULL)
    {
        configPRINTF((error("Failed to create Mutex in EVSE_Meter! /r/n")));
        while (1)
            ;
    }
#else
    SIGBRD_UART_BridgeEntry();
#endif /* ((ENABLE_SIGBOARD == 0) || (METER_SIGBOARD_LPUART_SHARED == 0)) */

    /* Create the event flags group used by Meter UART  */
    meter_event_flags = xEventGroupCreate();
    if (meter_event_flags == NULL)
    {
        configPRINTF((error("Failed to create Meter event flags in EVSE_Meter!\r\n")));
        while (1)
            ;
    }

    if (xTaskCreate(meter_refresh_task, "Meter Refresh", APP_METER_REFRESH_STACK_SIZE, NULL, APP_METER_REFRESH_PRIORITY,
                    NULL) != pdPASS)
    {
        configPRINTF((error("Failed to create Meter Refresh Task!\r\n")));
        while (1)
            ;
    }

    if (xTaskCreate(meter_request_task, "Meter Request", APP_METER_REQUEST_STACK_SIZE, NULL, APP_METER_REQUEST_PRIORITY,
                    NULL) != pdPASS)
    {
        configPRINTF((error("Failed to create Meter Request Task!\r\n")));
        while (1)
            ;
    }

    EVSE_Meter_Data_Init();
}

/* If we enable the sigboard we need to use the uart bridge to get meter information */
#if ((ENABLE_SIGBOARD == 0) || (METER_SIGBOARD_LPUART_SHARED == 0))
void METER_LPUART_IRQHandler(void)
{
    char data;
    uint16_t tmprxIndex = rxIndex;
    volatile uint32_t uart_status;

    uart_status = LPUART_GetStatusFlags(METER_LPUART);

    /* Check overrun and clear if any. */
    if ((kLPUART_RxOverrunFlag)&uart_status)
    {
        LPUART_ClearStatusFlags(METER_LPUART, kLPUART_RxOverrunFlag);
        /* Set meter_event_flags to signal overrun meter_refresh thread.  */
        EVSE_Set_MeterEvent(METER_OVERRUN_FLAG);
    }

    /* Check if new data arrived. */
    if ((kLPUART_RxDataRegFullFlag)&uart_status)
    {
        data = LPUART_ReadByte(METER_LPUART);
        if (tmprxIndex < METER_RING_BUFFER_SIZE)
        {
            meterRingBuffer[rxIndex] = data;
            if (data == '\r')
            {
                /* Set NUL character at end of data for str function handling*/
                meterRingBuffer[rxIndex] = 0;
                /* Set meter_event_flags to wakeup meter_refresh thread.  */
                EVSE_Set_MeterEvent(METER_FLAG);
            }
            rxIndex++;
            /* Restrict sequence of rxIndex to 0..255 */
            rxIndex %= METER_RING_BUFFER_SIZE;
        }
    }
    SDK_ISR_EXIT_BARRIER;
}
#endif /* ((ENABLE_SIGBOARD == 0) || (METER_SIGBOARD_LPUART_SHARED == 0)) */

static void meter_send(uint8_t command_code)
{
#if ((ENABLE_SIGBOARD == 0) || (METER_SIGBOARD_LPUART_SHARED == 0))

    /* Get the UART MuteX with suspension. Wait forever. */
    if (xSemaphoreTake(xMutexUart, portMAX_DELAY) != pdTRUE)
    {
        configPRINTF((warning("Could not get MuteX for UART in request now\r\n")));
    }

    /* Send command to Meter board. */
    LPUART_WriteByte(METER_LPUART, command_code);

    /* Wait for UART Outputs to complete before entering reduced power mode. */
    while (!(LPUART_GetStatusFlags(METER_LPUART) & kLPUART_TxDataRegEmptyFlag))
    {
    }
    /* Release the UART MuteX. */
    if (xSemaphoreGive(xMutexUart) != pdTRUE)
    {
        configPRINTF((warning("Could not release MuteX for UART in request now\r\n")));
    }
#else
    SIGBRD_EVSE_UARTCommsProcess(command_code, 0, NULL);
#endif /* ((ENABLE_SIGBOARD == 0) || (METER_SIGBOARD_LPUART_SHARED == 0)) */
}

/* This function sends an update request to the metering board to obtain latest measurement values.
 * The request is sent every METER_REQUEST_INTERVAL_MS ms or when the UI asks for an
 * update on meter data.
 * The UART transfer to the metering is governed by MuteX as more than one function could
 * attempt access the UART at the same time.
 */
static void meter_request_task(void *pvParameters)
{
    //   uint8_t command_all = '0'; // ASCII '0' Request ALL meter data parameters
    //   uint8_t command_current = '1'; // Request iRMS
    //   uint8_t command_voltage = '2'; // Request vRMS
    //   uint8_t command_power = '3';   // Request Power
    //   uint8_t command_status = '4';  // Request STATE
    //   uint8_t command_status = 'A';  // Set STATE

    uint8_t command_all = '0';

    configPRINTF((info("Configured to ask for updates every %d ms"), METER_REQUEST_INTERVAL_MS));

    while (1)
    {
        /* Wait for the METER_READ event which is set by the UI */
        xEventGroupWaitBits(
            meter_event_flags,                         /* The event group handle. */
            METER_READ,                                /* The bit pattern the event group is waiting for. */
            pdTRUE,                                    /* The bit pattern will be cleared automatically. */
            pdFALSE,                                   /* Don't wait for both bits, either bit unblocks task. */
            pdMS_TO_TICKS(METER_REQUEST_INTERVAL_MS)); /* Block for a maximum of METER_REQUEST_INTERVAL_MS ms. */

        meter_send(command_all);
        request_data = true;

        configPRINTF((success("Requested meter data\r\n")));
    }
}

/* Process and assign meter measurements if new values have been received. */
static void meter_refresh_task(void *pvParameters)
{
    EventBits_t event_bits;

    /* For an ALL data request, the raw data received from the metering board has the following format:
     * "irms[1]vrms[2]power[3]evse_state[4]reactive_power[5]apparent_power[6]"
     *
     * "[1]" -> Current Data -IRMS
     * "[2]" -> Voltage Data -VRMS
     * "[3]" -> Power Data   -Wh
     * "[4]" -> EVSE State   -A,B,C,D,E,F
     * "[5]" -> Re-Active Power Value Q
     * "[6]" -> Apparent Power Value S
     *
     * This task waits for the data to be received from the metering board and then processes it.
     * */
    while (1)
    {
        /* Wait for meter_event_flags which is set by the METER_LPUART_IRQHandler only when full packet received from
         * Meter. */
        event_bits = xEventGroupWaitBits(
            meter_event_flags,                                    /* The event group handle. */
            METER_FLAG | METER_OVERRUN_FLAG | METER_REFRESH_DATA, /* The bit pattern the event group is waiting for. */
            pdTRUE,                                               /* The bit pattern will be cleared automatically. */
            pdFALSE,        /* Don't wait for both bits, either bit unblock task. */
            portMAX_DELAY); /* Block indefinitely to wait for the condition to be met. */

        if ((event_bits & METER_OVERRUN_FLAG) == METER_OVERRUN_FLAG)
        {
            configPRINTF((error("Error: Rx overrun detected!\r\n")));
            break;
        }

        if ((event_bits & METER_FLAG) == METER_FLAG)
        {
            configPRINTF((success("Received meter data\r\n ")));
            meter_data_t parsed_data = {0};
            uint32_t found_fields    = 0;
            /* Process data in meterRingBuffer from index 0 to rxIndex -1. */
            EVSE_Meter_ParseMeterReply(meterRingBuffer, rxIndex - 1, &parsed_data, &found_fields);
            EVSE_Meter_SetMeterData(&parsed_data, found_fields);
            rxIndex = 0;
        }

        if ((event_bits & METER_REFRESH_DATA) == METER_REFRESH_DATA)
        {
            if (oldMeterState != meter_data.meterState)
            {
                /* Send a session status event to the UI */
                EVSE_UI_SetEvent(EVSE_UI_ChargingSession);
            }

            if (request_data == true)
            {
                /* Announce telemetry data can be processed and sent out */
#if ENABLE_OCPP
                EVSE_OCPP_SetEvent(EVSE_TELEMETRY_READY_EVENT);
#endif
                request_data = false;
            }

            /* Trigger UI update */
            EVSE_UI_SetEvent(EVSE_UI_TelemetryData);
        }
    }
}

void EVSE_Set_MeterEvent(meter_events_t event)
{
    if (meter_event_flags != NULL)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        BaseType_t xResult                  = pdFAIL;
        uint32_t fromISR                    = __get_IPSR();

        if (fromISR)
        {
            xResult = xEventGroupSetBitsFromISR(meter_event_flags, event, &xHigherPriorityTaskWoken);
            if (xResult != pdFAIL)
            {
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
        }
        else
        {
            xEventGroupSetBits(meter_event_flags, event);
        }
    }
}

/*
 * Convert remaining time into an iso8601 formatted time string
 * 00H:00M:00S
 */
static void convert_time_remaining(uint32_t timeToCharge)
{
    uint32_t hhremain = 0; /* time remaining in hours */
    uint8_t mmremain  = 0; /* time remaining in minutes */
    uint8_t ssremain  = 0; /* time remaining in seconds */

    char HHstr[3] = "00";
    char MMstr[3] = "00";
    char SSstr[3] = "00";
    char *searchptr;

    ssremain = (timeToCharge % 60);
    mmremain = (timeToCharge / 60) % 60;
    hhremain = (timeToCharge / 3600);

    if (hhremain > 99)
    {
        hhremain = 99;
    }

    /* Convert int to str */
    itoa(hhremain, HHstr, 10);
    if (hhremain < 10)
    {
        HHstr[1] = HHstr[0];
        HHstr[0] = '0';
        HHstr[2] = '\0';
    }
    itoa(mmremain, MMstr, 10);
    if (mmremain < 10)
    {
        MMstr[1] = MMstr[0];
        MMstr[0] = '0';
        MMstr[2] = '\0';
    }

    itoa(ssremain, SSstr, 10);
    if (ssremain < 10)
    {
        SSstr[1] = SSstr[0];
        SSstr[0] = '0';
        SSstr[2] = '\0';
    }

    /* To build Hours, find character of interest in timeBuff and return its ptr. */
    searchptr = strchr(timeBuff, 'H');
    searchptr = searchptr - 2;
    strncpy(searchptr, HHstr, 2);

    /* To build Minutes, find character of interest in timeBuff and return its ptr. */
    searchptr = strchr(timeBuff, 'M');
    searchptr = searchptr - 2;
    strncpy(searchptr, MMstr, 2);

    /* To build Seconds, find character of interest in timeBuff and return its ptr. */
    searchptr = strchr(timeBuff, 'S');
    searchptr = searchptr - 2;
    strncpy(searchptr, SSstr, 2);

}

const bool EVSE_Meter_CheckValidMeterData(void)
{
    bool result = true;

    if (meter_data.irms > MAX_IRMS)
    {
        result = false;
    }
    else if (meter_data.vrms > MAX_VRMS)
    {
        result = false;
    }
    else if (meter_data.wh > meter_data.irms * meter_data.vrms)
    {
        result = false;
    }

    return result;
}

const meter_data_t *EVSE_Meter_GetMeterData(void)
{
    meter_data.EVSE_ChargeState = EVSE_ChargingProtocol_GetCPStateString();
    if (EVSE_Meter_CheckValidMeterData())
    {
        meter_data.S    = meter_data.irms * meter_data.vrms;
        double S_squre  = pow(meter_data.S, 2);
        double P_square = pow(meter_data.wh, 2);
        meter_data.Q    = sqrt(S_squre - P_square);
    }
    else
    {
        configPRINTF((error("Invalid meter data\r\n")));
    }

    return &meter_data;
}

const evse_data_t *EVSE_Meter_GetEVSEData(void)
{
    static evse_data_t evse_data_copy;
    evse_data_copy.EVSE_Temperature       = evse_data.EVSE_Temperature;
    evse_data_copy.EVSE_GridPowerLimit    = property_data.GridPowerLimit;
    evse_data_copy.EVSE_Rating            = evse_data.EVSE_Rating;
    evse_data_copy.EVSE_Uptime            = EVSE_GetSecondsSinceBoot();
    evse_data_copy.EVSE_ChargeCost        = evse_data.EVSE_ChargeCost;
    evse_data_copy.EVSE_IsCharging        = EVSE_ChargingProtocol_isCharging();
    evse_data_copy.EVSE_ChargingDirection = EVSE_ChargingProtocol_ChargingDirection();
#if ENABLE_SIGBOARD
    if (SIGBRD_HW_version == INVALID_SIGBOARD_HW_VERSION)
    {
        SIGBRD_GetGetHWVersion(&SIGBRD_HW_version);
    }
    if (SIGBRD_SW_version_major == 0)
    {
        SIGBRD_GetSWVersion(&SIGBRD_SW_version_major, &SIGBRD_SW_version_minor, &SIGBRD_SW_version_bugfix);
    }
#endif /* ENABLE_SIGBOARD */
    evse_data_copy.SIGBRD_HW_version        = SIGBRD_HW_version;
    evse_data_copy.SIGBRD_SW_version_major  = SIGBRD_SW_version_major;
    evse_data_copy.SIGBRD_SW_version_minor  = SIGBRD_SW_version_minor;
    evse_data_copy.SIGBRD_SW_version_bugfix = SIGBRD_SW_version_bugfix;
    strcpy(evse_data_copy.EVSE_Id, evse_data.EVSE_Id);

    return &evse_data_copy;
}

uint32_t EVSE_Meter_GetFirmwareVersion(void)
{
    return (FIRMWARE_VERSION_MAJOR * MAJOR_VERSION_MULTIPLIER + FIRMWARE_VERSION_MINOR * MINOR_VERSION_MULTIPLIER +
            FIRMWARE_VERSION_HOTFIX);
}

void EVSE_Meter_SetMeterData(const meter_data_t *new_meter_data, uint32_t fields_to_update)
{
    if (new_meter_data == NULL)
    {
        return;
    }

    if ((fields_to_update & (1 << METER_FIELD_IRMS)) == (1 << METER_FIELD_IRMS))
    {
        meter_data.irms = new_meter_data->irms;
    }
    if ((fields_to_update & (1 << METER_FIELD_VRMS)) == (1 << METER_FIELD_VRMS))
    {
        meter_data.vrms = new_meter_data->vrms;
    }
    if ((fields_to_update & (1 << METER_FIELD_WH)) == (1 << METER_FIELD_WH))
    {
        meter_data.wh = new_meter_data->wh;
    }
    if ((fields_to_update & (1 << METER_FIELD_STATE)) == (1 << METER_FIELD_STATE))
    {
        oldMeterState               = meter_data.meterState;
        meter_data.meterState       = new_meter_data->meterState;
        meter_data.EVSE_ChargeState = new_meter_data->EVSE_ChargeState;
    }
    if ((fields_to_update & (1 << METER_FIELD_Q)) == (1 << METER_FIELD_Q))
    {
        meter_data.Q = new_meter_data->Q;
    }
    if ((fields_to_update & (1 << METER_FIELD_S)) == (1 << METER_FIELD_S))
    {
        meter_data.S = new_meter_data->S;
    }

    if (fields_to_update)
    {
        EVSE_Set_MeterEvent(METER_REFRESH_DATA);
    }
}

void EVSE_Meter_ParseMeterReply(uint8_t *meter_reply,
                                uint32_t reply_size,
                                meter_data_t *parsed_meter_data,
                                uint32_t *found_fields)
{
    uint32_t chartoprocess                       = reply_size;
    uint32_t len                                 = 0;
    float ret                                    = 0.00;
    char data_label[METER_LABEL_STRING_SIZE + 1] = "";
    char *ptr                                    = NULL;
    char *str_ptr                                = NULL;

    if ((meter_reply == NULL) || (found_fields == NULL) || (parsed_meter_data == NULL))
    {
        return;
    }
    else
    {
        *found_fields = 0;
        memset(parsed_meter_data, 0, sizeof(meter_data_t));
    }

    /* Set pointer to start of receiver array. */
    str_ptr = &meter_reply[0];

    while (chartoprocess)
    {
        /* Extract the meter value (in float). */
        ret = strtod(str_ptr, &ptr);
        len = ptr - str_ptr;

        /* Extract the meter data label [1], [2] etc. */
        strncpy(data_label, ptr, METER_LABEL_STRING_SIZE);

        /* Save the meter value in both float and string formats. */
        if (strcmp(EVSE_LABEL_strings[METER_FIELD_IRMS], data_label) == 0)
        {
            parsed_meter_data->irms = ret;
            *found_fields |= 1 << METER_FIELD_IRMS;
        }
        else if (strcmp(EVSE_LABEL_strings[METER_FIELD_VRMS], data_label) == 0)
        {
            parsed_meter_data->vrms = ret;
            *found_fields |= 1 << METER_FIELD_VRMS;
        }
        else if (strcmp(EVSE_LABEL_strings[METER_FIELD_WH], data_label) == 0)
        {
            parsed_meter_data->wh = ret;
            *found_fields |= 1 << METER_FIELD_WH;
        }
        else if (strcmp(EVSE_LABEL_strings[METER_FIELD_STATE], data_label) == 0)
        {
            parsed_meter_data->meterState = ret / 1;
            if ((parsed_meter_data->meterState < 1) || (parsed_meter_data->meterState > 6))
            {
                parsed_meter_data->EVSE_ChargeState = EVSE_CHARGE_STATE_strings[0];
                parsed_meter_data->meterState       = 0;
            }
            else
            {
                parsed_meter_data->EVSE_ChargeState = EVSE_CHARGE_STATE_strings[parsed_meter_data->meterState];
            }

            *found_fields |= 1 << METER_FIELD_STATE;
        }
        else if (strcmp(EVSE_LABEL_strings[METER_FIELD_Q], data_label) == 0)
        {
            parsed_meter_data->Q = ret;
            *found_fields |= 1 << METER_FIELD_Q;
        }
        else if (strcmp(EVSE_LABEL_strings[METER_FIELD_S], data_label) == 0)
        {
            parsed_meter_data->S = ret;
            *found_fields |= 1 << METER_FIELD_S;
        }

        /* Point to the next meter value. */
        ptr     = ptr + METER_LABEL_STRING_SIZE;
        str_ptr = ptr;
        chartoprocess -= (len + METER_LABEL_STRING_SIZE);

        if ((chartoprocess >= reply_size))
        {
            chartoprocess = 0;
            /** pointer usage underflowed this variable */
            configPRINTF((error("Error processing meter data")));
        }
    }
}

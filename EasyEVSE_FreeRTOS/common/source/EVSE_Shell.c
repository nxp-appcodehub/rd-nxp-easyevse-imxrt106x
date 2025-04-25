/*
 * Copyright 2024-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#if (ENABLE_SHELL == 1)

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

#include "fsl_component_serial_manager.h"
#include "fsl_component_serial_port_usb.h"
#include "virtual_com.h"
#include "fsl_shell.h"

#include "EVSE_Shell.h"
#include "EVSE_ConnectivityConfig.h"

#if ENABLE_WIFI
#include "EVSE_Connectivity.h" //for wifi credentials
#endif                         /* ENABLE_WIFI */

#if ENABLE_SE
#include "se05x_BinaryFile.h"
#include "EVSE_Secure_Element.h"
#endif /* ENABLE_SE */

#if ENABLE_SIGBOARD
#include "EVSE_Metering.h"
#endif /* ENABLE_SIGBOARD */

#if ENABLE_ISO15118
#include <ISO15118.h>
extern void stxV2GSetNFCDetected(uint8_t *cardUID, uint8_t size);
extern void stxV2GApplExt_EVSESetPaymentMethod(vehicle_auth_methods_t payment_method);
#endif /* ENABLE_ISO15118 */

#define EVSE_SERIAL_MANAGER_RECEIVE_BUFFER_LEN 2048U

/* Maximum number of arguments for a command */
#define EVSE_ARGC_MAX 5
/* Maximum argv string size */
#define EVSE_ARGV_STR_MAX 64

#if ENABLE_ISO15118
#define AUTH_CARD_UID "AUTH_FROM_CLI"
#endif /* ENABLE_ISO15118 */

#define CONTROLLER_ID kSerialManager_UsbControllerEhci0

extern void USB_DeviceClockInit(void);

extern bool deviceIDReady;

static shell_status_t EVSE_version_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_wifi_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_se05x_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_auth_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_payment_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_cloud_info_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_charging_handler(shell_handle_t shellHandle, int32_t argc, char **argv);

/*******************************************************************************
 * Variables
 ******************************************************************************/
SHELL_COMMAND_DEFINE(version,
                     "\r\n\"version\": Print SW version of the EVSE and SW/HW version of the SIGBOARD\r\n",
                     EVSE_version_handler,
                     0);

#if ENABLE_WIFI
SHELL_COMMAND_DEFINE(wifi,
                     "\r\n\"wifi\": Setup WiFi Network Credentials:\r\n"
                     "                 Usage:\r\n"
                     "                     wifi set SSID PASSWORD\r\n"
                     "                 Parameters:\r\n"
                     "                 SSID:       The name of the WiFi network [max 32 characters]\r\n"
                     "                 PASSWORD:   The password for the WiFi network [max 64 characters]\r\n"
                     "                 ssid and password should not contain double quotes\r\n"
                     "                 ssid and password must be placed between double quotes if they contain spaces "
                     "(e.g \"password 123\")\r\n"
                     "              Erase WiFi Network Credentials from flash:\r\n"
                     "                 Usage:\r\n"
                     "                     wifi erase\r\n"
                     "              Print WiFi Network Credentials from flash:\r\n"
                     "                 Usage:\r\n"
                     "                     wifi print\r\n",
                     /* if more than the specified number of parameters, the rest of the parameters will be ignored */
                     EVSE_wifi_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
#endif /* ENABLE_WIFI */

#if ENABLE_SE
SHELL_COMMAND_DEFINE(se05x,
                     "\r\n\"se05x\": Erase content of Secure Element:\r\n"
                     "                 Usage:\r\n"
                     "                     se05x erase cloud_certificate/hostname/cpokey\r\n",
                     /* if more than the specified number of parameters, the rest of the parameters will be ignored */
                     EVSE_se05x_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);
#endif /* ENABLE_SE */

#if ENABLE_ISO15118
SHELL_COMMAND_DEFINE(auth,
                     "\r\n\"auth\": Authenticate the EV without tapping the NFC card:\r\n"
                     "                 Usage:\r\n"
                     "                     auth true\r\n",
                     /* if more than the specified number of parameters, the rest of the parameters will be ignored */
                     EVSE_auth_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);

SHELL_COMMAND_DEFINE(iso_payment,
                     "\r\n\"iso_payment\": Set iso15118 payment method:\r\n"
                     "                 Usage:\r\n"
                     "                     iso_payment eim - for EIM \r\n"
                     "                     iso_payment pnc - for PNC \r\n",
                     /* if more than the specified number of parameters, the rest of the parameters will be ignored */
                     EVSE_payment_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);

#endif /* ENABLE_ISO15118 */

SHELL_COMMAND_DEFINE(cloud_info,
                     "\r\n\"cloud_info\": Print Cloud connection info\r\n"
                     "                 Usage:\r\n"
                     "                     cloud_info\r\n",
                     EVSE_cloud_info_handler,
                     0);

SHELL_COMMAND_DEFINE(charging,
                     "\r\n\"charging stop\": Stop the charging operation"
                     "\r\n\"         start\": Start the charging operation\r\n",
                     EVSE_charging_handler,
                     SHELL_IGNORE_PARAMETER_COUNT);

static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE];
static shell_handle_t s_shellHandle = NULL;

static uint8_t s_serialHandleBuffer[SERIAL_MANAGER_HANDLE_SIZE];
static serial_handle_t s_serialHandle = &s_serialHandleBuffer[0];

static uint8_t ucSampleIotHubDeviceId[EVSE_DEVICEID_MAX_BUFFER];

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_readRingBuffer[EVSE_SERIAL_MANAGER_RECEIVE_BUFFER_LEN], 32);
#endif

extern serial_handle_t g_serialHandle;

static EventGroupHandle_t s_shell_event;

static uint8_t evse_argc                                    = 0;
static char evse_argv[EVSE_ARGC_MAX + 1][EVSE_ARGV_STR_MAX] = {0};

/*******************************************************************************
 * Code
 ******************************************************************************/

int EVSE_Shell_Printf(const char *formatString, ...)
{
    /* Check format string */

    va_list args;
    int status;

    va_start(args, formatString);

    /* Check if shell initialized */
    if (s_shellHandle != NULL)
    {
        status = SHELL_PrintfVaList(s_shellHandle, formatString, args);
        va_end(args);
        return status;
    }

    va_end(args);
    return -1;
}

#if ENABLE_WIFI
static shell_status_t EVSE_wifi_cmd_do(void)
{
    if ((evse_argc == 1) || (evse_argc > 4))
    {
        return kStatus_SHELL_Error;
    }

    if (strcmp(evse_argv[1], "set") == 0)
    {
        evse_wifi_cred_t evse_wifi_credentials = {0};
        char *wifi_ssid                        = NULL;
        char *wifi_pass                        = NULL;
        uint32_t ssid_len                      = 0;
        uint32_t pass_len                      = 0;

        wifi_ssid = evse_argv[2];
        ssid_len  = strlen(wifi_ssid);
        if (ssid_len > sizeof(evse_wifi_credentials.ssid.name))
        {
            return kStatus_SHELL_Error;
        }
        memcpy(evse_wifi_credentials.ssid.name, wifi_ssid, ssid_len);
        evse_wifi_credentials.ssid.length = ssid_len;

        if (evse_argc == 4)
        {
            wifi_pass = evse_argv[3];
            pass_len  = strlen(wifi_pass);
            if (pass_len > sizeof(evse_wifi_credentials.pass.name))
            {
                return kStatus_SHELL_Error;
            }

            memcpy(evse_wifi_credentials.pass.name, wifi_pass, pass_len);
            evse_wifi_credentials.pass.length = pass_len;

            if (EVSE_Connectivity_Wifi_Credentials_Flash_Save(&evse_wifi_credentials) == FLASH_FS_OK)
            {
                SHELL_Printf(s_shellHandle, "Wifi credentials saved\r\n");
                NVIC_SystemReset();
            }
            else
            {
                return kStatus_SHELL_Error;
            }
        }
    }
    else if (strcmp(evse_argv[1], "erase") == 0)
    {
        if (EVSE_Connectivity_Wifi_Credentials_Flash_Erase() == FLASH_FS_OK)
        {
            SHELL_Printf(s_shellHandle, "Wifi credentials erased from flash\r\n");
        }
        else
        {
            return kStatus_SHELL_Error;
        }
    }
    else if (strcmp(evse_argv[1], "print") == 0)
    {
        evse_wifi_cred_t evse_wifi_credentials = {0};
        if (EVSE_Connectivity_Wifi_Credentials_Flash_Read(&evse_wifi_credentials) == FLASH_FS_OK)
        {
            SHELL_Printf(s_shellHandle, "ssid: %s\r\n", evse_wifi_credentials.ssid.name);
            SHELL_Printf(s_shellHandle, "password: %s\r\n", evse_wifi_credentials.pass.name);
        }
        else
        {
            return kStatus_SHELL_Error;
        }
    }
    else
    {
        SHELL_Printf(s_shellHandle, "\r\nIncorrect command parameter(s)\r\n");
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t EVSE_wifi_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult                  = pdFAIL;

    if (argc > EVSE_ARGC_MAX)
    {
        return kStatus_SHELL_Error;
    }

    evse_argc = argc;
    for (uint8_t i = 0; i < argc; i++)
    {
        strncpy(evse_argv[i], argv[i], EVSE_ARGV_STR_MAX);
        evse_argv[i][EVSE_ARGV_STR_MAX - 1] = '\0';
    }

    xResult = xEventGroupSetBitsFromISR(s_shell_event, EVSE_SHELL_WIFI_EVENT, &xHigherPriorityTaskWoken);
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return kStatus_SHELL_Success;
}
#endif /* ENABLE_WIFI */

#if ENABLE_SE
static shell_status_t EVSE_se05x_cmd_do(void)
{
    if ((evse_argc == 1) || (evse_argc > 3))
    {
        return kStatus_SHELL_Error;
    }

    if (strcmp(evse_argv[1], "erase") == 0)
    {
        EVSE_SE050_Open_Session();
        if (strcmp(evse_argv[2], "key") == 0)
        {
            SHELL_Printf(s_shellHandle, "Deprecated. Use command 'se05x erase cloud_certificate' instead.\r\n");
        }
        else if (strcmp(evse_argv[2], "cloud_certificate") == 0)
        {
            if (EVSE_SE050_Delete_Cloud_Certificate() == kStatus_Certificates_Success)
            {
                SHELL_Printf(s_shellHandle, "Successfully deleted certificate and corresponding key\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Delete failed. Certificate missing or unattainable.\r\n");
            }
        }
        else if (strcmp(evse_argv[2], "hostname") == 0)
        {
            if (EVSE_SE050_Delete_ServerHostname() == kStatus_Hostname_Success)
            {
                SHELL_Printf(s_shellHandle, "Successfully deleted hostname\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Delete failed. Hostname missing or unattainable.\r\n");
            }
        }
        else if (strcmp(evse_argv[2], "cpokey") == 0)
        {
#if ENABLE_ISO15118
            if (EVSE_SE050_Delete_CPOKey() == kStatus_CPOKey_Success)
            {
                SHELL_Printf(s_shellHandle, "Successfully deleted CPO key\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Delete failed. CPO key missing or unattainable.\r\n");
            }
#else
            SHELL_Printf(s_shellHandle, "ISO15118 not enabled. You can't access the CPO key.\r\n");
#endif /* ENABLE_ISO15118 */
        }
        else
        {
            SHELL_Printf(s_shellHandle, "\r\nIncorrect command parameter(s)\r\n");
            EVSE_SE050_Close_Session();
            return kStatus_SHELL_Error;
        }
        EVSE_SE050_Close_Session();
    }
    else
    {
        SHELL_Printf(s_shellHandle, "\r\nIncorrect command parameter(s)\r\n");
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t EVSE_se05x_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult                  = pdFAIL;

    if (argc > EVSE_ARGC_MAX)
    {
        return kStatus_SHELL_Error;
    }

    evse_argc = argc;
    for (uint8_t i = 0; i < argc; i++)
    {
        strncpy(evse_argv[i], argv[i], EVSE_ARGV_STR_MAX);
        evse_argv[i][EVSE_ARGV_STR_MAX - 1] = '\0';
    }

    xResult = xEventGroupSetBitsFromISR(s_shell_event, EVSE_SHELL_SE05X_EVENT, &xHigherPriorityTaskWoken);
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    return kStatus_SHELL_Success;
}

#endif /* ENABLE_SE */

#if ENABLE_ISO15118
static shell_status_t EVSE_payment_cmd_do(void)
{
    if (evse_argc != 2)
    {
        return kStatus_SHELL_Error;
    }

    if (strcmp(evse_argv[1], "eim") == 0)
    {
        stxV2GApplExt_EVSESetPaymentMethod(VehicleAuth_EIM);
    }
    else if (strcmp(evse_argv[1], "pnc") == 0)
    {
        stxV2GApplExt_EVSESetPaymentMethod(VehicleAuth_PnC);
    }
    else
    {
        return kStatus_SHELL_Error;
    }

    SHELL_Printf(s_shellHandle, "Set payment method %s\r\n", evse_argv[1]);

    return kStatus_SHELL_Success;
}

static shell_status_t EVSE_auth_cmd_do(void)
{
    if ((evse_argc == 1) || (evse_argc > 2))
    {
        return kStatus_SHELL_Error;
    }

    if (strcmp(evse_argv[1], "true") == 0)
    {
        stxV2GSetNFCDetected(AUTH_CARD_UID, sizeof(AUTH_CARD_UID));
        SHELL_Printf(s_shellHandle, "Set authenticated from cli\r\n");
    }
    else
    {
        return kStatus_SHELL_Error;
    }

    return kStatus_SHELL_Success;
}

static shell_status_t EVSE_payment_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult                  = pdFAIL;

    evse_argc = argc;
    for (uint8_t i = 0; i < argc; i++)
    {
        strncpy(evse_argv[i], argv[i], EVSE_ARGV_STR_MAX);
        evse_argv[i][EVSE_ARGV_STR_MAX - 1] = '\0';
    }

    xResult = xEventGroupSetBitsFromISR(s_shell_event, EVSE_SHELL_PAYMENT_EVENT, &xHigherPriorityTaskWoken);
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return kStatus_SHELL_Success;
}

static shell_status_t EVSE_auth_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult                  = pdFAIL;

    evse_argc = argc;
    for (uint8_t i = 0; i < argc; i++)
    {
        strncpy(evse_argv[i], argv[i], EVSE_ARGV_STR_MAX);
        evse_argv[i][EVSE_ARGV_STR_MAX - 1] = '\0';
    }

    xResult = xEventGroupSetBitsFromISR(s_shell_event, EVSE_SHELL_AUTH_EVENT, &xHigherPriorityTaskWoken);
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return kStatus_SHELL_Success;
}
#endif /* ENABLE_ISO15118 */

static void EVSE_version_cmd_do(void)
{
    SHELL_Printf(s_shellHandle, "EVSE SW: v%d.%d.%d\r\n", FIRMWARE_VERSION_MAJOR, FIRMWARE_VERSION_MINOR,
                 FIRMWARE_VERSION_HOTFIX);

#if ENABLE_SIGBOARD
    const evse_data_t *evse_data = EVSE_Meter_GetEVSEData();
    SHELL_Printf(s_shellHandle, "SIGBOARD HW: v%d\r\n", evse_data->SIGBRD_HW_version);
    SHELL_Printf(s_shellHandle, "SIGBOARD SW: v%d.%d.%d\r\n", evse_data->SIGBRD_SW_version_major,
                 evse_data->SIGBRD_SW_version_minor, evse_data->SIGBRD_SW_version_bugfix);
#else
    SHELL_Printf(s_shellHandle, "SIGBOARD HW: - \r\n");
    SHELL_Printf(s_shellHandle, "SIGBOARD SW: - \r\n");
#endif /* ENABLE_SIGBOARD */
}

static shell_status_t EVSE_version_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult                  = pdFAIL;

    xResult = xEventGroupSetBitsFromISR(s_shell_event, EVSE_SHELL_VERSION_EVENT, &xHigherPriorityTaskWoken);
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return kStatus_SHELL_Success;
}

static void EVSE_cloud_info_cmd_do(void)
{
    SHELL_Printf(s_shellHandle, "Authentication method: ");
#if (EVSE_SAS_AUTH == 1)
    SHELL_Printf(s_shellHandle, "Symmetric key authentication [EVSE_SAS_AUTH]\n\r");
#elif (EVSE_X509_AUTH == 1)
    SHELL_Printf(s_shellHandle, "Certificate stored in i.MX RT106x Flash [EVSE_X509_AUTH]\n\r");
#elif (EVSE_X509_SE050_AUTH == 1)
    SHELL_Printf(s_shellHandle, "Certificate stored securely in SE050 Secure Element [EVSE_X509_SE050_AUTH]\n\r");
#else
    SHELL_Printf(s_shellHandle, "Not selected\n\r");
#endif

    SHELL_Printf(s_shellHandle, "Device ID: ");
#if (EVSE_SAS_AUTH == 1)
    SHELL_Printf(s_shellHandle, "%s\r\n", DEVICE_ID);
#else
    if (deviceIDReady != true)
    {
        SHELL_Printf(s_shellHandle, "is not ready\n\r");
    }
    else
    {
#if (EVSE_X509_SE050_AUTH == 1)
        EVSE_SE050_Open_Session();
#endif
        if (EVSE_SE050_Get_DeviceIDFromCertificate((uint8_t *)&ucSampleIotHubDeviceId[0], EVSE_DEVICEID_MAX_BUFFER) ==
            kStatus_Certificates_Success)
        {
            uint8_t index_dec                           = 0;
            uint8_t index_str                           = 0;
            char deviceID_str[EVSE_DEVICEID_MAX_BUFFER] = {0};

            while (ucSampleIotHubDeviceId[index_dec])
            {
                index_str += sprintf(&deviceID_str[index_str], "%c", ucSampleIotHubDeviceId[index_dec++]);
            }
            SHELL_Printf(s_shellHandle, "%s\n\r", deviceID_str);
        }
        else
        {
            SHELL_Printf(s_shellHandle, "could not be retrieved\n\r");
        }
#if (EVSE_X509_SE050_AUTH == 1)
        EVSE_SE050_Close_Session();
#endif
    }

#endif

    SHELL_Printf(s_shellHandle, "Device ID scope: ");
#if (EVSE_DPS == 1)
    SHELL_Printf(s_shellHandle, "%s\r\n", ID_SCOPE);
#else
    SHELL_Printf(s_shellHandle, "not used\r\n");
#endif

#if (EVSE_EDGELOCK_AGENT == 1)
    SHELL_Printf(s_shellHandle, "Edgelock2Go hostname: %s \r\n", EDGELOCK2GO_HOSTNAME);
    SHELL_Printf(s_shellHandle, "Edgelock2Go port: %d \r\n", EDGELOCK2GO_PORT);
#else
    SHELL_Printf(s_shellHandle, "Edgelock2Go not used\r\n");
#endif
    SHELL_Printf(s_shellHandle, "DPS enabled: ");
#if (EVSE_DPS == 1)
    SHELL_Printf(s_shellHandle, "yes\r\n");
#else
    SHELL_Printf(s_shellHandle, "no\r\n");
#endif
}

static shell_status_t EVSE_cloud_info_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult                  = pdFAIL;

    xResult = xEventGroupSetBitsFromISR(s_shell_event, EVSE_SHELL_CLOUD_INFO_EVENT, &xHigherPriorityTaskWoken);
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return kStatus_SHELL_Success;
}

static void EVSE_charging_cmd_do(void)
{
    bool result = false;

#if ENABLE_ISO15118
    if (strcmp(evse_argv[1], "stop") == 0)
    {
        /* Do stop charging */
        stxV2GApplExt_EVSEStopCharging(&result);
    }
    else if (strcmp(evse_argv[1], "start") == 0)
    {
        /* Do start charging */
        stxV2GApplExt_EVSEStartCharging(&result);
    }
#endif /* ENABLE_ISO15118 */
}

static shell_status_t EVSE_charging_handler(shell_handle_t shellHandle, int32_t argc, char **argv)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    BaseType_t xResult                  = pdFAIL;

    shell_status_t status = kStatus_SHELL_Success;

    if (argc != 2)
    {
        status = kStatus_SHELL_Error;
    }

    if (status == kStatus_SHELL_Success)
    {
        if ((strcmp(argv[1], "stop") != 0) && (strcmp(argv[1], "start") != 0))
        {
            return kStatus_SHELL_Error;
        }

        evse_argc = argc;
        for (uint8_t i = 0; i < argc; i++)
        {
            strncpy(evse_argv[i], argv[i], EVSE_ARGV_STR_MAX);
            evse_argv[i][EVSE_ARGV_STR_MAX - 1] = '\0';
        }
    }

    xResult = xEventGroupSetBitsFromISR(s_shell_event, EVSE_SHELL_CHARGING_EVENT, &xHigherPriorityTaskWoken);
    if (xResult != pdFAIL)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    return kStatus_SHELL_Success;
}

void EVSE_Shell_Task(void *arg)
{
    EventBits_t shellBits = 0U;

    configPRINTF(("EVSE shell ready to receive commands"));

    SHELL_Printf(s_shellHandle, "Welcome to EasyEVSE! \r\n");
    SHELL_Printf(s_shellHandle, "EVSE_SHELL>> ");

    while (1)
    {
        shellBits = xEventGroupWaitBits(s_shell_event,         /* The event group being tested. */
                                        EVSE_SHELL_ALL_EVENTS, /* The bits within the event group to wait for. */
                                        pdTRUE, pdFALSE,       /* Don't wait for all bits, either bit will do. */
                                        portMAX_DELAY);        /* Wait until event */

        if (shellBits & (EVSE_SHELL_VERSION_EVENT))
        {
            EVSE_version_cmd_do();
        }

#if ENABLE_WIFI
        if (shellBits & (EVSE_SHELL_WIFI_EVENT))
        {
            EVSE_wifi_cmd_do();
        }
#endif /* ENABLE_WIFI */

#if ENABLE_SE
        if (shellBits & (EVSE_SHELL_SE05X_EVENT))
        {
            EVSE_se05x_cmd_do();
        }
#endif /* ENABLE_SE */

#if ENABLE_ISO15118
        if (shellBits & (EVSE_SHELL_AUTH_EVENT))
        {
            EVSE_auth_cmd_do();
        }
        else if (shellBits & (EVSE_SHELL_PAYMENT_EVENT))
        {
            EVSE_payment_cmd_do();
        }
#endif /* ENABLE_ISO15118 */
        if (shellBits & (EVSE_SHELL_CLOUD_INFO_EVENT))
        {
            EVSE_cloud_info_cmd_do();
        }

        if (shellBits & (EVSE_SHELL_CHARGING_EVENT))
        {
            EVSE_charging_cmd_do();
        }
    }

    /* Shouldn't reach here, but to be sure */
    vTaskDelete(NULL);
}

void EVSE_Shell_Init(void)
{
    /* Init Serial Manager */
    serial_manager_config_t serialConfig;

    serial_port_usb_cdc_config_t usbCdcConfig = {
        .controllerIndex = (serial_port_usb_cdc_controller_index_t)CONTROLLER_ID,
    };

    USB_DeviceClockInit();

    serialConfig.type       = kSerialPort_UsbCdc;
    serialConfig.portConfig = &usbCdcConfig;
    serialConfig.blockType  = kSerialManager_Blocking;
#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
    serialConfig.ringBuffer     = &s_readRingBuffer[0];
    serialConfig.ringBufferSize = EVSE_SERIAL_MANAGER_RECEIVE_BUFFER_LEN;
    serialConfig.blockType      = kSerialManager_NonBlocking;
#endif
    if (SerialManager_Init(s_serialHandle, &serialConfig) != kStatus_SerialManager_Success)
    {
        configPRINTF((error(" %s Failed to init serial manager "), __FUNCTION__));
        while (1)
            ;
    }

    /* Init EVSE Shell */
    s_shellHandle = &s_shellHandleBuffer[0];

    SHELL_Init(s_shellHandle, s_serialHandle, "EVSE_SHELL>> ");
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(version));
#if ENABLE_WIFI
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(wifi));
#endif /* ENABLE_WIFI */
#if ENABLE_SE
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(se05x));
#endif /* ENABLE_SE */
#if ENABLE_ISO15118
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(auth));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(iso_payment));
#endif /* ENABLE_ISO15118 */
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(cloud_info));
    SHELL_RegisterCommand(s_shellHandle, SHELL_COMMAND(charging));

    s_shell_event = xEventGroupCreate();
    if (s_shell_event == NULL)
    {
        configPRINTF((error(" %s Failed to create event group "), __FUNCTION__));
        while (1)
            ;
    }

    if (xTaskCreate(EVSE_Shell_Task, "Shell Task", APP_SHELL_STACK_SIZE, NULL, APP_SHELL_PRIORITY, NULL) != pdPASS)
    {
        configPRINTF((error(" %s Failed to create task "), __FUNCTION__));
        while (1)
            ;
    }
}

#endif /* ENABLE_SHELL */

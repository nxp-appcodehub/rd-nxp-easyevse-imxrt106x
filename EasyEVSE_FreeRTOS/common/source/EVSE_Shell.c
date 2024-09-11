/*
 * Copyright 2024 NXP
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

#include "virtual_com.h"

#if ENABLE_WIFI
#include "EVSE_Connectivity.h" //for wifi credentials
#endif                         /* ENABLE_WIFI */

#if ENABLE_SE
#include "se05x_BinaryFile.h"
#include "EVSE_Secure_Element.h"
#endif /* ENABLE_WIFI */

#if ENABLE_SIGBOARD
#include "EVSE_Metering.h"
#endif /* ENABLE_SIGBOARD */

#if ENABLE_ISO15118
#include "ISO15118-2.h"
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

static shell_status_t EVSE_version_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_wifi_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_se05x_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_auth_handler(shell_handle_t shellHandle, int32_t argc, char **argv);
static shell_status_t EVSE_payment_handler(shell_handle_t shellHandle, int32_t argc, char **argv);

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
                     "                     se05x erase key/certificate/hostname\r\n",
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

static uint8_t s_shellHandleBuffer[SHELL_HANDLE_SIZE];
static shell_handle_t s_shellHandle;

static uint8_t s_serialHandleBuffer[SERIAL_MANAGER_HANDLE_SIZE];
static serial_handle_t s_serialHandle = &s_serialHandleBuffer[0];

#if (defined(SERIAL_MANAGER_NON_BLOCKING_MODE) && (SERIAL_MANAGER_NON_BLOCKING_MODE > 0U))
AT_NONCACHEABLE_SECTION_ALIGN(uint8_t s_readRingBuffer[EVSE_SERIAL_MANAGER_RECEIVE_BUFFER_LEN], 32);
#endif

extern serial_handle_t g_serialHandle;

static EventGroupHandle_t s_shell_event;

static uint8_t evse_argc                                    = 0;
static char evse_argv[EVSE_ARGC_MAX + 1][EVSE_ARGV_STR_MAX] = {0};

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
            if (se05x_DeleteBinaryFile(AZURE_IOT_KEY_INDEX_SM) == kStatus_SSS_Success)
            {
                SHELL_Printf(s_shellHandle, "Successfully deleted key\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Delete failed. Key missing or unattainable.\r\n");
            }
        }
        else if (strcmp(evse_argv[2], "certificate") == 0)
        {
            if (se05x_DeleteBinaryFile(AZURE_IOT_CLIENT_CERT_INDEX_SM) == kStatus_SSS_Success)
            {
                SHELL_Printf(s_shellHandle, "Successfully deleted certificate\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Delete failed. Certificate missing or unattainable.\r\n");
            }
        }
        else if (strcmp(evse_argv[2], "hostname") == 0)
        {
            if (se05x_DeleteBinaryFile(AZURE_IOT_HOSTNAME_INDEX_SM) == kStatus_SSS_Success)
            {
                SHELL_Printf(s_shellHandle, "Successfully deleted hostname\r\n");
            }
            else
            {
                SHELL_Printf(s_shellHandle, "Delete failed. Hostname missing or unattainable.\r\n");
            }
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

/* Copyright (c) Microsoft Corporation.
/* Copyright 2023-2024 NXP

 * Licensed under the MIT License. */

/* Standard includes. */
#include <EVSE_Cloud_Connectivity.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/* Azure Provisioning/IoT Hub library includes */
#include "azure_iot_hub_client.h"
#include "azure_iot_hub_client_properties.h"
#include "azure_iot_provisioning_client.h"

/* Azure JSON includes */
#include "azure_iot_json_reader.h"
#include "azure_iot_json_writer.h"

/* Exponential backoff retry include. */
#include "backoff_algorithm.h"

/* Transport interface implementation include header for TLS. */
#include "using_mbedtls.h"

/* Crypto helper header. */
#include "azure_sample_crypto.h"

/* Demo Specific configs. */
#include "azure_iot_config.h"

#include "EVSE_Connectivity.h"
#include "EVSE_UI.h"
#include "EVSE_Metering.h"
#include "EVSE_Utils.h"
#include "EVSE_Secure_Element.h"

#if EVSE_EDGELOCK_AGENT
#include "EVSE_EdgeLock2goAgent.h"
#include "nxp_iot_agent_config.h"
#endif

/*-----------------------------------------------------------*/

/* Compile time error for undefined configs. */

#if !defined(ENDPOINT) && defined(ENABLE_DPS_SAMPLE)
#error "Define the config dps endpoint by following the instructions in file EVSE_config.h."
#endif

#ifndef democonfigROOT_CA_PEM
#error "Please define Root CA certificate of the IoT Hub (democonfigROOT_CA_PEM) in EVSE_config.h."
#endif

#if defined(EVSE_SAS_AUTH) && defined(democonfigCLIENT_CERTIFICATE_PEM)
#error "Please define only one auth EVSE_SAS_AUTH or democonfigCLIENT_CERTIFICATE_PEM in EVSE_config.h."
#endif

#if (EVSE_SAS_AUTH == 1) && !defined(DEVICE_SYMMETRIC_KEY)
#error "Please define the symmetric key"
#endif

#if !defined(EVSE_SAS_AUTH) && !defined(democonfigCLIENT_CERTIFICATE_PEM)
#error "Please define one auth EVSE_SAS_AUTH or democonfigCLIENT_CERTIFICATE_PEM in EVSE_config.h."
#endif
/*-----------------------------------------------------------*/

#define TELEMETRY_BUFFER_SIZE           512
#define COMMAND_BUFFER_SIZE             256
#define REPORTED_PROPERTIES_BUFFER_SIZE 380

#define HTTP_OK_RESPONSE                200

#define TLS_SEND       TLS_FreeRTOS_Send
#define TLS_RECV       TLS_FreeRTOS_Recv
#define TLS_CONNECT    TLS_FreeRTOS_Connect
#define TLS_DISCONNECT TLS_FreeRTOS_Disconnect

#define IOT_HUB_MAX_RETRIES    5
#define EVSE_WAIT_CLOUD_EVENTS 1000

/*-----------------------------------------------------------*/

/* Device command */
extern const char method_name_terminate[METHOD_NAME_MAX_SIZE];

extern const az_span method_status_name;
extern const az_span method_status_ok;
extern const az_span success_response_description;

/*-----------------------------------------------------------*/

static cloudConnectionState_t s_cloudConnectionState = EVSE_Cloud_NotConnected;

static const char *cloudConnectionStateString[EVSE_Cloud_Last + 1] = {
    [EVSE_Cloud_NotConnected]     = "No contact attempt yet",
    [EVSE_Cloud_EdgeLock_Request] = "Try EdgeLock connection",
    [EVSE_Cloud_EdgeLock_Error]   = "Failed connection to EdgeLock",
    [EVSE_Cloud_DPSRequest]       = "Try DPS connection",
    [EVSE_Cloud_DPSError]         = "Failed connection to DPS",
    [EVSE_Cloud_HubRequest]       = "Try Hub connection",
    [EVSE_Cloud_HubError]         = "Failed connection to Hub",
    [EVSE_Cloud_HubDisconnected]  = "Hub Disconnected",
    [EVSE_Cloud_HubReconnection]  = "Try hub reconnection",
    [EVSE_Cloud_HubConnected]     = "IoTHub connected",
    [EVSE_Cloud_Last]             = "LAST",
};

/**
 * @brief Unix time.
 *
 * @return Time in milliseconds.
 */
static uint64_t ullGetUnixTime(void);

/*-----------------------------------------------------------*/

/* Define buffer for IoT Hub info.  */
static uint8_t ucSampleIotHubHostname[EVSE_HOSTNAME_MAX_BUFFER];
static uint8_t ucSampleIotHubDeviceId[EVSE_DEVICEID_MAX_BUFFER];
#if ENABLE_DPS_SAMPLE
static AzureIoTProvisioningClient_t xAzureIoTProvisioningClient;
#endif /* ENABLE_DPS_SAMPLE */

/* Each compilation unit must define the NetworkContext_t struct. */
static NetworkContext_t xNetworkContext = {0};
static AzureIoTHubClient_t xAzureIoTHubClient;

/* Telemetry buffers */
static uint8_t ucScratchBuffer[TELEMETRY_BUFFER_SIZE];

/* Command buffers */
static uint8_t ucCommandResponsePayloadBuffer[COMMAND_BUFFER_SIZE];

/* Reported Properties buffers */
static uint8_t ucReportedPropertiesUpdate[REPORTED_PROPERTIES_BUFFER_SIZE];
static uint32_t ulReportedPropertiesUpdateLength;

static EventGroupHandle_t s_evse_event = NULL;

/* Counter for telemetry packets */
static uint32_t telemetryCounter = 0U;

/* 1 if initial telemetry was sent, 0 otherwise. */
static uint8_t EVSE_initialTelemetrySent = 0U;

/**
 * @brief Static buffer used to hold MQTT messages being sent and received.
 */
static uint8_t ucMQTTMessageBuffer[democonfigNETWORK_BUFFER_SIZE];

/*-----------------------------------------------------------*/

#if ENABLE_DPS_SAMPLE

/**
 * @brief Gets the IoT Hub endpoint and deviceId from Provisioning service.
 *   This function will block for Provisioning service for result or return failure.
 *
 * @param[in] pXNetworkCredentials  Network credential used to connect to Provisioning service
 * @param[out] ppucIothubHostname  Pointer to uint8_t* IoT Hub hostname return from Provisioning Service
 * @param[in,out] pulIothubHostnameLength  Length of hostname
 * @param[out] ppucIothubDeviceId  Pointer to uint8_t* deviceId return from Provisioning Service
 * @param[in,out] pulIothubDeviceIdLength  Length of deviceId
 */
static AzureIoTResult_t prvIoTHubInfoGet(NetworkCredentials_t *pXNetworkCredentials,
                                 uint8_t **ppucIothubHostname,
                                 uint32_t *pulIothubHostnameLength,
                                 uint8_t **ppucIothubDeviceId,
                                 uint32_t *pulIothubDeviceIdLength);

#endif /* ENABLE_DPS_SAMPLE */

/**
 * Telemetry PUBACK callback
 *
 **/
static void prvTelemetryPubackCallback(uint16_t usPacketID)
{
    AZLogInfo(("Puback received for packet id in callback: 0x%08x", usPacketID));
}

/**
 * @brief Connect to endpoint with reconnection retries.
 *
 * If connection fails, retry is attempted after a timeout.
 * Timeout value will exponentially increase until maximum
 * timeout value is reached or the number of attempts are exhausted.
 *
 * @param pcHostName Hostname of the endpoint to connect to.
 * @param ulPort Endpoint port.
 * @param pxNetworkCredentials Pointer to Network credentials.
 * @param pxNetworkContext Point to Network context created.
 * @return uint32_t The status of the final connection attempt.
 */
static uint32_t prvConnectToServerWithBackoffRetries(const char *pcHostName,
                                                     uint32_t ulPort,
                                                     NetworkCredentials_t *pxNetworkCredentials,
                                                     NetworkContext_t *pxNetworkContext);

/**
 * @brief Disconnects the AzureIoTHubClient and closes TLS socket.
 * @param reconnection set it true in order to do a reconnection after disconnect
 */
static void EVSE_Cloud_Disconnect(bool reconnection);

/*-----------------------------------------------------------*/

/**
 * Set state and advertise change state.
 * @param connectionState new cloud state;
 */
static void prvEVSE_Cloud_SetConnectionState(cloudConnectionState_t cloudConnectionState)
{
    if (s_cloudConnectionState == cloudConnectionState)
    {
        return;
    }

    s_cloudConnectionState = cloudConnectionState;

#if (ENABLE_LCD)
    EVSE_UI_SetEvent(EVSE_UI_CloudStatus);
#endif /* ENABLE_LCD */
}

/**
 * @brief Internal function for handling Command requests.
 *
 * @remark This function is required for the interface with samples to work properly.
 */
static void prvHandleCommand(AzureIoTHubClientCommandRequest_t *pxMessage, void *pvContext)
{
    LogInfo(("Cloud message payload : %.*s \r\n", (int)pxMessage->ulPayloadLength,
             (const char *)pxMessage->pvMessagePayload));

    (void)pvContext;
    AzureIoTHubClient_t *pxHandle  = (AzureIoTHubClient_t *)pvContext;
    uint32_t ulResponseStatus      = 0;
    AzureIoTResult_t xResult       = eAzureIoTErrorFailed;
    meter_status_t status          = kMeterStatus_Fail;
    uint32_t payloadLength         = 0;
    uint16_t method_name_length    = pxMessage->usCommandNameLength;
    const uint8_t *method_name_ptr = pxMessage->pucCommandName;

    if ((method_name_length == (sizeof(method_name_terminate) - 1)) &&
        (memcmp((void *)method_name_ptr, (void *)method_name_terminate, sizeof(method_name_terminate) - 1) == 0))
    {
        status = serialize_terminate_charging(ucCommandResponsePayloadBuffer, COMMAND_BUFFER_SIZE, &payloadLength);
        if (status == kMeterStatus_Success)
        {
            if ((xResult = AzureIoTHubClient_SendCommandResponse(pxHandle, pxMessage, ulResponseStatus,
                                                                 ucCommandResponsePayloadBuffer, payloadLength)) !=
                eAzureIoTSuccess)
            {
                LogError(("Error sending command response: result 0x%08x", (uint16_t)xResult));
            }
            else
            {
                LogInfo(("Successfully sent command response %d", (int16_t)ulResponseStatus));

                LogInfo(("Attempt to receive publish message from IoT Hub.\r\n"));
                xResult = AzureIoTHubClient_ProcessLoop(&xAzureIoTHubClient, sampleazureiotPROCESS_LOOP_TIMEOUT_MS);
                if (xResult != eAzureIoTSuccess)
                {
                    LogError(("Error processing loop: result 0x%08x", (uint16_t)xResult));
                    if (xResult == eAzureIoTErrorFailed)
                    {
                        LogInfo(("Requesting cloud disconnect ...\r\n"));
                        EVSE_Cloud_Disconnect(true);
                    }
                }
            }
        }
        else
        {
            LogInfo(("Failed to create json payload for command response."));
        }
    }
    else
    {
        LogInfo(("Unknown command."));
    }
}

/**
 * @brief Internal function for handling properties request.
 */
static void prvDispatchPropertiesRequestToSync(AzureIoTHubClientPropertiesResponse_t *pxMessage)
{
    meter_status_t status    = kMeterStatus_Fail;
    AzureIoTResult_t xResult = eAzureIoTErrorFailed;

    /* Extract the properties new values and update them locally. */
    if (do_property_sync_locally(pxMessage) != kMeterStatus_Success)
    {
        LogError(("Failed to update properties locally."));
        return;
    }

    /* Serialize the properties and send them to cloud. */
    status = serialize_reported_property(ucReportedPropertiesUpdate,
                                         REPORTED_PROPERTIES_BUFFER_SIZE, &ulReportedPropertiesUpdateLength);
    if (status == kMeterStatus_Success)
    {
        xResult = AzureIoTHubClient_SendPropertiesReported(&xAzureIoTHubClient,
                                                           ucReportedPropertiesUpdate,
                                                           ulReportedPropertiesUpdateLength,
                                                           NULL);
        if (xResult !=  eAzureIoTSuccess)
        {
            LogError(("Error sending reported properties: result 0x%08x", (uint16_t)xResult));
        }
        else
        {
            LogInfo(("Successfully sent reported properties."));
        }
    }
    else
    {
        LogInfo(("Failed to create json payload for property request response."));
    }
}

/**
 * @brief Internal function for handling properties updates.
 */
static void prvDispatchPropertiesUpdate(AzureIoTHubClientPropertiesResponse_t *pxMessage)
{
    meter_status_t status          = kMeterStatus_Fail;
    AzureIoTResult_t xResult       = eAzureIoTErrorFailed;
    uint32_t propertyVersion       = 0U;
    uint8_t propertiesToUpdate     = 0U;

    /* Extract the properties that need updating, update them locally and populate propertyVersion and propertiesToUpdate. */
    if (do_property_update_locally(pxMessage, &propertyVersion, &propertiesToUpdate) != kMeterStatus_Success)
    {
        LogError(("Failed to update properties locally."));
        return;
    }

    /* Send response to cloud to confirm property update. */
    status = serialize_reported_property_update(HTTP_OK_RESPONSE, propertyVersion, success_response_description,
                                                propertiesToUpdate, ucReportedPropertiesUpdate,
                                                REPORTED_PROPERTIES_BUFFER_SIZE, &ulReportedPropertiesUpdateLength);
    if (status == kMeterStatus_Success)
    {
        xResult = AzureIoTHubClient_SendPropertiesReported(&xAzureIoTHubClient,
                                                           ucReportedPropertiesUpdate,
                                                           ulReportedPropertiesUpdateLength,
                                                           NULL);
        if (xResult !=  eAzureIoTSuccess)
        {
            LogError(("Error sending reported properties: result 0x%08x", (uint16_t)xResult));
        }
        else
        {
            LogInfo(("Successfully sent reported properties."));
        }
    }
    else
    {
        LogInfo(("Failed to create json payload for property update response."));
    }
}

static uint64_t ullGetUnixTime(void)
{
    return (uint64_t)EVSE_Connectivity_GetUnixTime();
}

/**
 * @brief Private property message callback handler.
 *        This handler dispatches the calls to the functions that handle
 *        the properties requests or updates.
 */
static void prvHandleProperties(AzureIoTHubClientPropertiesResponse_t *pxMessage, void *pvContext)
{
    (void)pvContext;

    LogDebug(("Property document payload : %.*s \r\n", (int16_t)pxMessage->ulPayloadLength,
              (const char *)pxMessage->pvMessagePayload));

    switch (pxMessage->xMessageType)
    {
        /* Update local properties to sync with cloud settings (requested by the EVSE). */
        case eAzureIoTHubPropertiesRequestedMessage:
            LogInfo(("Device property document GET received"));
            prvDispatchPropertiesRequestToSync(pxMessage);
            break;

        /* Update local properties to sync with cloud settings (requested by cloud). */
        case eAzureIoTHubPropertiesWritablePropertyMessage:
            LogInfo(("Device writeable property received"));
            prvDispatchPropertiesUpdate(pxMessage);
            break;

        case eAzureIoTHubPropertiesReportedResponseMessage:
            LogInfo(("Device reported property response received"));
            break;

        default:
            LogError(("Unknown property message: 0x%08x", pxMessage->xMessageType));
            configASSERT(false);
    }
}
/*-----------------------------------------------------------*/

/**
 * @brief Setup Device Provisioning Service credentials.
 */
static uint32_t prvSetupDPSNetworkCredentials(NetworkCredentials_t *pxNetworkCredentials)
{
    /* Set additional parameters in case of X.509 storage inside SE*/
#if (EVSE_X509_SE050_AUTH == 1)
    static char keyLabel[20]  = {0};
    static char certLabel[20] = {0};

    pex_sss_demo_tls_ctx->client_cert_index    = AZURE_IOT_CLIENT_CERT_INDEX_SM;
    pex_sss_demo_tls_ctx->client_keyPair_index = AZURE_IOT_KEY_INDEX_SM;

    memset(certLabel, 0, sizeof(certLabel));
    if (snprintf(certLabel, sizeof(certLabel), "sss:%08lx", pex_sss_demo_tls_ctx->client_cert_index) < 0){
        LogError(("snprintf error"));
        return TLS_TRANSPORT_INVALID_CREDENTIALS;
    }
    memset(keyLabel, 0, sizeof(keyLabel));
    if (snprintf(keyLabel, sizeof(keyLabel), "sss:%08lx", pex_sss_demo_tls_ctx->client_keyPair_index) < 0){
        LogError(("snprintf error"));
        return TLS_TRANSPORT_INVALID_CREDENTIALS;
    }

    /* Set the credentials for establishing a TLS connection. */
    pxNetworkCredentials->pClientCertLabel = (const char *)&certLabel[0];
    pxNetworkCredentials->pPrivateKeyLabel = (const char *)&keyLabel[0];
#endif /* EVSE_X509_SE050_AUTH */

    pxNetworkCredentials->disableSni = pdFALSE;
    /* Set the credentials for establishing a TLS connection. */
    pxNetworkCredentials->pRootCa    = (const unsigned char *)democonfigROOT_CA_PEM;
    pxNetworkCredentials->rootCaSize = sizeof(democonfigROOT_CA_PEM);

#if (EVSE_X509_AUTH == 1)
    pxNetworkCredentials->pucClientCert   = (const unsigned char *)X509_CERTIFICATE_PEM;
    pxNetworkCredentials->xClientCertSize = sizeof(X509_CERTIFICATE_PEM);
    pxNetworkCredentials->pucPrivateKey   = (const unsigned char *)RSA_KEYPAIR_PEM;
    pxNetworkCredentials->xPrivateKeySize = sizeof(RSA_KEYPAIR_PEM);
#endif

    return 0;
}

/**
 * @brief Setup IoT Hub Network credentials.
 */
static uint32_t prvSetupHubNetworkCredentials(NetworkCredentials_t *pxNetworkCredentials)
{

#if (EVSE_X509_SE050_AUTH == 1)
    static char keyLabel[ 20 ] = {0};
    static char certLabel[ 20 ] = {0};

    pex_sss_demo_tls_ctx->client_cert_index    = AZURE_IOT_CLIENT_CERT_INDEX_SM;
    pex_sss_demo_tls_ctx->client_keyPair_index = AZURE_IOT_KEY_INDEX_SM;

    memset(certLabel, 0, sizeof(certLabel));
    if (snprintf(certLabel, sizeof(certLabel), "sss:%08lx", pex_sss_demo_tls_ctx->client_cert_index) < 0){
        LogError(("snprintf error"));
        return TLS_TRANSPORT_INVALID_CREDENTIALS;
    }
    memset(keyLabel, 0, sizeof(keyLabel));
    if (snprintf(keyLabel, sizeof(keyLabel), "sss:%08lx", pex_sss_demo_tls_ctx->client_keyPair_index) < 0){
        LogError(("snprintf error"));
        return TLS_TRANSPORT_INVALID_CREDENTIALS;
    }

    /* Set the credentials for establishing a TLS connection. */
    pxNetworkCredentials->pClientCertLabel = ( const char * ) &certLabel[ 0 ];
    pxNetworkCredentials->pPrivateKeyLabel = ( const char * ) &keyLabel[ 0 ];
#endif /* EVSE_X509_SE050_AUTH */

    pxNetworkCredentials->disableSni = pdFALSE;
    /* Set the credentials for establishing a TLS connection. */
    pxNetworkCredentials->pRootCa    = (const unsigned char *)demoHubconfigROOT_CA_PEM;
    pxNetworkCredentials->rootCaSize = sizeof(demoHubconfigROOT_CA_PEM);

#if (EVSE_X509_AUTH == 1)
    pxNetworkCredentials->pucClientCert   = (const unsigned char *)X509_CERTIFICATE_PEM;
    pxNetworkCredentials->xClientCertSize = sizeof(X509_CERTIFICATE_PEM);
    pxNetworkCredentials->pucPrivateKey   = (const unsigned char *)RSA_KEYPAIR_PEM;
    pxNetworkCredentials->xPrivateKeySize = sizeof(RSA_KEYPAIR_PEM);
#endif

    return 0;
}

static void prvClearNetworkContext(NetworkContext_t *pxNetworkContext)
{
    if (pxNetworkContext == NULL)
    {
        return;
    }

    (void)memset(pxNetworkContext, 0U, sizeof(NetworkContext_t));

}

/*-----------------------------------------------------------*/

/**
 * @brief Sends the initial telemetry packet.
 */
static void prvAzureTelemetrySendOnConnect()
{
    uint32_t ulScratchBufferLength = 0U;
    AzureIoTResult_t xResult       = eAzureIoTErrorFailed;
    meter_status_t status          = kMeterStatus_Fail;

    LogInfo(("Start sending initial telemetry data ..."));

    status = serialize_telemetry_action_on_connect(ucScratchBuffer, TELEMETRY_BUFFER_SIZE, &ulScratchBufferLength);
    if ((status == kMeterStatus_Success) && (ulScratchBufferLength > 0))
    {
        /* Send telemetry and block until the message is sent over the network (or fails), though it will not block waiting for an acknowledgement */
        xResult = AzureIoTHubClient_SendTelemetry(&xAzureIoTHubClient, ucScratchBuffer, ulScratchBufferLength, NULL,
                                                  eAzureIoTHubMessageQoS1, NULL);
        if (xResult != eAzureIoTSuccess)
        {
            LogError(("Error sending telemetry: result 0x%08x", (uint16_t)xResult));
            /* todo: In the future, log this to filesystem */
        }
        else
        {
            LogInfo(("Finished sending initial telemetry data ..."));

            LogInfo(("Attempt to receive publish message from IoT Hub.\r\n"));
            xResult = AzureIoTHubClient_ProcessLoop(&xAzureIoTHubClient, sampleazureiotPROCESS_LOOP_TIMEOUT_MS);
            if (xResult != eAzureIoTSuccess)
            {
                LogError(("Error processing loop: result 0x%08x", (uint16_t)xResult));
            }
        }
    }

    if (xResult != eAzureIoTSuccess)
    {
        /* We are properly disconnected but try to do a proper cleanup */
        LogInfo(("Requesting cloud disconnect ...\r\n"));
        EVSE_Cloud_Disconnect(true);
    }
}

/**
 * @brief Sends telemetry packet.
 */
static void prvAzureTelemetrySend()
{
    uint32_t ulScratchBufferLength = 0U;
    AzureIoTResult_t xResult       = eAzureIoTErrorFailed;
    meter_status_t status          = kMeterStatus_Fail;

    configPRINTF(("Start sending telemetry data ..."));

    if (!EVSE_Connectivity_IsConnectedToInternet())
    {
        return;
    }

    status = serialize_telemetry_action(ucScratchBuffer, TELEMETRY_BUFFER_SIZE, &ulScratchBufferLength);
    if ((status == kMeterStatus_Success) && (ulScratchBufferLength > 0))
    {
        xResult = AzureIoTHubClient_SendTelemetry(&xAzureIoTHubClient, ucScratchBuffer, ulScratchBufferLength, NULL,
                                                  eAzureIoTHubMessageQoS1, NULL);
        if (xResult != eAzureIoTSuccess)
        {
            LogError(("Error sending telemetry: result 0x%08x", (uint16_t)xResult));
            /* todo: In the future, log this to filesystem */
        }
        else
        {
            LogInfo(("Finished sending telemetry data ..."));

            LogInfo(("Attempt to receive publish message from IoT Hub.\r\n"));
            xResult = AzureIoTHubClient_ProcessLoop(&xAzureIoTHubClient, sampleazureiotPROCESS_LOOP_TIMEOUT_MS);

            if (xResult != eAzureIoTSuccess)
            {
                LogError(("Error processing loop: result 0x%08x", (uint16_t)xResult));
            }
        }
    }

    if (xResult != eAzureIoTSuccess)
    {
        configPRINTF((error("Requesting cloud disconnect ...\r\n")));
        EVSE_Cloud_Disconnect(true);
    }
    else
    {
        configPRINTF((success("Telemetry packet %d sent successfully"), telemetryCounter));
        /* Only increase the telemetry counter on success */
        telemetryCounter = telemetryCounter + 1;
    }
}

/**
 * @brief Checks if the credentials for the Cloud Connection are present in the EVSE.
 */
static void prvAzureCheckProvisioned(void)
{
#if (EVSE_X509_SE050_AUTH == 1)
    certificates_status_t certStatus = kStatus_Certificates_Failed;
    uint32_t cert_size               = CERT_BUFFER_SIZE;

    EVSE_SE050_Open_Session();
    certStatus = EVSE_SE050_Check_Certificate(AZURE_IOT_CLIENT_CERT_INDEX_SM, &cert_size);
    EVSE_SE050_Close_Session();

    if (certStatus != kStatus_Certificates_Success)
    {
#if EVSE_EDGELOCK_AGENT
        /* Contact EdgeLock2Go agent to get certificates for Cloud connection into the SE */
        prvEVSE_Cloud_SetConnectionState(EVSE_Cloud_EdgeLock_Request);

        if (EVSE_EdgeLock_RunClient() != IOT_AGENT_SUCCESS)
        {
            configPRINTF((error("EdgeLock 2GO client failed!\r\n")));
            prvEVSE_Cloud_SetConnectionState(EVSE_Cloud_EdgeLock_Error);
            return;
        }

        EVSE_SE050_Open_Session();
        EVSE_SE050_Delete_ServerHostname(AZURE_IOT_HOSTNAME_INDEX_SM);
        EVSE_SE050_Close_Session();
#else
        configPRINTF((error("No certificates provisioned in the secure element!\r\n")));
        return;
#endif /* EVSE_EDGELOCK_AGENT */
    }

    EVSE_SE050_Open_Session();
    if (EVSE_SE050_Get_DeviceIDFromCertificate((uint8_t *)&ucSampleIotHubDeviceId[0], EVSE_DEVICEID_MAX_BUFFER) !=
        kStatus_Certificates_Success)
    {
        configPRINTF((error("Reading the device id failed!\r\n")));
    }
    EVSE_SE050_Close_Session();

#elif (EVSE_X509_AUTH == 1)
    if (EVSE_SE050_Get_DeviceIDFromCertificate((uint8_t *)&ucSampleIotHubDeviceId[0], EVSE_DEVICEID_MAX_BUFFER) !=
        kStatus_Certificates_Success)
    {
        configPRINTF((error("Reading the device id failed!\r\n")));
    }
#endif /* (EVSE_X509_SE050_AUTH == 1)) */

    xEventGroupSetBits(s_evse_event, EVSE_INITIALIZATION_EVENT);
}

/**
 * @brief Monitors Internet connection for the Azure IoT Hub.
 */
static void prvMonitorConnection(void)
{

    if (!EVSE_Connectivity_IsConnectedToInternet())
    {
        if (s_cloudConnectionState == EVSE_Cloud_HubConnected)
        {
            EVSE_Cloud_Disconnect(false);
        }
    }
    else
    {
        if (s_cloudConnectionState == EVSE_Cloud_HubDisconnected)
        {
            xEventGroupSetBits(s_evse_event, EVSE_INITIALIZATION_EVENT);
        }
    }
}
/**
 * @brief Connects to Azure IoT Hub.
 * @param trySEHostname true if we use the hostname stored in the Secure Element
 *                   false otherwise
 */
static void prvAzureCloudConnection(bool trySEHostname)
{
    static NetworkCredentials_t xNetworkCredentials = {0};
    static AzureIoTTransportInterface_t xTransport  = {0};
    static AzureIoTHubClientOptions_t xHubOptions = {0};
    AzureIoTResult_t xResult;
    uint32_t ulStatus;
    AzureIoTResult_t IoTHub_status;
    hostname_config_status_t status = kStatus_Hostname_Failed;
    bool xSessionPresent;
    static uint8_t *pucIotHubHostname = NULL;
    uint32_t pulIothubHostnameLength = EVSE_HOSTNAME_MAX_BUFFER;

    EVSE_SE050_Open_Session();
    if (trySEHostname)
    {
        status = EVSE_SE050_Get_ServerHostname(ucSampleIotHubHostname, &pulIothubHostnameLength);
        if (status == kStatus_Hostname_Success)
        {
            pucIotHubHostname = (uint8_t*)ucSampleIotHubHostname;
        }
    }

#if !defined(ENABLE_DPS_SAMPLE)
    if (status != kStatus_Hostname_Success)
    {
        LogError(("DPS disabled, hostname not present in plain text or SE!\r\n"));
        configASSERT(status == 0);
    }
#endif

    /* Load Device ID */
#if ((EVSE_X509_AUTH == 1) || (EVSE_X509_SE050_AUTH == 1))
    uint8_t *pucIotHubDeviceId       = (uint8_t *)ucSampleIotHubDeviceId;
    uint32_t pulIothubDeviceIdLength = strlen(ucSampleIotHubDeviceId);
#else
    uint8_t *pucIotHubDeviceId       = (uint8_t *)DEVICE_ID;
    uint32_t pulIothubDeviceIdLength = sizeof(DEVICE_ID) - 1;
#endif /* ((EVSE_X509_AUTH == 1) || (EVSE_X509_SE050_AUTH == 1)) */
    configPRINTF(("Loaded device id: %s.\r\n", pucIotHubDeviceId));

    /* Initialize Azure IoT Middleware.  */
    configASSERT(AzureIoT_Init() == eAzureIoTSuccess);

#if ENABLE_DPS_SAMPLE

    /* Try DPS in case of SE lack of key storage or failed reading */
    if (status != kStatus_Hostname_Success)
    {
        prvEVSE_Cloud_SetConnectionState(EVSE_Cloud_DPSRequest);
        configPRINTF(("Try DPS connection %s.\n\r", pucIotHubHostname));
        ulStatus = prvSetupDPSNetworkCredentials(&xNetworkCredentials);
        configASSERT(ulStatus == 0);

        /* Run DPS. */
        if ((IoTHub_status = prvIoTHubInfoGet(&xNetworkCredentials, &pucIotHubHostname, &pulIothubHostnameLength,
                                     &pucIotHubDeviceId, &pulIothubDeviceIdLength)) != eAzureIoTSuccess)
        {
            LogError(("Failed on sample_dps_entry!: error code = 0x%08x\r\n", IoTHub_status));
            prvEVSE_Cloud_SetConnectionState(EVSE_Cloud_DPSError);
            goto exit;
        }
    }

#endif /* ENABLE_DPS_SAMPLE */

    prvClearNetworkContext(&xNetworkContext);

    if (EVSE_Connectivity_IsConnectedToInternet())
    {
        prvEVSE_Cloud_SetConnectionState(EVSE_Cloud_HubRequest);

        ulStatus = prvSetupHubNetworkCredentials(&xNetworkCredentials);
        if (ulStatus != 0)
        {
            configPRINTF(("Failed to setup network credentials \r\n"));
        }

        /* Attempt to establish TLS session with IoT Hub. If connection fails,
         * retry after a timeout. Timeout value will be exponentially increased
         * until  the maximum number of attempts are reached or the maximum timeout
         * value is reached. The function returns a failure status if the TCP
         * connection cannot be established to the IoT Hub after the configured
         * number of attempts. */

        ulStatus = prvConnectToServerWithBackoffRetries((const char *)pucIotHubHostname, democonfigIOTHUB_PORT,
                                       &xNetworkCredentials, &xNetworkContext);
        if (ulStatus != 0)
        {
            configPRINTF(("Failed to connect to server with backoff retries\r\n"));
            goto exit;
        }

        /* Fill in Transport Interface send and receive function pointers. */

        xTransport.pxNetworkContext = &xNetworkContext;
        xTransport.xSend            = TLS_SEND;
        xTransport.xRecv            = TLS_RECV;

        /* Init IoT Hub option */
        xResult = AzureIoTHubClient_OptionsInit(&xHubOptions);
        if (xResult != eAzureIoTSuccess)
        {
            configPRINTF(("Failed to init IoT Hub options\r\n"));
        }

        xHubOptions.pucModelID         = (const uint8_t *)SAMPLE_PNP_MODEL_ID;
        xHubOptions.ulModelIDLength    = sizeof(SAMPLE_PNP_MODEL_ID) - 1;
        xHubOptions.xTelemetryCallback = prvTelemetryPubackCallback;
        //            xHubOptions.pucModuleID = ( const uint8_t * ) MODULE_ID;
        //            xHubOptions.ulModuleIDLength = sizeof( MODULE_ID ) - 1;

#if democonfigPNP_COMPONENTS_LIST_LENGTH
#if democonfigPNP_COMPONENTS_LIST_LENGTH > 0
        xHubOptions.pxComponentList       = democonfigPNP_COMPONENTS_LIST;
        xHubOptions.ulComponentListLength = democonfigPNP_COMPONENTS_LIST_LENGTH;
#endif /* > 0 */
#endif /* democonfigPNP_COMPONENTS_LIST_LENGTH */

        xResult = AzureIoTHubClient_Init(&xAzureIoTHubClient, pucIotHubHostname, pulIothubHostnameLength,
                                         pucIotHubDeviceId, pulIothubDeviceIdLength, &xHubOptions, ucMQTTMessageBuffer,
                                         sizeof(ucMQTTMessageBuffer), ullGetUnixTime, &xTransport);
        if (xResult != eAzureIoTSuccess)
        {
            configPRINTF(("Failed to init IoT Hub client\r\n"));
            goto exit;
        }

#if EVSE_SAS_AUTH
        xResult = AzureIoTHubClient_SetSymmetricKey(&xAzureIoTHubClient, (const uint8_t *)DEVICE_SYMMETRIC_KEY,
                                                    sizeof(DEVICE_SYMMETRIC_KEY) - 1, Crypto_HMAC);
        configASSERT(xResult == eAzureIoTSuccess);
#endif /* EVSE_SAS_AUTH */

        /* Sends an MQTT Connect packet over the already established TLS connection,
         * and waits for connection acknowledgment (CONNACK) packet. */
        configPRINTF(("Creating an MQTT connection to %s.\r\n", pucIotHubHostname));

        xResult = AzureIoTHubClient_Connect(&xAzureIoTHubClient, false, &xSessionPresent,
                                            sampleazureiotCONNACK_RECV_TIMEOUT_MS);

        if (xResult != eAzureIoTSuccess)
        {
            configPRINTF(("Failed to connect to IoTHub. Will retry...\r\n"));
            xEventGroupSetBits(s_evse_event, EVSE_REINITIALIZATION_EVENT);
            goto exit;
        }

        prvEVSE_Cloud_SetConnectionState(EVSE_Cloud_HubConnected);

        xResult = AzureIoTHubClient_SubscribeCommand(&xAzureIoTHubClient, prvHandleCommand, &xAzureIoTHubClient,
                                                     sampleazureiotSUBSCRIBE_TIMEOUT);
        if (xResult != eAzureIoTSuccess)
        {
            configPRINTF(("Failed to subscribe to Azure IoT Hub Commands. This feature will NOT be available\r\n"));
        }

        xResult = AzureIoTHubClient_SubscribeProperties(&xAzureIoTHubClient, prvHandleProperties, &xAzureIoTHubClient,
                                                        sampleazureiotSUBSCRIBE_TIMEOUT);
        if (xResult != eAzureIoTSuccess)
        {
            configPRINTF(("Failed to subscribe to Azure IoT Hub Properties. This feature will NOT be available\r\n"));
        }
        else
        {
            /* Get property document after initial connection */
            xResult = AzureIoTHubClient_RequestPropertiesAsync(&xAzureIoTHubClient);
            if (xResult != eAzureIoTSuccess)
            {
                configPRINTF(("Failed to request property document from Azure IoT Hub\r\n"));
            }
        }

        /* Send initial telemetry message. */
        prvAzureTelemetrySendOnConnect();

        EVSE_initialTelemetrySent = 1;
        return;
    }

    LogError(("EVSE is not connected to the Internet.\r\n"));

exit:
    EVSE_SE050_Close_Session();
    return;
}

/**
 * Disconnects the AzureIoTHubClient and closes TLS socket.
 */
static void EVSE_Cloud_Disconnect(bool reconnection)
{
    AzureIoTResult_t xResult = eAzureIoTSuccess;

    if (EVSE_Connectivity_IsConnectedToInternet())
    {
        LogInfo(("EVSE disconnected, unsubscribing..."));

        xResult = AzureIoTHubClient_UnsubscribeProperties(&xAzureIoTHubClient);

        xResult |= AzureIoTHubClient_UnsubscribeCommand(&xAzureIoTHubClient);

        /* Send an MQTT Disconnect packet over the already connected TLS over
         * TCP connection. There is no corresponding response for the disconnect
         * packet. After sending disconnect, client must close the network
         * connection. */
        xResult |= AzureIoTHubClient_Disconnect(&xAzureIoTHubClient);

        if (xResult != eAzureIoTSuccess)
        {
            LogError(("Failed to disconnect, probably already disconnected"));
        }
        else
        {
            LogInfo(("Successfully disconnected the AzureIoTHubClient"));
        }
    }

    /* Close the network connection. */

    TLS_DISCONNECT(&xNetworkContext);

    memset(&xNetworkContext, 0, sizeof(xNetworkContext));

    /* Set new cloud state. */
    prvEVSE_Cloud_SetConnectionState(EVSE_Cloud_HubDisconnected);
    EVSE_initialTelemetrySent = 0;
    EVSE_SE050_Close_Session();

    if (reconnection == true)
    {
        /* If reconnection flag is set, set a init event */
        xEventGroupSetBits(s_evse_event, EVSE_INITIALIZATION_EVENT);
    }
}
/*-----------------------------------------------------------*/

#if ENABLE_DPS_SAMPLE

/**
 * @brief Get IoT Hub endpoint and device Id info, when Provisioning service is used.
 *   This function will block for Provisioning service for result or return failure.
 */
static AzureIoTResult_t prvIoTHubInfoGet(NetworkCredentials_t *pXNetworkCredentials,
                                 uint8_t **ppucIothubHostname,
                                 uint32_t *pulIothubHostnameLength,
                                 uint8_t **ppucIothubDeviceId,
                                 uint32_t *pulIothubDeviceIdLength)
{

    AzureIoTResult_t xResult;
    AzureIoTTransportInterface_t xTransport;

    prvClearNetworkContext(&xNetworkContext);

    uint32_t ucSamplepIothubHostnameLength = sizeof(ucSampleIotHubHostname);
    uint32_t ucSamplepIothubDeviceIdLength = sizeof(ucSampleIotHubDeviceId);
    uint32_t ulStatus;

    ulStatus = prvConnectToServerWithBackoffRetries(ENDPOINT, democonfigIOTHUB_PORT, pXNetworkCredentials, &xNetworkContext);

    if(ulStatus != 0)
    {
        xResult = eAzureIoTErrorServerError;
        goto exit;
    }

    /* Fill in Transport Interface send and receive function pointers. */

    xTransport.pxNetworkContext = &xNetworkContext;
    xTransport.xSend            = TLS_SEND;
    xTransport.xRecv            = TLS_RECV;

#if ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1))
#undef REGISTRATION_ID
#define REGISTRATION_ID ucSampleIotHubDeviceId
#endif /* ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1)) */

    xResult = AzureIoTProvisioningClient_Init(
        &xAzureIoTProvisioningClient, (const uint8_t *)ENDPOINT, sizeof(ENDPOINT) - 1, (const uint8_t *)ID_SCOPE,
        sizeof(ID_SCOPE) - 1, (const uint8_t *)REGISTRATION_ID,
#if ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1))
        strlen(REGISTRATION_ID),
#else
        sizeof(REGISTRATION_ID) - 1,
#endif
        NULL, ucMQTTMessageBuffer, sizeof(ucMQTTMessageBuffer), ullGetUnixTime, &xTransport);
    if(xResult != eAzureIoTSuccess)
    {
        goto exit;
    }

#if EVSE_SAS_AUTH
    xResult =
        AzureIoTProvisioningClient_SetSymmetricKey(&xAzureIoTProvisioningClient, (const uint8_t *)DEVICE_SYMMETRIC_KEY,
                                                   sizeof(DEVICE_SYMMETRIC_KEY) - 1, Crypto_HMAC);
    if(xResult != eAzureIoTSuccess)
    {
        goto exit;
    }
#endif /* EVSE_SAS_AUTH */

    xResult = AzureIoTProvisioningClient_SetRegistrationPayload(&xAzureIoTProvisioningClient,
                                                                (const uint8_t *)sampleazureiotPROVISIONING_PAYLOAD,
                                                                sizeof(sampleazureiotPROVISIONING_PAYLOAD) - 1);
    if(xResult != eAzureIoTSuccess)
    {
        goto exit;
    }

    do
    {
        xResult = AzureIoTProvisioningClient_Register(&xAzureIoTProvisioningClient,
                                                      sampleazureiotProvisioning_Registration_TIMEOUT_MS);
    } while (xResult == eAzureIoTErrorPending);

    if (xResult == eAzureIoTSuccess)
    {
        LogInfo(("Successfully acquired IoT Hub name and Device ID"));
    }
    else
    {
        LogInfo(("Error getting IoT Hub name and Device ID: 0x%08x", (uint16_t)xResult));
        goto exit;
    }

    xResult = AzureIoTProvisioningClient_GetDeviceAndHub(&xAzureIoTProvisioningClient, ucSampleIotHubHostname,
                                                         &ucSamplepIothubHostnameLength, ucSampleIotHubDeviceId,
                                                         &ucSamplepIothubDeviceIdLength);
    if(xResult != eAzureIoTSuccess)
    {
        goto exit;
    }

    AzureIoTProvisioningClient_Deinit(&xAzureIoTProvisioningClient);

    /* Close the network connection.  */

    TLS_DISCONNECT(&xNetworkContext);

    if (EVSE_SE050_Set_ServerHostname((char*)ucSampleIotHubHostname, (uint32_t)ucSamplepIothubHostnameLength) == kStatus_Hostname_Failed)
    {
        LogError(("Could not store Hostname in Secure Element!\r\n"));
    }

    *ppucIothubHostname      = ucSampleIotHubHostname;
    *pulIothubHostnameLength = ucSamplepIothubHostnameLength;
    *ppucIothubDeviceId      = ucSampleIotHubDeviceId;
    *pulIothubDeviceIdLength = ucSamplepIothubDeviceIdLength;

    xResult = eAzureIoTSuccess;

exit:
    return xResult;
}

#endif /* ENABLE_DPS_SAMPLE */

/**
 * @brief Connect to server with backoff retries.
 */

static uint32_t prvConnectToServerWithBackoffRetries(const char *pcHostName,
                                                     uint32_t port,
                                                     NetworkCredentials_t *pxNetworkCredentials,
                                                     NetworkContext_t *pxNetworkContext)
{
    TlsTransportStatus_t xNetworkStatus;
    BackoffAlgorithmStatus_t xBackoffAlgStatus = BackoffAlgorithmSuccess;
    BackoffAlgorithmContext_t xReconnectParams;
    uint16_t usNextRetryBackOff = 0U;

    /* Initialize reconnect attempts and interval. */
    BackoffAlgorithm_InitializeParams(&xReconnectParams, sampleazureiotRETRY_BACKOFF_BASE_MS,
                                      sampleazureiotRETRY_MAX_BACKOFF_DELAY_MS, sampleazureiotRETRY_MAX_ATTEMPTS);

    /* Attempt to connect to IoT Hub. If connection fails, retry after
     * a timeout. Timeout value will exponentially increase till maximum
     * attempts are reached.
     */
    do
    {
        LogInfo(("Creating a TLS connection to %s:%u.\r\n", pcHostName, (uint16_t)port));


        /* Attempt to create a mutually authenticated TLS connection. */
        xNetworkStatus = TLS_CONNECT(pxNetworkContext, pcHostName, port, pxNetworkCredentials,
                                            sampleazureiotTRANSPORT_SEND_RECV_TIMEOUT_MS,
                                            sampleazureiotTRANSPORT_SEND_RECV_TIMEOUT_MS);

        if (xNetworkStatus != 0)
        {
            /* Generate a random number and calculate backoff value (in milliseconds) for
             * the next connection retry.
             * Note: It is recommended to seed the random number generator with a device-specific
             * entropy source so that possibility of multiple devices retrying failed network operations
             * at similar intervals can be avoided. */
            xBackoffAlgStatus = BackoffAlgorithm_GetNextBackoff(&xReconnectParams, EVSE_Random(), &usNextRetryBackOff);

            if (xBackoffAlgStatus == BackoffAlgorithmRetriesExhausted)
            {
                LogError(("Connection to the IoT Hub failed, all attempts exhausted."));
            }
            else if (xBackoffAlgStatus == BackoffAlgorithmSuccess)
            {
                LogWarn(
                    ("Connection to the IoT Hub failed [%d]. "
                     "Retrying connection with backoff and jitter [%d]ms.",
                     xNetworkStatus, usNextRetryBackOff));
                vTaskDelay(pdMS_TO_TICKS(usNextRetryBackOff));
            }
        }
    } while ((xNetworkStatus != 0) && (xBackoffAlgStatus == BackoffAlgorithmSuccess));

    return xNetworkStatus;
}

static void prvAzureCloudConnectionTask(void *pvParameters)
{
    EventBits_t uxBits;
    (void)pvParameters;
    uint8_t retries = 0;
    const TickType_t xTicksToWait = EVSE_WAIT_CLOUD_EVENTS / portTICK_PERIOD_MS;

    while (1)
    {
        uxBits = xEventGroupWaitBits(s_evse_event,
                                     EVSE_CLOUD_ALL_EVENTS,
                                     pdTRUE, pdFALSE,       /* Don't wait for all bits, either bit will do. */
                                     xTicksToWait);

        if (uxBits & (EVSE_INIT_CREDENTIALS_EVENT))
        {
            prvAzureCheckProvisioned();
        }

        if (uxBits & (EVSE_INITIALIZATION_EVENT))
        {
            prvAzureCloudConnection(true);
        }

        if ((uxBits & (EVSE_REINITIALIZATION_EVENT)) && (retries < IOT_HUB_MAX_RETRIES))
        {
            prvAzureCloudConnection(false);
            retries++;
        }

        if ((uxBits & (EVSE_TELEMETRY_SEND_EVENT)) && (EVSE_initialTelemetrySent))
        {
            prvAzureTelemetrySend();
        }

        prvMonitorConnection();

    }

    /* Shouldn't reach here, but to be sure */
    vTaskDelete(NULL);
}
/*-----------------------------------------------------------*/

const uint32_t EVSE_Cloud_GetTelemetryCount(void)
{
    return telemetryCounter;
}

const char *EVSE_Cloud_GetStringFromState(cloudConnectionState_t cloudState)
{
    return cloudConnectionStateString[cloudState];
}

cloudConnectionState_t EVSE_Cloud_GetConnectionState()
{
    configPRINTF((info("Cloud connection:%s."), cloudConnectionStateString[s_cloudConnectionState]));
    return s_cloudConnectionState;
}

void EVSE_Cloud_TelemetryReadyEvent(void)
{
    if (s_evse_event)
    {
        xEventGroupSetBits(s_evse_event, EVSE_TELEMETRY_SEND_EVENT);
    }
}

/*
 * @brief Create the task that demonstrates the AzureIoTHub demo
 */
void EVSE_Cloud_Init(void)
{
    BaseType_t xReturned = pdPASS;

    if (s_evse_event != NULL)
    {
        /* Already initialized */
        return;
    }

    /* Attempt to create the event group. */
    s_evse_event = xEventGroupCreate();

    if (s_evse_event == NULL)
    {
        xReturned = pdFAIL;
    }

    xEventGroupSetBits(s_evse_event, EVSE_INIT_CREDENTIALS_EVENT);

    if (xReturned == pdPASS)
    {
        /* This example uses a single application task, which in turn is used to
         * connect, subscribe, publish, unsubscribe and disconnect from the IoT Hub */
        xReturned = xTaskCreate(
            prvAzureCloudConnectionTask,            /* Function that implements the task. */
            "Azure Cloud Connectivity",           /* Text name for the task - only used for debugging. */
            APP_EVSE_AZURE_CONNECTIVITY_STACK_SIZE, /* Size of stack (in words, not bytes) to allocate for the task. */
            NULL,                                   /* Task parameter - not used in this case. */
            APP_EVSE_AZURE_CONNECTIVITY_PRIORITY,   /* Task priority, must be between 0 and configMAX_PRIORITIES - 1. */
            NULL); /* Used to pass out a handle to the created task - not used in this case. */
        if (xReturned != pdPASS)
        {
            configPRINTF((error(" %s Failed to create task "), __FUNCTION__));
        }
    }
    else
    {
        configPRINTF((error(" %s Failed to create event group "), __FUNCTION__));
    }

    if (xReturned != pdPASS)
    {
        while (1)
            ;
    }
}
/*-----------------------------------------------------------*/

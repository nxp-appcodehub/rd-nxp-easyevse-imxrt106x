/* Copyright (c) Microsoft Corporation.
 * Licensed under the MIT License. */

#ifndef AZURE_IOT_CONFIG_H
#define AZURE_IOT_CONFIG_H

/**
 *
 * Configuring middleware to use custom logging
 *
 * Note: below mapping assumes user providing LogE,
 *  LogW, LogI and LogD implementation.
 *
 * */
extern void vLoggingPrintf(const char *pcFormatString, ...);

#define AZLog(x) vLoggingPrintf x
#define AZLogError(message)                                  \
    AZLog(("[ERROR] [AZ IoT] [%s:%d]", __FILE__, __LINE__)); \
    AZLog(message);                                          \
    AZLog(("\r\n"))
#define AZLogWarn(message)                                  \
    AZLog(("[WARN] [AZ IoT] [%s:%d]", __FILE__, __LINE__)); \
    AZLog(message);                                         \
    AZLog(("\r\n"))
// #define AZLogInfo( message )     AZLog( ( "[INFO] [AZ IoT] [%s:%d]", __FILE__, __LINE__ ) ); AZLog( message ); AZLog(
// ( "\r\n" ) ) #define AZLogDebug( message )    AZLog( ( "[DEBUG] [AZ IoT] [%s:%d]", __FILE__, __LINE__ ) ); AZLog(
// message ); AZLog( ( "\r\n" ) )
#define AZLogInfo(message)
#define AZLogDebug(message)

/**
 *
 * Configuring middleware to use FreeRTOS logging
 *
 * */

#include "logging_levels.h"

#ifndef LIBRARY_LOG_NAME
#define LIBRARY_LOG_NAME "AzureIoTADUDemo"
#endif

/**
 * @brief The maximum number of retries for network operation with server.
 */
#define sampleazureiotRETRY_MAX_ATTEMPTS (5U)

/**
 * @brief The maximum back-off delay (in milliseconds) for retrying failed operation
 *  with server.
 */
#define sampleazureiotRETRY_MAX_BACKOFF_DELAY_MS (5000U)

/**
 * @brief The base back-off delay (in milliseconds) to use for network operation retry
 * attempts.
 */
#define sampleazureiotRETRY_BACKOFF_BASE_MS (500U)

/**
 * @brief Timeout for receiving CONNACK packet in milliseconds.
 */
#define sampleazureiotCONNACK_RECV_TIMEOUT_MS (10 * 1000U)

/**
 * @brief Date-time to use for the model id
 */
#define sampleazureiotDATE_TIME_FORMAT "%Y-%m-%dT%H:%M:%S.000Z"

/**
 * @brief Time in ticks to wait between each cycle of the demo implemented
 * by prvMQTTDemoTask().
 */
#define sampleazureiotDELAY_BETWEEN_DEMO_ITERATIONS_TICKS (pdMS_TO_TICKS(5000U))

/**
 * @brief Timeout for MQTT_ProcessLoop in milliseconds.
 */
#define sampleazureiotPROCESS_LOOP_TIMEOUT_MS (500U)

/**
 * @brief Delay (in ticks) between consecutive cycles of MQTT publish operations in a
 * demo iteration.
 *
 * Note that the process loop also has a timeout, so the total time between
 * publishes is the sum of the two delays.
 */
#define sampleazureiotDELAY_BETWEEN_PUBLISHES_TICKS (pdMS_TO_TICKS(2000U))

/**
 * @brief Transport timeout in milliseconds for transport send and receive.
 */
#define sampleazureiotTRANSPORT_SEND_RECV_TIMEOUT_MS (2000U)

/**
 * @brief Transport timeout in milliseconds for transport send and receive.
 */
#define sampleazureiotProvisioning_Registration_TIMEOUT_MS (3 * 1000U)

/**
 * @brief Wait timeout for subscribe to finish.
 */
#define sampleazureiotSUBSCRIBE_TIMEOUT (10 * 1000U)

/**
 * @brief Wait between doing constant poll on the mqtt process
 */
#define sampleazureiotconfigPOLL_WAIT_INTERVAL_MS (10U)

/**
 * @brief ROOT CA certificate used for registering with the DPS server and connecting to Azure IoT Hub.
 */
#define demoHubconfigROOT_CA_PEM                                           \
    "-----BEGIN CERTIFICATE-----\r\n"                                      \
    "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\r\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\r\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\r\n" \
    "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\r\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\r\n" \
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\r\n" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\r\n" \
    "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\r\n" \
    "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\r\n" \
    "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\r\n" \
    "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\r\n" \
    "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\r\n" \
    "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\r\n" \
    "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\r\n" \
    "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\r\n" \
    "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\r\n" \
    "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\r\n" \
    "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\r\n" \
    "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\r\n" \
    "MrY=\r\n"                                                             \
    "-----END CERTIFICATE-----\r\n"

/**
 * @brief Locally generated X509 certificate used for IotHub connection.
 */
#define X509_CERTIFICATE_PEM "paste here your X509 certificates"

/**
 * @brief Locally generated RSA key-pair used for IotHub connection.
 */
#define RSA_KEYPAIR_PEM "paste here your RSA key-pair"

#define SAMPLE_PNP_MODEL_ID                "dtmi:nxp:nxp;3"
#define sampleazureiotPROVISIONING_PAYLOAD "{\"modelId\":\"" SAMPLE_PNP_MODEL_ID "\"}"

/* Optional module ID.  */
#ifndef MODULE_ID
#define MODULE_ID "dtmi:nxp:nxp;3"
#endif /* MODULE_ID */
/**
 * @brief IoTHub endpoint port.
 */
#define democonfigIOTHUB_PORT 8883

#define democonfigNETWORK_BUFFER_SIZE 0x10000
#endif /* AZURE_IOT_CONFIG_H */

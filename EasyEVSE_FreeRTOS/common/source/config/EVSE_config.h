/**************************************************************************/
/*                                                                                                                              */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*       Copyright 2018-2024 NXP										  */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                                                                               */
/**************************************************************************/

#ifdef __cplusplus
extern   "C" {
#endif

#include "task_config.h"

#define FIRMWARE_VERSION_MAJOR  3U
#define FIRMWARE_VERSION_MINOR  0U
#define FIRMWARE_VERSION_HOTFIX 5U

/* ENABLE_ISO15118 contains also the J1772 basic charging */
#ifndef ENABLE_ISO15118
#define ENABLE_ISO15118 0
#endif /* ENABLE_ISO15118 */

#ifndef ENABLE_J1772
#define ENABLE_J1772 1
#endif /* ENABLE_J1772 */

#if ((ENABLE_ISO15118 == 1) && (ENABLE_J1772 == 1))
#error "ENABLE only one charging protocol. ISO15118 contains also BC J1772"
#endif

#if ENABLE_ISO15118 || ENABLE_J1772
#define ENABLE_CHARGING_PROTOCOL 1
#endif /* ENABLE_ISO15118 || ENABLE_J1772 */

#ifndef ENABLE_SIGBOARD
#define ENABLE_SIGBOARD      1
#endif /* ENABLE_SIGBOARD */

#ifndef ENABLE_ISO15118
#if ENABLE_SIGBOARD
#define ENABLE_ISO15118 1
#else
#define ENABLE_ISO15118 0
#endif /* ENABLE_SIGBOARD */
#endif /* ENABLE_ISO15118 */

#if (ENABLE_SIGBOARD == 0) && (ENABLE_ISO15118 == 1)
#error "ISO15118-2 needs sigboard"
#endif

#ifndef ENABLE_WIFI
#define ENABLE_WIFI          0
#endif /* ENABLE_WIFI */

#ifndef BOARD_NFC_ARDUINO_HEADER
#define BOARD_NFC_ARDUINO_HEADER 0
#endif /* BOARD_NFC_ARDUINO_HEADER */

#ifndef BOARD_SIGBOARD_ARDUINO_HEADER
#define BOARD_SIGBOARD_ARDUINO_HEADER 1
#endif /* BOARD_SIGBOARD_ARDUINO_HEADER */

#ifndef EASYEVSE_EV
#if ((ENABLE_WIFI == 1) && ((BOARD_NFC_ARDUINO_HEADER == 1) || (BOARD_SIGBOARD_ARDUINO_HEADER == 1)))
#error "When using WiFi, both BOARD_NFC_ARDUINO_HEADER and BOARD_SIGBOARD_ARDUINO_HEADER must be set to 0"
#endif /* ENABLE_WIFI == 1 && BOARD_NFC_ARDUINO_HEADER == 1 */
#endif /* EASYEVSE_EV */

/** The arduino interface exposes only one SPI chip select, because of that, only one
 * peripheral connected in the arduino interface that uses SPI can exist at a time.
 * To go around this limitation  check the user manual on where the other SPI interface is located.
 */
#ifndef EASYEVSE_EV
#if (BOARD_SIGBOARD_ARDUINO_HEADER == 1) && (BOARD_NFC_ARDUINO_HEADER == 1)
#error "Can't have both at the same time in the ARDUINO HEADER as they share the same CS pin"
#endif /* (BOARD_SIGBOARD_ARDUINO_HEADER == 1) && (BOARD_NFC_ARDUINO_HEADER == 1) */
#endif /* EASYEVSE_EV */

#define EVSE_CLOUD_TELEMETRY 1

#define EVSE_EDGELOCK_AGENT 1

/*************************************************************************************************/
/* This sample uses three different schemes to authenticate to IoT Central                                   */
/*  Select ONLY ONE authentication scheme by setting it to 1, the other two should be set to 0    */
/*************************************************************************************************/
#define EVSE_SAS_AUTH            0   /*  Set to 1 to use  : symmetric keys  */
#define EVSE_X509_AUTH           0   /*  Set to 1 to use  : certificate stored in i.MX RT106x Flash*/
#define EVSE_X509_SE050_AUTH     1   /*  Set to 1 to use  : certificate stored securely in SE050 Secure module */

#if (((EVSE_SAS_AUTH == EVSE_X509_AUTH) && (EVSE_SAS_AUTH == 1)) || ((EVSE_X509_AUTH == EVSE_X509_SE050_AUTH) && (EVSE_X509_AUTH == 1)) || ((EVSE_X509_SE050_AUTH == EVSE_SAS_AUTH) && (EVSE_X509_SE050_AUTH == 1)))
#error "Can't have more than one connection security method enabled at a time."
#endif /* EVSE_X509_SE050_AUTH */

#if ((EVSE_SAS_AUTH == 0) && (EVSE_X509_AUTH == 0) && (EVSE_X509_SE050_AUTH == 0))
#error "No security method enabled."
#endif

#if ((EVSE_SAS_AUTH == 1) || (EVSE_X509_AUTH == 1))
#undef EVSE_EDGELOCK_AGENT
#define EVSE_EDGELOCK_AGENT 0
#endif /* (EVSE_SAS_AUTH == 1) || (EVSE_X509_AUTH == 1) */

#if (EVSE_EDGELOCK_AGENT == 1)
/* The hostname to use for the connection to the EdgeLock 2GO cloud service.
 * This is specific to your customer account, the needed value can be obtained in the GUI of the
 * EdgeLock 2GO cloud service in the company settings of the account. */
#define EDGELOCK2GO_HOSTNAME "copy here your edgelock2go hostname"
/* The port to use for the communication to the EdgeLock 2GO cloud service. */
#define EDGELOCK2GO_PORT 443U

#endif /* EVSE_EDGELOCK_AGENT */

#if    ( EVSE_SAS_AUTH == 1)
    #define USE_DEVICE_SYMMETRIC_KEY 1 /* do not modify */
                                       /* after the device has been registered and  provisioned with DPS, */
                                       /* the DEVICE_ID is the same as REGISTRATION ID                          */
#define DEVICE_ID "paste here your device ID"
/* Paste the symmetric key value from the device connection prompt in the Azure Iot Central Application */
#define DEVICE_SYMMETRIC_KEY "copy here your device symmetric key"
#elif ( EVSE_X509_AUTH == 1)
    #define USE_DEVICE_CERTIFICATE 1
#elif (EVSE_X509_SE050_AUTH == 1 )
#define USE_SE05x_DEVICE_CERTIFICATE 1 /** do not modify this define*/
#endif /* Authentication scheme selection */

/*******************************************************************************************/
/* Set to 1 to contact DPS service or 0 to connect to application directly                              */
/* If this is the first time the device is registered, you must set this define to 1,         */
/* After a successful registration, you may change it to 0, reducing subsequent        */
 /* device connection times                                                                                                            */
/******************************************************************************************/
#define EVSE_DPS    1

#if (EVSE_DPS == 1)

#define ENABLE_DPS_SAMPLE 1 /** do not modify the ENDPOINT define */

/*  Fixed DPS endpoint, we recommended to leave it as it is, unless you have a private DPS endpoint */
#define ENDPOINT "global.azure-devices-provisioning.net"

/* Paste your device ID_SCOPE from the Azure Iot Central application */
#define ID_SCOPE "copy here your device ID scope"

/* Use the same registration ID that you used to register the device in the Azure Iot Central application
*  Please note that  for X509 enrollment registration ID  must match device certificate CN */
#if (EVSE_SAS_AUTH == 1)
#define REGISTRATION_ID DEVICE_ID
#else
#define REGISTRATION_ID ""
#endif

#elif (EVSE_DPS == 0)
/* Required when DPS is not used.  */
/******************************************************************************************/
 /* These values can be picked from device connection string which is of format :           */
 /* HostName=<host1>;DeviceId=<device1>;SharedAccessKey=<key1>                           */
 /* HOST_NAME can be set to <host1>,                                                                                      */
 /* DEVICE_ID can be set to <device1>,                                                                                       */
 /* DEVICE_SYMMETRIC_KEY can be set to <key1>.                                                                  */
/*  The Host name string  can be picked from the Log terminal                                            */
/******************************************************************************************/

/* Define HOST_NAME in case of not using SE05X Secure Element secure storage*/
#define HOST_NAME                                   "copy your host name or address of your Azure Iot central application here" /***/

#endif /* DPS selection */

#define EVSE_HOSTNAME_MAX_BUFFER       128
#define EVSE_DEVICEID_MAX_BUFFER       128

#ifndef EVSE_SNTP_SERVER_NAME
#define EVSE_SNTP_SERVER_NAME "0.pool.ntp.org" /* SNTP Server.  */
#endif /* SAMPLE_SNTP_SERVER_NAME */

#ifndef EVSE_SNTP_SYNC_MAX
#define EVSE_SNTP_SYNC_MAX 9	/* Max number of SNTP server connection attempts in case of unsuccessful connection */
#endif /* SAMPLE_SNTP_SYNC_MAX */

#ifndef EVSE_SNTP_UPDATE_MAX
#define EVSE_SNTP_UPDATE_MAX 10	/* Max number of SNTP time sync attempts */
#endif /* SAMPLE_SNTP_UPDATE_MAX */

#ifndef EVSE_SNTP_UPDATE_INTERVAL
#define EVSE_SNTP_UPDATE_INTERVAL (200)	/* Time between successive attempts */
#endif /* SAMPLE_SNTP_UPDATE_INTERVAL */

#define RED_TEXT(x) "\033[31;1m" x "\033[0m"
#define GREEN_TEXT(x) "\033[32;1m" x "\033[0m"
#define YELLOW_TEXT(x) "\033[33;1m" x "\033[0m"
#define BLUE_TEXT(x) "\033[36;1m" x "\033[0m"

#define error(x)	RED_TEXT(x)
#define success(x)	GREEN_TEXT(x)
#define warning(x)	YELLOW_TEXT(x)
#define info(x)		BLUE_TEXT(x)

#ifdef __cplusplus
}
#endif

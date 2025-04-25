/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef CONFIG_EVSE_CONNECTIVITYCONFIG_H_
#define CONFIG_EVSE_CONNECTIVITYCONFIG_H_

#include "EVSE_config.h"

#if ENABLE_WIFI
#define WIFI_SSID "copy here your network ssid"
#define WIFI_PASS "copy here your network password"
#endif

#define EVSE_CLOUD_TELEMETRY 1

#define EVSE_EDGELOCK_AGENT 1

#if (ENABLE_SE == 0)
#undef EVSE_EDGELOCK_AGENT
#define EVSE_EDGELOCK_AGENT 0
#endif /* (ENABLE_SE == 0)*/

#if (EVSE_EDGELOCK_AGENT == 1)
/* The hostname to use for the connection to the EdgeLock 2GO cloud service.
 * This is specific to your customer account, the needed value can be obtained in the GUI of the
 * EdgeLock 2GO cloud service in the company settings of the account. */
#define EDGELOCK2GO_HOSTNAME "copy here your edgelock2go hostname"
/* The port to use for the communication to the EdgeLock 2GO cloud service. */
#define EDGELOCK2GO_PORT 443U

#endif /* EVSE_EDGELOCK_AGENT */

#if (EVSE_SAS_AUTH == 1)
#define USE_DEVICE_SYMMETRIC_KEY 1 /* do not modify */
                                   /* after the device has been registered and  provisioned with DPS, */
                                   /* the DEVICE_ID is the same as REGISTRATION ID                          */
#define DEVICE_ID "paste here your device ID"
/* Paste the symmetric key value from the device connection prompt in the Azure Iot Central Application */
#define DEVICE_SYMMETRIC_KEY "copy here your device symmetric key"
#elif (EVSE_X509_AUTH == 1)
#define USE_DEVICE_CERTIFICATE 1
#elif (EVSE_X509_SE050_AUTH == 1)
#define USE_SE05x_DEVICE_CERTIFICATE 1 /** do not modify this define*/
#endif                                 /* Authentication scheme selection */

/*******************************************************************************************/
/* Set to 1 to contact DPS service or 0 to connect to application directly                              */
/* If this is the first time the device is registered, you must set this define to 1,         */
/* After a successful registration, you may change it to 0, reducing subsequent        */
/* device connection times */
/******************************************************************************************/
#define EVSE_DPS 1

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
/* HOST_NAME can be set to <host1>, */
/* DEVICE_ID can be set to <device1>, */
/* DEVICE_SYMMETRIC_KEY can be set to <key1>.                                                                  */
/*  The Host name string  can be picked from the Log terminal                                            */
/******************************************************************************************/

/* Define HOST_NAME in case of not using SE05X Secure Element secure storage*/
#define HOST_NAME "copy your host name or address of your Azure Iot central application here" /***/

#endif                                                                                        /* DPS selection */

#define EVSE_HOSTNAME_MAX_BUFFER 128
#define EVSE_DEVICEID_MAX_BUFFER 128

#ifndef EVSE_SNTP_SERVER_NAME
#define EVSE_SNTP_SERVER_NAME "0.pool.ntp.org" /* SNTP Server.  */
#endif                                         /* SAMPLE_SNTP_SERVER_NAME */

#ifndef EVSE_SNTP_SYNC_MAX
#define EVSE_SNTP_SYNC_MAX 9 /* Max number of SNTP server connection attempts in case of unsuccessful connection */
#endif                       /* SAMPLE_SNTP_SYNC_MAX */

#ifndef EVSE_SNTP_UPDATE_MAX
#define EVSE_SNTP_UPDATE_MAX 10 /* Max number of SNTP time sync attempts */
#endif                          /* SAMPLE_SNTP_UPDATE_MAX */

#ifndef EVSE_SNTP_UPDATE_INTERVAL
#define EVSE_SNTP_UPDATE_INTERVAL (200) /* Time between successive attempts */
#endif                                  /* SAMPLE_SNTP_UPDATE_INTERVAL */

#endif                                  /* CONFIG_EVSE_CONNECTIVITYCONFIG_H_ */

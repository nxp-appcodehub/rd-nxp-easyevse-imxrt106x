/*
 * Copyright 2024-2026 NXP
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

#ifndef EVSE_EDGELOCK_AGENT
#define EVSE_EDGELOCK_AGENT 1
#endif

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

/* URL of the OCPP server (CSMS). The URL string has the following format:
 * url="ws://<CSMS IP or hostname>:<CSMS port>"
 * Security level 1: url="ws://<CSMS IP or hostname>:8082"
 * Security level 2: url="wss://<CSMS IP or hostname>:8443"
 * Security level 3: url="wss://<CSMS IP or hostname>:8444" */
#define EVSE_OCPP_SERVER_URL     "paste your ocpp server address here"
/* Charge point ID as registered with the CSMS */
#define CHARGE_POINT_ID "paste your charge point id here"
/* Charge point password to be used for HTTP authentication with security level 1 and 2 */
#define CHARGE_POINT_AUTH_PASSWORD "paste your http auth password here"
/* Security level used for the connection to the server:
 * Level 1: Basic HTTP Authentication
 * Level 2: TLS with Basic HTTP Authentication
 * Level 3: TLS (mutual authentication) */
#define EVSE_OCPP_SECURITY_LEVEL 3
/* Timeout used when connectiong to the CSMS (in seconds) */
#define EVSE_OCPP_TIMEOUT_S      30
/* Don't sign meter values for now. */
#define EVSE_OCPP_SIGNED_METER_VALUES 0

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

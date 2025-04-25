/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "task_config.h"

#define FIRMWARE_VERSION_MAJOR  4U
#define FIRMWARE_VERSION_MINOR  0U
#define FIRMWARE_VERSION_HOTFIX 4U

/* ENABLE_ISO15118 contains also the J1772 basic charging */
#ifndef ENABLE_ISO15118
#define ENABLE_ISO15118 0
#endif /* ENABLE_ISO15118 */

#ifndef ENABLE_J1772
#define ENABLE_J1772 1
#endif /* ENABLE_J1772 */

#if ENABLE_ISO15118 || ENABLE_J1772
#define ENABLE_CHARGING_PROTOCOL 1
#endif /* ENABLE_ISO15118 || ENABLE_J1772 */

#ifndef ENABLE_SIGBOARD
#define ENABLE_SIGBOARD 1
#endif /* ENABLE_SIGBOARD */

#ifndef ENABLE_ISO15118
#if ENABLE_SIGBOARD
#define ENABLE_ISO15118 1
#else
#define ENABLE_ISO15118 0
#endif /* ENABLE_SIGBOARD */
#endif /* ENABLE_ISO15118 */

#ifndef BOARD_SIGBOARD_ETH
/* Cloud connection is done over ETH (default option) */
#ifndef ENABLE_ETH
#define ENABLE_ETH 1
#endif /* ENABLE_ETH */
/* Cloud connection is done over WiFi */
#ifndef ENABLE_WIFI
#define ENABLE_WIFI 0
#endif /* ENABLE_WIFI */
#endif

#ifndef BOARD_NFC_ARDUINO_HEADER
#define BOARD_NFC_ARDUINO_HEADER 0
#endif /* BOARD_NFC_ARDUINO_HEADER */

#ifndef BOARD_SIGBOARD_ARDUINO_HEADER
#define BOARD_SIGBOARD_ARDUINO_HEADER 0
#endif /* BOARD_SIGBOARD_ARDUINO_HEADER */

#if BOARD_SIGBOARD_ETH
/* It doesn't matter where you connect when using the BOARD_SIGBOARD_ETH */
#undef BOARD_SIGBOARD_ARDUINO_HEADER
#endif /* BOARD_SIGBOARD_ETH */

/*************************************************************************************************/
/* This sample uses three different schemes to authenticate to IoT Central                                   */
/*  Select ONLY ONE authentication scheme by setting it to 1, the other two should be set to 0    */
/*************************************************************************************************/
#define EVSE_SAS_AUTH        0 /*  Set to 1 to use  : symmetric keys  */
#define EVSE_X509_AUTH       0 /*  Set to 1 to use  : certificate stored in i.MX RT106x Flash*/
#define EVSE_X509_SE050_AUTH 1 /*  Set to 1 to use  : certificate stored securely in SE050 Secure module */

#define RED_TEXT(x)    "\033[31;1m" x "\033[0m"
#define GREEN_TEXT(x)  "\033[32;1m" x "\033[0m"
#define YELLOW_TEXT(x) "\033[33;1m" x "\033[0m"
#define BLUE_TEXT(x)   "\033[36;1m" x "\033[0m"

#define error(x)   RED_TEXT(x)
#define success(x) GREEN_TEXT(x)
#define warning(x) YELLOW_TEXT(x)
#define info(x)    BLUE_TEXT(x)

#ifdef __cplusplus
}
#endif

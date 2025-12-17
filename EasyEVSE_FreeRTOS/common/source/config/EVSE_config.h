/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "task_config.h"

#define FIRMWARE_VERSION_MAJOR  5U
#define FIRMWARE_VERSION_MINOR  0U
#define FIRMWARE_VERSION_HOTFIX 8U

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

#ifndef BOARD_SIGBOARD_ETH
/* Internet connection is done over ETH (default option) */
#ifndef ENABLE_ETH
#define ENABLE_ETH 1
#endif /* ENABLE_ETH */
/* Internet connection is done over WiFi */
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

#if EASYEVSE_EV
#define ENABLE_CLEV663_NFC  0
#define ENABLE_CONNECTIVITY 0
#define ENABLE_METER        0
#define ENABLE_SE           0
#define ENABLE_SHELL        1
#define ENABLE_OCPP         0
#if BUILD_EV_UI
#define ENABLE_EV_UI        1
#endif /* BUILD_EV_UI */
#else
#define ENABLE_CLEV663_NFC  0
#define ENABLE_CONNECTIVITY 1
#define ENABLE_METER        1
#define ENABLE_SE           1
#define ENABLE_SHELL        1
#define ENABLE_EVSE_UI      1
#define ENABLE_OCPP         1
#endif

#if (ENABLE_EVSE_UI || ENABLE_EV_UI)
#undef ENABLE_LCD
#define ENABLE_LCD 1
#endif

#ifndef PKCS11_SUPPORTED
#define PKCS11_SUPPORTED 1
#endif

#if (ENABLE_SE == 0)
#undef PKCS11_SUPPORTED
#define PKCS11_SUPPORTED 0
#endif /* (ENABLE_SE == 0)*/

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

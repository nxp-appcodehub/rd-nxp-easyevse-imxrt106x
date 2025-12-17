/*
 * Copyright 2024-2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#if (ENABLE_SIGBOARD == 0) && (ENABLE_ISO15118 == 1)
#error "ISO15118-2 needs sigboard"
#endif

#if (ENABLE_CONNECTIVITY == 1)
#if ((ENABLE_ETH == 0) && (ENABLE_WIFI == 0))
#error "Connectivity enabled, but no connectivity method defined"
#elif ((ENABLE_ETH == 1) && (ENABLE_WIFI == 1))
#error "Can't support both connectivity methods at the same time"
#endif /* ((ENABLE_ETH == 1) && (ENABLE_WIFI == 1)) */
#elif (ENABLE_CONNECTIVITY == 0)
#if ((ENABLE_ETH == 1) || (ENABLE_WIFI == 1))
#warning "Connectivity disabled, but connectivity method defined"
#endif /* ((ENABLE_ETH == 1) || (ENABLE_WIFI == 1)) */
#endif /* (ENABLE_CONNECTIVITY == 1) */

#if ((BOARD_SIGBOARD_ETH == 1) && (ENABLE_ETH == 1))
#error "BOARD_SIGBOARD_ETH build configuration uses the only Ethernet port, use the SPI configuration or Wi-Fi"
#endif /* BOARD_SIGBOARD_ETH */

#ifndef EASYEVSE_EV
#if ((ENABLE_WIFI == 1) && ((BOARD_NFC_ARDUINO_HEADER == 1) || (BOARD_SIGBOARD_ARDUINO_HEADER == 1)))
#error "When using WiFi, both BOARD_NFC_ARDUINO_HEADER and BOARD_SIGBOARD_ARDUINO_HEADER must be set to 0"
#endif /* ENABLE_WIFI == 1 && BOARD_NFC_ARDUINO_HEADER == 1 */
#endif /* EASYEVSE_EV */

#if (defined(BUILD_EV_UI) && (ENABLE_EV_UI == 0))
#error "EV_UI build config is selected, enable ENABLE_EV_UI"
#endif

#if (ENABLE_EVSE_UI && ENABLE_EV_UI)
#error "Choose only one UI version"
#endif

#if ((ENABLE_LCD == 0) && (ENABLE_EVSE_UI || ENABLE_EV_UI))
#error "Enable the display when enabling a UI"
#endif

#if (ENABLE_LCD && (ENABLE_EVSE_UI == 0) && (ENABLE_EV_UI == 0))
#error "Display enabled but no UI selected"
#endif

#if EASYEVSE_EV
#if(ENABLE_CLEV663_NFC || ENABLE_CONNECTIVITY || ENABLE_METER || ENABLE_SE)
#error "EV does not support NFC, connectivity, meter and SE. Please disable all of these modules."
#endif
#endif

#if ((defined(BUILD_EVSE) || defined(BUILD_EVSE_ETH) || defined(BUILD_EV)) && ENABLE_EV_UI)
#error "Enable ENABLE_EV_UI only for EV_UI build config"
#endif

#if ((defined(BUILD_EV_UI)|| defined(BUILD_EV)) && ENABLE_EVSE_UI)
#error "Disable ENABLE_EVSE_UI when building EV build configs"
#endif

#if ((ENABLE_ISO15118 == 1) && (ENABLE_J1772 == 1))
#error "ENABLE only one charging protocol. ISO15118 contains also BC J1772"
#endif

/** The arduino interface exposes only one SPI chip select, because of that, only one
 * peripheral connected in the arduino interface that uses SPI can exist at a time.
 * To go around this limitation  check the user manual on where the other SPI interface is located.
 */
#ifndef EASYEVSE_EV
#if (BOARD_SIGBOARD_ARDUINO_HEADER == 1) && (BOARD_NFC_ARDUINO_HEADER == 1)
#error "Can't have both at the same time in the ARDUINO HEADER as they share the same CS pin"
#endif /* (BOARD_SIGBOARD_ARDUINO_HEADER == 1) && (BOARD_NFC_ARDUINO_HEADER == 1) */
#endif /* EASYEVSE_EV */

#if (ENABLE_SE == 0)
#if (PKCS11_SUPPORTED == 1)
#error "PKCS11 needs SE05X enabled."
#endif
#if (EVSE_EDGELOCK_AGENT == 1)
#error "EdgeLock2GO agent needs SE05x enabled."
#endif
#endif /* ENABLE_SE == 0 */


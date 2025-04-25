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

#if (((EVSE_SAS_AUTH == EVSE_X509_AUTH) && (EVSE_SAS_AUTH == 1)) ||         \
     ((EVSE_X509_AUTH == EVSE_X509_SE050_AUTH) && (EVSE_X509_AUTH == 1)) || \
     ((EVSE_X509_SE050_AUTH == EVSE_SAS_AUTH) && (EVSE_X509_SE050_AUTH == 1)))
#error "Can't have more than one connection security method enabled at a time."
#endif /* EVSE_X509_SE050_AUTH */

#if ((EVSE_SAS_AUTH == 0) && (EVSE_X509_AUTH == 0) && (EVSE_X509_SE050_AUTH == 0))
#error "No security method enabled."
#endif

#if (ENABLE_SE == 0)
#if (PKCS11_SUPPORTED == 1)
#error "PKCS11 needs SE05X enabled."
#endif
#if (EVSE_EDGELOCK_AGENT == 1)
#error "EdgeLock2GO agent needs SE05x enabled."
#endif
#if ((ENABLE_CONNECTIVITY == 1) && (EVSE_X509_SE050_AUTH == 1))
#error "Authentication method needs SE05X enabled."
#endif
#endif /* ENABLE_SE == 0 */


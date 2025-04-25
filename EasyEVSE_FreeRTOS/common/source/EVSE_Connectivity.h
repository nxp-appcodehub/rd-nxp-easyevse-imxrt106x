/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_CONNECTIVITY_H_
#define EVSE_CONNECTIVITY_H_

#if ENABLE_WIFI
#include "flash_fs.h"
#endif /* ENABLE_WIFI */

#if ENABLE_WIFI
#define SSID_MAX_SIZE 32
#define PASS_MAX_SIZE 64
#endif /* ENABLE_WIFI */

#define UPDATE_LOCALTIME_PERIOD_MS 500

typedef enum _connectionState
{
    EVSE_Network_NotConnected = 0,
    EVSE_Network_InitComunicationModule,
    EVSE_Network_InitComunicationModuleError,
    EVSE_Network_InitCommunicationStack,
    EVSE_Network_InitCommunicationStackError,
    EVSE_Network_APConnectTry,
    EVSE_Network_APConnected,
    EVSE_Network_APConnectError,
    EVSE_Network_DHCPRequest,
    EVSE_Network_DHCPError,
    EVSE_Network_SNTPRequest,
    EVSE_Network_SNTPError,
    EVSE_Network_NetworkConnected,
    EVSE_Network_NetworkDisconnected,
    EVSE_Network_Last
} connectionState_t;

typedef enum _connectionModule
{
    EVSE_ConnectionModule_None,
    EVSE_ConnectionModule_ETH,
    EVSE_ConnectionModule_WiFi,
} connectionModule_t;

#if ENABLE_WIFI

typedef struct
{
    uint8_t name[SSID_MAX_SIZE];
    uint8_t length;
} evse_wifi_ssid_t;

typedef struct
{
    uint8_t name[PASS_MAX_SIZE];
    uint8_t length;
} evse_wifi_pass_t;

typedef struct
{
    evse_wifi_ssid_t ssid;
    evse_wifi_pass_t pass;
} evse_wifi_cred_t;

#endif /* ENABLE_WIFI */

/**
 * Get if the application enables the wifi or the eth
 * @return
 */
const connectionModule_t EVSE_Connectivity_GetConnectionModule();

/**
 * Convert the connectivity state into a readable string
 * @param connectivityState connectivity state for which to get the string
 * @return return a constant string.
 */
const char *EVSE_Connectivity_GetStringFromState(connectionState_t connectivityState);

/**
 * Get the current connection state
 * @return the current connection state
 */
connectionState_t EVSE_Connectivity_GetConnectionState(void);

/**
 * Get Internet connection status.
 * @return true if it is connected to the Internet, false otherwise
 */
bool EVSE_Connectivity_IsConnectedToInternet(void);

/**
 * Call this function to set the UNIX time if obtained from an external method
 * @param sec Number of seconds which have passed since 00:00:00 UTC on Thursday, 1 January 1970
 */
void EVSE_Connectivity_SetUnixTime(uint32_t sec);

/**
 * Get the Unix Time
 * @return seconds which have passed since 00:00:00 UTC on Thursday, 1 January 1970
 */
uint32_t EVSE_Connectivity_GetUnixTime(void);

/*
 * Return the IP address.
 * Retuns NULL if the connection is not complete
 */
const char *EVSE_Connectivity_GetIPv4(void);

/**
 *  Connectivity task entry point.
 * Handle connection to an access point
 */
void EVSE_Connectivity_Init(void);

/* API Available only in the WIFI configuration */
#if (ENABLE_WIFI == 1)

/**
 * Return a const char to the AP SSID
 */
const char *EVSE_Connectivity_GetAPName(void);

/**
 * Read wifi credentials stored in flash.
 * @param wifi_cred pointer to the struct that saves the credentials
 * @return FLASH_FS_OK on success, an error code otherwise.
 */
flash_fs_status_t EVSE_Connectivity_Wifi_Credentials_Flash_Read(evse_wifi_cred_t *wifi_cred);

/**
 * Save wifi credentials in flash.
 * @param wifi_cred pointer to the struct with the credentials to be saved
 * @return FLASH_FS_OK on success, an error code otherwise.
 */
flash_fs_status_t EVSE_Connectivity_Wifi_Credentials_Flash_Save(evse_wifi_cred_t *wifi_cred);

/**
 * Erase the wifi credentials stored in flash.
 * @return FLASH_FS_OK on success, an error code otherwise.
 */
flash_fs_status_t EVSE_Connectivity_Wifi_Credentials_Flash_Erase(void);

#endif /* ENABLE_WIFI */

#endif /* EVSE_CONNECTIVITY_H_ */

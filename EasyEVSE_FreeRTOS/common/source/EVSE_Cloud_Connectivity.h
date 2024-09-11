/*
 * Copyright 2023-2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/**
 * @brief Defines an interface to be used by samples when interacting with azure sample base modules.
 */

#ifndef AZURE_SAMPLE_CONNECTION_H
#define AZURE_SAMPLE_CONNECTION_H

#include <stdbool.h>
#include <stdint.h>

/**********************
 *      TYPEDEFS
 **********************/
typedef enum _cloud_events
{
    EVSE_CONNECT_EVENT                       = 1 << 1,
    EVSE_INIT_CREDENTIALS_EVENT              = 1 << 2,
    EVSE_INITIALIZATION_EVENT                = 1 << 3,
    EVSE_REINITIALIZATION_EVENT              = 1 << 4,
    EVSE_METHOD_MESSAGE_EVENT                = 1 << 5,
    EVSE_DEVICE_TWIN_GET_EVENT               = 1 << 6,
    EVSE_DEVICE_TWIN_DESIRED_PROPERTY_EVENT  = 1 << 7,
    EVSE_TELEMETRY_SEND_EVENT                = 1 << 8,
    EVSE_DEVICE_TWIN_REPORTED_PROPERTY_EVENT = 1 << 9,
    EVSE_DISCONNECT_EVENT                    = 1 << 10,
    EVSE_RECONNECT_EVENT                     = 1 << 11,
    EVSE_CONNECTED_EVENT                     = 1 << 12,
    EVSE_CLOUD_ALL_EVENTS                    = 0xFFFFFF,
} cloud_events_t;

typedef enum _cloudConnectionState
{
    EVSE_Cloud_NotConnected = 0,
    EVSE_Cloud_EdgeLock_Request,
    EVSE_Cloud_EdgeLock_Error,
    EVSE_Cloud_DPSRequest,
    EVSE_Cloud_DPSError,
    EVSE_Cloud_HubRequest,
    EVSE_Cloud_HubError,
    EVSE_Cloud_HubDisconnected,
    EVSE_Cloud_HubReconnection,
    EVSE_Cloud_HubConnected,
    EVSE_Cloud_Last
} cloudConnectionState_t;

/**
 * @brief Convert the cloud state into a readable string
 * @param cloudState cloud state for which to get the string
 * @return return a constant string
 */
const char *EVSE_Cloud_GetStringFromState(cloudConnectionState_t cloudState);

/**
 * @brief Get the current cloud connection state
 * @return the current cloud connection state
 */
cloudConnectionState_t EVSE_Cloud_GetConnectionState();

/**
 *  @brief Set an event for the Cloud connectivity task, to send telemetry data.
 */
void EVSE_Cloud_TelemetryReadyEvent(void);

/**
 * @brief Creates the Cloud connectivity Task, initializes internal components.
 */
void EVSE_Cloud_Init(void);

/**
 * @brief Get the number of telemetry packets sent to the cloud.
 */
const uint32_t EVSE_Cloud_GetTelemetryCount(void);

#endif /* AZURE_SAMPLE_CONNECTION_H */

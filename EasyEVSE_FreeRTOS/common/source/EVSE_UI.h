/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_UI_H_
#define EVSE_UI_H_

typedef enum _ui_events
{
    EVSE_UI_TelemetryData         = 1 << 1,
    EVSE_UI_NFC                   = 1 << 2,
    EVSE_UI_NetworkStatus         = 1 << 3,
    EVSE_UI_ChargingSession       = 1 << 4,
    EVSE_UI_OcppStatus            = 1 << 5,
	EVSE_UI_OcppDisplayMessage    = 1 << 6,
    EVSE_UI_ISO15118_Stack_Status = 1 << 7,
    EVSE_UI_RequestedPower        = 1 << 8,
    EVSE_UI_UpdateLocalTime       = 1 << 9,
    EVSE_UI_NFCActivation         = 1 << 10,
    /* Internal use only */
    EVSE_UI_RefreshScreen = 0xF00000,
    EVSE_UI_ALL_EVENTS    = 0xFFFFFF,
} ui_events_t;

/**
 * Set events for the UI task. Before calling EVSE_UI_SetEvent call EVSE_UI_Init.
 * If EVSE_UI_Init is not called this function does nothing.
 * @param event EVSE_UI_TelemetryData - the UI task will refresh the telemetry data
 * EVSE_UI_NFC - the UI will refresh the NFC data
 * EVSE_UI_Network - The UI will refresh the Network data
 */
void EVSE_UI_SetEvent(ui_events_t event);

#if ENABLE_LCD
/**
 * Creates refresh timer, creates UI task
 */
void EVSE_UI_Init(void);
#endif

#endif /* EVSE_UI_H_ */

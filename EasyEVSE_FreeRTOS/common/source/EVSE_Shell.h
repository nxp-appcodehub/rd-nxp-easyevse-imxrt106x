/*
 * Copyright 2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#if (ENABLE_SHELL == 1)

#ifndef EVSE_SHELL_H_
#define EVSE_SHELL_H_

typedef enum _shell_events
{
    EVSE_SHELL_WIFI_EVENT    = 1 << 1,
    EVSE_SHELL_VERSION_EVENT = 1 << 2,
    EVSE_SHELL_SE05X_EVENT   = 1 << 3,
    EVSE_SHELL_AUTH_EVENT    = 1 << 4,
    EVSE_SHELL_PAYMENT_EVENT = 1 << 5,
    EVSE_SHELL_ALL_EVENTS    = 0xFFFFFF,
} shell_events_t;

/* Entry point for EVSE shell.
 * Handles initialization of the USB task.*/
void EVSE_Shell_Init(void);

#endif /* EVSE_SHELL_H_ */

#endif /* ENABLE_SHELL */

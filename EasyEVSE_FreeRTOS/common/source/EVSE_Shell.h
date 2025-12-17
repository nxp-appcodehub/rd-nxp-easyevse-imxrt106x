/*
 * Copyright 2024-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#if (ENABLE_SHELL == 1)

#ifndef EVSE_SHELL_H_
#define EVSE_SHELL_H_

#if EASYEVSE_EV
#define SHELL_PROMPT   "EV_SHELL>> "
#else
#define SHELL_PROMPT "EVSE_SHELL>> "
#endif
#define COMMAND_ERROR "Incorrect use of CLI command. Please check \'help\' menu and retry\r\n"

typedef enum _shell_events
{
    EVSE_SHELL_WIFI_EVENT       = 1 << 1,
    EVSE_SHELL_VERSION_EVENT    = 1 << 2,
    EVSE_SHELL_SE05X_EVENT      = 1 << 3,
    EVSE_SHELL_AUTH_EVENT       = 1 << 4,
    EVSE_SHELL_PAYMENT_EVENT    = 1 << 5,
    EVSE_SHELL_OCPP_INFO_EVENT  = 1 << 6,
    EVSE_SHELL_CHARGING_EVENT   = 1 << 7,
    EV_SHELL_CONNECT_DISCONNECT_CP_EVENT = 1 << 8,
    EV_SHELL_START_STOP_CHARGING_EVENT   = 1 << 9,
    EV_SHELL_CHANGE_AUTH_METHOD_EVENT    = 1 << 10,
    EV_SHELL_CHANGE_PROTOCOL_EVENT       = 1 << 11,
    EV_SHELL_RESET_BATTERY_LEVEL_EVENT   = 1 << 12,
    EV_SHELL_CHANGE_CHARGING_DIRECTION_EVENT   = 1 << 13,
    EV_SHELL_VERSION_EVENT = 1 << 14,
    EVSE_SHELL_ALL_EVENTS       = 0xFFFFFF,
} shell_events_t;

/* Entry point for EVSE shell.
 * Handles initialization of the USB task.*/
void EVSE_Shell_Init(void);

/* Printf function that writes output to the shell output stream.*/
int EVSE_Shell_Printf(const char *formatString, ...);

void EVSE_Shell_SetCLIAuth(bool CLI_auth);

bool EVSE_Shell_AllowAuth(bool *auth_result);

#endif /* EVSE_SHELL_H_ */

#endif /* ENABLE_SHELL */

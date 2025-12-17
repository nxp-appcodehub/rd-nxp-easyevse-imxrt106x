/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <sys/time.h>
#include <errno.h>
#include "FreeRTOS.h"
#include "task.h"

extern uint32_t EVSE_Connectivity_GetUnixTime(void);

// OCPP reasonable epoch: 2024-01-01 00:00:00 UTC
//#define OCPP_EPOCH_START 0UL //1704067200UL

int _gettimeofday_r(struct _reent *ptr, struct timeval *tv, struct timezone *tz)
{
    (void)ptr;
    (void)tz;

    if (tv == NULL) {
        return -1;
    }

    // Get milliseconds since boot
    TickType_t ticks = xTaskGetTickCount();
    uint64_t ms_since_boot = (uint64_t)ticks * portTICK_PERIOD_MS;

    // Convert to Unix timestamp
    //tv->tv_sec = (time_t)(OCPP_EPOCH_START + (ms_since_boot / 1000));
    tv->tv_sec = (time_t)EVSE_Connectivity_GetUnixTime();
    tv->tv_usec = (suseconds_t)((ms_since_boot % 1000) * 1000);

    return 0;
}

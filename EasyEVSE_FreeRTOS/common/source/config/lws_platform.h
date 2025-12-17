/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef _LWS_PLATFORM_H_
#define _LWS_PLATFORM_H_

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#ifndef LWS_VISIBLE
#define LWS_VISIBLE
#endif
#ifndef LWS_EXTERN
#define LWS_EXTERN extern
#endif
#ifndef LWS_INLINE
#define LWS_INLINE inline
#endif
#ifndef LWS_WARN_UNUSED_RESULT
#define LWS_WARN_UNUSED_RESULT
#endif

#define LWS_TIMEO_MS(x) ((x) / portTICK_PERIOD_MS)

static inline void lws_platform_sleep_ms(int ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

#endif /* _LWS_PLATFORM_H_ */


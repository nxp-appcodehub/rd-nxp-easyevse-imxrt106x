/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include "message_core.h"
#include "fsl_common_arm.h"

uint32_t primask;

status_t MESSAGE_LOCK()
{
    primask = DisableGlobalIRQ();
    return kStatus_Success;
}

status_t MESSAGE_UNLOCK()
{
    EnableGlobalIRQ(primask);
    return kStatus_Success;
}

status_t MESSAGE_LOCKINIT()
{
    primask = 0;
    return kStatus_Success;
}

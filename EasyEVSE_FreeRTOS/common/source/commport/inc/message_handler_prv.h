/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MESSAGE_HANDLER_PRV_H
#define MESSAGE_HANDLER_PRV_H

#include "stdint.h"

#define MAX_PENDING_REQUEST 4

typedef struct
{
    uint16_t msg_id;
    void *respond_msg;
    bool in_use;
} request_entry_t;

#endif /* MESSAGE_HANDLER_PRV_H */

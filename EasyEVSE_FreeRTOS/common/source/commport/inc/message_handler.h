/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "message_descriptor.pb.h"

typedef enum _MessageStatus
{
    MessageHandler_Success          = 0,
    MessageHandler_Fail             = -1,
    MessageHandler_NotImplemented   = -3,
    MessageHandler_InvalidParameter = -2,
} MessageStatus;

typedef struct _MessageHandlerCallbacks
{
    MessageStatus (*on_request)(const uint16_t msg_id, const request_t *req, respond_t *respond);
    MessageStatus (*on_respond)(const uint16_t msg_id, const uint16_t correlation_id, const  respond_t *rsp);
} MessageHandlerCallbacks;

MessageStatus MessageHandler_Init(const MessageHandlerCallbacks *message_callback_handler, device_type_t device_type);

MessageStatus MessageHandler_Message(const e_meter_message_crc_t *message);

#endif /* MESSAGE_HANDLER_H */

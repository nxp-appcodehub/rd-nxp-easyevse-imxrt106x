/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "message_core.h"
#include "message_raw.h"
#include "message_transport_pal.h"
#include "message_handler.h"
#include "message_handler_prv.h"
#include "message_pool.h"
#include "message_builder.h"
static MessageHandlerCallbacks s_message_handler_callback = {0};
static device_type_t s_device_type                        = DEVICE_TYPE_DEVICE_NOT_SET;
static request_entry_t request_table[MAX_PENDING_REQUEST] = {0};

static void set_correlation_id(const uint16_t msg_id, e_meter_message_t *message)
{
    if (s_device_type != DEVICE_TYPE_DEVICE_NOT_SET)
    {
        message->metadata.has_device_type = true;
        message->metadata.device_type     = s_device_type;
    }

    message->metadata.has_correlation_id = true;
    message->metadata.correlation_id     = msg_id;
}

static void *MessageHandler_GetMessageFromMsgId(const uint16_t msg_id)
{
    /* Check if the message is already in the queue */
    for (uint8_t i = 0; i < MAX_PENDING_REQUEST; i++)
    {
        if (request_table[i].msg_id == msg_id)
        {
            return request_table[i].respond_msg;
        }
    }
    return NULL;
}

static status_t MessageHandler_RegisterRequest(const uint16_t msg_id, void *respond_msg)
{
    status_t status = kStatus_Fail;

    /* Check if the message is already in the queue */
    for (uint8_t i = 0; i < MAX_PENDING_REQUEST; i++)
    {
        if (request_table[i].msg_id == msg_id)
        {
            status = kStatus_Fail;
            break;
        }
        else if (request_table[i].in_use == false)
        {
            /* If the message is not in the queue, occupy the slot */
            request_table[i].msg_id      = msg_id;
            request_table[i].respond_msg = respond_msg;
            request_table[i].in_use      = true;
            status                       = kStatus_Success;
            break;
        }
    }

    return status;
}

static status_t MessageHandler_RemoveRequest(const uint16_t msg_id)
{
    status_t status = kStatus_Fail;

    /* Check if the message is  in the queue */
    for (uint8_t i = 0; i < MAX_PENDING_REQUEST; i++)
    {
        if (request_table[i].msg_id == msg_id)
        {
            request_table[i].msg_id      = INVALID_MESSAGE_ID;
            request_table[i].respond_msg = NULL;
            request_table[i].in_use      = false;
            status                       = kStatus_Success;
            break;
        }
    }
    return status;
}

static MessageStatus MessageHandler_Request(const uint16_t msg_id, const request_t *request, respond_t *respond_message)
{
    MessageStatus handler_status = MessageHandler_Fail;
    if (s_message_handler_callback.on_request)
    {
        handler_status = s_message_handler_callback.on_request(msg_id, request, respond_message);
    }
    respond_message->command_id = request->command_id;

    return handler_status;
}

static MessageStatus MessageHandler_Respond(const uint16_t msg_id, const uint16_t correlation_id, const respond_t *respond)
{
    MessageStatus handler_status = MessageHandler_Fail;
    if (s_message_handler_callback.on_respond)
    {
        handler_status =  s_message_handler_callback.on_respond(msg_id, correlation_id, respond);
    }
    return handler_status;
}

static MessageStatus MessageHandler_MessagePayload(const uint16_t msg_id, const e_meter_message_t *message_payload)
{
    /* TO DO , put the response in the same message */
    MessageStatus handler_status = MessageHandler_Success;

    if (message_payload == NULL)
    {
        return MessageHandler_InvalidParameter;
    }

    if (message_payload->which_message_payload == E_METER_MESSAGE_RESPOND_TAG)
    {
        handler_status = MessageHandler_Respond(msg_id, message_payload->metadata.correlation_id, &message_payload->respond);
    }
    else if (message_payload->which_message_payload == E_METER_MESSAGE_REQUEST_TAG)
    {
        /* This is a request not a reply */
        e_meter_message_crc_t *msg_respond = MessagePool_Acquire();

        if (msg_respond == NULL)
        {
            return MessageHandler_Fail;
        }

        set_correlation_id(msg_id, &msg_respond->message);
        msg_respond->message.which_message_payload = E_METER_MESSAGE_RESPOND_TAG;
        handler_status = MessageHandler_Request(msg_id, &message_payload->request, &msg_respond->message.respond);

        if (handler_status != MessageHandler_Success)
        {
            MessageBuilder_ErrorMessage(&msg_respond->message, handler_status);
        }

        /* Construct the frame */
        MessageBuilder_PrepareFrame(msg_respond);
        /* Start sending it */
        MessageTransport_Send(msg_respond);
        /* Free the msg */
        MessagePool_Release(msg_respond);
    }

    return handler_status;
}

MessageStatus MessageHandler_HandlerCallback(const uint16_t msg_id)
{
    e_meter_message_crc_t *msg_respond = (e_meter_message_crc_t *)MessageHandler_GetMessageFromMsgId(msg_id);

    if (msg_respond)
    {
        /* Construct the frame */
        MessageBuilder_PrepareFrame(msg_respond);
        /* Start sending it */
        MessageTransport_Send(msg_respond);
        /* Free the msg */
        MessagePool_Release(msg_respond);
    }

    return MessageHandler_Success;
}

MessageStatus MessageHandler_Init(const MessageHandlerCallbacks *message_callback_handler, device_type_t device_type)
{
    /* avoid not used warning */
    (void)&MessageHandler_RemoveRequest;

    for (uint8_t i = 0; i < MAX_PENDING_REQUEST; i++)
    {
        request_table[i].msg_id = INVALID_MESSAGE_ID;
        request_table[i].in_use = false;
    }

    if (message_callback_handler != NULL)
    {
        s_message_handler_callback.on_request = message_callback_handler->on_request;
        s_message_handler_callback.on_respond = message_callback_handler->on_respond;
    }

    if (device_type != DEVICE_TYPE_DEVICE_NOT_SET)
    {
        s_device_type = device_type;
    }

    return MessageHandler_Success;
}

MessageStatus MessageHandler_Message(const e_meter_message_crc_t *message)
{
    MessageStatus message_status = MessageHandler_Success;

    (void)&MessageHandler_RegisterRequest;
    if (message == NULL)
    {
        message_status = MessageHandler_InvalidParameter;
    }
    else
    {
        const e_meter_message_t *emeter_message    = &message->message;
        const message_metadata_t *message_metadata = &emeter_message->metadata;

        if ((message_metadata->has_status) && (message_metadata->status == STATUS_CODE_ERROR))
        {
            /* this is a reply for a request and it failed */
        }
        else
        {
            message_status = MessageHandler_MessagePayload(message->message.msg_id, emeter_message);
        }
    }

    return message_status;
}

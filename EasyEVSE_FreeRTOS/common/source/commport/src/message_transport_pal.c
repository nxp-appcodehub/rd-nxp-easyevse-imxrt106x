/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "message_descriptor.pb.h"

#include "message_core.h"
#include "message_raw.h"
#include "message_transport_pal.h"
#include "message_handler.h"
#include "message_pool.h"

#define NUM_BUFFERS_SEND 2
#define NUM_BUFFERS_RCV  2

typedef struct
{
    bool in_use;
    serial_buffer_t buffer_info;
} buffer_slot;

#define MAX_BUFFER_SIZE (MESSAGE_DESCRIPTOR_PB_H_MAX_SIZE + ENCODING_MAX_OVERHEAD)

static uint8_t send_buffer[NUM_BUFFERS_SEND][MAX_BUFFER_SIZE];
static uint8_t rcv_buffer[NUM_BUFFERS_RCV][MAX_BUFFER_SIZE];
static buffer_slot send_buffer_slot[NUM_BUFFERS_SEND];
static buffer_slot rcv_buffer_slot[NUM_BUFFERS_RCV];

static serial_handler_message_handler_t s_serial_handler;

serial_buffer_t *MessageTransport_AcquireRcvBuffer(void)
{
    MESSAGE_LOCK();
    serial_buffer_t *buffer = NULL;
    for (int i = 0; i < NUM_BUFFERS_RCV; i++)
    {
        if (rcv_buffer_slot[i].in_use == false)
        {
            rcv_buffer_slot[i].in_use = true;
            buffer                    = &rcv_buffer_slot[i].buffer_info;
            break;
        }
    }
    MESSAGE_UNLOCK();
    return buffer;
}

static void MessageTransport_FreeRcvBuffer(const uint8_t *buffer)
{
    MESSAGE_LOCK();
    for (int i = 0; i < NUM_BUFFERS_RCV; i++)
    {
        if (rcv_buffer_slot[i].buffer_info.buffer == buffer)
        {
            rcv_buffer_slot[i].in_use           = false;
            rcv_buffer_slot[i].buffer_info.size = MAX_BUFFER_SIZE;
        }
    }
    MESSAGE_UNLOCK();
}

serial_buffer_t *MessageTransport_AcquireSendBufferSlot(void)
{
    MESSAGE_LOCK();
    serial_buffer_t *buffer = NULL;
    for (int i = 0; i < NUM_BUFFERS_SEND; i++)
    {
        if (send_buffer_slot[i].in_use == false)
        {
            send_buffer_slot[i].in_use = true;
            buffer                     = &send_buffer_slot[i].buffer_info;
            break;
        }
    }
    MESSAGE_UNLOCK();
    return buffer;
}

static void MessageTransport_FreeSendBufferSlot(const uint8_t *buffer)
{
    MESSAGE_LOCK();
    for (int i = 0; i < NUM_BUFFERS_SEND; i++)
    {
        if (send_buffer_slot[i].buffer_info.buffer == buffer)
        {
            send_buffer_slot[i].in_use           = false;
            send_buffer_slot[i].buffer_info.size = MAX_BUFFER_SIZE;
        }
    }
    MESSAGE_UNLOCK();
}

message_transport_status_t MessageTransport_RcvProcess(const e_meter_message_crc_t *pMsg)
{
    message_transport_status_t message_status = MessageTransport_Ok;
    if (pMsg != NULL)
    {
        MessageHandler_Message(pMsg);
        MessagePool_Release(pMsg);
    }
    return message_status;
}

message_transport_status_t MessageTransport_RcvCallback(serial_buffer_t *rx_buffer, e_meter_message_crc_t **pMsg)
{
    message_transport_status_t message_status = MessageTransport_Ok;

    if (rx_buffer == NULL || rx_buffer->buffer == NULL || rx_buffer->size == 0)
    {
        return MessageTransport_Fail;
    }
    /* Remove the trailing 0, assume Cobs framing*/
    if (rx_buffer->buffer[rx_buffer->size - 1] == 0)
    {
        rx_buffer->size--;
    }

    if (pMsg != NULL)
    {
        *pMsg = MessagePool_Acquire();
        if (*pMsg == NULL)
        {
            message_status = MessageTransport_Fail;
        }
        else if (MessageRaw_Decoder(rx_buffer->buffer, rx_buffer->size, *pMsg) != kStatus_Success)
        {
            MessagePool_Release(*pMsg);
            *pMsg          = NULL;
            message_status = MessageTransport_Fail;
        }
    }
    else
    {
        message_status = MessageTransport_Fail;
    }

    MessageTransport_FreeRcvBuffer(rx_buffer->buffer);

    return message_status;
}

message_transport_status_t MessageTransport_SendCallback(const uint8_t *buffer)
{
    if (buffer == NULL)
    {
        return MessageTransport_Fail;
    }

    MessageTransport_FreeSendBufferSlot(buffer);

    return MessageTransport_Ok;
}

status_t MessageTransport_Send(e_meter_message_crc_t *pMsg)
{
    status_t status = kStatus_Success;

    if ((pMsg == NULL) || (s_serial_handler.send == NULL))
    {
        return kStatus_InvalidArgument;
    }

    serial_buffer_t *send_buffer = MessageTransport_AcquireSendBufferSlot();

    if (send_buffer != NULL)
    {
        status = MessageRaw_Encode(pMsg, send_buffer->buffer, &send_buffer->size);

        if (status == kStatus_Success)
        {
            message_transport_status_t status_send = s_serial_handler.send(send_buffer);

            if ((status_send == MessageTransport_Ok) || (status_send == MessageTransport_Fail))
            {
                MessageTransport_FreeSendBufferSlot(send_buffer->buffer);
            }
            else if (status_send == MessageTransport_NonBlocking)
            {
                /* Do nothing we will rcv an async callback */
            }
        }
    }
    else
    {
        status = kStatus_Fail;
    }

    return status;
}

status_t MessageTransport_Read(uint8_t *inputBuffer, size_t *input_length)
{
    status_t status = kStatus_Success;

    if ((inputBuffer == NULL) || (input_length == NULL) || (s_serial_handler.rcv == NULL))
    {
        return kStatus_InvalidArgument;
    }

    /* No implementation for now */
    s_serial_handler.rcv(inputBuffer, input_length);

    return status;
}

status_t MessageTransport_Init(const serial_handler_message_handler_t *serial_handler)
{
    if (serial_handler == NULL)
    {
        return kStatus_InvalidArgument;
    }

    for (uint8_t i = 0; i < NUM_BUFFERS_RCV; i++)
    {
        rcv_buffer_slot[i].in_use             = false;
        rcv_buffer_slot[i].buffer_info.buffer = rcv_buffer[i];
        rcv_buffer_slot[i].buffer_info.size   = MAX_BUFFER_SIZE;
    }

    for (uint8_t i = 0; i < NUM_BUFFERS_SEND; i++)
    {
        send_buffer_slot[i].in_use             = false;
        send_buffer_slot[i].buffer_info.buffer = send_buffer[i];
        send_buffer_slot[i].buffer_info.size   = MAX_BUFFER_SIZE;
    }

    s_serial_handler.send = serial_handler->send;
    s_serial_handler.rcv  = serial_handler->rcv;

    return kStatus_Success;
}

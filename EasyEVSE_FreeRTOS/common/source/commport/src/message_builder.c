/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_common.h"

#include "message_descriptor.pb.h"
#include "message_builder.h"
#include "message_handler.h"
#include "message_core.h"
#include "hal_crc.h"

/**
 * @note The function is marked as weak (`__attribute__ ((weak))`), allowing it to be
 *       overridden by another implementation platform linked
 */
__attribute__((weak)) status_t HAL_CRC32_GET(const uint8_t *data, uint32_t data_len, uint32_t *crc32)
{
    uint32_t i, j, crc, mask;

    if ((data == NULL) || (crc32 == NULL))
    {
        return kStatus_InvalidArgument;
    }

    crc = 0xFFFFFFFF;
    for (i = 0; i < data_len; i++)
    {
        crc ^= data[i];
        for (j = 0; j < 8; j++)
        {
            mask = -(crc & 1);
            crc  = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }

    *crc32 = ~crc;
    return kStatus_Success;
}

static uint16_t builder_generate_unique_message_id(void)
{
    static uint16_t next_msg_id = INVALID_MESSAGE_ID;
    MESSAGE_LOCK();
    next_msg_id = (next_msg_id + 1) % (UINT16_MAX + 1);

    if (next_msg_id == INVALID_MESSAGE_ID)
    {
        next_msg_id++;
    }

    MESSAGE_UNLOCK();
    return next_msg_id;
}

// status_t MessageBuilder_GenerateCRC(e_meter_message_t *pMessage, uint32_t *crc)
//{
//	status_t status = kStatus_Success;
//	uint32_t input_crc = 0;
//	if (pMessage == NULL || crc == NULL)
//	{
//		return kStatus_InvalidArgument;
//	}
//
//	HAL_CRC32_GET((uint8_t *)pMessage, E_METER_MESSAGE_SIZE, crc);
//     return status;
// }

void MessageBuilder_ErrorMessage(e_meter_message_t *pMessage, MessageStatus reason)
{
    if (pMessage == NULL)
    {
        return;
    }

    pMessage->metadata.has_status = true;

    if (reason == MessageHandler_Fail)
    {
        pMessage->metadata.status = STATUS_CODE_ERROR;
    }
    else if (reason == MessageHandler_InvalidParameter)
    {
        pMessage->metadata.status = STATUS_CODE_INVALID_DATA;
    }
    else if (reason == MessageHandler_NotImplemented)
    {
        pMessage->metadata.status = STATUS_CODE_NOT_IMPLEMENTED;
    }
}

void MessageBuilder_PrepareFrame(e_meter_message_crc_t *pMessage_crc)
{
    if (pMessage_crc == NULL)
    {
        return;
    }

    pMessage_crc->message.msg_id = builder_generate_unique_message_id();
}

void MessageBuilder_PrepareMessage(e_meter_message_crc_t *pMessage_crc,
                                   message_payload_type_t payload_type,
                                   device_type_t device_type)
{
    if (pMessage_crc == NULL)
    {
        return;
    }

    if (device_type != DEVICE_TYPE_DEVICE_NOT_SET)
    {
        pMessage_crc->message.metadata.has_device_type = true;
        pMessage_crc->message.metadata.device_type     = device_type;
    }

    switch (payload_type)
    {
        case PAYLOAD_HOST_REQUEST:
        case PAYLOAD_EMETER_REQUEST:
            // Handle host_request
            pMessage_crc->message.which_message_payload = E_METER_MESSAGE_REQUEST_TAG;
            break;

        case PAYLOAD_EMETER_RESPOND:
        case PAYLOAD_HOST_RESPOND:
            // Handle emeter_respond
            pMessage_crc->message.which_message_payload = E_METER_MESSAGE_RESPOND_TAG;
            break;

        default:
            // Handle unknown or invalid payload type
            break;
    }

    MessageBuilder_PrepareFrame(pMessage_crc);
}

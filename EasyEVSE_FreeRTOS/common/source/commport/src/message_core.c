/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "message_descriptor.pb.h"
#include "message_handler.h"
#include "hal_crc.h"

#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

__attribute__((weak)) status_t MESSAGE_LOCK()
{
    return kStatus_Success;
}

__attribute__((weak)) status_t MESSAGE_UNLOCK()
{
    return kStatus_Success;
}

__attribute__((weak)) status_t MESSAGE_LOCKINIT()
{
    return kStatus_Success;
}

static status_t MessageCore_Init(e_meter_message_crc_t *pMessage)
{
    status_t status = kStatus_Success;
    if (pMessage == NULL)
    {
        return kStatus_InvalidArgument;
    }

    *pMessage = (e_meter_message_crc_t)E_METER_MESSAGE_CRC_INIT_DEFAULT;

    // MESSAGE_LOCKINIT();
    return status;
}

status_t MessageCore_Deserialize(const uint8_t *input_buffer,
                                 const uint16_t input_size,
                                 e_meter_message_crc_t *decoded_message)
{
    status_t status = kStatus_Success;

    if ((decoded_message == NULL) || (input_buffer == NULL))
    {
        return kStatus_InvalidArgument;
    }

    status = MessageCore_Init(decoded_message);

    if (status == kStatus_Success)
    {
        bool success = true, eof = false;
        uint32_t wiretype = 0, tag = 0, crc = 0;

        pb_istream_t istream = pb_istream_from_buffer(input_buffer, input_size);
        pb_decode_tag(&istream, (pb_wire_type_t *)&wiretype, &tag, &eof);

        if ((tag == E_METER_MESSAGE_CRC_MESSAGE_TAG) && (wiretype == PB_WT_STRING))
        {
            pb_istream_t substream = {0};
            if (pb_make_string_substream(&istream, &substream))
            {
                success &= pb_decode(&substream, E_METER_MESSAGE_FIELDS, &decoded_message->message);
                success &= pb_close_string_substream(&istream, &substream);
            }

            if (success == true)
            {
                HAL_CRC32_GET(input_buffer, input_size - istream.bytes_left, &crc);
            }
        }
        else
        {
            success = false;
        }

        if (success == true)
        {
            pb_decode_tag(&istream, (pb_wire_type_t *)&wiretype, &tag, &eof);
            if (tag == E_METER_MESSAGE_CRC_CRC_TAG)
            {
                success &= pb_decode_varint32(&istream, &decoded_message->crc);
                if ((success == true) && (decoded_message->crc != crc))
                {
                    success = false;
                }
            }
            else
            {
                success = false;
            }
        }

        if (success != true)
        {
            status = kStatus_Fail;
        }
    }

    return status;
}

status_t MessageCore_Serialize(e_meter_message_crc_t *pMessage, uint8_t *output_buffer, uint16_t *output_buffer_len)
{
    status_t status = kStatus_Success;

    if ((pMessage == NULL) || (output_buffer == NULL) || (output_buffer_len == NULL))
    {
        return kStatus_InvalidArgument;
    }

    if (status == kStatus_Success)
    {
        pb_ostream_t stream = pb_ostream_from_buffer(output_buffer, *output_buffer_len);
        bool success        = true;

        success &= pb_encode_tag(&stream, PB_WT_STRING, E_METER_MESSAGE_CRC_MESSAGE_TAG);
        success &= pb_encode_submessage(&stream, E_METER_MESSAGE_FIELDS, &pMessage->message);

        HAL_CRC32_GET(output_buffer, stream.bytes_written, &pMessage->crc);

        success &= pb_encode_tag(&stream, PB_WT_VARINT, E_METER_MESSAGE_CRC_CRC_TAG);
        success &= pb_encode_varint(&stream, pMessage->crc);

        if (success == true)
        {
            *output_buffer_len = stream.bytes_written;
        }
        else
        {
            status = kStatus_Fail;
        }
    }
    else
    {
    }

    return status;
}

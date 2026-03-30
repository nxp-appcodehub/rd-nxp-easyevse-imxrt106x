/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "message_raw.h"
#include "message_core.h"
#include "message_builder.h"
#include "cobs.h"

status_t MessageRaw_Encode(e_meter_message_crc_t *inputMessage, uint8_t *output_buffer, size_t *output_buffer_length)
{
    status_t status = kStatus_Success;

    if ((inputMessage == NULL) || (output_buffer == NULL) || (output_buffer_length == NULL))
    {
        return kStatus_InvalidArgument;
    }

    /* Make a copy of the output_max size so we can use in the inplace encoding */

    size_t copy_output_buffer_length = *output_buffer_length;

    status = MessageCore_Serialize(inputMessage, output_buffer, (uint16_t *)&copy_output_buffer_length);

    if (status == kStatus_Success)
    {
        // In-place COBS encoding
        status = cobs_encode_inplace(output_buffer, copy_output_buffer_length, output_buffer_length);
    }

    return status;
}

status_t MessageRaw_Decoder(uint8_t *inputBuffer, size_t input_length, e_meter_message_crc_t *output_message)
{
    status_t status = kStatus_Success;

    if (inputBuffer == NULL || output_message == NULL)
    {
        return kStatus_InvalidArgument;
    }

    size_t decode_message_length = input_length;

    status = cobs_decode_inplace(inputBuffer, input_length, &decode_message_length);

    if (status == kStatus_Success)
    {
        status = MessageCore_Deserialize(inputBuffer, decode_message_length, output_message);
    }

    return status;
}

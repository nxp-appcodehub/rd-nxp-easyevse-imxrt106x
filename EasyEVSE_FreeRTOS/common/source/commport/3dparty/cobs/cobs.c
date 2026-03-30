/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "cobs.h"
#include <string.h>

#define COBS_MAX_ELEMENTS 0xFF

size_t cobs_get_size(const size_t input_length)
{
    return input_length + COBS_TOTAL_OVERHEAD;
}

status_t cobs_encode_inplace(uint8_t *buffer, const size_t input_length, size_t *output_length)
{
    if (buffer == NULL || output_length == NULL || input_length == 0)
    {
        return kStatus_InvalidArgument; // Invalid input or output buffer length
    }

    if (input_length > *output_length - COBS_TOTAL_OVERHEAD)
    {
        return kStatus_OutOfRange;
    }

    status_t decode_status = kStatus_Success;
    size_t read_index      = 0;
    size_t code_index      = 0;
    uint8_t code           = 1;
    uint8_t read_copy;

    read_copy = buffer[code_index];

    while (read_index < input_length)
    {
        if ((code_index >= *output_length) || (read_index + 1 >= *output_length))
        {
            decode_status = kStatus_Fail; // buffer overflow
            break;
        }

        if ((read_copy == 0) || (code == COBS_MAX_ELEMENTS))
        {
            buffer[code_index] = code;
            code_index         = read_index + 1;
            code               = 1;
            read_copy          = buffer[read_index + 1];
        }
        else
        {
            uint8_t read_copy_tmp  = buffer[read_index + 1];
            buffer[read_index + 1] = read_copy;
            read_copy              = read_copy_tmp;
            code++;
        }

        read_index++;
    }

    buffer[code_index] = code;
    read_index++;

    /* TODO remove the framing from here */
    if (decode_status == kStatus_Success)
    {
        if ((read_index < *output_length))
        {
            buffer[read_index++] = 0;
        }
        else
        {
            decode_status = kStatus_Fail;
        }
    }

    *output_length = read_index;

    return decode_status;
}

/**
 * @brief Encode data using COBS.
 */
status_t cobs_encode(const uint8_t *input, const size_t input_length, uint8_t *output, size_t *output_length)
{
    status_t decode_status = kStatus_Success;
    size_t read_index      = 0;
    size_t write_index     = 1;
    size_t code_index      = 0;
    uint8_t code           = 1;

    if (input == NULL || output == NULL || output_length == NULL || input_length == 0 || *output_length == 0)
    {
        return kStatus_InvalidArgument; // Invalid input or output buffer length
    }

    while (read_index < input_length)
    {
        if ((code_index > *output_length) || (write_index > *output_length))
        {
            decode_status = kStatus_Fail; // buffer overflow
            break;
        }

        if ((input[read_index] == 0) || (code == COBS_MAX_ELEMENTS))
        {
            output[code_index] = code;
            code_index         = write_index;
            code               = 1;
        }
        else
        {
            output[write_index] = input[read_index];
            code++;
        }

        write_index++;
        read_index++;
    }

    /* TODO remove the framing from here */
    if (decode_status == kStatus_Success)
    {
        if ((write_index < *output_length))
        {
            output[code_index]    = code;
            output[write_index++] = 0;
        }
        else
        {
            decode_status = kStatus_Fail;
        }
    }
    *output_length = write_index;

    return decode_status;
}

status_t cobs_decode_inplace(uint8_t *buffer, const size_t input_length, size_t *output_length)
{
    return cobs_decode(buffer, input_length, buffer, output_length);
}

status_t cobs_decode(const uint8_t *input, const size_t input_length, uint8_t *output, size_t *output_length)
{
    status_t decode_status = kStatus_Success;
    size_t read_index      = 0;
    size_t write_index     = 0;
    uint8_t code;

    if (input == NULL || output == NULL || output_length == NULL || input_length < 3 || *output_length == 0)
    {
        return kStatus_InvalidArgument; // Invalid input or output buffer length
    }

    if (input[input_length - 1] == 0)
    {
        return kStatus_Fail;
    }

    while (read_index < input_length)
    {
        code = input[read_index++];

        if ((code == 0) || ((code == 1) && (read_index == input_length - 1)))
        {
            break;
        }

        if (((read_index + code - 1) > input_length) || ((write_index + code - 1) >= *output_length))
        {
            decode_status = kStatus_Fail; // Invalid input: code byte is zero or exceeds input length
            break;
        }

        for (uint8_t i = 1; i < code; i++)
        {
            if (input[read_index] == 0)
                return kStatus_Fail;
            output[write_index++] = input[read_index++];
        }

        if ((code != COBS_MAX_ELEMENTS) && (read_index != input_length))
        {
            output[write_index++] = 0;
        }
    }

    *output_length = write_index;

    return decode_status;
}

/**
 * @brief Initialize the COBS decoder state.
 */
status_t cobs_decoder_init(cobs_decoder_state_t *state)
{
    status_t decode_status = kStatus_Success;

    if (state == NULL)
    {
        return kStatus_InvalidArgument;
    }

    state->code         = 0;
    state->code_index   = 0;
    state->state        = 0;
    state->valid_output = false;

    return decode_status;
}

/**
 * @brief Decode data using COBS byte-by-byte in real-time.
 */
status_t cobs_decode_byte_stream(uint8_t input_byte, uint8_t *output_byte, cobs_decoder_state_t *state)
{
    status_t decode_status = kStatus_Success;

    if ((state == NULL) || (output_byte == NULL))
    {
        return kStatus_InvalidArgument;
    }

    state->valid_output = false;

    if (state->state == 1)
    {
        state->valid_output = true;

        if (state->code_index < (state->code - 1))
        {
            /* copy the number */
            *output_byte = input_byte;
            state->code_index++;
        }
        else
        {
            *output_byte = 0;
            state->state = 0;
        }
    }

    if (state->state == 0)
    {
        if (input_byte != 0x00)
        {
            state->code       = input_byte;
            state->code_index = 0;
            state->state      = 1;
        }
        else
        {
            /* Most likely the end */
            state->valid_output = false;
        }
    }

    return decode_status;
}

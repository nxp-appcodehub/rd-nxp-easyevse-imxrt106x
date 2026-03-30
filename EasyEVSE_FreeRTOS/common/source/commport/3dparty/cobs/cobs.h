/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file cobs.h
 * @brief COBS (Consistent Overhead Byte Stuffing) encoding and decoding API.
 */

#ifndef COBS_H
#define COBS_H

#include <stddef.h>
#include <stdint.h>

#include "fsl_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define COBS_FRAMING        1
#define COBS_OVERHEAD       1
#define COBS_TOTAL_OVERHEAD (COBS_FRAMING + COBS_OVERHEAD)

/**
 * @brief Real-time COBS byte-by-byte decoder state.
 *
 * This structure maintains the state required for decoding COBS-encoded data
 * byte-by-byte in real-time. It is used to keep track of the current code byte,
 * the index within the current code sequence, and the overall state of the decoder.
 */
typedef struct
{
    uint8_t code;       /**< The current code byte indicating the number of non-zero bytes. */
    uint8_t code_index; /**< The index within the current code sequence. */
    uint8_t state;      /**< The state of the decoder (0 for initial state, 1 for processing). */
    bool valid_output;
} cobs_decoder_state_t;

size_t cobs_get_size(const size_t input_length);

/**
 * @brief Encode data using Consistent Overhead Byte Stuffing (COBS) in-place.
 *
 * This function performs in-place COBS encoding on the provided buffer.
 * The input data must be placed at the beginning of the buffer, and the buffer
 * must be large enough to accommodate the encoded output, which may be up to
 * input_length + 1 bytes due to COBS overhead and 1 byte for framing.
 *
 * @param buffer Pointer to the buffer containing input data and where the encoded output will be written.
 *               The buffer must be at least (input_length + 2) bytes in size.
 * @param input_length Length of the input data in bytes.
 * @param output_length Pointer to a variable where the function will store the number of bytes written.
 * @return status_t Status code indicating success or failure of the encoding operation.
 */
status_t cobs_encode_inplace(uint8_t *buffer, const size_t input_length, size_t *output_length);

/**
 * @brief Encode data using COBS.
 *
 * @param input Pointer to the input data buffer.
 * @param input_length Length of the input data.
 * @param output Pointer to the output data buffer.
 * @param output_length Length of the output data buffer.
 * @return size_t Number of bytes written to the output buffer.
 */
status_t cobs_encode(const uint8_t *input, size_t input_length, uint8_t *output, size_t *output_length);

/**
 * @brief Decodes a COBS (Consistent Overhead Byte Stuffing) encoded buffer in place.
 *
 * This function decodes the provided COBS-encoded buffer and writes the decoded data
 * back to the same buffer location. The output length of the decoded data is returned
 * via the output_length pointer.
 *
 * @param[in,out] buffer         Pointer to the buffer containing COBS-encoded data. The decoded data will be written
 * back to this buffer.
 * @param[in]     input_length   Length of the input buffer containing the encoded data.
 * @param[out]    output_length  Pointer to a variable where the length of the decoded output will be stored.
 *
 * @return status_t Returns a status code indicating success or failure of the decoding operation.
 */
status_t cobs_decode_inplace(uint8_t *buffer, const size_t input_length, size_t *output_length);

/**
 * @brief Decode data using COBS.
 *
 * @param input Pointer to the input data buffer.
 * @param input_length Length of the input data.
 * @param output Pointer to the output data buffer.
 * @param output_length Length of the output data buffer.
 * @return size_t Number of bytes written to the output buffer.
 */
status_t cobs_decode(const uint8_t *input, size_t input_length, uint8_t *output, size_t *output_length);

/**
 * @brief Decode data using COBS byte-by-byte in real-time.
 *
 * @param input_byte The current byte from the input data.
 * @param output_byte Pointer to the current byte in the output data.
 * @param state Pointer to the decoder state.
 * @return int Status code (0 for success, -1 for error).
 */
status_t cobs_decode_byte_stream(uint8_t input_byte, uint8_t *output_byte, cobs_decoder_state_t *state);

/**
 * @brief Initialize the COBS decoder state.
 *
 * @param state Pointer to the decoder state.
 */
status_t cobs_decoder_init(cobs_decoder_state_t *state);

#ifdef __cplusplus
}
#endif

#endif // COBS_H

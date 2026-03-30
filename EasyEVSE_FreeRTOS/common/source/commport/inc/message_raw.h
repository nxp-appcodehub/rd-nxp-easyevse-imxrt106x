/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MESSAGE_RAW_H
#define MESSAGE_RAW_H

#include "fsl_common.h"
#include "message_descriptor.pb.h"

/* Encoding info */
#define ENABLE_COBS 1
#if ENABLE_COBS
#include "cobs.h"
#define ENCODING_MAX_OVERHEAD COBS_TOTAL_OVERHEAD
#else
#define ENCODING_MAX_OVERHEAD 0
#endif /* ENABLE_COBS */

/**
 * @brief Encode a raw message using COBS encoding.
 *
 * This function first encodes the input message using MessageCore_Encode and then performs COBS encoding.
 * It supports in-place encoding.
 *
 * @param inputMessage Pointer to the input message structure.
 * @param input_length Length of the input message.
 * @param output_buffer Pointer to the output buffer where the encoded message will be stored.
 * @param output_buffer_length Pointer to the length of the output buffer.
 * @return status_t Status of the encoding operation.
 */
status_t MessageRaw_Encode(e_meter_message_crc_t *inputMessage, uint8_t *output_buffer, size_t *output_buffer_length);

/**
 * @brief Decode a raw message using COBS decoding.
 *
 * This function first performs COBS decoding and then decodes the message using MessageCore_Decode.
 * It supports in-place decoding.
 *
 * @param inputBuffer Pointer to the input buffer containing the encoded message.
 * @param input_length Length of the input buffer.
 * @param output_message Pointer to the output message structure where the decoded message will be stored.
 * @return status_t Status of the decoding operation.
 */
status_t MessageRaw_Decoder(uint8_t *inputBuffer, size_t input_length, e_meter_message_crc_t *output_message);

#endif // MESSAGE_RAW_H

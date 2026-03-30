/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

/**
 * @file message_core.h
 * @brief Header file for message core encoding and decoding functions.
 *
 * This file contains the declarations of the functions for initializing, encoding, decoding,
 * and verifying CRC for EMeterMessageCRC messages.
 */

#ifndef MESSAGE_CORE_H
#define MESSAGE_CORE_H

#include "message_descriptor.pb.h"
#include "fsl_common.h"

status_t MESSAGE_LOCK();

status_t MESSAGE_UNLOCK();

status_t MESSAGE_LOCKINIT();

/**
 * @brief Initializes an EMeterMessageCRC message.
 *
 * This function initializes the given EMeterMessageCRC message to zero.
 *
 * @param pMessage Pointer to the message to be initialized.
 * @return kStatus_Success if the message was successfully initialized, kStatus_InvalidArgument if the input is NULL.
 */
status_t MessageCore_Init(void *pMessage);

/**
 * @brief Decodes an EMeterMessageCRC message from a buffer.
 *
 * This function decodes an EMeterMessageCRC message from the given input buffer.
 *
 * @param input_buffer Pointer to the buffer containing the encoded message.
 * @param input_size Size of the input buffer.
 * @param decoded_message Pointer to the message that will store the decoded message.
 * @return kStatus_Success if the message was successfully decoded, kStatus_InvalidArgument if the input is NULL,
 * kStatus_Fail if decoding failed.
 */
status_t MessageCore_Deserialize(const uint8_t *input_buffer,
                                 const uint16_t input_size,
                                 e_meter_message_crc_t *decoded_message);

/**
 * @brief Encodes an EMeterMessageCRC message to a buffer.
 *
 * This function encodes the given EMeterMessageCRC message to the provided output buffer.
 *
 * @param pMessage Pointer to the message to be encoded.
 * @param output_buffer Pointer to the buffer that will store the encoded message.
 * @param output_buffer_len Pointer to a variable that will store the length of the encoded message.
 * @return kStatus_Success if the message was successfully encoded, kStatus_InvalidArgument if the input is NULL,
 * kStatus_Fail if encoding failed.
 */
status_t MessageCore_Serialize(e_meter_message_crc_t *pMessage, uint8_t *output_buffer, uint16_t *output_buffer_len);

#endif // MESSAGE_CORE_H

/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MESSAGE_BUILDER_H
#define MESSAGE_BUILDER_H

#include <stdint.h>
#include "message_descriptor.pb.h"
#include "message_handler.h"
#include "hal_crc.h"

// Function declarations
#ifdef __cplusplus
extern "C" {
#endif

#define INVALID_MESSAGE_ID 0U

typedef enum
{
    PAYLOAD_HOST_REQUEST = 1,
    PAYLOAD_EMETER_RESPOND,
    PAYLOAD_EMETER_REQUEST,
    PAYLOAD_HOST_RESPOND
} message_payload_type_t;

/**
 * @brief Generates CRC32 checksum for a message.
 *
 * This function computes the CRC for the provided e_meter_message and stores the result
 * in the crc_ccitt variable.
 *
 * @param[in]  pMessage    Pointer to the input message structure.
 * @param[out] crc_ccitt   Pointer to the output variable to store CRC result.
 *
 * @return status_t        kStatus_Success on success, kStatus_InvalidArgument if input is NULL.
 */
status_t MessageBuilder_GenerateCRC(e_meter_message_t *pMessage, uint32_t *crc_ccitt);

/**
 * @brief Populates the message with an error status code.
 *
 * Sets the `has_status` flag and updates the `status` field to indicate an error.
 *
 * @param[in,out] pMessage  Pointer to the message structure to be updated.
 */
void MessageBuilder_ErrorMessage(e_meter_message_t *pMessage, MessageStatus reason);

/**
 * @brief Prepares a message frame with a unique ID and CRC checksum.
 *
 * Generates a unique message ID and calculates the CRC for the message payload.
 *
 * @param[in,out] pMessage_crc  Pointer to the message-with-CRC structure.
 */
void MessageBuilder_PrepareFrame(e_meter_message_crc_t *pMessage_crc);

/**
 * @brief Initializes and prepares the message based on its payload type.
 *
 * Sets the payload tag based on the provided type, initializes metadata and appends a CRC.
 *
 * @param[in,out] pMessage_crc   Pointer to the message-with-CRC structure.
 * @param[in]     payload_type   Enum indicating the type of payload.
 */
void MessageBuilder_PrepareMessage(e_meter_message_crc_t *pMessage_crc,
                                   message_payload_type_t payload_type,
                                   device_type_t device_type);
#ifdef __cplusplus
}
#endif

#endif // MESSAGE_BUILDER_H

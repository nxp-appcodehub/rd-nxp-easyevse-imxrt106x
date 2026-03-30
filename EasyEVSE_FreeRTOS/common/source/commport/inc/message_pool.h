/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MESSAGE_POOL_H
#define MESSAGE_POOL_H

#include <stdint.h>
#include <stdbool.h>

// Adjust this as needed based on your actual message type
#include "message_descriptor.pb.h" // Defines e_meter_message_crc_t

#define MESSAGE_POOL_SIZE 3        // Configurable pool size

#ifdef __cplusplus
extern "C" {
#endif

//  Status codes for pool operations
typedef enum
{
    MessagePool_Ok = 0,
    MessagePool_InvalidParameter,
    MessagePool_NotFound,
    MessagePool_Exhausted
} MessagePoolStatus;

/**
 * @brief Acquires a free message slot from the pool.
 *
 * Searches the message pool for an unused message object and marks it as in use. Will init the message with default
 * values
 *
 * @return Pointer to a free e_meter_message_crc_t if available, or NULL if the pool is exhausted.
 */
e_meter_message_crc_t *MessagePool_Acquire(void);

/**
 * @brief Releases a previously acquired message back to the pool.
 *
 * Frees the specified message, making it available for future use. If the message pointer
 * is not part of the pool, the function returns MessagePool_NotFound.
 *
 * @param msg Pointer to the message to release.
 * @return MessagePoolStatus indicating success or failure.
 *         - MessagePool_Ok: Released successfully.
 *         - MessagePool_InvalidParameter: Null pointer provided.
 *         - MessagePool_NotFound: Message not found in the pool.
 */
MessagePoolStatus MessagePool_Release(const e_meter_message_crc_t *pMessage_crc);
#ifdef __cplusplus
}
#endif

#endif // MESSAGE_POOL_H

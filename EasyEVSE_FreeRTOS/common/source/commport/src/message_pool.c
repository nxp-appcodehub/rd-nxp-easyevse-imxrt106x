/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "message_pool.h"
#include "message_core.h"
#include "stdint.h"

typedef struct
{
    e_meter_message_crc_t msg;
    bool in_use;
} message_slot;

static message_slot message_pool[MESSAGE_POOL_SIZE] = {0};
static bool is_init                                 = false;

static void MessagePool_Init(void)
{
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++)
    {
        message_pool[i].in_use = false;
        memset(&message_pool[i].msg, 0, sizeof(e_meter_message_crc_t));
    }
    is_init = true;
}

e_meter_message_crc_t *MessagePool_Acquire()
{
    e_meter_message_crc_t *msg = NULL;
    MESSAGE_LOCK();
    if (!is_init)
    {
        MessagePool_Init();
    }

    for (int i = 0; i < MESSAGE_POOL_SIZE; i++)
    {
        if (!message_pool[i].in_use)
        {
            message_pool[i].in_use = true;
            memset(&message_pool[i].msg, 0, sizeof(e_meter_message_crc_t));
            msg = &message_pool[i].msg;
            break;
        }
    }
    MESSAGE_UNLOCK();
    return msg; // pool exhausted if NULL
}

/*
 * Releases a message back to the pool in a thread-safe manner (uses internal locking).
 * If pMessage_crc is not found in the pool, MessagePool_NotFound is returned.
 * If pMessage_crc is NULL, MessagePool_InvalidParameter is returned.
 */
MessagePoolStatus MessagePool_Release(const e_meter_message_crc_t *pMessage_crc)
{
    MessagePoolStatus message_pool_status = MessagePool_NotFound;
    if (pMessage_crc == NULL)
    {
        return MessagePool_InvalidParameter;
    }

    MESSAGE_LOCK();
    for (int i = 0; i < MESSAGE_POOL_SIZE; i++)
    {
        if (&message_pool[i].msg == pMessage_crc)
        {
            message_pool[i].in_use = false;
            memset(&message_pool[i].msg, 0, sizeof(e_meter_message_crc_t));
            message_pool_status = MessagePool_Ok;
            break;
        }
    }
    MESSAGE_UNLOCK();
    // If the pointer was not found, MessagePool_NotFound is returned.
    return message_pool_status;
}

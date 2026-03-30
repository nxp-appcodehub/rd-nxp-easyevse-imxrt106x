/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef MESSAGE_TRANSPORT_PAL_H
#define MESSAGE_TRANSPORT_PAL_H

#include "message_descriptor.pb.h"

typedef struct serial_buffer
{
    uint8_t *buffer;
    size_t size;
} serial_buffer_t;

typedef enum message_transport_status
{
    MessageTransport_Ok,
    MessageTransport_NonBlocking,
    MessageTransport_Fail = -1
} message_transport_status_t;

typedef message_transport_status_t (*MessageTransportSend)(const serial_buffer_t *buffer);
typedef message_transport_status_t (*MessageTransportReceive)(uint8_t *buffer, size_t *length);

typedef struct serial_handler_message_handler
{
    /* Low level send */
    MessageTransportSend send;
    /* Low level rcv */
    MessageTransportReceive rcv;
} serial_handler_message_handler_t;

/**
 * @brief Get a buffer where to store a message. Store raw values obtained over UART/SPI
 *
 * @return serial_buffer_t* buffer
 */
serial_buffer_t *MessageTransport_AcquireRcvBuffer(void);

/**
 * @brief Call this to parse the message obtained. The rx_buffer is obtained MessageTransport_AcquireRcvBuffer
 *
 * @param rx_buffer buffer obtained from calling MessageTransport_AcquireRcvBuffer
 * @param pMsg Will contained a pointer to where the parsed message is placed
 * @return message_transport_status_t
 */
message_transport_status_t MessageTransport_RcvCallback(serial_buffer_t *rx_buffer, e_meter_message_crc_t **pMsg);

/**
 * @brief Call this when a message has been send. Used when the send works in NonBlocking way
 *
 * @param buffer Buffer that was send
 * @return message_transport_status_t
 */
message_transport_status_t MessageTransport_SendCallback(const uint8_t *buffer);

/**
 * @brief Start the process of a message.
 *
 * @param pMsg The message can either be compose manually or it is a return from the calling
 * MessageTransport_RcvCallback. the pMsg is obtained from calling MessagePool_Acquire();
 * @return message_transport_status_t
 */
message_transport_status_t MessageTransport_RcvProcess(const e_meter_message_crc_t *pMsg);

/**
 * @brief
 *
 * @param inputBuffer
 * @param input_length
 * @return status_t
 */
status_t MessageTransport_Read(uint8_t *inputBuffer, size_t *input_length);

/**
 * @brief Handles data serialization and data encoding. Call MessageBuilder_PrepareFrame before this
 *
 * @param message_crc
 * @return status_t
 */
status_t MessageTransport_Send(e_meter_message_crc_t *pMessage_crc);

/**
 * @brief
 *
 * @param serial_handler
 * @return status_t
 */
status_t MessageTransport_Init(const serial_handler_message_handler_t *serial_handler);

#endif /* MESSAGE_TRANSPORT_PAL_H */

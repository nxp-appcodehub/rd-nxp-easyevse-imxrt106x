/*
 * Copyright 2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef HAL_CRC
#define HAL_CRC

#include "fsl_common.h"

status_t HAL_CRC_Init();
status_t HAL_CRC_GetCRC16CCITT(const uint8_t *data, uint16_t length, uint32_t *crc);

/**
 * @brief Computes the CRC32 checksum for a given data buffer.
 *
 * This function calculates the CRC32 checksum using the standard polynomial
 * `0xEDB88320`. It processes each byte of the input data and returns the
 * final CRC value through the output pointer.
 *
 * @param[in]  data     Pointer to the input data buffer.
 * @param[in]  data_len Length of the input data in bytes.
 * @param[out] crc32    Pointer to a variable where the computed CRC32 value will be stored.
 *
 * @return status_t
 *         - kStatus_Success: CRC32 computation completed successfully.
 *         - kStatus_InvalidArgument: One or more input pointers are NULL.
 */

status_t HAL_CRC32_GET(const uint8_t *data, uint32_t data_len, uint32_t *crc32);
#endif /* BOARDSELECTION_H */

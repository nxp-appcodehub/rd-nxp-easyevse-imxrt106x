/*
 *
 * Copyright 2019-2020, 2023, 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SE05X_BINARY_FILE_H_
#define SE05X_BINARY_FILE_H_

#include <fsl_sss_api.h>

sss_status_t se05x_WriteKeyFile(uint32_t objectId, uint8_t *data, size_t data_length, sss_key_part_t key_type);
sss_status_t se05x_ReadObjectType(uint32_t objectId, uint32_t* objectType);
sss_status_t se05x_WriteBinaryFile(uint32_t objectId, uint8_t *data, size_t data_length);
sss_status_t se05x_ReadBinaryFile(uint32_t objectId, uint8_t *data, size_t *data_length);
sss_status_t se05x_DeleteBinaryFile(uint32_t objectId);

#endif /* SE05X_BINARY_FILE_H */

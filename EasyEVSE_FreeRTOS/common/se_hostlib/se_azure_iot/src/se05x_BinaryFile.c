/*
 *
 * Copyright 2019-2020, 2023, 2025 NXP
 * SPDX-License-Identifier: Apache-2.0
 */

#include "se05x_BinaryFile.h"

#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxLog_App.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include <nxEnsure.h>

#include "se05x_sss_boot_pcontext.h"

#define SE05x_LOG_BINARY_FILE       1

//only writes P-256 keys
sss_status_t se05x_WriteKeyFile(uint32_t objectId, uint8_t *data, size_t data_length, sss_key_part_t key_type)
{
    sss_status_t status      = kStatus_SSS_Fail;

    ex_sss_boot_ctx_t *pCtx = EX_SSS_BOOT_PCONTEXT;
    sss_object_t obj;

    if ((key_type != kSSS_KeyPart_Private) && (key_type != kSSS_KeyPart_Pair))
	{
        LOG_E("Unsupported key type. Provide a private key or a key pair.");
        goto exit;
	}

    status = sss_key_object_init(&obj, &pCtx->ks);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_init");
        goto exit;
    }

    status = sss_key_object_allocate_handle(
            &obj,
            objectId,
			key_type,
			kSSS_CipherType_EC_NIST_P,
            data_length * 8,
            kKeyObject_Mode_Persistent);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_allocate_handle");
        goto exit;
    }

    status = sss_key_store_set_key(
            &pCtx->ks,
            &obj,
            data,
            data_length,
            256,
            NULL,
            0);

#if (SE05x_LOG_BINARY_FILE == 1)
    LOG_MAU8_I("\r\nSE05X: Write Binary File: ", data, data_length);
#endif

exit:
    sss_key_object_free(&obj);
    return status;
}

sss_status_t se05x_WriteBinaryFile(uint32_t objectId, uint8_t *data, size_t data_length)
{
    sss_status_t status      = kStatus_SSS_Fail;
    size_t binary_dataLength = data_length * 8;

    ex_sss_boot_ctx_t *pCtx = EX_SSS_BOOT_PCONTEXT;
    sss_object_t obj;

    status = sss_key_object_init(&obj, &pCtx->ks);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_init");
        goto exit;
    }

    status = sss_key_object_allocate_handle(
            &obj,
            objectId,
            kSSS_KeyPart_NONE,
            kSSS_CipherType_Binary,
            binary_dataLength,
            kKeyObject_Mode_Persistent);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_allocate_handle");
        goto exit;
    }

    status = sss_key_store_set_key(
            &pCtx->ks,
            &obj,
            data,
            data_length,
            binary_dataLength,
            NULL,
            0);

#if (SE05x_LOG_BINARY_FILE == 1)
    LOG_MAU8_I("\r\nSE05X: Write Binary File: ", data, data_length);
#endif

exit:
    sss_key_object_free(&obj);
    return status;
}

sss_status_t se05x_ReadObjectType(uint32_t objectId, uint32_t* objectType)
{
    sss_status_t status      = kStatus_SSS_Fail;

    ex_sss_boot_ctx_t *pCtx = EX_SSS_BOOT_PCONTEXT;
    sss_object_t obj;

    if (objectType == NULL)
    {
        goto exit;
    }

    status = sss_key_object_init(&obj, &pCtx->ks);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_init");
        goto exit;
    }

    status = sss_key_object_get_handle(&obj, objectId);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_get_handle");
        goto exit;
    }

    *objectType = obj.objectType;

exit:
    sss_key_object_free(&obj);
    return status;
}

sss_status_t se05x_ReadBinaryFile(uint32_t objectId, uint8_t *data, size_t *data_length)
{
    sss_status_t status      = kStatus_SSS_Fail;
    size_t binary_dataLength = *data_length * 8;

    ex_sss_boot_ctx_t *pCtx = EX_SSS_BOOT_PCONTEXT;
    sss_object_t obj;

    status = sss_key_object_init(&obj, &pCtx->ks);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_init");
        goto exit;
    }

    status = sss_key_object_get_handle(&obj, objectId);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_get_handle");
        goto exit;
    }

    status = sss_key_store_get_key(&pCtx->ks, &obj, data, data_length, &binary_dataLength);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_get_key");
        goto exit;
    }

#if (SE05x_LOG_BINARY_FILE == 1)
    LOG_MAU8_I("\r\nSE05X: Read Binary File: ", data, *data_length);
#endif

exit:
    sss_key_object_free(&obj);
    return status;
}

sss_status_t se05x_DeleteBinaryFile(uint32_t objectId)
{
    sss_status_t status = kStatus_SSS_Fail;

    ex_sss_boot_ctx_t *pCtx = EX_SSS_BOOT_PCONTEXT;
    sss_object_t obj;

    status = sss_key_object_init(&obj, &pCtx->ks);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_init");
        goto exit;
    }

    status = sss_key_object_get_handle(&obj, objectId);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_get_handle");
        goto exit;
    }

    obj.keyId = objectId;
    status = sss_key_store_erase_key(&pCtx->ks, &obj);
    if (kStatus_SSS_Success != status)
    {
        LOG_E("Failed sss_key_object_erase_key");
        goto exit;
    }

exit:
    sss_key_object_free(&obj);
    return status;
}

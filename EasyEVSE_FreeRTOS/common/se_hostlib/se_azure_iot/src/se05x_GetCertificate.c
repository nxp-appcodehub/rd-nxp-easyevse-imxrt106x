/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */
#include "FreeRTOS.h"

#include <ex_sss.h>
#include <ex_sss_boot.h>
#include <fsl_sss_se05x_apis.h>
#include <nxLog_App.h>
#include <se05x_APDU.h>
#include <se05x_const.h>
#include <se05x_ecc_curves.h>
#include <se05x_ecc_curves_values.h>
#include <se05x_tlv.h>
#include <nxEnsure.h>

#include "se05x_sss_boot_pcontext.h"

#define SE05x_LOG_GET_CERTIFICATE 		1

sss_status_t se05x_GetCertificate(uint32_t certId,  uint8_t *certificate_data, size_t *certLength)
{
	size_t keyBitLen  = *certLength * 8;

    sss_status_t status = kStatus_SSS_Fail;
    int ret             = 0;
    uint32_t keyId = (uint32_t)(certId);

    ex_sss_boot_ctx_t *pCtx = EX_SSS_BOOT_PCONTEXT;
    sss_object_t obj;

    status = sss_key_object_init(&obj, &pCtx->ks);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

	status = sss_key_object_get_handle(&obj, keyId);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

	status = sss_key_store_get_key(&pCtx->ks, &obj, certificate_data, certLength, &keyBitLen);
	ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

#if (SE05x_LOG_GET_CERTIFICATE == 1)
	LOG_MAU8_I("\r\nSE05x:Get Certificate ",certificate_data, *certLength);
#endif

exit:
	return status;
}

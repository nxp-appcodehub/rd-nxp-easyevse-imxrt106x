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
#include <nxEnsure.h>

#include "se05x_sss_boot_pcontext.h"

#define SE05x_LOG_GET_RANDOM		1

sss_status_t se05x_GetRandom(uint8_t *random_data, size_t random_dataLen)
{
    sss_status_t status = kStatus_SSS_Fail;
    sss_rng_context_t sss_rng_ctx;

    ex_sss_boot_ctx_t *pCtx = EX_SSS_BOOT_PCONTEXT;

    status = sss_rng_context_init(&sss_rng_ctx, &pCtx->session);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

    status = sss_rng_get_random(&sss_rng_ctx, random_data, random_dataLen);
    sss_rng_context_free(&sss_rng_ctx);
    ENSURE_OR_GO_EXIT(status == kStatus_SSS_Success);

	#if (SE05x_LOG_GET_RANDOM == 1)
		LOG_MAU8_I("\r\nSE05x: Get Random ",random_data, random_dataLen);
	#endif


exit:
	return status;
}

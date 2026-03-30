/*
 * Copyright 2026 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef _MBEDTLS_HSM_H_
#define _MBEDTLS_HSM_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "mbedtls/pk.h"

#define HSM_LABEL "sss:"

void lws_tls_mbedtls_hsm_init(void);

int lws_tls_mbedtls_use_hsm_key(mbedtls_pk_context *pk_ctx, const char *key_label);

void lws_tls_mbedtls_free_hsm_key(mbedtls_pk_context *pk_ctx);

void lws_tls_mbedtls_hsm_deinit(void);

#ifdef __cplusplus
 }
#endif

#endif /* _MBEDTLS_HSM_H_ */

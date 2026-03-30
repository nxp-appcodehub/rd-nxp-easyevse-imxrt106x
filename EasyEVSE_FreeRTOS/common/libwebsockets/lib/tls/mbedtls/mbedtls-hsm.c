/*
 * Copyright 2026 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * SE050 integration for libwebsockets mbedTLS
 */

#include "private-lib-core.h"
#include "private-lib-tls-mbedtls.h"

#if defined(LWS_WITH_TLS_MBEDTLS_HSM) && (LWS_WITH_TLS_MBEDTLS_HSM == 1)

#include "core_pkcs11.h"
#include "core_pki_utils.h"
#include "pkcs11.h"
#include "core_pkcs11_config.h"

/* Include mbedTLS internal header to access pk_info_t structure */
#include "mbedtls/pk_internal.h"

#include "EVSE_Secure_Element.h"

/* SE050 context for libwebsockets */
typedef struct {
    CK_SESSION_HANDLE xP11Session;
    CK_OBJECT_HANDLE xP11PrivateKey;
    CK_KEY_TYPE xKeyType;
    CK_FUNCTION_LIST_PTR pxP11FunctionList;
    mbedtls_pk_info_t *privKeyInfo;
} lws_se050_key_ctx_t;

/* Forward declarations */
static int lws_se050_sign_callback(void *pvContext,
                                   mbedtls_md_type_t xMdAlg,
                                   const unsigned char *pucHash,
                                   size_t xHashLen,
                                   unsigned char *pucSig,
                                   size_t *pxSigLen,
                                   int (*piRng)(void *, unsigned char *, size_t),
                                   void *pvRng);

static void lws_se050_ctx_free_func(void* pk_ctx);

/**
 * Initialize SE050 key context for libwebsockets
 * Adapted from initializeClientKeys() in using_mbedtls.c
 */
static CK_RV
lws_se050_init_key(lws_se050_key_ctx_t *ctx, const char *pcLabelName)
{
    CK_RV xResult = CKR_OK;
    CK_SLOT_ID *pxSlotIds = NULL;
    CK_ULONG xCount = 0;
    CK_ATTRIBUTE xTemplate[2];
    mbedtls_pk_type_t xKeyAlgo = (mbedtls_pk_type_t)~0;

    /* Initialize PKCS#11 */
    xResult = xInitializePkcs11Session(&ctx->xP11Session);
    if (xResult != CKR_OK) {
        lwsl_err("%s: Failed to initialize PKCS#11 session: 0x%lx\n", 
                 __func__, xResult);
        return xResult;
    }

    C_GetFunctionList(&ctx->pxP11FunctionList);

    if ((ctx->pxP11FunctionList->C_GetSlotList == NULL) || 
    (ctx->pxP11FunctionList->C_Login == NULL) || 
    (ctx->pxP11FunctionList->C_GetAttributeValue == NULL))
    {
        lwsl_err("%s: Function list not properly initialized\n", __func__);
        return CKR_GENERAL_ERROR;
    }

    /* Get the PKCS #11 module/token slot count */
    xResult = ctx->pxP11FunctionList->C_GetSlotList(CK_TRUE, NULL, &xCount);
    if (xResult != CKR_OK) {
        lwsl_err("%s: C_GetSlotList failed: 0x%lx\n", __func__, xResult);
        return xResult;
    }

    /* Allocate memory to store the token slots */
    pxSlotIds = (CK_SLOT_ID *)lws_malloc(sizeof(CK_SLOT_ID) * xCount, __func__);
    if (!pxSlotIds) {
        lwsl_err("%s: Failed to allocate slot list\n", __func__);
        return CKR_HOST_MEMORY;
    }

    /* Get all of the available private key slot identities */
    xResult = ctx->pxP11FunctionList->C_GetSlotList(CK_TRUE, pxSlotIds, &xCount);
    if (xResult != CKR_OK) {
        lwsl_err("%s: C_GetSlotList (2) failed: 0x%lx\n", __func__, xResult);
        goto cleanup;
    }

    /* Put the module in authenticated mode */
    xResult = ctx->pxP11FunctionList->C_Login(ctx->xP11Session,
                                              CKU_USER,
                                              (CK_UTF8CHAR_PTR)configPKCS11_DEFAULT_USER_PIN,
                                              sizeof(configPKCS11_DEFAULT_USER_PIN) - 1);
    if (xResult != CKR_OK) {
        lwsl_err("%s: C_Login failed: 0x%lx\n", __func__, xResult);
        goto cleanup;
    }

    /* Get the handle of the device private key */
    xResult = xFindObjectWithLabelAndClass(ctx->xP11Session,
                                           (char *)pcLabelName,
                                           strnlen(pcLabelName, pkcs11configMAX_LABEL_LENGTH),
                                           CKO_PRIVATE_KEY,
                                           &ctx->xP11PrivateKey);
    if (xResult != CKR_OK || ctx->xP11PrivateKey == CK_INVALID_HANDLE) {
        lwsl_err("%s: Could not find private key with label '%s': 0x%lx\n",
                 __func__, pcLabelName, xResult);
        xResult = CK_INVALID_HANDLE;
        goto cleanup;
    }

    lwsl_notice("%s: Found SE050 key with label '%s', handle=0x%lx\n",
                __func__, pcLabelName, ctx->xP11PrivateKey);

    /* Query the device private key type */
    xTemplate[0].type = CKA_KEY_TYPE;
    xTemplate[0].pValue = &ctx->xKeyType;
    xTemplate[0].ulValueLen = sizeof(CK_KEY_TYPE);
    xResult = ctx->pxP11FunctionList->C_GetAttributeValue(ctx->xP11Session,
                                                          ctx->xP11PrivateKey,
                                                          xTemplate, 1);
    if (xResult != CKR_OK) {
        lwsl_err("%s: Failed to get key type: 0x%lx\n", __func__, xResult);
        goto cleanup;
    }

    /* Map the PKCS #11 key type to an mbedTLS algorithm */
    switch (ctx->xKeyType) {
    case CKK_RSA:
        xKeyAlgo = MBEDTLS_PK_RSA;
        lwsl_notice("%s: Key type: RSA\n", __func__);
        break;
    case CKK_EC:
        xKeyAlgo = MBEDTLS_PK_ECKEY;
        lwsl_notice("%s: Key type: EC\n", __func__);
        break;
    default:
        lwsl_err("%s: Unsupported key type: 0x%lx\n", __func__, ctx->xKeyType);
        xResult = CKR_ATTRIBUTE_VALUE_INVALID;
        goto cleanup;
    }

    /* Allocate pk_info on heap */
        ctx->privKeyInfo = (mbedtls_pk_info_t *)lws_zalloc(sizeof(mbedtls_pk_info_t), __func__);
        if (!ctx->privKeyInfo) {
            lwsl_err("%s: Failed to allocate pk_info\n", __func__);
            xResult = CKR_HOST_MEMORY;
            goto cleanup;
        }

    /* Map the mbedTLS algorithm to its internal metadata */
    memcpy(ctx->privKeyInfo, mbedtls_pk_info_from_type(xKeyAlgo),
           sizeof(mbedtls_pk_info_t));

    /* Assign unimplemented function pointers to NULL */
    ctx->privKeyInfo->get_bitlen = NULL;
    ctx->privKeyInfo->verify_func = NULL;
#if defined(MBEDTLS_ECDSA_C) && defined(MBEDTLS_ECP_RESTARTABLE)
    ctx->privKeyInfo->verify_rs_func = NULL;
    ctx->privKeyInfo->sign_rs_func = NULL;
#endif
    ctx->privKeyInfo->decrypt_func = NULL;
    ctx->privKeyInfo->encrypt_func = NULL;
    ctx->privKeyInfo->check_pair_func = NULL;
    ctx->privKeyInfo->ctx_alloc_func = NULL;
    ctx->privKeyInfo->ctx_free_func = lws_se050_ctx_free_func;
#if defined(MBEDTLS_ECDSA_C) && defined(MBEDTLS_ECP_RESTARTABLE)
    ctx->privKeyInfo->rs_alloc_func = NULL;
    ctx->privKeyInfo->rs_free_func = NULL;
#endif
    ctx->privKeyInfo->debug_func = NULL;

    /* Set our custom signing callback */
    ctx->privKeyInfo->sign_func = lws_se050_sign_callback;

cleanup:
    lws_free(pxSlotIds);
    return xResult;
}

/**
 * Signing callback for SE050 keys
 * Adapted from privateKeySigningCallback() in using_mbedtls.c
 */
static int
lws_se050_sign_callback(void *pvContext,
                       mbedtls_md_type_t xMdAlg,
                       const unsigned char *pucHash,
                       size_t xHashLen,
                       unsigned char *pucSig,
                       size_t *pxSigLen,
                       int (*piRng)(void *, unsigned char *, size_t),
                       void *pvRng)
{
    CK_RV xResult = CKR_OK;
    lws_se050_key_ctx_t *ctx = (lws_se050_key_ctx_t *)pvContext;
    CK_MECHANISM xMech = {0};
    CK_BYTE xToBeSigned[256];
    CK_ULONG xToBeSignedLen = sizeof(xToBeSigned);

    (void)piRng;
    (void)pvRng;
    (void)xMdAlg;

    /* Sanity check buffer length */
    if (xHashLen > sizeof(xToBeSigned)) {
        lwsl_err("%s: Hash too large: %zu\n", __func__, xHashLen);
        return CKR_ARGUMENTS_BAD;
    }

    /* Format the hash data to be signed */
    if (CKK_RSA == ctx->xKeyType) {
        xMech.mechanism = CKM_RSA_PKCS;
        
        /* mbedTLS expects hashed data without padding, but PKCS #11 C_Sign
         * performs hash & sign if hash algorithm is specified. This helper
         * function applies padding indicating data was hashed with SHA-256. */
        xResult = vAppendSHA256AlgorithmIdentifierSequence((uint8_t *)pucHash, 
                                                           xToBeSigned);
        if (xResult != CKR_OK) {
            lwsl_err("%s: Failed to append SHA256 identifier\n", __func__);
            return -1;
        }
        xToBeSignedLen = pkcs11RSA_SIGNATURE_INPUT_LENGTH;
        
    } else if (CKK_EC == ctx->xKeyType) {
        xMech.mechanism = CKM_ECDSA;
        memcpy(xToBeSigned, pucHash, xHashLen);
        xToBeSignedLen = xHashLen;
        
    } else {
        lwsl_err("%s: Unsupported key type: 0x%lx\n", __func__, ctx->xKeyType);
        return CKR_ARGUMENTS_BAD;
    }

    /* Use the PKCS#11 module to sign */
    xResult = ctx->pxP11FunctionList->C_SignInit(ctx->xP11Session,
                                                 &xMech,
                                                 ctx->xP11PrivateKey);
    if (xResult != CKR_OK) {
        lwsl_err("%s: C_SignInit failed: 0x%lx\n", __func__, xResult);
        return -1;
    }

    *pxSigLen = sizeof(xToBeSigned);
    xResult = ctx->pxP11FunctionList->C_Sign(ctx->xP11Session,
                                             xToBeSigned,
                                             xToBeSignedLen,
                                             pucSig,
                                             (CK_ULONG_PTR)pxSigLen);
    if (xResult != CKR_OK) {
        lwsl_err("%s: C_Sign failed: 0x%lx\n", __func__, xResult);
        return -1;
    }

    /* For ECC, convert PKCS#11 signature format to ASN.1 */
    if (CKK_EC == ctx->xKeyType) {
        /* PKCS #11 for P256 returns a 64-byte signature with 32 bytes for R
         * and 32 bytes for S. This must be converted to ASN.1 encoded array. */
        if (*pxSigLen != pkcs11ECDSA_P256_SIGNATURE_LENGTH) {
            lwsl_err("%s: Unexpected ECC signature length: %zu\n", 
                     __func__, *pxSigLen);
            return -1;
        }

        PKI_pkcs11SignatureTombedTLSSignature(pucSig, pxSigLen);
    }

    lwsl_info("%s: Successfully signed with SE050, signature length=%zu\n",
              __func__, *pxSigLen);

    return 0;
}

/**
 * Setup mbedTLS pk_context to use SE050 key
 */
int
lws_tls_mbedtls_use_hsm_key(mbedtls_pk_context *pk_ctx, const char *key_label)
{
    lws_se050_key_ctx_t *se_ctx;
    CK_RV result;

    if (!pk_ctx || !key_label) {
        lwsl_err("%s: Invalid parameters\n", __func__);
        return -1;
    }

    /* Allocate SE050 context */
    se_ctx = (lws_se050_key_ctx_t *)lws_zalloc(sizeof(lws_se050_key_ctx_t),
                                                __func__);
    if (!se_ctx) {
        lwsl_err("%s: Failed to allocate SE050 context\n", __func__);
        return -1;
    }

    /* Initialize SE050 key */
    result = lws_se050_init_key(se_ctx, key_label);
    if (result != CKR_OK) {
        lwsl_err("%s: Failed to initialize SE050 key: 0x%lx\n", 
                 __func__, result);
        lws_free(se_ctx);
        return -1;
    }

    /* Setup mbedTLS pk_context */
    pk_ctx->pk_info = se_ctx->privKeyInfo;
    pk_ctx->pk_ctx = se_ctx;  /* Store our context as the pk_ctx */

    lwsl_notice("%s: Successfully configured SE050 key '%s'\n", 
                __func__, key_label);

    return 0;
}

/**
 * Cleanup SE050 key context
 */
static void lws_se050_ctx_free_func(void* pk_ctx)
{
	lws_se050_key_ctx_t *se_ctx = (lws_se050_key_ctx_t *)pk_ctx;

	if (!se_ctx)
	{
		return;
	}

	/* Close PKCS#11 session */
	if (se_ctx->pxP11FunctionList && se_ctx->xP11Session && se_ctx->pxP11FunctionList->C_CloseSession) 
    {
		se_ctx->pxP11FunctionList->C_CloseSession(se_ctx->xP11Session);
	}

	/* Free the heap-allocated pk_info */
	if (se_ctx->privKeyInfo) {
		lws_free(se_ctx->privKeyInfo);
		se_ctx->privKeyInfo = NULL;
	}

	se_ctx->xP11PrivateKey = CK_INVALID_HANDLE;
	se_ctx->xKeyType = 0;
	se_ctx->pxP11FunctionList = NULL;

	lws_free(se_ctx);

	EVSE_SE050_Close_Session();
}

/**
 * Cleanup SE050 key context
 */
void
lws_tls_mbedtls_free_hsm_key(mbedtls_pk_context *pk_ctx)
{
    lws_se050_key_ctx_t *se_ctx;

    if (!pk_ctx || !pk_ctx->pk_ctx)
        return;

    se_ctx = (lws_se050_key_ctx_t *)pk_ctx->pk_ctx;

    /* Close PKCS#11 session */
    if (se_ctx->pxP11FunctionList && se_ctx->xP11Session && se_ctx->pxP11FunctionList->C_CloseSession) 
    {
        se_ctx->pxP11FunctionList->C_CloseSession(se_ctx->xP11Session);
    }

    /* Free the heap-allocated pk_info */
    if (se_ctx->privKeyInfo) {
        lws_free(se_ctx->privKeyInfo);
        se_ctx->privKeyInfo = NULL;
    }

    se_ctx->xP11PrivateKey = CK_INVALID_HANDLE;
    se_ctx->xKeyType = 0;
    se_ctx->pxP11FunctionList = NULL;

    lws_free(se_ctx);
    pk_ctx->pk_ctx = NULL;
    pk_ctx->pk_info = NULL;
}

/**
 * Load certificate from SE050 into mbedTLS context
 * Adapted from readCertificateIntoContext() in using_mbedtls.c
 */
int
lws_tls_mbedtls_use_se050_cert(mbedtls_x509_crt *cert_ctx, const char *cert_label)
{
    CK_RV xResult = CKR_OK;
    CK_SESSION_HANDLE xSession;
    CK_FUNCTION_LIST_PTR pxFunctionList;
    CK_ATTRIBUTE xTemplate = {0};
    CK_OBJECT_HANDLE xCertObj = 0;

    if (!cert_ctx || !cert_label) {
        lwsl_err("%s: Invalid parameters\n", __func__);
        return -1;
    }

    /* Initialize PKCS#11 session */
    xResult = xInitializePkcs11Session(&xSession);
    if (xResult != CKR_OK) {
        lwsl_err("%s: Failed to initialize PKCS#11 session: 0x%lx\n",
                 __func__, xResult);
        return -1;
    }

    C_GetFunctionList(&pxFunctionList);

    if ((pxFunctionList->C_CloseSession == NULL) || (pxFunctionList->C_GetAttributeValue == NULL))
    {
        lwsl_err("%s: Function list not properly initialized\n", __func__);
        return -1;
    }

    /* Get the handle of the certificate */
    xResult = xFindObjectWithLabelAndClass(xSession,
                                           (char *)cert_label,
                                           strnlen(cert_label, pkcs11configMAX_LABEL_LENGTH),
                                           CKO_CERTIFICATE,
                                           &xCertObj);

    if ((xResult != CKR_OK) || (xCertObj == CK_INVALID_HANDLE)) {
        lwsl_err("%s: Could not find certificate with label '%s': 0x%lx\n",
                 __func__, cert_label, xResult);
        pxFunctionList->C_CloseSession(xSession);
        return -1;
    }

    lwsl_notice("%s: Found SE050 certificate with label '%s', handle=0x%lx\n",
                __func__, cert_label, xCertObj);

    /* Query the certificate size */
    xTemplate.type = CKA_VALUE;
    xTemplate.ulValueLen = 0;
    xTemplate.pValue = NULL;
    xResult = pxFunctionList->C_GetAttributeValue(xSession, xCertObj, &xTemplate, 1);
    if (xResult != CKR_OK) {
        lwsl_err("%s: Failed to get certificate size: 0x%lx\n", __func__, xResult);
        pxFunctionList->C_CloseSession(xSession);
        return -1;
    }

    /* Allocate buffer for the certificate */
    xTemplate.pValue = lws_malloc(xTemplate.ulValueLen, __func__);
    if (!xTemplate.pValue) {
        lwsl_err("%s: Failed to allocate certificate buffer\n", __func__);
        pxFunctionList->C_CloseSession(xSession);
        return -1;
    }

    /* Export the certificate */
    xResult = pxFunctionList->C_GetAttributeValue(xSession, xCertObj, &xTemplate, 1);
    if (xResult != CKR_OK) {
        lwsl_err("%s: Failed to export certificate: 0x%lx\n", __func__, xResult);
        lws_free(xTemplate.pValue);
        pxFunctionList->C_CloseSession(xSession);
        return -1;
    }

    /* Parse the certificate into mbedTLS context */
    xResult = mbedtls_x509_crt_parse(cert_ctx,
                                     (const unsigned char *)xTemplate.pValue,
                                     xTemplate.ulValueLen);
    if (xResult != 0) {
        lwsl_err("%s: Failed to parse certificate: -0x%x\n", __func__, -xResult);
        lws_free(xTemplate.pValue);
        pxFunctionList->C_CloseSession(xSession);
        return -1;
    }

    lwsl_notice("%s: Successfully loaded certificate from SE050\n", __func__);

    /* Free memory */
    lws_free(xTemplate.pValue);
    pxFunctionList->C_CloseSession(xSession);

    return 0;
}

void lws_tls_mbedtls_hsm_init(void)
{
	EVSE_SE050_Open_Session();
}

void lws_tls_mbedtls_hsm_deinit(void)
{
	EVSE_SE050_Close_Session();
}

#endif /* LWS_WITH_TLS_MBEDTLS_HSM */

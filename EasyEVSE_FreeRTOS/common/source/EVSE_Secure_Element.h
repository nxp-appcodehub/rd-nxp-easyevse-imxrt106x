/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#ifndef EVSE_SECURE_ELEMENT_H
#define EVSE_SECURE_ELEMENT_H

#include "ex_sss_boot.h"
#include "se05x_BinaryFile.h"
#include "se05x_GetCertificate.h"

#define AZURE_IOT_CLIENTCERT_BUFSIZE 500

#define CHECK_EMPTY_HOSTNAME(X) (strlen((char *)X) == 0) ? 1 : 0

extern ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx;
extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;

typedef enum
{
	kStatus_Session_Success = 0x00,
	kStatus_Session_Failed,
    kStatus_Hostname_Success,
    kStatus_Hostname_Failed,
	kStatus_Certificates_Success,
    kStatus_Certificates_Failed,
    kStatus_CPOKey_Success,
    kStatus_CPOKey_Failed
} EVSE_SE05X_status_t;

typedef enum _se_events
{
    SE_NOT_INITIALIZED_EVENT = 1 << 0,
    SE_INIT_SUCCESS_EVENT    = 1 << 1,
    SE_INIT_FAIL_EVENT       = 1 << 2,
    SE_ALL_EVENTS            = SE_NOT_INITIALIZED_EVENT | SE_INIT_SUCCESS_EVENT | SE_INIT_FAIL_EVENT
} se_events_t;


/**
 * EVSE Secure Element entry point.
 * Handles the initialization of Secure Element.
 *
 */
void EVSE_Secure_Element_Init(void);

/**
 * @brief Opens a Secure Element session.
 */
EVSE_SE05X_status_t EVSE_SE050_Open_Session(void);

/**
 * @brief Closes a Secure Element session.
 */
void EVSE_SE050_Close_Session(void);

/**
 * @brief Checks if the certificate is stored.
 * @param [in/out] len The length of the certificate
 * @return kStatus_Certificates_Success on succeeded operation or kStatus_Certificates_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Check_Cloud_Certificate(uint32_t *len);

/**
 * @brief Sets the certificate and key indexes to a cloud context.
 * @param ctx The cloud context to set the indexes to.
 * @return kStatus_Certificates_Success on succeeded operation or kStatus_Certificates_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Set_CertificateIdx(ex_sss_cloud_ctx_t *ctx);

/**
 * @brief Retrieves the device id from a x.509 certificate
 * @param deviceid Buffer where the device id is stored
 * @param len The length of the provided buffer.
 * @return kStatus_Certificates_Success on succeeded operation or kStatus_Certificates_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Get_DeviceIDFromCertificate(uint8_t *deviceid, size_t len);

/**
 * @brief Deletes the certificate object and the associated key.
 * @return kStatus_Certificates_Success on succeeded operation or kStatus_Certificates_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Delete_Cloud_Certificate(void);

/**
 * @brief Receive the hostname and its length
 * @param hostname Points to the memory location where the hostname is stored
 * @param len Holds the length of the provided buffer. After call it will hold the actual hostname length
 * @return kStatus_Hostname_Success on succeeded operation or kStatus_Hostname_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Get_ServerHostname(char *hostname, uint32_t *len);

/**
 * @brief Set the  Hostname
 * @param hostname received from DPS path
 * @return kStatus_Hostname_Success on succeeded operation or kStatus_Hostname_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Set_ServerHostname(const char *hostname, uint32_t len);

/**
 * @brief Delete the Hostname from SE05X memory
 * @return kStatus_Hostname_Success on succeeded operation or kStatus_Hostname_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Delete_ServerHostname(void);

/**
 * @brief Retrieves the OID value from certificate
 * @return kStatus_Hostname_Success on succeeded operation or kStatus_Hostname_Failed on failed operation
 */
EVSE_SE05X_status_t get_oid_value_in_subject(
    uint8_t *cert_buffer, size_t cert_len, char *oid, char *value, size_t max_size);

#if ENABLE_ISO15118
/**
 * @brief Checks if the cpo key is stored in the secured element.
 * @return kStatus_CPOKey_Success on succeeded operation or kStatus_CPOKey_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Check_CPOKey(void);

/**
 * @brief Stores CPO key to SE05X in the appropriate format
 * @return kStatus_CPOKey_Success on succeeded operation or kStatus_CPOKey_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Handle_CPOKey(void);

/**
 * @brief Delete the CPO key from SE05X memory
 * @return kStatus_CPOKey_Success on succeeded operation or kStatus_CPOKey_Failed on failed operation
 */
EVSE_SE05X_status_t EVSE_SE050_Delete_CPOKey(void);
#endif /* ENABLE_ISO15118 */

#endif /* EVSE_SECURE_ELEMENT_H */

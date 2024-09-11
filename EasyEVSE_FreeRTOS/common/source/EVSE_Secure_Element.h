/*
 * Copyright 2023-2024 NXP
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

#define AZURE_IOT_KEY_INDEX_SM          0x223344
#define AZURE_IOT_CLIENT_CERT_INDEX_SM  0x223345
#define AZURE_IOT_HOSTNAME_INDEX_SM     0x000013

#define CERT_BUFFER_SIZE 500

#define CHECK_EMPTY_HOSTNAME(X) (strlen((char *)X) == 0) ? 1 : 0

extern ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx;
extern ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx;

typedef enum
{
    /** Operation was successful */
    kStatus_Hostname_Success = 0x00,
    /** Operation failed */
    kStatus_Hostname_Failed
} hostname_config_status_t;

typedef enum _se_events
{
    SE_NOT_INITIALIZED_EVENT = 1 << 0,
    SE_INIT_SUCCESS_EVENT    = 1 << 1,
    SE_INIT_FAIL_EVENT       = 1 << 2,
    SE_ALL_EVENTS            = SE_NOT_INITIALIZED_EVENT | SE_INIT_SUCCESS_EVENT | SE_INIT_FAIL_EVENT
} se_events_t;

typedef enum
{
    /** Operation was successful */
    kStatus_Certificates_Success = 0x00,
    /** Operation failed */
    kStatus_Certificates_Failed
} certificates_status_t;

/**
 * EVSE Secure Element entry point.
 * Handles the initialization of Secure Element.
 *
 */
void EVSE_Secure_Element_Init(void);

/**
 * @brief Opens a Secure Element session.
 */
void EVSE_SE050_Open_Session(void);

/**
 * @brief Closes a Secure Element session.
 */
void EVSE_SE050_Close_Session(void);

/**
 * @brief Checks if the certificate is stored.
 * @param certidx The location where the certificate is stored
 * @param [in/out] len The length of the certificate
 * @return kStatus_Certificates_Success on succeeded operation or kStatus_Certificates_Failed on failed operation
 */
certificates_status_t EVSE_SE050_Check_Certificate(uint32_t certidx, uint32_t *len);

/**
 * @brief Retrieves the device id from a x.509 certificate
 * @param deviceid Buffer where the device id is stored
 * @param len The length of the provided buffer.
 * @return kStatus_Certificates_Success on succeeded operation or kStatus_Certificates_Failed on failed operation
 */
certificates_status_t EVSE_SE050_Get_DeviceIDFromCertificate(uint8_t *deviceid, size_t len);

/**
 * @brief Deletes the certificate object and the associated key.
 * @param certidx The location where the certificate is stored
 * @param keyidx The location where the key associated with the certificate is stored
 * @return kStatus_Certificates_Success on succeeded operation or kStatus_Certificates_Failed on failed operation
 */
certificates_status_t EVSE_SE050_Delete_Certificate(uint32_t certidx, uint32_t keyidx);

/**
 * @brief Receive the hostname and its length
 * @param hostname Points to the memory location where the hostname is stored
 * @param len Holds the length of the provided buffer. After call it will hold the actual hostname length
 * @return kStatus_Hostname_Success on succeeded operation or kStatus_Hostname_Failed on failed operation
 */
hostname_config_status_t EVSE_SE050_Get_ServerHostname(char* hostname, uint32_t* len);

/**
 * @brief Set the  Hostname
 * @param hostname received from DPS path
 * @return kStatus_Hostname_Success on succeeded operation or kStatus_Hostname_Failed on failed operation
 */
hostname_config_status_t EVSE_SE050_Set_ServerHostname(const char* hostname, uint32_t len);

/**
 * @brief Delete the Hostname from SE05X memory
 * @return kStatus_Hostname_Success on succeeded operation or kStatus_Hostname_Failed on failed operation
 */
hostname_config_status_t EVSE_SE050_Delete_ServerHostname(uint32_t index);

#endif /* EVSE_SECURE_ELEMENT_H */

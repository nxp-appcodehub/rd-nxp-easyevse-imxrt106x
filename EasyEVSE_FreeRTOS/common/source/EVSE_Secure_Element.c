/*
 * Copyright 2023-2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "event_groups.h"

/* SE05X includes */
#include "ex_sss_boot.h"
#include "fsl_sss_se05x_apis.h"
#include "nxLog_App.h"
#include "se05x_APDU.h"
#include "se05x_const.h"
#include "se05x_ecc_curves.h"
#include "se05x_ecc_curves_values.h"
#include "se05x_sss_boot_pcontext.h"
#include "se05x_tlv.h"
#include "EVSE_Secure_Element.h"

#if ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1))
#include <mbedtls/sha256.h>
#include <mbedtls/oid.h>
#include <mbedtls/x509_crt.h>
#endif /* ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1)) */

#if (EVSE_X509_AUTH == 1)
#include "azure_iot_config.h"
#endif
/*******************************************************************************
 * Definitions
 ******************************************************************************/
//AT_NONCACHEABLE_SECTION_ALIGN(ex_sss_boot_ctx_t gex_sss_demo_boot_ctx,64);
//AT_NONCACHEABLE_SECTION_ALIGN(ex_sss_cloud_ctx_t gex_sss_demo_tls_ctx,64);
ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;
ex_sss_boot_ctx_t *pex_sss_demo_boot_ctx = &gex_sss_demo_boot_ctx;

ex_sss_cloud_ctx_t gex_sss_demo_tls_ctx;
ex_sss_cloud_ctx_t *pex_sss_demo_tls_ctx = &gex_sss_demo_tls_ctx;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#if ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1))
static certificates_status_t get_oid_value_in_subject(
    uint8_t *cert_buffer, size_t cert_len, char *oid, char *value, size_t max_size);
#endif /* ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1)) */
/*******************************************************************************
 * Variables
 ******************************************************************************/
static se_events_t se_event_flag;

#if (EVSE_X509_SE050_AUTH == 1)
static uint8_t certificateBuffer[CERT_BUFFER_SIZE];
#endif /* ((EVSE_X509_SE050_AUTH == 1) */

#if (EVSE_X509_AUTH == 1)
static uint8_t *certificatePtr = NULL;
#endif /* (EVSE_X509_AUTH == 1)) */
/*******************************************************************************
 * Code
 ******************************************************************************/

#if 0
/* This task handles the initialization of the SE05x secure element. */
void EVSE_Secure_Element_Task(void *args)
{

    sss_status_t status = kStatus_SSS_Fail;
    const char *portName = NULL;

    /* NXP SE05x Secure Element initialization */
    configPRINTF((info("Starting Comms to SE050...\r\n")));

    memset(PCONTEXT, 0, sizeof(*PCONTEXT));

    configPRINTF((info("Running ex_sss_boot_open()...")));
    status = ex_sss_boot_open(&gex_sss_demo_boot_ctx, portName);
    if (status != kStatus_SSS_Success)
    {
        se_event_flag = SE_INIT_FAIL_EVENT;
        configPRINTF((error("ex_sss_session_open Failed")));
        goto end_task;
    }
    else
    {
        se_event_flag = SE_INIT_SUCCESS_EVENT;
    }

    if (((PCONTEXT)->session.subsystem) == kType_SSS_SubSystem_NONE)
    {
        /* Nothing to do. Device is not opened
         * This is needed for the case when we open a generic communication
         * channel, without being specific to SE05X
         */
    }
    else
    {
        status = ex_sss_key_store_and_object_init((PCONTEXT));
        if (status != kStatus_SSS_Success)
        {
            configPRINTF((error("ex_sss_key_store_and_object_init Failed")));
            goto end_task;
        }
    }

    pex_sss_demo_tls_ctx->client_keyPair_index = AZURE_IOT_KEY_INDEX_SM;
    pex_sss_demo_tls_ctx->client_cert_index    = AZURE_IOT_CLIENT_CERT_INDEX_SM;

end_task:
    vTaskDelete(NULL);

}
#endif /* 0 */

void EVSE_SE050_Open_Session(void)
{
    sss_status_t status  = kStatus_SSS_Fail;
    const char *portName = NULL;

    /* NXP SE05x Secure Element initialization */
    configPRINTF((info("Starting Comms to SE050...\r\n")));

    // memset(PCONTEXT, 0, sizeof(*PCONTEXT));

    configPRINTF((info("Running ex_sss_boot_open()...")));
    status = ex_sss_boot_open(&gex_sss_demo_boot_ctx, portName);
    if (status != kStatus_SSS_Success)
    {
        se_event_flag = SE_INIT_FAIL_EVENT;
        configPRINTF((error("ex_sss_session_open Failed")));
        goto exit;
    }

    se_event_flag = SE_INIT_SUCCESS_EVENT;

    if (((PCONTEXT)->session.subsystem) == kType_SSS_SubSystem_NONE)
    {
        /* Nothing to do. Device is not opened
         * This is needed for the case when we open a generic communication
         * channel, without being specific to SE05X
         */
    }
    else
    {
        status = ex_sss_key_store_and_object_init((PCONTEXT));
        if (status != kStatus_SSS_Success)
        {
            configPRINTF((error("ex_sss_key_store_and_object_init Failed")));
            goto exit;
        }
    }

exit:
    return;
}

void EVSE_SE050_Close_Session(void)
{
    ex_sss_session_close(&gex_sss_demo_boot_ctx);
    se_event_flag = SE_NOT_INITIALIZED_EVENT;
}

certificates_status_t EVSE_SE050_Get_DeviceIDFromCertificate(uint8_t *deviceid, size_t len)
{
#if ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1))
    certificates_status_t cert_status = kStatus_Certificates_Failed;
    uint32_t cert_size                = CERT_BUFFER_SIZE;

    if (deviceid == NULL)
    {
        return kStatus_Certificates_Failed;
    }

#if (EVSE_X509_SE050_AUTH == 1)
    if (se_event_flag != SE_INIT_SUCCESS_EVENT)
    {
        return kStatus_Certificates_Failed;
    }
#endif /* EVSE_X509_SE050_AUTH == 1 */

    cert_status = EVSE_SE050_Check_Certificate(AZURE_IOT_CLIENT_CERT_INDEX_SM, &cert_size);
    if (cert_status != kStatus_Certificates_Success)
    {
        return kStatus_Certificates_Failed;
    }

#if (EVSE_X509_SE050_AUTH == 1)
    cert_status = get_oid_value_in_subject((uint8_t *)&certificateBuffer[0], cert_size, "CN", &deviceid[0], len);

    memset((uint8_t *)&certificateBuffer[0], 0, cert_size);
#endif /* EVSE_X509_SE050_AUTH == 1 */

#if (EVSE_X509_AUTH == 1)
    cert_status = get_oid_value_in_subject(certificatePtr, cert_size, "CN", &deviceid[0], len);
#endif /* EVSE_X509_AUTH == 1*/

    if (cert_status == kStatus_Certificates_Success)
    {
        configPRINTF(("Certificate was issued for device id: %s\r\n", deviceid));
    }

    return cert_status;
#endif /* ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1)) */
}

certificates_status_t EVSE_SE050_Check_Certificate(uint32_t certidx, uint32_t *certsize)
{
    sss_status_t status = kStatus_SSS_Fail;

    if (certsize == NULL)
    {
        return status;
    }

#if (EVSE_X509_SE050_AUTH == 1)
    if (se_event_flag == SE_INIT_SUCCESS_EVENT)
    {
        status = se05x_GetCertificate(certidx, (uint8_t *)&certificateBuffer[0], certsize);
    }

#elif (EVSE_X509_AUTH == 1)
    certificatePtr = (const unsigned char *)X509_CERTIFICATE_PEM;
    if (certificatePtr != NULL)
    {
        *certsize = sizeof(X509_CERTIFICATE_PEM);
        status    = kStatus_SSS_Success;
    }
#endif

    return (status == kStatus_SSS_Success) ? kStatus_Certificates_Success : kStatus_Certificates_Failed;
}

certificates_status_t EVSE_SE050_Delete_Certificate(uint32_t certidx, uint32_t keyidx)
{
    sss_status_t status                      = kStatus_SSS_Fail;
    sss_status_t keystatus                   = kStatus_SSS_Fail;
    certificates_status_t status_certificate = kStatus_Certificates_Failed;

    if (se_event_flag == SE_INIT_SUCCESS_EVENT)
    {
        keystatus = se05x_DeleteBinaryFile(keyidx);
        status    = se05x_DeleteBinaryFile(certidx);
        if ((status == kStatus_SSS_Success) && (keystatus == kStatus_SSS_Success))
        {
            status_certificate = kStatus_Certificates_Success;
        }
    }

    return status_certificate;
}

hostname_config_status_t EVSE_SE050_Get_ServerHostname(char* hostname, uint32_t* hostname_len)
{
    hostname_config_status_t status_hostname_s = kStatus_Hostname_Failed;

    if (hostname == NULL)
    {
        goto end;
    }

#if defined(HOST_NAME)
    unsigned char* host_name_buff = (unsigned char*)HOST_NAME;

    if (CHECK_EMPTY_HOSTNAME(host_name_buff))
    {
        configPRINTF((error("Hostname is NOT present in plain text\r\n")));
        status_hostname_s = kStatus_Hostname_Failed;
    }
    else
    {
        configPRINTF((info("Hostname is present in plain text\r\n")));
        *hostname_len = strlen(host_name_buff);
        if (*hostname_len < EVSE_HOSTNAME_MAX_BUFFER)
        {
            memcpy(hostname, host_name_buff, (size_t)*hostname_len);
            status_hostname_s = kStatus_Hostname_Success;
        }
        else
        {
            status_hostname_s = kStatus_Hostname_Failed;
        }
    }
#else
    if (se_event_flag == SE_INIT_SUCCESS_EVENT)
    {
        sss_status_t status = kStatus_SSS_Fail;

        memset((char*)hostname, 0x00, (size_t)*hostname_len);
        status = se05x_ReadBinaryFile(AZURE_IOT_HOSTNAME_INDEX_SM, (uint8_t*)hostname, (size_t*)hostname_len);
        status_hostname_s = (status == kStatus_SSS_Success) ? kStatus_Hostname_Success : kStatus_Hostname_Failed;
    }

#endif /* defined(HOST_NAME) */

end:
    return status_hostname_s;

}

hostname_config_status_t EVSE_SE050_Set_ServerHostname(const char* hostname, uint32_t hostname_len)
{
    hostname_config_status_t status_hostname_s = kStatus_Hostname_Failed;
    sss_status_t status = kStatus_SSS_Fail;

    if (hostname == NULL)
    {
        status_hostname_s = kStatus_Hostname_Failed;
        goto end;
    }

    if (se_event_flag == SE_INIT_SUCCESS_EVENT)
    {
        uint8_t *binary_data_buff = (char*)hostname;
        size_t size_buff = (size_t)hostname_len;
        status = se05x_WriteBinaryFile(AZURE_IOT_HOSTNAME_INDEX_SM, binary_data_buff, size_buff);

        if (status != kStatus_SSS_Success)
        {
            configPRINTF((error("Failed to retrieve hostname from SE\r\n")));
            status_hostname_s = kStatus_Hostname_Failed;
            goto end;
        }
        else
        {
            status_hostname_s = kStatus_Hostname_Success;
        }
    }

end:
    return status_hostname_s;
}

hostname_config_status_t EVSE_SE050_Delete_ServerHostname(uint32_t index)
{
    sss_status_t status = kStatus_SSS_Fail;
    hostname_config_status_t status_hostname_s = kStatus_Hostname_Failed;

    if (se_event_flag == SE_INIT_SUCCESS_EVENT)
    {
        status = se05x_DeleteBinaryFile(index);
        if (status == kStatus_SSS_Success)
        {
            configPRINTF(("Successfully deleted hostname from SE\r\n"));
            status_hostname_s = kStatus_Hostname_Success;
        }
        else
        {
            configPRINTF(("Failed to delete hostname from SE\r\n"));
        }
    }

    return status_hostname_s;
}

static certificates_status_t get_oid_value_in_subject(
    uint8_t *cert_buffer, size_t cert_len, char *oid, char *value, size_t max_size)
{
#if ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1))
    certificates_status_t cert_status = kStatus_Certificates_Success;
    mbedtls_x509_crt client_cert      = {0};
    char *oid_name                    = NULL;
    mbedtls_x509_name *oid_ptr;
    size_t len     = 0;
    bool oid_found = false;

    if ((cert_buffer == NULL) || (oid == NULL) || (value == NULL))
    {
        configPRINTF(("Invalid arguments"));
        return kStatus_Certificates_Failed;
    }

    mbedtls_x509_crt_init(&client_cert);

#if (EVSE_X509_AUTH == 1)
    if (mbedtls_x509_crt_parse(&client_cert, cert_buffer, cert_len))
    {
        cert_status = kStatus_Certificates_Failed;
        configPRINTF((error(" %s Error in loading client certificate"), __FUNCTION__));
        goto exit;
    }
#else
    if (mbedtls_x509_crt_parse_der(&client_cert, cert_buffer, cert_len))
    {
        cert_status = kStatus_Certificates_Failed;
        configPRINTF((error(" %s Error in loading client certificate"), __FUNCTION__));
        goto exit;
    }
#endif
    oid_ptr = &client_cert.subject;

    while (oid_ptr != NULL)
    {
        mbedtls_oid_get_attr_short_name((mbedtls_asn1_buf *)oid_ptr, (const char **)&oid_name);
        if (strcmp(oid_name, oid) == 0)
        {
            len = oid_ptr->val.len;

            if (len > max_size)
            {
                cert_status = kStatus_Certificates_Failed;
                configPRINTF((error(" %s Error in loading client certificate"), __FUNCTION__));
                goto exit;
            }

            for (size_t i = 0; i < len; i++)
            {
                *(value + i) = oid_ptr->val.p[i];
            }

            oid_found = true;
        }

        oid_ptr = oid_ptr->next;
    }

    if (!oid_found)
    {
        cert_status = kStatus_Certificates_Failed;
        configPRINTF((error(" %s Unable to find the OID"), __FUNCTION__));
        goto exit;
    }

exit:
    mbedtls_x509_crt_free(&client_cert);
    return cert_status;
#endif /* ((EVSE_X509_SE050_AUTH == 1) || (EVSE_X509_AUTH == 1)) */
}

#if 0
/*
 * @brief Create the task that initializes Secure Element
 */
void EVSE_Secure_Element_Init(void)
{
    BaseType_t xReturned = pdPASS;

    if (xTaskCreate(EVSE_Secure_Element_Task, "Secure Element", APP_SE05X_STACK_SIZE, NULL, APP_SE05X_PRIORITY, NULL) != pdPASS)
    {
        configPRINTF((error(" %s Failed to create task"), __FUNCTION__));
        xReturned = pdFAIL;
    }

    if (xReturned != pdPASS)
    {
        while(1)
            ;
    }
}
#endif /* 0 */

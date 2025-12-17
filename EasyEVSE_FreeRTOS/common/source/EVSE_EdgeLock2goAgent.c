/*
 * Copyright 2024-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#include "EVSE_ConnectivityConfig.h"
#include "EVSE_Ocpp.h"
#include "EVSE_Secure_Element.h"

#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"

/* EdgeLock 2GO agent includes */
#include "nxp_iot_agent.h"
#include "nxp_iot_agent_keystore_sss_se05x.h"
#include "nxp_iot_agent_datastore_plain.h"
#include "nxp_iot_agent_utils.h"
#include "nxp_iot_agent_session.h"
#include "nxp_iot_agent_macros.h"
#include "nxp_iot_agent_time.h"
#include "iot_agent_mqtt_freertos.h"
#include "se05x_APDU.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_UID_DECIMAL_STRING_SIZE 44U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static iot_agent_status_t agent_start(ex_sss_boot_ctx_t *pCtx);
static void print_status_report(const nxp_iot_UpdateStatusReport *status_report);
static iot_agent_status_t iot_agent_print_uid(sss_se05x_session_t *pSession);
static void iot_agent_print_uid_integer(char *hexString, size_t len);
static bool EVSE_EdgeLock_CheckProvisioned(void);
static bool edgelock_ready = false;

/*******************************************************************************
 * Variables
 ******************************************************************************/

extern ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;
extern ex_sss_cloud_ctx_t gex_sss_demo_tls_ctx;

/*******************************************************************************
 * Code
 ******************************************************************************/

static bool EVSE_EdgeLock_CheckProvisioned(void)
{
    bool provDone = true;
    EVSE_SE05X_status_t status;

#if ((ENABLE_ISO15118 == 1) && (PKCS11_SUPPORTED == 1))
    status = kStatus_CPOKey_Failed;

    EVSE_SE050_Open_Session();
    status = EVSE_SE050_Check_CPOKey();
    EVSE_SE050_Close_Session();

    provDone = (status == kStatus_CPOKey_Success) ? true : false;
#endif /* ENABLE_ISO15118 && PKCS11_SUPPORTED */

    return provDone;
}

bool EVSE_EdgeLock_IsReady()
{
    return edgelock_ready;
}

void EVSE_EdgeLock_RunClient(void *arg)
{
    iot_agent_status_t agent_status = IOT_AGENT_FAILURE;

    if(EVSE_EdgeLock_CheckProvisioned() == false)
    {
        configPRINTF(("Starting EdgeLock2GO agent."));

        /* Start an EdgeLock 2GO agent session */
        agent_status = agent_start(&gex_sss_demo_boot_ctx);
        if (agent_status != IOT_AGENT_SUCCESS)
        {
            configPRINTF(("agent_start() failed with %d", agent_status));
        }
        else
        {
#if ((ENABLE_ISO15118 == 1) && (PKCS11_SUPPORTED == 1))
        EVSE_SE050_Open_Session();
        EVSE_SE050_Handle_CPOKey();
        EVSE_SE050_Close_Session();
#endif /* ENABLE_ISO15118 && PKCS11_SUPPORTED */
            configPRINTF((success("EdgeLock2GO agent started.")));
            edgelock_ready = true;
        }
    }
    else
    {
        configPRINTF((info("CPO key already provisioned")));
        edgelock_ready = true;
    }

    vTaskDelete(NULL);
}

void EVSE_EdgeLock_Init(void)
{
	static bool first_time = false;

	if (first_time == false)
	{
		if (xTaskCreate(EVSE_EdgeLock_RunClient, "EdgeLock2Go", APP_EVSE_EDGELOCK_STACK_SIZE, NULL, APP_EVSE_EDGELOCK_PRIORITY, NULL) != pdPASS)
		{
			configPRINTF((error(" %s Failed to create task "), __FUNCTION__));
			while (1)
				;
		}

		first_time = true;
	}

}
/* Open an IoT agent session */
static iot_agent_status_t agent_start(ex_sss_boot_ctx_t *pCtx)
{
    iot_agent_status_t agent_status       = IOT_AGENT_SUCCESS;
    iot_agent_context_t iot_agent_context = {0};

    /* The datastore holding data to connect to EdgeLock 2GO cloud service.
     * This right now is left empty. This implies that the agent will fall
     * back to the configuration defined at compile time
     * in nxp_iot_agent_config.h. */
    iot_agent_datastore_t edgelock2go_datastore = {0};

    /* The datastore that is to be filled with service descriptors
     * for customer cloud services. */
    iot_agent_datastore_t datastore = {0};

    /* The keystore (it holds credentials for connecting to EdgeLock 2GO
     * cloud service as well as for customer cloud services). */
    iot_agent_keystore_t keystore = {0};

    /* Intializations of local variables */
    size_t number_of_services                = 0U;
    nxp_iot_UpdateStatusReport status_report = nxp_iot_UpdateStatusReport_init_default;

    configPRINTF(("Starting edgelock2go agent session"));

    /* Initialize and open a session using the secure element. */
    agent_status = iot_agent_session_init(0, NULL, pCtx);
    AGENT_SUCCESS_OR_EXIT();

#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE
    agent_status = iot_agent_claimcode_inject(pCtx, IOT_AGENT_CLAIMCODE_STRING, strlen(IOT_AGENT_CLAIMCODE_STRING));
    AGENT_SUCCESS_OR_EXIT_MSG("Injecting claimcode failed");
#endif // IOT_AGENT_CLAIMCODE_INJECT_ENABLE

    /* Initialize contexts */
    agent_status = iot_agent_init(&iot_agent_context);
    AGENT_SUCCESS_OR_EXIT();
    configPRINTF(("Printing uid: "));
    agent_status = iot_agent_print_uid((sss_se05x_session_t *)&pCtx->session);
    AGENT_SUCCESS_OR_EXIT();

    /************* Register keystore *************/

    agent_status = iot_agent_keystore_sss_se05x_init(&keystore, EDGELOCK2GO_KEYSTORE_ID, pCtx, true);
    AGENT_SUCCESS_OR_EXIT();

    agent_status = iot_agent_register_keystore(&iot_agent_context, &keystore);
    AGENT_SUCCESS_OR_EXIT();

    /************* Register datastore *************/

    /* This is the datastore that holds connection information on how to connect to
     * the EdgeLock 2GO cloud service itself. Typically this would be a persistent
     * datastore which supports transactions to allow for atomic updates of
     * the URL/port/etc. If a filesystem is available, here, we use a file-based
     * datastore.
     * Note, that the ID of the datastore is a given. */
    agent_status =
        iot_agent_datastore_plain_init(&edgelock2go_datastore, nxp_iot_DatastoreIdentifiers_DATASTORE_EDGELOCK2GO_ID);
    AGENT_SUCCESS_OR_EXIT();

    iot_agent_status_t my_status = IOT_AGENT_FAILURE;
    /* If the contents of the datastore for the connectin to the EdgeLock 2O datastore
     * are not valid (e.g. on the first boot), fill the datastore with contents
     * from the settings contained in nxp_iot_agent_config.h */
    if (!iot_agent_service_is_configuration_data_valid(&edgelock2go_datastore))
    {
        my_status =
            iot_agent_utils_write_edgelock2go_datastore(&keystore, &edgelock2go_datastore, EDGELOCK2GO_HOSTNAME,
                                                        EDGELOCK2GO_PORT, iot_agent_trusted_root_ca_certificates, NULL);
    }

    configPRINTF(("iot_agent_utils_write_edgelock2go_datastore status is %d", my_status));

    /* For connecting to the EdgeLock 2GO cloud service, we also need to register the
     * datastore that contains the information on how to connect there. */
    agent_status = iot_agent_set_edgelock2go_datastore(&iot_agent_context, &edgelock2go_datastore);
    configPRINTF(("iot_agent_set_edgelock2go_datastore status is %d", agent_status));
    AGENT_SUCCESS_OR_EXIT();

    /* Init and register datastore, no file system */
    agent_status = iot_agent_datastore_plain_init(&datastore, 0U);
    configPRINTF(("iot_agent_datastore_plain_init status is %d", agent_status));
    AGENT_SUCCESS_OR_EXIT();

    agent_status = iot_agent_register_datastore(&iot_agent_context, &datastore);
    AGENT_SUCCESS_OR_EXIT();

#if IOT_AGENT_CLAIMCODE_INJECT_ENABLE
    iot_agent_time.init_time -= iot_agent_time.claimcode_inject_time;
#endif

    /* Update device configuration */
    agent_status = iot_agent_update_device_configuration(&iot_agent_context, &status_report);

    /* We have a status report which can have some details on the operations that happened.
     * This gives the opportunity to programmatically react on errors/failures. For demonstration
     * purpose, print that information to the console here. */
    if ((agent_status == IOT_AGENT_SUCCESS || agent_status == IOT_AGENT_UPDATE_FAILED) && status_report.has_status)
    {
        print_status_report(&status_report);
    }

    configPRINTF(("iot_agent_update_device_configuration status is %d", agent_status));
    AGENT_SUCCESS_OR_EXIT();

    /* Validate services */
    if (!iot_agent_is_service_configuration_data_valid(&iot_agent_context))
    {
        configPRINTF(("Failure: Not all configuration data is valid"));
        goto exit;
    }

    /* Get total number of services */
    number_of_services = iot_agent_get_number_of_services(&iot_agent_context);
    configPRINTF(("Found configuration data for %d services.", (int)number_of_services));

    /* Trigger MQTT connection */
#if (AX_EMBEDDED && defined(USE_RTOS) && USE_RTOS == 1) || (SSS_HAVE_HOSTCRYPTO_OPENSSL)
    agent_status = iot_agent_verify_mqtt_connection(&iot_agent_context);
    AGENT_SUCCESS_OR_EXIT();
#endif

exit:
    /* Close SE session */
    iot_agent_keystore_close_session(&keystore);
    iot_agent_free_update_status_report(&status_report);
    iot_agent_datastore_free(&edgelock2go_datastore);
    iot_agent_datastore_free(&datastore);
    iot_agent_keystore_free(&keystore);
    return agent_status;
}

static void iot_agent_print_uid_integer(char *hexString, size_t len)
{
    char decimalString[MAX_UID_DECIMAL_STRING_SIZE + 1];
    uint32_t weigthValueArray[MAX_UID_DECIMAL_STRING_SIZE]        = {0U};
    uint32_t decimalDigitWeightedSum[MAX_UID_DECIMAL_STRING_SIZE] = {0U};

    // initialize variables used in the calculation
    weigthValueArray[0] = 1U;
    memset(decimalString, '0', MAX_UID_DECIMAL_STRING_SIZE);
    decimalString[MAX_UID_DECIMAL_STRING_SIZE] = '\0';

    // for every loop iteration:
    // - pick the hex char in the string starting from last
    // - compute the the contribution of the actual char to all the weighted sum
    //   for single digits in decimal string
    // - compute the weight values for next calculation
    for (size_t i = 0U; i < len; i++)
    {
        for (uint8_t j = 0U; j < MAX_UID_DECIMAL_STRING_SIZE; j++)
        {
            // pick char
            uint32_t charWeightedContrib = 0U;
            char pickChar                = *(hexString + len - 1U - i);
            if ((pickChar >= '0') && (pickChar <= '9'))
                charWeightedContrib = (uint32_t)(pickChar - '0');
            else if ((pickChar >= 'A') && (pickChar <= 'F'))
                charWeightedContrib = (uint32_t)(pickChar - 'A') + 10U;
            else if ((pickChar >= 'a') && (pickChar <= 'f'))
                charWeightedContrib = (uint32_t)(pickChar - 'a') + 10U;
            charWeightedContrib *= weigthValueArray[j];
            decimalDigitWeightedSum[j] += charWeightedContrib;
        }
        uint32_t rem = 0U;
        for (uint8_t k = 0U; k < MAX_UID_DECIMAL_STRING_SIZE; k++)
        {
            weigthValueArray[k] = rem + (weigthValueArray[k] << 4);
            rem                 = weigthValueArray[k] / 10U;
            weigthValueArray[k] %= 10U;
        }
    }

    // from the decimal digit weighted sum compute the final decimal string
    uint32_t rem = 0U;
    for (uint8_t i = 0U; i < MAX_UID_DECIMAL_STRING_SIZE; i++)
    {
        uint8_t index = MAX_UID_DECIMAL_STRING_SIZE - 1 - i;
        decimalString[index] += ((rem + decimalDigitWeightedSum[i]) % 10U);
        rem = (rem + decimalDigitWeightedSum[i]) / 10U;
    }

    // print without leading 0s
    char *printedString;
    printedString = decimalString;
    while (*printedString && *printedString == '0')
        printedString++;
    configPRINTF(("UID in decimal format: %s", printedString));
}

static iot_agent_status_t iot_agent_print_uid(sss_se05x_session_t *pSession)
{
    iot_agent_status_t agent_status = IOT_AGENT_SUCCESS;
    uint8_t uid[SE050_MODULE_UNIQUE_ID_LEN];
    size_t uidLen = sizeof(uid);
    smStatus_t sm_status;
    SE05x_Result_t result = kSE05x_Result_NA;

    sm_status = Se05x_API_CheckObjectExists(&pSession->s_ctx, (uint32_t)kSE05x_AppletResID_UNIQUE_ID, &result);
    if (SM_OK != sm_status)
    {
        EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "UID not present on the secure element");
    }

    sm_status = Se05x_API_ReadObject(&pSession->s_ctx, (uint32_t)kSE05x_AppletResID_UNIQUE_ID, 0U, (uint16_t)uidLen,
                                     uid, &uidLen);
    if (SM_OK != sm_status)
    {
        EXIT_STATUS_MSG(IOT_AGENT_FAILURE, "Error in reading UID from the device");
    }
    char uidHexString[(SE050_MODULE_UNIQUE_ID_LEN * 2U) + 1U];
    for (uint8_t i = 0U; i < SE050_MODULE_UNIQUE_ID_LEN; i++)
    {
        sprintf((uidHexString + (i * 2)), "%02X", uid[i]);
    }
    configPRINTF(("UID in hex format: %s", uidHexString));
    iot_agent_print_uid_integer(uidHexString, (((size_t)SE050_MODULE_UNIQUE_ID_LEN) * 2U));
exit:
    return agent_status;
}

static const char *update_status_report_description(nxp_iot_UpdateStatusReport_UpdateStatus status)
{
    switch (status)
    {
        case nxp_iot_UpdateStatusReport_UpdateStatus_SUCCESS:
            return "SUCCESS";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_ENCODING:
            return "ERR_ENCODING";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_PROTOCOL:
            return "ERR_PROTOCOL";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_MEMORY_READ:
            return "ERR_MEMORY_READ";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_MEMORY_WRITE:
            return "ERR_MEMORY_WRITE";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_SSS_COMMUNICATION:
            return "ERR_SSS_COMMUNICATION";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_SSS_VERSION:
            return "ERR_SSS_VERSION";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_SSS_SECURE_CHANNEL:
            return "ERR_SSS_SECURE_CHANNEL";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_CONFIGURATION:
            return "ERR_CONFIGURATION";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_CONFIGURATION_TOO_MANY_DATASTORES:
            return "ERR_CONFIGURATION_TOO_MANY_DATASTORES";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_CONFIGURATION_TOO_MANY_KEYSTORES:
            return "ERR_CONFIGURATION_TOO_MANY_KEYSTORES";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_CONFIGURATION_SNI_MISSING:
            return "ERR_CONFIGURATION_SNI_MISSING";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_CONFIGURATION_SNI_INVALID:
            return "ERR_CONFIGURATION_SNI_INVALID";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_CONNECTION_QUOTA_EXCEEDED:
            return "ERR_CONNECTION_QUOTA_EXCEEDED";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_DEVICE_NOT_WHITELISTED:
            return "ERR_DEVICE_NOT_WHITELISTED";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_UPDATE_FAILED:
            return "ERR_UPDATE_FAILED";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_INTERNAL:
            return "ERR_INTERNAL";
        case nxp_iot_UpdateStatusReport_UpdateStatus_ERR_TIMEOUT:
            return "ERR_TIMEOUT";
        default:
            return "UNKNOWN";
    }
}

static const char *claim_status_description(nxp_iot_AgentClaimStatus_ClaimStatus status)
{
    switch (status)
    {
        case nxp_iot_AgentClaimStatus_ClaimStatus_SUCCESS:
            return "SUCCESS";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_NOT_FOUND:
            return "ERR_NOT_FOUND";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_WRONG_PRODUCT_TYPE:
            return "ERR_WRONG_PRODUCT_TYPE";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_CODE_REVOKED:
            return "ERR_CLAIM_CODE_REVOKED";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_CODE_LIMIT_REACHED:
            return "ERR_CLAIM_CODE_LIMIT_REACHED";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_CODE_REUSE_PROHIBITED:
            return "ERR_CLAIM_CODE_REUSE_PROHIBITED";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_CODE_READ:
            return "ERR_CLAIM_CODE_READ";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_CODE_POLICIES:
            return "ERR_CLAIM_CODE_POLICIES";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_CODE_TYPE:
            return "ERR_CLAIM_CODE_TYPE";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_FAILED:
            return "ERR_CLAIM_FAILED";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_CLAIM_CODE_FORMAT:
            return "ERR_CLAIM_CODE_FORMAT";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_TIMEOUT:
            return "ERR_TIMEOUT";
        case nxp_iot_AgentClaimStatus_ClaimStatus_ERR_INTERNAL:
            return "ERR_INTERNAL";
        default:
            return "UNKNOWN";
    }
}

static const char *rtp_status_description(nxp_iot_AgentRtpStatus_RtpStatus status)
{
    switch (status)
    {
        case nxp_iot_AgentRtpStatus_RtpStatus_SUCCESS:
            return "SUCCESS";
        case nxp_iot_AgentRtpStatus_RtpStatus_SUCCESS_NO_CHANGE:
            return "SUCCESS_NO_CHANGE";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_RTP_FAILED:
            return "ERR_RTP_FAILED";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_OBJECT_ATTRIBUTES_READ_FAILED:
            return "ERR_OBJECT_ATTRIBUTES_READ_FAILED";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_OBJECT_DELETE_FAILED:
            return "ERR_OBJECT_DELETE_FAILED";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_OBJECT_WRITE_FAILED:
            return "ERR_OBJECT_WRITE_FAILED";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_DEFECTIVE:
            return "ERR_DEFECTIVE";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_CURVE_INSTALLATION_FAILED:
            return "ERR_CURVE_INSTALLATION_FAILED";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_TIMEOUT:
            return "ERR_TIMEOUT";
        case nxp_iot_AgentRtpStatus_RtpStatus_ERR_INTERNAL:
            return "ERR_INTERNAL";
        default:
            return "UNKNOWN";
    }
}

static const char *csp_status_description(nxp_iot_AgentCspStatus_CspStatus status)
{
    switch (status)
    {
        case nxp_iot_AgentCspStatus_CspStatus_SUCCESS:
            return "SUCCESS";
        case nxp_iot_AgentCspStatus_CspStatus_SUCCESS_NO_CHANGE:
            return "SUCCESS_NO_CHANGE";
        case nxp_iot_AgentCspStatus_CspStatus_SUCCESS_REVOKED:
            return "SUCCESS_REVOKED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_CSP_FAILED:
            return "ERR_CSP_FAILED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_KEY_SLOT_OCCUPIED:
            return "ERR_KEY_SLOT_OCCUPIED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_KEY_GENERATION_FAILED:
            return "ERR_KEY_GENERATION_FAILED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_KEY_READOUT_FAILED:
            return "ERR_KEY_READOUT_FAILED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_MEMORY_READ_FAILED:
            return "ERR_MEMORY_READ_FAILED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_MEMORY_ALLOCATION_FAILED:
            return "ERR_MEMORY_ALLOCATION_FAILED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_SERVICE_DESCRIPTOR_WRITE_FAILED:
            return "ERR_SERVICE_DESCRIPTOR_WRITE_FAILED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_MEMORY_COMMIT_FAILED:
            return "ERR_MEMORY_COMMIT_FAILED";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_DEFECTIVE:
            return "ERR_DEFECTIVE";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_INTERNAL:
            return "ERR_INTERNAL";
        case nxp_iot_AgentCspStatus_CspStatus_ERR_TIMEOUT:
            return "ERR_TIMEOUT";
        default:
            return "UNKNOWN";
    }
}

static void print_status_report(const nxp_iot_UpdateStatusReport *status_report)
{
    configPRINTF(("Update status report:"));
    configPRINTF(
        ("  The device update %s (0x%04x: %s)",
         (status_report->status == nxp_iot_UpdateStatusReport_UpdateStatus_SUCCESS ? "was successful" : "FAILED"),
         status_report->status, update_status_report_description(status_report->status)));
    configPRINTF(("  The correlation-id for this update is %s.", status_report->correlationId));

    if (status_report->has_claimStatus)
    {
        configPRINTF(("  Status for claiming the device: 0x%04x: %s.", status_report->claimStatus.status,
                      claim_status_description(status_report->claimStatus.status)));
        for (size_t i = 0U; i < status_report->claimStatus.details_count; i++)
        {
            nxp_iot_AgentClaimStatus_DetailedClaimStatus *s = &status_report->claimStatus.details[i];
            configPRINTF(("    On endpoint 0x%08lx, status for claiming: 0x%04x: %s.", s->endpointId, s->status,
                          claim_status_description(s->status)));
        }
    }

    if (status_report->has_rtpStatus)
    {
        configPRINTF(("  Status for remote trust provisioning: 0x%04x: %s.", status_report->rtpStatus.status,
                      rtp_status_description(status_report->rtpStatus.status)));
        for (size_t i = 0U; i < status_report->rtpStatus.details_count; i++)
        {
            nxp_iot_AgentRtpStatus_RtpObjectStatus *s = &status_report->rtpStatus.details[i];
            configPRINTF(("    On endpoint 0x%08lx, for object 0x%08lx, status: 0x%04x: %s.", s->endpointId,
                          s->objectId, s->status, rtp_status_description(s->status)));
        }
    }

    if (status_report->has_cspStatus)
    {
        configPRINTF(("  Status for cloud service provisioning: 0x%04x: %s.", status_report->cspStatus.status,
                      csp_status_description(status_report->cspStatus.status)));
        for (size_t i = 0U; i < status_report->cspStatus.details_count; i++)
        {
            nxp_iot_AgentCspStatus_CspServiceStatus *s = &status_report->cspStatus.details[i];
            configPRINTF(("    On endpoint 0x%08lx, for service %lu, status: 0x%04x: %s.", s->endpointId,
                          (uint32_t)s->serviceId, s->status, csp_status_description(s->status)));
        }
    }
}

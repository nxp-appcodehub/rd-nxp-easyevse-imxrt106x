/*
 * Copyright 2023-2025 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#if (ENABLE_CLEV663_NFC == 1)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

/* NFC Frontend includes */
#include "NfcrdlibEx1_DiscoveryLoop.h"
#include <phApp_Init.h>
#include "phalMfDuoX.h"
#include "phOsal.h"
#include "EVSE_NFC.h"
#include "EVSE_UI.h"
#include "EVSE_Utils.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

// EV charging certificate
#define CERTIFICATE_TAG_SIZE          2
#define PUBLIC_KEY_TAG_SIZE           2
#define PUBLIC_KEY_LENGTH             70
#define SIGNATURE_TAG_SIZE            2
#define SIGNATURE_LENGTH              64
#define ECC_PUBLIC_KEY_TAG_SIZE       1
#define KEY_PARAMETER_TAG_SIZE        1
#define LENGTH_BYTE                   1
#define CERTIFICATE_HEADER_SIZE       4
#define CERTIFICATE_START_OFFSET      CERTIFICATE_HEADER_SIZE
#define PK_DATA_OBJ_TAG_SIZE          2
#define PK_DATA_OBJ_LENGTH            1
#define PK_UNCOMP_POINT_REPR_TAG_SIZE 1
#define PK_UNCOMP_POINT_REPR_LENGTH   1
#define RANDOM_MESSAGE_LENGTH         32
#define SHORT_CERTIFICATE_LENGTH      193
#define LONG_CERTIFICATE_LENGTH       199

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static StaticTask_t xNFCTaskBuffer;
static CardDetect NFCDetectCallbacks[NFC_CALLBACKS_NUMBER];

AT_NONCACHEABLE_SECTION_ALIGN(static StackType_t xStackNFC[APP_NFC_STACK_SIZE], 32);

phacDiscLoop_Sw_DataParams_t *pDiscLoop;
phalMfDuoX_Sw_DataParams_t *pMifareDUOX;

/* This is used to save restore Poll Config.
 * If in case application has update/change PollCfg to resolve Tech
 * when Multiple Tech was detected in previous poll cycle
 */
static uint16_t bSavePollTechCfg;
nfc_data_info_t NFC_retrieved_data;

const static nfc_whitelist_info_t white_list[] = {}; /* Add your own NFC card UID in this list if you want it whitelisted */

uint16_t size_whitelist = sizeof(white_list) / sizeof(white_list[0]);

AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t nfc_cert_signature[64], 32);
AT_NONCACHEABLE_SECTION_ALIGN(static uint8_t nfc_cert[160], 32);
static uint16_t nfc_cert_len;
const static uint8_t PubKeyNXPRootCA[] = {0x04, 0x5D, 0x70, 0xC6, 0x8A, 0xDC, 0x4B, 0x0A, 0x6A, 0xE4, 0x5B, 0x17, 0x82,
                                          0xD6, 0xFF, 0xC6, 0x69, 0x6E, 0x5E, 0x54, 0xF4, 0xF7, 0x84, 0x45, 0xE0, 0x49,
                                          0xB5, 0xB6, 0x3D, 0x11, 0xF3, 0xAF, 0xAA, 0x3E, 0x85, 0x1C, 0x22, 0x33, 0x36,
                                          0xA7, 0x26, 0xCE, 0x3D, 0x97, 0xD4, 0xB6, 0xD2, 0x22, 0xDB, 0x87, 0x87, 0x3C,
                                          0x54, 0x1A, 0x60, 0x2F, 0x50, 0x6C, 0x1F, 0xF7, 0xCB, 0x22, 0x72, 0x11, 0xAF};

uint16_t PubKeyNXPRootCA_len = sizeof(PubKeyNXPRootCA) / sizeof(PubKeyNXPRootCA[0]);

bool g_activateNFCRead = false;

/*******************************************************************************
 * Code
 ******************************************************************************/

static void EVSE_NFC_CallbackCall(uint8_t *CardUID, uint8_t SizeUID)
{
    for (uint8_t i = 0; i < NFC_CALLBACKS_NUMBER; i++)
    {
        if (NFCDetectCallbacks[i] != NULL)
        {
            NFCDetectCallbacks[i](CardUID, SizeUID);
        }
    }
}

void NFC_Hardware_Init(void)
{
    phStatus_t status          = PH_ERR_INTERNAL_ERROR;
    phNfcLib_Status_t dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;

#ifdef PH_PLATFORM_HAS_ICFRONTEND
    phNfcLib_AppContext_t AppContext = {0};
#endif /* PH_PLATFORM_HAS_ICFRONTEND */

    /* Initialise hardware and Stack */
    phApp_CPU_Init();

    /* Perform OSAL Initialisation. */
    (void)phOsal_Init();

    status = phbalReg_Init(&sBalParams, sizeof(phbalReg_Type_t));
    CHECK_STATUS(status);

    AppContext.pBalDataparams = &sBalParams;
    dwStatus                  = phNfcLib_SetContext(&AppContext);
    CHECK_NFCLIB_STATUS(dwStatus);
}

static nfc_status_t NFC_Lib_Init(void)
{
    /* Initialize library */
    phNfcLib_Status_t dwStatus;
    phStatus_t status = PH_ERR_INTERNAL_ERROR;

    dwStatus = phNfcLib_Init();
    CHECK_NFCLIB_STATUS(dwStatus);

    if (dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        return kNFCStatus_Fail;
    }

    /* Set the generic pointer */
    pHal        = phNfcLib_GetDataParams(PH_COMP_HAL);
    pDiscLoop   = phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);
    pMifareDUOX = phNfcLib_GetDataParams(PH_COMP_AL_MFDUOX);

    /* Initialize other components that are not initialized by NFCLIB and configure Discovery Loop. */
    status = phApp_Comp_Init(pDiscLoop);
    CHECK_STATUS(status);
    if (status != PH_ERR_SUCCESS)
    {
        return kNFCStatus_Fail;
    }

    /* Configure the IRQ */
    status = phApp_Configure_IRQ();
    CHECK_STATUS(status);
    if (status != PH_ERR_SUCCESS)
    {
        return kNFCStatus_Fail;
    }

    return kNFCStatus_Success;
}

static void NFC_Run_Discovery_Loop(void)
{
    phStatus_t status    = PH_ERR_INTERNAL_ERROR;
    uint16_t wEntryPoint = 0;
    uint8_t configureLPCDcounter = 0;

    /* Start Discovery Loop */
    status = phApp_HALConfigAutoColl();
    CHECK_STATUS(status);

    /* Get Poll Configuration */
    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, &bSavePollTechCfg);
    CHECK_STATUS(status);

    /* Start in poll mode */
    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
    status      = PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED;

    /* Switch off RF field */
    status = phhalHw_FieldOff(pHal);
    CHECK_STATUS(status);

    /* Bool to enable LPCD feature. */
    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, PH_ON);
    CHECK_STATUS(status);

    status = phApp_ConfigureLPCD();
    CHECK_STATUS(status);

    while (1)
    {
        configureLPCDcounter++;
#ifdef PH_EXAMPLE1_LPCD_ENABLE
#ifdef NXPBUILD__PHHAL_HW_RC663
        if ((wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL) && (configureLPCDcounter == LPCD_CONFIGURE_INTERVAL))
#else
        /* Configure LPCD */
        if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
#endif /* NXPBUILD__PHHAL_HW_RC663 */
        {
                status = phApp_ConfigureLPCD();
                CHECK_STATUS(status);
                configureLPCDcounter = 0;
        }

        if (EVSE_NFC_getNFCActivationStatus())
        {
            phhalHw_Rc663_SetIRQStatus(true);

            /* Bool to enable LPCD feature. */
            status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, PH_ON);
            CHECK_STATUS(status);
#endif      /* PH_EXAMPLE1_LPCD_ENABLE */

            phStatus_t statustmp   = 0;
            uint16_t wTagsDetected = 0;
            uint16_t wNumberOfTags = 0;
            uint8_t bIndex         = 0;
            uint16_t wValue        = 0;

            /* Set Discovery Poll State to Detection */
            statustmp = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,
                                               PHAC_DISCLOOP_POLL_STATE_DETECTION);
            CHECK_STATUS(statustmp);

            statustmp = phalMfDuoX_SetConfig(pMifareDUOX, PHAL_MFDUOX_AUTH_STATE, PHAL_MFDUOX_NOT_AUTHENTICATED);
            CHECK_STATUS(statustmp);

            /* Start Discovery Loop */
            status = phacDiscLoop_Run(pDiscLoop, wEntryPoint);
            if (wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL)
            {
                if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_TECH_DETECTED)
                {
                    configPRINTF((" \n\r Multiple technology detected: \n\r"));

                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                    CHECK_STATUS(status);

                    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_A))
                    {
                        configPRINTF((" \tType A detected... \n\r"));
                    }
                    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_B))
                    {
                        configPRINTF((" \tType B detected... \n\r"));
                    }
                    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F212))
                    {
                        configPRINTF((" \tType F detected with baud rate 212... \n\r"));
                    }
                    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_F424))
                    {
                        configPRINTF((" \tType F detected with baud rate 424... \n\r"));
                    }
                    if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, PHAC_DISCLOOP_POS_BIT_MASK_V))
                    {
                        configPRINTF((" \tType V / ISO 15693 / T5T detected... \n\r"));
                    }

                    /* Select 1st Detected Technology to Resolve*/
                    for (bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                    {
                        if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
                        {
                            /* Configure for one of the detected technology */
                            status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG,
                                                            (1 << bIndex));
                            CHECK_STATUS(status);
                            break;
                        }
                    }

                    /* Print the technology resolved */
                    phApp_PrintTech((1 << bIndex));

                    /* Set Discovery Poll State to collision resolution */
                    status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE,
                                                    PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION);
                    CHECK_STATUS(status);

                    /* Restart discovery loop in poll mode from collision resolution phase */
                    status = phacDiscLoop_Run(pDiscLoop, wEntryPoint);
                }

                if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
                {
                    /* Get Detected Technology Type */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                    CHECK_STATUS(status);

                    /* Get number of tags detected */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                    CHECK_STATUS(status);

                    configPRINTF((" \n\r Multiple cards resolved: %d cards \n\r", wNumberOfTags));
                    phApp_PrintTagInfo(pDiscLoop, wNumberOfTags, wTagsDetected);

                    if (wNumberOfTags > 1)
                    {
                        /* Get 1st Detected Tag and Activate device at index 0 */
                        for (bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                        {
                            if (PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
                            {
                                DEBUG_PRINTF("\t Activating one card...\n\r");
                                status = phacDiscLoop_ActivateCard(pDiscLoop, bIndex, 0);
                                break;
                            }
                        }

                        if (((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED) ||
                            ((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED) ||
                            ((status & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND))
                        {
                            /* Get Detected Technology Type */
                            status =
                                phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                            CHECK_STATUS(status);

                            phApp_PrintTagInfo(pDiscLoop, 0x01, wTagsDetected);
                        }
                        else
                        {
                            configPRINTF((error("\t\tCard activation failed...\n\r")));
                        }
                    }

                    /* Switch to LISTEN mode after POLL mode */
                }
                else if (((status & PH_ERR_MASK) == PHAC_DISCLOOP_NO_TECH_DETECTED) ||
                         ((status & PH_ERR_MASK) == PHAC_DISCLOOP_NO_DEVICE_RESOLVED))
                {
                    /* Switch to LISTEN mode after POLL mode */
                    continue;
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFON)
                {
                    /*
                     * If external RF is detected during POLL, return back so that the application
                     * can restart the loop in LISTEN mode
                     */
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_MERGED_SEL_RES_FOUND)
                {
                    configPRINTF((" \n\r Device having T4T and NFC-DEP support detected... \n\r"));
                    /* Get Detected Technology Type */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                    CHECK_STATUS(status);

                    phApp_PrintTagInfo(pDiscLoop, 1, wTagsDetected);

                    /* Switch to LISTEN mode after POLL mode */
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_DEVICE_ACTIVATED)
                {
                    configPRINTF((success("Card detected and activated successfully... ")));
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                    CHECK_STATUS(status);

                    /* Get Detected Technology Type */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                    CHECK_STATUS(status);

                    phApp_PrintTagInfo(pDiscLoop, wNumberOfTags, wTagsDetected);
                    configPRINTF((info(" Card UID: %s"), NFC_retrieved_data.CardUID));
                    configPRINTF((info(" Card Tek: %s"), NFC_retrieved_data.CardTek));
                    configPRINTF((info(" Card Type: %s"), NFC_retrieved_data.CardType));
                    configPRINTF((info(" UID Size: %d"), NFC_retrieved_data.SizeUID));

                    NFC_retrieved_data.isSecured     = CARD_ACCEPTED;
                    NFC_retrieved_data.ExactCardType = CARD_GENERIC;

                    if (memcmp(NFC_retrieved_data.CardType, "4A", sizeof("4A")) == 0)
                    {
                        phStatus_t DUOX_status = PH_ERR_INTERNAL_ERROR;
                        uint8_t *ppVersion;
                        uint16_t pVerLen;
                        DUOX_status = phalMfDuoX_GetVersion(pMifareDUOX, PHAL_MFDUOX_GET_VERSION_EXCLUDE_FAB_ID,
                                                            &ppVersion, &pVerLen);
                        if (DUOX_status == PH_ERR_SUCCESS)
                        {
                            if ((ppVersion[1] == 0x01) &&
                                (ppVersion[3] == 0xA0)) // if this condition is met, the scanned card is MIFARE DUOX
                            {
                                uint8_t randomMessage[RANDOM_MESSAGE_LENGTH] = {0};
                                uint8_t *ppResponse;
                                uint16_t pRspLen;
                                uint8_t *ppResponseSign;
                                uint16_t pRspLenSign;

                                uint8_t *ppFCI      = NULL;
                                uint16_t pFCILen    = 0;
                                uint8_t pDFName[16] = {0xA0, 0x00, 0x00, 0x08, 0x45, 0x00, 0x00, 0x00,
                                                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};

                                NFC_retrieved_data.ExactCardType = CARD_MIFARE_DUOX;
                                NFC_retrieved_data.isSecured     = CARD_NOT_SECURED;

                                DUOX_status = phalMfDuoX_IsoSelectFile(
                                    pMifareDUOX, PHAL_MFDUOX_FCI_RETURNED, PHAL_MFDUOX_SELECTOR_4, NULL, pDFName,
                                    sizeof(pDFName), PHAL_MFDUOX_APDU_FORMAT_SHORT_LEN, &ppFCI, &pFCILen);
                                if (DUOX_status != PH_ERR_SUCCESS)
                                {
                                    NFC_retrieved_data.isSecured = EMPTY_CARD;
                                    configPRINTF((error("File selection failed with error code %x"), DUOX_status));
                                }

                                if (DUOX_status == PH_ERR_SUCCESS)
                                {
                                    DUOX_status =
                                        phalMfDuoX_VdeReadData(pMifareDUOX, PH_EXCHANGE_DEFAULT, 0x00U, 0x0000U,
                                                               PHAL_MFDUOX_APDU_FORMAT_SHORT_LEN,
                                                               &ppResponse, /* certificate that needs to be verified */
                                                               &pRspLen);
                                    if (DUOX_status != PH_ERR_SUCCESS)
                                    {
                                        NFC_retrieved_data.isSecured = READ_FAIL;
                                        configPRINTF((error("Reading data failed with error code %x"), DUOX_status));
                                    }
                                    else
                                    {
                                        if ((pRspLen == SHORT_CERTIFICATE_LENGTH) ||
                                            (pRspLen == LONG_CERTIFICATE_LENGTH))
                                        {
                                            nfc_cert_len = pRspLen - CERTIFICATE_HEADER_SIZE - SIGNATURE_LENGTH -
                                                           SIGNATURE_TAG_SIZE - LENGTH_BYTE;
                                            /* Retrieve the certificate and certificate signature from the response data
                                             */
                                            memcpy(nfc_cert, &ppResponse[CERTIFICATE_START_OFFSET], nfc_cert_len);
                                            memcpy(nfc_cert_signature, &ppResponse[pRspLen - SIGNATURE_LENGTH],
                                                   SIGNATURE_LENGTH);
                                        }
                                        else
                                        {
                                            DUOX_status = PH_ERR_PARAMETER_SIZE;
                                        }
                                    }
                                }

                                if (DUOX_status == PH_ERR_SUCCESS)
                                {
                                    DUOX_status = phCryptoASym_InvalidateKey(pMifareDUOX->pCryptoDataParamsASym);
                                    if (DUOX_status != PH_ERR_SUCCESS)
                                    {
                                        configPRINTF(
                                            (error("Key invalidation failed with error code %x"), DUOX_status));
                                    }
                                }

                                if (DUOX_status == PH_ERR_SUCCESS)
                                {
                                    uint8_t *publicKey;
                                    uint8_t publickeyLength;
                                    uint8_t publicKeyOffset = pRspLen - SIGNATURE_LENGTH - LENGTH_BYTE -
                                                              SIGNATURE_TAG_SIZE - PUBLIC_KEY_LENGTH - LENGTH_BYTE -
                                                              PUBLIC_KEY_TAG_SIZE;

                                    if (ppResponse[publicKeyOffset] == 0x7F && ppResponse[publicKeyOffset + 1] == 0x49)
                                    {
                                        publicKeyOffset +=
                                            (PK_DATA_OBJ_TAG_SIZE + PK_DATA_OBJ_LENGTH);  // skip 0x7F49 tag and Length
                                        publicKeyOffset += (PK_UNCOMP_POINT_REPR_TAG_SIZE +
                                                            PK_UNCOMP_POINT_REPR_LENGTH); // skip 0xB0 tag and Length;
                                    }

                                    DUOX_status = phCryptoASym_ECC_LoadKeyDirect(
                                        pMifareDUOX->pCryptoDataParamsASym,
                                        PH_CRYPTOASYM_PUBLIC_KEY | PH_CRYPTOASYM_CURVE_ID_BRAINPOOL256R1,
                                        &ppResponse[publicKeyOffset], 0x41);

                                    if (DUOX_status != PH_ERR_SUCCESS)
                                    {
                                        configPRINTF((error("Key loading failed with error code %x"), DUOX_status));
                                    }
                                }

                                if (DUOX_status == PH_ERR_SUCCESS)
                                {
                                    /* Generate random message to be signed. */
                                    for (uint8_t i = 0; i < RANDOM_MESSAGE_LENGTH; i++)
                                    {
                                        randomMessage[i] = EVSE_Random() % 0xFF;
                                    }

                                    DUOX_status = phalMfDuoX_VdeECDSASign(
                                        pMifareDUOX, 0x0000U, PHAL_MFDUOX_APDU_FORMAT_SHORT_LEN, randomMessage,
                                        sizeof(randomMessage), &ppResponseSign, &pRspLenSign);

                                    if (DUOX_status != PH_ERR_SUCCESS)
                                    {
                                        NFC_retrieved_data.isSecured = SIGN_FAIL;
                                        configPRINTF((error("ECDSA signing failed with error code %x"), DUOX_status));
                                    }
                                }

                                if (DUOX_status == PH_ERR_SUCCESS)
                                {
                                    configPRINTF(("Start doing ECC verify for challenge"));

                                    /* Sign Transaction MAC Input. */
                                    DUOX_status =
                                        phCryptoASym_ECC_Verify(pMifareDUOX->pCryptoDataParamsASym, PH_EXCHANGE_DEFAULT,
                                                                PH_CRYPTOASYM_HASH_ALGO_SHA256, randomMessage,
                                                                sizeof(randomMessage), ppResponseSign, pRspLenSign - 2);
                                    if (DUOX_status != PH_ERR_SUCCESS)
                                    {
                                        NFC_retrieved_data.isSecured = CHALLENGE_VERIFY_FAIL;
                                        configPRINTF(
                                            (error("ECDSA verification failed with error code %x"), DUOX_status));
                                    }
                                }

                                if (DUOX_status == PH_ERR_SUCCESS)
                                {
                                    DUOX_status = phCryptoASym_InvalidateKey(pMifareDUOX->pCryptoDataParamsASym);
                                    DUOX_status = phCryptoASym_ECC_LoadKeyDirect(
                                        pMifareDUOX->pCryptoDataParamsASym,
                                        PH_CRYPTOASYM_PUBLIC_KEY | PH_CRYPTOASYM_CURVE_ID_BRAINPOOL256R1,
                                        PubKeyNXPRootCA, PubKeyNXPRootCA_len);
                                    if (DUOX_status != PH_ERR_SUCCESS)
                                    {
                                        NFC_retrieved_data.isSecured = PARSE_FAIL;
                                        configPRINTF(
                                            (error("Key loading failed for certificate signature with error code %x"),
                                             DUOX_status));
                                    }
                                    else
                                    {
                                        configPRINTF(("Start doing ECC verify for MIFARE DUOX certificate"));
                                        DUOX_status = phCryptoASym_ECC_Verify(
                                            pMifareDUOX->pCryptoDataParamsASym, PH_EXCHANGE_DEFAULT,
                                            PH_CRYPTOASYM_HASH_ALGO_SHA256, nfc_cert, nfc_cert_len, nfc_cert_signature,
                                            sizeof(nfc_cert_signature));
                                        if (DUOX_status != PH_ERR_SUCCESS)
                                        {
                                            NFC_retrieved_data.isSecured = CERT_VERIFY_FAIL;
                                            configPRINTF(
                                                (error("Failed to verify certificate for the card %x"), DUOX_status));
                                        }
                                    }
                                }

                                if (DUOX_status == PH_ERR_SUCCESS)
                                {
                                    NFC_retrieved_data.isSecured = CARD_ACCEPTED;
                                    configPRINTF(("Card verification successful"));
                                }
                            }
                            else if ((ppVersion[1] == 0x08) &&
                                     (ppVersion[3] ==
                                      0x30)) // if this condition is met, the scanned card is MIFARE DESFire Light
                            {
                                NFC_retrieved_data.ExactCardType = CARD_DESFIRE_LIGTH;
                            }
                            else if ((ppVersion[1] == 0x01) &&
                                     (ppVersion[3] ==
                                      0x33)) // if this condition is met, the scanned card is MIFARE DESFire EV3
                            {
                                NFC_retrieved_data.ExactCardType = CARD_DESFIRE_EV3;
                            }
                            else if ((ppVersion[1] == 0x01) &&
                                     (ppVersion[3] ==
                                      0x01)) // if this condition is met, the scanned card is MIFARE DESFire EV1
                            {
                                NFC_retrieved_data.ExactCardType = CARD_DESFIRE_EV1;
                            }
                            else // for unsupported ISO14443-4 cards
                            {
                                NFC_retrieved_data.ExactCardType = CARD_GENERIC;
                            }
                        }
                    }
                    else if (memcmp(NFC_retrieved_data.CardType, "2", sizeof("2")) == 0)
                    {
                        NFC_retrieved_data.ExactCardType = CARD_MIFARE_ULTRALIGHT;
                    }

                    if (memcmp(NFC_retrieved_data.isSecured, CARD_ACCEPTED, sizeof(CARD_ACCEPTED)) == 0)
                    {
                        EVSE_NFC_CallbackCall(NFC_retrieved_data.CardUID, NFC_retrieved_data.SizeUID);
                    }
#if (ENABLE_LCD)
                    EVSE_UI_SetEvent(EVSE_UI_NFC);
#endif
                    /* Switch to LISTEN mode after POLL mode */
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVE_TARGET_ACTIVATED)
                {
                    configPRINTF((success(" \n\r Active target detected... \n\r")));
                    /* Switch to LISTEN mode after POLL mode */
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_PASSIVE_TARGET_ACTIVATED)
                {
                    configPRINTF((success(" \n\r Passive target detected... \n\r")));

                    /* Get Detected Technology Type */
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                    CHECK_STATUS(status);

                    phApp_PrintTagInfo(pDiscLoop, 1, wTagsDetected);

                    /* Switch to LISTEN mode after POLL mode */
                }
                else if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
                {
                    /* LPCD has succeeded but no tag was detected. */
                    continue;
                }
                else
                {
                    if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                    {
                        configPRINTF((info("Card not detected. Card technology may be unsupported.")));
                        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                        CHECK_STATUS(status);
                        DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                    }
                    else
                    {
                        DEBUG_ERROR_PRINT(PrintErrorInfo(status));
                    }
                }
                /* We don't put RC663 frontend in LISTEN mode*/
                // wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
            }
            else
            {
                if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_EXTERNAL_RFOFF)
                {
                    /*
                     * Enters here if in the target/card mode and external RF is not available
                     * Wait for LISTEN timeout till an external RF is detected.
                     * Application may choose to go into standby at this point.
                     */
                    status = phhalHw_EventConsume(pHal);
                    CHECK_STATUS(status);

                    status = phhalHw_SetConfig(pHal, PHHAL_HW_CONFIG_RFON_INTERRUPT, PH_ON);
                    CHECK_STATUS(status);

                    status = phhalHw_EventWait(pHal, LISTEN_PHASE_TIME_MS);
                    if ((status & PH_ERR_MASK) == PH_ERR_IO_TIMEOUT)
                    {
                        wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
                    }
                    else
                    {
                        wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
                    }
                }
                else
                {
                    if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_ACTIVATED_BY_PEER)
                    {
                        configPRINTF((" \n\r Device activated in listen mode... \n\r"));
                    }
                    else if ((status & PH_ERR_MASK) == PH_ERR_INVALID_PARAMETER)
                    {
                        /* In case of RC663 Frontend usage, then listen mode is not supported.
                         * Switch from listen mode to poll mode. */
                    }
                    else
                    {
                        if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                        {
                            status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                            CHECK_STATUS(status);
                            DEBUG_ERROR_PRINT(PrintErrorInfo(wValue));
                        }
                        else
                        {
                            DEBUG_ERROR_PRINT(PrintErrorInfo(status));
                        }
                    }

                    /* On successful activation by Peer, switch to LISTEN mode */
                    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
                }
            }
            if (memcmp(NFC_retrieved_data.isSecured, CARD_ACCEPTED, sizeof(CARD_ACCEPTED)) == 0)
            {
                EVSE_NFC_setNFCActivationStatus(false);
            }

            /* Set Poll Configuration */
            status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, bSavePollTechCfg);
            CHECK_STATUS(status);

            /* Switch off RF field */
            status = phhalHw_FieldOff(pHal);
            CHECK_STATUS(status);

            /* Wait for field-off time-out */
            status = phhalHw_Wait(pHal, PHHAL_HW_TIME_MICROSECONDS, 5100);
            CHECK_STATUS(status);
        }
        phOsal_ThreadDelay(pdMS_TO_TICKS(500));
    }
}

void EVSE_NFC_RegisterCallbackFunction(CardDetect cb)
{
    if (cb == NULL)
        return;

    for (uint8_t i = 0; i < NFC_CALLBACKS_NUMBER; i++)
    {
        if (NFCDetectCallbacks[i] == NULL)
        {
            NFCDetectCallbacks[i] = cb;
            break;
        }
    }
}

const char *EVSE_NFC_Get_VehicleID()
{
    return NFC_retrieved_data.CardUID;
}

const char *EVSE_NFC_Get_CardType()
{
    return NFC_retrieved_data.ExactCardType;
}

const char *EVSE_NFC_Get_CardSecurityStatus()
{
    return NFC_retrieved_data.isSecured;
}

bool EVSE_NFC_getNFCActivationStatus()
{
    return g_activateNFCRead;
}

void EVSE_NFC_setNFCActivationStatus(bool activateNFC)
{
    if(activateNFC != g_activateNFCRead)
    {
        if (activateNFC == false)
        {
            phApp_TriggerIRQ(true);
            NVIC_SetPendingIRQ(BOARD_NFC_IRQn);
        }
        else
        {
            configPRINTF((info("NFC activated, please tap card...")));
        }
        g_activateNFCRead = activateNFC;
        EVSE_UI_SetEvent(EVSE_UI_NFCActivation);
    }
}

static void EVSE_NFC_Task(void *args)
{
    configPRINTF((info("Starting NFC Reader in Polling Mode...\r\n")));

    NFC_Hardware_Init();

    if (NFC_Lib_Init() != kNFCStatus_Success)
    {
        goto exit;
    }

    NFC_Run_Discovery_Loop();

exit:
    /* Shouldn't reach here, but to be sure */
    vTaskDelete(NULL);
}

void EVSE_NFC_SetVehicleID(uint8_t *cardUID, uint8_t size)
{
    if (cardUID != NULL)
    {
        if (size > CARD_MAX_SIZE_UID)
        {
            /* crop to fit the buffer */
            size = CARD_MAX_SIZE_UID;
        }

        NFC_retrieved_data.SizeUID = size;
        memcpy(NFC_retrieved_data.CardUID, cardUID, NFC_retrieved_data.SizeUID);
        EVSE_NFC_CallbackCall(NFC_retrieved_data.CardUID, NFC_retrieved_data.SizeUID);
        EVSE_UI_SetEvent(EVSE_UI_NFC);
    }
}

void EVSE_NFC_ClearVehicleID()
{
    memset(NFC_retrieved_data.CardUID, 0, CARD_MAX_SIZE_UID);
    NFC_retrieved_data.SizeUID = sizeof(CARD_NOT_PRESENT);
    memcpy(NFC_retrieved_data.CardUID, CARD_NOT_PRESENT, NFC_retrieved_data.SizeUID);
    NFC_retrieved_data.ExactCardType = CARD_NOT_PRESENT;
    NFC_retrieved_data.isSecured     = CARD_NOT_PRESENT;

    EVSE_UI_SetEvent(EVSE_UI_NFC);
    // EVSE_NFC_CallbackCall(NULL, 0);
}

bool EVSE_NFC_isWhitelisted(bool *auth_result)
{
    if(auth_result == NULL)
    {
        return true;
    }

    *auth_result = false;

    /* Authorize charging if the user has no card to whitelist although NFC is enabled*/
    if(size_whitelist == 0)
    {
        *auth_result = true;
        return true;
    }

    for (int i = 0; i < size_whitelist; i++)
    {
        if ((strcmp(NFC_retrieved_data.CardUID, white_list[i].UID) == 0)
            && (strcmp(NFC_retrieved_data.ExactCardType, white_list[i].ExactType) == 0))
        {
            configPRINTF(("Card is whitelisted"));
            *auth_result = true;
            break;
        }
    }

    return true;
}

void EVSE_NFC_Init(void)
{
    NFC_retrieved_data.SizeUID = sizeof(CARD_NOT_PRESENT);
    memcpy(NFC_retrieved_data.CardUID, CARD_NOT_PRESENT, NFC_retrieved_data.SizeUID);

    if (xTaskCreateStatic(EVSE_NFC_Task, "NFC", APP_NFC_STACK_SIZE, NULL, APP_NFC_PRIORITY, xStackNFC,
                          &xNFCTaskBuffer) == NULL)
    {
        configPRINTF((error("Failed to create NFC task")));
        while (1)
            ;
    }
}
#endif /* (ENABLE_CLEV663_NFC == 1) */

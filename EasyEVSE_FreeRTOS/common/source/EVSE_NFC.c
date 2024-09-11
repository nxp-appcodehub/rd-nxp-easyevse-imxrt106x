/*
 * Copyright 2023-2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to comply
 * with and are bound by, such license terms. If you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 */

#if (CLEV663_ENABLE == 1)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"

/* NFC Frontend includes */
#include "NfcrdlibEx1_BasicDiscoveryLoop.h"
#include <phApp_Init.h>
#include "phOsal.h"
#include "EVSE_NFC.h"
#include "EVSE_UI.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
static StaticTask_t xNFCTaskBuffer;
static CardDetect NFCDetectCallbacks[NFC_CALLBACKS_NUMBER];

AT_NONCACHEABLE_SECTION_ALIGN(static StackType_t xStackNFC[APP_NFC_STACK_SIZE], 32);

phacDiscLoop_Sw_DataParams_t       *pDiscLoop;

/* This is used to save restore Poll Config.
 * If in case application has update/change PollCfg to resolve Tech
 * when Multiple Tech was detected in previous poll cycle
 */
static uint16_t bSavePollTechCfg;
nfc_data_info_t NFC_retrieved_data;
/*******************************************************************************
 * Code
 ******************************************************************************/

static void EVSE_NFC_CallbackCall(uint8_t *CardUID, uint8_t SizeUID)
{
    for (uint8_t i = 0; i < NFC_CALLBACKS_NUMBER ; i++)
    {
        if (NFCDetectCallbacks[i] != NULL)
        {
            NFCDetectCallbacks[i](CardUID, SizeUID);
        }
    }
}

void NFC_Hardware_Init(void)
{
    phStatus_t          status   = PH_ERR_INTERNAL_ERROR;
    phNfcLib_Status_t   dwStatus = PH_NFCLIB_STATUS_INVALID_STATE;

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
    dwStatus = phNfcLib_SetContext(&AppContext);
    CHECK_NFCLIB_STATUS(dwStatus);
}

static nfc_status_t NFC_Lib_Init(void)
{
    /* Initialize library */
    phNfcLib_Status_t   dwStatus;
    phStatus_t          status = PH_ERR_INTERNAL_ERROR;

    dwStatus = phNfcLib_Init();
    CHECK_NFCLIB_STATUS(dwStatus);

    if(dwStatus != PH_NFCLIB_STATUS_SUCCESS)
    {
        return kNFCStatus_Fail;
    }

    /* Set the generic pointer */
    pHal = phNfcLib_GetDataParams(PH_COMP_HAL);
    pDiscLoop = phNfcLib_GetDataParams(PH_COMP_AC_DISCLOOP);

    /* Initialize other components that are not initialized by NFCLIB and configure Discovery Loop. */
    status = phApp_Comp_Init(pDiscLoop);
    CHECK_STATUS(status);
    if(status != PH_ERR_SUCCESS)
    {
        return kNFCStatus_Fail;
    }

    /* Configure the IRQ */
    status = phApp_Configure_IRQ();
    CHECK_STATUS(status);
    if(status != PH_ERR_SUCCESS)
    {
        return kNFCStatus_Fail;
    }

    return kNFCStatus_Success;
}

static void NFC_Run_Discovery_Loop(void)
{
    phStatus_t status    = PH_ERR_INTERNAL_ERROR;
    uint16_t wEntryPoint = 0;

    /* Start Discovery Loop */
    status = phApp_HALConfigAutoColl();
    CHECK_STATUS(status);

    /* Get Poll Configuration */
    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, &bSavePollTechCfg);
    CHECK_STATUS(status);

    /* Start in poll mode */
    wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_POLL;
    status = PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED;

    while(1)
    {
        phStatus_t statustmp   = 0;
        uint16_t wTagsDetected = 0;
        uint16_t wNumberOfTags = 0;
        uint8_t bIndex         = 0;
        uint16_t wValue        = 0;

        /* Switch off RF field */
        statustmp = phhalHw_FieldOff(pHal);
        CHECK_STATUS(statustmp);

        /* Set Discovery Poll State to Detection */
        statustmp = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_DETECTION);
        CHECK_STATUS(statustmp);

        /* Set Poll Configuration */
        statustmp = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, bSavePollTechCfg);
        CHECK_STATUS(statustmp);

#ifdef PH_EXAMPLE1_LPCD_ENABLE

#ifdef NXPBUILD__PHHAL_HW_RC663

        if (wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL)
#else
        /* Configure LPCD */
        if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_LPCD_NO_TECH_DETECTED)
#endif
        {
            status = phApp_ConfigureLPCD();
            CHECK_STATUS(status);
        }

        /* Bool to enable LPCD feature. */
        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ENABLE_LPCD, PH_ON);
        CHECK_STATUS(status);
#endif /* PH_EXAMPLE1_LPCD_ENABLE */

        /* Start Discovery Loop */
        status = phacDiscLoop_Run(pDiscLoop, wEntryPoint);

        if (wEntryPoint == PHAC_DISCLOOP_ENTRY_POINT_POLL)
        {
            if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_TECH_DETECTED)
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
                        status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_PAS_POLL_TECH_CFG, (1 << bIndex));
                        CHECK_STATUS(status);
                        break;
                    }
                }

                /* Print the technology resolved */
                phApp_PrintTech((1 << bIndex));

                /* Set Discovery Poll State to collision resolution */
                status = phacDiscLoop_SetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NEXT_POLL_STATE, PHAC_DISCLOOP_POLL_STATE_COLLISION_RESOLUTION);
                CHECK_STATUS(status);

                /* Restart discovery loop in poll mode from collision resolution phase */
                status = phacDiscLoop_Run(pDiscLoop, wEntryPoint);
            }

            if((status & PH_ERR_MASK) == PHAC_DISCLOOP_MULTI_DEVICES_RESOLVED)
            {
                /* Get Detected Technology Type */
                status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
                CHECK_STATUS(status);

                /* Get number of tags detected */
                status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_NR_TAGS_FOUND, &wNumberOfTags);
                CHECK_STATUS(status);

                configPRINTF((" \n\r Multiple cards resolved: %d cards \n\r",wNumberOfTags));
                phApp_PrintTagInfo(pDiscLoop, wNumberOfTags, wTagsDetected);

                if(wNumberOfTags > 1)
                {
                    /* Get 1st Detected Tag and Activate device at index 0 */
                    for(bIndex = 0; bIndex < PHAC_DISCLOOP_PASS_POLL_MAX_TECHS_SUPPORTED; bIndex++)
                    {
                        if(PHAC_DISCLOOP_CHECK_ANDMASK(wTagsDetected, (1 << bIndex)))
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
                        status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_TECH_DETECTED, &wTagsDetected);
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
                configPRINTF((info("Card Type: %s"), NFC_retrieved_data.CardType));
                configPRINTF((info(" UID Size: %d"), NFC_retrieved_data.SizeUID));

                EVSE_NFC_CallbackCall(NFC_retrieved_data.CardUID, NFC_retrieved_data.SizeUID);
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
            }
            else
            {
                if ((status & PH_ERR_MASK) == PHAC_DISCLOOP_FAILURE)
                {
                    configPRINTF((error("Discovery Loop failed. Card technology may be unsupported.")));
                    status = phacDiscLoop_GetConfig(pDiscLoop, PHAC_DISCLOOP_CONFIG_ADDITIONAL_INFO, &wValue);
                    CHECK_STATUS(status);
                    DEBUG_ERROR_PRINT(PrintErrorInfo(wValue))
                }
                else
                {
                    DEBUG_ERROR_PRINT(PrintErrorInfo(status));
                }
            }
            /* Update the Entry point to LISTEN mode. */
            wEntryPoint = PHAC_DISCLOOP_ENTRY_POINT_LISTEN;
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
        phOsal_ThreadDelay(pdMS_TO_TICKS(500));
    }
}

void EVSE_NFC_RegisterCallbackFunction(CardDetect cb)
{
    if (cb == NULL)
        return;

    for (uint8_t i = 0; i < NFC_CALLBACKS_NUMBER ; i++)
    {
        if (NFCDetectCallbacks[i] == NULL)
        {
            NFCDetectCallbacks[i] = cb;
            break;
        }
    }
}

const char* EVSE_NFC_Get_VehicleID()
{
	return NFC_retrieved_data.CardUID;
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


    EVSE_UI_SetEvent(EVSE_UI_NFC);
    //EVSE_NFC_CallbackCall(NULL, 0);
}

void EVSE_NFC_Init(void)
{
    NFC_retrieved_data.SizeUID = sizeof(CARD_NOT_PRESENT);
    memcpy(NFC_retrieved_data.CardUID, CARD_NOT_PRESENT, NFC_retrieved_data.SizeUID);

    if (xTaskCreateStatic(EVSE_NFC_Task, "NFC", APP_NFC_STACK_SIZE, NULL, APP_NFC_PRIORITY , xStackNFC, &xNFCTaskBuffer) == NULL)
    {
        configPRINTF((error("Failed to create NFC task")));
        while (1);
    }
}
#endif /* (CLEV663_ENABLE == 1) */

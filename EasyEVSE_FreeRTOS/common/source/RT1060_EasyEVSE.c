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

#include "EVSE_ConnectivityConfig.h"
#include <EVSE_ConfigChecks.h>

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_component_serial_manager.h"

#include "iot_logging_task.h"
#include "ksdk_mbedtls.h"

#include "flash_littlefs.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

extern void vApplicationTickHookRegister(void);

extern void EVSE_Connectivity_Init(void);
extern void EVSE_UI_Init(void);
extern void EVSE_Secure_Element_Init(void);
extern void EVSE_NFC_Init(void);
extern void EVSE_Meter_Init(void);
extern void EVSE_ChargingProtocol_Init(void);
extern void EVSE_Shell_Init(void);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Execute first run through kernel timer service
 *
 * Called from the daemon task exactly once on first execution.  Any application
 * initialization code that needs the RTOS to be running can be placed in this
 * hook function.
 *
 */
void vApplicationDaemonTaskStartupHook(void)
{
    PRINTF("Start the EasyEVSE MCU application based on FreeRTOS...\r\n");

    /* Init Crypto blocks (e.g. DCP) */
    CRYPTO_InitHardware();

    FLASH_LITTLEFS_Init(false);

    /* Register os hooks */
    vApplicationTickHookRegister();

    /* Init logging task */
    xLoggingTaskInitialize(LOGGING_TASK_STACK_SIZE, LOGGING_TASK_PRIORITY, LOGGING_QUEUE_LENGTH);

    /* Proceed to init the various components of the application. */
#if (ENABLE_CONNECTIVITY)
    EVSE_Connectivity_Init();
#endif

#if (ENABLE_LCD)
    /* Init UI */
    EVSE_UI_Init();
#endif

#if (ENABLE_SE)
    /* Init Secure Element */
//     EVSE_Secure_Element_Init();
#endif

#if (CLEV663_ENABLE == 1)
    /* Init NFC Frontend */
    EVSE_NFC_Init();
#endif

#if (ENABLE_METER == 1)
    /* Init Meter */
    EVSE_Meter_Init();
#endif

#if (ENABLE_CHARGING_PROTOCOL == 1)
    EVSE_ChargingProtocol_Init();
#endif

#if (ENABLE_SHELL == 1)
    EVSE_Shell_Init();
#endif
}

/*! @brief Main function */
int main(void)
{
    /* Enable cycle counter for tx_exection_profile.c use */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->LAR = 0xC5ACCE55;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Init board hardware. */
    BOARD_RelocateVectorTableToRam();
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    vTaskStartScheduler();
    for (;;)
        ;
}

/*
 * Copyright 2021-2022,2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "wifi_bt_config.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "wifi_config.h"
#include "sdmmc_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef WIFI_BT_USE_M2_INTERFACE
#warning "M.2 interface is not supported on this board"
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void BOARD_WIFI_BT_Enable(bool enable)
{
	BOARD_SDCardPowerControl(enable);
    vTaskDelay(pdMS_TO_TICKS(100));
}

void BOARD_WIFI_BT_Config(void *card, sdio_int_t cardInt)
{
    BOARD_SDIO_Config(card, NULL, BOARD_SDMMC_SDIO_HOST_IRQ_PRIORITY, cardInt);
    BOARD_WIFI_BT_Enable(false);
}

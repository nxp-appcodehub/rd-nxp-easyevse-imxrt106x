/** @file mlan_sdio.c
 *
 *  @brief This file provides mlan driver for SDIO
 *
 *  Copyright 2008-2024 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <wmerrno.h>
#include <fsl_os_abstraction.h>
#include <board.h>
#include <wifi_bt_config.h>

#if defined(SDK_OS_FREE_RTOS)

#include <mlan_sdio_api.h>
#include <fsl_common.h>
#include <fsl_clock.h>
#include <fsl_sdio.h>
#include <fsl_sdmmc_spec.h>
#include <fsl_usdhc.h>

#define SDIO_CMD_TIMEOUT 2000

extern void handle_cdint(int error);

static sdio_card_t wm_g_sd;

static OSA_MUTEX_HANDLE_DEFINE(sdio_mutex);

void sdio_enable_interrupt(void);

int sdio_drv_creg_read(int addr, int fn, uint32_t *resp)
{
    osa_status_t ret;

    ret = OSA_MutexLock(&sdio_mutex, osaWaitForever_c);
    if (ret != KOSA_StatusSuccess)
    {
        sdio_e("failed to get mutex\r\n");
        return 0;
    }

    if (SDIO_IO_Read_Direct(&wm_g_sd, (sdio_func_num_t)fn, (uint32_t)addr, (uint8_t *)resp) != KOSA_StatusSuccess)
    {
        (void)OSA_MutexUnlock(&sdio_mutex);
        return 0;
    }

    (void)OSA_MutexUnlock(&sdio_mutex);

    return 1;
}

int sdio_drv_creg_write(int addr, int fn, uint8_t data, uint32_t *resp)
{
    osa_status_t ret;

    ret = OSA_MutexLock(&sdio_mutex, osaWaitForever_c);
    if (ret != KOSA_StatusSuccess)
    {
        sdio_e("failed to get mutex\r\n");
        return 0;
    }

    if (SDIO_IO_Write_Direct(&wm_g_sd, (sdio_func_num_t)fn, (uint32_t)addr, &data, true) != KOSA_StatusSuccess)
    {
        (void)OSA_MutexUnlock(&sdio_mutex);
        return 0;
    }

    *resp = data;

    (void)OSA_MutexUnlock(&sdio_mutex);

    return 1;
}

int sdio_drv_read(uint32_t addr, uint32_t fn, uint32_t bcnt, uint32_t bsize, uint8_t *buf, uint32_t *resp)
{
    osa_status_t ret;
    uint32_t flags = 0;
    uint32_t param;

    ret = OSA_MutexLock(&sdio_mutex, osaWaitForever_c);
    if (ret != KOSA_StatusSuccess)
    {
        sdio_e("failed to get mutex\r\n");
        return 0;
    }

    if (bcnt > 1U)
    {
        flags |= SDIO_EXTEND_CMD_BLOCK_MODE_MASK;
        param = bcnt;
    }
    else
    {
        param = bsize;
    }

    if (SDIO_IO_Read_Extended(&wm_g_sd, (sdio_func_num_t)fn, addr, buf, param, flags) != KOSA_StatusSuccess)
    {
        (void)OSA_MutexUnlock((osa_mutex_handle_t)sdio_mutex);
        return 0;
    }

    (void)OSA_MutexUnlock(&sdio_mutex);

    return 1;
}

int sdio_drv_write(uint32_t addr, uint32_t fn, uint32_t bcnt, uint32_t bsize, uint8_t *buf, uint32_t *resp)
{
    osa_status_t ret;
    uint32_t flags = 0;
    uint32_t param;

    ret = OSA_MutexLock(&sdio_mutex, osaWaitForever_c);
    if (ret != KOSA_StatusSuccess)
    {
        sdio_e("failed to get mutex\r\n");
        return false;
    }

    if (bcnt > 1U)
    {
        flags |= SDIO_EXTEND_CMD_BLOCK_MODE_MASK;
        param = bcnt;
    }
    else
    {
        param = bsize;
    }

    if (SDIO_IO_Write_Extended(&wm_g_sd, (sdio_func_num_t)fn, addr, buf, param, flags) != KOSA_StatusSuccess)
    {
        (void)OSA_MutexUnlock(&sdio_mutex);
        return 0;
    }

    (void)OSA_MutexUnlock(&sdio_mutex);

    return 1;
}

static void SDIO_CardInterruptCallBack(void *userData)
{
    SDMMCHOST_EnableCardInt(wm_g_sd.host, false);
    handle_cdint(0);
}

void sdio_enable_interrupt(void)
{
    if (wm_g_sd.isHostReady)
    {
        SDMMCHOST_EnableCardInt(wm_g_sd.host, true);
    }
}

void sdio_disable_interrupt(void)
{
    if (wm_g_sd.isHostReady)
    {
        SDMMCHOST_EnableCardInt(wm_g_sd.host, false);
    }
}

static void sdio_controller_init(void)
{
    (void)memset(&wm_g_sd, 0, sizeof(sdio_card_t));

    BOARD_WIFI_BT_Config(&wm_g_sd, SDIO_CardInterruptCallBack);

#if defined(SD_TIMING_MAX)
    wm_g_sd.currentTiming = SD_TIMING_MAX;
#endif
#if defined(SD_CLOCK_MAX)
    wm_g_sd.usrParam.maxFreq = SD_CLOCK_MAX;
#endif
}

static int sdio_card_init(void)
{
    int ret = WM_SUCCESS;
    uint32_t resp;

    if (SDIO_HostInit(&wm_g_sd) != KOSA_StatusSuccess)
    {
        return kStatus_SDMMC_HostNotReady;
    }

#if defined(SDMMCHOST_OPERATION_VOLTAGE_3V3)
    /* Disable switch to 1.8V in SDIO_ProbeBusVoltage() */
    wm_g_sd.usrParam.ioVoltage = NULL;
#elif defined(SDMMCHOST_OPERATION_VOLTAGE_1V8)
    /* Switch to 1.8V */
    if ((wm_g_sd.usrParam.ioVoltage != NULL) && (wm_g_sd.usrParam.ioVoltage->type == kSD_IOVoltageCtrlByGpio))
    {
        if (wm_g_sd.usrParam.ioVoltage->func != NULL)
        {
            wm_g_sd.usrParam.ioVoltage->func(kSDMMC_OperationVoltage180V);
        }
    }
#if SDMMCHOST_SUPPORT_VOLTAGE_CONTROL
    else if ((wm_g_sd.usrParam.ioVoltage != NULL) && (wm_g_sd.usrParam.ioVoltage->type == kSD_IOVoltageCtrlByHost))
    {
        SDMMCHOST_SwitchToVoltage(wm_g_sd.host, (uint32_t)kSDMMC_OperationVoltage180V);
    }
#endif
    else
    {
        /* Do Nothing */
    }
    wm_g_sd.operationVoltage = kSDMMC_OperationVoltage180V;
#endif

#if !defined(COEX_APP_SUPPORT) || (defined(COEX_APP_SUPPORT) && !defined(CONFIG_WIFI_IND_DNLD))
    BOARD_WIFI_BT_Enable(true);
#endif

    ret = SDIO_CardInit(&wm_g_sd);
    if (ret != WM_SUCCESS)
    {
        return ret;
    }

    (void)sdio_drv_creg_read(0x0, 0, &resp);

    sdio_d("Card Version - (0x%x)", resp & 0xff);

    /* Mask interrupts in card */
    (void)sdio_drv_creg_write(0x4, 0, 0x3, &resp);
    /* Enable IO in card */
    (void)sdio_drv_creg_write(0x2, 0, 0x2, &resp);

    (void)SDIO_SetBlockSize(&wm_g_sd, (sdio_func_num_t)0, 256);
    (void)SDIO_SetBlockSize(&wm_g_sd, (sdio_func_num_t)1, 256);
    (void)SDIO_SetBlockSize(&wm_g_sd, (sdio_func_num_t)2, 256);

    return ret;
}

static void print_card_info(sdio_card_t *card)
{
    assert(card != NULL);

    if (card->operationVoltage == kSDMMC_OperationVoltage330V)
    {
        sdio_d("Voltage: 3.3V");
    }
    else if (card->operationVoltage == kSDMMC_OperationVoltage180V)
    {
        sdio_d("Voltage: 1.8V");
    }

    if (card->currentTiming == kSD_TimingSDR12DefaultMode)
    {
        if (card->operationVoltage == kSDMMC_OperationVoltage330V)
        {
            sdio_d("Timing mode: Default mode");
        }
        else if (card->operationVoltage == kSDMMC_OperationVoltage180V)
        {
            sdio_d("Timing mode: SDR12 mode");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR25HighSpeedMode)
    {
        if (card->operationVoltage == kSDMMC_OperationVoltage180V)
        {
            sdio_d("Timing mode: SDR25");
        }
        else
        {
            sdio_d("Timing mode: High Speed");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR50Mode)
    {
        sdio_d("Timing mode: SDR50");
    }
    else if (card->currentTiming == kSD_TimingSDR104Mode)
    {
        sdio_d("Timing mode: SDR104");
    }
    else if (card->currentTiming == kSD_TimingDDR50Mode)
    {
        sdio_d("Timing mode: DDR50");
    }
}

int sdio_drv_init(void (*cd_int)(int))
{
    osa_status_t ret;

    ret = OSA_MutexCreate(&sdio_mutex);
    if (ret != KOSA_StatusSuccess)
    {
        sdio_e("Failed to create mutex");
        return -WM_FAIL;
    }

    sdio_controller_init();

    if (sdio_card_init() != WM_SUCCESS)
    {
        sdio_e("Card initialization failed");
        return -WM_FAIL;
    }
    else
    {
        sdio_d("Card initialization successful");
    }

    print_card_info(&wm_g_sd);

    return WM_SUCCESS;
}

void sdio_drv_deinit(void)
{
    osa_status_t ret;

    SDIO_Deinit(&wm_g_sd);

    ret = OSA_MutexDestroy(&sdio_mutex);
    if (ret != KOSA_StatusSuccess)
    {
        sdio_e("Failed to delete mutex");
    }
}

#elif defined(__ZEPHYR__)

#include <mlan_sdio_api.h>
#include <osa.h>
#include <fsl_common.h>
#include <fsl_gpio.h>
#include <zephyr/sd/sdio.h>

#define SDIO_CMD_TIMEOUT 2000

const struct device *sdhc_dev = DEVICE_DT_GET(DT_BUS(DT_COMPAT_GET_ANY_STATUS_OKAY(nxp_wifi)));

static struct sd_card wm_g_sd;
static struct sdio_func g_sdio_funcs[8];

int sdio_drv_creg_read(int addr, int fn, uint32_t *resp)
{
    struct sdio_func *func = &g_sdio_funcs[fn];

    if (sdio_read_byte(func, addr, (uint8_t *)resp) != 0)
    {
        return 0;
    }

    return 1;
}

int sdio_drv_creg_write(int addr, int fn, uint8_t data, uint32_t *resp)
{
    struct sdio_func *func = &g_sdio_funcs[fn];

    if (sdio_rw_byte(func, addr, data, (uint8_t *)resp) != 0)
    {
        return 0;
    }

    return 1;
}

int sdio_drv_read(uint32_t addr, uint32_t fn, uint32_t bcnt, uint32_t bsize, uint8_t *buf, uint32_t *resp)
{
    struct sdio_func *func = &g_sdio_funcs[fn];

    if (sdio_read_addr(func, addr, buf, bcnt * bsize) != 0)
    {
        return 0;
    }

    return 1;
}

int sdio_drv_write(uint32_t addr, uint32_t fn, uint32_t bcnt, uint32_t bsize, uint8_t *buf, uint32_t *resp)
{
    struct sdio_func *func = &g_sdio_funcs[fn];

    if (sdio_write_addr(func, addr, buf, bcnt * bsize) != 0)
    {
        return 0;
    }

    return 1;
}

extern void handle_cdint(int error);

void sdio_irq_handler(const struct device *dev, int reason, const void *user_data)
{
    if (reason == SDHC_INT_SDIO)
    {
        sdhc_disable_interrupt(sdhc_dev, SDHC_INT_SDIO);
        handle_cdint(0);
    }
}

void sdio_enable_interrupt(void)
{
    sdhc_enable_interrupt(sdhc_dev, (sdhc_interrupt_cb_t)sdio_irq_handler, SDHC_INT_SDIO, NULL);
    return;
}

static void sdio_controller_init(void)
{
    (void)memset(&wm_g_sd, 0, sizeof(struct sd_card));
}

static int sdio_card_init(void)
{
    int ret = WM_SUCCESS;
    uint32_t resp;

    if (!device_is_ready(sdhc_dev))
    {
        sdio_e("SD controller not ready");
        return -EIO;
    }

    if (!sdhc_card_present(sdhc_dev))
    {
        sdio_e("SDIO card not present");
        return -EIO;
    }

    ret = sd_init(sdhc_dev, &wm_g_sd);
    if (ret)
    {
        return ret;
    }

    memcpy(&g_sdio_funcs[0], &wm_g_sd.func0, sizeof(struct sdio_func));
    (void)sdio_drv_creg_read(0x0, 0, &resp);

    sdio_d("Card Version - (0x%x)", resp & 0xff);
    /* Init SDIO functions */
    sdio_init_func(&wm_g_sd, &g_sdio_funcs[1], SDIO_FUNC_NUM_1);
    sdio_init_func(&wm_g_sd, &g_sdio_funcs[2], SDIO_FUNC_NUM_2);

    /* Mask interrupts in card */
    (void)sdio_drv_creg_write(0x4, 0, 0x3, &resp);
    /* Enable IO in card */
    (void)sdio_drv_creg_write(0x2, 0, 0x2, &resp);

    (void)sdio_set_block_size(&g_sdio_funcs[0], 256);
    (void)sdio_set_block_size(&g_sdio_funcs[1], 256);
    (void)sdio_set_block_size(&g_sdio_funcs[2], 256);

    return ret;
}

int sdio_drv_init(void (*cd_int)(int))
{
    sdio_controller_init();

    if (sdio_card_init() != WM_SUCCESS)
    {
        sdio_e("Card initialization failed");
        return -WM_FAIL;
    }
    else
    {
        sdio_d("Card initialization successful");
    }

    return WM_SUCCESS;
}

void sdio_drv_deinit(void)
{
    // SDIO_Deinit(&wm_g_sd);
}

#endif

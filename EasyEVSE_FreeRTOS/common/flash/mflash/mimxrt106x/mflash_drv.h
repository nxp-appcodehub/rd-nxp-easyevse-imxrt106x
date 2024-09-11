/*
 * Copyright 2018-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __MFLASH_DRV_H__
#define __MFLASH_DRV_H__

#include "board.h"

#if (MIMXRT1060_REVA == 1) || (MIMXRT1060_REVB == 1) || (MIMXRT1064_REVA == 1)
#include "IS25WP064AJBLE.h"
#elif (MIMXRT1060_REVC == 0)
/* TODO */
#endif

extern const uint32_t customLUT[CUSTOM_LUT_LENGTH];

#define NOR_CMD_LUT_SEQ_IDX_READ_NORMAL        2
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST          1
#define NOR_CMD_LUT_SEQ_IDX_READ_FAST_QUAD     0 // set it to index0 to align with xip settings
#define NOR_CMD_LUT_SEQ_IDX_READSTATUS         3
#define NOR_CMD_LUT_SEQ_IDX_WRITEENABLE        4
#define NOR_CMD_LUT_SEQ_IDX_ERASESECTOR        5
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_SINGLE 6
#define NOR_CMD_LUT_SEQ_IDX_PAGEPROGRAM_QUAD   7
#define NOR_CMD_LUT_SEQ_IDX_READID             8
#define NOR_CMD_LUT_SEQ_IDX_WRITESTATUSREG     9
#define NOR_CMD_LUT_SEQ_IDX_ENTERQPI           10
#define NOR_CMD_LUT_SEQ_IDX_EXITQPI            11
#define NOR_CMD_LUT_SEQ_IDX_READSTATUSREG      12

/* Flash constants */
#ifndef MFLASH_SECTOR_SIZE
#define MFLASH_SECTOR_SIZE FLASH_SECTOR_SIZE
#endif

#ifndef MFLASH_PAGE_SIZE
#define MFLASH_PAGE_SIZE FLASH_PAGE_SIZE
#endif

#ifndef MFLASH_FLEXSPI
#define MFLASH_FLEXSPI (BOARD_FLEXSPI)
#endif

#ifndef MFLASH_BASE_ADDRESS
#define MFLASH_BASE_ADDRESS (BOARD_FLEXSPI_BASE)
#endif

#endif

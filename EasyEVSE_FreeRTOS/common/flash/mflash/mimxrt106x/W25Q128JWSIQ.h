/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MIMXRT106X_W25Q128JWSIQ_H_
#define MIMXRT106X_W25Q128JWSIQ_H_

#include "fsl_flexspi.h"

/* Flash constants */

#define FLASH_SECTOR_SIZE (0x1000)
#define FLASH_PAGE_SIZE   (256)

#define CUSTOM_LUT_LENGTH        60
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0

#define NORFLASH_ADDR_LENGTH 0x18

#define WriteEnable 0x06

#define ReadStatusReg1 0x05
#define RDERPNV        0x81

#define WriteStatusReg1 0x01

#define ReadData     0x03
#define FastReadData 0x0B
#define FastReadQuad 0xEB

#define PageProgram     0x02
#define PageProgramQuad 0x32

#define SectorErase 0x20

#define ChipErase 0xC7

#define ProductID 0x9F

#endif /* MIMXRT106X_W25Q128JWSIQ_H_ */

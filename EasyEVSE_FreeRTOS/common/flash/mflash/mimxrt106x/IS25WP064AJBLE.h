/*
 * Copyright 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _IS25WP064AJBLE_H_
#define _IS25WP064AJBLE_H_

#include "fsl_flexspi.h"

/* Flash constants */

#define FLASH_SECTOR_SIZE   (0x1000)
#define FLASH_PAGE_SIZE     (256)


#define CUSTOM_LUT_LENGTH        60
#define FLASH_BUSY_STATUS_POL    1
#define FLASH_BUSY_STATUS_OFFSET 0

#define NORFLASH_ADDR_LENGTH   24
#define NORFLASH_ADDR_LENGTH4B 32

#define WriteEnable  0x06
#define WriteDisable 0x04

#define ReadStatusReg1  0x05
#define RDERPNV         0x05

#define WriteStatusReg1 0x01

#define ReadData           0x03
#define FastReadData       0x0B
#define FastReadQuad       0xEB

#define PageProgram       0x02
#define PageProgramQuad   0x32

#define BlockErase       0xD8
#define SectorErase      0x20

#define ChipErase        0xC7
#define PowerDown        0xB9

#define ProductID        0xAB
#define DeviceID         0x4B
#define ManufactDeviceID 0x90
#define JedecDeviceID    0x9F

#endif /* _IS25WP064AJBLE_H_ */

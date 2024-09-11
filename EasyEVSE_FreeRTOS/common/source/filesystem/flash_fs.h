
/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FLASH_FS_
#define _FLASH_FS_

/**
 * @brief Returns codes of the flash management functions
 */
typedef enum _flash_fs_status
{
    FLASH_FS_OK         = 0x00,  /**< No errors */
    FLASH_FS_FAIL       = -0x01, /**< Fail for unknown reason */
    FLASH_FS_ENOLOCK    = -0x02, /**< Error file is locked */
    FLASH_FS_ERETRY     = -0x10, /**< Fail to lock the file */
    FLASH_FS_ENOMEM     = -0x20, /**< Fail to allocate memory */
    FLASH_FS_ENOMEM2    = -0x21, /**< Fail to allocate memory */
    FLASH_FS_ENOMEM3    = -0x22, /**< Fail to allocate memory */
    FLASH_FS_ENOMEM4    = -0x23, /**< Fail to allocate memory */
    FLASH_FS_EIO        = -0x30, /**< Fail to erase the sector */
    FLASH_FS_EINVAL     = -0x40, /**< Invalid input value */
    FLASH_FS_EINVAL2    = -0x41, /**< Invalid input value */
    FLASH_FS_EINVAL3    = -0x42, /**< Invalid input value */
    FLASH_FS_ENOENTRY   = -0x50, /**< No entry/directory found in the flash */
    FLASH_FS_ENOENTRY2  = -0x51, /**< No entry found in the flash */
    FLASH_FS_ENOENTRY3  = -0x52, /**< No entry found in the flash */
    FLASH_FS_EOVERFLOW  = -0x60, /**< The flash map is full */
    FLASH_FS_EOVERFLOW2 = -0x61, /**< The save file doesn't fit in the current sector */
    FLASH_FS_EENCRYPT   = -0x70, /**< Fail to encrypt or decrypt */
    FLASH_FS_EENCRYPT2  = -0x71, /**< Crc mismatch */
    FLASH_FS_CORRUPTED  = -0x72, /**< File system corrupted */
    FLASH_FS_FILEEXIST  = -0x73, /**< File already exists */
} flash_fs_status_t;

/* Use the flash API with or without encryption */
#define FLASH_PLAIN     (false)
#define FLASH_ENCRYPTED (true)

typedef struct
{
    void (*post_lock_cb)(void);         /*! Callback to be called after the lock takes place */
    void (*post_unlock_cb)(void);       /*! Callback to be called after the unlock takes place */
    void (*pre_sector_erase_cb)(void);  /*! Callback to be called before erasing a sector */
    void (*post_sector_erase_cb)(void); /*! Callback to be called after erasing a sector */
} flash_fs_cbs_t;

#endif /* _FLASH_FS_ */

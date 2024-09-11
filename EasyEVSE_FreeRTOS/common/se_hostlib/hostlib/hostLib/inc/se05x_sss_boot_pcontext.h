/*
 *
 * Copyright 2019-2020 NXP
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef SE05X_SSS_BOOT_PCONTEXT_H_
#define SE05X_SSS_BOOT_PCONTEXT_H_

#include <ex_sss.h>
#include <ex_sss_boot.h>

extern ex_sss_boot_ctx_t gex_sss_demo_boot_ctx;

#define EX_SSS_BOOT_PCONTEXT (&gex_sss_demo_boot_ctx)
#define PCONTEXT EX_SSS_BOOT_PCONTEXT
#define EX_SSS_BOOT_EXPOSE_ARGC_ARGV 0
#define EX_SSS_BOOT_DO_ERASE 0

#endif /* SE05X_SSS_BOOT_PCONTEXT_H_ */

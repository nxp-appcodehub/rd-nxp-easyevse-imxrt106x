/*
 * Copyright 2020-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _USDHC_CLA_CONFIG_H_
#define _USDHC_CLA_CONFIG_H_

//
/*     Imported from  Clarinox Init USDHC Pins part                                 */
/*     PAY Attention Not re-generate with pinTool (otherwise this part disappear)   */
//

#include "fsl_common.h"

#define FREQ_50_MHZ     50000000U
#define FREQ_100_MHZ    100000000U

#define SDRAM_PIN_DRIVESTRENGTH 0x0110F9u // 0x0110D1u // 0x0110F9u // 0x0110D1u // 0x0110F9u

//Funtion prototype that Clarinox use to configurate the USDHC Pins
void BOARD_ConfigureUSDHCPins(uint32_t frequency);

#endif /* _USDHC_CLA_CONFIG_H_ */

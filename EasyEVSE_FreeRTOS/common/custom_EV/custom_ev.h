/*

 * Copyright 2023-2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CUSTOM_EVSE_H_
#define __CUSTOM_EVSE_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"
#include "EVSE_Metering.h"
#include "EVSE_Connectivity.h"
#include "EVSE_ChargingProtocol.h"
#include <ISO15118.h>
#include "EVSE_UI.h"
#include "EVSE_Utils.h"

#define NOT_CHARGING        0
#define MAX_EVSE_SW_VERSION 20

#define RED_LED   0xfc0702
#define GREEN_LED 0x40d440
#define BLUE_LED  0x0000e0

#define TIME_TO_CHARGE_STRING       "Time to charge"
#define TIME_TO_DISCHARGE_STRING    "Time to discharge"
#define NO_CHARGING_PROTOCOL        "None"
#define INFINITE_TIME               "Inf"

void UI_Update_ChargingParameters(const vehicle_data_t *vehicle_data);

#ifdef __cplusplus
}
#endif
#endif /* __CUSTOM_EVSE_H_ */

/*
* Copyright 2023-2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/


#ifndef EVENTS_INIT_H_
#define EVENTS_INIT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

void events_init(lv_ui *ui);

void events_init_Main_Screen(lv_ui *ui);
void events_init_Debug_Conn_Screen(lv_ui *ui);
void events_init_Car_Screen(lv_ui *ui);
void events_init_EVSE_Screen(lv_ui *ui);
void events_init_Meter_Screen(lv_ui *ui);
void events_init_NFC_Screen(lv_ui *ui);
void events_init_Debug_ISO_Screen(lv_ui *ui);

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */

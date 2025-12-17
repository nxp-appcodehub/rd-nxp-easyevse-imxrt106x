/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif


static void Main_screen_EV_img_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        extern void EV_ChargingProtocol_ChangeChargingDirection(void);
        EV_ChargingProtocol_ChangeChargingDirection();
        break;
    }
    default:
        break;
    }
}

void events_init_Main_screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Main_screen_EV_img, Main_screen_EV_img_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}

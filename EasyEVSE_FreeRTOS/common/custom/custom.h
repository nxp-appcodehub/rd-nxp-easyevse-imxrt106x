/*
 * Copyright 2023-2024 NXP
 * NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
 * accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
 * comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
 * terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef __CUSTOM_H_
#define __CUSTOM_H_
#ifdef __cplusplus
extern "C" {
#endif

#include "gui_guider.h"

#define RED_LED       0xfc0702
#define GREEEN_LED    0x40d440

/**
 * @brief Create a demo application
 * @param None
 * @return None
 */
void custom_init(lv_ui *ui);

/**
 * @brief Load another screen and update widgets
 * @param *new_screen pointer to the screen that should be loaded
 * @return None
*/
void ui_ChangeScreen(lv_obj_t *new_screen);

/**
 * @brief This function is called when button ID_meter_data_btn is pressed
 * @param None
 * @return None
 */
void update_meter_data(void);

/*
 * @brief This function is called when button read_UID_btn is pressed
 * @param None
 * @return None
 */
void read_NFC_UID(void);

#ifdef __cplusplus
}
#endif
#endif /* EVENT_CB_H_ */

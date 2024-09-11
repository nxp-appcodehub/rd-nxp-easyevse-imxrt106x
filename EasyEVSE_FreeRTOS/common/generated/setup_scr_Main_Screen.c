/*
* Copyright 2023-2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_Main_Screen(lv_ui *ui)
{
	//Write codes Main_Screen
	ui->Main_Screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->Main_Screen, 480, 272);
	lv_obj_set_scrollbar_mode(ui->Main_Screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for Main_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Main_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_Screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_Screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_src(ui->Main_Screen, &_mainpng_480x272, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_img_opa(ui->Main_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_ID_EV_btn
	ui->Main_Screen_ID_EV_btn = lv_imgbtn_create(ui->Main_Screen);
	lv_obj_add_flag(ui->Main_Screen_ID_EV_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Main_Screen_ID_EV_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_evse_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_EV_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_evse_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_EV_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_evse_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_EV_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_evse_alpha_60x61, NULL);
	ui->Main_Screen_ID_EV_btn_label = lv_label_create(ui->Main_Screen_ID_EV_btn);
	lv_label_set_text(ui->Main_Screen_ID_EV_btn_label, "");
	lv_label_set_long_mode(ui->Main_Screen_ID_EV_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Main_Screen_ID_EV_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Main_Screen_ID_EV_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Main_Screen_ID_EV_btn, 123, 170);
	lv_obj_set_size(ui->Main_Screen_ID_EV_btn, 60, 61);
	lv_obj_add_flag(ui->Main_Screen_ID_EV_btn, LV_OBJ_FLAG_CLICKABLE);

	//Write style for Main_Screen_ID_EV_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_Screen_ID_EV_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_EV_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_EV_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_ID_EV_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_EV_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_Screen_ID_EV_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_EV_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_EV_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_EV_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_EV_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_EV_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Main_Screen_ID_EV_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_EV_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_EV_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_EV_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_EV_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_EV_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Main_Screen_ID_EV_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_EV_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Main_Screen_ID_debug_conn_btn
	ui->Main_Screen_ID_debug_conn_btn = lv_imgbtn_create(ui->Main_Screen);
	lv_obj_add_flag(ui->Main_Screen_ID_debug_conn_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_conn_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_debug_alpha_62x62, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_conn_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_debug_alpha_62x62, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_conn_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_debug_alpha_62x62, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_conn_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_debug_alpha_62x62, NULL);
	ui->Main_Screen_ID_debug_conn_btn_label = lv_label_create(ui->Main_Screen_ID_debug_conn_btn);
	lv_label_set_text(ui->Main_Screen_ID_debug_conn_btn_label, "");
	lv_label_set_long_mode(ui->Main_Screen_ID_debug_conn_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Main_Screen_ID_debug_conn_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Main_Screen_ID_debug_conn_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Main_Screen_ID_debug_conn_btn, 21, 75);
	lv_obj_set_size(ui->Main_Screen_ID_debug_conn_btn, 62, 62);
	lv_obj_add_flag(ui->Main_Screen_ID_debug_conn_btn, LV_OBJ_FLAG_CLICKABLE);

	//Write style for Main_Screen_ID_debug_conn_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_Screen_ID_debug_conn_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_debug_conn_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_debug_conn_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_ID_debug_conn_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_debug_conn_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_Screen_ID_debug_conn_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_debug_conn_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_debug_conn_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_debug_conn_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_debug_conn_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_debug_conn_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Main_Screen_ID_debug_conn_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_debug_conn_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_debug_conn_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_debug_conn_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_debug_conn_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_debug_conn_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Main_Screen_ID_debug_conn_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_debug_conn_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Main_Screen_ID_car_btn
	ui->Main_Screen_ID_car_btn = lv_imgbtn_create(ui->Main_Screen);
	lv_obj_add_flag(ui->Main_Screen_ID_car_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Main_Screen_ID_car_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_carpng_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_car_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_carpng_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_car_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_carpng_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_car_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_carpng_alpha_60x61, NULL);
	ui->Main_Screen_ID_car_btn_label = lv_label_create(ui->Main_Screen_ID_car_btn);
	lv_label_set_text(ui->Main_Screen_ID_car_btn_label, "");
	lv_label_set_long_mode(ui->Main_Screen_ID_car_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Main_Screen_ID_car_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Main_Screen_ID_car_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Main_Screen_ID_car_btn, 123, 58);
	lv_obj_set_size(ui->Main_Screen_ID_car_btn, 60, 61);
	lv_obj_add_flag(ui->Main_Screen_ID_car_btn, LV_OBJ_FLAG_CLICKABLE);

	//Write style for Main_Screen_ID_car_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_Screen_ID_car_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_car_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_car_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_ID_car_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_car_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_Screen_ID_car_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_car_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_car_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_car_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_car_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_car_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Main_Screen_ID_car_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_car_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_car_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_car_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_car_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_car_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Main_Screen_ID_car_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_car_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Main_Screen_ID_meter_btn
	ui->Main_Screen_ID_meter_btn = lv_imgbtn_create(ui->Main_Screen);
	lv_obj_add_flag(ui->Main_Screen_ID_meter_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Main_Screen_ID_meter_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_meterguage_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_meter_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_meterguage_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_meter_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_meterguage_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_meter_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_meterguage_alpha_60x61, NULL);
	ui->Main_Screen_ID_meter_btn_label = lv_label_create(ui->Main_Screen_ID_meter_btn);
	lv_label_set_text(ui->Main_Screen_ID_meter_btn_label, "");
	lv_label_set_long_mode(ui->Main_Screen_ID_meter_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Main_Screen_ID_meter_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Main_Screen_ID_meter_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Main_Screen_ID_meter_btn, 315, 58);
	lv_obj_set_size(ui->Main_Screen_ID_meter_btn, 60, 61);
	lv_obj_add_flag(ui->Main_Screen_ID_meter_btn, LV_OBJ_FLAG_CLICKABLE);

	//Write style for Main_Screen_ID_meter_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_Screen_ID_meter_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_meter_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_meter_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_ID_meter_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_meter_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_Screen_ID_meter_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_meter_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_meter_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_meter_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_meter_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_meter_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Main_Screen_ID_meter_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_meter_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_meter_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_meter_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_meter_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_meter_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Main_Screen_ID_meter_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_meter_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Main_Screen_ID_NFC_btn
	ui->Main_Screen_ID_NFC_btn = lv_imgbtn_create(ui->Main_Screen);
	lv_obj_add_flag(ui->Main_Screen_ID_NFC_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Main_Screen_ID_NFC_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_nfc_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_NFC_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_nfc_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_NFC_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_nfc_alpha_60x61, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_NFC_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_nfc_alpha_60x61, NULL);
	ui->Main_Screen_ID_NFC_btn_label = lv_label_create(ui->Main_Screen_ID_NFC_btn);
	lv_label_set_text(ui->Main_Screen_ID_NFC_btn_label, "");
	lv_label_set_long_mode(ui->Main_Screen_ID_NFC_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Main_Screen_ID_NFC_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Main_Screen_ID_NFC_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Main_Screen_ID_NFC_btn, 315, 170);
	lv_obj_set_size(ui->Main_Screen_ID_NFC_btn, 60, 61);
	lv_obj_add_flag(ui->Main_Screen_ID_NFC_btn, LV_OBJ_FLAG_CLICKABLE);

	//Write style for Main_Screen_ID_NFC_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_Screen_ID_NFC_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_NFC_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_NFC_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_ID_NFC_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_NFC_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_Screen_ID_NFC_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_NFC_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_NFC_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_NFC_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_NFC_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_NFC_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Main_Screen_ID_NFC_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_NFC_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_NFC_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_NFC_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_NFC_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_NFC_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Main_Screen_ID_NFC_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_NFC_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Main_Screen_type_of_connection_label
	ui->Main_Screen_type_of_connection_label = lv_label_create(ui->Main_Screen);
	lv_label_set_text(ui->Main_Screen_type_of_connection_label, "Wi-Fi");
	lv_label_set_long_mode(ui->Main_Screen_type_of_connection_label, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_Screen_type_of_connection_label, 202, 161);
	lv_obj_set_size(ui->Main_Screen_type_of_connection_label, 92, 23);
	lv_obj_add_flag(ui->Main_Screen_type_of_connection_label, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	//Write style for Main_Screen_type_of_connection_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_Screen_type_of_connection_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_type_of_connection_label, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_type_of_connection_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_Screen_type_of_connection_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_type_of_connection_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_Screen_type_of_connection_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_SSID_prompt
	ui->Main_Screen_SSID_prompt = lv_label_create(ui->Main_Screen);
	lv_label_set_text(ui->Main_Screen_SSID_prompt, "No Network");
	lv_label_set_long_mode(ui->Main_Screen_SSID_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_Screen_SSID_prompt, 109, 17);
	lv_obj_set_size(ui->Main_Screen_SSID_prompt, 286, 24);

	//Write style for Main_Screen_SSID_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_Screen_SSID_prompt, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_Screen_SSID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_Screen_SSID_prompt, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_Screen_SSID_prompt, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_Screen_SSID_prompt, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_Screen_SSID_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_SSID_prompt, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_SSID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_Screen_SSID_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_SSID_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_Screen_SSID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_Screen_SSID_prompt, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_Screen_SSID_prompt, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_Screen_SSID_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_blue_line_top
	ui->Main_Screen_blue_line_top = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_blue_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Main_Screen_blue_line_top, Main_Screen_blue_line_top, 2);
	lv_obj_set_pos(ui->Main_Screen_blue_line_top, 5, 2);
	lv_obj_set_size(ui->Main_Screen_blue_line_top, 470, 1);

	//Write style for Main_Screen_blue_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_blue_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_blue_line_top, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_blue_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_blue_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_blue_line_bottom
	ui->Main_Screen_blue_line_bottom = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_blue_line_bottom[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Main_Screen_blue_line_bottom, Main_Screen_blue_line_bottom, 2);
	lv_obj_set_pos(ui->Main_Screen_blue_line_bottom, 3, 269);
	lv_obj_set_size(ui->Main_Screen_blue_line_bottom, 474, 1);

	//Write style for Main_Screen_blue_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_blue_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_blue_line_bottom, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_blue_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_blue_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_blue_line_left
	ui->Main_Screen_blue_line_left = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_blue_line_left[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Main_Screen_blue_line_left, Main_Screen_blue_line_left, 2);
	lv_obj_set_pos(ui->Main_Screen_blue_line_left, 3, 2);
	lv_obj_set_size(ui->Main_Screen_blue_line_left, 2, 268);

	//Write style for Main_Screen_blue_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_blue_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_blue_line_left, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_blue_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_blue_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_blue_line_right
	ui->Main_Screen_blue_line_right = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_blue_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Main_Screen_blue_line_right, Main_Screen_blue_line_right, 2);
	lv_obj_set_pos(ui->Main_Screen_blue_line_right, 477, 3);
	lv_obj_set_size(ui->Main_Screen_blue_line_right, 1, 264);

	//Write style for Main_Screen_blue_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_blue_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_blue_line_right, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_blue_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_blue_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_orange_line_left
	ui->Main_Screen_orange_line_left = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_orange_line_left[] ={{0, 0},{0, 270},};
	lv_line_set_points(ui->Main_Screen_orange_line_left, Main_Screen_orange_line_left, 2);
	lv_obj_set_pos(ui->Main_Screen_orange_line_left, 1, 2);
	lv_obj_set_size(ui->Main_Screen_orange_line_left, 1, 268);

	//Write style for Main_Screen_orange_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_orange_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_orange_line_left, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_orange_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_orange_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_green_line_top
	ui->Main_Screen_green_line_top = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_green_line_top[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->Main_Screen_green_line_top, Main_Screen_green_line_top, 2);
	lv_obj_set_pos(ui->Main_Screen_green_line_top, 6, 4);
	lv_obj_set_size(ui->Main_Screen_green_line_top, 468, 1);

	//Write style for Main_Screen_green_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_green_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_green_line_top, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_green_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_green_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_green_line_bottom
	ui->Main_Screen_green_line_bottom = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_green_line_bottom[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->Main_Screen_green_line_bottom, Main_Screen_green_line_bottom, 2);
	lv_obj_set_pos(ui->Main_Screen_green_line_bottom, 5, 267);
	lv_obj_set_size(ui->Main_Screen_green_line_bottom, 469, 1);

	//Write style for Main_Screen_green_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_green_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_green_line_bottom, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_green_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_green_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_green_line_left
	ui->Main_Screen_green_line_left = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_green_line_left[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->Main_Screen_green_line_left, Main_Screen_green_line_left, 2);
	lv_obj_set_pos(ui->Main_Screen_green_line_left, 5, 4);
	lv_obj_set_size(ui->Main_Screen_green_line_left, 1, 262);

	//Write style for Main_Screen_green_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_green_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_green_line_left, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_green_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_green_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_green_line_right
	ui->Main_Screen_green_line_right = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_green_line_right[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->Main_Screen_green_line_right, Main_Screen_green_line_right, 2);
	lv_obj_set_pos(ui->Main_Screen_green_line_right, 475, 4);
	lv_obj_set_size(ui->Main_Screen_green_line_right, 1, 260);

	//Write style for Main_Screen_green_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_green_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_green_line_right, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_green_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_green_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_orange_line_top
	ui->Main_Screen_orange_line_top = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_orange_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Main_Screen_orange_line_top, Main_Screen_orange_line_top, 2);
	lv_obj_set_pos(ui->Main_Screen_orange_line_top, 1, 1);
	lv_obj_set_size(ui->Main_Screen_orange_line_top, 476, 1);

	//Write style for Main_Screen_orange_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_orange_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_orange_line_top, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_orange_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_orange_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_orange_line_bottom
	ui->Main_Screen_orange_line_bottom = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_orange_line_bottom[] ={{0, 0},{478, 0},};
	lv_line_set_points(ui->Main_Screen_orange_line_bottom, Main_Screen_orange_line_bottom, 2);
	lv_obj_set_pos(ui->Main_Screen_orange_line_bottom, 1, 271);
	lv_obj_set_size(ui->Main_Screen_orange_line_bottom, 479, 1);

	//Write style for Main_Screen_orange_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_orange_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_orange_line_bottom, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_orange_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_orange_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_orange_line_right
	ui->Main_Screen_orange_line_right = lv_line_create(ui->Main_Screen);
	static lv_point_t Main_Screen_orange_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Main_Screen_orange_line_right, Main_Screen_orange_line_right, 2);
	lv_obj_set_pos(ui->Main_Screen_orange_line_right, 479, 1);
	lv_obj_set_size(ui->Main_Screen_orange_line_right, 1, 268);

	//Write style for Main_Screen_orange_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Main_Screen_orange_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Main_Screen_orange_line_right, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Main_Screen_orange_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Main_Screen_orange_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_EVSE_software_version
	ui->Main_Screen_EVSE_software_version = lv_label_create(ui->Main_Screen);
	lv_label_set_text(ui->Main_Screen_EVSE_software_version, "EVSE SW v");
	lv_label_set_long_mode(ui->Main_Screen_EVSE_software_version, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_Screen_EVSE_software_version, 375, 245);
	lv_obj_set_size(ui->Main_Screen_EVSE_software_version, 98, 20);

	//Write style for Main_Screen_EVSE_software_version, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_Screen_EVSE_software_version, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Main_Screen_EVSE_software_version, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Main_Screen_EVSE_software_version, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Main_Screen_EVSE_software_version, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_Screen_EVSE_software_version, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_Screen_EVSE_software_version, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_EVSE_software_version, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_EVSE_software_version, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_Screen_EVSE_software_version, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_EVSE_software_version, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_Screen_EVSE_software_version, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Main_Screen_EVSE_software_version, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Main_Screen_EVSE_software_version, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_Screen_EVSE_software_version, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Main_Screen_ID_debug_ISO_btn
	ui->Main_Screen_ID_debug_ISO_btn = lv_imgbtn_create(ui->Main_Screen);
	lv_obj_add_flag(ui->Main_Screen_ID_debug_ISO_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_ISO_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_debug_ISO15118_alpha_62x62, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_ISO_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_debug_ISO15118_alpha_62x62, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_ISO_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_debug_ISO15118_alpha_62x62, NULL);
	lv_imgbtn_set_src(ui->Main_Screen_ID_debug_ISO_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_debug_ISO15118_alpha_62x62, NULL);
	ui->Main_Screen_ID_debug_ISO_btn_label = lv_label_create(ui->Main_Screen_ID_debug_ISO_btn);
	lv_label_set_text(ui->Main_Screen_ID_debug_ISO_btn_label, "");
	lv_label_set_long_mode(ui->Main_Screen_ID_debug_ISO_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Main_Screen_ID_debug_ISO_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Main_Screen_ID_debug_ISO_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Main_Screen_ID_debug_ISO_btn, 21, 149);
	lv_obj_set_size(ui->Main_Screen_ID_debug_ISO_btn, 62, 62);
	lv_obj_add_flag(ui->Main_Screen_ID_debug_ISO_btn, LV_OBJ_FLAG_CLICKABLE);

	//Write style for Main_Screen_ID_debug_ISO_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Main_Screen_ID_debug_ISO_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_debug_ISO_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_debug_ISO_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_ID_debug_ISO_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_debug_ISO_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Main_Screen_ID_debug_ISO_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_debug_ISO_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_debug_ISO_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_debug_ISO_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_debug_ISO_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_debug_ISO_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Main_Screen_ID_debug_ISO_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_debug_ISO_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Main_Screen_ID_debug_ISO_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Main_Screen_ID_debug_ISO_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Main_Screen_ID_debug_ISO_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Main_Screen_ID_debug_ISO_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Main_Screen_ID_debug_ISO_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Main_Screen_ID_debug_ISO_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Main_Screen_clock
	ui->Main_Screen_clock = lv_label_create(ui->Main_Screen);
	lv_label_set_text(ui->Main_Screen_clock, "0:00");
	lv_label_set_long_mode(ui->Main_Screen_clock, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Main_Screen_clock, 15, 17);
	lv_obj_set_size(ui->Main_Screen_clock, 87, 24);

	//Write style for Main_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Main_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Main_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Main_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Main_Screen_clock, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Main_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of Main_Screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->Main_Screen);

	//Init events for screen.
	events_init_Main_Screen(ui);
}

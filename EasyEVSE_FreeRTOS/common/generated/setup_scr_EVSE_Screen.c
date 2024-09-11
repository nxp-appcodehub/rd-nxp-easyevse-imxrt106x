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



void setup_scr_EVSE_Screen(lv_ui *ui)
{
	//Write codes EVSE_Screen
	ui->EVSE_Screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->EVSE_Screen, 480, 272);
	lv_obj_set_scrollbar_mode(ui->EVSE_Screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for EVSE_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->EVSE_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_EVSE_Menu_Heading
	ui->EVSE_Screen_EVSE_Menu_Heading = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_EVSE_Menu_Heading, "EVSE Menu");
	lv_label_set_long_mode(ui->EVSE_Screen_EVSE_Menu_Heading, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_EVSE_Menu_Heading, 176, 19);
	lv_obj_set_size(ui->EVSE_Screen_EVSE_Menu_Heading, 164, 24);

	//Write style for EVSE_Screen_EVSE_Menu_Heading, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_EVSE_Menu_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_EVSE_Menu_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_EVSE_Menu_Heading, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_EVSE_Menu_Heading, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_EVSE_Menu_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_EVSE_Menu_Heading, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_EVSE_Menu_Heading, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_EVSE_Menu_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_EVSE_Menu_Heading, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_EVSE_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_EVSE_Menu_Heading, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_EVSE_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_EVSE_Menu_Heading, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_EVSE_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_EVSE_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_EVSE_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_EVSE_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_ID_back_btn
	ui->EVSE_Screen_ID_back_btn = lv_imgbtn_create(ui->EVSE_Screen);
	lv_obj_add_flag(ui->EVSE_Screen_ID_back_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->EVSE_Screen_ID_back_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->EVSE_Screen_ID_back_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->EVSE_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->EVSE_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	ui->EVSE_Screen_ID_back_btn_label = lv_label_create(ui->EVSE_Screen_ID_back_btn);
	lv_label_set_text(ui->EVSE_Screen_ID_back_btn_label, "");
	lv_label_set_long_mode(ui->EVSE_Screen_ID_back_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->EVSE_Screen_ID_back_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->EVSE_Screen_ID_back_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->EVSE_Screen_ID_back_btn, 430, 15);
	lv_obj_set_size(ui->EVSE_Screen_ID_back_btn, 32, 28);
	lv_obj_add_flag(ui->EVSE_Screen_ID_back_btn, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->EVSE_Screen_ID_back_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	//Write style for EVSE_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->EVSE_Screen_ID_back_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_ID_back_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for EVSE_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->EVSE_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->EVSE_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->EVSE_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for EVSE_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->EVSE_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->EVSE_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->EVSE_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for EVSE_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->EVSE_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes EVSE_Screen_power_rate_prompt
	ui->EVSE_Screen_power_rate_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_power_rate_prompt, "Power Rate [A]");
	lv_label_set_long_mode(ui->EVSE_Screen_power_rate_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_power_rate_prompt, 248, 126);
	lv_obj_set_size(ui->EVSE_Screen_power_rate_prompt, 114, 24);

	//Write style for EVSE_Screen_power_rate_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_power_rate_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_power_rate_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_power_rate_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_power_rate_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_power_rate_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_power_rate_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_power_rate_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_sigboard_versions_prompt
	ui->EVSE_Screen_sigboard_versions_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_sigboard_versions_prompt, "SIGBOARD");
	lv_label_set_long_mode(ui->EVSE_Screen_sigboard_versions_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_sigboard_versions_prompt, 20, 169);
	lv_obj_set_size(ui->EVSE_Screen_sigboard_versions_prompt, 84, 24);

	//Write style for EVSE_Screen_sigboard_versions_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_sigboard_versions_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_sigboard_versions_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_sigboard_versions_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_sigboard_versions_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_sigboard_versions_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_sigboard_versions_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_sigboard_versions_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_charge_cost_prompt
	ui->EVSE_Screen_charge_cost_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_charge_cost_prompt, "Charge Cost [$]");
	lv_label_set_long_mode(ui->EVSE_Screen_charge_cost_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_charge_cost_prompt, 248, 91);
	lv_obj_set_size(ui->EVSE_Screen_charge_cost_prompt, 110, 23);

	//Write style for EVSE_Screen_charge_cost_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_charge_cost_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_charge_cost_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_charge_cost_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_charge_cost_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_charge_cost_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_charge_cost_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_charge_cost_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_EVDE_ID_prompt
	ui->EVSE_Screen_EVDE_ID_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_EVDE_ID_prompt, "EVSE ID");
	lv_label_set_long_mode(ui->EVSE_Screen_EVDE_ID_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_EVDE_ID_prompt, -5, 90);
	lv_obj_set_size(ui->EVSE_Screen_EVDE_ID_prompt, 114, 24);

	//Write style for EVSE_Screen_EVDE_ID_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_EVDE_ID_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_EVDE_ID_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_EVDE_ID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_EVDE_ID_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_EVDE_ID_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_EVDE_ID_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_grid_limit_prompt
	ui->EVSE_Screen_grid_limit_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_grid_limit_prompt, "Grid Limit [A]");
	lv_label_set_long_mode(ui->EVSE_Screen_grid_limit_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_grid_limit_prompt, 252, 169);
	lv_obj_set_size(ui->EVSE_Screen_grid_limit_prompt, 114, 24);

	//Write style for EVSE_Screen_grid_limit_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_grid_limit_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_grid_limit_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_grid_limit_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_grid_limit_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_grid_limit_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_grid_limit_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_grid_limit_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_temperature_prompt
	ui->EVSE_Screen_temperature_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_temperature_prompt, "Temperature [C]");
	lv_label_set_long_mode(ui->EVSE_Screen_temperature_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_temperature_prompt, 237, 207);
	lv_obj_set_size(ui->EVSE_Screen_temperature_prompt, 128, 24);

	//Write style for EVSE_Screen_temperature_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_temperature_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_temperature_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_temperature_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_temperature_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_temperature_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_temperature_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_temperature_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_power_rate_value
	ui->EVSE_Screen_power_rate_value = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_power_rate_value, "0");
	lv_label_set_long_mode(ui->EVSE_Screen_power_rate_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_power_rate_value, 365, 126);
	lv_obj_set_size(ui->EVSE_Screen_power_rate_value, 90, 24);

	//Write style for EVSE_Screen_power_rate_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_power_rate_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_power_rate_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_power_rate_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_power_rate_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_power_rate_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_power_rate_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_power_rate_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_power_rate_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_power_rate_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_power_rate_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_power_rate_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_power_rate_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen_power_rate_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen_power_rate_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_power_rate_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_power_rate_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_power_rate_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_power_rate_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_power_rate_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_sigboard_versions_value
	ui->EVSE_Screen_sigboard_versions_value = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_sigboard_versions_value, "");
	lv_label_set_long_mode(ui->EVSE_Screen_sigboard_versions_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_sigboard_versions_value, 133, 169);
	lv_obj_set_size(ui->EVSE_Screen_sigboard_versions_value, 90, 24);

	//Write style for EVSE_Screen_sigboard_versions_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_sigboard_versions_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_sigboard_versions_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_sigboard_versions_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_sigboard_versions_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_sigboard_versions_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_sigboard_versions_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_sigboard_versions_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_sigboard_versions_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_sigboard_versions_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_sigboard_versions_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_sigboard_versions_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_sigboard_versions_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen_sigboard_versions_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen_sigboard_versions_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_sigboard_versions_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_sigboard_versions_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_sigboard_versions_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_sigboard_versions_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_sigboard_versions_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_charge_cost_value
	ui->EVSE_Screen_charge_cost_value = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_charge_cost_value, "2.54");
	lv_label_set_long_mode(ui->EVSE_Screen_charge_cost_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_charge_cost_value, 365, 90);
	lv_obj_set_size(ui->EVSE_Screen_charge_cost_value, 90, 24);

	//Write style for EVSE_Screen_charge_cost_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_charge_cost_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_charge_cost_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_charge_cost_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_charge_cost_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_charge_cost_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_charge_cost_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_charge_cost_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_charge_cost_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_charge_cost_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_charge_cost_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_charge_cost_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_charge_cost_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen_charge_cost_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen_charge_cost_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_charge_cost_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_charge_cost_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_charge_cost_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_charge_cost_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_charge_cost_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_EVSE_ID_value
	ui->EVSE_Screen_EVSE_ID_value = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_EVSE_ID_value, "xx");
	lv_label_set_long_mode(ui->EVSE_Screen_EVSE_ID_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_EVSE_ID_value, 133, 90);
	lv_obj_set_size(ui->EVSE_Screen_EVSE_ID_value, 90, 24);

	//Write style for EVSE_Screen_EVSE_ID_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_EVSE_ID_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_EVSE_ID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_EVSE_ID_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_EVSE_ID_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_EVSE_ID_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_EVSE_ID_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_EVSE_ID_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_EVSE_ID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_EVSE_ID_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_EVSE_ID_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_EVSE_ID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen_EVSE_ID_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen_EVSE_ID_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_EVSE_ID_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_grid_limit_value
	ui->EVSE_Screen_grid_limit_value = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_grid_limit_value, "0");
	lv_label_set_long_mode(ui->EVSE_Screen_grid_limit_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_grid_limit_value, 365, 169);
	lv_obj_set_size(ui->EVSE_Screen_grid_limit_value, 90, 24);

	//Write style for EVSE_Screen_grid_limit_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_grid_limit_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_grid_limit_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_grid_limit_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_grid_limit_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_grid_limit_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_grid_limit_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_grid_limit_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_grid_limit_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_grid_limit_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_grid_limit_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_grid_limit_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_grid_limit_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen_grid_limit_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen_grid_limit_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_grid_limit_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_grid_limit_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_grid_limit_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_grid_limit_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_grid_limit_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_temperature_value
	ui->EVSE_Screen_temperature_value = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_temperature_value, "0");
	lv_label_set_long_mode(ui->EVSE_Screen_temperature_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_temperature_value, 365, 210);
	lv_obj_set_size(ui->EVSE_Screen_temperature_value, 90, 24);

	//Write style for EVSE_Screen_temperature_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_temperature_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_temperature_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_temperature_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_temperature_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_temperature_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_temperature_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_temperature_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_temperature_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_temperature_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_temperature_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_temperature_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen_temperature_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen_temperature_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_temperature_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_temperature_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_blue_line_top
	ui->EVSE_Screen_blue_line_top = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_blue_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->EVSE_Screen_blue_line_top, EVSE_Screen_blue_line_top, 2);
	lv_obj_set_pos(ui->EVSE_Screen_blue_line_top, 5, 2);
	lv_obj_set_size(ui->EVSE_Screen_blue_line_top, 470, 1);

	//Write style for EVSE_Screen_blue_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_blue_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_blue_line_top, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_blue_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_blue_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_blue_line_bottom
	ui->EVSE_Screen_blue_line_bottom = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_blue_line_bottom[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->EVSE_Screen_blue_line_bottom, EVSE_Screen_blue_line_bottom, 2);
	lv_obj_set_pos(ui->EVSE_Screen_blue_line_bottom, 3, 269);
	lv_obj_set_size(ui->EVSE_Screen_blue_line_bottom, 474, 1);

	//Write style for EVSE_Screen_blue_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_blue_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_blue_line_bottom, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_blue_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_blue_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_blue_line_left
	ui->EVSE_Screen_blue_line_left = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_blue_line_left[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->EVSE_Screen_blue_line_left, EVSE_Screen_blue_line_left, 2);
	lv_obj_set_pos(ui->EVSE_Screen_blue_line_left, 3, 2);
	lv_obj_set_size(ui->EVSE_Screen_blue_line_left, 2, 268);

	//Write style for EVSE_Screen_blue_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_blue_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_blue_line_left, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_blue_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_blue_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_blue_line_right
	ui->EVSE_Screen_blue_line_right = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_blue_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->EVSE_Screen_blue_line_right, EVSE_Screen_blue_line_right, 2);
	lv_obj_set_pos(ui->EVSE_Screen_blue_line_right, 477, 3);
	lv_obj_set_size(ui->EVSE_Screen_blue_line_right, 1, 264);

	//Write style for EVSE_Screen_blue_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_blue_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_blue_line_right, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_blue_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_blue_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_green_line_top
	ui->EVSE_Screen_green_line_top = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_green_line_top[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->EVSE_Screen_green_line_top, EVSE_Screen_green_line_top, 2);
	lv_obj_set_pos(ui->EVSE_Screen_green_line_top, 6, 4);
	lv_obj_set_size(ui->EVSE_Screen_green_line_top, 468, 1);

	//Write style for EVSE_Screen_green_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_green_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_green_line_top, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_green_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_green_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_green_line_bottom
	ui->EVSE_Screen_green_line_bottom = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_green_line_bottom[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->EVSE_Screen_green_line_bottom, EVSE_Screen_green_line_bottom, 2);
	lv_obj_set_pos(ui->EVSE_Screen_green_line_bottom, 5, 267);
	lv_obj_set_size(ui->EVSE_Screen_green_line_bottom, 469, 1);

	//Write style for EVSE_Screen_green_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_green_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_green_line_bottom, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_green_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_green_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_green_line_left
	ui->EVSE_Screen_green_line_left = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_green_line_left[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->EVSE_Screen_green_line_left, EVSE_Screen_green_line_left, 2);
	lv_obj_set_pos(ui->EVSE_Screen_green_line_left, 5, 4);
	lv_obj_set_size(ui->EVSE_Screen_green_line_left, 1, 262);

	//Write style for EVSE_Screen_green_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_green_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_green_line_left, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_green_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_green_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_green_line_right
	ui->EVSE_Screen_green_line_right = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_green_line_right[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->EVSE_Screen_green_line_right, EVSE_Screen_green_line_right, 2);
	lv_obj_set_pos(ui->EVSE_Screen_green_line_right, 475, 4);
	lv_obj_set_size(ui->EVSE_Screen_green_line_right, 1, 260);

	//Write style for EVSE_Screen_green_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_green_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_green_line_right, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_green_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_green_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_orange_line_top
	ui->EVSE_Screen_orange_line_top = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_orange_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->EVSE_Screen_orange_line_top, EVSE_Screen_orange_line_top, 2);
	lv_obj_set_pos(ui->EVSE_Screen_orange_line_top, 1, 1);
	lv_obj_set_size(ui->EVSE_Screen_orange_line_top, 476, 1);

	//Write style for EVSE_Screen_orange_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_orange_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_orange_line_top, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_orange_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_orange_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_charge_state_prompt
	ui->EVSE_Screen_charge_state_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_charge_state_prompt, "Charge State");
	lv_label_set_long_mode(ui->EVSE_Screen_charge_state_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_charge_state_prompt, 20, 205);
	lv_obj_set_size(ui->EVSE_Screen_charge_state_prompt, 98, 25);

	//Write style for EVSE_Screen_charge_state_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_charge_state_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_charge_state_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_charge_state_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_charge_state_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_charge_state_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_charge_state_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_charge_state_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_orange_line_bottom
	ui->EVSE_Screen_orange_line_bottom = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_orange_line_bottom[] ={{0, 0},{478, 0},};
	lv_line_set_points(ui->EVSE_Screen_orange_line_bottom, EVSE_Screen_orange_line_bottom, 2);
	lv_obj_set_pos(ui->EVSE_Screen_orange_line_bottom, 1, 271);
	lv_obj_set_size(ui->EVSE_Screen_orange_line_bottom, 479, 1);

	//Write style for EVSE_Screen_orange_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_orange_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_orange_line_bottom, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_orange_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_orange_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_orange_line_left
	ui->EVSE_Screen_orange_line_left = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_orange_line_left[] ={{0, 0},{0, 270},};
	lv_line_set_points(ui->EVSE_Screen_orange_line_left, EVSE_Screen_orange_line_left, 2);
	lv_obj_set_pos(ui->EVSE_Screen_orange_line_left, 1, 2);
	lv_obj_set_size(ui->EVSE_Screen_orange_line_left, 1, 268);

	//Write style for EVSE_Screen_orange_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_orange_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_orange_line_left, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_orange_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_orange_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_orange_line_right
	ui->EVSE_Screen_orange_line_right = lv_line_create(ui->EVSE_Screen);
	static lv_point_t EVSE_Screen_orange_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->EVSE_Screen_orange_line_right, EVSE_Screen_orange_line_right, 2);
	lv_obj_set_pos(ui->EVSE_Screen_orange_line_right, 479, 1);
	lv_obj_set_size(ui->EVSE_Screen_orange_line_right, 1, 268);

	//Write style for EVSE_Screen_orange_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->EVSE_Screen_orange_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->EVSE_Screen_orange_line_right, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->EVSE_Screen_orange_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->EVSE_Screen_orange_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_charge_state_led
	ui->EVSE_Screen_charge_state_led = lv_led_create(ui->EVSE_Screen);
	lv_led_set_brightness(ui->EVSE_Screen_charge_state_led, 154);
	lv_led_set_color(ui->EVSE_Screen_charge_state_led, lv_color_hex(0x92c353));
	lv_obj_set_pos(ui->EVSE_Screen_charge_state_led, 168, 210);
	lv_obj_set_size(ui->EVSE_Screen_charge_state_led, 20, 20);

	//Write codes EVSE_Screen_up_time_value
	ui->EVSE_Screen_up_time_value = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_up_time_value, "0");
	lv_label_set_long_mode(ui->EVSE_Screen_up_time_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_up_time_value, 133, 126);
	lv_obj_set_size(ui->EVSE_Screen_up_time_value, 90, 24);

	//Write style for EVSE_Screen_up_time_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_up_time_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->EVSE_Screen_up_time_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->EVSE_Screen_up_time_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->EVSE_Screen_up_time_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_up_time_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_up_time_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_up_time_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_up_time_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_up_time_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_up_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_up_time_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_up_time_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->EVSE_Screen_up_time_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->EVSE_Screen_up_time_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_up_time_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_up_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_up_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_up_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_up_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_up_time_prompt
	ui->EVSE_Screen_up_time_prompt = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_up_time_prompt, "Station Uptime");
	lv_label_set_long_mode(ui->EVSE_Screen_up_time_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_up_time_prompt, 12, 126);
	lv_obj_set_size(ui->EVSE_Screen_up_time_prompt, 128, 24);

	//Write style for EVSE_Screen_up_time_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_up_time_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_up_time_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_up_time_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_up_time_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_up_time_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_up_time_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_up_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes EVSE_Screen_clock
	ui->EVSE_Screen_clock = lv_label_create(ui->EVSE_Screen);
	lv_label_set_text(ui->EVSE_Screen_clock, "0:00");
	lv_label_set_long_mode(ui->EVSE_Screen_clock, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->EVSE_Screen_clock, 15, 17);
	lv_obj_set_size(ui->EVSE_Screen_clock, 87, 24);

	//Write style for EVSE_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->EVSE_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->EVSE_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->EVSE_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->EVSE_Screen_clock, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->EVSE_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->EVSE_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of EVSE_Screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->EVSE_Screen);

	//Init events for screen.
	events_init_EVSE_Screen(ui);
}

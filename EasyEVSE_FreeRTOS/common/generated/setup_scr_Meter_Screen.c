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



void setup_scr_Meter_Screen(lv_ui *ui)
{
	//Write codes Meter_Screen
	ui->Meter_Screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->Meter_Screen, 480, 272);
	lv_obj_set_scrollbar_mode(ui->Meter_Screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for Meter_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Meter_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_Meter_Menu_Heading
	ui->Meter_Screen_Meter_Menu_Heading = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_Meter_Menu_Heading, "Meter Menu");
	lv_label_set_long_mode(ui->Meter_Screen_Meter_Menu_Heading, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_Meter_Menu_Heading, 177, 17);
	lv_obj_set_size(ui->Meter_Screen_Meter_Menu_Heading, 158, 24);

	//Write style for Meter_Screen_Meter_Menu_Heading, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_Meter_Menu_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_Meter_Menu_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_Meter_Menu_Heading, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_Meter_Menu_Heading, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_Meter_Menu_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_Meter_Menu_Heading, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_Meter_Menu_Heading, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_Meter_Menu_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_Meter_Menu_Heading, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_Meter_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_Meter_Menu_Heading, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_Meter_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_Meter_Menu_Heading, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_Meter_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_Meter_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_Meter_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_Meter_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_V_RMS_prompt
	ui->Meter_Screen_V_RMS_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_V_RMS_prompt, "V RMS");
	lv_label_set_long_mode(ui->Meter_Screen_V_RMS_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_V_RMS_prompt, 50, 179);
	lv_obj_set_size(ui->Meter_Screen_V_RMS_prompt, 80, 24);

	//Write style for Meter_Screen_V_RMS_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_V_RMS_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_V_RMS_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_V_RMS_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_V_RMS_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_V_RMS_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_V_RMS_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_V_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_V_RMS_value
	ui->Meter_Screen_V_RMS_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_V_RMS_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_V_RMS_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_V_RMS_value, 123, 178);
	lv_obj_set_size(ui->Meter_Screen_V_RMS_value, 94, 24);

	//Write style for Meter_Screen_V_RMS_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_V_RMS_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_V_RMS_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_V_RMS_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_V_RMS_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_V_RMS_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_V_RMS_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_V_RMS_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_V_RMS_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_V_RMS_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_V_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_V_RMS_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_V_RMS_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_V_RMS_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_V_RMS_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_V_RMS_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_V_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_V_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_V_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_V_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_aparent_prompt
	ui->Meter_Screen_aparent_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_aparent_prompt, "Apparent (S)");
	lv_label_set_long_mode(ui->Meter_Screen_aparent_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_aparent_prompt, 234, 179);
	lv_obj_set_size(ui->Meter_Screen_aparent_prompt, 97, 24);

	//Write style for Meter_Screen_aparent_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_aparent_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_aparent_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_aparent_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_aparent_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_aparent_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_aparent_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_aparent_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_aparent_value
	ui->Meter_Screen_aparent_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_aparent_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_aparent_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_aparent_value, 336, 178);
	lv_obj_set_size(ui->Meter_Screen_aparent_value, 94, 24);

	//Write style for Meter_Screen_aparent_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_aparent_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_aparent_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_aparent_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_aparent_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_aparent_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_aparent_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_aparent_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_aparent_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_aparent_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_aparent_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_aparent_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_aparent_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_aparent_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_aparent_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_aparent_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_aparent_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_aparent_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_aparent_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_aparent_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_active_value
	ui->Meter_Screen_active_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_active_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_active_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_active_value, 335, 138);
	lv_obj_set_size(ui->Meter_Screen_active_value, 94, 24);

	//Write style for Meter_Screen_active_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_active_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_active_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_active_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_active_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_active_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_active_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_active_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_active_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_active_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_active_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_active_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_active_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_active_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_active_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_active_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_active_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_active_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_active_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_active_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_active_prompt
	ui->Meter_Screen_active_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_active_prompt, "Active (P)");
	lv_label_set_long_mode(ui->Meter_Screen_active_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_active_prompt, 239, 137);
	lv_obj_set_size(ui->Meter_Screen_active_prompt, 112, 24);

	//Write style for Meter_Screen_active_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_active_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_active_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_active_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_active_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_active_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_active_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_active_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_I_RMS_value
	ui->Meter_Screen_I_RMS_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_I_RMS_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_I_RMS_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_I_RMS_value, 122, 139);
	lv_obj_set_size(ui->Meter_Screen_I_RMS_value, 94, 24);

	//Write style for Meter_Screen_I_RMS_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_I_RMS_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_I_RMS_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_I_RMS_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_I_RMS_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_I_RMS_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_I_RMS_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_I_RMS_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_I_RMS_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_I_RMS_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_I_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_I_RMS_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_I_RMS_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_I_RMS_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_I_RMS_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_I_RMS_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_I_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_I_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_I_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_I_RMS_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_I_RMS_prompt
	ui->Meter_Screen_I_RMS_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_I_RMS_prompt, "I RMS");
	lv_label_set_long_mode(ui->Meter_Screen_I_RMS_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_I_RMS_prompt, 50, 139);
	lv_obj_set_size(ui->Meter_Screen_I_RMS_prompt, 80, 24);

	//Write style for Meter_Screen_I_RMS_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_I_RMS_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_I_RMS_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_I_RMS_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_I_RMS_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_I_RMS_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_I_RMS_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_I_RMS_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_VARh_value
	ui->Meter_Screen_VARh_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_VARh_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_VARh_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_VARh_value, 122, 102);
	lv_obj_set_size(ui->Meter_Screen_VARh_value, 94, 24);

	//Write style for Meter_Screen_VARh_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_VARh_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_VARh_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_VARh_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_VARh_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_VARh_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_VARh_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_VARh_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_VARh_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_VARh_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_VARh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_VARh_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_VARh_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_VARh_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_VARh_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_VARh_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_VARh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_VARh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_VARh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_VARh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_VARh_prompt
	ui->Meter_Screen_VARh_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_VARh_prompt, "VARh");
	lv_label_set_long_mode(ui->Meter_Screen_VARh_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_VARh_prompt, 53, 102);
	lv_obj_set_size(ui->Meter_Screen_VARh_prompt, 80, 24);

	//Write style for Meter_Screen_VARh_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_VARh_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_VARh_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_VARh_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_VARh_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_VARh_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_VARh_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_VARh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_kWh_prompt
	ui->Meter_Screen_kWh_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_kWh_prompt, "kWh");
	lv_label_set_long_mode(ui->Meter_Screen_kWh_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_kWh_prompt, 269, 67);
	lv_obj_set_size(ui->Meter_Screen_kWh_prompt, 80, 24);

	//Write style for Meter_Screen_kWh_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_kWh_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_kWh_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_kWh_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_kWh_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_kWh_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_kWh_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_kWh_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_reactive_prompt
	ui->Meter_Screen_reactive_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_reactive_prompt, "Reactive (Q)");
	lv_label_set_long_mode(ui->Meter_Screen_reactive_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_reactive_prompt, 237, 100);
	lv_obj_set_size(ui->Meter_Screen_reactive_prompt, 97, 24);

	//Write style for Meter_Screen_reactive_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_reactive_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_reactive_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_reactive_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_reactive_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_reactive_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_reactive_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_reactive_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_reactive_value
	ui->Meter_Screen_reactive_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_reactive_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_reactive_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_reactive_value, 335, 102);
	lv_obj_set_size(ui->Meter_Screen_reactive_value, 94, 24);

	//Write style for Meter_Screen_reactive_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_reactive_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_reactive_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_reactive_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_reactive_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_reactive_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_reactive_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_reactive_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_reactive_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_reactive_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_reactive_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_reactive_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_reactive_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_reactive_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_reactive_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_reactive_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_reactive_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_reactive_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_reactive_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_reactive_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_kWh_value
	ui->Meter_Screen_kWh_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_kWh_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_kWh_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_kWh_value, 335, 63);
	lv_obj_set_size(ui->Meter_Screen_kWh_value, 94, 24);

	//Write style for Meter_Screen_kWh_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_kWh_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_kWh_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_kWh_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_kWh_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_kWh_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_kWh_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_kWh_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_kWh_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_kWh_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_kWh_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_kWh_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_kWh_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_kWh_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_kWh_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_mode_value
	ui->Meter_Screen_mode_value = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_mode_value, "Measure");
	lv_label_set_long_mode(ui->Meter_Screen_mode_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_mode_value, 121, 64);
	lv_obj_set_size(ui->Meter_Screen_mode_value, 94, 24);

	//Write style for Meter_Screen_mode_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_mode_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Meter_Screen_mode_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Meter_Screen_mode_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Meter_Screen_mode_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_mode_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_mode_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_mode_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_mode_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_mode_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_mode_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_mode_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Meter_Screen_mode_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Meter_Screen_mode_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_mode_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_mode_prompt
	ui->Meter_Screen_mode_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_mode_prompt, "Mode");
	lv_label_set_long_mode(ui->Meter_Screen_mode_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_mode_prompt, 51, 66);
	lv_obj_set_size(ui->Meter_Screen_mode_prompt, 80, 24);

	//Write style for Meter_Screen_mode_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_mode_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_mode_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_mode_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_mode_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_mode_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_mode_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_ID_meter_data_btn
	ui->Meter_Screen_ID_meter_data_btn = lv_imgbtn_create(ui->Meter_Screen);
	lv_obj_add_flag(ui->Meter_Screen_ID_meter_data_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_meter_data_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_readdata_alpha_35x37, NULL);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_meter_data_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_readdata_alpha_35x37, NULL);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_meter_data_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_readdata_alpha_35x37, NULL);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_meter_data_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_readdata_alpha_35x37, NULL);
	ui->Meter_Screen_ID_meter_data_btn_label = lv_label_create(ui->Meter_Screen_ID_meter_data_btn);
	lv_label_set_text(ui->Meter_Screen_ID_meter_data_btn_label, "");
	lv_label_set_long_mode(ui->Meter_Screen_ID_meter_data_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Meter_Screen_ID_meter_data_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Meter_Screen_ID_meter_data_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Meter_Screen_ID_meter_data_btn, 236, 215);
	lv_obj_set_size(ui->Meter_Screen_ID_meter_data_btn, 35, 37);
	lv_obj_add_flag(ui->Meter_Screen_ID_meter_data_btn, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->Meter_Screen_ID_meter_data_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	//Write style for Meter_Screen_ID_meter_data_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Meter_Screen_ID_meter_data_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_ID_meter_data_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_ID_meter_data_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_ID_meter_data_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_ID_meter_data_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Meter_Screen_ID_meter_data_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Meter_Screen_ID_meter_data_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Meter_Screen_ID_meter_data_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Meter_Screen_ID_meter_data_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Meter_Screen_ID_meter_data_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_ID_meter_data_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Meter_Screen_ID_meter_data_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Meter_Screen_ID_meter_data_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Meter_Screen_ID_meter_data_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Meter_Screen_ID_meter_data_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Meter_Screen_ID_meter_data_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_ID_meter_data_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Meter_Screen_ID_meter_data_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Meter_Screen_ID_meter_data_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Meter_Screen_refresh_data_prompt
	ui->Meter_Screen_refresh_data_prompt = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_refresh_data_prompt, "Refresh data");
	lv_label_set_long_mode(ui->Meter_Screen_refresh_data_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_refresh_data_prompt, 102, 224);
	lv_obj_set_size(ui->Meter_Screen_refresh_data_prompt, 120, 20);

	//Write style for Meter_Screen_refresh_data_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_refresh_data_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_refresh_data_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_refresh_data_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_refresh_data_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_refresh_data_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_refresh_data_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_refresh_data_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_blue_line_top
	ui->Meter_Screen_blue_line_top = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_blue_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Meter_Screen_blue_line_top, Meter_Screen_blue_line_top, 2);
	lv_obj_set_pos(ui->Meter_Screen_blue_line_top, 5, 2);
	lv_obj_set_size(ui->Meter_Screen_blue_line_top, 470, 1);

	//Write style for Meter_Screen_blue_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_blue_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_blue_line_top, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_blue_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_blue_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_blue_line_bottom
	ui->Meter_Screen_blue_line_bottom = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_blue_line_bottom[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Meter_Screen_blue_line_bottom, Meter_Screen_blue_line_bottom, 2);
	lv_obj_set_pos(ui->Meter_Screen_blue_line_bottom, 3, 269);
	lv_obj_set_size(ui->Meter_Screen_blue_line_bottom, 474, 1);

	//Write style for Meter_Screen_blue_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_blue_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_blue_line_bottom, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_blue_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_blue_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_blue_line_left
	ui->Meter_Screen_blue_line_left = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_blue_line_left[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Meter_Screen_blue_line_left, Meter_Screen_blue_line_left, 2);
	lv_obj_set_pos(ui->Meter_Screen_blue_line_left, 3, 2);
	lv_obj_set_size(ui->Meter_Screen_blue_line_left, 2, 268);

	//Write style for Meter_Screen_blue_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_blue_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_blue_line_left, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_blue_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_blue_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_blue_line_right
	ui->Meter_Screen_blue_line_right = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_blue_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Meter_Screen_blue_line_right, Meter_Screen_blue_line_right, 2);
	lv_obj_set_pos(ui->Meter_Screen_blue_line_right, 477, 3);
	lv_obj_set_size(ui->Meter_Screen_blue_line_right, 1, 264);

	//Write style for Meter_Screen_blue_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_blue_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_blue_line_right, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_blue_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_blue_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_green_line_top
	ui->Meter_Screen_green_line_top = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_green_line_top[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->Meter_Screen_green_line_top, Meter_Screen_green_line_top, 2);
	lv_obj_set_pos(ui->Meter_Screen_green_line_top, 6, 4);
	lv_obj_set_size(ui->Meter_Screen_green_line_top, 468, 1);

	//Write style for Meter_Screen_green_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_green_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_green_line_top, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_green_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_green_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_green_line_bottom
	ui->Meter_Screen_green_line_bottom = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_green_line_bottom[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->Meter_Screen_green_line_bottom, Meter_Screen_green_line_bottom, 2);
	lv_obj_set_pos(ui->Meter_Screen_green_line_bottom, 5, 267);
	lv_obj_set_size(ui->Meter_Screen_green_line_bottom, 469, 1);

	//Write style for Meter_Screen_green_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_green_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_green_line_bottom, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_green_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_green_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_green_line_left
	ui->Meter_Screen_green_line_left = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_green_line_left[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->Meter_Screen_green_line_left, Meter_Screen_green_line_left, 2);
	lv_obj_set_pos(ui->Meter_Screen_green_line_left, 5, 4);
	lv_obj_set_size(ui->Meter_Screen_green_line_left, 1, 262);

	//Write style for Meter_Screen_green_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_green_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_green_line_left, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_green_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_green_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_green_line_right
	ui->Meter_Screen_green_line_right = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_green_line_right[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->Meter_Screen_green_line_right, Meter_Screen_green_line_right, 2);
	lv_obj_set_pos(ui->Meter_Screen_green_line_right, 475, 4);
	lv_obj_set_size(ui->Meter_Screen_green_line_right, 1, 260);

	//Write style for Meter_Screen_green_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_green_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_green_line_right, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_green_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_green_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_orange_line_top
	ui->Meter_Screen_orange_line_top = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_orange_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Meter_Screen_orange_line_top, Meter_Screen_orange_line_top, 2);
	lv_obj_set_pos(ui->Meter_Screen_orange_line_top, 1, 1);
	lv_obj_set_size(ui->Meter_Screen_orange_line_top, 476, 1);

	//Write style for Meter_Screen_orange_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_orange_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_orange_line_top, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_orange_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_orange_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_orange_line_bottom
	ui->Meter_Screen_orange_line_bottom = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_orange_line_bottom[] ={{0, 0},{478, 0},};
	lv_line_set_points(ui->Meter_Screen_orange_line_bottom, Meter_Screen_orange_line_bottom, 2);
	lv_obj_set_pos(ui->Meter_Screen_orange_line_bottom, 1, 271);
	lv_obj_set_size(ui->Meter_Screen_orange_line_bottom, 479, 1);

	//Write style for Meter_Screen_orange_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_orange_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_orange_line_bottom, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_orange_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_orange_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_orange_line_left
	ui->Meter_Screen_orange_line_left = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_orange_line_left[] ={{0, 0},{0, 270},};
	lv_line_set_points(ui->Meter_Screen_orange_line_left, Meter_Screen_orange_line_left, 2);
	lv_obj_set_pos(ui->Meter_Screen_orange_line_left, 1, 2);
	lv_obj_set_size(ui->Meter_Screen_orange_line_left, 1, 268);

	//Write style for Meter_Screen_orange_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_orange_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_orange_line_left, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_orange_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_orange_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_orange_line_right
	ui->Meter_Screen_orange_line_right = lv_line_create(ui->Meter_Screen);
	static lv_point_t Meter_Screen_orange_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Meter_Screen_orange_line_right, Meter_Screen_orange_line_right, 2);
	lv_obj_set_pos(ui->Meter_Screen_orange_line_right, 479, 1);
	lv_obj_set_size(ui->Meter_Screen_orange_line_right, 1, 268);

	//Write style for Meter_Screen_orange_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Meter_Screen_orange_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Meter_Screen_orange_line_right, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Meter_Screen_orange_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Meter_Screen_orange_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Meter_Screen_ID_back_btn
	ui->Meter_Screen_ID_back_btn = lv_imgbtn_create(ui->Meter_Screen);
	lv_obj_add_flag(ui->Meter_Screen_ID_back_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_back_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_back_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->Meter_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	ui->Meter_Screen_ID_back_btn_label = lv_label_create(ui->Meter_Screen_ID_back_btn);
	lv_label_set_text(ui->Meter_Screen_ID_back_btn_label, "");
	lv_label_set_long_mode(ui->Meter_Screen_ID_back_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Meter_Screen_ID_back_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Meter_Screen_ID_back_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Meter_Screen_ID_back_btn, 430, 15);
	lv_obj_set_size(ui->Meter_Screen_ID_back_btn, 32, 28);
	lv_obj_add_flag(ui->Meter_Screen_ID_back_btn, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->Meter_Screen_ID_back_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	//Write style for Meter_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Meter_Screen_ID_back_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_ID_back_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Meter_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Meter_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Meter_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Meter_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Meter_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Meter_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Meter_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Meter_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Meter_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Meter_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Meter_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Meter_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Meter_Screen_clock
	ui->Meter_Screen_clock = lv_label_create(ui->Meter_Screen);
	lv_label_set_text(ui->Meter_Screen_clock, "0:00");
	lv_label_set_long_mode(ui->Meter_Screen_clock, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Meter_Screen_clock, 15, 17);
	lv_obj_set_size(ui->Meter_Screen_clock, 87, 24);

	//Write style for Meter_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Meter_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Meter_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Meter_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Meter_Screen_clock, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Meter_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Meter_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of Meter_Screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->Meter_Screen);

	//Init events for screen.
	events_init_Meter_Screen(ui);
}

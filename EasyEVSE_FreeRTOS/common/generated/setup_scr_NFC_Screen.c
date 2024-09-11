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



void setup_scr_NFC_Screen(lv_ui *ui)
{
	//Write codes NFC_Screen
	ui->NFC_Screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->NFC_Screen, 480, 272);
	lv_obj_set_scrollbar_mode(ui->NFC_Screen, LV_SCROLLBAR_MODE_OFF);
	lv_obj_add_flag(ui->NFC_Screen, LV_OBJ_FLAG_PRESS_LOCK);

	//Write style for NFC_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->NFC_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->NFC_Screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->NFC_Screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_NFC_Menu_Heading
	ui->NFC_Screen_NFC_Menu_Heading = lv_label_create(ui->NFC_Screen);
	lv_label_set_text(ui->NFC_Screen_NFC_Menu_Heading, "NFC Menu");
	lv_label_set_long_mode(ui->NFC_Screen_NFC_Menu_Heading, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->NFC_Screen_NFC_Menu_Heading, 174, 27);
	lv_obj_set_size(ui->NFC_Screen_NFC_Menu_Heading, 164, 24);

	//Write style for NFC_Screen_NFC_Menu_Heading, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->NFC_Screen_NFC_Menu_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->NFC_Screen_NFC_Menu_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->NFC_Screen_NFC_Menu_Heading, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->NFC_Screen_NFC_Menu_Heading, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->NFC_Screen_NFC_Menu_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->NFC_Screen_NFC_Menu_Heading, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->NFC_Screen_NFC_Menu_Heading, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->NFC_Screen_NFC_Menu_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->NFC_Screen_NFC_Menu_Heading, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->NFC_Screen_NFC_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->NFC_Screen_NFC_Menu_Heading, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->NFC_Screen_NFC_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->NFC_Screen_NFC_Menu_Heading, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->NFC_Screen_NFC_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->NFC_Screen_NFC_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->NFC_Screen_NFC_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_NFC_Menu_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_Read_UID_prompt
	ui->NFC_Screen_Read_UID_prompt = lv_label_create(ui->NFC_Screen);
	lv_label_set_text(ui->NFC_Screen_Read_UID_prompt, "Read UID");
	lv_label_set_long_mode(ui->NFC_Screen_Read_UID_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->NFC_Screen_Read_UID_prompt, 63, 115);
	lv_obj_set_size(ui->NFC_Screen_Read_UID_prompt, 157, 24);

	//Write style for NFC_Screen_Read_UID_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->NFC_Screen_Read_UID_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->NFC_Screen_Read_UID_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->NFC_Screen_Read_UID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->NFC_Screen_Read_UID_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->NFC_Screen_Read_UID_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->NFC_Screen_Read_UID_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_Read_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_card_UID_prompt
	ui->NFC_Screen_card_UID_prompt = lv_label_create(ui->NFC_Screen);
	lv_label_set_text(ui->NFC_Screen_card_UID_prompt, "Card UID");
	lv_label_set_long_mode(ui->NFC_Screen_card_UID_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->NFC_Screen_card_UID_prompt, 61, 189);
	lv_obj_set_size(ui->NFC_Screen_card_UID_prompt, 157, 24);

	//Write style for NFC_Screen_card_UID_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->NFC_Screen_card_UID_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->NFC_Screen_card_UID_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->NFC_Screen_card_UID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->NFC_Screen_card_UID_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->NFC_Screen_card_UID_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->NFC_Screen_card_UID_prompt, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_card_UID_value
	ui->NFC_Screen_card_UID_value = lv_label_create(ui->NFC_Screen);
	lv_label_set_text(ui->NFC_Screen_card_UID_value, "N/A");
	lv_label_set_long_mode(ui->NFC_Screen_card_UID_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->NFC_Screen_card_UID_value, 199, 185);
	lv_obj_set_size(ui->NFC_Screen_card_UID_value, 181, 24);

	//Write style for NFC_Screen_card_UID_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->NFC_Screen_card_UID_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->NFC_Screen_card_UID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->NFC_Screen_card_UID_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->NFC_Screen_card_UID_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->NFC_Screen_card_UID_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->NFC_Screen_card_UID_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->NFC_Screen_card_UID_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->NFC_Screen_card_UID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->NFC_Screen_card_UID_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->NFC_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->NFC_Screen_card_UID_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->NFC_Screen_card_UID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->NFC_Screen_card_UID_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->NFC_Screen_card_UID_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->NFC_Screen_card_UID_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->NFC_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->NFC_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->NFC_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_read_UID_btn
	ui->NFC_Screen_read_UID_btn = lv_imgbtn_create(ui->NFC_Screen);
	lv_obj_add_flag(ui->NFC_Screen_read_UID_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->NFC_Screen_read_UID_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_readnfc_alpha_91x63, NULL);
	lv_imgbtn_set_src(ui->NFC_Screen_read_UID_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_readnfc_alpha_91x63, NULL);
	lv_imgbtn_set_src(ui->NFC_Screen_read_UID_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_readnfc_alpha_91x63, NULL);
	lv_imgbtn_set_src(ui->NFC_Screen_read_UID_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_readnfc_alpha_91x63, NULL);
	ui->NFC_Screen_read_UID_btn_label = lv_label_create(ui->NFC_Screen_read_UID_btn);
	lv_label_set_text(ui->NFC_Screen_read_UID_btn_label, "");
	lv_label_set_long_mode(ui->NFC_Screen_read_UID_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->NFC_Screen_read_UID_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->NFC_Screen_read_UID_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->NFC_Screen_read_UID_btn, 219, 101);
	lv_obj_set_size(ui->NFC_Screen_read_UID_btn, 91, 63);
	lv_obj_add_flag(ui->NFC_Screen_read_UID_btn, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->NFC_Screen_read_UID_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	//Write style for NFC_Screen_read_UID_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->NFC_Screen_read_UID_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->NFC_Screen_read_UID_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->NFC_Screen_read_UID_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->NFC_Screen_read_UID_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_read_UID_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for NFC_Screen_read_UID_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->NFC_Screen_read_UID_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->NFC_Screen_read_UID_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->NFC_Screen_read_UID_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->NFC_Screen_read_UID_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_read_UID_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for NFC_Screen_read_UID_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->NFC_Screen_read_UID_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->NFC_Screen_read_UID_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->NFC_Screen_read_UID_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->NFC_Screen_read_UID_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_read_UID_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for NFC_Screen_read_UID_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->NFC_Screen_read_UID_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes NFC_Screen_blue_line_top
	ui->NFC_Screen_blue_line_top = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_blue_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->NFC_Screen_blue_line_top, NFC_Screen_blue_line_top, 2);
	lv_obj_set_pos(ui->NFC_Screen_blue_line_top, 5, 2);
	lv_obj_set_size(ui->NFC_Screen_blue_line_top, 470, 1);

	//Write style for NFC_Screen_blue_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_blue_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_blue_line_top, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_blue_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_blue_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_blue_line_bottom
	ui->NFC_Screen_blue_line_bottom = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_blue_line_bottom[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->NFC_Screen_blue_line_bottom, NFC_Screen_blue_line_bottom, 2);
	lv_obj_set_pos(ui->NFC_Screen_blue_line_bottom, 3, 269);
	lv_obj_set_size(ui->NFC_Screen_blue_line_bottom, 474, 1);

	//Write style for NFC_Screen_blue_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_blue_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_blue_line_bottom, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_blue_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_blue_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_blue_line_left
	ui->NFC_Screen_blue_line_left = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_blue_line_left[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->NFC_Screen_blue_line_left, NFC_Screen_blue_line_left, 2);
	lv_obj_set_pos(ui->NFC_Screen_blue_line_left, 3, 2);
	lv_obj_set_size(ui->NFC_Screen_blue_line_left, 2, 268);

	//Write style for NFC_Screen_blue_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_blue_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_blue_line_left, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_blue_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_blue_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_blue_line_right
	ui->NFC_Screen_blue_line_right = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_blue_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->NFC_Screen_blue_line_right, NFC_Screen_blue_line_right, 2);
	lv_obj_set_pos(ui->NFC_Screen_blue_line_right, 477, 3);
	lv_obj_set_size(ui->NFC_Screen_blue_line_right, 1, 264);

	//Write style for NFC_Screen_blue_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_blue_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_blue_line_right, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_blue_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_blue_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_green_line_top
	ui->NFC_Screen_green_line_top = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_green_line_top[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->NFC_Screen_green_line_top, NFC_Screen_green_line_top, 2);
	lv_obj_set_pos(ui->NFC_Screen_green_line_top, 6, 4);
	lv_obj_set_size(ui->NFC_Screen_green_line_top, 468, 1);

	//Write style for NFC_Screen_green_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_green_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_green_line_top, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_green_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_green_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_green_line_bottom
	ui->NFC_Screen_green_line_bottom = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_green_line_bottom[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->NFC_Screen_green_line_bottom, NFC_Screen_green_line_bottom, 2);
	lv_obj_set_pos(ui->NFC_Screen_green_line_bottom, 5, 267);
	lv_obj_set_size(ui->NFC_Screen_green_line_bottom, 469, 1);

	//Write style for NFC_Screen_green_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_green_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_green_line_bottom, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_green_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_green_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_green_line_left
	ui->NFC_Screen_green_line_left = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_green_line_left[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->NFC_Screen_green_line_left, NFC_Screen_green_line_left, 2);
	lv_obj_set_pos(ui->NFC_Screen_green_line_left, 5, 4);
	lv_obj_set_size(ui->NFC_Screen_green_line_left, 1, 262);

	//Write style for NFC_Screen_green_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_green_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_green_line_left, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_green_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_green_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_green_line_right
	ui->NFC_Screen_green_line_right = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_green_line_right[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->NFC_Screen_green_line_right, NFC_Screen_green_line_right, 2);
	lv_obj_set_pos(ui->NFC_Screen_green_line_right, 475, 4);
	lv_obj_set_size(ui->NFC_Screen_green_line_right, 1, 260);

	//Write style for NFC_Screen_green_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_green_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_green_line_right, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_green_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_green_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_orange_line_top
	ui->NFC_Screen_orange_line_top = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_orange_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->NFC_Screen_orange_line_top, NFC_Screen_orange_line_top, 2);
	lv_obj_set_pos(ui->NFC_Screen_orange_line_top, 1, 1);
	lv_obj_set_size(ui->NFC_Screen_orange_line_top, 476, 1);

	//Write style for NFC_Screen_orange_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_orange_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_orange_line_top, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_orange_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_orange_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_orange_line_bottom
	ui->NFC_Screen_orange_line_bottom = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_orange_line_bottom[] ={{0, 0},{478, 0},};
	lv_line_set_points(ui->NFC_Screen_orange_line_bottom, NFC_Screen_orange_line_bottom, 2);
	lv_obj_set_pos(ui->NFC_Screen_orange_line_bottom, 1, 271);
	lv_obj_set_size(ui->NFC_Screen_orange_line_bottom, 479, 1);

	//Write style for NFC_Screen_orange_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_orange_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_orange_line_bottom, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_orange_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_orange_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_orange_line_left
	ui->NFC_Screen_orange_line_left = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_orange_line_left[] ={{0, 0},{0, 270},};
	lv_line_set_points(ui->NFC_Screen_orange_line_left, NFC_Screen_orange_line_left, 2);
	lv_obj_set_pos(ui->NFC_Screen_orange_line_left, 1, 2);
	lv_obj_set_size(ui->NFC_Screen_orange_line_left, 1, 268);

	//Write style for NFC_Screen_orange_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_orange_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_orange_line_left, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_orange_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_orange_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_orange_line_right
	ui->NFC_Screen_orange_line_right = lv_line_create(ui->NFC_Screen);
	static lv_point_t NFC_Screen_orange_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->NFC_Screen_orange_line_right, NFC_Screen_orange_line_right, 2);
	lv_obj_set_pos(ui->NFC_Screen_orange_line_right, 479, 1);
	lv_obj_set_size(ui->NFC_Screen_orange_line_right, 1, 268);

	//Write style for NFC_Screen_orange_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->NFC_Screen_orange_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->NFC_Screen_orange_line_right, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->NFC_Screen_orange_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->NFC_Screen_orange_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes NFC_Screen_ID_back_btn
	ui->NFC_Screen_ID_back_btn = lv_imgbtn_create(ui->NFC_Screen);
	lv_obj_add_flag(ui->NFC_Screen_ID_back_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->NFC_Screen_ID_back_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->NFC_Screen_ID_back_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->NFC_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->NFC_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	ui->NFC_Screen_ID_back_btn_label = lv_label_create(ui->NFC_Screen_ID_back_btn);
	lv_label_set_text(ui->NFC_Screen_ID_back_btn_label, "");
	lv_label_set_long_mode(ui->NFC_Screen_ID_back_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->NFC_Screen_ID_back_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->NFC_Screen_ID_back_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->NFC_Screen_ID_back_btn, 430, 15);
	lv_obj_set_size(ui->NFC_Screen_ID_back_btn, 32, 28);
	lv_obj_add_flag(ui->NFC_Screen_ID_back_btn, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->NFC_Screen_ID_back_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	//Write style for NFC_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->NFC_Screen_ID_back_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->NFC_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->NFC_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->NFC_Screen_ID_back_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for NFC_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->NFC_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->NFC_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->NFC_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->NFC_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for NFC_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->NFC_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->NFC_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->NFC_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->NFC_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for NFC_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->NFC_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes NFC_Screen_clock
	ui->NFC_Screen_clock = lv_label_create(ui->NFC_Screen);
	lv_label_set_text(ui->NFC_Screen_clock, "0:00");
	lv_label_set_long_mode(ui->NFC_Screen_clock, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->NFC_Screen_clock, 15, 17);
	lv_obj_set_size(ui->NFC_Screen_clock, 87, 24);

	//Write style for NFC_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->NFC_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->NFC_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->NFC_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->NFC_Screen_clock, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->NFC_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->NFC_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of NFC_Screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->NFC_Screen);

	//Init events for screen.
	events_init_NFC_Screen(ui);
}

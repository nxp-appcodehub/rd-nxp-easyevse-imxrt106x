/*
* Copyright 2024 NXP
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



void setup_scr_Debug_ISO_Screen(lv_ui *ui)
{
	//Write codes Debug_ISO_Screen
	ui->Debug_ISO_Screen = lv_obj_create(NULL);
	lv_obj_set_size(ui->Debug_ISO_Screen, 480, 272);
	lv_obj_set_scrollbar_mode(ui->Debug_ISO_Screen, LV_SCROLLBAR_MODE_OFF);

	//Write style for Debug_ISO_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Debug_ISO_Screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Debug_ISO_Screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_orange_line_right
	ui->Debug_ISO_Screen_orange_line_right = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_orange_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Debug_ISO_Screen_orange_line_right, Debug_ISO_Screen_orange_line_right, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_orange_line_right, 479, 1);
	lv_obj_set_size(ui->Debug_ISO_Screen_orange_line_right, 1, 268);

	//Write style for Debug_ISO_Screen_orange_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_orange_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_orange_line_right, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_orange_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_orange_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_orange_line_left
	ui->Debug_ISO_Screen_orange_line_left = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_orange_line_left[] ={{0, 0},{0, 270},};
	lv_line_set_points(ui->Debug_ISO_Screen_orange_line_left, Debug_ISO_Screen_orange_line_left, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_orange_line_left, 1, 2);
	lv_obj_set_size(ui->Debug_ISO_Screen_orange_line_left, 1, 268);

	//Write style for Debug_ISO_Screen_orange_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_orange_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_orange_line_left, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_orange_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_orange_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_orange_line_bottom
	ui->Debug_ISO_Screen_orange_line_bottom = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_orange_line_bottom[] ={{0, 0},{478, 0},};
	lv_line_set_points(ui->Debug_ISO_Screen_orange_line_bottom, Debug_ISO_Screen_orange_line_bottom, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_orange_line_bottom, 1, 271);
	lv_obj_set_size(ui->Debug_ISO_Screen_orange_line_bottom, 479, 1);

	//Write style for Debug_ISO_Screen_orange_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_orange_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_orange_line_bottom, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_orange_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_orange_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_orange_line_top
	ui->Debug_ISO_Screen_orange_line_top = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_orange_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Debug_ISO_Screen_orange_line_top, Debug_ISO_Screen_orange_line_top, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_orange_line_top, 1, 1);
	lv_obj_set_size(ui->Debug_ISO_Screen_orange_line_top, 476, 1);

	//Write style for Debug_ISO_Screen_orange_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_orange_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_orange_line_top, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_orange_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_orange_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_green_line_right
	ui->Debug_ISO_Screen_green_line_right = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_green_line_right[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->Debug_ISO_Screen_green_line_right, Debug_ISO_Screen_green_line_right, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_green_line_right, 475, 4);
	lv_obj_set_size(ui->Debug_ISO_Screen_green_line_right, 1, 260);

	//Write style for Debug_ISO_Screen_green_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_green_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_green_line_right, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_green_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_green_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_green_line_left
	ui->Debug_ISO_Screen_green_line_left = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_green_line_left[] ={{0, 0},{0, 264},};
	lv_line_set_points(ui->Debug_ISO_Screen_green_line_left, Debug_ISO_Screen_green_line_left, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_green_line_left, 5, 4);
	lv_obj_set_size(ui->Debug_ISO_Screen_green_line_left, 1, 262);

	//Write style for Debug_ISO_Screen_green_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_green_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_green_line_left, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_green_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_green_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_green_line_bottom
	ui->Debug_ISO_Screen_green_line_bottom = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_green_line_bottom[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->Debug_ISO_Screen_green_line_bottom, Debug_ISO_Screen_green_line_bottom, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_green_line_bottom, 5, 267);
	lv_obj_set_size(ui->Debug_ISO_Screen_green_line_bottom, 469, 1);

	//Write style for Debug_ISO_Screen_green_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_green_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_green_line_bottom, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_green_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_green_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_green_line_top
	ui->Debug_ISO_Screen_green_line_top = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_green_line_top[] ={{0, 0},{472, 0},};
	lv_line_set_points(ui->Debug_ISO_Screen_green_line_top, Debug_ISO_Screen_green_line_top, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_green_line_top, 6, 4);
	lv_obj_set_size(ui->Debug_ISO_Screen_green_line_top, 468, 1);

	//Write style for Debug_ISO_Screen_green_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_green_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_green_line_top, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_green_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_green_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_blue_line_right
	ui->Debug_ISO_Screen_blue_line_right = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_blue_line_right[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Debug_ISO_Screen_blue_line_right, Debug_ISO_Screen_blue_line_right, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_blue_line_right, 477, 3);
	lv_obj_set_size(ui->Debug_ISO_Screen_blue_line_right, 1, 264);

	//Write style for Debug_ISO_Screen_blue_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_blue_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_blue_line_right, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_blue_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_blue_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_blue_line_left
	ui->Debug_ISO_Screen_blue_line_left = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_blue_line_left[] ={{0, 0},{0, 268},};
	lv_line_set_points(ui->Debug_ISO_Screen_blue_line_left, Debug_ISO_Screen_blue_line_left, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_blue_line_left, 3, 2);
	lv_obj_set_size(ui->Debug_ISO_Screen_blue_line_left, 2, 266);

	//Write style for Debug_ISO_Screen_blue_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_blue_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_blue_line_left, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_blue_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_blue_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_blue_line_bottom
	ui->Debug_ISO_Screen_blue_line_bottom = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_blue_line_bottom[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Debug_ISO_Screen_blue_line_bottom, Debug_ISO_Screen_blue_line_bottom, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_blue_line_bottom, 3, 269);
	lv_obj_set_size(ui->Debug_ISO_Screen_blue_line_bottom, 473, 1);

	//Write style for Debug_ISO_Screen_blue_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_blue_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_blue_line_bottom, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_blue_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_blue_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_ISO15118_status_value
	ui->Debug_ISO_Screen_ISO15118_status_value = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_ISO15118_status_value, "ISO15118 Not Activated");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_ISO15118_status_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_ISO15118_status_value, 191, 120);
	lv_obj_set_size(ui->Debug_ISO_Screen_ISO15118_status_value, 276, 24);

	//Write style for Debug_ISO_Screen_ISO15118_status_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_ISO15118_status_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Debug_ISO_Screen_ISO15118_status_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Debug_ISO_Screen_ISO15118_status_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Debug_ISO_Screen_ISO15118_status_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_ISO15118_status_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_ISO15118_status_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_ISO15118_status_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_ISO15118_status_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_ISO15118_status_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_ISO15118_status_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_ISO15118_status_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Debug_ISO_Screen_ISO15118_status_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Debug_ISO_Screen_ISO15118_status_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_ISO15118_status_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_blue_line_top
	ui->Debug_ISO_Screen_blue_line_top = lv_line_create(ui->Debug_ISO_Screen);
	static lv_point_t Debug_ISO_Screen_blue_line_top[] ={{0, 0},{476, 0},};
	lv_line_set_points(ui->Debug_ISO_Screen_blue_line_top, Debug_ISO_Screen_blue_line_top, 2);
	lv_obj_set_pos(ui->Debug_ISO_Screen_blue_line_top, 5, 2);
	lv_obj_set_size(ui->Debug_ISO_Screen_blue_line_top, 470, 1);

	//Write style for Debug_ISO_Screen_blue_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_line_width(ui->Debug_ISO_Screen_blue_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_color(ui->Debug_ISO_Screen_blue_line_top, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_opa(ui->Debug_ISO_Screen_blue_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_line_rounded(ui->Debug_ISO_Screen_blue_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_ID_back_btn
	ui->Debug_ISO_Screen_ID_back_btn = lv_imgbtn_create(ui->Debug_ISO_Screen);
	lv_obj_add_flag(ui->Debug_ISO_Screen_ID_back_btn, LV_OBJ_FLAG_CHECKABLE);
	lv_imgbtn_set_src(ui->Debug_ISO_Screen_ID_back_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->Debug_ISO_Screen_ID_back_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->Debug_ISO_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_returnpng_alpha_32x28, NULL);
	lv_imgbtn_set_src(ui->Debug_ISO_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_returnpng_alpha_32x28, NULL);
	ui->Debug_ISO_Screen_ID_back_btn_label = lv_label_create(ui->Debug_ISO_Screen_ID_back_btn);
	lv_label_set_text(ui->Debug_ISO_Screen_ID_back_btn_label, "");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_ID_back_btn_label, LV_LABEL_LONG_WRAP);
	lv_obj_align(ui->Debug_ISO_Screen_ID_back_btn_label, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_style_pad_all(ui->Debug_ISO_Screen_ID_back_btn, 0, LV_STATE_DEFAULT);
	lv_obj_set_pos(ui->Debug_ISO_Screen_ID_back_btn, 430, 15);
	lv_obj_set_size(ui->Debug_ISO_Screen_ID_back_btn, 32, 28);
	lv_obj_add_flag(ui->Debug_ISO_Screen_ID_back_btn, LV_OBJ_FLAG_CLICKABLE);
	lv_obj_add_flag(ui->Debug_ISO_Screen_ID_back_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

	//Write style for Debug_ISO_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_ID_back_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_ID_back_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write style for Debug_ISO_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
	lv_obj_set_style_img_opa(ui->Debug_ISO_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

	//Write style for Debug_ISO_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
	lv_obj_set_style_img_opa(ui->Debug_ISO_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

	//Write style for Debug_ISO_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
	lv_obj_set_style_img_opa(ui->Debug_ISO_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

	//Write codes Debug_ISO_Screen_DebugISO_Heading
	ui->Debug_ISO_Screen_DebugISO_Heading = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_DebugISO_Heading, "Debug ISO15118 Menu");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_DebugISO_Heading, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_DebugISO_Heading, 150, 17);
	lv_obj_set_size(ui->Debug_ISO_Screen_DebugISO_Heading, 219, 24);

	//Write style for Debug_ISO_Screen_DebugISO_Heading, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_DebugISO_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Debug_ISO_Screen_DebugISO_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Debug_ISO_Screen_DebugISO_Heading, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Debug_ISO_Screen_DebugISO_Heading, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_DebugISO_Heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_DebugISO_Heading, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_DebugISO_Heading, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_DebugISO_Heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_DebugISO_Heading, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_DebugISO_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_DebugISO_Heading, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_DebugISO_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_DebugISO_Heading, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_DebugISO_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_DebugISO_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_DebugISO_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_DebugISO_Heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_power_request_cntr_value
	ui->Debug_ISO_Screen_power_request_cntr_value = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_power_request_cntr_value, "0");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_power_request_cntr_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_power_request_cntr_value, 191, 216);
	lv_obj_set_size(ui->Debug_ISO_Screen_power_request_cntr_value, 107, 24);
	lv_obj_add_flag(ui->Debug_ISO_Screen_power_request_cntr_value, LV_OBJ_FLAG_HIDDEN);

	//Write style for Debug_ISO_Screen_power_request_cntr_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_power_request_cntr_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Debug_ISO_Screen_power_request_cntr_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Debug_ISO_Screen_power_request_cntr_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Debug_ISO_Screen_power_request_cntr_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_power_request_cntr_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_power_request_cntr_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_power_request_cntr_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_power_request_cntr_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_power_request_cntr_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_power_request_cntr_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_power_request_cntr_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Debug_ISO_Screen_power_request_cntr_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Debug_ISO_Screen_power_request_cntr_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_power_request_cntr_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_power_request_cntr_prompt
	ui->Debug_ISO_Screen_power_request_cntr_prompt = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_power_request_cntr_prompt, "Power Request Cntr");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_power_request_cntr_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_power_request_cntr_prompt, 20, 218);
	lv_obj_set_size(ui->Debug_ISO_Screen_power_request_cntr_prompt, 160, 23);
	lv_obj_add_flag(ui->Debug_ISO_Screen_power_request_cntr_prompt, LV_OBJ_FLAG_HIDDEN);

	//Write style for Debug_ISO_Screen_power_request_cntr_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_power_request_cntr_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_power_request_cntr_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_power_request_cntr_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_power_request_cntr_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_power_request_cntr_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_power_request_cntr_prompt, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_V2G_demo_value
	ui->Debug_ISO_Screen_V2G_demo_value = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_V2G_demo_value, "Not connected");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_V2G_demo_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_V2G_demo_value, 191, 166);
	lv_obj_set_size(ui->Debug_ISO_Screen_V2G_demo_value, 275, 24);
	lv_obj_add_flag(ui->Debug_ISO_Screen_V2G_demo_value, LV_OBJ_FLAG_HIDDEN);

	//Write style for Debug_ISO_Screen_V2G_demo_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_V2G_demo_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Debug_ISO_Screen_V2G_demo_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Debug_ISO_Screen_V2G_demo_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Debug_ISO_Screen_V2G_demo_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_V2G_demo_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_V2G_demo_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_V2G_demo_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_V2G_demo_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_V2G_demo_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_V2G_demo_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_V2G_demo_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_V2G_demo_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Debug_ISO_Screen_V2G_demo_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Debug_ISO_Screen_V2G_demo_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_V2G_demo_value, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_V2G_demo_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_V2G_demo_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_V2G_demo_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_V2G_demo_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_V2G_demo_prompt
	ui->Debug_ISO_Screen_V2G_demo_prompt = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_V2G_demo_prompt, "V2G Demo");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_V2G_demo_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_V2G_demo_prompt, 18, 168);
	lv_obj_set_size(ui->Debug_ISO_Screen_V2G_demo_prompt, 96, 23);
	lv_obj_add_flag(ui->Debug_ISO_Screen_V2G_demo_prompt, LV_OBJ_FLAG_HIDDEN);

	//Write style for Debug_ISO_Screen_V2G_demo_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_V2G_demo_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_V2G_demo_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_V2G_demo_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_V2G_demo_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_V2G_demo_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_V2G_demo_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_V2G_demo_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_ISO15118_status_prompt
	ui->Debug_ISO_Screen_ISO15118_status_prompt = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_ISO15118_status_prompt, "ISO15118 Status");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_ISO15118_status_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_ISO15118_status_prompt, 30, 125);
	lv_obj_set_size(ui->Debug_ISO_Screen_ISO15118_status_prompt, 109, 18);

	//Write style for Debug_ISO_Screen_ISO15118_status_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_ISO15118_status_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_ISO15118_status_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_ISO15118_status_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_ISO15118_status_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_ISO15118_status_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_ISO15118_status_prompt, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_clock
	ui->Debug_ISO_Screen_clock = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_clock, "0:00");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_clock, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_clock, 15, 17);
	lv_obj_set_size(ui->Debug_ISO_Screen_clock, 87, 24);

	//Write style for Debug_ISO_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_clock, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_SIGBOARD_connection_prompt
	ui->Debug_ISO_Screen_SIGBOARD_connection_prompt = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, "SIGBOARD connection");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 26, 76);
	lv_obj_set_size(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 160, 22);

	//Write style for Debug_ISO_Screen_SIGBOARD_connection_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//Write codes Debug_ISO_Screen_SIGBOARD_connection_value
	ui->Debug_ISO_Screen_SIGBOARD_connection_value = lv_label_create(ui->Debug_ISO_Screen);
	lv_label_set_text(ui->Debug_ISO_Screen_SIGBOARD_connection_value, "Arduino header");
	lv_label_set_long_mode(ui->Debug_ISO_Screen_SIGBOARD_connection_value, LV_LABEL_LONG_WRAP);
	lv_obj_set_pos(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 191, 74);
	lv_obj_set_size(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 276, 24);

	//Write style for Debug_ISO_Screen_SIGBOARD_connection_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
	lv_obj_set_style_border_width(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_opa(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_color(ui->Debug_ISO_Screen_SIGBOARD_connection_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_border_side(ui->Debug_ISO_Screen_SIGBOARD_connection_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_radius(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_color(ui->Debug_ISO_Screen_SIGBOARD_connection_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_font(ui->Debug_ISO_Screen_SIGBOARD_connection_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_opa(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_letter_space(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_line_space(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_text_align(ui->Debug_ISO_Screen_SIGBOARD_connection_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_opa(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_color(ui->Debug_ISO_Screen_SIGBOARD_connection_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_bg_grad_dir(ui->Debug_ISO_Screen_SIGBOARD_connection_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_top(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_right(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_bottom(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_pad_left(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
	lv_obj_set_style_shadow_width(ui->Debug_ISO_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

	//The custom code of Debug_ISO_Screen.
	

	//Update current screen layout.
	lv_obj_update_layout(ui->Debug_ISO_Screen);

	//Init events for screen.
	events_init_Debug_ISO_Screen(ui);
}

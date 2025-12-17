/*
* Copyright 2025 NXP
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
    lv_obj_set_style_bg_opa(ui->Main_Screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui->Main_Screen, &_welcome_bak_RGB565_480x272, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(ui->Main_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor_opa(ui->Main_Screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_cont_cover
    ui->Main_Screen_cont_cover = lv_obj_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_cont_cover, 2, 0);
    lv_obj_set_size(ui->Main_Screen_cont_cover, 475, 269);
    lv_obj_set_scrollbar_mode(ui->Main_Screen_cont_cover, LV_SCROLLBAR_MODE_OFF);

    //Write style for Main_Screen_cont_cover, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_cont_cover, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_label_welcome
    ui->Main_Screen_label_welcome = lv_label_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_label_welcome, 162, -102);
    lv_obj_set_size(ui->Main_Screen_label_welcome, 237, 91);
    lv_label_set_text(ui->Main_Screen_label_welcome, "Welcome to EasyEVSE\nNXP EV Charger");
    lv_label_set_long_mode(ui->Main_Screen_label_welcome, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_label_welcome, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_label_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_label_welcome, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_label_welcome, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_label_welcome, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_label_welcome, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_label_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_label_welcome, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_label_welcome, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_label_welcome, 215, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_Screen_label_welcome, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_Screen_label_welcome, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_label_welcome, 26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_label_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_label_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_label_welcome, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_label_welcome, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Main_Screen_label_welcome, lv_color_hex(0x111314), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Main_Screen_label_welcome, 185, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Main_Screen_label_welcome, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Main_Screen_label_welcome, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Main_Screen_label_welcome, 1, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_spangroup_A
    ui->Main_Screen_spangroup_A = lv_spangroup_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_spangroup_A, -158, 171);
    lv_obj_set_size(ui->Main_Screen_spangroup_A, 144, 56);
    lv_obj_add_flag(ui->Main_Screen_spangroup_A, LV_OBJ_FLAG_CLICKABLE);
    lv_spangroup_set_align(ui->Main_Screen_spangroup_A, LV_TEXT_ALIGN_LEFT);
    lv_spangroup_set_overflow(ui->Main_Screen_spangroup_A, LV_SPAN_OVERFLOW_CLIP);
    lv_spangroup_set_mode(ui->Main_Screen_spangroup_A, LV_SPAN_MODE_FIXED);
    //create span
    ui->Main_Screen_spangroup_A_span = lv_spangroup_new_span(ui->Main_Screen_spangroup_A);
    lv_span_set_text(ui->Main_Screen_spangroup_A_span, "L");
    lv_style_set_text_color(lv_span_get_style(ui->Main_Screen_spangroup_A_span), lv_color_hex(0x59564d));
    lv_style_set_text_decor(lv_span_get_style(ui->Main_Screen_spangroup_A_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->Main_Screen_spangroup_A_span), &lv_font_montserratMedium_50);
    ui->Main_Screen_spangroup_A_span = lv_spangroup_new_span(ui->Main_Screen_spangroup_A);
    lv_span_set_text(ui->Main_Screen_spangroup_A_span, "et's go!");
    lv_style_set_text_color(lv_span_get_style(ui->Main_Screen_spangroup_A_span), lv_color_hex(0x000000));
    lv_style_set_text_decor(lv_span_get_style(ui->Main_Screen_spangroup_A_span), LV_TEXT_DECOR_NONE);
    lv_style_set_text_font(lv_span_get_style(ui->Main_Screen_spangroup_A_span), &lv_font_montserratMedium_20);

    //Write style state: LV_STATE_DEFAULT for &style_Main_Screen_spangroup_A_main_main_default
    static lv_style_t style_Main_Screen_spangroup_A_main_main_default;
    ui_init_style(&style_Main_Screen_spangroup_A_main_main_default);

    lv_style_set_border_width(&style_Main_Screen_spangroup_A_main_main_default, 0);
    lv_style_set_radius(&style_Main_Screen_spangroup_A_main_main_default, 9);
    lv_style_set_bg_opa(&style_Main_Screen_spangroup_A_main_main_default, 207);
    lv_style_set_bg_color(&style_Main_Screen_spangroup_A_main_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_Main_Screen_spangroup_A_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_pad_top(&style_Main_Screen_spangroup_A_main_main_default, 2);
    lv_style_set_pad_right(&style_Main_Screen_spangroup_A_main_main_default, 0);
    lv_style_set_pad_bottom(&style_Main_Screen_spangroup_A_main_main_default, 2);
    lv_style_set_pad_left(&style_Main_Screen_spangroup_A_main_main_default, 24);
    lv_style_set_shadow_width(&style_Main_Screen_spangroup_A_main_main_default, 7);
    lv_style_set_shadow_color(&style_Main_Screen_spangroup_A_main_main_default, lv_color_hex(0x111314));
    lv_style_set_shadow_opa(&style_Main_Screen_spangroup_A_main_main_default, 255);
    lv_style_set_shadow_spread(&style_Main_Screen_spangroup_A_main_main_default, 1);
    lv_style_set_shadow_offset_x(&style_Main_Screen_spangroup_A_main_main_default, 2);
    lv_style_set_shadow_offset_y(&style_Main_Screen_spangroup_A_main_main_default, 2);
    lv_obj_add_style(ui->Main_Screen_spangroup_A, &style_Main_Screen_spangroup_A_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_spangroup_refr_mode(ui->Main_Screen_spangroup_A);

    //Write codes Main_Screen_clock
    ui->Main_Screen_clock = lv_label_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_clock, 14, 17);
    lv_obj_set_size(ui->Main_Screen_clock, 87, 24);
    lv_label_set_text(ui->Main_Screen_clock, "0:00");
    lv_label_set_long_mode(ui->Main_Screen_clock, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_EVSE_software_version
    ui->Main_Screen_EVSE_software_version = lv_label_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_EVSE_software_version, 369, 250);
    lv_obj_set_size(ui->Main_Screen_EVSE_software_version, 98, 16);
    lv_label_set_text(ui->Main_Screen_EVSE_software_version, "EVSE SW v");
    lv_label_set_long_mode(ui->Main_Screen_EVSE_software_version, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_EVSE_software_version, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_EVSE_software_version, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_EVSE_software_version, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_EVSE_software_version, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_EVSE_software_version, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_EVSE_software_version, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_EVSE_software_version, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_EVSE_software_version, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_cont_1
    ui->Main_Screen_cont_1 = lv_obj_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_cont_1, 14, 225);
    lv_obj_set_size(ui->Main_Screen_cont_1, 195, 37);
    lv_obj_set_scrollbar_mode(ui->Main_Screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for Main_Screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_cont_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_cont_1, 42, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_Screen_cont_1, lv_color_hex(0x1c1c1c), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_Screen_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_cont_1, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Main_Screen_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Main_Screen_cont_1, 175, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Main_Screen_cont_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Main_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Main_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_wifi_symbol
    ui->Main_Screen_wifi_symbol = lv_label_create(ui->Main_Screen_cont_1);
    lv_obj_set_pos(ui->Main_Screen_wifi_symbol, 12, 2);
    lv_obj_set_size(ui->Main_Screen_wifi_symbol, 21, 21);
    lv_obj_add_flag(ui->Main_Screen_wifi_symbol, LV_OBJ_FLAG_CLICKABLE);
    lv_label_set_text(ui->Main_Screen_wifi_symbol, "" LV_SYMBOL_WIFI " ");
    lv_label_set_long_mode(ui->Main_Screen_wifi_symbol, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_wifi_symbol, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_wifi_symbol, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_wifi_symbol, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_wifi_symbol, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_wifi_symbol, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_wifi_symbol, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_wifi_symbol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_SSID_prompt
    ui->Main_Screen_SSID_prompt = lv_label_create(ui->Main_Screen_cont_1);
    lv_obj_set_pos(ui->Main_Screen_SSID_prompt, 36, 5);
    lv_obj_set_size(ui->Main_Screen_SSID_prompt, 153, 27);
    lv_label_set_text(ui->Main_Screen_SSID_prompt, "");
    lv_label_set_long_mode(ui->Main_Screen_SSID_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_SSID_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_SSID_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_SSID_prompt, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_SSID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_SSID_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_SSID_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_SSID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_line_1
    ui->Main_Screen_line_1 = lv_line_create(ui->Main_Screen_cont_1);
    lv_obj_set_pos(ui->Main_Screen_line_1, 42, 1);
    lv_obj_set_size(ui->Main_Screen_line_1, 1, 36);
    static lv_point_precise_t Main_Screen_line_1[] = {{0, 0},{0, 40}};
    lv_line_set_points(ui->Main_Screen_line_1, Main_Screen_line_1, 2);

    //Write style for Main_Screen_line_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Main_Screen_line_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Main_Screen_line_1, lv_color_hex(0xa0a0a0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Main_Screen_line_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Main_Screen_line_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_type_of_connection_label
    ui->Main_Screen_type_of_connection_label = lv_label_create(ui->Main_Screen_cont_1);
    lv_obj_set_pos(ui->Main_Screen_type_of_connection_label, -1, 22);
    lv_obj_set_size(ui->Main_Screen_type_of_connection_label, 41, 10);
    lv_label_set_text(ui->Main_Screen_type_of_connection_label, "Wi-Fi");
    lv_label_set_long_mode(ui->Main_Screen_type_of_connection_label, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_type_of_connection_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_type_of_connection_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_type_of_connection_label, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_type_of_connection_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_type_of_connection_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_type_of_connection_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_cont_2
    ui->Main_Screen_cont_2 = lv_obj_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_cont_2, -464, 108);
    lv_obj_set_size(ui->Main_Screen_cont_2, 300, 164);
    lv_obj_set_scrollbar_mode(ui->Main_Screen_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for Main_Screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_cont_2, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_cont_2, 230, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_Screen_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_Screen_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_cont_2, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Main_Screen_cont_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Main_Screen_cont_2, 175, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Main_Screen_cont_2, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Main_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Main_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_OCPP_status_prompt
    ui->Main_Screen_OCPP_status_prompt = lv_label_create(ui->Main_Screen_cont_2);
    lv_obj_set_pos(ui->Main_Screen_OCPP_status_prompt, 18, 46);
    lv_obj_set_size(ui->Main_Screen_OCPP_status_prompt, 135, 23);
    lv_label_set_text(ui->Main_Screen_OCPP_status_prompt, "OCPP status");
    lv_label_set_long_mode(ui->Main_Screen_OCPP_status_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_OCPP_status_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_OCPP_status_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_OCPP_status_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_OCPP_status_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_OCPP_status_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_OCPP_status_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_OCPP_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_OCPP_status_value
    ui->Main_Screen_OCPP_status_value = lv_label_create(ui->Main_Screen_cont_2);
    lv_obj_set_pos(ui->Main_Screen_OCPP_status_value, 154, 44);
    lv_obj_set_size(ui->Main_Screen_OCPP_status_value, 125, 34);
    lv_label_set_text(ui->Main_Screen_OCPP_status_value, "");
    lv_label_set_long_mode(ui->Main_Screen_OCPP_status_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_OCPP_status_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_OCPP_status_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_OCPP_status_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_OCPP_status_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_OCPP_status_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_OCPP_status_value, 218, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_Screen_OCPP_status_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_Screen_OCPP_status_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_OCPP_status_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_OCPP_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_telemetry_value
    ui->Main_Screen_telemetry_value = lv_label_create(ui->Main_Screen_cont_2);
    lv_obj_set_pos(ui->Main_Screen_telemetry_value, 155, 96);
    lv_obj_set_size(ui->Main_Screen_telemetry_value, 125, 22);
    lv_label_set_text(ui->Main_Screen_telemetry_value, "0");
    lv_label_set_long_mode(ui->Main_Screen_telemetry_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_telemetry_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_telemetry_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_telemetry_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_telemetry_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_telemetry_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_telemetry_value, 218, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_Screen_telemetry_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_Screen_telemetry_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_telemetry_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_telemetry_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_telemetry_prompt
    ui->Main_Screen_telemetry_prompt = lv_label_create(ui->Main_Screen_cont_2);
    lv_obj_set_pos(ui->Main_Screen_telemetry_prompt, 18, 96);
    lv_obj_set_size(ui->Main_Screen_telemetry_prompt, 135, 23);
    lv_label_set_text(ui->Main_Screen_telemetry_prompt, "Telemetry cntr");
    lv_label_set_long_mode(ui->Main_Screen_telemetry_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_telemetry_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_telemetry_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_telemetry_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_telemetry_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_telemetry_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_telemetry_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_telemetry_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_close_btn
    ui->Main_Screen_close_btn = lv_button_create(ui->Main_Screen_cont_2);
    lv_obj_set_pos(ui->Main_Screen_close_btn, 264, 7);
    lv_obj_set_size(ui->Main_Screen_close_btn, 29, 29);
    lv_obj_add_flag(ui->Main_Screen_close_btn, LV_OBJ_FLAG_CLICKABLE);
    ui->Main_Screen_close_btn_label = lv_label_create(ui->Main_Screen_close_btn);
    lv_label_set_text(ui->Main_Screen_close_btn_label, "" LV_SYMBOL_CLOSE " ");
    lv_label_set_long_mode(ui->Main_Screen_close_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Main_Screen_close_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Main_Screen_close_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Main_Screen_close_btn_label, LV_PCT(100));

    //Write style for Main_Screen_close_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Main_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Main_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_close_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_close_btn, lv_color_hex(0x656565), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_close_btn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_close_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_close_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_Screen_NFC_activation_status
    ui->Main_Screen_NFC_activation_status = lv_label_create(ui->Main_Screen);
    lv_obj_set_pos(ui->Main_Screen_NFC_activation_status, 95, 12);
    lv_obj_set_size(ui->Main_Screen_NFC_activation_status, 24, 14);
    lv_label_set_text(ui->Main_Screen_NFC_activation_status, "" LV_SYMBOL_WIFI " ");
    lv_label_set_long_mode(ui->Main_Screen_NFC_activation_status, LV_LABEL_LONG_WRAP);

    //Write style for Main_Screen_NFC_activation_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_Screen_NFC_activation_status, lv_color_hex(0xc51f1f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_Screen_NFC_activation_status, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_Screen_NFC_activation_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_Screen_NFC_activation_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Main_Screen.
    lv_obj_set_style_transform_angle(ui->Main_Screen_NFC_activation_status, 900, 0);

    //Update current screen layout.
    lv_obj_update_layout(ui->Main_Screen);

    //Init events for screen.
    events_init_Main_Screen(ui);
}

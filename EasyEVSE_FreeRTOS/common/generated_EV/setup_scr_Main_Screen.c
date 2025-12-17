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



void setup_scr_Main_screen(lv_ui *ui)
{
    //Write codes Main_screen
    ui->Main_screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->Main_screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->Main_screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for Main_screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Main_screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_battery_indicator
    ui->Main_screen_battery_indicator = lv_bar_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_battery_indicator, 125, 194);
    lv_obj_set_size(ui->Main_screen_battery_indicator, 312, 37);
    lv_obj_set_style_anim_duration(ui->Main_screen_battery_indicator, 1000, 0);
    lv_bar_set_mode(ui->Main_screen_battery_indicator, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->Main_screen_battery_indicator, 0, 100);
    lv_bar_set_value(ui->Main_screen_battery_indicator, 100, LV_ANIM_OFF);

    //Write style for Main_screen_battery_indicator, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Main_screen_battery_indicator, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_battery_indicator, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_battery_indicator, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Main_screen_battery_indicator, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Main_screen_battery_indicator, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen_battery_indicator, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen_battery_indicator, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_battery_indicator, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes Main_screen_battery_value
    ui->Main_screen_battery_value = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_battery_value, 125, 194);
    lv_obj_set_size(ui->Main_screen_battery_value, 312, 37);
    lv_label_set_text(ui->Main_screen_battery_value, "100%");
    lv_label_set_long_mode(ui->Main_screen_battery_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_battery_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_battery_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_battery_value, &lv_font_montserratMedium_21, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_battery_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_battery_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_battery_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_battery_value, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_battery_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_power_value
    ui->Main_screen_power_value = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_power_value, 355, 124);
    lv_obj_set_size(ui->Main_screen_power_value, 112, 23);
    lv_label_set_text(ui->Main_screen_power_value, "");
    lv_label_set_long_mode(ui->Main_screen_power_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_power_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_power_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Main_screen_power_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Main_screen_power_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Main_screen_power_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_power_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_power_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_power_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_power_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_power_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_power_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_power_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_power_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen_power_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen_power_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_power_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_power_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_power_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_power_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_power_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_voltage_value
    ui->Main_screen_voltage_value = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_voltage_value, 355, 73);
    lv_obj_set_size(ui->Main_screen_voltage_value, 112, 23);
    lv_label_set_text(ui->Main_screen_voltage_value, "");
    lv_label_set_long_mode(ui->Main_screen_voltage_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_voltage_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_voltage_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Main_screen_voltage_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Main_screen_voltage_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Main_screen_voltage_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_voltage_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_voltage_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_voltage_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_voltage_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_voltage_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_voltage_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_voltage_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen_voltage_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen_voltage_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_voltage_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_current_value
    ui->Main_screen_current_value = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_current_value, 355, 27);
    lv_obj_set_size(ui->Main_screen_current_value, 112, 23);
    lv_label_set_text(ui->Main_screen_current_value, "");
    lv_label_set_long_mode(ui->Main_screen_current_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_current_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_current_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Main_screen_current_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Main_screen_current_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Main_screen_current_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_current_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_current_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_current_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_current_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_current_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_current_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_current_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen_current_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen_current_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_current_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_time_of_charge_value
    ui->Main_screen_time_of_charge_value = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_time_of_charge_value, 111, 124);
    lv_obj_set_size(ui->Main_screen_time_of_charge_value, 112, 23);
    lv_label_set_text(ui->Main_screen_time_of_charge_value, "");
    lv_label_set_long_mode(ui->Main_screen_time_of_charge_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_time_of_charge_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_time_of_charge_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Main_screen_time_of_charge_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Main_screen_time_of_charge_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Main_screen_time_of_charge_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_time_of_charge_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_time_of_charge_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_time_of_charge_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_time_of_charge_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_time_of_charge_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_time_of_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_time_of_charge_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_time_of_charge_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen_time_of_charge_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen_time_of_charge_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_time_of_charge_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_time_of_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_time_of_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_time_of_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_time_of_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_auth_value
    ui->Main_screen_auth_value = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_auth_value, 111, 73);
    lv_obj_set_size(ui->Main_screen_auth_value, 112, 23);
    lv_label_set_text(ui->Main_screen_auth_value, "");
    lv_label_set_long_mode(ui->Main_screen_auth_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_auth_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_auth_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Main_screen_auth_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Main_screen_auth_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Main_screen_auth_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_auth_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_auth_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_auth_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_auth_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_auth_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_auth_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_auth_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_auth_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen_auth_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen_auth_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_auth_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_auth_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_auth_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_auth_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_auth_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_protocol_value
    ui->Main_screen_protocol_value = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_protocol_value, 111, 27);
    lv_obj_set_size(ui->Main_screen_protocol_value, 112, 23);
    lv_label_set_text(ui->Main_screen_protocol_value, "");
    lv_label_set_long_mode(ui->Main_screen_protocol_value, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_protocol_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_protocol_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Main_screen_protocol_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Main_screen_protocol_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Main_screen_protocol_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_protocol_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_protocol_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_protocol_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_protocol_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_protocol_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_protocol_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_protocol_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Main_screen_protocol_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Main_screen_protocol_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_protocol_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_protocol_label
    ui->Main_screen_protocol_label = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_protocol_label, 17, 31);
    lv_obj_set_size(ui->Main_screen_protocol_label, 85, 21);
    lv_label_set_text(ui->Main_screen_protocol_label, "Protocol");
    lv_label_set_long_mode(ui->Main_screen_protocol_label, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_protocol_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_protocol_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_protocol_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_protocol_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_protocol_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_protocol_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_protocol_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_auth_label
    ui->Main_screen_auth_label = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_auth_label, 9, 73);
    lv_obj_set_size(ui->Main_screen_auth_label, 94, 28);
    lv_label_set_text(ui->Main_screen_auth_label, "Auth method");
    lv_label_set_long_mode(ui->Main_screen_auth_label, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_auth_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_auth_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_auth_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_auth_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_auth_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_auth_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_auth_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_time_of_charge_label
    ui->Main_screen_time_of_charge_label = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_time_of_charge_label, 17, 124);
    lv_obj_set_size(ui->Main_screen_time_of_charge_label, 85, 34);
    lv_label_set_text(ui->Main_screen_time_of_charge_label, "Time to charge");
    lv_label_set_long_mode(ui->Main_screen_time_of_charge_label, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_time_of_charge_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_time_of_charge_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_time_of_charge_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_time_of_charge_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_time_of_charge_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_time_of_charge_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_time_of_charge_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_current_label
    ui->Main_screen_current_label = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_current_label, 240, 27);
    lv_obj_set_size(ui->Main_screen_current_label, 111, 27);
    lv_label_set_text(ui->Main_screen_current_label, "Charging current [A]");
    lv_label_set_long_mode(ui->Main_screen_current_label, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_current_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_current_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_current_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_current_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_current_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_current_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_current_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_voltage_label
    ui->Main_screen_voltage_label = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_voltage_label, 240, 73);
    lv_obj_set_size(ui->Main_screen_voltage_label, 111, 27);
    lv_label_set_text(ui->Main_screen_voltage_label, "Charging voltage [V]");
    lv_label_set_long_mode(ui->Main_screen_voltage_label, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_voltage_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_voltage_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_voltage_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_voltage_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_voltage_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_voltage_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_voltage_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_power_label
    ui->Main_screen_power_label = lv_label_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_power_label, 240, 121);
    lv_obj_set_size(ui->Main_screen_power_label, 111, 27);
    lv_label_set_text(ui->Main_screen_power_label, "Charging power [W]");
    lv_label_set_long_mode(ui->Main_screen_power_label, LV_LABEL_LONG_WRAP);

    //Write style for Main_screen_power_label, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Main_screen_power_label, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Main_screen_power_label, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Main_screen_power_label, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Main_screen_power_label, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Main_screen_power_label, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Main_screen_power_label, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Main_screen_EV_img
    ui->Main_screen_EV_img = lv_image_create(ui->Main_screen);
    lv_obj_set_pos(ui->Main_screen_EV_img, 20, 172);
    lv_obj_set_size(ui->Main_screen_EV_img, 78, 77);
    lv_obj_add_flag(ui->Main_screen_EV_img, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Main_screen_EV_img, &_EV_RGB565A8_78x77);
    lv_image_set_pivot(ui->Main_screen_EV_img, 50,50);
    lv_image_set_rotation(ui->Main_screen_EV_img, 0);

    //Write style for Main_screen_EV_img, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Main_screen_EV_img, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Main_screen_EV_img, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Main_screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->Main_screen);

    //Init events for screen.
    events_init_Main_screen(ui);
}

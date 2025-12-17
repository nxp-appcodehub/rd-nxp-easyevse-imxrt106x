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



void setup_scr_Charging_Screen(lv_ui *ui)
{
    //Write codes Charging_Screen
    ui->Charging_Screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->Charging_Screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->Charging_Screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for Charging_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Charging_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_img_car
    ui->Charging_Screen_img_car = lv_image_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_img_car, 11, -10);
    lv_obj_set_size(ui->Charging_Screen_img_car, 395, 216);
    lv_obj_add_flag(ui->Charging_Screen_img_car, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Charging_Screen_img_car, &_charging_bak_RGB565A8_395x216);
    lv_image_set_pivot(ui->Charging_Screen_img_car, 50,50);
    lv_image_set_rotation(ui->Charging_Screen_img_car, 0);

    //Write style for Charging_Screen_img_car, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Charging_Screen_img_car, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Charging_Screen_img_car, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_bar_charging
    ui->Charging_Screen_bar_charging = lv_bar_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_bar_charging, 152, 59);
    lv_obj_set_size(ui->Charging_Screen_bar_charging, 200, 139);
    lv_obj_set_style_anim_duration(ui->Charging_Screen_bar_charging, 1000, 0);
    lv_bar_set_mode(ui->Charging_Screen_bar_charging, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->Charging_Screen_bar_charging, 0, 100);
    lv_bar_set_value(ui->Charging_Screen_bar_charging, 0, LV_ANIM_OFF);

    //Write style for Charging_Screen_bar_charging, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Charging_Screen_bar_charging, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_bar_charging, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_bar_charging, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Charging_Screen_bar_charging, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Charging_Screen_bar_charging, 0, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_bar_charging, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui->Charging_Screen_bar_charging, &_charing_RGB565A8_200x139, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(ui->Charging_Screen_bar_charging, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor(ui->Charging_Screen_bar_charging, lv_color_hex(0x57f900), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor_opa(ui->Charging_Screen_bar_charging, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_battery_level
    ui->Charging_Screen_battery_level = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_battery_level, 206, 104);
    lv_obj_set_size(ui->Charging_Screen_battery_level, 72, 39);
    lv_label_set_text(ui->Charging_Screen_battery_level, "0");
    lv_label_set_long_mode(ui->Charging_Screen_battery_level, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_battery_level, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_battery_level, lv_color_hex(0x0c400c), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_battery_level, &lv_font_montserratMedium_40, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_battery_level, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_battery_level, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_battery_level, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_percent
    ui->Charging_Screen_label_percent = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_label_percent, 270, 121);
    lv_obj_set_size(ui->Charging_Screen_label_percent, 25, 23);
    lv_label_set_text(ui->Charging_Screen_label_percent, "%");
    lv_label_set_long_mode(ui->Charging_Screen_label_percent, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_percent, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_percent, lv_color_hex(0x0c400c), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_percent, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_percent, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_percent, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_percent, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_img_slide
    ui->Charging_Screen_img_slide = lv_image_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_img_slide, 17, 122);
    lv_obj_set_size(ui->Charging_Screen_img_slide, 22, 13);
    lv_obj_add_flag(ui->Charging_Screen_img_slide, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Charging_Screen_img_slide, &_slide_RGB565A8_22x13);
    lv_image_set_pivot(ui->Charging_Screen_img_slide, 50,50);
    lv_image_set_rotation(ui->Charging_Screen_img_slide, 0);

    //Write style for Charging_Screen_img_slide, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Charging_Screen_img_slide, 110, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->Charging_Screen_img_slide, lv_color_hex(0x070303), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Charging_Screen_img_slide, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_cont_details
    ui->Charging_Screen_cont_details = lv_obj_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_cont_details, -461, 10);
    lv_obj_set_size(ui->Charging_Screen_cont_details, 303, 217);
    lv_obj_set_scrollbar_mode(ui->Charging_Screen_cont_details, LV_SCROLLBAR_MODE_OFF);

    //Write style for Charging_Screen_cont_details, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_cont_details, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_cont_details, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_cont_details, 230, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_cont_details, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_cont_details, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_cont_details, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_cont_details, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_cont_details, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_cont_details, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_cont_details, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Charging_Screen_cont_details, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Charging_Screen_cont_details, 175, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Charging_Screen_cont_details, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Charging_Screen_cont_details, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Charging_Screen_cont_details, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_charge_current_prompt
    ui->Charging_Screen_charge_current_prompt = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_charge_current_prompt, 16, 38);
    lv_obj_set_size(ui->Charging_Screen_charge_current_prompt, 135, 23);
    lv_label_set_text(ui->Charging_Screen_charge_current_prompt, "Current [A]");
    lv_label_set_long_mode(ui->Charging_Screen_charge_current_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_charge_current_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_charge_current_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_charge_current_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_charge_current_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_charge_current_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_charge_current_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_current_value
    ui->Charging_Screen_current_value = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_current_value, 137, 37);
    lv_obj_set_size(ui->Charging_Screen_current_value, 125, 22);
    lv_label_set_text(ui->Charging_Screen_current_value, "");
    lv_label_set_long_mode(ui->Charging_Screen_current_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_current_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_current_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_current_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_current_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_current_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_current_value, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_current_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_current_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_current_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_voltage_value
    ui->Charging_Screen_voltage_value = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_voltage_value, 138, 68);
    lv_obj_set_size(ui->Charging_Screen_voltage_value, 125, 22);
    lv_label_set_text(ui->Charging_Screen_voltage_value, "0");
    lv_label_set_long_mode(ui->Charging_Screen_voltage_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_voltage_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_voltage_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_voltage_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_voltage_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_voltage_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_voltage_value, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_voltage_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_voltage_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_voltage_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_voltage_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_46
    ui->Charging_Screen_label_46 = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_label_46, 15, 68);
    lv_obj_set_size(ui->Charging_Screen_label_46, 135, 23);
    lv_label_set_text(ui->Charging_Screen_label_46, "Voltage [V]");
    lv_label_set_long_mode(ui->Charging_Screen_label_46, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_46, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_46, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_46, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_46, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_46, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_46, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_46, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_protocol_value
    ui->Charging_Screen_protocol_value = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_protocol_value, 138, 158);
    lv_obj_set_size(ui->Charging_Screen_protocol_value, 125, 22);
    lv_label_set_text(ui->Charging_Screen_protocol_value, "");
    lv_label_set_long_mode(ui->Charging_Screen_protocol_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_protocol_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_protocol_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_protocol_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_protocol_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_protocol_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_protocol_value, 218, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_protocol_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_protocol_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_protocol_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_42
    ui->Charging_Screen_label_42 = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_label_42, 15, 158);
    lv_obj_set_size(ui->Charging_Screen_label_42, 135, 23);
    lv_label_set_text(ui->Charging_Screen_label_42, "Protocol");
    lv_label_set_long_mode(ui->Charging_Screen_label_42, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_42, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_42, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_42, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_42, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_42, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_42, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_42, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_charging_direction_value
    ui->Charging_Screen_charging_direction_value = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_charging_direction_value, 137, 187);
    lv_obj_set_size(ui->Charging_Screen_charging_direction_value, 125, 22);
    lv_label_set_text(ui->Charging_Screen_charging_direction_value, "");
    lv_label_set_long_mode(ui->Charging_Screen_charging_direction_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_charging_direction_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_charging_direction_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_charging_direction_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_charging_direction_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_charging_direction_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_charging_direction_value, 217, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_charging_direction_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_charging_direction_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_charging_direction_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_charging_direction_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_40
    ui->Charging_Screen_label_40 = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_label_40, 14, 188);
    lv_obj_set_size(ui->Charging_Screen_label_40, 135, 23);
    lv_label_set_text(ui->Charging_Screen_label_40, "Charging direction");
    lv_label_set_long_mode(ui->Charging_Screen_label_40, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_40, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_40, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_40, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_40, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_40, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_40, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_40, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_power_request_cntr_value
    ui->Charging_Screen_power_request_cntr_value = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_power_request_cntr_value, 137, 128);
    lv_obj_set_size(ui->Charging_Screen_power_request_cntr_value, 125, 22);
    lv_label_set_text(ui->Charging_Screen_power_request_cntr_value, "0");
    lv_label_set_long_mode(ui->Charging_Screen_power_request_cntr_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_power_request_cntr_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_power_request_cntr_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_power_request_cntr_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_power_request_cntr_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_power_request_cntr_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_power_request_cntr_value, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_power_request_cntr_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_power_request_cntr_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_power_request_cntr_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_power_request_cntr_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_power_request_cntr_prompt
    ui->Charging_Screen_power_request_cntr_prompt = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_power_request_cntr_prompt, 16, 128);
    lv_obj_set_size(ui->Charging_Screen_power_request_cntr_prompt, 135, 23);
    lv_label_set_text(ui->Charging_Screen_power_request_cntr_prompt, "Power Request Cntr");
    lv_label_set_long_mode(ui->Charging_Screen_power_request_cntr_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_power_request_cntr_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_power_request_cntr_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_power_request_cntr_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_power_request_cntr_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_power_request_cntr_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_power_request_cntr_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_power_request_cntr_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_close_btn
    ui->Charging_Screen_close_btn = lv_button_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_close_btn, 273, 4);
    lv_obj_set_size(ui->Charging_Screen_close_btn, 20, 27);
    lv_obj_add_flag(ui->Charging_Screen_close_btn, LV_OBJ_FLAG_CLICKABLE);
    ui->Charging_Screen_close_btn_label = lv_label_create(ui->Charging_Screen_close_btn);
    lv_label_set_text(ui->Charging_Screen_close_btn_label, "" LV_SYMBOL_CLOSE " ");
    lv_label_set_long_mode(ui->Charging_Screen_close_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Charging_Screen_close_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Charging_Screen_close_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Charging_Screen_close_btn_label, LV_PCT(100));

    //Write style for Charging_Screen_close_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Charging_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Charging_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_close_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_close_btn, lv_color_hex(0x656565), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_close_btn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_close_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_close_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_mode_value
    ui->Charging_Screen_mode_value = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_mode_value, 138, 8);
    lv_obj_set_size(ui->Charging_Screen_mode_value, 125, 22);
    lv_label_set_text(ui->Charging_Screen_mode_value, "");
    lv_label_set_long_mode(ui->Charging_Screen_mode_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_mode_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_mode_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_mode_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_mode_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_mode_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_mode_value, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_mode_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_mode_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_mode_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_53
    ui->Charging_Screen_label_53 = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_label_53, 15, 7);
    lv_obj_set_size(ui->Charging_Screen_label_53, 135, 23);
    lv_label_set_text(ui->Charging_Screen_label_53, "Mode");
    lv_label_set_long_mode(ui->Charging_Screen_label_53, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_53, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_53, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_53, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_53, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_53, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_53, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_53, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_time_to_charge_value
    ui->Charging_Screen_time_to_charge_value = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_time_to_charge_value, 138, 98);
    lv_obj_set_size(ui->Charging_Screen_time_to_charge_value, 125, 22);
    lv_label_set_text(ui->Charging_Screen_time_to_charge_value, "00:00:00");
    lv_label_set_long_mode(ui->Charging_Screen_time_to_charge_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_time_to_charge_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_time_to_charge_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_time_to_charge_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_time_to_charge_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_time_to_charge_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_time_to_charge_value, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Charging_Screen_time_to_charge_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Charging_Screen_time_to_charge_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_time_to_charge_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_remain
    ui->Charging_Screen_label_remain = lv_label_create(ui->Charging_Screen_cont_details);
    lv_obj_set_pos(ui->Charging_Screen_label_remain, 15, 97);
    lv_obj_set_size(ui->Charging_Screen_label_remain, 135, 23);
    lv_label_set_text(ui->Charging_Screen_label_remain, "Remaining time");
    lv_label_set_long_mode(ui->Charging_Screen_label_remain, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_remain, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_remain, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_remain, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_remain, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_remain, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_remain, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_remain, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_45
    ui->Charging_Screen_label_45 = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_label_45, 433, 250);
    lv_obj_set_size(ui->Charging_Screen_label_45, 34, 12);
    lv_label_set_text(ui->Charging_Screen_label_45, "kWh");
    lv_label_set_long_mode(ui->Charging_Screen_label_45, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_45, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_45, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_45, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_45, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_45, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_45, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_elapsed_time_value
    ui->Charging_Screen_elapsed_time_value = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_elapsed_time_value, 11, 214);
    lv_obj_set_size(ui->Charging_Screen_elapsed_time_value, 92, 16);
    lv_label_set_text(ui->Charging_Screen_elapsed_time_value, "00:00:00");
    lv_label_set_long_mode(ui->Charging_Screen_elapsed_time_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_elapsed_time_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_elapsed_time_value, lv_color_hex(0x46721c), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_elapsed_time_value, &lv_font_montserratMedium_17, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_elapsed_time_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_elapsed_time_value, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_charged1
    ui->Charging_Screen_label_charged1 = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_label_charged1, 11, 238);
    lv_obj_set_size(ui->Charging_Screen_label_charged1, 77, 26);
    lv_label_set_text(ui->Charging_Screen_label_charged1, "Charging Time (G2V)");
    lv_label_set_long_mode(ui->Charging_Screen_label_charged1, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_charged1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_charged1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_charged1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_charged1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_charged1, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_charged1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_clock
    ui->Charging_Screen_clock = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_clock, 11, 16);
    lv_obj_set_size(ui->Charging_Screen_clock, 87, 24);
    lv_label_set_text(ui->Charging_Screen_clock, "0:00");
    lv_label_set_long_mode(ui->Charging_Screen_clock, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_EVDE_ID_prompt
    ui->Charging_Screen_EVDE_ID_prompt = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_EVDE_ID_prompt, 319, 7);
    lv_obj_set_size(ui->Charging_Screen_EVDE_ID_prompt, 59, 17);
    lv_label_set_text(ui->Charging_Screen_EVDE_ID_prompt, "EVSE ID:");
    lv_label_set_long_mode(ui->Charging_Screen_EVDE_ID_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_EVDE_ID_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_EVDE_ID_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_EVDE_ID_prompt, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_EVDE_ID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_EVDE_ID_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_EVDE_ID_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_EVDE_ID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_EVSE_ID_value
    ui->Charging_Screen_EVSE_ID_value = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_EVSE_ID_value, 381, 7);
    lv_obj_set_size(ui->Charging_Screen_EVSE_ID_value, 90, 24);
    lv_label_set_text(ui->Charging_Screen_EVSE_ID_value, "xx");
    lv_label_set_long_mode(ui->Charging_Screen_EVSE_ID_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_EVSE_ID_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_EVSE_ID_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_EVSE_ID_value, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_EVSE_ID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_EVSE_ID_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_EVSE_ID_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_EVSE_ID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_vehicle_id_value
    ui->Charging_Screen_vehicle_id_value = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_vehicle_id_value, 366, 27);
    lv_obj_set_size(ui->Charging_Screen_vehicle_id_value, 105, 24);
    lv_label_set_text(ui->Charging_Screen_vehicle_id_value, "xx");
    lv_label_set_long_mode(ui->Charging_Screen_vehicle_id_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_vehicle_id_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_vehicle_id_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_vehicle_id_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_vehicle_id_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_vehicle_id_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_vehicle_id_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_label_51
    ui->Charging_Screen_label_51 = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_label_51, 319, 27);
    lv_obj_set_size(ui->Charging_Screen_label_51, 59, 16);
    lv_label_set_text(ui->Charging_Screen_label_51, "EV ID:");
    lv_label_set_long_mode(ui->Charging_Screen_label_51, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_label_51, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_label_51, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_label_51, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_label_51, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_label_51, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_label_51, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_label_51, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_home_btn
    ui->Charging_Screen_home_btn = lv_button_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_home_btn, 51, 6);
    lv_obj_set_size(ui->Charging_Screen_home_btn, 31, 31);
    lv_obj_add_flag(ui->Charging_Screen_home_btn, LV_OBJ_FLAG_CLICKABLE);
    ui->Charging_Screen_home_btn_label = lv_label_create(ui->Charging_Screen_home_btn);
    lv_label_set_text(ui->Charging_Screen_home_btn_label, "" LV_SYMBOL_HOME " ");
    lv_label_set_long_mode(ui->Charging_Screen_home_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Charging_Screen_home_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Charging_Screen_home_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Charging_Screen_home_btn_label, LV_PCT(100));

    //Write style for Charging_Screen_home_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Charging_Screen_home_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Charging_Screen_home_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_home_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_home_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_home_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_home_btn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_home_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_home_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_kWh_value
    ui->Charging_Screen_kWh_value = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_kWh_value, 400, 228);
    lv_obj_set_size(ui->Charging_Screen_kWh_value, 80, 15);
    lv_label_set_text(ui->Charging_Screen_kWh_value, "0");
    lv_label_set_long_mode(ui->Charging_Screen_kWh_value, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_kWh_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_kWh_value, lv_color_hex(0x46721c), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_kWh_value, &lv_font_montserratMedium_17, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_kWh_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_kWh_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_kWh_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Charging_Screen_NFC_activation_status
    ui->Charging_Screen_NFC_activation_status = lv_label_create(ui->Charging_Screen);
    lv_obj_set_pos(ui->Charging_Screen_NFC_activation_status, 95, 12);
    lv_obj_set_size(ui->Charging_Screen_NFC_activation_status, 24, 14);
    lv_label_set_text(ui->Charging_Screen_NFC_activation_status, "" LV_SYMBOL_WIFI " ");
    lv_label_set_long_mode(ui->Charging_Screen_NFC_activation_status, LV_LABEL_LONG_WRAP);

    //Write style for Charging_Screen_NFC_activation_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Charging_Screen_NFC_activation_status, lv_color_hex(0xc51f1f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Charging_Screen_NFC_activation_status, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Charging_Screen_NFC_activation_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Charging_Screen_NFC_activation_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Charging_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Charging_Screen.
    lv_obj_set_style_transform_angle(ui->Charging_Screen_NFC_activation_status, 900, 0);

    //Update current screen layout.
    lv_obj_update_layout(ui->Charging_Screen);

    //Init events for screen.
    events_init_Charging_Screen(ui);
}

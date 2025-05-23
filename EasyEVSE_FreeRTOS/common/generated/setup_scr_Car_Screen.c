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



void setup_scr_Car_Screen(lv_ui *ui)
{
    //Write codes Car_Screen
    ui->Car_Screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->Car_Screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->Car_Screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for Car_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Car_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_Vehicle_settings_heading
    ui->Car_Screen_Vehicle_settings_heading = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_Vehicle_settings_heading, "Vehicle Settings");
    lv_label_set_long_mode(ui->Car_Screen_Vehicle_settings_heading, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_Vehicle_settings_heading, 178, 15);
    lv_obj_set_size(ui->Car_Screen_Vehicle_settings_heading, 169, 24);

    //Write style for Car_Screen_Vehicle_settings_heading, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_Vehicle_settings_heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_Vehicle_settings_heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_Vehicle_settings_heading, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_Vehicle_settings_heading, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_Vehicle_settings_heading, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_Vehicle_settings_heading, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_Vehicle_settings_heading, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_Vehicle_settings_heading, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_Vehicle_settings_heading, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_Vehicle_settings_heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_Vehicle_settings_heading, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_Vehicle_settings_heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_Vehicle_settings_heading, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_Vehicle_settings_heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_Vehicle_settings_heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_Vehicle_settings_heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_Vehicle_settings_heading, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_ID_back_btn
    ui->Car_Screen_ID_back_btn = lv_imgbtn_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_ID_back_btn, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->Car_Screen_ID_back_btn, LV_IMGBTN_STATE_RELEASED, NULL, &_returnpng_32x28, NULL);
    lv_imgbtn_set_src(ui->Car_Screen_ID_back_btn, LV_IMGBTN_STATE_PRESSED, NULL, &_returnpng_32x28, NULL);
    lv_imgbtn_set_src(ui->Car_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_returnpng_32x28, NULL);
    lv_imgbtn_set_src(ui->Car_Screen_ID_back_btn, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_returnpng_32x28, NULL);
    ui->Car_Screen_ID_back_btn_label = lv_label_create(ui->Car_Screen_ID_back_btn);
    lv_label_set_text(ui->Car_Screen_ID_back_btn_label, "");
    lv_label_set_long_mode(ui->Car_Screen_ID_back_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Car_Screen_ID_back_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Car_Screen_ID_back_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->Car_Screen_ID_back_btn, 430, 15);
    lv_obj_set_size(ui->Car_Screen_ID_back_btn, 32, 28);
    lv_obj_add_flag(ui->Car_Screen_ID_back_btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_flag(ui->Car_Screen_ID_back_btn, LV_OBJ_FLAG_CLICK_FOCUSABLE);

    //Write style for Car_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->Car_Screen_ID_back_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_ID_back_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_ID_back_btn, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Car_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->Car_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->Car_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->Car_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->Car_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->Car_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for Car_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->Car_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->Car_Screen_ID_back_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->Car_Screen_ID_back_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->Car_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->Car_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for Car_Screen_ID_back_btn, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_ID_back_btn, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->Car_Screen_ID_back_btn, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes Car_Screen_vehicle_id_value
    ui->Car_Screen_vehicle_id_value = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_vehicle_id_value, "");
    lv_label_set_long_mode(ui->Car_Screen_vehicle_id_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_vehicle_id_value, 26, 82);
    lv_obj_set_size(ui->Car_Screen_vehicle_id_value, 120, 20);

    //Write style for Car_Screen_vehicle_id_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_vehicle_id_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_vehicle_id_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_vehicle_id_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_vehicle_id_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_vehicle_id_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_vehicle_id_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_vehicle_id_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_vehicle_id_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_vehicle_id_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_vehicle_id_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_vehicle_id_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_vehicle_id_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_vehicle_id_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_vehicle_id_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_vehicle_id_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_blue_line_top
    ui->Car_Screen_blue_line_top = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_blue_line_top[] = {{0, 0},{476, 0},};
    lv_line_set_points(ui->Car_Screen_blue_line_top, Car_Screen_blue_line_top, 2);
    lv_obj_set_pos(ui->Car_Screen_blue_line_top, 5, 2);
    lv_obj_set_size(ui->Car_Screen_blue_line_top, 470, 1);

    //Write style for Car_Screen_blue_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_blue_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_blue_line_top, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_blue_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_blue_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_blue_line_bottom
    ui->Car_Screen_blue_line_bottom = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_blue_line_bottom[] = {{0, 0},{476, 0},};
    lv_line_set_points(ui->Car_Screen_blue_line_bottom, Car_Screen_blue_line_bottom, 2);
    lv_obj_set_pos(ui->Car_Screen_blue_line_bottom, 3, 269);
    lv_obj_set_size(ui->Car_Screen_blue_line_bottom, 474, 1);

    //Write style for Car_Screen_blue_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_blue_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_blue_line_bottom, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_blue_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_blue_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_blue_line_left
    ui->Car_Screen_blue_line_left = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_blue_line_left[] = {{0, 0},{0, 268},};
    lv_line_set_points(ui->Car_Screen_blue_line_left, Car_Screen_blue_line_left, 2);
    lv_obj_set_pos(ui->Car_Screen_blue_line_left, 3, 2);
    lv_obj_set_size(ui->Car_Screen_blue_line_left, 2, 268);

    //Write style for Car_Screen_blue_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_blue_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_blue_line_left, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_blue_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_blue_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_blue_line_right
    ui->Car_Screen_blue_line_right = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_blue_line_right[] = {{0, 0},{0, 268},};
    lv_line_set_points(ui->Car_Screen_blue_line_right, Car_Screen_blue_line_right, 2);
    lv_obj_set_pos(ui->Car_Screen_blue_line_right, 477, 3);
    lv_obj_set_size(ui->Car_Screen_blue_line_right, 1, 264);

    //Write style for Car_Screen_blue_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_blue_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_blue_line_right, lv_color_hex(0x7bb1db), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_blue_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_blue_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_green_line_top
    ui->Car_Screen_green_line_top = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_green_line_top[] = {{0, 0},{472, 0},};
    lv_line_set_points(ui->Car_Screen_green_line_top, Car_Screen_green_line_top, 2);
    lv_obj_set_pos(ui->Car_Screen_green_line_top, 6, 4);
    lv_obj_set_size(ui->Car_Screen_green_line_top, 468, 1);

    //Write style for Car_Screen_green_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_green_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_green_line_top, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_green_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_green_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_green_line_bottom
    ui->Car_Screen_green_line_bottom = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_green_line_bottom[] = {{0, 0},{472, 0},};
    lv_line_set_points(ui->Car_Screen_green_line_bottom, Car_Screen_green_line_bottom, 2);
    lv_obj_set_pos(ui->Car_Screen_green_line_bottom, 5, 267);
    lv_obj_set_size(ui->Car_Screen_green_line_bottom, 469, 1);

    //Write style for Car_Screen_green_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_green_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_green_line_bottom, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_green_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_green_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_green_line_left
    ui->Car_Screen_green_line_left = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_green_line_left[] = {{0, 0},{0, 264},};
    lv_line_set_points(ui->Car_Screen_green_line_left, Car_Screen_green_line_left, 2);
    lv_obj_set_pos(ui->Car_Screen_green_line_left, 5, 4);
    lv_obj_set_size(ui->Car_Screen_green_line_left, 1, 262);

    //Write style for Car_Screen_green_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_green_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_green_line_left, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_green_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_green_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_green_line_right
    ui->Car_Screen_green_line_right = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_green_line_right[] = {{0, 0},{0, 264},};
    lv_line_set_points(ui->Car_Screen_green_line_right, Car_Screen_green_line_right, 2);
    lv_obj_set_pos(ui->Car_Screen_green_line_right, 475, 4);
    lv_obj_set_size(ui->Car_Screen_green_line_right, 1, 260);

    //Write style for Car_Screen_green_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_green_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_green_line_right, lv_color_hex(0xc9d200), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_green_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_green_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_orange_line_top
    ui->Car_Screen_orange_line_top = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_orange_line_top[] = {{0, 0},{476, 0},};
    lv_line_set_points(ui->Car_Screen_orange_line_top, Car_Screen_orange_line_top, 2);
    lv_obj_set_pos(ui->Car_Screen_orange_line_top, 1, 1);
    lv_obj_set_size(ui->Car_Screen_orange_line_top, 476, 1);

    //Write style for Car_Screen_orange_line_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_orange_line_top, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_orange_line_top, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_orange_line_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_orange_line_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_orange_line_bottom
    ui->Car_Screen_orange_line_bottom = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_orange_line_bottom[] = {{0, 0},{478, 0},};
    lv_line_set_points(ui->Car_Screen_orange_line_bottom, Car_Screen_orange_line_bottom, 2);
    lv_obj_set_pos(ui->Car_Screen_orange_line_bottom, 1, 271);
    lv_obj_set_size(ui->Car_Screen_orange_line_bottom, 479, 1);

    //Write style for Car_Screen_orange_line_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_orange_line_bottom, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_orange_line_bottom, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_orange_line_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_orange_line_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_orange_line_left
    ui->Car_Screen_orange_line_left = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_orange_line_left[] = {{0, 0},{0, 270},};
    lv_line_set_points(ui->Car_Screen_orange_line_left, Car_Screen_orange_line_left, 2);
    lv_obj_set_pos(ui->Car_Screen_orange_line_left, 1, 2);
    lv_obj_set_size(ui->Car_Screen_orange_line_left, 1, 268);

    //Write style for Car_Screen_orange_line_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_orange_line_left, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_orange_line_left, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_orange_line_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_orange_line_left, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_orange_line_right
    ui->Car_Screen_orange_line_right = lv_line_create(ui->Car_Screen);
    static lv_point_t Car_Screen_orange_line_right[] = {{0, 0},{0, 268},};
    lv_line_set_points(ui->Car_Screen_orange_line_right, Car_Screen_orange_line_right, 2);
    lv_obj_set_pos(ui->Car_Screen_orange_line_right, 479, 1);
    lv_obj_set_size(ui->Car_Screen_orange_line_right, 1, 268);

    //Write style for Car_Screen_orange_line_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_line_width(ui->Car_Screen_orange_line_right, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_color(ui->Car_Screen_orange_line_right, lv_color_hex(0xf9b500), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_opa(ui->Car_Screen_orange_line_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_line_rounded(ui->Car_Screen_orange_line_right, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_requested_energy_value
    ui->Car_Screen_requested_energy_value = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_requested_energy_value, "");
    lv_label_set_long_mode(ui->Car_Screen_requested_energy_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_requested_energy_value, 356, 122);
    lv_obj_set_size(ui->Car_Screen_requested_energy_value, 90, 15);
    lv_obj_add_flag(ui->Car_Screen_requested_energy_value, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_requested_energy_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_requested_energy_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_requested_energy_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_requested_energy_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_requested_energy_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_requested_energy_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_requested_energy_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_requested_energy_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_requested_energy_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_requested_energy_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_requested_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_requested_energy_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_requested_energy_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_requested_energy_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_requested_energy_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_requested_energy_value, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_requested_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_requested_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_requested_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_requested_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_delivered_energy_value
    ui->Car_Screen_delivered_energy_value = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_delivered_energy_value, "");
    lv_label_set_long_mode(ui->Car_Screen_delivered_energy_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_delivered_energy_value, 356, 158);
    lv_obj_set_size(ui->Car_Screen_delivered_energy_value, 90, 15);
    lv_obj_add_flag(ui->Car_Screen_delivered_energy_value, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_delivered_energy_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_delivered_energy_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_delivered_energy_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_delivered_energy_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_delivered_energy_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_delivered_energy_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_delivered_energy_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_delivered_energy_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_delivered_energy_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_delivered_energy_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_delivered_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_delivered_energy_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_delivered_energy_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_delivered_energy_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_delivered_energy_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_delivered_energy_value, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_delivered_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_delivered_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_delivered_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_delivered_energy_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_delivered_energy_prompt
    ui->Car_Screen_delivered_energy_prompt = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_delivered_energy_prompt, "Delivered\nEnergy [W]");
    lv_label_set_long_mode(ui->Car_Screen_delivered_energy_prompt, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_delivered_energy_prompt, 265, 149);
    lv_obj_set_size(ui->Car_Screen_delivered_energy_prompt, 80, 33);
    lv_obj_add_flag(ui->Car_Screen_delivered_energy_prompt, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_delivered_energy_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_delivered_energy_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_delivered_energy_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_delivered_energy_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_delivered_energy_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_delivered_energy_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_delivered_energy_prompt, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_delivered_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_protocol_value
    ui->Car_Screen_protocol_value = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_protocol_value, "");
    lv_label_set_long_mode(ui->Car_Screen_protocol_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_protocol_value, 304, 82);
    lv_obj_set_size(ui->Car_Screen_protocol_value, 156, 20);

    //Write style for Car_Screen_protocol_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_protocol_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_protocol_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_protocol_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_protocol_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_protocol_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_protocol_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_protocol_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_protocol_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_protocol_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_protocol_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_protocol_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_protocol_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_protocol_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_protocol_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_protocol_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_requested_energy_prompt
    ui->Car_Screen_requested_energy_prompt = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_requested_energy_prompt, "Requested\nEnergy [W]");
    lv_label_set_long_mode(ui->Car_Screen_requested_energy_prompt, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_requested_energy_prompt, 265, 110);
    lv_obj_set_size(ui->Car_Screen_requested_energy_prompt, 80, 36);
    lv_obj_add_flag(ui->Car_Screen_requested_energy_prompt, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_requested_energy_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_requested_energy_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_requested_energy_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_requested_energy_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_requested_energy_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_requested_energy_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_requested_energy_prompt, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_requested_energy_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_vehicle_id_prompt
    ui->Car_Screen_vehicle_id_prompt = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_vehicle_id_prompt, "Vehicle ID");
    lv_label_set_long_mode(ui->Car_Screen_vehicle_id_prompt, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_vehicle_id_prompt, 46, 50);
    lv_obj_set_size(ui->Car_Screen_vehicle_id_prompt, 80, 24);

    //Write style for Car_Screen_vehicle_id_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_vehicle_id_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_vehicle_id_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_vehicle_id_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_vehicle_id_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_vehicle_id_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_vehicle_id_prompt, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_vehicle_id_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_protocol_prompt
    ui->Car_Screen_protocol_prompt = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_protocol_prompt, "Protocol");
    lv_label_set_long_mode(ui->Car_Screen_protocol_prompt, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_protocol_prompt, 345, 50);
    lv_obj_set_size(ui->Car_Screen_protocol_prompt, 80, 18);

    //Write style for Car_Screen_protocol_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_protocol_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_protocol_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_protocol_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_protocol_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_protocol_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_protocol_prompt, 7, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_protocol_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_charge_current_value
    ui->Car_Screen_charge_current_value = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_charge_current_value, "");
    lv_label_set_long_mode(ui->Car_Screen_charge_current_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_charge_current_value, 181, 82);
    lv_obj_set_size(ui->Car_Screen_charge_current_value, 90, 20);

    //Write style for Car_Screen_charge_current_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_charge_current_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_charge_current_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_charge_current_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_charge_current_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_charge_current_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_charge_current_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_charge_current_value, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_charge_current_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_charge_current_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_charge_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_charge_current_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_charge_current_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_charge_current_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_charge_current_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_charge_current_value, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_charge_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_charge_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_charge_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_charge_current_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_charge_current_prompt
    ui->Car_Screen_charge_current_prompt = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_charge_current_prompt, "Charge Current [A]");
    lv_label_set_long_mode(ui->Car_Screen_charge_current_prompt, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_charge_current_prompt, 159, 50);
    lv_obj_set_size(ui->Car_Screen_charge_current_prompt, 140, 32);

    //Write style for Car_Screen_charge_current_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_charge_current_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_charge_current_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_charge_current_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_charge_current_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_charge_current_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_charge_current_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_charge_current_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_elapsed_time_value
    ui->Car_Screen_elapsed_time_value = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_elapsed_time_value, "0 : 00");
    lv_label_set_long_mode(ui->Car_Screen_elapsed_time_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_elapsed_time_value, 146, 119);
    lv_obj_set_size(ui->Car_Screen_elapsed_time_value, 90, 15);

    //Write style for Car_Screen_elapsed_time_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_elapsed_time_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_elapsed_time_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_elapsed_time_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_elapsed_time_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_elapsed_time_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_elapsed_time_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_elapsed_time_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_elapsed_time_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_elapsed_time_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_elapsed_time_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_elapsed_time_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_elapsed_time_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_elapsed_time_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_elapsed_time_value, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_elapsed_time_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_elapsed_time_prompt
    ui->Car_Screen_elapsed_time_prompt = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_elapsed_time_prompt, "Elapsed Time");
    lv_label_set_long_mode(ui->Car_Screen_elapsed_time_prompt, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_elapsed_time_prompt, 18, 113);
    lv_obj_set_size(ui->Car_Screen_elapsed_time_prompt, 124, 24);

    //Write style for Car_Screen_elapsed_time_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_elapsed_time_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_elapsed_time_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_elapsed_time_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_elapsed_time_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_elapsed_time_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_elapsed_time_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_elapsed_time_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_time_to_charge_prompt
    ui->Car_Screen_time_to_charge_prompt = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_time_to_charge_prompt, "Time to Charge");
    lv_label_set_long_mode(ui->Car_Screen_time_to_charge_prompt, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_time_to_charge_prompt, 18, 153);
    lv_obj_set_size(ui->Car_Screen_time_to_charge_prompt, 124, 24);

    //Write style for Car_Screen_time_to_charge_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_time_to_charge_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_time_to_charge_prompt, &lv_font_montserratMedium_10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_time_to_charge_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_time_to_charge_prompt, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_time_to_charge_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_time_to_charge_prompt, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_time_to_charge_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_time_to_charge_value
    ui->Car_Screen_time_to_charge_value = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_time_to_charge_value, "0 : 00");
    lv_label_set_long_mode(ui->Car_Screen_time_to_charge_value, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_time_to_charge_value, 146, 158);
    lv_obj_set_size(ui->Car_Screen_time_to_charge_value, 90, 15);

    //Write style for Car_Screen_time_to_charge_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_time_to_charge_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Car_Screen_time_to_charge_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Car_Screen_time_to_charge_value, lv_color_hex(0xb1b1b1), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Car_Screen_time_to_charge_value, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_time_to_charge_value, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_time_to_charge_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_time_to_charge_value, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_time_to_charge_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_time_to_charge_value, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_time_to_charge_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_time_to_charge_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_time_to_charge_value, lv_color_hex(0xebebeb), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_time_to_charge_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_time_to_charge_value, 3, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_time_to_charge_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_clock
    ui->Car_Screen_clock = lv_label_create(ui->Car_Screen);
    lv_label_set_text(ui->Car_Screen_clock, "0:00");
    lv_label_set_long_mode(ui->Car_Screen_clock, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->Car_Screen_clock, 15, 17);
    lv_obj_set_size(ui->Car_Screen_clock, 87, 24);

    //Write style for Car_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Car_Screen_clock, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_img_1
    ui->Car_Screen_img_1 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_img_1, &_charging_station_alpha_68x66);
    lv_img_set_pivot(ui->Car_Screen_img_1, 50,50);
    lv_img_set_angle(ui->Car_Screen_img_1, 0);
    lv_obj_set_pos(ui->Car_Screen_img_1, 46, 186);
    lv_obj_set_size(ui->Car_Screen_img_1, 68, 66);

    //Write style for Car_Screen_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_img_2
    ui->Car_Screen_img_2 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_img_2, &_EV_alpha_64x56);
    lv_img_set_pivot(ui->Car_Screen_img_2, 50,50);
    lv_img_set_angle(ui->Car_Screen_img_2, 0);
    lv_obj_set_pos(ui->Car_Screen_img_2, 265, 198);
    lv_obj_set_size(ui->Car_Screen_img_2, 64, 56);

    //Write style for Car_Screen_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_stop_resume_btn
    ui->Car_Screen_stop_resume_btn = lv_btn_create(ui->Car_Screen);
    ui->Car_Screen_stop_resume_btn_label = lv_label_create(ui->Car_Screen_stop_resume_btn);
    lv_label_set_text(ui->Car_Screen_stop_resume_btn_label, "");
    lv_label_set_long_mode(ui->Car_Screen_stop_resume_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Car_Screen_stop_resume_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Car_Screen_stop_resume_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Car_Screen_stop_resume_btn_label, LV_PCT(100));
    lv_obj_set_pos(ui->Car_Screen_stop_resume_btn, 356, 208);
    lv_obj_set_size(ui->Car_Screen_stop_resume_btn, 84, 34);
    lv_obj_add_flag(ui->Car_Screen_stop_resume_btn, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_stop_resume_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Car_Screen_stop_resume_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Car_Screen_stop_resume_btn, lv_color_hex(0xdddd00), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Car_Screen_stop_resume_btn, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Car_Screen_stop_resume_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_stop_resume_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Car_Screen_stop_resume_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Car_Screen_stop_resume_btn, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Car_Screen_stop_resume_btn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Car_Screen_stop_resume_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Car_Screen_stop_resume_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_v2g_img_1
    ui->Car_Screen_v2g_img_1 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_v2g_img_1, &_v2g_Blue_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_v2g_img_1, 50,50);
    lv_img_set_angle(ui->Car_Screen_v2g_img_1, 0);
    lv_obj_set_pos(ui->Car_Screen_v2g_img_1, 123, 214);
    lv_obj_set_size(ui->Car_Screen_v2g_img_1, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_v2g_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_v2g_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_v2g_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_v2g_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_v2g_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_v2g_img_2
    ui->Car_Screen_v2g_img_2 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_v2g_img_2, &_v2g_Blue_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_v2g_img_2, 50,50);
    lv_img_set_angle(ui->Car_Screen_v2g_img_2, 0);
    lv_obj_set_pos(ui->Car_Screen_v2g_img_2, 159, 214);
    lv_obj_set_size(ui->Car_Screen_v2g_img_2, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_v2g_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_v2g_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_v2g_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_v2g_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_v2g_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_v2g_img_3
    ui->Car_Screen_v2g_img_3 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_v2g_img_3, &_v2g_Blue_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_v2g_img_3, 50,50);
    lv_img_set_angle(ui->Car_Screen_v2g_img_3, 0);
    lv_obj_set_pos(ui->Car_Screen_v2g_img_3, 193, 214);
    lv_obj_set_size(ui->Car_Screen_v2g_img_3, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_v2g_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_v2g_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_v2g_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_v2g_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_v2g_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_v2g_img_4
    ui->Car_Screen_v2g_img_4 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_v2g_img_4, &_v2g_Blue_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_v2g_img_4, 50,50);
    lv_img_set_angle(ui->Car_Screen_v2g_img_4, 0);
    lv_obj_set_pos(ui->Car_Screen_v2g_img_4, 228, 214);
    lv_obj_set_size(ui->Car_Screen_v2g_img_4, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_v2g_img_4, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_v2g_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_v2g_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_v2g_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_v2g_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_v2g_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_g2v_img_1
    ui->Car_Screen_g2v_img_1 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_g2v_img_1, &_g2v_Green_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_g2v_img_1, 50,50);
    lv_img_set_angle(ui->Car_Screen_g2v_img_1, 0);
    lv_obj_set_pos(ui->Car_Screen_g2v_img_1, 123, 214);
    lv_obj_set_size(ui->Car_Screen_g2v_img_1, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_g2v_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_g2v_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_g2v_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_g2v_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_g2v_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_g2v_img_2
    ui->Car_Screen_g2v_img_2 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_g2v_img_2, &_g2v_Green_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_g2v_img_2, 50,50);
    lv_img_set_angle(ui->Car_Screen_g2v_img_2, 0);
    lv_obj_set_pos(ui->Car_Screen_g2v_img_2, 159, 214);
    lv_obj_set_size(ui->Car_Screen_g2v_img_2, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_2, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_g2v_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_g2v_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_g2v_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_g2v_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_g2v_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_g2v_img_3
    ui->Car_Screen_g2v_img_3 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_3, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_g2v_img_3, &_g2v_Green_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_g2v_img_3, 50,50);
    lv_img_set_angle(ui->Car_Screen_g2v_img_3, 0);
    lv_obj_set_pos(ui->Car_Screen_g2v_img_3, 193, 214);
    lv_obj_set_size(ui->Car_Screen_g2v_img_3, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_3, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_g2v_img_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_g2v_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_g2v_img_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_g2v_img_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_g2v_img_3, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Car_Screen_g2v_img_4
    ui->Car_Screen_g2v_img_4 = lv_img_create(ui->Car_Screen);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_4, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->Car_Screen_g2v_img_4, &_g2v_Green_250x200_25x20);
    lv_img_set_pivot(ui->Car_Screen_g2v_img_4, 50,50);
    lv_img_set_angle(ui->Car_Screen_g2v_img_4, 0);
    lv_obj_set_pos(ui->Car_Screen_g2v_img_4, 228, 214);
    lv_obj_set_size(ui->Car_Screen_g2v_img_4, 25, 20);
    lv_obj_add_flag(ui->Car_Screen_g2v_img_4, LV_OBJ_FLAG_HIDDEN);

    //Write style for Car_Screen_g2v_img_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->Car_Screen_g2v_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->Car_Screen_g2v_img_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Car_Screen_g2v_img_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->Car_Screen_g2v_img_4, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Car_Screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->Car_Screen);

    //Init events for screen.
    events_init_Car_Screen(ui);
}

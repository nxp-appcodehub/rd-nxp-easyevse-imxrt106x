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



void setup_scr_Linking_Screen(lv_ui *ui)
{
    //Write codes Linking_Screen
    ui->Linking_Screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->Linking_Screen, 480, 272);
    lv_obj_set_scrollbar_mode(ui->Linking_Screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for Linking_Screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Linking_Screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui->Linking_Screen, &_linking_bak_RGB565_480x272, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(ui->Linking_Screen, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor_opa(ui->Linking_Screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_cont_linked
    ui->Linking_Screen_cont_linked = lv_obj_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_cont_linked, -464, 17);
    lv_obj_set_size(ui->Linking_Screen_cont_linked, 300, 230);
    lv_obj_set_scrollbar_mode(ui->Linking_Screen_cont_linked, LV_SCROLLBAR_MODE_OFF);

    //Write style for Linking_Screen_cont_linked, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_cont_linked, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_cont_linked, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_cont_linked, 232, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_cont_linked, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_cont_linked, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_cont_linked, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_cont_linked, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_cont_linked, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_cont_linked, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_cont_linked, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Linking_Screen_cont_linked, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Linking_Screen_cont_linked, 175, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Linking_Screen_cont_linked, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Linking_Screen_cont_linked, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Linking_Screen_cont_linked, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_SIGBOARD_connection_prompt
    ui->Linking_Screen_SIGBOARD_connection_prompt = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_SIGBOARD_connection_prompt, 16, 49);
    lv_obj_set_size(ui->Linking_Screen_SIGBOARD_connection_prompt, 135, 23);
    lv_label_set_text(ui->Linking_Screen_SIGBOARD_connection_prompt, "SIGBOARD connection");
    lv_label_set_long_mode(ui->Linking_Screen_SIGBOARD_connection_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_SIGBOARD_connection_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_SIGBOARD_connection_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_SIGBOARD_connection_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_SIGBOARD_connection_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_SIGBOARD_connection_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_SIGBOARD_connection_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_SIGBOARD_connection_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_SIGBOARD_connection_value
    ui->Linking_Screen_SIGBOARD_connection_value = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_SIGBOARD_connection_value, 153, 48);
    lv_obj_set_size(ui->Linking_Screen_SIGBOARD_connection_value, 125, 22);
    lv_label_set_text(ui->Linking_Screen_SIGBOARD_connection_value, "Arduino header");
    lv_label_set_long_mode(ui->Linking_Screen_SIGBOARD_connection_value, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_SIGBOARD_connection_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_SIGBOARD_connection_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_SIGBOARD_connection_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_SIGBOARD_connection_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_SIGBOARD_connection_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_SIGBOARD_connection_value, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_SIGBOARD_connection_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_SIGBOARD_connection_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_SIGBOARD_connection_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_SIGBOARD_connection_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_card_UID_prompt
    ui->Linking_Screen_card_UID_prompt = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_card_UID_prompt, 16, 117);
    lv_obj_set_size(ui->Linking_Screen_card_UID_prompt, 135, 23);
    lv_label_set_text(ui->Linking_Screen_card_UID_prompt, "Card UID");
    lv_label_set_long_mode(ui->Linking_Screen_card_UID_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_card_UID_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_card_UID_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_card_UID_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_card_UID_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_card_UID_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_card_UID_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_card_UID_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_card_UID_value
    ui->Linking_Screen_card_UID_value = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_card_UID_value, 154, 117);
    lv_obj_set_size(ui->Linking_Screen_card_UID_value, 125, 22);
    lv_label_set_text(ui->Linking_Screen_card_UID_value, "N/A");
    lv_label_set_long_mode(ui->Linking_Screen_card_UID_value, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_card_UID_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_card_UID_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_card_UID_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_card_UID_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_card_UID_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_card_UID_value, 218, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_card_UID_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_card_UID_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_card_UID_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_card_UID_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_card_type_value
    ui->Linking_Screen_card_type_value = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_card_type_value, 154, 147);
    lv_obj_set_size(ui->Linking_Screen_card_type_value, 125, 22);
    lv_label_set_text(ui->Linking_Screen_card_type_value, "");
    lv_label_set_long_mode(ui->Linking_Screen_card_type_value, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_card_type_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_card_type_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_card_type_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_card_type_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_card_type_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_card_type_value, 218, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_card_type_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_card_type_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_card_type_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_card_type_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_card_type_prompt
    ui->Linking_Screen_card_type_prompt = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_card_type_prompt, 16, 147);
    lv_obj_set_size(ui->Linking_Screen_card_type_prompt, 135, 23);
    lv_label_set_text(ui->Linking_Screen_card_type_prompt, "Card type");
    lv_label_set_long_mode(ui->Linking_Screen_card_type_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_card_type_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_card_type_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_card_type_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_card_type_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_card_type_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_card_type_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_card_type_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_card_security_value
    ui->Linking_Screen_card_security_value = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_card_security_value, 154, 177);
    lv_obj_set_size(ui->Linking_Screen_card_security_value, 125, 41);
    lv_label_set_text(ui->Linking_Screen_card_security_value, "");
    lv_label_set_long_mode(ui->Linking_Screen_card_security_value, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_card_security_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_card_security_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_card_security_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_card_security_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_card_security_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_card_security_value, 217, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_card_security_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_card_security_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_card_security_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_card_security_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_card_security_prompt
    ui->Linking_Screen_card_security_prompt = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_card_security_prompt, 16, 177);
    lv_obj_set_size(ui->Linking_Screen_card_security_prompt, 135, 23);
    lv_label_set_text(ui->Linking_Screen_card_security_prompt, "Transaction status");
    lv_label_set_long_mode(ui->Linking_Screen_card_security_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_card_security_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_card_security_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_card_security_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_card_security_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_card_security_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_card_security_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_card_security_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_close_btn
    ui->Linking_Screen_close_btn = lv_button_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_close_btn, 261, 10);
    lv_obj_set_size(ui->Linking_Screen_close_btn, 29, 29);
    lv_obj_add_flag(ui->Linking_Screen_close_btn, LV_OBJ_FLAG_CLICKABLE);
    ui->Linking_Screen_close_btn_label = lv_label_create(ui->Linking_Screen_close_btn);
    lv_label_set_text(ui->Linking_Screen_close_btn_label, "" LV_SYMBOL_CLOSE " ");
    lv_label_set_long_mode(ui->Linking_Screen_close_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Linking_Screen_close_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Linking_Screen_close_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Linking_Screen_close_btn_label, LV_PCT(100));

    //Write style for Linking_Screen_close_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Linking_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Linking_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_close_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_close_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_close_btn, lv_color_hex(0x656565), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_close_btn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_close_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_close_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_mode_value
    ui->Linking_Screen_mode_value = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_mode_value, 153, 79);
    lv_obj_set_size(ui->Linking_Screen_mode_value, 125, 22);
    lv_label_set_text(ui->Linking_Screen_mode_value, "");
    lv_label_set_long_mode(ui->Linking_Screen_mode_value, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_mode_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_mode_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_mode_value, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_mode_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_mode_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_mode_value, 192, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_mode_value, lv_color_hex(0xcfcfcf), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_mode_value, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_mode_value, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_mode_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_mode_prompt
    ui->Linking_Screen_mode_prompt = lv_label_create(ui->Linking_Screen_cont_linked);
    lv_obj_set_pos(ui->Linking_Screen_mode_prompt, 15, 79);
    lv_obj_set_size(ui->Linking_Screen_mode_prompt, 135, 23);
    lv_label_set_text(ui->Linking_Screen_mode_prompt, "Mode");
    lv_label_set_long_mode(ui->Linking_Screen_mode_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_mode_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_mode_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_mode_prompt, &lv_font_montserratMedium_11, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_mode_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_mode_prompt, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_mode_prompt, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_mode_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_clock
    ui->Linking_Screen_clock = lv_label_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_clock, 14, 17);
    lv_obj_set_size(ui->Linking_Screen_clock, 87, 24);
    lv_label_set_text(ui->Linking_Screen_clock, "0:00");
    lv_label_set_long_mode(ui->Linking_Screen_clock, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_clock, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_clock, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_clock, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_clock, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_clock, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_clock, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_img_slide
    ui->Linking_Screen_img_slide = lv_image_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_img_slide, 17, 122);
    lv_obj_set_size(ui->Linking_Screen_img_slide, 22, 13);
    lv_obj_add_flag(ui->Linking_Screen_img_slide, LV_OBJ_FLAG_CLICKABLE);
    lv_image_set_src(ui->Linking_Screen_img_slide, &_slide_RGB565A8_22x13);
    lv_image_set_pivot(ui->Linking_Screen_img_slide, 50,50);
    lv_image_set_rotation(ui->Linking_Screen_img_slide, 0);

    //Write style for Linking_Screen_img_slide, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_image_recolor_opa(ui->Linking_Screen_img_slide, 110, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_recolor(ui->Linking_Screen_img_slide, lv_color_hex(0x070303), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_image_opa(ui->Linking_Screen_img_slide, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_cont_1
    ui->Linking_Screen_cont_1 = lv_obj_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_cont_1, 241, 6);
    lv_obj_set_size(ui->Linking_Screen_cont_1, 222, 36);
    lv_obj_set_scrollbar_mode(ui->Linking_Screen_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for Linking_Screen_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_cont_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Linking_Screen_cont_1, lv_color_hex(0x315b7f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Linking_Screen_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_ISO15118_status_prompt
    ui->Linking_Screen_ISO15118_status_prompt = lv_label_create(ui->Linking_Screen_cont_1);
    lv_obj_set_pos(ui->Linking_Screen_ISO15118_status_prompt, 5, 1);
    lv_obj_set_size(ui->Linking_Screen_ISO15118_status_prompt, 96, 18);
    lv_label_set_text(ui->Linking_Screen_ISO15118_status_prompt, "ISO15118 Status:");
    lv_label_set_long_mode(ui->Linking_Screen_ISO15118_status_prompt, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_ISO15118_status_prompt, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_ISO15118_status_prompt, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_ISO15118_status_prompt, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_ISO15118_status_prompt, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_ISO15118_status_prompt, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_ISO15118_status_prompt, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_ISO15118_status_value
    ui->Linking_Screen_ISO15118_status_value = lv_label_create(ui->Linking_Screen_cont_1);
    lv_obj_set_pos(ui->Linking_Screen_ISO15118_status_value, 117, 1);
    lv_obj_set_size(ui->Linking_Screen_ISO15118_status_value, 98, 28);
    lv_label_set_text(ui->Linking_Screen_ISO15118_status_value, "");
    lv_label_set_long_mode(ui->Linking_Screen_ISO15118_status_value, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_ISO15118_status_value, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_ISO15118_status_value, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_ISO15118_status_value, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_ISO15118_status_value, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_ISO15118_status_value, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_ISO15118_status_value, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_cont_2
    ui->Linking_Screen_cont_2 = lv_obj_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_cont_2, 425, 180);
    lv_obj_set_size(ui->Linking_Screen_cont_2, 52, 89);
    lv_obj_set_scrollbar_mode(ui->Linking_Screen_cont_2, LV_SCROLLBAR_MODE_OFF);

    //Write style for Linking_Screen_cont_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_cont_2, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->Linking_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->Linking_Screen_cont_2, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->Linking_Screen_cont_2, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_cont_2, 114, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_cont_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_cont_2, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_cont_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_register_card_btn
    ui->Linking_Screen_register_card_btn = lv_button_create(ui->Linking_Screen_cont_2);
    lv_obj_set_pos(ui->Linking_Screen_register_card_btn, 10, 13);
    lv_obj_set_size(ui->Linking_Screen_register_card_btn, 29, 27);
    ui->Linking_Screen_register_card_btn_label = lv_label_create(ui->Linking_Screen_register_card_btn);
    lv_label_set_text(ui->Linking_Screen_register_card_btn_label, "" LV_SYMBOL_PLUS "");
    lv_label_set_long_mode(ui->Linking_Screen_register_card_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Linking_Screen_register_card_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Linking_Screen_register_card_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Linking_Screen_register_card_btn_label, LV_PCT(100));

    //Write style for Linking_Screen_register_card_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Linking_Screen_register_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Linking_Screen_register_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_register_card_btn, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_register_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui->Linking_Screen_register_card_btn, &_start_btn_RGB565A8_29x27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(ui->Linking_Screen_register_card_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor_opa(ui->Linking_Screen_register_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_register_card_btn, lv_color_hex(0x08375a), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_register_card_btn, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_register_card_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_register_card_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_deregister_card_btn
    ui->Linking_Screen_deregister_card_btn = lv_button_create(ui->Linking_Screen_cont_2);
    lv_obj_set_pos(ui->Linking_Screen_deregister_card_btn, 10, 58);
    lv_obj_set_size(ui->Linking_Screen_deregister_card_btn, 29, 27);
    ui->Linking_Screen_deregister_card_btn_label = lv_label_create(ui->Linking_Screen_deregister_card_btn);
    lv_label_set_text(ui->Linking_Screen_deregister_card_btn_label, " " LV_SYMBOL_MINUS " ");
    lv_label_set_long_mode(ui->Linking_Screen_deregister_card_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Linking_Screen_deregister_card_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Linking_Screen_deregister_card_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Linking_Screen_deregister_card_btn_label, LV_PCT(100));

    //Write style for Linking_Screen_deregister_card_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Linking_Screen_deregister_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Linking_Screen_deregister_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_deregister_card_btn, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_deregister_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_src(ui->Linking_Screen_deregister_card_btn, &_start_btn_RGB565A8_29x27, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_opa(ui->Linking_Screen_deregister_card_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_image_recolor_opa(ui->Linking_Screen_deregister_card_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_deregister_card_btn, lv_color_hex(0x08375a), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_deregister_card_btn, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_deregister_card_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_deregister_card_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_label_1
    ui->Linking_Screen_label_1 = lv_label_create(ui->Linking_Screen_cont_2);
    lv_obj_set_pos(ui->Linking_Screen_label_1, 5, 0);
    lv_obj_set_size(ui->Linking_Screen_label_1, 40, 15);
    lv_label_set_text(ui->Linking_Screen_label_1, "Register card");
    lv_label_set_long_mode(ui->Linking_Screen_label_1, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_label_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_label_1, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_label_2
    ui->Linking_Screen_label_2 = lv_label_create(ui->Linking_Screen_cont_2);
    lv_obj_set_pos(ui->Linking_Screen_label_2, 2, 42);
    lv_obj_set_size(ui->Linking_Screen_label_2, 44, 15);
    lv_label_set_text(ui->Linking_Screen_label_2, "Deregister card");
    lv_label_set_long_mode(ui->Linking_Screen_label_2, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_label_2, &lv_font_montserratMedium_8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_error_cont
    ui->Linking_Screen_error_cont = lv_obj_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_error_cont, -121, -154);
    lv_obj_set_size(ui->Linking_Screen_error_cont, 291, 144);
    lv_obj_set_scrollbar_mode(ui->Linking_Screen_error_cont, LV_SCROLLBAR_MODE_OFF);

    //Write style for Linking_Screen_error_cont, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_error_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_error_cont, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_error_cont, 230, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->Linking_Screen_error_cont, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->Linking_Screen_error_cont, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_error_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_error_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_error_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_error_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_error_cont, 9, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui->Linking_Screen_error_cont, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui->Linking_Screen_error_cont, 175, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_spread(ui->Linking_Screen_error_cont, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_x(ui->Linking_Screen_error_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_offset_y(ui->Linking_Screen_error_cont, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_error_status
    ui->Linking_Screen_error_status = lv_label_create(ui->Linking_Screen_error_cont);
    lv_obj_set_pos(ui->Linking_Screen_error_status, 34, 48);
    lv_obj_set_size(ui->Linking_Screen_error_status, 219, 67);
    lv_label_set_text(ui->Linking_Screen_error_status, "");
    lv_label_set_long_mode(ui->Linking_Screen_error_status, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_error_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_error_status, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_error_status, &lv_font_montserratMedium_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_error_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_error_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_error_status, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_error_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_closebtn
    ui->Linking_Screen_closebtn = lv_button_create(ui->Linking_Screen_error_cont);
    lv_obj_set_pos(ui->Linking_Screen_closebtn, 261, 10);
    lv_obj_set_size(ui->Linking_Screen_closebtn, 29, 29);
    lv_obj_add_flag(ui->Linking_Screen_closebtn, LV_OBJ_FLAG_CLICKABLE);
    ui->Linking_Screen_closebtn_label = lv_label_create(ui->Linking_Screen_closebtn);
    lv_label_set_text(ui->Linking_Screen_closebtn_label, "" LV_SYMBOL_CLOSE " ");
    lv_label_set_long_mode(ui->Linking_Screen_closebtn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Linking_Screen_closebtn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Linking_Screen_closebtn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Linking_Screen_closebtn_label, LV_PCT(100));

    //Write style for Linking_Screen_closebtn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Linking_Screen_closebtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Linking_Screen_closebtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_closebtn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_closebtn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_closebtn, lv_color_hex(0x656565), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_closebtn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_closebtn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_closebtn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_home_btn
    ui->Linking_Screen_home_btn = lv_button_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_home_btn, 51, 6);
    lv_obj_set_size(ui->Linking_Screen_home_btn, 31, 31);
    lv_obj_add_flag(ui->Linking_Screen_home_btn, LV_OBJ_FLAG_CLICKABLE);
    ui->Linking_Screen_home_btn_label = lv_label_create(ui->Linking_Screen_home_btn);
    lv_label_set_text(ui->Linking_Screen_home_btn_label, "" LV_SYMBOL_HOME " ");
    lv_label_set_long_mode(ui->Linking_Screen_home_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Linking_Screen_home_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Linking_Screen_home_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->Linking_Screen_home_btn_label, LV_PCT(100));

    //Write style for Linking_Screen_home_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->Linking_Screen_home_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->Linking_Screen_home_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_home_btn, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_home_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_home_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_home_btn, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_home_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_home_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes Linking_Screen_next_btn
    ui->Linking_Screen_next_btn = lv_imagebutton_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_next_btn, 453, 122);
    lv_obj_set_size(ui->Linking_Screen_next_btn, 22, 13);
    lv_obj_add_flag(ui->Linking_Screen_next_btn, LV_OBJ_FLAG_CHECKABLE);
    lv_imagebutton_set_src(ui->Linking_Screen_next_btn, LV_IMAGEBUTTON_STATE_RELEASED, &_slide_RGB565A8_22x13, NULL, NULL);
    ui->Linking_Screen_next_btn_label = lv_label_create(ui->Linking_Screen_next_btn);
    lv_label_set_text(ui->Linking_Screen_next_btn_label, "");
    lv_label_set_long_mode(ui->Linking_Screen_next_btn_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->Linking_Screen_next_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->Linking_Screen_next_btn, 0, LV_STATE_DEFAULT);

    //Write style for Linking_Screen_next_btn, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->Linking_Screen_next_btn, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_next_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_next_btn, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_next_btn, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_next_btn, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for Linking_Screen_next_btn, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_image_recolor_opa(ui->Linking_Screen_next_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_image_opa(ui->Linking_Screen_next_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->Linking_Screen_next_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->Linking_Screen_next_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->Linking_Screen_next_btn, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_next_btn, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for Linking_Screen_next_btn, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_image_recolor_opa(ui->Linking_Screen_next_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_image_opa(ui->Linking_Screen_next_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->Linking_Screen_next_btn, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->Linking_Screen_next_btn, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->Linking_Screen_next_btn, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_next_btn, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for Linking_Screen_next_btn, Part: LV_PART_MAIN, State: LV_IMAGEBUTTON_STATE_RELEASED.
    lv_obj_set_style_image_recolor_opa(ui->Linking_Screen_next_btn, 0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);
    lv_obj_set_style_image_opa(ui->Linking_Screen_next_btn, 255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

    //Write codes Linking_Screen_NFC_activation_status
    ui->Linking_Screen_NFC_activation_status = lv_label_create(ui->Linking_Screen);
    lv_obj_set_pos(ui->Linking_Screen_NFC_activation_status, 95, 12);
    lv_obj_set_size(ui->Linking_Screen_NFC_activation_status, 24, 14);
    lv_label_set_text(ui->Linking_Screen_NFC_activation_status, "" LV_SYMBOL_WIFI " ");
    lv_label_set_long_mode(ui->Linking_Screen_NFC_activation_status, LV_LABEL_LONG_WRAP);

    //Write style for Linking_Screen_NFC_activation_status, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->Linking_Screen_NFC_activation_status, lv_color_hex(0xc51f1f), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->Linking_Screen_NFC_activation_status, &lv_font_montserratMedium_13, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->Linking_Screen_NFC_activation_status, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->Linking_Screen_NFC_activation_status, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->Linking_Screen_NFC_activation_status, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of Linking_Screen.
    lv_obj_set_style_transform_angle(ui->Linking_Screen_NFC_activation_status, 900, 0);

    //Update current screen layout.
    lv_obj_update_layout(ui->Linking_Screen);

    //Init events for screen.
    events_init_Linking_Screen(ui);
}

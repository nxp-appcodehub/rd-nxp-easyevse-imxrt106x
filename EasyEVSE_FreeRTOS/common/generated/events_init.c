/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "events_init.h"
#include <stdio.h>
#include "lvgl.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "freemaster_client.h"
#endif

#include "custom.h"

static void Main_Screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        ui_animation(guider_ui.Main_Screen_label_welcome, 600, 1000, lv_obj_get_x(guider_ui.Main_Screen_label_welcome), 120, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.Main_Screen_label_welcome, 600, 1000, lv_obj_get_y(guider_ui.Main_Screen_label_welcome), 10, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
        ui_animation(guider_ui.Main_Screen_spangroup_A, 500, 1600, lv_obj_get_x(guider_ui.Main_Screen_spangroup_A), 306, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        lv_obj_set_y(guider_ui.Main_Screen_spangroup_A, 184);
        ui_animation(guider_ui.Main_Screen_cont_cover, 10, 2100, lv_obj_get_width(guider_ui.Main_Screen_cont_cover), 0, &lv_anim_path_linear, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_width, NULL, NULL, NULL);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_spangroup_A_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Linking_Screen, guider_ui.Linking_Screen_del, &guider_ui.Main_Screen_del, setup_scr_Linking_Screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 200, 200, true, true);
        ui_ChangeScreen(guider_ui.Linking_Screen);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_cont_1_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_animation(guider_ui.Main_Screen_cont_2, 300, 0, lv_obj_get_x(guider_ui.Main_Screen_cont_2), 45, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.Main_Screen_cont_2, 300, 0, lv_obj_get_y(guider_ui.Main_Screen_cont_2), 30, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_close_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_animation(guider_ui.Main_Screen_cont_2, 300, 0, lv_obj_get_x(guider_ui.Main_Screen_cont_2), -464, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.Main_Screen_cont_2, 300, 0, lv_obj_get_y(guider_ui.Main_Screen_cont_2), 108, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
        break;
    }
    default:
        break;
    }
}

void events_init_Main_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Main_Screen, Main_Screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_spangroup_A, Main_Screen_spangroup_A_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_cont_1, Main_Screen_cont_1_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_close_btn, Main_Screen_close_btn_event_handler, LV_EVENT_ALL, ui);
}

static void Linking_Screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        ui_animation(guider_ui.Linking_Screen_img_slide, 500, 400, lv_obj_get_x(guider_ui.Linking_Screen_img_slide), 20, &lv_anim_path_linear, -1, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_ChangeScreen(guider_ui.Linking_Screen);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_animation(guider_ui.Linking_Screen_cont_linked, 300, 0, lv_obj_get_x(guider_ui.Linking_Screen_cont_linked), 45, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
            ui_animation(guider_ui.Linking_Screen_cont_linked, 300, 0, lv_obj_get_y(guider_ui.Linking_Screen_cont_linked), 30, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void Linking_Screen_close_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_animation(guider_ui.Linking_Screen_cont_linked, 300, 0, lv_obj_get_x(guider_ui.Linking_Screen_cont_linked), -464, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.Linking_Screen_cont_linked, 300, 0, lv_obj_get_y(guider_ui.Linking_Screen_cont_linked), 18, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
        break;
    }
    default:
        break;
    }
}

static void Linking_Screen_closebtn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_animation(guider_ui.Linking_Screen_error_cont, 300, 0, lv_obj_get_x(guider_ui.Linking_Screen_error_cont), -121, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.Linking_Screen_error_cont, 300, 0, lv_obj_get_y(guider_ui.Linking_Screen_error_cont), -154, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
        break;
    }
    default:
        break;
    }
}

static void Linking_Screen_home_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.Linking_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 200, 200, true, true);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

static void Linking_Screen_next_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Charging_Screen, guider_ui.Charging_Screen_del, &guider_ui.Linking_Screen_del, setup_scr_Charging_Screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 200, 200, true, true);
        ui_ChangeScreen(guider_ui.Charging_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_Linking_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Linking_Screen, Linking_Screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Linking_Screen_close_btn, Linking_Screen_close_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Linking_Screen_closebtn, Linking_Screen_closebtn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Linking_Screen_home_btn, Linking_Screen_home_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Linking_Screen_next_btn, Linking_Screen_next_btn_event_handler, LV_EVENT_ALL, ui);
}

static void Charging_Screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        ui_animation(guider_ui.Charging_Screen_img_car, 600, 0, lv_obj_get_x(guider_ui.Charging_Screen_img_car), 52, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.Charging_Screen_img_car, 600, 0, lv_obj_get_y(guider_ui.Charging_Screen_img_car), 40, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
        ui_animation(guider_ui.Charging_Screen_img_slide, 1400, 1000, lv_obj_get_x(guider_ui.Charging_Screen_img_slide), 30, &lv_anim_path_linear, -1, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_ChangeScreen(guider_ui.Charging_Screen);
        break;
    }
    case LV_EVENT_GESTURE:
    {
        lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
        switch(dir) {
        case LV_DIR_RIGHT:
        {
            lv_indev_wait_release(lv_indev_active());
            ui_animation(guider_ui.Charging_Screen_cont_details, 300, 0, lv_obj_get_x(guider_ui.Charging_Screen_cont_details), 95, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
            ui_animation(guider_ui.Charging_Screen_cont_details, 300, 0, lv_obj_get_y(guider_ui.Charging_Screen_cont_details), 50, &lv_anim_path_ease_out, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
            break;
        }
        default:
            break;
        }
        break;
    }
    default:
        break;
    }
}

static void Charging_Screen_close_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_animation(guider_ui.Charging_Screen_cont_details, 300, 0, lv_obj_get_x(guider_ui.Charging_Screen_cont_details), -464, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_x, NULL, NULL, NULL);
        ui_animation(guider_ui.Charging_Screen_cont_details, 300, 0, lv_obj_get_y(guider_ui.Charging_Screen_cont_details), 17, &lv_anim_path_overshoot, 0, 0, 0, 0, (lv_anim_exec_xcb_t)lv_obj_set_y, NULL, NULL, NULL);
        break;
    }
    default:
        break;
    }
}

static void Charging_Screen_home_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.Charging_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 200, 200, true, true);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_Charging_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Charging_Screen, Charging_Screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Charging_Screen_close_btn, Charging_Screen_close_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Charging_Screen_home_btn, Charging_Screen_home_btn_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}

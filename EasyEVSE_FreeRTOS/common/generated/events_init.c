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
#include "custom.h"

static void Main_Screen_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_SCREEN_LOADED:
    {
        static uint8_t firstLoad = true;
        if(firstLoad)
        {
            ui_ChangeScreen(guider_ui.Main_Screen);
            firstLoad = false;
        }
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_ID_EV_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.EVSE_Screen, guider_ui.EVSE_Screen_del, &guider_ui.Main_Screen_del, setup_scr_EVSE_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.EVSE_Screen);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_ID_debug_conn_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Debug_Conn_Screen, guider_ui.Debug_Conn_Screen_del, &guider_ui.Main_Screen_del, setup_scr_Debug_Conn_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Debug_Conn_Screen);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_ID_car_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Car_Screen, guider_ui.Car_Screen_del, &guider_ui.Main_Screen_del, setup_scr_Car_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Car_Screen);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_ID_meter_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Meter_Screen, guider_ui.Meter_Screen_del, &guider_ui.Main_Screen_del, setup_scr_Meter_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Meter_Screen);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_ID_NFC_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.NFC_Screen, guider_ui.NFC_Screen_del, &guider_ui.Main_Screen_del, setup_scr_NFC_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.NFC_Screen);
        break;
    }
    default:
        break;
    }
}

static void Main_Screen_ID_debug_ISO_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Debug_ISO_Screen, guider_ui.Debug_ISO_Screen_del, &guider_ui.Main_Screen_del, setup_scr_Debug_ISO_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Debug_ISO_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_Main_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Main_Screen, Main_Screen_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_ID_EV_btn, Main_Screen_ID_EV_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_ID_debug_conn_btn, Main_Screen_ID_debug_conn_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_ID_car_btn, Main_Screen_ID_car_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_ID_meter_btn, Main_Screen_ID_meter_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_ID_NFC_btn, Main_Screen_ID_NFC_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Main_Screen_ID_debug_ISO_btn, Main_Screen_ID_debug_ISO_btn_event_handler, LV_EVENT_ALL, ui);
}

static void Debug_Conn_Screen_ID_back_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.Debug_Conn_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_Debug_Conn_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Debug_Conn_Screen_ID_back_btn, Debug_Conn_Screen_ID_back_btn_event_handler, LV_EVENT_ALL, ui);
}

static void Car_Screen_ID_back_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.Car_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

static void Car_Screen_stop_resume_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        EVSE_Pause_Resume();
        break;
    }
    default:
        break;
    }
}

void events_init_Car_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Car_Screen_ID_back_btn, Car_Screen_ID_back_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Car_Screen_stop_resume_btn, Car_Screen_stop_resume_btn_event_handler, LV_EVENT_ALL, ui);
}

static void EVSE_Screen_ID_back_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.EVSE_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_EVSE_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->EVSE_Screen_ID_back_btn, EVSE_Screen_ID_back_btn_event_handler, LV_EVENT_ALL, ui);
}

static void Meter_Screen_ID_meter_data_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        update_meter_data();
        break;
    }
    default:
        break;
    }
}

static void Meter_Screen_ID_back_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.Meter_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_Meter_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Meter_Screen_ID_meter_data_btn, Meter_Screen_ID_meter_data_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->Meter_Screen_ID_back_btn, Meter_Screen_ID_back_btn_event_handler, LV_EVENT_ALL, ui);
}

static void NFC_Screen_read_UID_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        read_NFC_UID();
        break;
    }
    default:
        break;
    }
}

static void NFC_Screen_ID_back_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.NFC_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_NFC_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->NFC_Screen_read_UID_btn, NFC_Screen_read_UID_btn_event_handler, LV_EVENT_ALL, ui);
    lv_obj_add_event_cb(ui->NFC_Screen_ID_back_btn, NFC_Screen_ID_back_btn_event_handler, LV_EVENT_ALL, ui);
}

static void Debug_ISO_Screen_ID_back_btn_event_handler (lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    switch (code) {
    case LV_EVENT_CLICKED:
    {
        ui_load_scr_animation(&guider_ui, &guider_ui.Main_Screen, guider_ui.Main_Screen_del, &guider_ui.Debug_ISO_Screen_del, setup_scr_Main_Screen, LV_SCR_LOAD_ANIM_NONE, 10, 10, false, false);
        ui_ChangeScreen(guider_ui.Main_Screen);
        break;
    }
    default:
        break;
    }
}

void events_init_Debug_ISO_Screen (lv_ui *ui)
{
    lv_obj_add_event_cb(ui->Debug_ISO_Screen_ID_back_btn, Debug_ISO_Screen_ID_back_btn_event_handler, LV_EVENT_ALL, ui);
}


void events_init(lv_ui *ui)
{

}

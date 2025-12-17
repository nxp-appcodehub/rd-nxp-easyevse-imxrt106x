/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"


typedef struct
{
  
	lv_obj_t *Main_Screen;
	bool Main_Screen_del;
	lv_obj_t *Main_Screen_cont_cover;
	lv_obj_t *Main_Screen_label_welcome;
	lv_obj_t *Main_Screen_spangroup_A;
	lv_span_t *Main_Screen_spangroup_A_span;
	lv_obj_t *Main_Screen_clock;
	lv_obj_t *Main_Screen_EVSE_software_version;
	lv_obj_t *Main_Screen_cont_1;
	lv_obj_t *Main_Screen_wifi_symbol;
	lv_obj_t *Main_Screen_SSID_prompt;
	lv_obj_t *Main_Screen_line_1;
	lv_obj_t *Main_Screen_type_of_connection_label;
	lv_obj_t *Main_Screen_cont_2;
	lv_obj_t *Main_Screen_OCPP_status_prompt;
	lv_obj_t *Main_Screen_OCPP_status_value;
	lv_obj_t *Main_Screen_telemetry_value;
	lv_obj_t *Main_Screen_telemetry_prompt;
	lv_obj_t *Main_Screen_close_btn;
	lv_obj_t *Main_Screen_close_btn_label;
	lv_obj_t *Main_Screen_NFC_activation_status;
	lv_obj_t *Linking_Screen;
	bool Linking_Screen_del;
	lv_obj_t *Linking_Screen_cont_linked;
	lv_obj_t *Linking_Screen_SIGBOARD_connection_prompt;
	lv_obj_t *Linking_Screen_SIGBOARD_connection_value;
	lv_obj_t *Linking_Screen_card_UID_prompt;
	lv_obj_t *Linking_Screen_card_UID_value;
	lv_obj_t *Linking_Screen_card_type_value;
	lv_obj_t *Linking_Screen_card_type_prompt;
	lv_obj_t *Linking_Screen_card_security_value;
	lv_obj_t *Linking_Screen_card_security_prompt;
	lv_obj_t *Linking_Screen_close_btn;
	lv_obj_t *Linking_Screen_close_btn_label;
	lv_obj_t *Linking_Screen_mode_value;
	lv_obj_t *Linking_Screen_mode_prompt;
	lv_obj_t *Linking_Screen_clock;
	lv_obj_t *Linking_Screen_img_slide;
	lv_obj_t *Linking_Screen_cont_1;
	lv_obj_t *Linking_Screen_ISO15118_status_prompt;
	lv_obj_t *Linking_Screen_ISO15118_status_value;
	lv_obj_t *Linking_Screen_cont_2;
	lv_obj_t *Linking_Screen_register_card_btn;
	lv_obj_t *Linking_Screen_register_card_btn_label;
	lv_obj_t *Linking_Screen_deregister_card_btn;
	lv_obj_t *Linking_Screen_deregister_card_btn_label;
	lv_obj_t *Linking_Screen_label_1;
	lv_obj_t *Linking_Screen_label_2;
	lv_obj_t *Linking_Screen_error_cont;
	lv_obj_t *Linking_Screen_error_status;
	lv_obj_t *Linking_Screen_closebtn;
	lv_obj_t *Linking_Screen_closebtn_label;
	lv_obj_t *Linking_Screen_home_btn;
	lv_obj_t *Linking_Screen_home_btn_label;
	lv_obj_t *Linking_Screen_next_btn;
	lv_obj_t *Linking_Screen_next_btn_label;
	lv_obj_t *Linking_Screen_NFC_activation_status;
	lv_obj_t *Charging_Screen;
	bool Charging_Screen_del;
	lv_obj_t *Charging_Screen_img_car;
	lv_obj_t *Charging_Screen_bar_charging;
	lv_obj_t *Charging_Screen_battery_level;
	lv_obj_t *Charging_Screen_label_percent;
	lv_obj_t *Charging_Screen_img_slide;
	lv_obj_t *Charging_Screen_cont_details;
	lv_obj_t *Charging_Screen_charge_current_prompt;
	lv_obj_t *Charging_Screen_current_value;
	lv_obj_t *Charging_Screen_voltage_value;
	lv_obj_t *Charging_Screen_label_46;
	lv_obj_t *Charging_Screen_protocol_value;
	lv_obj_t *Charging_Screen_label_42;
	lv_obj_t *Charging_Screen_charging_direction_value;
	lv_obj_t *Charging_Screen_label_40;
	lv_obj_t *Charging_Screen_power_request_cntr_value;
	lv_obj_t *Charging_Screen_power_request_cntr_prompt;
	lv_obj_t *Charging_Screen_close_btn;
	lv_obj_t *Charging_Screen_close_btn_label;
	lv_obj_t *Charging_Screen_mode_value;
	lv_obj_t *Charging_Screen_label_53;
	lv_obj_t *Charging_Screen_time_to_charge_value;
	lv_obj_t *Charging_Screen_label_remain;
	lv_obj_t *Charging_Screen_label_45;
	lv_obj_t *Charging_Screen_elapsed_time_value;
	lv_obj_t *Charging_Screen_label_charged1;
	lv_obj_t *Charging_Screen_clock;
	lv_obj_t *Charging_Screen_EVDE_ID_prompt;
	lv_obj_t *Charging_Screen_EVSE_ID_value;
	lv_obj_t *Charging_Screen_vehicle_id_value;
	lv_obj_t *Charging_Screen_label_51;
	lv_obj_t *Charging_Screen_home_btn;
	lv_obj_t *Charging_Screen_home_btn_label;
	lv_obj_t *Charging_Screen_kWh_value;
	lv_obj_t *Charging_Screen_NFC_activation_status;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_screen_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, uint32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint32_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_completed_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_bottom_layer(void);

void setup_ui(lv_ui *ui);

void video_play(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_Main_Screen(lv_ui *ui);
void setup_scr_Linking_Screen(lv_ui *ui);
void setup_scr_Charging_Screen(lv_ui *ui);

LV_IMAGE_DECLARE(_welcome_bak_RGB565_480x272);

LV_IMAGE_DECLARE(_linking_bak_RGB565_480x272);
LV_IMAGE_DECLARE(_slide_RGB565A8_22x13);

LV_IMAGE_DECLARE(_start_btn_RGB565A8_29x27);
LV_IMAGE_DECLARE(_charging_bak_RGB565A8_395x216);

LV_IMAGE_DECLARE(_charing_RGB565A8_200x139);

LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_montserratMedium_10)
LV_FONT_DECLARE(lv_font_montserratMedium_50)
LV_FONT_DECLARE(lv_font_montserratMedium_20)
LV_FONT_DECLARE(lv_font_montserratMedium_13)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_8)
LV_FONT_DECLARE(lv_font_montserratMedium_11)
LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_15)
LV_FONT_DECLARE(lv_font_montserratMedium_40)
LV_FONT_DECLARE(lv_font_montserratMedium_17)


#ifdef __cplusplus
}
#endif
#endif

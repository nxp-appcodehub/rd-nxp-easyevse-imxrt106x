/*
* Copyright 2024 NXP
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
	lv_obj_t *Main_Screen_ID_EV_btn;
	lv_obj_t *Main_Screen_ID_EV_btn_label;
	lv_obj_t *Main_Screen_ID_debug_conn_btn;
	lv_obj_t *Main_Screen_ID_debug_conn_btn_label;
	lv_obj_t *Main_Screen_ID_car_btn;
	lv_obj_t *Main_Screen_ID_car_btn_label;
	lv_obj_t *Main_Screen_ID_meter_btn;
	lv_obj_t *Main_Screen_ID_meter_btn_label;
	lv_obj_t *Main_Screen_ID_NFC_btn;
	lv_obj_t *Main_Screen_ID_NFC_btn_label;
	lv_obj_t *Main_Screen_type_of_connection_label;
	lv_obj_t *Main_Screen_SSID_prompt;
	lv_obj_t *Main_Screen_blue_line_top;
	lv_obj_t *Main_Screen_blue_line_bottom;
	lv_obj_t *Main_Screen_blue_line_left;
	lv_obj_t *Main_Screen_blue_line_right;
	lv_obj_t *Main_Screen_orange_line_left;
	lv_obj_t *Main_Screen_green_line_top;
	lv_obj_t *Main_Screen_green_line_bottom;
	lv_obj_t *Main_Screen_green_line_left;
	lv_obj_t *Main_Screen_green_line_right;
	lv_obj_t *Main_Screen_orange_line_top;
	lv_obj_t *Main_Screen_orange_line_bottom;
	lv_obj_t *Main_Screen_orange_line_right;
	lv_obj_t *Main_Screen_EVSE_software_version;
	lv_obj_t *Main_Screen_ID_debug_ISO_btn;
	lv_obj_t *Main_Screen_ID_debug_ISO_btn_label;
	lv_obj_t *Main_Screen_clock;
	lv_obj_t *Debug_Conn_Screen;
	bool Debug_Conn_Screen_del;
	lv_obj_t *Debug_Conn_Screen_DebugConn_Heading;
	lv_obj_t *Debug_Conn_Screen_network_status_prompt;
	lv_obj_t *Debug_Conn_Screen_cloud_status_prompt;
	lv_obj_t *Debug_Conn_Screen_telemetry_prompt;
	lv_obj_t *Debug_Conn_Screen_ID_back_btn;
	lv_obj_t *Debug_Conn_Screen_ID_back_btn_label;
	lv_obj_t *Debug_Conn_Screen_network_status_value;
	lv_obj_t *Debug_Conn_Screen_cloud_status_value;
	lv_obj_t *Debug_Conn_Screen_telemetry_value;
	lv_obj_t *Debug_Conn_Screen_blue_line_top;
	lv_obj_t *Debug_Conn_Screen_blue_line_bottom;
	lv_obj_t *Debug_Conn_Screen_blue_line_left;
	lv_obj_t *Debug_Conn_Screen_blue_line_right;
	lv_obj_t *Debug_Conn_Screen_green_line_top;
	lv_obj_t *Debug_Conn_Screen_green_line_bottom;
	lv_obj_t *Debug_Conn_Screen_green_line_left;
	lv_obj_t *Debug_Conn_Screen_green_line_right;
	lv_obj_t *Debug_Conn_Screen_orange_line_top;
	lv_obj_t *Debug_Conn_Screen_orange_line_bottom;
	lv_obj_t *Debug_Conn_Screen_orange_line_left;
	lv_obj_t *Debug_Conn_Screen_orange_line_right;
	lv_obj_t *Debug_Conn_Screen_clock;
	lv_obj_t *Car_Screen;
	bool Car_Screen_del;
	lv_obj_t *Car_Screen_Vehicle_settings_heading;
	lv_obj_t *Car_Screen_ID_back_btn;
	lv_obj_t *Car_Screen_ID_back_btn_label;
	lv_obj_t *Car_Screen_vehicle_id_value;
	lv_obj_t *Car_Screen_blue_line_top;
	lv_obj_t *Car_Screen_blue_line_bottom;
	lv_obj_t *Car_Screen_blue_line_left;
	lv_obj_t *Car_Screen_blue_line_right;
	lv_obj_t *Car_Screen_green_line_top;
	lv_obj_t *Car_Screen_green_line_bottom;
	lv_obj_t *Car_Screen_green_line_left;
	lv_obj_t *Car_Screen_green_line_right;
	lv_obj_t *Car_Screen_orange_line_top;
	lv_obj_t *Car_Screen_orange_line_bottom;
	lv_obj_t *Car_Screen_orange_line_left;
	lv_obj_t *Car_Screen_orange_line_right;
	lv_obj_t *Car_Screen_requested_energy_value;
	lv_obj_t *Car_Screen_delivered_energy_value;
	lv_obj_t *Car_Screen_delivered_energy_prompt;
	lv_obj_t *Car_Screen_protocol_value;
	lv_obj_t *Car_Screen_requested_energy_prompt;
	lv_obj_t *Car_Screen_vehicle_id_prompt;
	lv_obj_t *Car_Screen_protocol_prompt;
	lv_obj_t *Car_Screen_charge_current_value;
	lv_obj_t *Car_Screen_charge_current_prompt;
	lv_obj_t *Car_Screen_elapsed_time_value;
	lv_obj_t *Car_Screen_elapsed_time_prompt;
	lv_obj_t *Car_Screen_time_to_charge_prompt;
	lv_obj_t *Car_Screen_time_to_charge_value;
	lv_obj_t *Car_Screen_energy_progress_bar;
	lv_obj_t *Car_Screen_energy_progress_prompt;
	lv_obj_t *Car_Screen_clock;
	lv_obj_t *EVSE_Screen;
	bool EVSE_Screen_del;
	lv_obj_t *EVSE_Screen_EVSE_Menu_Heading;
	lv_obj_t *EVSE_Screen_ID_back_btn;
	lv_obj_t *EVSE_Screen_ID_back_btn_label;
	lv_obj_t *EVSE_Screen_power_rate_prompt;
	lv_obj_t *EVSE_Screen_sigboard_versions_prompt;
	lv_obj_t *EVSE_Screen_charge_cost_prompt;
	lv_obj_t *EVSE_Screen_EVDE_ID_prompt;
	lv_obj_t *EVSE_Screen_grid_limit_prompt;
	lv_obj_t *EVSE_Screen_temperature_prompt;
	lv_obj_t *EVSE_Screen_power_rate_value;
	lv_obj_t *EVSE_Screen_sigboard_versions_value;
	lv_obj_t *EVSE_Screen_charge_cost_value;
	lv_obj_t *EVSE_Screen_EVSE_ID_value;
	lv_obj_t *EVSE_Screen_grid_limit_value;
	lv_obj_t *EVSE_Screen_temperature_value;
	lv_obj_t *EVSE_Screen_blue_line_top;
	lv_obj_t *EVSE_Screen_blue_line_bottom;
	lv_obj_t *EVSE_Screen_blue_line_left;
	lv_obj_t *EVSE_Screen_blue_line_right;
	lv_obj_t *EVSE_Screen_green_line_top;
	lv_obj_t *EVSE_Screen_green_line_bottom;
	lv_obj_t *EVSE_Screen_green_line_left;
	lv_obj_t *EVSE_Screen_green_line_right;
	lv_obj_t *EVSE_Screen_orange_line_top;
	lv_obj_t *EVSE_Screen_charge_state_prompt;
	lv_obj_t *EVSE_Screen_orange_line_bottom;
	lv_obj_t *EVSE_Screen_orange_line_left;
	lv_obj_t *EVSE_Screen_orange_line_right;
	lv_obj_t *EVSE_Screen_charge_state_led;
	lv_obj_t *EVSE_Screen_up_time_value;
	lv_obj_t *EVSE_Screen_up_time_prompt;
	lv_obj_t *EVSE_Screen_clock;
	lv_obj_t *Meter_Screen;
	bool Meter_Screen_del;
	lv_obj_t *Meter_Screen_Meter_Menu_Heading;
	lv_obj_t *Meter_Screen_V_RMS_prompt;
	lv_obj_t *Meter_Screen_V_RMS_value;
	lv_obj_t *Meter_Screen_aparent_prompt;
	lv_obj_t *Meter_Screen_aparent_value;
	lv_obj_t *Meter_Screen_active_value;
	lv_obj_t *Meter_Screen_active_prompt;
	lv_obj_t *Meter_Screen_I_RMS_value;
	lv_obj_t *Meter_Screen_I_RMS_prompt;
	lv_obj_t *Meter_Screen_VARh_value;
	lv_obj_t *Meter_Screen_VARh_prompt;
	lv_obj_t *Meter_Screen_kWh_prompt;
	lv_obj_t *Meter_Screen_reactive_prompt;
	lv_obj_t *Meter_Screen_reactive_value;
	lv_obj_t *Meter_Screen_kWh_value;
	lv_obj_t *Meter_Screen_mode_value;
	lv_obj_t *Meter_Screen_mode_prompt;
	lv_obj_t *Meter_Screen_ID_meter_data_btn;
	lv_obj_t *Meter_Screen_ID_meter_data_btn_label;
	lv_obj_t *Meter_Screen_refresh_data_prompt;
	lv_obj_t *Meter_Screen_blue_line_top;
	lv_obj_t *Meter_Screen_blue_line_bottom;
	lv_obj_t *Meter_Screen_blue_line_left;
	lv_obj_t *Meter_Screen_blue_line_right;
	lv_obj_t *Meter_Screen_green_line_top;
	lv_obj_t *Meter_Screen_green_line_bottom;
	lv_obj_t *Meter_Screen_green_line_left;
	lv_obj_t *Meter_Screen_green_line_right;
	lv_obj_t *Meter_Screen_orange_line_top;
	lv_obj_t *Meter_Screen_orange_line_bottom;
	lv_obj_t *Meter_Screen_orange_line_left;
	lv_obj_t *Meter_Screen_orange_line_right;
	lv_obj_t *Meter_Screen_ID_back_btn;
	lv_obj_t *Meter_Screen_ID_back_btn_label;
	lv_obj_t *Meter_Screen_clock;
	lv_obj_t *NFC_Screen;
	bool NFC_Screen_del;
	lv_obj_t *NFC_Screen_NFC_Menu_Heading;
	lv_obj_t *NFC_Screen_Read_UID_prompt;
	lv_obj_t *NFC_Screen_card_UID_prompt;
	lv_obj_t *NFC_Screen_card_UID_value;
	lv_obj_t *NFC_Screen_read_UID_btn;
	lv_obj_t *NFC_Screen_read_UID_btn_label;
	lv_obj_t *NFC_Screen_blue_line_top;
	lv_obj_t *NFC_Screen_blue_line_bottom;
	lv_obj_t *NFC_Screen_blue_line_left;
	lv_obj_t *NFC_Screen_blue_line_right;
	lv_obj_t *NFC_Screen_green_line_top;
	lv_obj_t *NFC_Screen_green_line_bottom;
	lv_obj_t *NFC_Screen_green_line_left;
	lv_obj_t *NFC_Screen_green_line_right;
	lv_obj_t *NFC_Screen_orange_line_top;
	lv_obj_t *NFC_Screen_orange_line_bottom;
	lv_obj_t *NFC_Screen_orange_line_left;
	lv_obj_t *NFC_Screen_orange_line_right;
	lv_obj_t *NFC_Screen_ID_back_btn;
	lv_obj_t *NFC_Screen_ID_back_btn_label;
	lv_obj_t *NFC_Screen_clock;
	lv_obj_t *Debug_ISO_Screen;
	bool Debug_ISO_Screen_del;
	lv_obj_t *Debug_ISO_Screen_orange_line_right;
	lv_obj_t *Debug_ISO_Screen_orange_line_left;
	lv_obj_t *Debug_ISO_Screen_orange_line_bottom;
	lv_obj_t *Debug_ISO_Screen_orange_line_top;
	lv_obj_t *Debug_ISO_Screen_green_line_right;
	lv_obj_t *Debug_ISO_Screen_green_line_left;
	lv_obj_t *Debug_ISO_Screen_green_line_bottom;
	lv_obj_t *Debug_ISO_Screen_green_line_top;
	lv_obj_t *Debug_ISO_Screen_blue_line_right;
	lv_obj_t *Debug_ISO_Screen_blue_line_left;
	lv_obj_t *Debug_ISO_Screen_blue_line_bottom;
	lv_obj_t *Debug_ISO_Screen_ISO15118_status_value;
	lv_obj_t *Debug_ISO_Screen_blue_line_top;
	lv_obj_t *Debug_ISO_Screen_ID_back_btn;
	lv_obj_t *Debug_ISO_Screen_ID_back_btn_label;
	lv_obj_t *Debug_ISO_Screen_DebugISO_Heading;
	lv_obj_t *Debug_ISO_Screen_power_request_cntr_value;
	lv_obj_t *Debug_ISO_Screen_power_request_cntr_prompt;
	lv_obj_t *Debug_ISO_Screen_V2G_demo_value;
	lv_obj_t *Debug_ISO_Screen_V2G_demo_prompt;
	lv_obj_t *Debug_ISO_Screen_ISO15118_status_prompt;
	lv_obj_t *Debug_ISO_Screen_clock;
	lv_obj_t *Debug_ISO_Screen_SIGBOARD_connection_prompt;
	lv_obj_t *Debug_ISO_Screen_SIGBOARD_connection_value;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_move_animation(void * var, int32_t duration, int32_t delay, int32_t x_end, int32_t y_end, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_scale_animation(void * var, int32_t duration, int32_t delay, int32_t width, int32_t height, lv_anim_path_cb_t path_cb,
                        uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                        lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_zoom_animation(void * var, int32_t duration, int32_t delay, int32_t zoom, lv_anim_path_cb_t path_cb,
                           uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                           lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void ui_img_rotate_animation(void * var, int32_t duration, int32_t delay, lv_coord_t x, lv_coord_t y, int32_t rotate,
                   lv_anim_path_cb_t path_cb, uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time,
                   uint32_t playback_delay, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);

void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);


extern lv_ui guider_ui;


void setup_scr_Main_Screen(lv_ui *ui);
void setup_scr_Debug_Conn_Screen(lv_ui *ui);
void setup_scr_Car_Screen(lv_ui *ui);
void setup_scr_EVSE_Screen(lv_ui *ui);
void setup_scr_Meter_Screen(lv_ui *ui);
void setup_scr_NFC_Screen(lv_ui *ui);
void setup_scr_Debug_ISO_Screen(lv_ui *ui);

LV_IMG_DECLARE(_mainpng_480x272);
LV_IMG_DECLARE(_evse_alpha_60x61);
LV_IMG_DECLARE(_evse_alpha_60x61);
LV_IMG_DECLARE(_evse_alpha_60x61);
LV_IMG_DECLARE(_evse_alpha_60x61);
LV_IMG_DECLARE(_debug_alpha_62x62);
LV_IMG_DECLARE(_debug_alpha_62x62);
LV_IMG_DECLARE(_debug_alpha_62x62);
LV_IMG_DECLARE(_debug_alpha_62x62);
LV_IMG_DECLARE(_carpng_alpha_60x61);
LV_IMG_DECLARE(_carpng_alpha_60x61);
LV_IMG_DECLARE(_carpng_alpha_60x61);
LV_IMG_DECLARE(_carpng_alpha_60x61);
LV_IMG_DECLARE(_meterguage_alpha_60x61);
LV_IMG_DECLARE(_meterguage_alpha_60x61);
LV_IMG_DECLARE(_meterguage_alpha_60x61);
LV_IMG_DECLARE(_meterguage_alpha_60x61);
LV_IMG_DECLARE(_nfc_alpha_60x61);
LV_IMG_DECLARE(_nfc_alpha_60x61);
LV_IMG_DECLARE(_nfc_alpha_60x61);
LV_IMG_DECLARE(_nfc_alpha_60x61);
LV_IMG_DECLARE(_debug_ISO15118_alpha_62x62);
LV_IMG_DECLARE(_debug_ISO15118_alpha_62x62);
LV_IMG_DECLARE(_debug_ISO15118_alpha_62x62);
LV_IMG_DECLARE(_debug_ISO15118_alpha_62x62);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_readdata_alpha_35x37);
LV_IMG_DECLARE(_readdata_alpha_35x37);
LV_IMG_DECLARE(_readdata_alpha_35x37);
LV_IMG_DECLARE(_readdata_alpha_35x37);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_readnfc_alpha_91x63);
LV_IMG_DECLARE(_readnfc_alpha_91x63);
LV_IMG_DECLARE(_readnfc_alpha_91x63);
LV_IMG_DECLARE(_readnfc_alpha_91x63);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);
LV_IMG_DECLARE(_returnpng_alpha_32x28);

LV_FONT_DECLARE(lv_font_montserratMedium_12)
LV_FONT_DECLARE(lv_font_montserratMedium_18)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_montserratMedium_13)
LV_FONT_DECLARE(lv_font_montserratMedium_8)
LV_FONT_DECLARE(lv_font_montserratMedium_10)


#ifdef __cplusplus
}
#endif
#endif

#include "ui.h"
#include <Arduino.h>
#include "components/helper.h"
#include <stdint.h>
#include <stdio.h>

extern uint8_t searchWifiDevices();

static lv_obj_t * list_bluetooth_devices = NULL;
static lv_obj_t * list_wifi_devices = NULL;

static void button_search_bluetooth_devices_cb(lv_event_t * event){
    lv_event_code_t event_code = lv_event_get_code(event);
    lv_list_add_text(list_bluetooth_devices,"Hallo");


    if(event_code == LV_EVENT_CLICKED){
        lv_obj_t * btn;
        btn = lv_list_add_button(list_bluetooth_devices,LV_SYMBOL_BLUETOOTH,"Device");
    }

};
static void button_search_wifi_devices_cb(lv_event_t * event){
    lv_event_code_t event_code = lv_event_get_code(event);

    if(event_code == LV_EVENT_CLICKED){
        uint8_t devices = searchWifiDevices();
        lv_obj_t *btn;
        char buf[20];
        sprintf(buf,"%d", devices);
        lv_list_add_button(list_wifi_devices, LV_SYMBOL_WIFI, buf);

    }
    

    
};


void ui_init(void){


    // lv_obj_t *screen = lv_obj_create(lv_scr_act());
    // lv_obj_set_size(screen, 800,480);

    lv_obj_t *tabView =  lv_tabview_create(lv_screen_active());

    lv_obj_t *tab_Weather = lv_tabview_add_tab(tabView, "Weather");

    lv_obj_t *tab_Bluetooth = lv_tabview_add_tab(tabView, "Bluetooth");
    lv_obj_set_flex_flow(tab_Bluetooth, LV_FLEX_FLOW_COLUMN);

    list_bluetooth_devices = lv_list_create(tab_Bluetooth);
    lv_obj_set_width(list_bluetooth_devices,500);
    lv_list_add_text(list_bluetooth_devices,"Found devices: ");

    lv_obj_t * button_search_bluetooth_devices = createButton(tab_Bluetooth,"Refresh");
    lv_obj_add_event_cb(button_search_bluetooth_devices,button_search_bluetooth_devices_cb, LV_EVENT_CLICKED, NULL);
    
    // lv_obj_align_to(button_search_bluetooth_devices, list_bluetooth_devices, LV_ALIGN_BOTTOM_RIGHT,0,0);


    lv_obj_t *tab_WiFi = lv_tabview_add_tab(tabView, "WiFi");
    lv_obj_set_flex_flow(tab_WiFi, LV_FLEX_FLOW_ROW);

    list_wifi_devices = lv_list_create(tab_WiFi);
    lv_obj_set_width(list_wifi_devices,500);
    lv_list_add_text(list_wifi_devices,"Found devices: ");
    lv_obj_t * button_search_wifi_devices = createButton(tab_WiFi,"Refresh");
    lv_obj_add_event_cb(button_search_wifi_devices,button_search_wifi_devices_cb, LV_EVENT_CLICKED, NULL);


    lv_obj_t *tab_DeviceInfo = lv_tabview_add_tab(tabView, "Device");
    lv_obj_set_flex_flow(tab_DeviceInfo, LV_FLEX_FLOW_COLUMN);


    lv_obj_t * label_brightness = lv_label_create(tab_DeviceInfo);
    lv_label_set_text_fmt(label_brightness,"%s\n", "Set Display brightness..");

    lv_obj_t * slider_brightness = lv_slider_create(tab_DeviceInfo);
    lv_slider_set_range(slider_brightness,20,100);
    lv_slider_set_value(slider_brightness,100,LV_ANIM_ON);
    lv_obj_set_style_anim_duration(slider_brightness, 2000,0);

    lv_obj_t * label_slider_brightness = lv_label_create(slider_brightness);
    lv_label_set_text(label_slider_brightness,"0");
    lv_obj_align_to(label_slider_brightness, slider_brightness, LV_ALIGN_BOTTOM_MID,0,0);

    
    
    


    // lv_obj_t * button = createButton(screen,"Hallo");
}
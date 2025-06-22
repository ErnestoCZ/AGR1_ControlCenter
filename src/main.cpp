#include "arduino_secrets.h"
#include <Arduino.h>
#include <Arduino_GigaDisplayTouch.h>
#include <Arduino_H7_Video.h>
#include <GigaDisplayRGB.h>
#include <mbed.h>
#include <Thread.h>
#include <Mutex.h>
#include "custom_typedefs.h"
#include <string.h>
#include "ui.h"
#include "wifi_helper.h"

constexpr uint8_t LED_INTENSITY = 10;
static char ssid[] = SECRET_SSID;
static char pass[] = SECRET_PW;

//states
connection_status_t connection_status;

// put function declarations here:
Arduino_H7_Video Display(800, 480);
Arduino_GigaDisplayTouch touchDetector;
GigaDisplayRGB rgb_led;

//OS relevant structures
rtos::Thread status_thread, gui_thread;
rtos::Mutex mutex;

WiFiClient client;

void gui_thread_task(void){

    while(true){
      lv_timer_handler();
    }
}

void status_thread_handler(void){
  if(Serial) Serial.println("Status Thread started!");
  while(true){
    //refresh connectivity status

    mutex.lock();
    connection_status.wifi_status = WiFi.status();
    mutex.unlock();

    //refresh led indicator
    if(connection_status.wifi_status == WL_CONNECTED){
      rgb_led.on(0,LED_INTENSITY,0);
    }else{
      rgb_led.on(LED_INTENSITY,0,0);
      connectToWifiNetwork(SECRET_SSID,SECRET_PW);
    }
    rtos::ThisThread::sleep_for(2000);
  }
}


void setup() {

  delay(1000);
  rgb_led.begin();
  status_thread.start(status_thread_handler);

  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with the WiFi module failed");
    while (true);
  }
  WiFi.setHostname("AGR1_ControlCenter");

  if (!Display.begin()) {
    Serial.println("Display initialized");
  }
  if (touchDetector.begin()) {
    Serial.println("TouchDetector initialized");
  }
  

  ui_init();
  gui_thread.start(gui_thread_task);

}

void loop() {
  // put your main code here, to run repeatedly:
}


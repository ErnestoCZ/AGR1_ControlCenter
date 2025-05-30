#include "arduino_secrets.h"
#include <Arduino.h>
#include <Arduino_GigaDisplayTouch.h>
#include <Arduino_H7_Video.h>
#include <GigaDisplayRGB.h>
#include <WiFi.h>
#include <mbed.h>
#include <Thread.h>
#include <Mutex.h>
#include <EventQueue.h>
#include "ui.h"
#include "custom_typedefs.h"
#include <string.h>
#define LED_INTENSITY 30

//states
connection_status_t connection_status;


// put function declarations here:
Arduino_H7_Video Display(800, 480);
Arduino_GigaDisplayTouch touchDetector;
GigaDisplayRGB rgb_led;

//OS relevant structures
events::EventQueue myQueue(32*EVENTS_EVENT_SIZE);
rtos::Thread status_thread;
rtos::Thread ui_thread;
rtos::Thread queue_thread;
rtos::Mutex mutex;


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
    }
    rtos::ThisThread::sleep_for(2000);
  }
}
void ui_thread_handler(void){
  while(true){
    lv_timer_handler();
  }
}

void connectToPreferredWifi(const char* ssid, const char* pass);

static char ssid[] = SECRET_SSID;
static char pass[] = SECRET_PW;

void setup() {

  delay(1000);
  rgb_led.begin();
  status_thread.start(status_thread_handler);
  queue_thread.start(callback(&myQueue, &events::EventQueue::dispatch_forever));

  Serial.begin(9600);

  while (!Serial) {
  };

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with the WiFi module failed");
    while (true)
      ;
  }
  WiFi.setHostname("AGR1_ControlCenter");

  // myQueue.event(connectToPreferredWifi,ssid,pass);
  connectToPreferredWifi(ssid,pass);

  if (!Display.begin()) {
    Serial.println("Display initialized");
  }
  if (touchDetector.begin()) {
    Serial.println("TouchDetector initialized");
  }

  ui_init();
  ui_thread.start(ui_thread_handler);

}

void loop() {
  // put your main code here, to run repeatedly:
}

void connectToPreferredWifi(const char* ssid, const char* pass) {
  uint8_t numberFoundNetworks = WiFi.scanNetworks();

  if (0 < numberFoundNetworks) {
    Serial.print("Found Networks: ");
    Serial.println(numberFoundNetworks);
    for (uint8_t currentNetwork = 0; currentNetwork < numberFoundNetworks;
         currentNetwork++) {
      if ((strlen(WiFi.SSID(currentNetwork)) == strlen(ssid)) &&
          (strcmp(WiFi.SSID(currentNetwork), ssid) == 0)) {
        int connectionAttempts = 1;
        while (WiFi.status() != WL_CONNECTED) {
          WiFi.begin(WiFi.SSID(currentNetwork), pass);
          if (connectionAttempts == 10)
            return;
          Serial.print("Trying to connect to preferred network...");
          Serial.println(connectionAttempts);
          connectionAttempts++;
          delay(1000);
        };
        if (WiFi.status() == WL_CONNECTED) {
          Serial.print("Connected to Network : ");
          Serial.println(WiFi.SSID(currentNetwork));
        }
      }
    }
  }
};
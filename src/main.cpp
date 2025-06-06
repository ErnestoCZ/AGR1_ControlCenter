#include "arduino_secrets.h"
#include <Arduino.h>
#include <Arduino_GigaDisplayTouch.h>
#include <Arduino_H7_Video.h>
#include <GigaDisplayRGB.h>
#include <WiFi.h>
#include <mbed.h>
#include <Thread.h>
#include <Mutex.h>
#include "custom_typedefs.h"
#include <string.h>
#include "ui.h"
  
constexpr uint8_t LED_INTENSITY = 15;

//states
connection_status_t connection_status;


// put function declarations here:
Arduino_H7_Video Display(800, 480);
Arduino_GigaDisplayTouch touchDetector;
GigaDisplayRGB rgb_led;

//OS relevant structures
rtos::Thread status_thread;
rtos::Mutex mutex;

WiFiClient client;


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

void connectToPreferredWifi(const char* ssid, const char* pass);

static char ssid[] = SECRET_SSID;
static char pass[] = SECRET_PW;
// auto event = myQueue.make
void setup() {

  delay(1000);
  rgb_led.begin();
  status_thread.start(status_thread_handler);

  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with the WiFi module failed");
    while (true);
  }
  // WiFi.setHostname("AGR1_ControlCenter");

  // connectToPreferredWifi(ssid,pass);

  if(connection_status.wifi_status == WL_CONNECTED){
    client.connect("rapidapi.com",80);

    Serial.println("Connected!");

  }

  if (!Display.begin()) {
    Serial.println("Display initialized");
  }
  if (touchDetector.begin()) {
    Serial.println("TouchDetector initialized");
  }
  

  ui_init();

}

void loop() {
  lv_timer_handler();
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
          delay(500);
        };
        if (WiFi.status() == WL_CONNECTED) {
          Serial.print("Connected to Network : ");
          Serial.println(WiFi.SSID(currentNetwork));
          connection_status.wifi_status = WL_CONNECTED;
        }
      }
    }
  }
};
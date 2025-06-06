#include "helper.h"
#include <BLE.h>
#include <Arduino.h>
#include <stdint.h>
#include <WiFi.h>

uint8_t searchWifiDevices(void){

    uint8_t foundWifiDevices = WiFi.scanNetworks();
    return foundWifiDevices;
};


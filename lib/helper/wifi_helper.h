#ifndef WIFI_HELPER 
#define WIFI_HELPER
#include <WiFi.h>
#include <string.h>
#include "custom_typedefs.h"
extern connection_status_t connection_status;
void connectToWifiNetwork(const char* ssid, const char* pass);

#endif //WIFI_HELPER
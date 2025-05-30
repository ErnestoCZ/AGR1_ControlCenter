#pragma once
#include <WiFi.h>



typedef struct connection_status{
  uint8_t wifi_status = WL_IDLE_STATUS;
} connection_status_t;
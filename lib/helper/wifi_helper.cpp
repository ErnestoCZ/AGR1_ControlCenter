#include "wifi_helper.h"

void connectToWifiNetwork(const char* ssid, const char* pass) {
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
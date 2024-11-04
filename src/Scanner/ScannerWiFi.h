#ifndef SCANNER_WIFI_H
#define SCANNER_WIFI_H

#include "config.h"

#include <HTTPClient.h>
#include <WiFi.h>

namespace ScannerWifi
{
  void init_wifi()
  {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ICRS_SSID, ICRS_PASSWORD);

    Serial.print("Connecting to WiFi ..");

    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print('.');
      delay(1000);
    }

    Serial.println(WiFi.localIP());
  }

  bool ready() {
    return WiFi.isConnected();
  }
} // namespace ScannerWifi

#endif

#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "userinfo.h"
#include "config.h"
#include "display.h"

int WIFI_TIMEOUT = 10000;
int WIFI_CHECK_INTERVAL = 10000;
int wifiCheckTime = 0;

// TODO: switch to WIFI Manager https://github.com/tzapu/WiFiManager


void connectWifi(){
  int WifiTimeLimit = millis() + WIFI_TIMEOUT;
  Serial.print("Connecting to WiFi ..");
  clearScreen();
  cleared = false;
  tft.setCursor(12, 12);
  tft.print("connecting...");


  while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print('.');
      delay(1000);

      if (millis()  > WifiTimeLimit)
      {
        clearScreen();
        tft.setCursor(12, 12);
        Serial.println("Failed to connect to WiFi");
        tft.print("No WiFi :(");
        delay(5000);
        return;
      }
    }

    tft.print("Connected!");
    Serial.println(WiFi.localIP());
}


void initWifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    connectWifi();

    WiFi.onEvent(
        [](WiFiEvent_t event, WiFiEventInfo_t info) {
            Serial.println("WiFi disconnected");
            connectWifi();
        },
        WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
}

void autoReconnectWifi(){
  if (millis() < wifiCheckTime + WIFI_CHECK_INTERVAL) return;
  wifiCheckTime = millis();
  if ((WiFi.status() == WL_CONNECTED)) return;
  connectWifi();
}

UserInfo getUserInfo(String uid)
{
  UserInfo user;
  HTTPClient http;

  http.begin(ICRS_SERVER_HOST, ICRS_SERVER_PORT, "/api/access/member/permissions/uuid?uuid=" + uid); // HTTP
  http.setAuthorization(ICRS_SERVER_USERNAME, ICRS_SERVER_PASSWORD);

  int response_code = http.GET();

  Serial.println(response_code);

  if (response_code > 0)
  {
    String s = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, s);

    JsonObject obj = doc.as<JsonObject>();

    String shortcode = obj["shortcode"].as<String>();
    bool inducted = obj["inducted"].as<bool>();

    user.found = true;
    user.shortcode = shortcode;
    user.inducted = inducted;
    Serial.println(s);
  }
  http.end();

  return user;
}


int sendToPrintWindow(String uid)
{
  HTTPClient http;

  http.begin(ICRS_SERVER_HOST, ICRS_SERVER_PORT, "/api/access/print-window/update?uuid=" + uid); // HTTP
  http.setAuthorization(ICRS_SERVER_USERNAME, ICRS_SERVER_PASSWORD);

  int response_code = http.GET();

  Serial.println(response_code);

  return response_code;
}

#endif

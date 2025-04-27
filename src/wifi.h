#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include "userinfo.h"
#include "config.h"

void initWifi()
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

  String card_uid = "";
bool card_uid_set = false;

void waitForWifi(){
  if ((WiFi.status() == WL_CONNECTED)) return;

  // reconnect
  Serial.println("Reconnecting to WiFi...");
  WiFi.disconnect();
  WiFi.reconnect();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
}


UserInfo getUserInfo(String uid)
{
  UserInfo user;

  Serial.println("basic_info");
  waitForWifi();

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
  waitForWifi();
  HTTPClient http;

  http.begin(ICRS_SERVER_HOST, ICRS_SERVER_PORT, "/api/access//print-window/update?uuid=" + uid); // HTTP
  http.setAuthorization(ICRS_SERVER_USERNAME, ICRS_SERVER_PASSWORD);

  int response_code = http.GET();

  Serial.println(response_code);

  return response_code;
}

#endif

#include "ScannerWiFi.h"

#include <Wire.h>
#include <ArduinoJson.h>

// #include <HTTPClient.h>

String card_uid = "";
bool card_uid_set = false;

basic_info::UserInfo get_info(const String &uid)
{
  basic_info::UserInfo user;

  if ((WiFi.status() == WL_CONNECTED))
  {
    Serial.println("Attempting to POST ID To Server");
    HTTPClient http;

    // configure traged server and url
    http.begin(ICRS_SERVER_HOST, ICRS_SERVER_PORT, "/member/permissions/uuid?uuid=" + uid); // HTTP
    http.setAuthorization(ICRS_SERVER_USERNAME, ICRS_SERVER_PASSWORD);

    // http.addHeader("Content-Type", "application/json");
    // http.addHeader("Accept", "*/*");
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
  }
  
  return user;
  
}





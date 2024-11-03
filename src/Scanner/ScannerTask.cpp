#include "tasks.h"
#include "ScannerWiFi.h"

#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

#include <ArduinoJson.h>

// #include <HTTPClient.h>

String card_uid = "";
bool card_uid_set = false;

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

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

basic_info::UserInfo scanned_user_info;

void readNFC()
{
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
    tag.print();

    int uidLength = tag.getUidLength();
    byte uid_[uidLength];
    tag.getUid(uid_, uidLength);

    String tagId = "";
    for (size_t i = 0; i < uidLength; i++)
    {
      if (uid_[i] < 16)
      {
        tagId += "0";
      }
      tagId += String(uid_[i], HEX);
    }

    Serial.println(card_uid);
    tagId.toUpperCase();
    card_uid = tagId;
    auto info = get_info(card_uid);
    scanned_user_info = info;

    card_uid_set = true;
    current_view = view::BASIC_INFO;
    // tagId = tag.getUidString();
    Serial.println(tagId);
  }
  delay(50);
}

void ScannerTask(void *pvParameters)
{
  (void)pvParameters;
  // Setup I2C bus
  // Wire.begin(21, 22, 100000);  
  // Begin NFC
  
  ScannerWifi::init_wifi();
  // Setup some freeRTOS stuff
  const TickType_t xFrequency = 1000 / 24; // 100 ms period
  TickType_t xLastTickTime = xTaskGetTickCount();
  Serial.println("NFC Begin");
  // xTaskDelayUntil(&xLastTickTime, xFrequency * 5);
  
  nfc.begin();

  for (;;)
  {
    vTaskDelayUntil(&xLastTickTime, xFrequency);
    readNFC();
  }
}



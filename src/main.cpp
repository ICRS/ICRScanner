#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <Wire.h>
#include <mutex>

#include "tasks.h"
#include "config.h"

#include "GUI/ViewController.h"

const UBaseType_t taskPriority = 1;

TaskHandle_t xGUITaskHandle;
TaskHandle_t xWifiTaskHandle;
TaskHandle_t xScannerTaskHandle;


void IRAM_ATTR nav_button_isr()
{
  nav_button.pressed = true;
}

void IRAM_ATTR ent_button_isr()
{
  ent_button.pressed = true;
}

Adafruit_PN532 nfc(PN532_IRQ, PN532_RST);
basic_info::UserInfo scanned_user_info;
bool success = false;

uint8_t uid[] = {0, 0, 0, 0, 0, 0, 0};
uint8_t uidLength = 4; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

bool detected = false;
void IRAM_ATTR scanner_interrupt()
{
  detected = true;
}

void ScannerTask(void *pvParameters)
{
  (void)pvParameters;

  const TickType_t xFrequency = 1000 / 24; // 100 ms period
  TickType_t xLastTickTime = xTaskGetTickCount();
  Serial.println("NFC Begin");

  for (;;)
  {
    if (detected)
    {
      detected = false;

      if (current_view != SCAN_CARD) {
        nfc.readDetectedPassiveTargetID(uid, &uidLength);
        nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
        continue;
      }

      success = nfc.readDetectedPassiveTargetID(uid, &uidLength);
      // Serial.println("Scanner");

      if (success)
      {
        nfc.PrintHex(uid, uidLength);
        String tagId = "";
        for (size_t i = 0; i < uidLength; i++)
        {
          if (uid[i] < 16)
          {
            tagId += "0";
          }
          tagId += String(uid[i], HEX);
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
      nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
    }
    xTaskDelayUntil(&xLastTickTime, xFrequency * 5);
  }
}

void WifiTask(void *)
{
  const TickType_t xFrequency = 1000 / 24; // 100 ms period
  TickType_t xLastTickTime = xTaskGetTickCount();
  Serial.println("Init Wifi Begin");

  ScannerWifi::init_wifi();

  bool render_updated = true;
  for (;;) {
    if (!ScannerWifi::ready()) {
      ScannerWifi::reconnect_wifi();
      render_no_wifi_icon(true);
    } else {
      render_no_wifi_icon(false);
    }
    xTaskDelayUntil(&xLastTickTime, xFrequency * 5);
  }
} 


void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(10); // for Leonardo/Micro/Zero

  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata)
  {
    Serial.print("Didn't find PN53x board");
    while (1)
      ; // halt
  }
  Serial.println("attach interrupt");
  // nfc.SAMConfig();
  Serial.print("Found chip PN5");
  Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. ");
  Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF, DEC);

  // nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
  Serial.println("Registering ISRs");
  nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);

  pinMode(nav_button.PIN, INPUT_PULLUP);
  pinMode(ent_button.PIN, INPUT_PULLUP);
  attachInterrupt(nav_button.PIN, nav_button_isr, FALLING);
  attachInterrupt(ent_button.PIN, ent_button_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(PN532_IRQ), scanner_interrupt, FALLING);


  Serial.println("ISRs Registered");

  xTaskCreate(
      WifiTask,
      "Wifi Task",
      5000,
      NULL,
      taskPriority,
      &xGUITaskHandle);

  xTaskCreate(
      GUITask,
      "GUI Task",
      5000,
      NULL,
      taskPriority + 1,
      &xGUITaskHandle);

  xTaskCreate(
      ScannerTask,
      "Scanner Task",
      5000,
      NULL,
      taskPriority + 2,
      &xScannerTaskHandle);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

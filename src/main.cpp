#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <Wire.h>
#include <mutex>

#include "tasks.h"
#include "config.h"

#include "GUI/ViewController.h"

#define INCLUDE_vTaskSuspend 1
#define INCLUDE_xTaskResumeFromISR 1


const UBaseType_t taskPriority = 1;

TaskHandle_t xGUITaskHandle;
TaskHandle_t xWifiTaskHandle;
TaskHandle_t xScannerTaskHandle;


GUI::ViewController viewController;

long nav = 0;
void IRAM_ATTR nav_button_isr()
{
  if ((nav + 300) > millis())
  {
    return;
  }

  nav = millis();

  uint32_t ulPreviousValue = 0;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  xTaskNotifyAndQueryFromISR(
      xGUITaskHandle,
      0x101,
      eSetValueWithOverwrite,
      &ulPreviousValue,
      &xHigherPriorityTaskWoken);

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  // if(ulPreviousValue & (1 << 2)) {
  //   auto v = xTaskResumeFromISR(xGUITaskHandle);
  //   if (v == pdTRUE) portYIELD_FROM_ISR();
  // }
}

long ent = 0;
void IRAM_ATTR ent_button_isr()
{
  if ((ent + 300) > millis())
  {
    return;
  }
  ent = millis();

  uint32_t ulPreviousValue = 0;
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  xTaskNotifyAndQueryFromISR(
      xGUITaskHandle,
      0x11,
      eSetValueWithOverwrite,
      &ulPreviousValue,
      &xHigherPriorityTaskWoken);

  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  // if(ulPreviousValue) {
  //   auto v = xTaskResumeFromISR(xGUITaskHandle);
  //   if (v == pdTRUE) portYIELD_FROM_ISR();
  // }
}

void loadFonts()
{
  // make sure all the font files are loaded
  if (!SPIFFS.begin())
  {
    Serial.println("SPIFFS initialisation failed!");
    while (1)
      yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS available!");

  // ESP32 will crash if any of the fonts are missing
  bool font_missing = false;
  if (SPIFFS.exists("/PixelOperatorMono.vlw") == false)
    font_missing = true;
  if (SPIFFS.exists("/PixelOperatorMonoBold.vlw") == false)
    font_missing = true;

  if (font_missing)
  {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while (1)
      yield();
  }
  else
    Serial.println("\r\nFonts found OK.");
}

void GUITask(void *pvParameters)
{
  (void)pvParameters;
  GUI::tft.begin();
  GUI::tft.setRotation(1);
  GUI::tft.fillScreen(TFT_BLACK);

  loadFonts();
  // handleScan();

  const TickType_t xFrequency = pdMS_TO_TICKS( 100 ); // 100 ms period
  TickType_t xLastTickTime = xTaskGetTickCount();

  xTaskNotifyStateClear(xGUITaskHandle);
  for (;;) {
    uint32_t v = 0;
    if (v = ulTaskNotifyTake(pdTRUE, xFrequency))
    {
      
      switch (v & 0x111)
      {
      case (0x101):
      //   viewController.handle_double_up(); 
      //   break;
      // case (0x100):
        viewController.handle_single_up(); 
        break;
      case (0x011):
      //   viewController.handle_double_down(); 
      //   break;
      // case (0x010):
        viewController.handle_single_down(); 
        break;
      default:
        break;
      }
    }

    viewController.render();
    // ulTaskNotifyTake(pdTRUE, xFrequency * 50);
  }
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

        viewController.request_view(GUI::view_t::BASIC_INFO);
        card_uid_set = true;
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

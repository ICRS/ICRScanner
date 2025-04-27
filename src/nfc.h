#ifndef NFC_H
#define NFC_H

#include <Arduino.h>
#include <Adafruit_PN532.h>
#include "config.h"

int NFC_TIMEOUT = 2000;
bool readyToScan = true;
int last_NFC = 0;

Adafruit_PN532 nfc(PN532_IRQ, PN532_RST);
uint8_t uidLength = 4;
uint8_t uid[7] = {0, 0, 0, 0, 0, 0, 0};

void loadNFC(){
    nfc.begin();

    uint32_t versiondata = nfc.getFirmwareVersion();
    if (!versiondata)
    {
      Serial.print("Didn't find PN53x board");
      while (1)
        ; // halt
    }
    Serial.println("Registering ISRs");
    nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);

    Serial.println("ISRs Registered");
}

String readNFC()
{
  int time = millis();
  if (!last_NFC < time + NFC_TIMEOUT) return "";


  readyToScan = true;

  bool foundNFC = nfc.readDetectedPassiveTargetID(uid, &uidLength);
  if (!foundNFC) return "";

  last_NFC = millis();
  readyToScan = false;

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

  tagId.toUpperCase();
  Serial.println("card_uid");
  Serial.println(tagId);

  return tagId;
//   nfc.startPassiveTargetIDDetection(PN532_MIFARE_ISO14443A);
}

#endif

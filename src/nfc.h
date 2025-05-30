#ifndef NFC_H
#define NFC_H

#include <Arduino.h>
#include <Adafruit_PN532.h>
#include "config.h"

int NFC_TIMEOUT = 2000;
bool readyToScan = true;
int lastNFC = 0;

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
}

String readNFC()
{
  if (lastNFC > (millis() - NFC_TIMEOUT)) return "";

  readyToScan = true;

  bool foundNFC = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 100);

  if (!foundNFC) {
    Serial.println("No NFC detected");
    return "";
  }


  lastNFC = millis();
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
}

#endif

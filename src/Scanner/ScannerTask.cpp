#include "tasks.h"
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>


PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void readNFC() {
  if (nfc.tagPresent()) {
    NfcTag tag = nfc.read();
    tag.print();

    int uidLength = tag.getUidLength();
    byte uid[uidLength];
    tag.getUid(uid, uidLength);
    
    String tagId = "";
    for(size_t i = 0; i < uidLength; i++)
    {
      if(uid[i] < 16)
      {
        tagId += "0";
      }
      tagId += String(uid[i], HEX);
    }
    tagId.toUpperCase();
    // tagId = tag.getUidString();
    Serial.println(tagId);
  }
  delay(50);
}




void ScannerTask(void *pvParameters)
{
    (void)pvParameters;
    // Setup I2C bus
    Wire.begin(SCANNER_SDA,SCANNER_SCL,100000);
    // Begin NFC
    nfc.begin();

    // Setup some freeRTOS stuff
    const TickType_t xFrequency = 1000 / 24; // 100 ms period
    TickType_t xLastTickTime = xTaskGetTickCount();
    for (;;)
    {
        vTaskDelayUntil(&xLastTickTime, xFrequency);
        readNFC();
    }
}

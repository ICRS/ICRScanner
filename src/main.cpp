#include <Arduino.h>
#include "config.h"
#include "nfc.h"
#include "display.h"
#include "wifi.h"

#include "routines/get_info.h"
#include "routines/printer_auth.h"

int routine = 0;

void setup()
{
  Serial.begin(115200);

  initDisplay();
  loadNFC();
  initWifi();

  pinMode(NAV_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ENT_BUTTON_PIN, INPUT_PULLUP);

}

void handleButtons(){
  if (digitalRead(NAV_BUTTON_PIN) == LOW)
  {
    routine++;
    if (routine > 3)
    {
      routine = 0;
    }
    delay(500);
  }
}

void performRoutine(){
  switch (routine)
  {
  case 0:
    Serial.println("Get Info");
    getInfoRoutine();
    break;
  case 1:
    printerAuthRoutine();
    Serial.println("Auth Printing");
    break;
  case 2:
    Serial.println("TODO: Print Label");
    break;
  case 3:
    Serial.println("TODO: Add to DB");
    break;
  default:
    break;
  }
}

void loop()
{
  handleButtons();
  performRoutine();

  delay(100);
}

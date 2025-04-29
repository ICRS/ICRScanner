#include <Arduino.h>
#include "config.h"
#include "nfc.h"
#include "display.h"
#include "wifi.h"

#include "routines/get_info.h"
#include "routines/printer_auth.h"
#include "routines/print_label.h"


int routine = 0;

void setup()
{
  Serial.begin(115200);

  initDisplay();
  initWifi();
  loadNFC();

  pinMode(NAV_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ENT_BUTTON_PIN, INPUT_PULLUP);

}

void handleButtons(){
  if (digitalRead(NAV_BUTTON_PIN) == LOW)
  {
    routine = (routine + 1) % 3;
    clearScreen();
    delay(200);
  }
}

void performRoutine(){
  switch (routine)
  {
  case 0:
    getInfoRoutine();
    break;
  case 1:
    printerAuthRoutine();
    break;
  case 2:
    printlabelRoutine();
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
  autoReconnectWifi();

  delay(300);
}

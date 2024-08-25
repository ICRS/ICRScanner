#include <Arduino.h>
#include "tasks.h"
#include "config.h"

const UBaseType_t taskPriority = 1;

TaskHandle_t xGUITaskHandle = NULL;


void IRAM_ATTR nav_button_isr()
{
  nav_button.pressed = true;
}

void IRAM_ATTR ent_button_isr()
{
  ent_button.pressed = true;
}

void setup()
{
  Serial.begin(115200);

  Serial.println("Registering ISRs");
  pinMode(nav_button.PIN, INPUT_PULLUP);
  pinMode(ent_button.PIN, INPUT_PULLUP);
  attachInterrupt(nav_button.PIN, nav_button_isr, FALLING);
  attachInterrupt(ent_button.PIN, ent_button_isr, FALLING);
  Serial.println("ISRs Registered");

  
  
  xTaskCreate(
      GUITask,
      "GUI Task",
      5000,
      NULL,
      taskPriority,
      &xGUITaskHandle);
}

void loop()
{
  // put your main code here, to run repeatedly:
}

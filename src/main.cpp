#include <Arduino.h>
#include "tasks.h"

const UBaseType_t taskPriority = 1;



void setup()
{
  TaskHandle_t xGUITaskHandle = NULL;
  Serial.begin(115200);
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

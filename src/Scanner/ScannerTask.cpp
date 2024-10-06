#include "tasks.h"
#include <Adafruit_PN532.h>

void ScannerTask(void *pvParameters)
{
    (void)pvParameters;
    // Setup TFT

    // Setup some freeRTOS stuff
    const TickType_t xFrequency = 1000 / 24; // 100 ms period
    TickType_t xLastTickTime = xTaskGetTickCount();
    for (;;)
    {
        vTaskDelayUntil(&xLastTickTime, xFrequency);
    }
}

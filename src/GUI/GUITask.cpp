#include "tasks.h"
#define FONT_SMALL "PixelOperatorMono"
// Font files are stored in SPIFFS, so load the library
#include <FS.h>

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();

void loadFonts(){
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
    //Setup TFT
    tft.begin();
    tft.setRotation(1);

    //Load the fonts
    loadFonts();
    
    //Setup some freeRTOS stuff
    const TickType_t xFrequency = 1000 / 1; // 100 ms period
    TickType_t xLastTickTime = xTaskGetTickCount();

    for (;;)
    {

    }
}

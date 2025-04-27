#ifndef DISPLAY_H
#define DISPLAY_H

#include "assets.h"

#define FONT_SMALL "PixelOperatorMono"
#define FONT_BIG "PixelOperatorMonoBold"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
int animationState = 0;

void clearScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.drawBitmap(2, 2, epd_bitmap_nfc_border, 236, 131, TFT_WHITE);
    tft.loadFont(FONT_BIG);
}

void waitingScreen(){
    clearScreen();
    tft.setCursor(12, 12);
    tft.print("Scan your");
    tft.setCursor(12, 44);
    tft.print("Card!");

    (++animationState) %= 4;

    tft.fillRect(110, 31, 25, 63, TFT_BLACK);
    tft.fillRect(135, 8, 67, 108, TFT_BLACK);

    if (animationState >= 0)
        tft.drawBitmap(111, 58, epd_bitmap_wave_1, 21, 34, TFT_WHITE);
    if (animationState >= 1)
        tft.drawBitmap(123, 41, epd_bitmap_wave_2, 31, 57, TFT_WHITE);
    if (animationState >= 2)
        tft.drawBitmap(135, 25, epd_bitmap_wave_3, 42, 81, TFT_WHITE);
    if (animationState >= 3)
        tft.drawBitmap(147, 9, epd_bitmap_wave_4, 53, 104, TFT_WHITE);
}

void initDisplay(){
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

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

#endif
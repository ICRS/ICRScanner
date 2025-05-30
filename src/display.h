#ifndef DISPLAY_H
#define DISPLAY_H

#include "assets.h"

#define FONT_SMALL "PixelOperatorMono"
#define FONT_BIG "PixelOperatorMonoBold"
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
int animationState = 0;
bool cleared = false;

void clearScreen()
{
    tft.fillScreen(TFT_BLACK);
    tft.drawBitmap(2, 2, epd_bitmap_nfc_border, 236, 131, TFT_WHITE);
    tft.loadFont(FONT_BIG);
}

void displayLoading(String uid){
  clearScreen();
  tft.setCursor(12, 12);
  tft.print(uid);
  tft.drawWideLine(12.0f, 40.0f, 228.0f, 40.0f, 2.0f, TFT_WHITE);

  tft.setCursor(12, 50);
  tft.print("Loading...");
};

void waitingScreen(String screenName = ""){
    tft.loadFont(FONT_SMALL);
    tft.setCursor(12, 100);
    tft.print(screenName);

    tft.loadFont(FONT_BIG);
    tft.setCursor(12, 12);
    tft.print("Scan your");
    tft.setCursor(12, 38);
    tft.print("Card!");

    (++animationState) %= 4;

    tft.drawBitmap(72, 75, epd_bitmap_harry, 58, 40, TFT_WHITE);


    tft.fillRect(130, 31, 25, 63, TFT_BLACK);
    tft.fillRect(155, 8, 67, 108, TFT_BLACK);

    if (animationState >= 0)
        tft.drawBitmap(131, 58, epd_bitmap_wave_1, 21, 34, TFT_WHITE);
    if (animationState >= 1)
        tft.drawBitmap(143, 41, epd_bitmap_wave_2, 31, 57, TFT_WHITE);
    if (animationState >= 2)
        tft.drawBitmap(155, 25, epd_bitmap_wave_3, 42, 81, TFT_WHITE);
    if (animationState >= 3)
        tft.drawBitmap(167, 9, epd_bitmap_wave_4, 53, 104, TFT_WHITE);
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

  clearScreen();
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(1);
  tft.setCursor(12, 100);
  tft.print("Starting up...");
}

#endif
#include "tasks.h"
#include "assets.h"
#define FONT_SMALL "PixelOperatorMono"
#define FONT_BIG "PixelOperatorMonoBold"
#include <vector>

// Font files are stored in SPIFFS, so load the library
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

enum view
{
    SCAN_CARD,
    MENU,
    BASIC_INFO,
    PRINT_INFO,
    ACTION_MENU,
    SETTINGS
};

struct item
{
    String name;
    const uint8_t *icon;
    view next_view;
};

bool new_view = true;
int nfc_animation_state = 0;
long nfc_last_update = millis();

Button nav_button = {NAV_BUTTON_PIN, false};
Button ent_button = {ENT_BUTTON_PIN, false};

view current_view = SCAN_CARD;
int menu_cursor = 0;
TFT_eSPI tft = TFT_eSPI();
int cursor[2] = {0, 0};

std::vector<item> menu_items = {
    {"Basic Info", epd_bitmap_basic_info, BASIC_INFO},
    {"Print Info", epd_bitmap_print_info, PRINT_INFO},
    {"Actions", epd_bitmap_action, ACTION_MENU},
    {"Settings", epd_bitmap_settings, ACTION_MENU}};

void loadFonts()
{
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

// percent: 0 - 100
void handleScroller(int percent)
{
    // tft.drawRect(234, 0, 6, 135, TFT_BLACK);
    tft.drawBitmap(235, 2, epd_bitmap_scroll, 2, 130, TFT_WHITE);
    int y = round(map(percent, 0, 100, 8, 108));
    tft.drawBitmap(234, y, epd_bitmap_handle, 4, 10, TFT_WHITE);
}

void handleMenu(int index)
{

    std::vector<item> current_items;
    // Serial.println("1");
    tft.fillScreen(TFT_BLACK);
    int pos;

    // Serial.println("2");
    if (index == 0)
    {
        pos = 0;
        current_items.push_back(menu_items[0]);
        current_items.push_back(menu_items[1]);
        current_items.push_back(menu_items[2]);
        // Serial.println("3a");
    }
    else if (index == (menu_items.size() - 1))
    {
        pos = 2;
        current_items.push_back(menu_items[index - 2]);
        current_items.push_back(menu_items[index - 1]);
        current_items.push_back(menu_items[index]);
        // Serial.println("3b");
    }
    else
    {
        pos = 1;
        current_items.push_back(menu_items[index - 1]);
        current_items.push_back(menu_items[index]);
        current_items.push_back(menu_items[index + 1]);
        // Serial.println("3c");
    }
    // Serial.println("3");
    // === ICONS === //

    // Serial.println("4");
    // === TEXT === //
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    // Serial.println("5");
    for (int i = 0; i < 3; i++)
    {
        if (i == pos)
        {
            tft.loadFont(FONT_BIG);
            tft.drawBitmap(1, 1 + (i * 45), epd_bitmap_border, 232, 43, TFT_WHITE);
        }
        else
        {
            tft.loadFont(FONT_SMALL);
        }
        tft.drawBitmap(5, 5 + 45 * i, current_items[i].icon, 32, 32, TFT_WHITE);
        tft.setCursor(45, 12 + 45 * i);
        tft.print(current_items[i].name);
    }
    // Serial.println("6");
    handleScroller(static_cast<int>(100 * index / (menu_items.size() - 1)));
}

void handleNavigation()
{
    if (nav_button.pressed)
    {
        menu_cursor = (menu_cursor + 1) % menu_items.size();
        switch (current_view)
        {
        case (MENU):
            handleMenu(menu_cursor);
            break;
        default:
            handleMenu(menu_cursor);
            break;
        }
        nav_button.pressed = false;
    }
    if (ent_button.pressed)
    {
        menu_cursor = (menu_cursor + 1) % menu_items.size();
        nav_button.pressed = false;
    }
}

void handleScan()
{
    if (new_view){
        tft.fillScreen(TFT_BLACK);
        // Render border
        tft.drawBitmap(2, 2, epd_bitmap_nfc_border, 236, 131, TFT_WHITE);
        // Render harry
        tft.drawBitmap(52, 75, epd_bitmap_harry, 58, 40, TFT_WHITE);
        // Write Message
        tft.loadFont(FONT_BIG);
        tft.setCursor(12,12);
        tft.print("Scan your");
        tft.setCursor(12,44);
        tft.print("Card!");
        new_view = false;
    }


    if ((millis() - nfc_last_update) > 1000)
    {
        nfc_animation_state = (nfc_animation_state + 1) % 4;
        nfc_last_update = millis();
        
        tft.fillRect(110,31,25,63,TFT_BLACK);
        tft.fillRect(135,8,67,108,TFT_BLACK);
    Serial.printf("Draw state: %d\n",nfc_animation_state);
        if (nfc_animation_state >= 0)
        {
            tft.drawBitmap(111, 58, epd_bitmap_wave_1, 21, 34, TFT_WHITE);
        }
        if (nfc_animation_state >= 1)
        {
            tft.drawBitmap(123, 41, epd_bitmap_wave_2, 31, 57, TFT_WHITE);
        }
        if (nfc_animation_state >= 2)
        {
            tft.drawBitmap(135, 25, epd_bitmap_wave_3, 42, 81, TFT_WHITE);
        }
        if (nfc_animation_state >= 3)
        {
            tft.drawBitmap(147, 9, epd_bitmap_wave_4, 53, 104, TFT_WHITE);
        }
    }
}

void handleViews()
{
    switch (current_view)
    {
    case (SCAN_CARD):
        handleScan();
        break;
    default:
        handleNavigation();
        break;
    }
}

void GUITask(void *pvParameters)
{
    (void)pvParameters;
    // Setup TFT
    tft.begin();
    tft.setRotation(1);

    // Load the fonts

    loadFonts();
    handleScan();
    // Setup some freeRTOS stuff
    const TickType_t xFrequency = 1000 / 24; // 100 ms period
    TickType_t xLastTickTime = xTaskGetTickCount();
    for (;;)
    {
        vTaskDelayUntil(&xLastTickTime, xFrequency);
        handleViews();
    }
}

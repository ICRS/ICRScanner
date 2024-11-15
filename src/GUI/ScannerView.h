#ifndef SCANNER_VIEW_H
#define SCANNER_VIEW_H

#include "GUIView.h"

namespace GUI
{
    class ScannerView : public View
    {
        bool new_view = true;
        u_int8_t nfc_animation_state = 0;
        unsigned long nfc_last_update = 0;

    public:
        ScannerView() = default;

        void reset() override {
            View::reset();
            new_view = true;
            nfc_animation_state = 0;
            long nfc_last_update = 0;
            
        }

        void render() override
        {
            render_no_wifi();
            // Serial.println("Rendering Scanner View");

            if (new_view)
            {
                tft_.fillScreen(TFT_BLACK);
                // Render border
                tft_.drawBitmap(2, 2, epd_bitmap_nfc_border, 236, 131, TFT_WHITE);
                // Render harry
                tft_.drawBitmap(52, 75, epd_bitmap_harry, 58, 40, TFT_WHITE);
                // Write Message
                tft_.loadFont(FONT_BIG);
                tft_.setCursor(12, 12);
                tft_.print("Scan your");
                tft_.setCursor(12, 44);
                tft_.print("Card!");
                new_view = false;
            }

            if ((millis() - nfc_last_update) > 1000)
            {
                (++nfc_animation_state) %= 4;
                nfc_last_update = millis();

                tft_.fillRect(110, 31, 25, 63, TFT_BLACK);
                tft_.fillRect(135, 8, 67, 108, TFT_BLACK);
                Serial.printf("Draw state: %d\n", nfc_animation_state);
                if (nfc_animation_state >= 0)
                {
                    tft_.drawBitmap(111, 58, epd_bitmap_wave_1, 21, 34, TFT_WHITE);
                }
                if (nfc_animation_state >= 1)
                {
                    tft_.drawBitmap(123, 41, epd_bitmap_wave_2, 31, 57, TFT_WHITE);
                }
                if (nfc_animation_state >= 2)
                {
                    tft_.drawBitmap(135, 25, epd_bitmap_wave_3, 42, 81, TFT_WHITE);
                }
                if (nfc_animation_state >= 3)
                {
                    tft_.drawBitmap(147, 9, epd_bitmap_wave_4, 53, 104, TFT_WHITE);
                }
            }
        }
    };
};

#endif
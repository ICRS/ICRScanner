#ifndef BASIC_DETAIL_VIEW_H
#define BASIC_DETAIL_VIEW_H

#include "GUIView.h"
#include "basic_info.h"

namespace GUI
{
    class BasicDetailView : public View
    {
        bool basic_info_updated = false;
        basic_info::UserInfo user_info;

    public:
        BasicDetailView() {}

        void set_user_info(const basic_info::UserInfo &info)
        {
            user_info = info;
        }

        void set_user_info(basic_info::UserInfo &&info)
        {
            user_info = info;
            // info = nullptr;
        }

        void reset() override
        {
            View::reset();
            basic_info_updated = false;
        }

        void render() override
        {
            if (!basic_info_updated)
            {
                tft_.fillScreen(TFT_BLACK);
                tft_.drawBitmap(2, 2, epd_bitmap_nfc_border, 236, 131, TFT_WHITE);
                tft_.loadFont(FONT_BIG);
                tft_.setCursor(12, 12);

                tft_.print(card_uid);
                tft_.drawWideLine(12.0f, 40.0f, 228.0f, 40.0f, 2.0f, TFT_WHITE);

                tft_.setCursor(12, 50);
                if (scanned_user_info.found)
                {
                    tft_.print("Shortcode: " + scanned_user_info.shortcode);
                    tft_.setCursor(12, 82);
                    String v = scanned_user_info.inducted ? "True" : "False";
                    tft_.print("Inducted:  " + v);
                }
                else
                {
                    tft_.print("User not found");
                }
                basic_info_updated = true;
            }
        }
    };
};

#endif
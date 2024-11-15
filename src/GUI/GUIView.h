#ifndef GUI_VIEW_H
#define GUI_VIEW_H

#include <WiFi.h>
#include <vector>

#include "assets.h"

#define FONT_SMALL "PixelOperatorMono"
#define FONT_BIG "PixelOperatorMonoBold"

// Font files are stored in SPIFFS, so load the library
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library


namespace GUI {
    TFT_eSPI tft = TFT_eSPI();

    class View
    {
        bool wifi_last_status = WiFi.isConnected();

        protected:
            TFT_eSPI& tft_ = tft;

        public:
        virtual void render() {
            render_no_wifi();
        }

        virtual void render_no_wifi() {
            bool status = WiFi.isConnected();
            if (status != wifi_last_status) {
                render_no_wifi_icon(!(wifi_last_status = status));
            }
        }

        virtual void handle_up()
        {

        }

        virtual void handle_down()
        {
        }

        virtual void handle_double_up()
        {
        }

        virtual void handle_double_down()
        {
        }

        virtual void reset()
        {
            tft_.fillScreen(TFT_BLACK);
        }
    };
};

#endif
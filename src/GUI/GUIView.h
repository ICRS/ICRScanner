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
    class View
    {
        bool wifi_last_status = WiFi.isConnected();

        protected:
            TFT_eSPI tft_ = TFT_eSPI();

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

        virtual void reset() {
        }
    };
};

#endif
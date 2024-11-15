#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <unordered_map>
#include <memory>
#include <atomic>

#include "GUIView.h"
#include "ScannerView.h"
#include "BasicDetailView.h"

namespace GUI
{
    enum view_t
    {
        SCAN_CARD,
        MENU,
        BASIC_INFO,
        PRINT_INFO,
        ACTION_MENU,
        SETTINGS,
        CARD_INFO,
    };

    class ViewController
    {
        std::unordered_map<view_t, std::shared_ptr<View>> views = {
            {SCAN_CARD, std::make_shared<ScannerView>(ScannerView())},
            {BASIC_INFO, std::make_shared<BasicDetailView>(BasicDetailView())}
        };
        view_t current_view = SCAN_CARD;
        view_t last_view = SCAN_CARD;


    public:
        void handle_double_up() {
            Serial.println("Double Up");
            switch (current_view)
            {
            case SCAN_CARD:
                /* code */
                break;

            default:
                break;
            }
        }

        void handle_double_down()
        {
            Serial.println("Double Down");
            switch (current_view)
            {
            case SCAN_CARD:
                request_view(SCAN_CARD);
                break;

            default:
                break;
            }
        }

        void handle_single_up()
        {
            Serial.println("Single Up");
            switch (current_view)
            {
            case SCAN_CARD:
                /* code */
                break;

            default:
                break;
            }
        }

        void handle_single_down()
        {
            Serial.println("Single Down");
            switch (current_view)
            {
            case BASIC_INFO:
                request_view(SCAN_CARD);
                break;

            default:
                break;
            }
        }

        void render()
        {
            auto v = current_view;
            if (last_view != v)
            {
                views[last_view]->reset();
                last_view = v;
            }

            views[v]->render();
        }

        void request_view(const view_t &view)
        {
            if (current_view != view)
            {
                current_view = view;
            }
        }
    };
};

#endif
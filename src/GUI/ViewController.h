#ifndef VIEW_CONTROLLER_H
#define VIEW_CONTROLLER_H

#include <unordered_map>
#include <memory>

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
            {SCAN_CARD, std::make_shared<View>(ScannerView())},
            {BASIC_INFO, std::make_shared<View>(BasicDetailView())}};
        view_t current_view = SCAN_CARD;
        view_t last_view = SCAN_CARD;

    public:
        void render()
        {
            if (last_view != current_view)
            {
                views[last_view]->reset();
                last_view = current_view;
            }
            views[current_view]->render();
        }

        void request_view(const view_t& view)
        {
            if (current_view != view) {
                current_view = view;
            }
        }
    };
};

#endif
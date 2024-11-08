#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>
#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#include "assets.h"
#include "config.h"
#include "basic_info.h"

struct Button
{
	const uint8_t PIN;
	bool pressed;
};
extern Button nav_button;
extern Button ent_button;


void render_no_wifi_icon(bool on);

enum view
{
    SCAN_CARD,
    MENU,
    BASIC_INFO,
    PRINT_INFO,
    ACTION_MENU,
    SETTINGS,
    CARD_INFO
};

namespace ScannerWifi {
    bool ready();
    void init_wifi();
    void reconnect_wifi();
}

extern view current_view;

extern String card_uid;
extern bool card_uid_set;

extern basic_info::UserInfo scanned_user_info;

void GUITask(void *pvParamaters);

basic_info::UserInfo get_info(const String &uid);

#endif
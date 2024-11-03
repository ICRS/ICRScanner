#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>
#include <Arduino.h>
#include "config.h"

struct Button
{
	const uint8_t PIN;
	bool pressed;
};
extern Button nav_button;
extern Button ent_button;

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

extern view current_view;

extern String card_uid;
extern bool card_uid_set;

void GUITask(void *pvParamaters);
void ScannerTask(void *pvParamaters);

#endif
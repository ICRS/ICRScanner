#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>
#include "config.h"

struct Button {
	const uint8_t PIN;
	bool pressed;
};
extern Button nav_button;
extern Button ent_button;

void GUITask(void *pvParamaters);

#endif
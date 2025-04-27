#include <Arduino.h>

#ifndef BASIC_INFO_H
#define BASIC_INFO_H

struct UserInfo {
	bool found = false;
	bool can_print = false;
	bool inducted = false;
	String card_uid = "";
	String shortcode = "";
};

#endif

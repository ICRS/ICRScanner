#ifndef GUITASK_H
#define GUITASK_H

#include "assets.h"
#include "tasks.h"

#define FONT_SMALL "PixelOperatorMono"
#define FONT_BIG "PixelOperatorMonoBold"
#include <vector>

// Font files are stored in SPIFFS, so load the library
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

std::mutex rendering_mutex;

#endif
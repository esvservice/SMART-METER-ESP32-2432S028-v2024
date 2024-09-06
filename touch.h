#pragma once
#include "XPT2046_Touchscreen.h"
#define _sclk              25
#define _mosi              32 
#define _miso              39
#define TOUCH_IRQ_PIN      36
#define TOUCH_CS_PIN       33



XPT2046_Touchscreen touch(TOUCH_CS_PIN, TOUCH_IRQ_PIN);

TS_Point rawLocation;
int xt = rawLocation.x;// touch settings
int yt = rawLocation.y;//touch settings                  // set to 64 or 256
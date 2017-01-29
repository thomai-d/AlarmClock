#ifndef PIN_DEFS_H
#define PIN_DEFS_H

#include "Config.h"

#define DO_LED_R	5
#define DO_LED_G	9
#define DO_LED_B	10

#define DO_BEEP		3

// Display.
#define DO_DISPLAY_BL	6
#define DO_DISPLAY_A0	7
#define DO_DISPLAY_CS	8
#define DO_DISPLAY_RST	4

// Buttons.
#define DI_BUTTON2		2

// Analog
#ifdef ALTERNATIVE_LDR
#define AI_LDR			A7
#else
#define AI_LDR			A6
#endif

#endif

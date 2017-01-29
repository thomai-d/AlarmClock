#ifndef ICONS_H
#define ICONS_H

#include <Ucglib.h>
#include <stdint.h>

#include "Display.h"

class Icons
{
public:
	static void DrawAlarmClock(int xpos, int ypos, uint8_t r, uint8_t g, uint8_t b);
	static void DrawEditIcon(int xpos, int ypos, uint8_t r, uint8_t g, uint8_t b);

private:

	static void DrawIcon(const byte data[16][16], int xpos, int ypos, uint8_t r, uint8_t g, uint8_t b);
};

#endif



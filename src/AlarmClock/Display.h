#ifndef DISPLAY_H
#define DISPLAY_H

#include <Ucglib.h>
#include <stdint.h>

#include "InputHardware.h"
#include "PinDefinitions.h"
#include "Debug.h"
#include "Config.h"

class DisplayClass
{
public:
	void Initialize();

	void Print0(uint8_t x, uint8_t y, uint8_t value, uint8_t grayValue);
	void Print0(uint8_t value);

	void ActivateBacklight();
	void DeactivateBacklight();

	void OnTimer2Tick();

	bool IsBacklightOn() { return this->isBacklightOn; }
	
private:

	// Gets the target light level depeding
	// on the environment light.
	uint8_t GetAutoLevel();

	float currentLevel;
	uint8_t targetLevel;

	bool isBacklightOn;
	bool isAutoLevelEnabled;

	uint8_t lightSamples[LIGHT_SAMPLES];
	uint8_t lightSample = 0;
};

extern DisplayClass Display2;
extern Ucglib_ST7735_18x128x160_HWSPI Display;

#endif
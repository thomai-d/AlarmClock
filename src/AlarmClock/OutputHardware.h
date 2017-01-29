#ifndef OUTPUT_HARDWARE_H
#define OUTPUT_HARDWARE_H

#include <Arduino.h>
#include <hsv2rgb.h>
#include <EEPROM.h>

#include "PinDefinitions.h"

#define ALARM_TICK_MAX			250

class OutputHardwareClass
{
public:
	OutputHardwareClass();

	void AlarmTest();

	void SetAlarm(bool state);
	void SetLED(uint8_t intensity);

	void OnTimer2Tick();

	hsv GetColor() { return this->color; }
	void SetColor(hsv color);
	void PersistColor();

private:
	uint8_t alarmTickCounter;
	uint8_t alarmTickMax;
	bool alarmEnabled;

	uint8_t intensity;

	hsv color;
};

extern OutputHardwareClass OutputHardware;

#endif

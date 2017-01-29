#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>
#include <stdint.h>
#include <Time.h>

#include "Config.h"

class Alarm
{
public:
	void UpdateAlarmLevel(tmElements_t now);

	inline void Mute() { this->isMute = true; }
	inline void UnMute() { this->isMute = false; }
	inline void ToggleOn() { this->isOn = !this->isOn; }
	inline void Enable() { this->isOn = true; }

	void Set(uint8_t h, uint8_t m, bool on);

	void AddMinutes(uint8_t minutes);

	inline bool IsOn() { return this->isOn; }
	inline uint8_t GetAlarmLevel() { return this->alarmLevel; }
	inline uint8_t H() { return this->h; }
	inline uint8_t M() { return this->m; }

private:

	uint8_t	h;
	uint8_t m;
	bool isOn;

	uint8_t alarmLevel;
	bool isMute;
};

#endif
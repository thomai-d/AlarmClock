#include "Alarm.h"

uint32_t secondsSinceMidnight(uint8_t h, uint8_t m, uint8_t s = 0)
{
	return (uint32_t)h * 60 * 60 + m * 60 + s;
}

void Alarm::UpdateAlarmLevel(tmElements_t now)
{
	if (!this->isOn)
	{
		this->alarmLevel = 0;
		return;
	}

	// Time for alarm?
	bool isAlarmMinute = (now.Hour == this->h && now.Minute == this->m);

	// Convert now/alarm into "seconds since midnight".
	uint32_t time = secondsSinceMidnight(now.Hour, now.Minute, now.Second);
	uint32_t alarm = secondsSinceMidnight(this->h, this->m);

	// Next alarm time is tomorrow.
	if (alarm < time)
		alarm += (uint32_t)60 * 60 * 24;

	// Next alarm in seconds?
	uint32_t nextAlarm = alarm - time;

	uint8_t level = nextAlarm <= ALARM_RAMP_SECONDS
		? map(nextAlarm, 0, ALARM_RAMP_SECONDS, 254, 0)
		: 0;

	// Wake up!
	if (isAlarmMinute)
		level = 255;

	// Reset mute flag if alarm is off.
	if (level == 0)
	{
		this->isMute = false;
		this->alarmLevel = 0;
		return;
	}

	this->alarmLevel = this->isMute ? 0 : level;
}

void Alarm::Set(uint8_t h, uint8_t m, bool on)
{
	this->h = h;
	this->m = m;
	this->isOn = on;
}

void Alarm::AddMinutes(uint8_t minutes)
{
	int8_t hours = this->h;
	int8_t min = this->m;

	min += (minutes);
	while (min >= 60)
	{
		min -= 60;
		hours++;
	}

	while (min < 0)
	{
		min += 60;
		hours--;
	}

	if (hours < 0)
		hours = 23;

	if (hours > 23)
		hours = 0;

	this->h = hours;
	this->m = min;
}

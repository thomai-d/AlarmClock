#ifndef DATETIME_H
#define DATETIME_H

#include <TimeLib.h>
#include <DS3232RTC.h>
#include "UserActivityManager.h"

class DateTimeClass
{
public:
	// Initializes the time from the RTC.
	void Initialize();

	// Gets the local time.
	tmElements_t Now();

	// Synchronizes the current UTC time from serial.
	// Expects:
	// T[UnixTimeStamp]\n
	void SyncFromSerial();

	// Modifies the time and saves the
	// new time in EEPROM.
	void SetTime(unsigned long newtime);

private:

	bool IsDST(tmElements_t time);
};

extern DateTimeClass DateTime;

#endif
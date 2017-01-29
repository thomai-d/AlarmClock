#include "DateTime.h"

void DateTimeClass::Initialize()
{
	setSyncProvider(RTC.get);
}

tmElements_t DateTimeClass::Now()
{
	time_t unixTime = now();

	// Check for daylight saving time.
	tmElements_t time;
	breakTime(unixTime, time);
	if (this->IsDST(time))
	{
		unixTime += 60 * 60; // Add 1 hour.
	}

	// Convert to local time.
	unixTime += 60 * 60; // GMT + 1.
	breakTime(unixTime, time);
	return time;
}

void DateTimeClass::SyncFromSerial()
{
	unsigned long pctime = 0L;
	const unsigned long DEFAULT_TIME = 1477440000; // 2016-10-16

	if (Serial.find("T")) {
		pctime = Serial.parseInt();
		if (pctime > DEFAULT_TIME) 
		{
			UserActivityManager.SetEnabled(false);
			RTC.set(pctime);
			setTime(pctime);
			UserActivityManager.SetEnabled(true);
		}
	}
}

void DateTimeClass::SetTime(unsigned long newtime)
{
	UserActivityManager.SetEnabled(false);
	RTC.set(newtime);
	setTime(newtime);
	UserActivityManager.SetEnabled(true);
}

bool DateTimeClass::IsDST(tmElements_t time)
{
	uint8_t month = time.Month;
	uint8_t day = time.Day;
	uint8_t dow = time.Wday - 1; // Sunday is 1, it needs to be 0 here.
	uint8_t hour = time.Hour;

	// April to September is always DST.
	if (month >= 4 && month <= 9)
		return true;

	// dowDelta >= 25 means 'after last sunday'
	// unless dow == 0 (then, it IS the last sunday).
	bool lastSundayPassed = (day - dow) >= 25;

	// Check March.
	if (month == 3)
	{
		// It is DST if last sunday passed
		// except ON last sunday before 1 o'clock.
		return lastSundayPassed
			&& !(dow == 0 && hour < 1);
	}

	// Check October.
	if (month == 10)
	{
		// It is DST if the last sunday did not pass
		// or ON last sunday before 1 o'clock.
		return !lastSundayPassed
			|| (dow == 0 && hour < 1);
	}

	// The rest is not DST.
	return false;
}

DateTimeClass DateTime;
#ifndef USER_ACTIVITY_MANAGER_H
#define USER_ACTIVITY_MANAGER_H

#include <time.h>

#include "PageManager.h"
#include "AlarmManager.h"

class UserActivityManagerClass
{
public:
	void ReportUserActivity() { this->lastUserEvent = now(); }

	void Service();

	void SetTimeout(uint8_t seconds) { this->timeout = seconds; }

	void SetEnabled(bool e);

private:
	
	// Gets the user inactivity time in seconds.
	uint16_t GetUserInactivityTime();

	time_t	lastUserEvent;

	// User inactivity timeout [s].
	uint8_t timeout = 10;

	volatile bool enabled = true;

};

extern UserActivityManagerClass UserActivityManager;

#endif
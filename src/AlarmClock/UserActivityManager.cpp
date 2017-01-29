#include "UserActivityManager.h"

uint16_t UserActivityManagerClass::GetUserInactivityTime()
{
	uint32_t delta = now() - this->lastUserEvent;
	if (delta > 32000) return 32000;
	return (uint16_t)delta;
}

void UserActivityManagerClass::Service()
{
	if (!this->enabled)
		return;

	if (!Display2.IsBacklightOn()
		|| AlarmManager.IsAlarmActive()
		|| this->GetUserInactivityTime() < this->timeout)
		return;

	PageManager.OnUserInactivity();
}

void UserActivityManagerClass::SetEnabled(bool e)
{
	auto x = SREG;
	cli();
	this->ReportUserActivity(); 
	this->enabled = e;
	SREG = x;
}

UserActivityManagerClass UserActivityManager;
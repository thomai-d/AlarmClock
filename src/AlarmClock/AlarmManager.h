#ifndef ALARMMANAGER_H
#define ALARMMANAGER_H

#include <EEPROM.h>
#include <inttypes.h>

#include "Alarm.h"
#include "Debug.h"
#include "DateTime.h"
#include "OutputHardware.h"
#include "Config.h"

#define MAX_ALARMS			2

extern void ShowMainPage(bool fade = true);
extern void OnAlarmActivated();

class AlarmManagerClass
{
public:
	/// Activates/Deactives the alarm system.
	/// If disabled, all alarms are reset (including mute).
	void SetEnabled(bool enabled);

	// Gets alarm data.
	Alarm GetAlarm(uint8_t idx);

	// Toggles the ON-state of the given alarm.
	void ToggleOn(uint8_t idx);

	// Step can be +1 or -1 !
	void AddOffset(uint8_t idx, int8_t step);

	/// Quits the next alarm (or all, if the alarm level is identical).
	void QuitFirstAlarm();

	// Gets a value whether any alarm is active.
	bool IsAlarmActive();

	// This method must be invoked cyclically.
	// Typically at least once a second.
	void Service();

	// Persists the alarm times to EEPROM.
	void PersistAlarms();

	// Reads the alarms from EEPROM.
	void ReloadAlarms();

	// Resets EEPROM.
	void FactoryReset();

	// Toggles the manual override of the led light which is used to indicate alarm.
	void ToggleLEDOverride();

private:

	Alarm alarm[MAX_ALARMS];

	bool enabled = true;

	// Forces the light to be active.
	bool lightOverride = false;
	bool lastLightOverride = false;

	// Set this flag from an interrupt routine
	// to disable the manager in the next Service() call.
	// (synchronization).
	bool disableRequest;

	// Tracks the last alarm level.
	uint8_t lastAlarmLevel;

};

extern AlarmManagerClass AlarmManager;

#endif
#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <Ucglib.h>

#include "Page.h"
#include "OutputHardware.h"
#include "SensorHardware.h"
#include "UserActivityManager.h"
#include "AlarmManager.h"
#include "Config.h"
#include "DateTime.h"
#include "Display.h"
#include "Icons.h"
#include "Alarm.h"
#include "Debug.h"

enum Mode
{
	Normal,
	EditAlarm1,
	EditAlarm2
};

class MainPage : public Page
{
public:

	void OnViewLoaded() override;
	void OnViewUnLoaded() override;
	void OnUserInactivity() override;
	void Refresh(bool force) override;

	void OnButton0Down() override;
	void OnButton1Down() override;
	void OnButton0Up(uint16_t downTimeMs) override;
	void OnButton1Up(uint16_t downTimeMs) override;
	void OnRotaryChanged(int8_t offset) override;

	bool ForceReceiveButton1() override;

private:

	void RefreshClock(bool force);
	void RefreshAlarms(bool force);

	void SwitchToNormalMode();

	void DrawAlarm(uint8_t idx, bool isEdited, bool editModeChanged, bool force);

	// UI Cache.
	uint8_t h, m, s;
	uint8_t alarmH[MAX_ALARMS];
	uint8_t alarmM[MAX_ALARMS];
	uint8_t alarmOn[MAX_ALARMS];
	bool editModeChanged;

	Mode currentMode;

};

#endif
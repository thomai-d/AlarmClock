#include "MainPage.h"

const uint8_t xpos = 8;
const uint8_t ypos = 25;

void MainPage::OnViewLoaded()
{
	UserActivityManager.SetTimeout(USER_INACTIVITY_TIMEOUT_NORMAL);

	Display.clearScreen();
	Display.setFontPosTop();
	Display.setFontMode(UCG_FONT_MODE_SOLID);

	this->currentMode = Mode::Normal;

	// Time background.
	Display.setFont(FONT_BIG24);
	Display.setColor(255, 255, 255);
	Display.setPrintPos(43, ypos);
	Display.print(":");
	Display.setPrintPos(93, ypos);
	Display.print(":");

	this->Refresh(true);
}

void MainPage::OnViewUnLoaded()
{
	this->SwitchToNormalMode();
}

void MainPage::OnUserInactivity()
{
	this->SwitchToNormalMode();
	this->Refresh(false);
}

void MainPage::OnButton1Down()
{
	switch (this->currentMode)
	{
	case Mode::EditAlarm1:
		AlarmManager.ToggleOn(0);
		break;

	case Mode::EditAlarm2:
		AlarmManager.ToggleOn(1);
		break;
	}
}

void MainPage::OnButton0Down()
{
	switch (this->currentMode)
	{
	case Mode::Normal:
		this->currentMode = Mode::EditAlarm1;
		AlarmManager.SetEnabled(false);
		UserActivityManager.SetTimeout(USER_INACTIVITY_TIMEOUT_INPUT);
		break;

	case Mode::EditAlarm1:
		this->currentMode = Mode::EditAlarm2;
		AlarmManager.SetEnabled(false);
		AlarmManager.PersistAlarms();
		UserActivityManager.SetTimeout(USER_INACTIVITY_TIMEOUT_INPUT);
		break;

	case Mode::EditAlarm2:
		this->SwitchToNormalMode();
		break;
	}

	this->editModeChanged = true;
}

void MainPage::OnButton0Up(uint16_t downTimeMs)
{
}

void MainPage::OnButton1Up(uint16_t downTimeMs)
{
}

void MainPage::OnRotaryChanged(int8_t offset)
{
	switch (this->currentMode)
	{
	case Mode::EditAlarm1:
		if (!AlarmManager.GetAlarm(0).IsOn())
		{
			AlarmManager.ToggleOn(0);
			break;
		}
		AlarmManager.AddOffset(0, offset);
		break;
	case Mode::EditAlarm2:
		if (!AlarmManager.GetAlarm(1).IsOn())
		{
			AlarmManager.ToggleOn(1);
			break;
		}
		AlarmManager.AddOffset(1, offset);
		break;
	}
}

void MainPage::RefreshClock(bool force)
{
	tmElements_t time = DateTime.Now();
	uint8_t h = time.Hour;
	uint8_t m = time.Minute;
	uint8_t s = time.Second;
	Display.setFont(FONT_BIG24);

	if (h != this->h || force)
	{ 
		this->h = h;
		Display2.Print0(xpos, ypos, h, 255);
	}

	if (m != this->m || force)
	{ 
		this->m = m;
		Display2.Print0(xpos + 50, ypos, m, 255);
	}

	if (s != this->s || force)
	{ 
		this->s = s;
		Display2.Print0(xpos + 100, ypos, s, 255);
	}
}

void MainPage::DrawAlarm(uint8_t idx, bool isEdited, bool editModeChanged, bool force)
{
	const uint8_t alarm_x = 65;
	const uint8_t icon_x = 42;
	const uint8_t alarmY[MAX_ALARMS] = { 70, 92 };
	const uint8_t colorR[MAX_ALARMS] = { 0xff, 0x00 };
	const uint8_t colorB[MAX_ALARMS] = { 0x00, 0xff };

	if (editModeChanged || force)
	{
		if (isEdited)
			Icons::DrawEditIcon(icon_x, alarmY[idx], colorR[idx], 0x00, colorB[idx]);
		else
			Icons::DrawAlarmClock(icon_x, alarmY[idx], colorR[idx], 0x00, colorB[idx]);
	}

	Alarm alarm = AlarmManager.GetAlarm(idx);

	Display.setColor(200, 200, 200);
	Display.setFont(FONT_MED14);

	if (alarm.IsOn())
	{
		// Invalidate OFF text.
		this->alarmOn[idx] = true;

		if (alarm.H() != this->alarmH[idx] || force)
		{
			this->alarmH[idx] = alarm.H();
			Display2.Print0(alarm_x, alarmY[idx], alarm.H(), 255);
			Display.print(":");
		}

		if (alarm.M() != this->alarmM[idx] || force)
		{
			this->alarmM[idx] = alarm.M();
			Display2.Print0(alarm_x + 24, alarmY[idx], alarm.M(), 255);
		}
	}
	else
	{
		// Invalidate HH:MM.
		this->alarmH[idx] = 255;
		this->alarmM[idx] = 255;

		if (alarm.IsOn() != this->alarmOn[idx] || force)
		{
			this->alarmOn[idx] = alarm.IsOn();
			Display.setColor(100, 100, 100);
			Display.setPrintPos(alarm_x, alarmY[idx]);
			Display.print("OFF   ");
		}
	}
}

void MainPage::RefreshAlarms(bool force)
{
	cli();
	bool editModeChanged = this->editModeChanged;
	this->editModeChanged = false;
	sei();
	this->DrawAlarm(0, this->currentMode == Mode::EditAlarm1, editModeChanged, force);
	this->DrawAlarm(1, this->currentMode == Mode::EditAlarm2, editModeChanged, force);
}

void MainPage::SwitchToNormalMode()
{
	// Activate alarm manager again when
	// page is switched in edit mode.
	if (this->currentMode != Mode::Normal)
	{
		AlarmManager.SetEnabled(true);
		AlarmManager.PersistAlarms();
		UserActivityManager.SetTimeout(USER_INACTIVITY_TIMEOUT_NORMAL);

		this->currentMode = Mode::Normal;
		this->editModeChanged = true;
	}
}

void MainPage::Refresh(bool force)
{
	this->RefreshClock(force);
	this->RefreshAlarms(force);
}
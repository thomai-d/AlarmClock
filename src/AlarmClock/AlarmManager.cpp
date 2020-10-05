#include "AlarmManager.h"

void AlarmManagerClass::SetEnabled(bool enabled)
{
	if (enabled)
		this->enabled = true;

	this->disableRequest = !enabled;
}

Alarm AlarmManagerClass::GetAlarm(uint8_t idx)
{
	cli();
	Alarm temp = this->alarm[idx];
	sei();
	return temp;
}

void AlarmManagerClass::ToggleLEDOverride()
{
	this->lightOverride = !this->lightOverride;
	Serial.println(this->lightOverride);
}

void AlarmManagerClass::ToggleOn(uint8_t idx)
{
	this->alarm[idx].ToggleOn();
}

void AlarmManagerClass::AddOffset(uint8_t idx, int8_t step)
{
	if (this->alarm[idx].IsOn())
		this->alarm[idx].AddMinutes(step * ALARM_STEPS_MINUTES);
}

void AlarmManagerClass::QuitFirstAlarm()
{
	uint8_t alarmLevelToQuit = max(this->alarm[0].GetAlarmLevel(), this->alarm[1].GetAlarmLevel());

	// Nothing to quit.
	if (alarmLevelToQuit == 0)
		return;

	for (uint8_t id = 0; id < MAX_ALARMS; id++)
	{
		if (this->alarm[id].GetAlarmLevel() == alarmLevelToQuit)
		{
			this->alarm[id].Mute();
		}
	}
}

bool AlarmManagerClass::IsAlarmActive()
{
	return this->lastAlarmLevel > 0;
}

void AlarmManagerClass::Service()
{
	if (this->disableRequest)
	{
		this->alarm[0].UnMute();
		this->alarm[1].UnMute();
		this->lastAlarmLevel = 0;
		OutputHardware.SetLED(this->lightOverride ? LED_ON_INTENSITY : 0);
		OutputHardware.SetAlarm(false);
		this->disableRequest = false;
		this->enabled = false;
	}

	if (this->lightOverride != this->lastLightOverride)
	{
		OutputHardware.SetLED(this->lightOverride ? LED_ON_INTENSITY : 0);
		this->lastLightOverride = this->lightOverride;
	}

	if (!this->enabled)
		return;

	for (int id = 0; id < MAX_ALARMS; id++)
	{
		// Query time.
		tmElements_t time = DateTime.Now();
		this->alarm[id].UpdateAlarmLevel(time);
	}

	uint8_t maxLevel = max(this->alarm[0].GetAlarmLevel(), this->alarm[1].GetAlarmLevel());
	if (maxLevel == this->lastAlarmLevel)
		return;

	if (maxLevel > 0 && this->lastAlarmLevel == 0)
	{
		OnAlarmActivated();
	}

	OutputHardware.SetAlarm(maxLevel == 255);
	this->lastAlarmLevel = maxLevel;

	// Set led light level.
	uint8_t ledLevel = powf((float)maxLevel / 255.0f, 3) * 255.0f;
	if (maxLevel > 0 && ledLevel == 0) ledLevel = 1;
	OutputHardware.SetLED(this->lightOverride ? LED_ON_INTENSITY : ledLevel);
}

void AlarmManagerClass::PersistAlarms()
{
	EEPROM.write(EEPROM_ALARM0_H, this->alarm[0].H());
	EEPROM.write(EEPROM_ALARM0_M, this->alarm[0].M());
	EEPROM.write(EEPROM_ALARM0_ON, this->alarm[0].IsOn());
	EEPROM.write(EEPROM_ALARM1_H, this->alarm[1].H());
	EEPROM.write(EEPROM_ALARM1_M, this->alarm[1].M());
	EEPROM.write(EEPROM_ALARM1_ON, this->alarm[1].IsOn());
}

void AlarmManagerClass::ReloadAlarms()
{
	this->alarm[0].Set(EEPROM.read(EEPROM_ALARM0_H), EEPROM.read(EEPROM_ALARM0_M), EEPROM.read(EEPROM_ALARM0_ON));
	this->alarm[1].Set(EEPROM.read(EEPROM_ALARM1_H), EEPROM.read(EEPROM_ALARM1_M), EEPROM.read(EEPROM_ALARM1_ON));
}

void AlarmManagerClass::FactoryReset()
{
	this->alarm[0].Set(0, 0, false);
	this->alarm[1].Set(0, 0, false);
	this->PersistAlarms();
}

AlarmManagerClass AlarmManager;
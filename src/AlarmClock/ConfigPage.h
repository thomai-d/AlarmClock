#ifndef CONFIG_PAGE_H
#define CONFIG_PAGE_H

#include "Display.h"
#include "DateTime.h"
#include "UserActivityManager.h"
#include "AlarmManager.h"
#include "Config.h"

enum ConfigMode
{
	DisplayOnly,
	Seconds,
	Minutes,
	Hours,
	Days,
	Month,
	Year,
	Hue,
	Saturation
};

class ConfigPage : public Page
{
public:
	void OnViewLoaded() override;
	void OnViewUnLoaded() override;
	void Refresh(bool force) override;

	void OnButton0Down() override;
	void OnButton1Down() override;
	void OnRotaryChanged(int8_t offset) override;

private:
	int offset;

	void Print0(uint8_t value, bool highlight);
	void ProcessInput();

	void SetColor(bool highlight);

	ConfigMode mode = ConfigMode::DisplayOnly;
};

#endif
#include "ConfigPage.h"

void ConfigPage::OnViewLoaded()
{
	AlarmManager.SetEnabled(false);

	Display.clearScreen();
	Display.setFontMode(UCG_FONT_MODE_SOLID);

	this->mode = ConfigMode::DisplayOnly;

	UserActivityManager.SetTimeout(USER_INACTIVITY_TIMEOUT_SETTINGS);

	Display.setFont(FONT_MED14);
	Display.setPrintPos(5, 25);
	Display.print(F("Einstellungen"));

	this->Refresh(true);
}

void ConfigPage::OnViewUnLoaded()
{
	OutputHardware.SetLED(0);
	OutputHardware.PersistColor();
	AlarmManager.SetEnabled(true);
}

void ConfigPage::Refresh(bool force)
{
	this->ProcessInput();

	// LED color.
	Display.setPrintPos(10, 50);
	this->SetColor(false);
	Display.print(F("LED: "));

	hsv color = OutputHardware.GetColor();
	this->SetColor(this->mode == ConfigMode::Saturation);
	Display.print(color.s, 1);
	this->SetColor(false);
	Display.print(F("/"));
	this->SetColor(this->mode == ConfigMode::Hue);
	Display.print(color.h, 0);
	Display.print(F("   "));

	// Time.
	auto now = DateTime.Now();
	Display.setPrintPos(10, 75);
	this->Print0(now.Hour, this->mode == ConfigMode::Hours);
	Display.print(':');
	this->Print0(now.Minute, this->mode == ConfigMode::Minutes);
	Display.print(':');
	this->Print0(now.Second, this->mode == ConfigMode::Seconds);

	// Date.
	Display.setPrintPos(10, 100);
	this->Print0(now.Day, this->mode == ConfigMode::Days);
	Display.print('.');
	this->Print0(now.Month, this->mode == ConfigMode::Month);
	Display.print('.');

	this->SetColor(this->mode == ConfigMode::Year);
	Display.print((int)1970 + now.Year);
}

void ConfigPage::OnButton0Down()
{
	switch (this->mode)
	{
	case ConfigMode::DisplayOnly:
		OutputHardware.SetLED(255);
		this->mode = ConfigMode::Saturation;
		break;
	case ConfigMode::Saturation:
		this->mode = ConfigMode::Hue;
		break;
	case ConfigMode::Hue:
		OutputHardware.SetLED(0);
		this->mode = ConfigMode::Hours;
		break;
	case ConfigMode::Hours:
		this->mode = ConfigMode::Minutes;
		break;
	case ConfigMode::Minutes:
		this->mode = ConfigMode::Seconds;
		break;
	case ConfigMode::Seconds:
		this->mode = ConfigMode::Days;
		break;
	case ConfigMode::Days:
		this->mode = ConfigMode::Month;
		break;
	case ConfigMode::Month:
		this->mode = ConfigMode::Year;
		break;
	case ConfigMode::Year:
		this->mode = ConfigMode::DisplayOnly;
		break;
	}
}

void ConfigPage::OnButton1Down()
{
}

void ConfigPage::OnRotaryChanged(int8_t offset)
{
	this->offset += offset;
}

void ConfigPage::Print0(uint8_t value, bool highlight)
{
	if (highlight)
		Display.setColor(255, 100, 100);
	else
		Display.setColor(200, 200, 200);
	Display2.Print0(value);
}

void ConfigPage::ProcessInput()
{
	int offset;
	cli();
	offset = this->offset;
	this->offset = 0;
	sei();

	if (offset == 0)
		return;

	hsv color;
	time_t newTime = now();
	tmElements_t te;
	int month;

	switch (this->mode)
	{
	case ConfigMode::Seconds:
		newTime += offset;
		break;
	case ConfigMode::Minutes:
		newTime += offset * 60;
		break;
	case ConfigMode::Hours:
		newTime += offset * 60 * 60;
		break;
	case ConfigMode::Days:
		newTime += (time_t)offset * 60 * 60 * 24;
		break;
	case ConfigMode::Month:
		breakTime(newTime, te);
		month = (int)te.Month + offset;
		while (month > 12) { month-=12; te.Year++; }
		while (month < 1) { month+=12; te.Year--; }
		te.Month = (uint8_t)month;
		newTime = makeTime(te);
		break;
	case ConfigMode::Year:
		breakTime(newTime, te);
		te.Year += offset;
		newTime = makeTime(te);
		break;
	case ConfigMode::Hue:
		color = OutputHardware.GetColor();
		color.h = min(max(0, (color.h + offset * 10)), 360);
		OutputHardware.SetColor(color);
		break;
	case ConfigMode::Saturation:
		color = OutputHardware.GetColor();
		color.s = min(max(0, (color.s + (float)offset / 10)), 1.0);
		OutputHardware.SetColor(color);
		break;

	}

	DateTime.SetTime(newTime);
}

void ConfigPage::SetColor(bool highlight)
{
	if (highlight)
		Display.setColor(255, 100, 100);
	else
		Display.setColor(200, 200, 200);
}

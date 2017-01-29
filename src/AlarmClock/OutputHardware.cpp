#include "OutputHardware.h"

OutputHardwareClass::OutputHardwareClass()
{
	// Set output pin modes.
	pinMode(DO_LED_R, OUTPUT);
	pinMode(DO_LED_G, OUTPUT);
	pinMode(DO_LED_B, OUTPUT);
	pinMode(DO_BEEP, OUTPUT);
	pinMode(DO_DISPLAY_BL, OUTPUT);

	this->color.h = EEPROM.read(EEPROM_HUE) * 10;
	this->color.s = EEPROM.read(EEPROM_SAT) / 10;
}

void OutputHardwareClass::AlarmTest()
{
	for (int n = 0; n < 3; n++)
	{
		digitalWrite(DO_BEEP, true);
		delay(3);
		digitalWrite(DO_BEEP, false);
		delay(100);
	}
}

void OutputHardwareClass::SetAlarm(bool state)
{
	auto sreg = SREG;
	cli();

	if (state)
	{
		this->alarmTickCounter = 0;
		this->alarmTickMax = ALARM_TICK_MAX;
		this->alarmEnabled = true;
	}
	else
	{
		this->alarmEnabled = false;
		digitalWrite(DO_BEEP, false);
	}

	SREG = sreg;
}

void OutputHardwareClass::SetLED(uint8_t intensity)
{
	this->intensity = intensity;

	this->color.v = (float)intensity / 255.0f;

	rgb col = hsv2rgb(this->color);

	analogWrite(DO_LED_R, (uint8_t)(col.r * 255));
	analogWrite(DO_LED_G, (uint8_t)(col.g * 255));
	analogWrite(DO_LED_B, (uint8_t)(col.b * 255));
}

void OutputHardwareClass::OnTimer2Tick()
{
	if (!this->alarmEnabled)
		return;

	digitalWrite(DO_BEEP, this->alarmTickCounter < 2);

	this->alarmTickCounter++;

	if (this->alarmTickCounter > this->alarmTickMax)
	{
		this->alarmTickCounter = 0;
		this->alarmTickMax -= this->alarmTickMax / 12;
	}
}

void OutputHardwareClass::SetColor(hsv color)
{
	this->color = color;
	if (this->intensity > 0)
		this->SetLED(this->intensity);
}

void OutputHardwareClass::PersistColor()
{
	EEPROM.write(EEPROM_HUE, this->color.h / 10);
	EEPROM.write(EEPROM_SAT, this->color.s * 10);
}

OutputHardwareClass OutputHardware;

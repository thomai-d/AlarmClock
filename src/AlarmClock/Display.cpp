#include "Display.h"

DisplayClass Display2;

Ucglib_ST7735_18x128x160_HWSPI Display(DO_DISPLAY_A0, DO_DISPLAY_CS, DO_DISPLAY_RST);

void DisplayClass::Initialize()
{
	Display.begin(UCG_FONT_MODE_SOLID);
	Display.setRotate90();
	Display.setColor(0, 255, 255, 255);
	Display.clearScreen();
}

void DisplayClass::Print0(uint8_t x, uint8_t y, uint8_t value, uint8_t grayValue)
{
	Display.setPrintPos(x, y);
	Display.setColor(grayValue, grayValue, grayValue);
	this->Print0(value);
}

void DisplayClass::Print0(uint8_t value)
{
	if (value < 10)
		Display.print('0');
	Display.print(value);
}

void DisplayClass::ActivateBacklight()
{
	auto sreg = SREG;
	cli();

	this->isBacklightOn = true;
	this->isAutoLevelEnabled = false;
	this->targetLevel = this->GetAutoLevel();

	SREG = sreg;
}

void DisplayClass::DeactivateBacklight()
{
	auto sreg = SREG;
	cli();

	this->isBacklightOn = false;
	this->isAutoLevelEnabled = false;
	this->targetLevel = 0;

	SREG = sreg;
}

void DisplayClass::OnTimer2Tick()
{
	if (this->isAutoLevelEnabled)
	{
		this->targetLevel = this->GetAutoLevel();
	}
	else
	{
		if ((uint8_t)this->currentLevel == this->targetLevel)
			return;
	}

	// Light averaging.
	this->lightSamples[this->lightSample] = InputHardware.ReadBrightness();
	if (++this->lightSample >= LIGHT_SAMPLES)
		this->lightSample = 0;

	float delta = (float)this->targetLevel - this->currentLevel;
	if (abs(delta) > 1.0)
	{
		this->currentLevel += (this->isAutoLevelEnabled ? (delta/8.0f) : min(delta,10.0f));
		if (this->currentLevel < 0)
			this->currentLevel = 0;
		if (this->currentLevel > BACKLIGHT_MAX_LEVEL)
			this->currentLevel = BACKLIGHT_MAX_LEVEL;
	}

	if (abs(this->targetLevel - this->currentLevel) < 1.0)
	{
		// Enable auto leveling.
		this->currentLevel = this->targetLevel;
		this->isAutoLevelEnabled = this->isBacklightOn;
	}

	analogWrite(DO_DISPLAY_BL, (uint8_t)this->currentLevel);
}

uint8_t DisplayClass::GetAutoLevel()
{
	int sum = 0;
	for (uint8_t n = 0; n < LIGHT_SAMPLES; n++)
		sum += this->lightSamples[n];

	int brightness = sum / LIGHT_SAMPLES;

	uint8_t level = 255 - brightness;
	return min(max(BACKLIGHT_MIN_LEVEL, level), BACKLIGHT_MAX_LEVEL);
}

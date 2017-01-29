#include "TrendPage.h"

#define TREND_OFFSET_X	5	

void TrendPage::OnViewLoaded()
{
	UserActivityManager.SetTimeout(USER_INACTIVITY_TIMEOUT_TREND);

	Display.clearScreen();
	Display.setFontMode(UCG_FONT_MODE_SOLID);

	this->Refresh(true);
}

void TrendPage::OnViewUnLoaded()
{
}

void TrendPage::Refresh(bool force)
{
	if (this->lastTrendPos == TrendData.currentItem && !force)
		return;

	Display2.DeactivateBacklight();

	Display.clearScreen();
	
	// Draw gridlines.
	Display.setFont(FONT_TINY4);
	Display.setFontPosBottom();
	for (int n = TREND_TEMP_MIN + TREND_TEMP_STEP; n <= TREND_TEMP_MAX - TREND_TEMP_STEP; n += TREND_TEMP_STEP)
	{
		Display.setColor(40, 40, 40);
		uint8_t y = TrendData.TemperatureToY(n);
		Display.setPrintPos(0, y + 1);
		Display.print(n);

		Display.setColor(20, 20, 20);
		Display.drawHLine(0, y, TREND_MAX_ITEMS + TREND_OFFSET_X);
	}
	for (int n = TREND_PRESSURE_MIN + TREND_PRESSURE_STEP; n <= TREND_PRESSURE_MAX - TREND_PRESSURE_STEP; n += TREND_PRESSURE_STEP)
	{
		Display.setColor(40, 40, 40);
		uint8_t y = TrendData.PressureToY(n) + 64;
		Display.setPrintPos(0, y + 1);
		Display.print(n);

		Display.setColor(20, 20, 20);
		Display.drawHLine(0, y, TREND_MAX_ITEMS + TREND_OFFSET_X);
	}

	// Draw temperature.
	Display.setColor(255, 0, 0);
	this->Draw(TrendData.tempData, 0);

	// Setup font for current values.
	Display.setFont(FONT_MED14);
	Display.setFontPosCenter();

	// Draw current temperature.
	float currentTemp = SensorHardware.GetTemperature();
	uint8_t currentTempY = min(max(TrendData.TemperatureToY(currentTemp), 14), 50);
	Display.setPrintPos(TREND_MAX_ITEMS + 10, currentTempY + CURRENTVALUE_YOFFSET);
	Display.print(currentTemp, 1);

	// Separator
	Display.setColor(40, 40, 40);
	Display.drawHLine(0, 63, Display.getWidth());
	Display.drawHLine(0, 64, Display.getWidth());

	// Draw pressure.
	Display.setColor(0, 0, 255);
	this->Draw(TrendData.pressureData, 64);

	// Draw current pressure.
	float currentPressure = SensorHardware.GetPressure();
	uint8_t currentPressureY = min(max(TrendData.PressureToY(currentPressure), 14), 50);
	Display.setPrintPos(TREND_MAX_ITEMS + 10, currentPressureY + 64 + CURRENTVALUE_YOFFSET);
	Display.print(currentPressure, 1);

	this->lastTrendPos = TrendData.currentItem;

	Display2.ActivateBacklight();
}

void TrendPage::Draw(uint8_t* data, uint8_t yoffset)
{
	uint8_t pos = TrendData.currentItem;
	uint8_t lastY;

	for (int x = TREND_MAX_ITEMS + TREND_OFFSET_X - 1; x >= TREND_OFFSET_X; x--)
	{
		auto y = data[pos] + yoffset;

		if (x < TREND_MAX_ITEMS + TREND_OFFSET_X - 1)
		{
			Display.drawLine(x, y, x + 1, lastY);
		}

		lastY = y;

		if (pos == 0)
			pos = TREND_MAX_ITEMS - 1;
		else
			pos--;
	}
}

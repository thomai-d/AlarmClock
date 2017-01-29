#ifndef TrendPage_H
#define TrendPage_H

#include "TrendData.h"
#include "SensorHardware.h"
#include "UserActivityManager.h"
#include "OutputHardware.h"
#include "Config.h"
#include "Debug.h"
#include "Display.h"
#include "Page.h"

#define CURRENTVALUE_YOFFSET	1		// Y-Offset added to the current value text.

class TrendPage : public Page
{
public:
	void OnViewLoaded() override;
	void OnViewUnLoaded() override;
	void Refresh(bool force) override;

private:

	void Draw(uint8_t* data, uint8_t yoffset);

	uint8_t lastTrendPos;

};

#endif

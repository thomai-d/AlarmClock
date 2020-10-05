#include "InputHardware.h"

const uint8_t InputHardwareClass::lookupTable[7][4] = {
	{ R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START },
	{ R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW },
	{ R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START },
	{ R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START },
	{ R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START },
	{ R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW },
	{ R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START }
};

InputHardwareClass::InputHardwareClass()
{
	cli();

	// Setup pins.
	DDRC &= !(_BV(DDC0) | _BV(DDC1) | _BV(DDC2) | _BV(DDC3));
	PORTC |= _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	pinMode(DI_BUTTON2, INPUT_PULLUP);

	// Setup PCINT.
	PCICR |= _BV(PCIE1);		// Enable PCINT1
	PCIFR |= _BV(PCIF1);		// Clear pending PCINT1
	PCMSK1 |= _BV(PCINT8)		// Enable pins.
		| _BV(PCINT9)
		| _BV(PCINT10)
		| _BV(PCINT11);

	// Setup INT0.
	bitClear(EICRA, ISC00);		// Configure INT0 on
	bitSet(EICRA, ISC01);		// Falling edge.
	bitSet(EIMSK, INT0);		// Enable interrupt.

	sei();
}

void InputHardwareClass::Initialize()
{
	UserActivityManager.ReportUserActivity();
}

void InputHardwareClass::OnINT0()
{
	cli();

	if (this->blockModeButton)
		return;

	uint32_t timeMs = millis();
	uint32_t delta = timeMs - b2_lastEvent;
	if (delta > BUTTON_DEBOUNCE_MS)
	{
		OnButton2Down();

		this->b2_lastEvent = timeMs;
		UserActivityManager.ReportUserActivity();
	}

	sei();
}

uint8_t InputHardwareClass::ReadBrightness()
{
#ifdef NO_LDR
	return 240;
#else
	return analogRead(AI_LDR) >> 2;
#endif
}

void InputHardwareClass::OnPCINT1()
{
	cli();
	
	uint8_t pinc = PINC;

	// Check buttons.
	bool b0wasUp = bitRead(button_states, BUTTON0_BIT);
	bool b1wasUp = bitRead(button_states, BUTTON1_BIT);
	bool b0isUp = bitRead(pinc, BUTTON0_BIT);
	bool b1isUp = bitRead(pinc, BUTTON1_BIT);

	bool b0Changed = b0wasUp != b0isUp;
	bool b1Changed = b1wasUp != b1isUp;
	if (b0Changed || b1Changed)
	{
		uint32_t timeMs = millis();
		UserActivityManager.ReportUserActivity();

		if (b0Changed)
		{
			uint32_t delta = timeMs - b0_lastEvent;
			if (delta > BUTTON_DEBOUNCE_MS)
			{
				if (Focus && !blockInput)
				{
					if (b0isUp)
					{
						Focus->OnButton0Up(delta);
					}
					else
					{
						Focus->OnButton0Down();
					}
				}

				b0_lastEvent = timeMs;
				bitWrite(button_states, BUTTON0_BIT, b0isUp);
			}
		}

		if (b1Changed)
		{
			uint32_t delta = timeMs - b1_lastEvent;
			if (delta > BUTTON_DEBOUNCE_MS)
			{
				if (Focus && (!blockInput || Focus->ForceReceiveButton1()))
				{
					if (b1isUp)
					{
						Focus->OnButton1Up(delta);
					}
					else
					{
						Focus->OnButton1Down();
					}
				}

				b1_lastEvent = timeMs;
				bitWrite(button_states, BUTTON1_BIT, b1isUp);
			}
		}

		sei();
	}

	
	// Check rotary.
	rotary_state = lookupTable[rotary_state & 0xf][(pinc >> 1) & 3];

	if (Focus && !blockInput)
	{
		UserActivityManager.ReportUserActivity();

		switch (rotary_state & 0x30)
		{
		case DIR_CW:
#ifdef ALTERNATIVE_ROTARY
			Focus->OnRotaryChanged(-1);
#else
			Focus->OnRotaryChanged(+1);
#endif
			break;

		case DIR_CCW:
#ifdef ALTERNATIVE_ROTARY
			Focus->OnRotaryChanged(+1);
#else
			Focus->OnRotaryChanged(-1);
#endif
			break;
		}
	}
}

ISR(PCINT1_vect)
{
	cli();
	InputHardware.OnPCINT1();
	sei();
}

ISR(INT0_vect)
{
	cli();
	InputHardware.OnINT0();
	sei();
}

InputHardwareClass InputHardware;
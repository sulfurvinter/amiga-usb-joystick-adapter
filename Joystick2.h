// 2026-03-23 — Copied verbatim from https://github.com/ytmytm/classic-joystick by Claude
/*
  Joystick2.h
  Copyright (c) 2015, Matthew Heironimus
  LGPL 2.1
*/

#ifndef JOYSTICK_h
#define JOYSTICK_h

#include "HID.h"

#if !defined(_USING_HID)
#warning "Using legacy HID core (non pluggable)"
#else

class Joystick_
{
private:
	uint8_t  joystickId;
	bool     autoSendState;
	int8_t   xAxis;
	int8_t   yAxis;
	uint16_t buttons;

public:
	Joystick_(uint8_t initJoystickId);

	void begin(bool initAutoSendState = true);
	void end();

	void setXAxis(int8_t value);
	void setYAxis(int8_t value);

	void setButton(uint8_t button, uint8_t value);
	void pressButton(uint8_t button);
	void releaseButton(uint8_t button);

	void sendState();
};

#endif
#endif

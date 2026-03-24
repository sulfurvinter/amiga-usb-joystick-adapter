// 2026-03-24 — Modified by Claude: joystick 2 axes changed to Rx/Ry so Linux exposes them
//              as ABS_RX/ABS_RY instead of duplicating ABS_X/ABS_Y, enabling both joysticks
//              to appear as distinct axes on the same HID device.
// 2026-03-23 — Copied verbatim from https://github.com/ytmytm/classic-joystick by Claude
/*
  Joystick2.cpp
  Copyright (c) 2015, Matthew Heironimus
  LGPL 2.1
*/

#include "Joystick2.h"

#if defined(_USING_HID)

#define JOYSTICK_REPORT_ID  0x03
#define JOYSTICK2_REPORT_ID 0x04
#define JOYSTICK_STATE_SIZE 4

static const uint8_t _hidReportDescriptor[] PROGMEM = {

    // Joystick #1
    0x05, 0x01,               // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,               // USAGE (Joystick)
    0xa1, 0x01,               // COLLECTION (Application)
    0x85, JOYSTICK_REPORT_ID, // REPORT_ID (3)
    // 1 button
    0x05, 0x09,               //   USAGE_PAGE (Button)
    0x19, 0x01,               //   USAGE_MINIMUM (Button 1)
    0x29, 0x01,               //   USAGE_MAXIMUM (Button 1)
    0x15, 0x00,               //   LOGICAL_MINIMUM (0)
    0x25, 0x01,               //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,               //   REPORT_SIZE (1)
    0x95, 0x10,               //   REPORT_COUNT (16)
    0x55, 0x00,               //   UNIT_EXPONENT (0)
    0x65, 0x00,               //   UNIT (None)
    0x81, 0x02,               //   INPUT (Data,Var,Abs)
    // X and Y Axis
    0x05, 0x01,               //   USAGE_PAGE (Generic Desktop)
    0x09, 0x01,               //   USAGE (Pointer)
    0x15, 0x81,               //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,               //   LOGICAL_MAXIMUM (127)
    0xA1, 0x00,               //   COLLECTION (Physical)
    0x09, 0x30,               //     USAGE (x)
    0x09, 0x31,               //     USAGE (y)
    0x75, 0x08,               //     REPORT_SIZE (8)
    0x95, 0x02,               //     REPORT_COUNT (2)
    0x81, 0x02,               //     INPUT (Data,Var,Abs)
    0xc0,                     //   END_COLLECTION
    0xc0,                     // END_COLLECTION

    // Joystick #2 — uses Rx/Ry axes so Linux maps to ABS_RX/ABS_RY (not ABS_X/ABS_Y)
    0x05, 0x01,                // USAGE_PAGE (Generic Desktop)
    0x09, 0x04,                // USAGE (Joystick)
    0xa1, 0x01,                // COLLECTION (Application)
    0x85, JOYSTICK2_REPORT_ID, // REPORT_ID (4)
    // 1 button
    0x05, 0x09,                //   USAGE_PAGE (Button)
    0x19, 0x01,                //   USAGE_MINIMUM (Button 1)
    0x29, 0x01,                //   USAGE_MAXIMUM (Button 1)
    0x15, 0x00,                //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                //   REPORT_SIZE (1)
    0x95, 0x10,                //   REPORT_COUNT (16)
    0x55, 0x00,                //   UNIT_EXPONENT (0)
    0x65, 0x00,                //   UNIT (None)
    0x81, 0x02,                //   INPUT (Data,Var,Abs)
    // Rx and Ry axes (distinct from joystick 1's X/Y)
    0x05, 0x01,                //   USAGE_PAGE (Generic Desktop)
    0x15, 0x81,                //   LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                //   LOGICAL_MAXIMUM (127)
    0x09, 0x33,                //   USAGE (Rx)
    0x09, 0x34,                //   USAGE (Ry)
    0x75, 0x08,                //   REPORT_SIZE (8)
    0x95, 0x02,                //   REPORT_COUNT (2)
    0x81, 0x02,                //   INPUT (Data,Var,Abs)
    0xc0                       // END_COLLECTION
};

Joystick_::Joystick_(uint8_t initJoystickId)
{
    static bool usbSetup = false;
    if (!usbSetup) {
        static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
        HID().AppendDescriptor(&node);
        usbSetup = true;
    }
    joystickId = initJoystickId;
    xAxis   = 0;
    yAxis   = 0;
    buttons = 0;
}

void Joystick_::begin(bool initAutoSendState)
{
    autoSendState = initAutoSendState;
    sendState();
}

void Joystick_::end() {}

void Joystick_::setButton(uint8_t button, uint8_t value)
{
    if (value == 0) releaseButton(button);
    else            pressButton(button);
}

void Joystick_::pressButton(uint8_t button)
{
    bitSet(buttons, button);
    if (autoSendState) sendState();
}

void Joystick_::releaseButton(uint8_t button)
{
    bitClear(buttons, button);
    if (autoSendState) sendState();
}

void Joystick_::setXAxis(int8_t value)
{
    xAxis = value;
    if (autoSendState) sendState();
}

void Joystick_::setYAxis(int8_t value)
{
    yAxis = value;
    if (autoSendState) sendState();
}

void Joystick_::sendState()
{
    int8_t data[JOYSTICK_STATE_SIZE];
    uint16_t buttonTmp = buttons;
    data[0] = buttonTmp & 0xFF;
    buttonTmp >>= 8;
    data[1] = buttonTmp & 0xFF;
    data[2] = xAxis;
    data[3] = yAxis;
    HID().SendReport(JOYSTICK_REPORT_ID + joystickId, data, JOYSTICK_STATE_SIZE);
}

#endif

// 2026-03-24 — Updated: swapped JOY1_UP/DOWN to fix Y axis direction, by Claude
// 2026-03-23 — Amiga dual joystick USB adapter, by Claude
// Based on https://github.com/ytmytm/classic-joystick by Maciej Witkowiak
// Adapted pin mapping to project pinout; keyboard emulation removed — pure HID joystick only
// Target: Arduino Micro (ATmega32U4)

#include "Joystick2.h"

// --- Wiring: Arduino Micro pin -> DB9 pin -> function ---
// Arduino pin    DB9 pin    Function
// 5              1          Joystick 1 Up
// 6              2          Joystick 1 Down
// 4              3          Joystick 1 Left
// 3              4          Joystick 1 Right
// -              5          Joystick 1 POTY (not connected)
// 2              6          Joystick 1 Fire
// VCC            7          Joystick 1+2 +5V
// GND            8          Joystick 1+2 GND
// -              9          Joystick 1 POTX (not connected)
//
// A0             1          Joystick 2 Up
// 15             2          Joystick 2 Down
// A1             3          Joystick 2 Left
// A2             4          Joystick 2 Right
// -              5          Joystick 2 POTY (not connected)
// A3             6          Joystick 2 Fire
// -              9          Joystick 2 POTX (not connected)
// -------------------------------------------------------

// --- Pin definitions ---
#define JOY1_UP    5
#define JOY1_DOWN  6
#define JOY1_LEFT  4
#define JOY1_RIGHT 3
#define JOY1_FIRE  2

#define JOY2_UP    A0
#define JOY2_DOWN  15
#define JOY2_LEFT  A1
#define JOY2_RIGHT A2
#define JOY2_FIRE  A3
// -----------------------

class JoyAxis {
  public:
    JoyAxis(const uint8_t pinup, const uint8_t pindown) :
      m_pinup(pinup), m_pindown(pindown) {
        pinMode(m_pinup, INPUT_PULLUP);
        pinMode(m_pindown, INPUT_PULLUP);
      }
    bool update() {
      int8_t state = 0;
      if (!digitalRead(m_pinup))   state = -127;
      if (!digitalRead(m_pindown)) state =  127;
      if (state != m_state) {
        m_state = state;
        return true;
      }
      return false;
    }
    int8_t getState() const { return m_state; }
  private:
    uint8_t m_pinup, m_pindown;
    int8_t  m_state {0};
};

class JoyTrigger {
  public:
    JoyTrigger(const uint8_t pin) : m_pin(pin) {
      pinMode(pin, INPUT_PULLUP);
    }
    bool update() {
      uint8_t state = digitalRead(m_pin) ? 0 : 1;
      if (state != m_state) {
        m_state = state;
        return true;
      }
      return false;
    }
    uint8_t getState() const { return m_state; }
  private:
    uint8_t m_pin, m_state{0};
};

class DigitalJoystick {
  public:
    DigitalJoystick(const Joystick_ usbJoystick,
                    const JoyAxis yAxis, const JoyAxis xAxis,
                    const JoyTrigger trigger) :
      m_usb(usbJoystick), m_y(yAxis), m_x(xAxis), m_btn(trigger)
    {
      m_usb.begin();
    }
    void update() {
      if (m_btn.update()) m_usb.setButton(0, m_btn.getState());
      if (m_x.update())   m_usb.setXAxis(m_x.getState());
      if (m_y.update())   m_usb.setYAxis(m_y.getState());
    }
  private:
    Joystick_  m_usb;
    JoyTrigger m_btn;
    JoyAxis    m_x, m_y;
};

DigitalJoystick digiJoysticks[] = {
  DigitalJoystick(Joystick_(0), JoyAxis(JOY1_UP, JOY1_DOWN), JoyAxis(JOY1_LEFT, JOY1_RIGHT), JoyTrigger(JOY1_FIRE)),
  DigitalJoystick(Joystick_(1), JoyAxis(JOY2_UP, JOY2_DOWN), JoyAxis(JOY2_LEFT, JOY2_RIGHT), JoyTrigger(JOY2_FIRE)),
};

void setup() {}

void loop() {
  for (auto &j : digiJoysticks) {
    j.update();
  }
  delay(10);
}

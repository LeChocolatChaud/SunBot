#include "Gamepad.h"
#include "Arduino.h"

Gamepad::Gamepad(uint8_t deadRadius) : DEAD_ZONE_RADIUS(deadRadius) {}

byte Gamepad::waitUntilConnected(unsigned long interval, byte maxTries) {
  byte error = 0;
  byte tryCount = 0;
  do {
    error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
    if (error == 0) {
      break;
    } else {
      tryCount++;
      delay(interval);
    }
  } while (maxTries == 0 ? 1 : tryCount < maxTries);
  return error;
}

boolean Gamepad::update() { return ps2x.read_gamepad(false, 0); }

boolean Gamepad::readButton(Button button) { return ps2x.Button(button); }

boolean Gamepad::readButtonPressed(Button button) {
  return ps2x.ButtonPressed(button);
}

boolean Gamepad::readButtonReleased(Button button) {
  return ps2x.ButtonReleased(button);
}

uint8_t smoothInput(uint8_t x) {
  double dx = x;
  double after = 0.0001 * pow(x - 127.5, 3) + 127.5;
  if (after > 255) {
    after = 255;
  } else if (after < 0) {
    after = 0;
  }
  uint8_t result = floor(after);
  return result;
}

int8_t Gamepad::readStickAxis(StickAxis axis, boolean smooth) {
  uint8_t value = ps2x.Analog(axis);
  value = 255 - value;
  if (smooth) {
    value = smoothInput(value);
  }

  int8_t result = map(value, 0, 255, -128, 127);
  return result;
}

boolean Gamepad::hasStickInput() {
  uint8_t ly = ps2x.Analog(PSS_LY);
  uint8_t lx = ps2x.Analog(PSS_LX);
  uint8_t ry = ps2x.Analog(PSS_RY);
  uint8_t rx = ps2x.Analog(PSS_RX);

  return !(isDead(ly) && isDead(lx) && isDead(ry) && isDead(rx));
}

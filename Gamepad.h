#ifndef GAMEPAD_H
#define GAMEPAD_H

#include <Arduino.h>
#include "PS2X_lib.h"

enum Button : uint16_t {
  DPAD_UP = PSB_PAD_UP,
  DPAD_DOWN = PSB_PAD_DOWN,
  DPAD_LEFT = PSB_PAD_LEFT,
  DPAD_RIGHT = PSB_PAD_RIGHT,
  CROSS = PSB_CROSS,
  CIRCLE = PSB_CIRCLE,
  SQUARE = PSB_SQUARE,
  TRIANGLE = PSB_TRIANGLE,
  START = PSB_START,
  SELECT = PSB_SELECT,
  LEFT_BUMPER = PSB_L1,
  LEFT_TRIGGER = PSB_L2,
  RIGHT_BUMPER = PSB_R1,
  RIGHT_TRIGGER = PSB_R2,
  LEFT_STICK = PSB_L3,
  RIGHT_STICK = PSB_R3
};

enum StickAxis : uint16_t {
  LEFT_Y = PSS_LY,
  LEFT_X = PSS_LX,
  RIGHT_Y = PSS_RY,
  RIGHT_X = PSS_RX
};

class Gamepad {
 private:
  PS2X ps2x;
  const uint8_t DEAD_ZONE_RADIUS;

  boolean isDead(uint8_t value) {
    return value > 127 - DEAD_ZONE_RADIUS && value < 128 + DEAD_ZONE_RADIUS;
  }

 public:
  Gamepad(uint8_t deadRadius = 2);
  byte waitUntilConnected(unsigned long interval = 100, byte maxTries = 0);
  boolean update();

  boolean readButton(Button button);
  boolean readButtonPressed(Button button);
  boolean readButtonReleased(Button button);
  int8_t readStickAxis(StickAxis axis, boolean smooth = true);

  boolean hasStickInput();
};

#endif  // GAMEPAD_H

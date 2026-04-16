#include <Wire.h>
#include "QGPMaker_MotorShield.h"
#include "Chassis.h"
#include "Arm.h"
#include "Gamepad.h"

// Create the motor shield driver with the default I2C address
QGPMaker_MotorShield driver = QGPMaker_MotorShield();
Gamepad gamepad;

// Motor definitions
QGPMaker_DCMotor *frontleftMotor = driver.getMotor(3);
QGPMaker_DCMotor *backleftMotor = driver.getMotor(4);
QGPMaker_DCMotor *frontrightMotor = driver.getMotor(2);
QGPMaker_DCMotor *backrightMotor = driver.getMotor(1);

// Servo definitions
QGPMaker_Servo *shoulderRollServo = driver.getServo(0);
QGPMaker_Servo *shoulderPitchServo = driver.getServo(1);
QGPMaker_Servo *elbowPitchServo = driver.getServo(2);
QGPMaker_Servo *wristRollServo = driver.getServo(4);
QGPMaker_Servo *wristPitchServo = driver.getServo(3);
QGPMaker_Servo *handServo = driver.getServo(5);

Chassis chassis(frontleftMotor, backleftMotor, frontrightMotor, backrightMotor, 64);

ServoProxy shoulderRollProxy(shoulderRollServo, 0, 160);
ServoProxy shoulderPitchProxy(shoulderPitchServo, 0, 160);
ServoProxy elbowPitchProxy(elbowPitchServo, 0, 160);
ServoProxy wristRollProxy(wristRollServo, 0, 160);
ServoProxy wristPitchProxy(wristPitchServo, 0, 160);
ServoProxy handProxy(handServo, 0, 160);

Arm arm(&shoulderRollProxy, &shoulderPitchProxy, &elbowPitchProxy, &wristRollProxy, &wristPitchProxy, &handProxy);

void setup() {
  Serial.begin(57600);
  driver.begin(50);  // create with the default frequency 50Hz
  gamepad.waitUntilConnected();
}

float x = 10.0f;
float y = 0;
float z = 10.0f;
uint8_t pitch = 140;
uint8_t roll = 160;
uint8_t hand = 0;

int8_t crossDirection = 1;
int8_t circleDirection = 1;

void loop() {
  gamepad.update();

  if (!gamepad.hasStickInput()) {
    chassis.stop();
  } else {
    int8_t ly = gamepad.readStickAxis(LEFT_Y);
    int8_t lx = gamepad.readStickAxis(LEFT_X);
    int8_t rx = gamepad.readStickAxis(RIGHT_X);

    Serial.print("[Main] Controller processed input: ly=");
    Serial.print(ly);
    Serial.print(" lx=");
    Serial.print(lx);
    Serial.print(" rx=");
    Serial.println(rx);
    chassis.drive(ly, lx, rx);
  }

  boolean up = gamepad.readButton(DPAD_UP);
  boolean down = gamepad.readButton(DPAD_DOWN);
  boolean left = gamepad.readButton(DPAD_LEFT);
  boolean right = gamepad.readButton(DPAD_RIGHT);
  boolean cross = gamepad.readButton(CROSS);
  boolean circle = gamepad.readButton(CIRCLE);
  boolean leftBumper = gamepad.readButton(LEFT_BUMPER);
  boolean leftTrigger = gamepad.readButton(LEFT_TRIGGER);
  boolean rightBumper = gamepad.readButton(RIGHT_BUMPER);
  boolean rightTrigger = gamepad.readButton(RIGHT_TRIGGER);

  if (up) {
    x += 0.5;
  }
  if (down) {
    x -= 0.5;
  }
  if (left) {
    y += 0.5;
  }
  if (right) {
    y -= 0.5;
  }

  float dist = sqrt(x * x + y * y);
  if (dist > 19.2f) {
    x = x / dist * 19.2f;
    y = y / dist * 19.2f;
  }

  if (cross) {
    z += crossDirection * 0.5;
  } else if (gamepad.readButtonReleased(CROSS)) {
    crossDirection = -crossDirection;
  }

  if (leftBumper) {
    if (pitch >= 5) pitch -= 5;
    else pitch = 0;
  }
  if (rightBumper) {
    if (pitch <= 155) pitch += 5;
    else pitch = 160;
  }

  if (rightTrigger) {
    if (roll >= 5) roll -= 5;
    else roll = 0;
  }
  if (leftTrigger) {
    if (roll <= 155) roll += 5;
    else roll = 160;
  }

  if (circle) {
    hand += circleDirection * 5;
    if (hand >= 250) hand = 0;
  } else if (gamepad.readButtonReleased(CIRCLE)) {
    circleDirection = -circleDirection;
  }

  pitch = constrain(pitch, 0, 160);
  roll = constrain(roll, 0, 160);
  hand = constrain(hand, 0, 160);

  arm.move(x, y, z, pitch, roll, hand);

  delay(50);
}

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
QGPMaker_Servo *wristRollServo = driver.getServo(3);
QGPMaker_Servo *wristPitchServo = driver.getServo(4);
QGPMaker_Servo *handServo = driver.getServo(5);

Chassis chassis(frontleftMotor, backleftMotor, frontrightMotor, backrightMotor);
ServoProxy shoulderRollProxy(shoulderRollServo, 0, 180);
ServoProxy shoulderPitchProxy(shoulderPitchServo, 0, 180);
ServoProxy elbowPitchProxy(elbowPitchServo, 0, 180);
ServoProxy wristRollProxy(wristRollServo, 0, 180);
ServoProxy wristPitchProxy(wristPitchServo, 0, 180);
ServoProxy handProxy(handServo, 0, 180);

Arm arm(shoulderRollProxy, shoulderPitchProxy, elbowPitchProxy, wristRollProxy, wristPitchProxy, handProxy);

void setup() {
  Serial.begin(57600);
  driver.begin(50);  // create with the default frequency 50Hz
  gamepad.waitUntilConnected();
}

float x = 0;
float y = 0;
float z = 0;
uint8_t pitch = 70;
uint8_t roll = 0;
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

  if (right) {
    x += 0.1;
  }
  if (left) {
    x -= 0.1;
  }
  if (up) {
    y += 0.1;
  }
  if (down) {
    y -= 0.1;
  }

  if (cross) {
    z += crossDirection * 0.1;
  } else if (gamepad.readButtonReleased(CROSS)) {
    crossDirection = -crossDirection;
  }

  if (leftBumper) {
    pitch -= 1;
  }
  if (rightBumper) {
    pitch += 1;
  }

  if (leftTrigger) {
    roll -= 1;
  }
  if (rightTrigger) {
    roll += 1;
  }

  if (circle) {
    hand += circleDirection * 5;
  } else if (gamepad.readButtonReleased(CIRCLE)) {
    circleDirection = -circleDirection;
  }

  arm.move(x, y, z, pitch, roll, hand);

  delay(50);
}

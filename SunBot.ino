#include <Wire.h>
#include "QGPMaker_MotorShield.h"
#include "PS2X_lib.h"
#include "Chassis.h"
#include "Arm.h"

constexpr int DEAD_ZONE_RADIUS = 2;

// Create the motor shield driver with the default I2C address
QGPMaker_MotorShield driver = QGPMaker_MotorShield();
PS2X ps2x;

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

void setup() {
  Serial.begin(57600);
  driver.begin(50);  // create with the default frequency 50Hz
  int error;
  do {
    error = ps2x.config_gamepad(13, 11, 10, 12, true, true);
    if (error == 0) {
      break;
    } else {
      delay(100);
    }
  } while (1);
}

int isDead(uint8_t value) {
  return value > 127 - DEAD_ZONE_RADIUS && value < 128 + DEAD_ZONE_RADIUS;
}

uint8_t smoothInput(uint8_t x) {
  double dx = x;
  double after = 0.0001 * pow(x - 127.5, 3) + 127.5;
  if (after > 255) after = 255;
  else if (after < 0) after = 0;
  uint8_t result = floor(after);
  return result;
}

void loop() {
  ps2x.read_gamepad(false, 0);

  uint8_t ly = ps2x.Analog(PSS_LY);
  uint8_t lx = ps2x.Analog(PSS_LX);
  uint8_t ry = ps2x.Analog(PSS_RY);
  uint8_t rx = ps2x.Analog(PSS_RX);

  Serial.print("[Main] Controller raw input: ly=");
  Serial.print(ly);
  Serial.print(" lx=");
  Serial.print(lx);
  Serial.print(" ry=");
  Serial.print(ry);
  Serial.print(" rx=");
  Serial.println(rx);

  if (isDead(ly) && isDead(lx) && isDead(ry) && isDead(rx)) {
    chassis.stop();
    Serial.println("[Main] Controller stick in dead zone.");
  } else {
    ly = 255 - ly;
    lx = 255 - lx;
    rx = 255 - rx;

    ly = smoothInput(ly);
    lx = smoothInput(lx);
    rx = smoothInput(rx);

    Serial.print("[Main] Controller processed input: ly=");
    Serial.print(ly);
    Serial.print(" lx=");
    Serial.print(lx);
    Serial.print(" rx=");
    Serial.println(rx);
    chassis.drive(ly, lx, rx);
  }

  delay(50);
}

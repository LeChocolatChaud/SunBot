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

void setup() {
  Serial.begin(57600);
  driver.begin(50);  // create with the default frequency 50Hz
  gamepad.waitUntilConnected();
}



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

  delay(50);
}

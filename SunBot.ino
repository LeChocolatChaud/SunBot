#include <QGPMaker_MotorShield.h>

QGPMaker_MotorShield driver;

QGPMaker_DCMotor *flMotor = driver.getMotor(1);
QGPMaker_DCMotor *frMotor = driver.getMotor(2);
QGPMaker_DCMotor *blMotor = driver.getMotor(3);
QGPMaker_DCMotor *brMotor = driver.getMotor(4);

void setup() {
  driver.begin(50);
}

void loop() {
}

#include <stdlib.h>
#include "Chassis.h"
#include "Arduino.h"

Chassis::Chassis(QGPMaker_DCMotor *frontleft, QGPMaker_DCMotor *backleft,
                 QGPMaker_DCMotor *frontright, QGPMaker_DCMotor *backright,
                 int maxPower = 127)
    : frontleft(frontleft), backleft(backleft), frontright(frontright),
      backright(backright), MAX_POWER(min(maxPower, 127)) {}

void Chassis::stopMotor(QGPMaker_DCMotor *motor) {
    motor->setSpeed(0);
    motor->run(RELEASE);
}

void Chassis::stop() {
    stopMotor(frontleft);
    stopMotor(backleft);
    stopMotor(frontright);
    stopMotor(backright);
}

void Chassis::setMotorPower(QGPMaker_DCMotor *motor, long power) {
    if (power < -2) {
        uint8_t speed = map(-power, 3, 128, 0, 255);
        speed = map(speed, 0, 255, 0, 127);
        motor->setSpeed(speed);
        motor->run(FORWARD);
        Serial.print(speed);
        Serial.print(",FORWARD");
    } else if (power >= 2) {
        uint8_t speed = map(power, 2, 127, 0, 255);
        speed = map(speed, 0, 255, 0, 127);
        motor->setSpeed(speed);
        motor->run(BACKWARD);
        Serial.print(speed);
        Serial.print(",BACKWARD");
    } else {
        stopMotor(motor);
        Serial.print("STOPPED");
    }
}

void Chassis::drive(uint8_t axial, uint8_t lateral, uint8_t yaw) {
    long axials = mapStickValue(axial);
    long laterals = mapStickValue(lateral);
    long yaws = mapStickValue(yaw);

    long frontleftPower = axials - laterals - yaws;
    long frontrightPower = axials + laterals + yaws;
    long backleftPower = axials + laterals - yaws;
    long backrightPower = axials - laterals + yaws;

    long maxPower = max(abs(frontleftPower), abs(frontrightPower));
    maxPower = max(maxPower, abs(backleftPower));
    maxPower = max(maxPower, abs(backrightPower));

    if (maxPower > MAX_POWER) {
        frontleftPower =
            map(frontleftPower, -maxPower, maxPower, -MAX_POWER - 1, MAX_POWER);
        frontrightPower = map(frontrightPower, -maxPower, maxPower,
                              -MAX_POWER - 1, MAX_POWER);
        backleftPower =
            map(backleftPower, -maxPower, maxPower, -MAX_POWER - 1, MAX_POWER);
        backrightPower =
            map(backrightPower, -maxPower, maxPower, -MAX_POWER - 1, MAX_POWER);
    }

    Serial.print("[Chassis] Calculated motor power: fl=");
    Serial.print(frontleftPower);
    Serial.print(" fr=");
    Serial.print(frontrightPower);
    Serial.print(" bl=");
    Serial.print(backleftPower);
    Serial.print(" br=");
    Serial.println(backrightPower);

    Serial.print("[Chassis] Effective motor power: fl=");
    setMotorPower(frontleft, frontleftPower);
    Serial.print(" fr=");
    setMotorPower(backleft, backleftPower);
    Serial.print(" bl=");
    setMotorPower(frontright, frontrightPower);
    Serial.print(" br=");
    setMotorPower(backright, backrightPower);
    Serial.println();
}

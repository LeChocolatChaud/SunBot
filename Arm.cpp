#include "Arm.h"

ServoProxy::ServoProxy(QGPMaker_Servo *servo, uint8_t minAngle,
                       uint8_t maxAngle)
    : servo(servo), MIN_ANGLE(minAngle), MAX_ANGLE(maxAngle) {}

uint8_t ServoProxy::readDegrees() { return servo->readDegrees(); }

void ServoProxy::writeServo(uint8_t angle) {
    servo->writeServo(constrain(angle, MIN_ANGLE, MAX_ANGLE));
}

void ServoProxy::setServoPulse(double pulse) { servo->writeServo(pulse); }

void Arm::calculateJointAngles(float x, float y, float z,
                               uint8_t wristPitchAngle) {
    float thetaBase = atan2(x, z) * (180.0f / PI);
    float r = sqrt(x * x + z * z);

    float yEff = y;

    float cosElbow =
        (r * r + yEff * yEff - L1 * L1 - L2 * L2) / (2.0f * L1 * L2);
    cosElbow = constrain(cosElbow, -1.0f, 1.0f);
    float thetaElbow = acos(cosElbow) * (180.0f / PI);

    float phi = atan2(yEff, r);
    float cosPsi = (L1 * L1 + r * r + yEff * yEff - L2 * L2) /
                   (2.0f * L1 * sqrt(r * r + yEff * yEff));
    cosPsi = constrain(cosPsi, -1.0f, 1.0f);
    float psi = acos(cosPsi);
    float thetaShoulder = (phi + psi) * (180.0f / PI);

    float thetaWrist = wristPitchAngle - thetaShoulder + thetaElbow;

    angles[0] = static_cast<int>(round(thetaBase));
    angles[1] = static_cast<int>(round(thetaShoulder));
    angles[2] = static_cast<int>(round(thetaElbow));
    angles[3] = static_cast<int>(round(thetaWrist));
}

#include "Arm.h"
#include "Arduino.h"
#include "HardwareSerial.h"

void Arm::calculateJointAngles(float x, float y, float z,
                               uint8_t wristPitchAngle) {
  float thetaBase = atan2(y, x) * (180.0f / PI);
  float r = sqrt(x * x + y * y);

  float cosElbow = (r * r + z * z - L1 * L1 - L2 * L2) / (2.0f * L1 * L2);
  cosElbow = constrain(cosElbow, -1.0f, 1.0f);
  float thetaElbow = acos(cosElbow) * (180.0f / PI);

  float phi = atan2(z, r);
  float cosPsi =
      (L1 * L1 + r * r + z * z - L2 * L2) / (2.0f * L1 * sqrt(r * r + z * z));
  cosPsi = constrain(cosPsi, -1.0f, 1.0f);
  float psi = acos(cosPsi);
  float thetaShoulder = (phi + psi) * (180.0f / PI);

  float thetaWrist = wristPitchAngle + thetaShoulder - thetaElbow;

  angles[0] = constrain(static_cast<int>(round(thetaBase)) + 73, 0, 160);
  angles[1] = constrain(static_cast<int>(round(thetaShoulder)), 0, 160);
  angles[2] = constrain(static_cast<int>(round(thetaElbow)) + 70, 0, 160);
  angles[3] = constrain(static_cast<int>(round(thetaWrist)), 0, 160);

  if (flipped) {
    angles[1] = 160 - angles[1];
    angles[2] = 160 - angles[2];
    angles[3] = 160 - angles[3];
  }
}

Arm::Arm(QGPMaker_Servo *shoulderRollServo, QGPMaker_Servo *shoulderPitchServo,
         QGPMaker_Servo *elbowPitchServo, QGPMaker_Servo *wristRollServo,
         QGPMaker_Servo *wristPitchServo, QGPMaker_Servo *handServo)
    : shoulderRollServo(shoulderRollServo),
      shoulderPitchServo(shoulderPitchServo), elbowPitchServo(elbowPitchServo),
      wristRollServo(wristRollServo), wristPitchServo(wristPitchServo),
      handServo(handServo) {}

void Arm::move(float x, float y, float z, uint8_t pitch, uint8_t roll,
               uint8_t hand) {
  calculateJointAngles(x, y, z, pitch);

  Serial.print("[Arm] shoulderRoll=");
  Serial.print(angles[0]);
  Serial.print(" shoulderPitch=");
  Serial.print(angles[1]);
  Serial.print(" elbowPitch=");
  Serial.print(angles[2]);
  Serial.print(" wristRoll=");
  Serial.print(roll);
  Serial.print(" wristPitch=");
  Serial.print(angles[3]);
  Serial.print(" hand=");
  Serial.print(hand);

  shoulderRollServo->writeServo(angles[0]);
  shoulderPitchServo->writeServo(angles[1]);
  elbowPitchServo->writeServo(angles[2]);
  wristRollServo->writeServo(constrain(roll, 0, 160));
  wristPitchServo->writeServo(angles[3]);
  handServo->writeServo(constrain(hand, 0, 160));
}

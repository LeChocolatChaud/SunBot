#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include "QGPMaker_MotorShield.h"

/**
 * A 6-DoF arm.
 */
class Arm {
  private:
    int angles[4]{0, 0, 0, 0};
    const float L1 = 10.4f;  // Shoulder to elbow (cm)
    const float L2 = 8.8f;   // Elbow to wrist (cm)

    QGPMaker_Servo *shoulderRollServo;
    QGPMaker_Servo *shoulderPitchServo;
    QGPMaker_Servo *elbowPitchServo;
    QGPMaker_Servo *wristRollServo;
    QGPMaker_Servo *wristPitchServo;
    QGPMaker_Servo *handServo;

    void calculateJointAngles(float x, float y, float z, uint8_t pitch);

  public:
    Arm(QGPMaker_Servo *shoulderRollServo, QGPMaker_Servo *shoulderPitchServo,
        QGPMaker_Servo *elbowPitchServo, QGPMaker_Servo *wristRollServo,
        QGPMaker_Servo *wristPitchServo, QGPMaker_Servo *handServo);

  /** Whether the arm is flipped. */
  boolean flipped = false;

  /**
   * Move the arm to the desired location. The origin is at the center of the
   * shoulder joint.
   *
   * @param x The axial distance in cm from origin to wrist. (+ = forward, - =
   * backward)
   * @param y The lateral distance in cm from otigin to wrist. (+ = left, - =
   * right)
   * @param z The vertical distance in cm from origin to wrist.
   * @param pitch The pitch of wrist, using vertical up as 0.
   * @param roll The roll of wrist.
   * @param hand The hand servo angle.
   */
  void move(float x, float y, float z, uint8_t pitch, uint8_t roll,
            uint8_t hand);
};

#endif  // ARM_H

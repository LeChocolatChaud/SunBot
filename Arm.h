#ifndef ARM_H
#define ARM_H

#include <Arduino.h>
#include "QGPMaker_MotorShield.h"

/**
 * A proxy to control servo.
 */
class ServoProxy {
   private:
    QGPMaker_Servo *servo;

   public:
    const uint8_t MIN_ANGLE;
    const uint8_t MAX_ANGLE;

    /**
     * @param servo The servo that this ServoProxy will control.
     * @param minAngle The minimum rotation angle, defaults to 0.
     * @param maxAngle The maximum rotation angle, defaults to 180.
     */
    ServoProxy(QGPMaker_Servo *servo, uint8_t minAngle, uint8_t maxAngle);

    /**
     * Read servo angle as unsigned degrees.
     *
     * @return The servo angle.
     */
    uint8_t readDegrees();

    /**
     * Write an angle in degrees to the servo.
     * The actual written value will be constrained to [MIN_ANGLE, MAX_ANGLE].
     *
     * @param angle The desired angle.
     */
    void writeServo(uint8_t angle);

    /**
     * Set the PWM pulse width of the servo.
     *
     * @param pulse The pulse width in seconds.
     */
    void setServoPulse(double pulse);
};

/**
 * A 6-DoF arm.
 */
class Arm {
   private:
    int angles[4]{0, 0, 0, 0};
    const float L1 = 10.4f;  // Shoulder to elbow (cm)
    const float L2 = 8.8f;   // Elbow to wrist (cm)

    ServoProxy* shoulderRollServo;
    ServoProxy* shoulderPitchServo;
    ServoProxy* elbowPitchServo;
    ServoProxy* wristRollServo;
    ServoProxy* wristPitchServo;
    ServoProxy* handServo;

    void calculateJointAngles(float x, float y, float z, uint8_t pitch);

   public:
    Arm(ServoProxy* shoulderRollServo, ServoProxy* shoulderPitchServo,
        ServoProxy* elbowPitchServo, ServoProxy* wristRollServo,
        ServoProxy* wristPitchServo, ServoProxy* handServo);

    /**
     * Move the arm to the desired location. The origin is at the center of the
     * shoulder joint.
     *
     * @param x The lateral distance in cm from origin to wrist.
     * @param y The axial distance in cm from otigin to wrist.
     * @param z The vertical distance in cm from origin to wrist.
     * @param pitch The pitch of wrist, using vertical up as 0.
     * @param roll The roll of wrist.
     * @param hand The hand servo angle.
     */
    void move(float x, float y, float z, uint8_t pitch, uint8_t roll,
              uint8_t hand);
};

#endif  // ARM_H

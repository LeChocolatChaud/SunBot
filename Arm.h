#ifndef ARM_H
#define ARM_H

#include <Arduino.h>

class Arm {
   public:
    static constexpr float L1 = 10.4f;  // Shoulder to elbow (cm)
    static constexpr float L2 = 8.8f;   // Elbow to wrist (cm)
    static constexpr float L3 = 17.2f;  // Wrist to claw (cm)

    static const int MIN_SHOULDER_ROLL = -90;
    static const int MAX_SHOULDER_ROLL = 90;

    static const int MIN_SHOULDER_PITCH = -90;
    static const int MAX_SHOULDER_PITCH = 90;

    static const int MIN_ELBOW_PITCH = -130;
    static const int MAX_ELBOW_PITCH = 130;

    static const int MIN_WRIST_PITCH = -90;
    static const int MAX_WRIST_PITCH = 90;

    static int *calculateJointAngles(int x, int y, int z, int wristPitchAngle);

   private:
    static int angles[4];
};

#endif  // ARM_H

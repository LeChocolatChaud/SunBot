#include "Arm.h"

int Arm::angles[4] = {0, 0, 0, 0};

int *Arm::calculateJointAngles(int targetX, int targetY, int targetZ,
                               int wristPitchAngle) {
    float x = static_cast<float>(targetX);
    float y = static_cast<float>(targetY);
    float z = static_cast<float>(targetZ);

    float thetaBase = atan2(x, z) * (180.0f / PI);
    float r = sqrt(x * x + z * z);

    float yEff = y - L3;

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

    thetaBase = constrain(thetaBase, static_cast<float>(MIN_SHOULDER_ROLL),
                          static_cast<float>(MAX_SHOULDER_ROLL));
    thetaShoulder =
        constrain(thetaShoulder, static_cast<float>(MIN_SHOULDER_PITCH),
                  static_cast<float>(MAX_SHOULDER_PITCH));
    thetaElbow = constrain(thetaElbow, static_cast<float>(MIN_ELBOW_PITCH),
                           static_cast<float>(MAX_ELBOW_PITCH));
    thetaWrist = constrain(thetaWrist, static_cast<float>(MIN_WRIST_PITCH),
                           static_cast<float>(MAX_WRIST_PITCH));

    angles[0] = static_cast<int>(round(thetaBase));
    angles[1] = static_cast<int>(round(thetaShoulder));
    angles[2] = static_cast<int>(round(thetaElbow));
    angles[3] = static_cast<int>(round(thetaWrist));

    return angles;
}

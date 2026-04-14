#ifndef CHASSIS_H
#define CHASSIS_H

#include <QGPMaker_MotorShield.h>

class Chassis {
   private:
    QGPMaker_DCMotor *frontleft;
    QGPMaker_DCMotor *backleft;
    QGPMaker_DCMotor *frontright;
    QGPMaker_DCMotor *backright;

    const int MAX_POWER;

    static void setMotorPower(QGPMaker_DCMotor *motor, long power);
    static void stopMotor(QGPMaker_DCMotor *motor);
    static long mapStickValue(uint8_t stick) {
        return map(stick, 0, 255, -128, 127);
    }

   public:
    Chassis(QGPMaker_DCMotor *frontleft, QGPMaker_DCMotor *backleft,
            QGPMaker_DCMotor *frontright, QGPMaker_DCMotor *backright,
            int maxPower);
    void drive(uint8_t axial, uint8_t lateral, uint8_t yaw);
    void stop();
};

#endif  // CHASSIS_H

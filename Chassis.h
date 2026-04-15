#ifndef CHASSIS_H
#define CHASSIS_H

#include <QGPMaker_MotorShield.h>

/**
 * A Mecanum-driven chassis.
 */
class Chassis {
 private:
  QGPMaker_DCMotor *frontleft;
  QGPMaker_DCMotor *backleft;
  QGPMaker_DCMotor *frontright;
  QGPMaker_DCMotor *backright;

  const int MAX_POWER;

  static void setMotorPower(QGPMaker_DCMotor *motor, long power);
  static void stopMotor(QGPMaker_DCMotor *motor);

 public:
  Chassis(QGPMaker_DCMotor *frontleft, QGPMaker_DCMotor *backleft,
          QGPMaker_DCMotor *frontright, QGPMaker_DCMotor *backright,
          int maxPower = 127);

  /**
   * Drive the chassis according to speed components.
   *
   * @param axial The axial speed component.
   * @param lateral The lateral speed component.
   * @param yaw The yaw stick speed component.
   */
  void drive(int8_t axial, int8_t lateral, int8_t yaw);

  /**
   * Stop the chassis.
   */
  void stop();
};

#endif  // CHASSIS_H

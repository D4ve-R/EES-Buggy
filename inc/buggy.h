#pragma once

#include <vector>

#include "adafruitmotohat.h"
#include "ultrasonic.h"
#include "gy521.h"


/**
 * Represents the buggy
 * bundles functionality
 */
class Buggy
{
  Buggy();
  ~Buggy();

  void moveForward();
  void moveBackward();
  void turnLeft(int deg = 90);
  void turnRight(int deg = 90);
  void stop();
  void setSpeed(int _speed)

  private:
    int speed;
    std::vector<std::shared_ptr<AdafruitDCMotor>> motors;
    AdafruitMotorHAT hat;
    Ultrasonic sonic;
    GY521 gyro;

};

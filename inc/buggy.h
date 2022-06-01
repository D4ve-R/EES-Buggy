#pragma once

#include <vector>

#include "adafruitmotorhat.h"
#include "ultrasonic.h"
#include "gy521.h"


#define MAX_SPEED   255
#define MIN_SPEED   0


/**
 * Represents the buggy
 * bundles functionality
 */
class Buggy
{
    public:

    Buggy();
    ~Buggy();

    void drive();

    void move(AdafruitDCMotor::Command command, int _speed, int delay_ms);
    void moveForward(int _speed = MAX_SPEED, int delay_ms = 500);
    void moveBackward(int _speed = MAX_SPEED, int delay_ms = 1000);
    void turnLeft(int deg = 90);
    void turnRight(int deg = 90);
    void rotate(int deg = 90, bool clockwise = true);
    void stop();

    private:

    int speed;
    std::vector<std::shared_ptr<AdafruitDCMotor> > motors;
    AdafruitMotorHAT hat;
    Ultrasonic sonic;
    GY521 gyro;

    void releaseAll();
    void setSpeed(int _speed);

};

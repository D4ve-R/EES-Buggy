#pragma once

#include <vector>

#include "adafruit/adafruitmotorhat.h"
#include "sensors/hcsr04.h"
#include "sensors/gy521.h"
#include "led.h"


#define MAX_SPEED   255
#define MIN_SPEED   0

// gpio numbers using wiringPi mapping
#define GPIO_0    0   // BCM GPIO 17
#define GPIO_1    1   // BCM GPIO 18
#define GPIO_2    2   // BCM GPIO 27

#define BUGGY_MOTOR_1   1
#define BUGGY_MOTOR_4   4

/**
 * Represents the buggy
 * bundles functionality
 */
class Buggy
{
    int speed;
    double estSpeedMS;
    double speedMax;

    std::vector<std::shared_ptr<AdafruitDCMotor> > motors;
    AdafruitMotorHAT* hat;
    HCSR04* sonic;
    GY521* gyro;
    Led* backlight;

    void releaseAll();
    void setSpeed(int _speed);
    bool safetyCheck();

    public:

    Buggy(bool verbose = true);
    ~Buggy();

    void drive();

    void move(AdafruitDCMotor::Command command, int delay_ms);
    void moveForward(int _speed = MAX_SPEED, int delay_ms = 1000);
    void moveBackward(int _speed = MAX_SPEED, int delay_ms = 1000);
    void turnLeft(int deg = 90);
    void turnRight(int deg = 90);
    void rotate(int deg = 90, bool clockwise = true);
    void stop();
    
    double getEstSpeedMS();
    double getSpeedMax();
    void estimateSpeed(double& dist, int& time);

    // fpr development purposes
    void _debug();

};


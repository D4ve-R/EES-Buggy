#pragma once

#include <vector>

#include "adafruitmotorhat.h"
#include "hcsr04.h"
#include "gy521.h"
#include "led.h"


#define MAX_SPEED   255
#define MIN_SPEED   0

// gpio numbers using wiringPi mapping
#define GPIO_0    0   // BCM GPIO 17
#define GPIO_1    1   // BCM GPIO 18
#define GPIO_2    2   // BCM GPIO 27

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

    private:

    int speed;
    double estSpeedMS;
    double speedMax;

    std::vector<std::shared_ptr<AdafruitDCMotor> > motors;
    AdafruitMotorHAT hat;
    HCSR04 sonic;
    GY521 gyro;
    Led backlight;

    void releaseAll();
    void setSpeed(int _speed);

};


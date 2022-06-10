#include "buggy.h"

#include <iostream>
#include <thread>
#include <wiringPi.h>


/**
 * Constructor for buggy
 *
 */ 
Buggy::Buggy():
  speed {0},
  estSpeedMS {0.0},
  speedMax {0.0},
  hat {nullptr},
  sonic {nullptr},
  gyro {nullptr},
  backlight {nullptr}
{
    // setup wiringPi using wiringPi gpio mapping
    // needed for Led:: & Ultrasonic::
    // should only be called once
    wiringPiSetup();

    hat = new AdafruitMotorHAT();
    gyro = new GY521();
    backlight = new Led(GPIO_2);

    sonic = new HCSR04(GPIO_0, GPIO_1);
    sonic->startMeasurement();

    // get connected motors
    motors.push_back(hat->getMotor(BUGGY_MOTOR_1));
    motors.push_back(hat->getMotor(BUGGY_MOTOR_4));
    setSpeed(speed);
}

/**
 * Deconsturctor for Buggy
 */
Buggy::~Buggy()
{
    if(sonic)
        delete sonic;

    if(gyro)
        delete gyro;

    if(backlight)
        delete backlight;
    
    if(hat)
    {
        releaseAll();
        delete hat;
    }
}

/**
 * "self" driving
 * checks ultrasonic sensor for distance
 */
void Buggy::drive()
{
  while(1)
  {
    if(sonic->distance() < HCSR04_MIN_RANGE_CM + 1)
    {
      stop();
      return;
    }

    while(sonic->distance() > 50)
    {
      moveForward();
    }

    while(sonic->distance() > 15)
    {
      moveForward((int)(MAX_SPEED/2 - sonic->distance()), 500);
    }
  }

  stop();
  return;

}

/**
 * move buggy in X direction
 * int command : AdafruitDCMotor::Command for direction
 * int _speed : speed of driving between 0 and 255
 * int delay_ms : time of driving in milliseconds
 */
void Buggy::move(AdafruitDCMotor::Command command, int delay_ms)
{
    backlight->off();

    for(auto motor : motors)
    {
      motor->setSpeed(speed);
      motor->run(command);
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( delay_ms ) );
}

/**
 * move buggy forward
 * estimates speed while driving
 * int _speed   : speed between 0 - 255, default(255)
 * int delay_ms : time to drive in milliseconds, default(1000)
 */
void Buggy::moveForward(int _speed, int delay_ms)
{
    if(!safetyCheck())
        return;

    setSpeed(_speed);

    double dist = sonic->distance();
    move(AdafruitDCMotor::kForward, delay_ms);
    dist -= sonic->distance();

    estimateSpeed(dist, delay_ms);
}

/**
 * move buggy backward
 * estimates speed while driving
 * int _speed   : speed between 0 - 255, default(255)
 * int delay_ms : time to drive in milliseconds, default(1000)
 */
void Buggy::moveBackward(int _speed, int delay_ms)
{
  setSpeed(_speed);

  double dist = sonic->distance();
  move(AdafruitDCMotor::kBackward, delay_ms);
  dist = sonic->distance() - dist;

  estimateSpeed(dist, delay_ms);
}

/**
 * turn buggy to left
 * int deg : degrees of turn, default 90
 */
void Buggy::turnLeft(int deg)
{
    if(!safetyCheck())
        return;

    backlight->off();

  if (motors.size() >= 2)
  {
    auto motorL = motors[0];
    auto motorR = motors[1];

    motorL->setSpeed(MAX_SPEED/2);
    motorR->setSpeed(MAX_SPEED);

    motorL->run(AdafruitDCMotor::kForward);
    motorR->run(AdafruitDCMotor::kForward);

    std::this_thread::sleep_for(std::chrono::milliseconds((deg/360)*1000));
  }
}

/**
 * turn buggy to right
 * int deg : degrees of turn, default(90)
 */
void Buggy::turnRight(int deg)
{
    if(!safetyCheck())
        return;

    backlight->off();

  if (motors.size() >= 2)
  {
    auto motorL = motors[0];
    auto motorR = motors[1];

    motorL->setSpeed(MAX_SPEED);
    motorR->setSpeed(MAX_SPEED/2);

    motorL->run(AdafruitDCMotor::kForward);
    motorR->run(AdafruitDCMotor::kForward);

    std::this_thread::sleep_for(std::chrono::milliseconds((deg/360)*1000));
  }

}

/**
 * rotate buggy
 * int deg : degrees of rotation default(90)
 * int clockwise : if true buggy turns clockwise, if false counter clockwise default(true)
 */
void Buggy::rotate(int deg, bool clockwise)
{
  if (motors.size() >= 2)
  {
    motors[0]->setSpeed(MAX_SPEED/2);
    motors[1]->setSpeed(MAX_SPEED/2);

    if (clockwise)
    {
      motors[0]->run(AdafruitDCMotor::kForward);
      motors[1]->run(AdafruitDCMotor::kBackward);
    }
    else 
    {
      motors[1]->run(AdafruitDCMotor::kForward);
      motors[0]->run(AdafruitDCMotor::kBackward);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds((deg/360)*1000));
  }
}

/**
 * stop buggy
 */
void Buggy::stop()
{
    backlight->on();

    for(auto motor : motors)
        motor->run(AdafruitDCMotor::kBrake);
}

double Buggy::getEstSpeedMS()
{
  return estSpeedMS;
}

double Buggy::getSpeedMax()
{
  return speedMax;
}

/**
 * estimate speed in meter/seconds from distance & time
 * double& distCM : distance travelled in centimeter
 * int& timeMS : time travelled in milliseconds
 */
void Buggy::estimateSpeed(double& distCM, int& timeMS)
{
  estSpeedMS = (distCM/100.0) / (timeMS/1000.0);

  speedMax = estSpeedMS > speedMax ? estSpeedMS : speedMax;
}

void Buggy::releaseAll()
{
  for(auto motor : motors)
    motor->run(AdafruitDCMotor::kRelease);
}

/**
 * sets the speed to a value between 0 and 255 inclusive
 */
void Buggy::setSpeed(int _speed)
{
  if (_speed < MIN_SPEED)
  {
    speed = MIN_SPEED;
    return;
  }

  if (_speed > MAX_SPEED)
  {
    speed = MAX_SPEED;
    return;
  }

  speed = _speed;
}

/**
 * check if buggy is too close to obstacle
 */
bool Buggy::safetyCheck()
{
    if(sonic->distance() < (HCSR04_MIN_RANGE_CM + (25 * (speed / 255))))
    {
      stop();
      return false;
    }

    return true;
}

void Buggy::_debug()
{
    sonic->distance();
    gyro->update();
    delay(100);
}

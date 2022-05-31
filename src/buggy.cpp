#include "buggy.h"


/**
 * Constructor for buggy
 *
 */
Buggy::Buggy():
  speed {0},
{

  // get all connected motors
  for(int i = 1; i < 5; i++)
  {
    if(auto motor = hat.getMotor(i))
    {
      motors.push_back(motor);
      motor->setSpeed(speed);
    }
  }

}

Buggy::~Buggy()
{

}

/**
 * move buggy forward
 */
void Buggy::moveForward()
{
  for(auto motor : motors)
  {
    motor->setSpeed(255);
    motor->run(AdafriutDCMotor::kForward);
  }
}

/**
 * move buggy backward
 */
void Buggy::moveBackward()
{
  for(auto motor : motors)
  {
    motor->setSpeed(255);
    motor->run(AdafriutDCMotor::kBackward);
  }

}

/**
 * turn buggy to left
 * int deg : degrees of turn, default 90
 */
void Buggy::turnLeft(int deg)
{

}

/**
 * turn buggy to right
 * int deg : degrees of turn, default 90
 */
void Buggy::turnRight(int deg)
{

}

/**
 * stop buggy
 */
void Buggy::stop()
{

}

/**
 * sets the speed to a value between 0 and 255 inclusive
 */
void Buggy::setSpeed(int _speed)
{
  if (_speed < 0)
  {
    speed = 0;
    return;
  }

  if (_speed > 255)
  {
    speed = 255;
    return;
  }

  speed = _speed;
}

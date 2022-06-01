#include "buggy.h"

#include <thread>
#include <wiringPi.h>

// gpio numbers using wiringPi mapping
#define GPIO_0    0
#define GPIO_1    1

/**
 * Constructor for buggy
 *
 */ 
Buggy::Buggy():
  speed {0},
  hat {}
{
  // setup wiringPi using wPi gpio mapping
  wiringPiSetup();

  sonic = Ultrasonic(GPIO_0, GPIO_1);

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

/**
 * Deconsturctor for Buggy
 */
Buggy::~Buggy()
{
  releaseAll();
}

/**
 * "self" driving
 * checks ultrasonic sensor for distance
 */
void Buggy::drive()
{
  while(1)
  {
    if(sonic.distance() < 5)
    {
      stop();
      return;
    }

    while(sonic.distance() > 50)
    {
      moveForward();
    }

    while(sonic.distance() > 15)
    {
      for(auto motor : motors)
      {
        motor->setSpeed(127 - sonic.distance());
        motor->run(AdafruitDCMotor::kForward);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
      }
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
void Buggy::move(AdafruitDCMotor::Command command, int _speed, int delay_ms)
{
    setSpeed(_speed);

    for(auto motor : motors)
    {
      motor->setSpeed(speed);
      motor->run(command);
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( delay_ms ) );
}

/**
 * move buggy forward
 */
void Buggy::moveForward(int _speed, int delay_ms)
{
  move(AdafruitDCMotor::kForward, _speed, delay_ms);
}

/**
 * move buggy backward
 */
void Buggy::moveBackward(int _speed, int delay_ms)
{
  move(AdafruitDCMotor::kBackward, _speed, delay_ms);
}

/**
 * turn buggy to left
 * int deg : degrees of turn, default 90
 */
void Buggy::turnLeft(int deg)
{
  if (motors.size() == 2)
  {
    auto motorL = motors[0];
    auto motorR = motors[1];

    motorL->setSpeed(MAX_SPEED);
    motorR->setSpeed(MAX_SPEED / 2);

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
  if (motors.size() == 2)
  {
    auto motorL = motors[0];
    auto motorR = motors[1];

    motorL->setSpeed(MAX_SPEED / 2);
    motorR->setSpeed(MAX_SPEED);

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
  if (motors.size() == 2)
  {
    motors[0]->setSpeed(MAX_SPEED);
    motors[1]->setSpeed(MAX_SPEED);

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
  for(auto motor : motors)
    motor->run(AdafruitDCMotor::kBrake);
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


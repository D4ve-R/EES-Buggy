#include "buggy.h"

#include <iostream>
#include <thread>
#include <wiringPi.h>


/**
 * Constructor for buggy
 *
 */ 
Buggy::Buggy(bool verbose):
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
    backlight = new Led(GPIO_2);

    sonic = verbose ? new HCSR04_LOG(GPIO_0, GPIO_1, false) : new HCSR04(GPIO_0, GPIO_1);
    gyro = verbose ?  new GY521_LOG() : new GY521();

    // get connected motors
    motors.push_back(hat->getMotor(BUGGY_MOTOR_1));
    motors.push_back(hat->getMotor(BUGGY_MOTOR_4));
    setSpeed(speed);

    // start measuring distance
    sonic->startMeasurement();

    delay(100);
}

/**
 * Deconstructor for Buggy
 * deconstruct all components
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
 * if obstacle in front, rotats 90 deg clockwise
 * check will be performed 8 times, before stoping
 */
void Buggy::drive()
{
    uint8_t errorCnt = 0;
    uint8_t maxErrors = 4;
  while(1)
  {
      if(!safetyCheck())
      {
          if(++errorCnt > maxErrors)
              break;

          rotateGY(90);
          continue;
      }
      
      moveForward(MAX_SPEED/2, 1000);

      errorCnt = 0;
  }

  stop();
}

/**
 * move buggy in X direction
 * int command : AdafruitDCMotor::Command for direction
 * int delay_ms : time of driving in milliseconds
 */
void Buggy::move(AdafruitDCMotor::Command command, int delay_ms)
{
    for(auto motor : motors)
    {
      motor->setSpeed(speed);
      motor->run(command);
    }

    std::this_thread::sleep_for( std::chrono::milliseconds( delay_ms ) );    
}

/**
 * move buggy forward
 * implements simple pid controller algorithm
 * estimates speed while driving
 * int _speed   : speed between 0 - 255, default(255)
 * int delay_ms : time to drive in milliseconds, default(1000)
 */
void Buggy::moveForward(int _speed, int delay_ms)
{
    static uint32_t T;

    backlight->off();
    gyro->setZRotation(0);

    double dist = sonic->distance();

    const int factor = 10;
    const uint32_t begin = millis();

    while(millis() - begin < delay_ms)
    {
        if(millis() - T > 10)
        {
            if(!safetyCheck())
                break;

            int rot = (int) gyro->getZRotation();
            int speedL = _speed + rot * factor;
            int speedR = _speed - rot * factor;

            speedL = speedL > MAX_SPEED ? MAX_SPEED : speedL;
            speedL = speedL < 10 ? 10 : speedL;
            speedR = speedR > MAX_SPEED ? MAX_SPEED : speedR;
            speedR = speedR < 10 ? 10 : speedR;

            motors[0]->setSpeed(speedL);
            motors[1]->setSpeed(speedR);

            motors[0]->run(AdafruitDCMotor::kForward);
            motors[1]->run(AdafruitDCMotor::kForward);

            T = millis();
        }
    }

    gyro->setZRotation(0);
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
    backlight->off();
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

    motorL->setSpeed(MAX_SPEED/4);
    motorR->setSpeed(MAX_SPEED/2);

    motorL->run(AdafruitDCMotor::kForward);
    motorR->run(AdafruitDCMotor::kForward);

    std::this_thread::sleep_for(std::chrono::milliseconds((deg/90)*1500));
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

    motorL->setSpeed(MAX_SPEED/2);
    motorR->setSpeed(MAX_SPEED/4);

    motorL->run(AdafruitDCMotor::kForward);
    motorR->run(AdafruitDCMotor::kForward);

    std::this_thread::sleep_for(std::chrono::milliseconds((deg/90)*1500));
  }

}

/**
 * rotate buggy
 * int deg : degrees of rotation default(90)
 * int clockwise : if true buggy turns clockwise, if false counter clockwise default(true)
 */
void Buggy::rotate(int deg, bool clockwise)
{
    backlight->off();
  if (motors.size() >= 2)
  {
    motors[0]->setSpeed(MAX_SPEED/3);
    motors[1]->setSpeed(MAX_SPEED/3);

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

    std::this_thread::sleep_for(std::chrono::milliseconds((deg/90)*1000));

    stop();
  }
}

/**
 * rotate buggy
 * int deg : degrees of rotation,
 * if deg is pos, rotates clockwise
 * if deg is neg, rotates counter-clockwise 
 */
void Buggy::rotateGY(int8_t deg)
{
    backlight->off();

    // reset z origin to 0 for orientation
    gyro->setZRotation();

    if (motors.size() >= 2)
    {
        motors[0]->setSpeed(MAX_SPEED/3);
        motors[1]->setSpeed(MAX_SPEED/3);

        if (deg < 0)
        {
            // counter-clockwise
            motors[1]->run(AdafruitDCMotor::kForward);
            motors[0]->run(AdafruitDCMotor::kBackward);

            while(gyro->getZRotation() < -deg)
                delay(10);
        }
        else
        {
            // clockwise
            motors[0]->run(AdafruitDCMotor::kForward);
            motors[1]->run(AdafruitDCMotor::kBackward);

            while((gyro->getZRotation()) > -deg)
                delay(10);
        }

        stop();

        gyro->setZRotation();
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
    if(sonic->distance() < HCSR04_MIN_RANGE_CM + 15.0)
    {
      stop();
      return false;
    }

    return true;
}

void Buggy::_debug()
{
    while(1)
    {
        gyro->update();
        delay(500);
    }
}

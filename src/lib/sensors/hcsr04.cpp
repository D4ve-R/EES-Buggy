#include "sensors/hcsr04.h"
#include <wiringPi.h>

/**
 * Constructor for ultrasonic sensor
 * @param uint8_t _pinTrigger : pin number of trigger pin
 * @param uint8_t _pinEcho    : pin number of echo pin
 */
HCSR04::HCSR04(uint8_t _pinTrigger, uint8_t _pinEcho):
    pinTrigger  {_pinTrigger},
    pinEcho     {_pinEcho},
    dist        {0.0},
    run      {false}
{
    // will be handled in parent class Buggy
    // can only be called once
    // wiringPiSetup();

    // set pin modes 
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinEcho, INPUT);

    // enable built-in pull down resistor of min. 50kOhm
    pullUpDnControl(pinEcho, PUD_DOWN);

    digitalWrite(pinTrigger, LOW);

    delay(10);
}

/**
 * Destroy the HCSR04::HCSR04 object
 * stop measurement thread if running
 * set gpios to low
 */
HCSR04::~HCSR04()
{
    if(run.load())
        stopMeasurement();

    digitalWrite(pinTrigger, LOW);
    digitalWrite(pinEcho, LOW);
}

/*
 * measures time in seconds and
 * calculate distance in cm
 * @return double distance[cm]
 */
double HCSR04::distance()
{
    if(!run.load())
        this->measurement();

    return getDist();
}

/**
 * loop function for measurement thread
 */
void HCSR04::threadLoop()
{
    while(run.load())
        this->measurement();
}

/**
 * starts new measurement thread
 * with threadLoop()
 */
void HCSR04::startMeasurement()
{
    run = true;
    t = std::thread(&threadLoop, this);

}

/**
 * stop measurement thread
 * and wait for thread to suspend
 */
void HCSR04::stopMeasurement()
{
    run = false;
    t.join();
}

/**
 * converts time period in distance
 * velocity is speed of sound
 * double timeS : time in seconds
 * @return double distance in centimeter
 */
double HCSR04::timeToDistanceCM(double timeS)
{
    // sound speed is 34320 cm/s
    return (timeS * 34320.0) / 2;
}

/**
 * triggers sensor to measurement
 * time periods according to datasheet
 */
void HCSR04::trigger()
{
    digitalWrite(pinTrigger, LOW);
    delayMicroseconds(2);

    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);
}

/**
 * measurement of distance
 * trigger measurement, wait for response and calculate dist from time
 * assigns new distance value with setDist()
 * prevents overtrigger
 * 
 * @return double distance[cm] 
 */
double HCSR04::measurement()
{
    uint32_t start = micros();
    uint32_t time = 0;

    trigger();

    while(digitalRead(pinEcho) == 0)
        start = micros();

    while(digitalRead(pinEcho) == 1)
    {
        time = micros() - start;

        // if bigger than 20ms, no detection
        if((time/1000) > 21)
        {
            // calculates to MAX_RANGE
            time = 23309;
            break;
        }
    }
   
    double d = timeToDistanceCM(time  / (2.0 * 1000000));
    setDist(d);

    // prevent over trigger, see datasheet
    delay(60);

    return d;
}

/**
 * set shared member variable dist thread safe
 * @param _dist 
 */
void HCSR04::setDist(double _dist)
{
    std::unique_lock<std::mutex> lock(mtx);

    dist = _dist;

    lock.unlock();
}

/**
 * get shared member variable dist thread safe
 * @return double latest distance dist 
 */
double HCSR04::getDist()
{
    double ret = 0.0;

    std::unique_lock<std::mutex> lock(mtx);

    ret = dist;

    lock.unlock();

    return ret;
}

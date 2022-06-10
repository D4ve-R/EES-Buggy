#include "sensors/hcsr04.h"
#include "adafruit/util.h"

#include <string>
#include <wiringPi.h>

/**
 * Constructor for ultrasonic sensor
 * uint8_t _pinTrigger : pin number of trigger pin
 * uint8_t _pinEcho    : pin number of echo pin
 */
HCSR04::HCSR04(uint8_t _pinTrigger, uint8_t _pinEcho, bool _verbose):
    pinTrigger  {_pinTrigger},
    pinEcho     {_pinEcho},
    dist        {0.0},
    run      {false},
    verbose  {_verbose}
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
 * returns distance[cm]
 */
double HCSR04::distance(void)
{
    if(!run.load())
        measurement();

    return getDist();
}

void HCSR04::threadLoop()
{
    while(run.load())
        measurement();
}

void HCSR04::startMeasurement()
{
    run = true;
    t = std::thread(&HCSR04::threadLoop, this);

}

void HCSR04::stopMeasurement()
{
    run = false;
    t.join();
}

/**
 * converts time period in distance
 * velocity is speed of sound
 * double timeS : time in seconds
 * returns distance in centimeter
 */
double HCSR04::timeToDistanceCM(double timeS)
{
    // sound speed is 34320 cm/s
    return (timeS * 34320.0) / 2;
}

/**
 * triggers sensor to measurement
 */
void HCSR04::trigger(void)
{
    digitalWrite(pinTrigger, LOW);
    delayMicroseconds(2);

    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);
}


void HCSR04::measurement()
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

    if(verbose)
        logger::output("Dist: " + std::to_string(d));

    // prevent over trigger, see datasheet
    delay(60);
}

void HCSR04::setDist(double _dist)
{
    std::unique_lock<std::mutex> lock(mtx);

    dist = _dist;

    lock.unlock();
}

double HCSR04::getDist()
{
    double ret = 0.0;

    std::unique_lock<std::mutex> lock(mtx);

    ret = dist;

    lock.unlock();

    return ret;
}

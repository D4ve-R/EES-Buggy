#include "ultrasonic.h"
#include <wiringPi.h>
#include <chrono>

/**
 * Constructor for ultrasonic sensor
 * uint8_t _pinTrigger : pin number of trigger pin
 * uint8_t _pinEcho    : pin number of echo pin
 */
Ultrasonic::Ultrasonic(uint8_t _pinTrigger, uint8_t _pinEcho):
    pinTrigger  {_pinTrigger},
    pinEcho     {_pinEcho},
{
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinEcho, INPUT);

    digitalWrite(pinTrigger, LOW);
    delay(200);
}

/*
 * measures time in seconds and
 * calculate distance in cm
 * returns distance[cm]
 */
double Ultrasonic::distance(void)
{
    trigger();

    auto start, stop = std::chrono::steady_clock::now();

    while(digitalRead(pinEcho) == 0)
        start = std::chrono::steady_clock::now();

    while(digitalRead(pinEcho) == 1)
        stop = std::chrono::steady_clock::now();

    // time in seconds
    std::chrono::duration<double> time = stop - start;

    // divided by 2 to measure only one way
    return timeToDistanceCM(time / 2.0);
}

/**
 * converts time period in distance
 * velocity is speed of sound
 * double timeS : time in seconds
 * returns distance in centimeter
 */
double timeToDistanceCM(double timeS)
{
    // sound speed is 34320 cm/s
    return timeS * 34320;
}

/**
 * triggers sensor to measurement
 */
void trigger(void)
{
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);
}

#include "ultrasonic.h"
#include <wiringPi.h>
#include <chrono>

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
 * return distance in cm
 *
 */
double Ultrasonic::distance(void)
{
    digitalWrite(pinTrigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);
    auto start, stop = std::chrono::steady_clock::now();

    while(digitalRead(pinEcho) == 0)
        start = std::chrono::steady_clock::now();

    while(digitalRead(pinEcho) == 1)
        stop = std::chrono::steady_clock::now();

    std::chrono::duration<double> time = stop - start;

    return time * 34320 / 2;
}



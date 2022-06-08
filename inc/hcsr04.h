#pragma once

#define HCSR04_MIN_RANGE_CM  2
#define HCSR04_MAX_RANGE_CM  399

#include <cstdint>

/**
 * Represents an ultrasonic sensor
 * like HC-SR04
 */
class HCSR04{
    uint8_t pinTrigger;
    uint8_t pinEcho;

    double timeToDistanceCM(double timeS);
    void trigger(void);

    public:
    HCSR04(uint8_t _pinTrigger, uint8_t _pinEcho);
    ~HCSR04();

    double distance(void);

};

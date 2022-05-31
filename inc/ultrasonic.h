#pragma once

/**
 * Represents an ultrasonic sensor
 * like HC-SR04
 */
class Ultrasonic {
    Ultrasonic(uint8_t _pinTrigger, uint8_t _pinEcho);
    
    double distance(void);

    private:
    uint8_t pinTrigger;
    uint8_t pinEcho;

    double timeToDistanceCM(double timeS)
    void trigger(void);
};

#pragma once

/**
 * Represents an ultrasonic sensor
 * like HC-SR04
 */
class Ultrasonic {
    public:
    Ultrasonic(){}
    Ultrasonic(int _pinTrigger, int _pinEcho);
    ~Ultrasonic(){}

    double distance(void);

    private:
    int pinTrigger;
    int pinEcho;

    double timeToDistanceCM(double timeS);
    void trigger(void);
};

#pragma once

#define HCSR04_MIN_RANGE_CM  2
#define HCSR04_MAX_RANGE_CM  399

#include <cstdint>
#include <thread>
#include <mutex>
#include <atomic>

/**
 * Represents an ultrasonic sensor
 * like HC-SR04
 */
class HCSR04{
    uint8_t pinTrigger;
    uint8_t pinEcho;
    
    double dist;
    
    std::atomic<bool> run;
    std::thread t;
    std::mutex mtx;

    double timeToDistanceCM(double timeS);
    void trigger(void);
    void measurement();
    void setDist(double _dist);
    double getDist();

    public:
    HCSR04(uint8_t _pinTrigger, uint8_t _pinEcho);
    ~HCSR04();

    double distance(void);
    void startMeasurement();
    void stopMeasurement();

};

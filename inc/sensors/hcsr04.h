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
    
    std::mutex mtx;

    double timeToDistanceCM(double timeS);
    void trigger(void);
    void threadLoop();
    void setDist(double _dist);

    protected:
    std::atomic<bool> run;
    std::thread t;

    double measurement();
    double getDist();

    public:
    HCSR04(uint8_t _pinTrigger, uint8_t _pinEcho);
    ~HCSR04();

    double distance(void);
    void startMeasurement();
    void stopMeasurement();

};

class HCSR04_LOG : public HCSR04
{
    bool verbose;

    double measurement();
    void threadLoop();

    public:

    HCSR04_LOG(uint8_t _pinTrigger, uint8_t _pinEcho, bool _verbose = true);
    ~HCSR04_LOG(){}

    double distance(void);
    void startMeasurement();
    void setVerbose(bool _verbose);
    bool getVerbose();
};

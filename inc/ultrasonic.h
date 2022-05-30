#pragma once

class Ultrasonic {
    Ultrasonic(uint8_t _pinTrigger, uint8_t _pinEcho);
    
    double distance(void);

    private:
    uint8_t pinTrigger;
    uint8_t pinEcho;
};

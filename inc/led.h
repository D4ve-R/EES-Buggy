#pragma once

#include <wiringPi.h>

class Led
{
    int pinNumber;

  public:
    Led(){}
    Led(int _pinNumber);
    ~Led();
    
    void on();
    void off();
    void blink();

};

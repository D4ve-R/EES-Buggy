#pragma once

#include <wiringPi.h>

class Led
{
  public:
    Led(){}
    Led(int _pinNumber);
    ~Led();
    
    void on();
    void off();
    void blink();

  private:
    int pinNumber;
};

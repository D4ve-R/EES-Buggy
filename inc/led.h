#pragma once

#include <wiringPi.h>

class Led
{
  public:
    Led(int _pinNumber = 0);
    ~Led();
    
    void on();
    void off();
    void blink();

  private:
    int pinNumber;
};

#include "led.h"

Led::Led(int _pinNumber):
  pinNumber {_pinNumber}
{
  //wiringPiSetup();
  pinMode(pinNumber, OUTPUT);
  off();
}

Led::~Led()
{
}

void Led::on()
{
  digitalWrite(pinNumber, HIGH);
}

void Led::off()
{
  digitalWrite(pinNumber, LOW);
}

void Led::blink()
{
  on();
  delay(500);
  off();
  delay(500);
}


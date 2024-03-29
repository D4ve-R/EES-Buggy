#include "led.h"

/**
 * Constructor for Led
 * @param int _pinNumber : GPIO pin number
 */
Led::Led(int _pinNumber):
  pinNumber {_pinNumber}
{
  //wiringPiSetup();
  pinMode(pinNumber, OUTPUT);
  off();
}

/**
 * @brief Destroy the Led:: Led object
 * 
 */
Led::~Led()
{
    off();
}

/**
 * turn led on
 */
void Led::on()
{
  digitalWrite(pinNumber, HIGH);
}

/**
 * turn led off
 */
void Led::off()
{
  digitalWrite(pinNumber, LOW);
}

/**
 * blick led once
 */
void Led::blink()
{
  on();
  delay(500);
  off();
  delay(500);
}


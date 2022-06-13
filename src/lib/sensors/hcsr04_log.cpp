#include "sensors/hcsr04.h"
#include "adafruit/util.h"

#include <string>

HCSR04_LOG::HCSR04_LOG(uint8_t _pinTrigger, uint8_t _pinEcho, bool _verbose):
    HCSR04  {_pinTrigger, _pinEcho},
    verbose {_verbose}
{

}

double HCSR04_LOG::measurement()
{
    double d = HCSR04::measurement(); 

    if(verbose)
        logger::output("Dist: " + std::to_string(d));

    return d;
}

void HCSR04_LOG::setVerbose(bool _verbose)
{
    verbose = _verbose;
}

bool HCSR04_LOG::getVerbose()
{
    return verbose;
}

#pragma once
#include "i2cdevice.h"

class PMW {
    PMW(int address);
    void setChannel();
    void setFrequency();

    private:
    I2CDevice device;
}

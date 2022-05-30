#include "i2cdevice.h"

#include <wiringPiI2C.h>
#include <string>

I2CDeviceWP::I2CDeviceWP (int i2cAddress)
    : address {i2cAddress}
    , handle {-1}
{
    handle = wiringPiI2CSetup(address);
}

I2CDeviceWP::~I2CDeviceWP()
{
}

void I2CDeviceWP::write8 (int deviceRegister, int data)
{
    if (isValid())
    {
        wiringPiI2CWriteReg8(handle, deviceRegister, data);
    }
}

int I2CDeviceWP::read8 (int deviceRegister)
{
    if (isValid())
    {
        return wiringPiI2CReadReg8(handle, deviceRegister);
    }
    return -1;
}

bool I2CDeviceWP::isValid(){
    return handle != -1;
}


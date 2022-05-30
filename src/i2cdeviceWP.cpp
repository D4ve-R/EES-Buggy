#include "i2cdevice.h"

#include <wiringPiI2C.h>

I2CDeviceWP::I2CDeviceWP (int i2cAddress)
    : address {i2cAddress}
    , handle {-1}
{
    handle = wiringPiI2CSetup(address);
}

I2CDeviceWP::~I2CDeviceWP()
{
}

void I2CDeviceWP::write(int data)
{
    if (isValid())
        wiringPiI2CWrite(handle, data);
}

void I2CDeviceWP::write8 (int deviceRegister, int data)
{
    if (isValid())
        wiringPiI2CWriteReg8(handle, deviceRegister, data);
}

void I2CDeviceWP::write16 (int deviceRegister, int data)
{
    if (isValid())
        wiringPiI2CWriteReg16(handle, deviceRegister, data);
}

int I2CDeviceWP::read(void)
{
    if (isValid())
        return wiringPiI2CRead();

    return -1;
}

int I2CDeviceWP::read8(int deviceRegister)
{
    if (isValid())
        return wiringPiI2CReadReg8(handle, deviceRegister);

    return -1;
}

int I2CDeviceWP::read16(int deviceRegister)
{
    if (isValid())
        return wiringPiI2CReadReg16(handle, deviceRegister);

    return -1;
}

bool I2CDeviceWP::isValid(){
    return handle != -1;
}


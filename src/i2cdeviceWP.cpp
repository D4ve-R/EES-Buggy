#include "i2cdevice.h"

#include <wiringPiI2C.h>

/**
 * Constructor for I2C device
 * int i2cAddress : i2c device address
 */
I2CDeviceWP::I2CDeviceWP (int i2cAddress)
    : address {i2cAddress}
    , handle {-1}
{
    handle = wiringPiI2CSetup(address);
}

I2CDeviceWP::~I2CDeviceWP()
{
}

/**
 * write byte to i2c device
 * int data : byte data to be written
 */
void I2CDeviceWP::write(int data)
{
    if (isValid())
        wiringPiI2CWrite(handle, data);
}

/**
 * write 1 byte to specific device-register on i2c device
 *  int deviceRegister : register address on i2c device
 *  int data : byte data to be written
 */
void I2CDeviceWP::write8 (int deviceRegister, int data)
{
    if (isValid())
        wiringPiI2CWriteReg8(handle, deviceRegister, (data & 0xFF));
}

/**
 * write 2 bytes to specific device-register on i2c device
 * int deviceRegister : register address on i2c device
 * int data : 2 bytes data to be written
 */
void I2CDeviceWP::write16 (int deviceRegister, int data)
{
    if (isValid())
        wiringPiI2CWriteReg16(handle, deviceRegister, (data & 0xFFFF));
}

/**
 * read 1 byte from i2c device 
 * returns data
 */
int I2CDeviceWP::read(void)
{
    if (isValid())
        return wiringPiI2CRead(handle);

    return -1;
}

/**
 * read 1 byte from specific device-register on i2c device
 * int deviceRegister : register address on i2c device
 * returns 1 byte to data or -1 if failed
 */
int I2CDeviceWP::read8(int deviceRegister)
{
    if (isValid())
        return wiringPiI2CReadReg8(handle, deviceRegister);

    return -1;
}

/**
 * read 2 byte from specific device-register on i2c device
 * int deviceRegister : register address on i2c device
 * returns 2 byte to data or -1 if failed
 */
int I2CDeviceWP::read16(int deviceRegister)
{
    if (isValid())
        return wiringPiI2CReadReg16(handle, deviceRegister);

    return -1;
}

/**
 * check if i2c device is usable
 */
bool I2CDeviceWP::isValid(){
    return handle != -1;
}


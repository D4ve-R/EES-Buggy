#include "i2cdeviceWP.h"
#include "adafruit/util.h"

#include <string>
#include <wiringPi.h>
#include <wiringPiI2C.h>

/**
 * Constructor for I2C device
 * int i2cAddress : i2c device address
 */
I2CDeviceWP::I2CDeviceWP (uint8_t i2cAddress)
    : address {i2cAddress}
    , handle {-1}
{
    handle = wiringPiI2CSetup(address);
    
    char s[6];
    sprintf(s, "%#04X", address);
    if(handle < 0)
        logger::error(std::string("Failed to open I2C device at ") + std::string(s));
    else
        logger::success(std::string("I2C device connected at ") + std::string(s));

    delay(5);
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
 * todo
 */
void I2CDeviceWP::write(uint8_t* buffer)
{
    if (isValid())
    {
        while(*buffer++ != '\0')
        {
            wiringPiI2CWrite(handle, *buffer);
        }
    }

}

/**
 * write 1 byte to specific device-register on i2c device
 *  int deviceRegister : register address on i2c device
 *  int data : byte data to be written
 */
void I2CDeviceWP::write8 (uint8_t deviceRegister, uint8_t data)
{
    if (isValid())
        wiringPiI2CWriteReg8(handle, deviceRegister, (data & 0xFF));
}

/**
 * write 2 bytes to specific device-register on i2c device
 * int deviceRegister : register address on i2c device
 * int data : 2 bytes data to be written
 */
void I2CDeviceWP::write16 (uint8_t deviceRegister, uint16_t data)
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
int I2CDeviceWP::read8(uint8_t deviceRegister)
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
int I2CDeviceWP::read16(uint8_t deviceRegister)
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


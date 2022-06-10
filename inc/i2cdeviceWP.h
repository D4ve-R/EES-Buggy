#pragma once

#include <cstdint>

/**
 * Represents an I2C device.
 * using wiringPiI2C lib
 */
class I2CDeviceWP
{
    const uint8_t address;
    int handle;

    public:

    I2CDeviceWP(uint8_t i2cAddress);
    ~I2CDeviceWP();

    void write(int data);
    void write(uint8_t* buffer);
    void write8(uint8_t deviceRegister, uint8_t data);
    void write16(uint8_t deviceRegister, uint16_t data);

    int read(void);
    int read8(uint8_t deviceRegister);
    int read16(uint8_t deviceRegister);

    bool isValid();

};

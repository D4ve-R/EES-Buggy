#pragma once

#include "i2cdevice.h"

#define GY521_I2C_ADDR      0x69U
#define GY521_PWR_MGMT_1    0x6BU

class GY521{
    GY521(int i2cAddress = GY521_I2C_ADDR);

    private:
    int acc_x, acc_y, acc_z;

    I2CDeviceWP device;
}

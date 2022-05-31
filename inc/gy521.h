#pragma once

#include "i2cdevice.h"

// default i2c address
#define GY521_I2C_ADDR      0x68U
//#define GY_521_I2C_ADDR   0x69U

// Register addresses
#define GY521_PWR_MGMT_1    0x6BU

/**
 * Represents a gy521 gyroskop
 * connected via I2C
 */
class GY521{
    GY521(int i2cAddress = GY521_I2C_ADDR);

    private:
    int acc_x, acc_y, acc_z;
    int gy_x, gy_y, gy_z;
    int temp;
    I2CDeviceWP device;
}

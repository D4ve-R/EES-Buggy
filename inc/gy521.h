#pragma once

#include "i2cdevice.h"

// default i2c address
#define GY521_I2C_ADDR      0x68
//#define GY_521_I2C_ADDR   0x69

// mp60X0 register addresses
#define GY521_PWR_MGMT_1    0x6BU
#define GY521_ACCEL_X       0x3BU
#define GY521_ACCEL_Y       0x3DU
#define GY521_ACCEL_Z       0x3FU
#define GY521_GYRO_X        0x43U
#define GY521_GYRO_Y        0x45U
#define GY521_GYRO_Z        0x47U
#define GY521_TEMP          0x41U

/**
 * Represents a gy521 on mp60X0
 * connected via I2C
 */
class GY521
{
    public:
    GY521(int i2cAddress = GY521_I2C_ADDR);
    ~GY521();

    void readAccel();
    void readGyro();
    void readTemp();

    private:
    int acc_x, acc_y, acc_z;
    int gy_x, gy_y, gy_z;
    int temp;
    I2CDeviceWP device;
};


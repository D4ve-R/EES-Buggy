#pragma once

#include <cstdint>
#include <math.h>

#include "i2cdevice.h"

constexpr double pi() { return std::atan(1)*4; }
constexpr double rad_to_deg() { return 180.0 / pi(); }

// default i2c address
#define GY521_I2C_ADDR      0x68
//#define GY_521_I2C_ADDR   0x69

// mp60X0 register addresses
#define GY521_PWR_MGMT_1    0x6BU
#define GY521_GYRO_CONFIG   0x1BU
#define GY521_ACCEL_CONFIG  0x1CU
#define GY521_INT_ENABLE    0x38U
#define GY521_INT_STATUS    0x3AU
#define GY521_ACCEL_X       0x3BU
#define GY521_ACCEL_Y       0x3DU
#define GY521_ACCEL_Z       0x3FU
#define GY521_GYRO_X        0x43U
#define GY521_GYRO_Y        0x45U
#define GY521_GYRO_Z        0x47U
#define GY521_TEMP          0x41U

namespace MPU6050 
{

}

/**
 * Represents a gy521 on mp60X0
 * connected via I2C
 */
class GY521
{
    enum CONFIG_REG
    {
        ACCEL = GY521_ACCEL_CONFIG,
        GYRO = GY521_GYRO_CONFIG
    };

    enum FS_SEL
    {
        _250 = 0,
        _500 = 1,
        _1000 = 2,
        _2000 = 3
    };

    enum AFS_SEL
    {
        _2G = 0,
        _4G = 1,
        _8G = 2,
        _12G = 3
    };

    float acc_x, acc_y, acc_z;
    float gy_x, gy_y, gy_z;
    float temp;
    
    float angle_x, angle_y, angle_z;

    uint32_t t, elapsedTime;

    I2CDeviceWP device;

    void configFullScaleRange(CONFIG_REG reg, uint8_t mode);

    void readAccel();
    void readGyro();
    void readTemp();

    public:
    GY521(uint8_t i2cAddress = GY521_I2C_ADDR);
    ~GY521();

    void readData();

    void configAccel(AFS_SEL mode = AFS_SEL::_2G);
    void configGyro(FS_SEL mode = FS_SEL::_250);
    void configTemp(bool on = true);
    void reset();

};


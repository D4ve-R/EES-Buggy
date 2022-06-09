#pragma once

#include <cstdint>
#include <math.h>

#include "i2cdevice.h"

constexpr double pi() { return std::atan(1)*4; }
constexp double rad_to_deg() { return 180.0 / pi(); }

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


/**
 * Represents a gy521 on mp60X0
 * connected via I2C
 */
class GY521
{
    struct enum CONFIG_REG
    {
        ACCEL = GY521_ACCEL_CONFIG,
        GYRO = GY521_GYRO_CONFIG
    }

    struct enum FS_SEL
    {
        _250 = 0,
        _500 = 1,
        _1000 = 2,
        _2000 = 3
    }

    struct enum AFS_SEL
    {
        2G = 0,
        4G = 1,
        8G = 2,
        12G = 3
    }

    int16_t acc_x, acc_y, acc_z;
    int16_t gy_x, gy_y, gy_z;
    int16_t temp;
    
    int angle_x, angle_y, angle_z;

    int time, elapsedTime;

    I2CDeviceWP device;

    void configFullScaleRange(CONFIG_REG reg, int mode);

    public:
    GY521(uint8_t i2cAddress = GY521_I2C_ADDR);
    ~GY521();

    void readAccel();
    void readGyro();
    void readTemp();
    void readData();

    void configAccel(AFS_SEL mode = AFS_SEL.2G);
    void configGyro(FS_SEL mode = FS_SEL._250);
    void configTemp(bool on = true);
    void reset();

};


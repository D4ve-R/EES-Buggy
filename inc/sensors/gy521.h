#pragma once

#include <cstdint>
#include <math.h>
#include <wiringPi.h>

#include "i2cdeviceWP.h"

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
#define GY521_WHOAMI        0x75U


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

    I2CDeviceWP device;
    bool initial;
    
    float acc_x, acc_y, acc_z;
    float gy_x, gy_y, gy_z;
    float temp;

    float acc_scale, gy_scale;
    float acc_x_off, acc_y_off, acc_z_off, gy_x_off, gy_y_off, gy_z_off;
    uint32_t t;
    float gy_angle_x, gy_angle_y;
    float angle_x, angle_y, angle_z;

    void configFullScaleRange(CONFIG_REG reg, uint8_t mode);
    void calcOffset();

    void readAccel();
    void readGyro();
    void readTemp();

    public:

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

    GY521(uint8_t i2cAddress = GY521_I2C_ADDR, AFS_SEL acc_mode = AFS_SEL::_2G, FS_SEL gy_mode = FS_SEL::_500);
    ~GY521();

    void configAccel(AFS_SEL mode);
    void configGyro(FS_SEL mode);
    void configTemp(bool on = true);
    void reset();
    void update();

    int getI2CAddr();

};

class GY521_LOG : public GY521
{
    bool verbose;

    void calcOffset();

    public:
    GY521_LOG(bool _verbose = true);
    ~GY521_LOG(){}

    void update();

}

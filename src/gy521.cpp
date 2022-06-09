#include "gy521.h"

#include <iostream>
#include <wiringPi.h>

/**
 * Constructor for gy521
 * uint8_t i2cAddress : i2c address of device
 */
GY521::GY521(uint8_t i2cAddress):
    device {i2cAddress},
    acc_x {0.0}, acc_y {0.0}, acc_z {0.0}, 
    gy_x {0.0}, gy_y {0.0}, gy_z{0.0},
    temp {0.0},
    angle_x {0.0}, angle_y {0.0}, angle_z {0.0}
{
    // disable sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x00);

    //configAccel();
    //configGyro();
    //configTemp();

    t = millis();
}

GY521::~GY521()
{
    // set sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x40);
}

void GY521::readAccel()
{
    acc_x = (float) (device.read8(GY521_ACCEL_X) << 8 | device.read8(GY521_ACCEL_X + 1)) / 16384.0;
    acc_y = (float) (device.read8(GY521_ACCEL_Y) << 8 | device.read8(GY521_ACCEL_Y + 1)) / 16384.0;
    acc_z = (float) (device.read8(GY521_ACCEL_Z) << 8 | device.read8(GY521_ACCEL_Z + 1)) / 16384.0;
}

void GY521::readGyro()
{
    int tmp = t;
    t = millis();
    elapsedTime = (t - tmp) / 1000;

    gy_x = (float) (device.read8(GY521_GYRO_X) << 8 | device.read8(GY521_GYRO_X + 1)) / 131.0;
    gy_y = (float) (device.read8(GY521_GYRO_Y) << 8 | device.read8(GY521_GYRO_Y + 1)) / 131.0;
    gy_z = (float) (device.read8(GY521_GYRO_Z) << 8 | device.read8(GY521_GYRO_Z + 1)) / 131.0;
}

void GY521::readTemp()
{
    temp = ((float) (device.read8(GY521_TEMP) << 8 | device.read8(GY521_TEMP + 1)) / 340.0) + 36.53;
}

void GY521::readData()
{
    readAccel();
    readGyro();
    readTemp();

    float acc_angle_x = atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2))) * rad_to_deg();
    float acc_angle_y = atan(-1 * acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2))) * rad_to_deg();

    float gy_angle_x = gy_x * elapsedTime;
    float gy_angle_y = gy_y * elapsedTime;

    angle_x = 0.98 * (angle_x + gy_angle_x) + 0.02 * acc_angle_x;
    angle_y = 0.98 * (angle_y + gy_angle_y) + 0.02 * acc_angle_y;

    std::cout << "AngleX: "<< angle_x << std::endl;
    std::cout << "AngleY: " << angle_y << std::endl;
    //std::cout << "Temp: " << temp << std::endl;
}

void GY521::configAccel(AFS_SEL mode)
{
    configFullScaleRange(CONFIG_REG::ACCEL, mode);
}
void GY521::configGyro(FS_SEL mode)
{
    configFullScaleRange(CONFIG_REG::GYRO, mode);
}

void GY521::configFullScaleRange(CONFIG_REG reg, uint8_t mode)
{
    device.write8(reg, mode << 3);
}

void GY521::configTemp(bool on)
{
    if(on)
    {    
        device.write8(GY521_PWR_MGMT_1, (device.read8(GY521_PWR_MGMT_1) & ~(1 << 3)));
        return;
    }

    device.write8(GY521_PWR_MGMT_1, (device.read8(GY521_PWR_MGMT_1) | (1 << 3)));
}

void GY521::reset()
{
    device.write8(GY521_PWR_MGMT_1, (device.read8(GY521_PWR_MGMT_1) | (1 << 7)));
}

/*
void GY521::readReg(int* data, int regAddr)
{
    int _data = device.read8(regAddr);
    _data = _data << 8;
    _data = _data | device.read8(regAddr + 1);
    data = _data;
}
*/


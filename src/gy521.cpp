#include "gy521.h"

#include <iostream>
#include <wiringPi.h>

/**
 * Constructor for gy521
 * uint8_t i2cAddress : i2c address of device
 */
GY521::GY521(uint8_t i2cAddress):
    device {i2cAddress},
    acc_x {0}, acc_y {0}, acc_z {0}, 
    gy_x {0}, gy_y {0}, gy_z{0},
    temp {0},
    angle_x {0}, angle_y {0}, angle_z {0}
{
    // disable sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x00);

    configAccel();
    configGyro();
    configTemp();
}

GY521::~GY521()
{
    // set sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x40);
}

void GY521::readAccel()
{
    acc_x = device.read16(GY521_ACCEL_X) / 16384;
    acc_y = device.read16(GY521_ACCEL_Y) / 16384;
    acc_z = device.read16(GY521_ACCEL_Z) / 16384;
}

void GY521::readGyro()
{
    int tmp = time;
    time = millis();
    elapsedTime = (time - tmp) / 1000

    gy_x = device.read16(GY521_GYRO_X) / 131;
    gy_y = device.read16(GY521_GYRO_Y) / 131;
    gy_z = device.read16(GY521_GYRO_Z) / 131;
}

void GY521::readTemp()
{
    temp = device.read16(GY521_TEMP) / 340 + 37;
}

void GY521::readData()
{

    readAccel();
    readGyro();
    readTemp();

    int acc_angle_x = atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2))) * rad_to_deg();
    int acc_angle_y = atan(-1 * acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2))) * rad_to_deg();

    int gy_angle_x = gy_x * elapsedTime;
    int gy_angle_y = gy_y * elapsedTime;

    angle_x = 0.98 * (angle_x + gy_angle_x) + 0.02 * acc_angle_x;
    angle_y = 0.98 * (angle_y + gy_angle_y) + 0.02 * acc_angle_y;

    std::cout << "AngleX: "<< angle_x << std::endl;
    std::cout << "AngleY: " << angle_y << std::endl;
    std::cout << "Temp: " << temp << std::endl;
}

void GY521::configAccel(AFS_SEL mode)
{
    configFullScaleRange(CONFIG_REG.ACCEL, mode);
}
void GY521::configGyro(FS_SEL mode);
{
    configFullScaleRange(CONFIG_REG.GYRO, mode);
}

void GY521::configFullScaleRange(CONFIG_REG reg, int mode)
{
    device.write8(deviceConfigRegister, mode << 3);
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


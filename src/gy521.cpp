#include "gy521.h"

#include <iostream>

/**
 * Constructor for gy521
 * uint8_t i2cAddress : i2c address of device
 */
GY521::GY521(uint8_t i2cAddress):
    device {i2cAddress},
    acc_x {0}, acc_y {0}, acc_z {0}, 
    gy_x {0}, gy_y {0}, gy_z{0},
    temp {0}
{
    // disable sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x00);
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

    /*
    readReg(&acc_x, GY521_ACCEL_X);
    readReg(&acc_y, GY521_ACCEL_Y);
    readReg(&acc_z, GY521_ACCEL_Z);
    */
}

void GY521::readGyro()
{
    gy_x = device.read16(GY521_GYRO_X) / 131;
    gy_y = device.read16(GY521_GYRO_Y) / 131;
    gy_z = device.read16(GY521_GYRO_Z) / 131;

    /*
    readReg(&gy_x, GY521_GYRO_X);
    readReg(&gy_y, GY521_GYRO_Y);
    readReg(&gy_z, GY521_GYRO_Z);
    */
}

void GY521::readTemp()
{
    temp = device.read16(GY521_TEMP) / 340 + 37;

    /*
    readReg(&temp, GY521_TEMP);
    */
}

void GY521::readData()
{
  readAccel();
  readGyro();
  readTemp();
  std::cout << "ACC: "<< acc_x << acc_y << acc_z << std::endl;
  std::cout << "GY: " << gy_x << gy_y << gy_z << std::endl;
  std::cout << "Temp: " << temp << std::endl;
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


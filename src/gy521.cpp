#include "gy521.h"

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
    device.write8(GY521_PWR_MGMT_1, 0x00);
}

GY521::~GY521()
{
}

void GY521::readAccel()
{
    acc_x = device.read16(GY521_ACCEL_X);
    acc_y = device.read16(GY521_ACCEL_Y);
    acc_z = device.read16(GY521_ACCEL_Z);

    /*
    readReg(&acc_x, GY521_ACCEL_X);
    readReg(&acc_y, GY521_ACCEL_Y);
    readReg(&acc_z, GY521_ACCEL_Z);
    */
}

void GY521::readGyro()
{
    gy_x = device.read16(GY521_GYRO_X);
    gy_y = device.read16(GY521_GYRO_Y);
    gy_z = device.read16(GY521_GYRO_Z);

    /*
    readReg(&gy_x, GY521_GYRO_X);
    readReg(&gy_y, GY521_GYRO_Y);
    readReg(&gy_z, GY521_GYRO_Z);
    */
}

void GY521::readTemp()
{
    temp = device.read16(GY521_TEMP);

    /*
    readReg(&temp, GY521_TEMP);
    */
}

void GY521::readData()
{
  readAccel();
  readGyro();
  readTemp();
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


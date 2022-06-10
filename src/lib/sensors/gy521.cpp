#include "sensors/gy521.h"
#include "adafruit/util.h"

#include <iostream>
#include <string>
#include <wiringPi.h>


/**
 * Constructor for gy521
 * uint8_t i2cAddress : i2c address of device
 */
GY521::GY521(uint8_t i2cAddress, AFS_SEL acc_mode, FS_SEL gy_mode, bool _verbose):
    device {i2cAddress},
    initial {true},
    acc_x {0.0}, acc_y {0.0}, acc_z {0.0}, 
    gy_x {0.0}, gy_y {0.0}, gy_z{0.0},
    temp {0.0},
    angle_x {0.0}, angle_y {0.0}, angle_z {0.0},
    acc_x_off {0.0}, acc_y_off {0.0}, acc_z_off {0.0},
    gy_x_off {0.0}, gy_y_off {0.0}, gy_z_off {0.0},
    gy_angle_x {0.0}, gy_angle_y {0.0},
    verbose {_verbose}
{
    // disable sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x00);

    configAccel(acc_mode);
    configGyro(gy_mode);
    configTemp();
    
    calcOffset();

    t = millis();
}

GY521::~GY521()
{
    reset();
    // set sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x40);
}

void GY521::readAccel()
{
    acc_x = ((device.read8(GY521_ACCEL_X) << 8 | device.read8(GY521_ACCEL_X + 1)) - acc_x_off) / acc_scale;
    acc_y = ((device.read8(GY521_ACCEL_Y) << 8 | device.read8(GY521_ACCEL_Y + 1)) - acc_y_off) / acc_scale;
    acc_z = ((device.read8(GY521_ACCEL_Z) << 8 | device.read8(GY521_ACCEL_Z + 1)) - acc_z_off) / acc_scale;
}

void GY521::readGyro()
{
    gy_x = ((device.read8(GY521_GYRO_X) << 8 | device.read8(GY521_GYRO_X + 1)) - gy_x_off) / gy_scale;
    gy_y = ((device.read8(GY521_GYRO_Y) << 8 | device.read8(GY521_GYRO_Y + 1)) - gy_y_off) / gy_scale;
    gy_z = ((device.read8(GY521_GYRO_Z) << 8 | device.read8(GY521_GYRO_Z + 1)) - gy_z_off) / gy_scale;
}

void GY521::readTemp()
{
    temp = (device.read8(GY521_TEMP) << 8 | device.read8(GY521_TEMP + 1)) / 340.0 + 36.53;
}

void GY521::update()
{
    readTemp();
    readAccel();
    
    //float neg = acc_z >= 0.0 ? 1.0 : -1.0;
    float acc_angle_x = atan(acc_y / sqrt(pow(acc_z, 2) + pow(acc_x, 2))) * rad_to_deg();
    float acc_angle_y = atan(-1 * acc_x /  sqrt(pow(acc_z, 2) + pow(acc_y, 2))) * rad_to_deg();

    readGyro();

    if(initial)
    {
        initial = false;
        t = millis();
    }

    uint32_t tmp = millis();
    float period = (tmp - t) / 1000.0;
    t = tmp;

    gy_angle_x += gy_x * period;
    gy_angle_y += gy_y * period;

    angle_x = 0.95 * gy_angle_x + 0.05 * acc_angle_x;
    angle_y = 0.95 * gy_angle_y + 0.05 * acc_angle_y;
    angle_z += gy_z * period;

    //std::cout << "Pitch: "<< angle_x << std::endl;
    //std::cout << "Roll: " << angle_y << std::endl;
    //std::cout << "Yaw: " << angle_z << std::endl;
    if(verbose)
    {
        logger::output("Temp: " + std::to_string(temp));
        logger::output("ACC_X: " + std::to_string(acc_x) + " ACC_Y: " + std::to_string(acc_y) + " ACC_Z: " + std::to_string(acc_z));
        logger::output("GY_X: " + std::to_string(gy_x) + " GY_Y: " + std::to_string(gy_y) + " GY_Z: " + std::to_string(gy_z));
    }
}


void GY521::configAccel(AFS_SEL mode)
{
    configFullScaleRange(CONFIG_REG::ACCEL, mode);
    
    switch(mode)
    {
        default:
        case AFS_SEL::_2G:
            acc_scale = 16384.0;
            break;
        case AFS_SEL::_4G:
            acc_scale = 8192.0;
            break;
        case AFS_SEL::_8G:
            acc_scale = 4096.0;
            break;
        case AFS_SEL::_12G:
            acc_scale = 2048.0;
            break;
    }
}
void GY521::configGyro(FS_SEL mode)
{
    configFullScaleRange(CONFIG_REG::GYRO, mode);

    switch(mode)
    {
        default:
        case FS_SEL::_250:
            gy_scale = 131.0;
            break;
        case FS_SEL::_500:
            gy_scale = 65.5;
            break;
        case FS_SEL::_1000:
            gy_scale = 32.8;
            break;
        case FS_SEL::_2000:
            gy_scale = 16.4;
            break;
    }
}

void GY521::configFullScaleRange(CONFIG_REG reg, uint8_t mode)
{
    device.write8(reg, (mode << 3));
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

void GY521::calcOffset()
{
    uint8_t cnt = 200;
    float sum[6] {0};
    
    uint8_t i = 0;
    for(; i < cnt; i++)
    {
        readAccel();
        readGyro();
        sum[0] += acc_x;
        sum[1] += acc_y;
        sum[2] += acc_z;
        sum[3] += gy_x;
        sum[4] += gy_y;
        sum[5] += gy_z;
        delay(1);
    }

    acc_x_off = sum[0] / cnt;
    acc_y_off = sum[1] / cnt;
    acc_z_off = sum[2] / cnt;
    gy_x_off  = sum[3] / cnt;
    gy_y_off  = sum[4] / cnt;
    gy_z_off  = sum[5] / cnt;

    if(verbose)
        for(i = 0; i < 6; i++)
        {
            char s[25]{'\0'};
            if(i < 3)
            {
                sprintf(s, "acc error %c : %4.2f \n", ((char)(88 + (i%3))), sum[i]);
                logger::ouput(s);
            }
            else
            {
                sprintf(s, "gyro error %c : %4.2f \n", ((char)(88 + (i%3))), sum[i]);
                logger::ouput(s);
            }
        }
}

int GY521::getI2CAddr()
{
    return device.read8(GY521_WHOAMI);
}

/*
void GY521::readReg(int* data, int regAddr)
{
    int _data = device.read8(regAddr);
    _data = _data << 8;
    _data = _data | device.read8(regAddr + 1);
    data = _data;
}

void GY521::readData()
{
    readTemp();
    readAccel();
    readGyro();

    float acc_angle_x = atan(acc_x / sqrt(pow(acc_y, 2) + pow(acc_z, 2))) * rad_to_deg();
    float acc_angle_y = atan(acc_y / sqrt(pow(acc_x, 2) + pow(acc_z, 2))) * rad_to_deg();
    float acc_angle_z = atan(sqrt(pow(acc_x, 2) + pow(acc_y, 2)) / acc_z);

    if(initial)
    {
        initial = false;

        gy_angle_x = acc_angle_x;
        gy_angle_y = acc_angle_y;
        gy_angle_z = acc_angle_z;
    }
    else
    {
        //gy_angle_x = gy_x * elapsedTime;
        //gy_angle_y = gy_y * elapsedTime;
        //gy_angle_z = gy_z * elapsedTime;
    }

    angle_x = 0.96 * (angle_x + gy_angle_x) + (0.04 * acc_angle_x);
    angle_y = 0.96 * (angle_y + gy_angle_y) + (0.04 * acc_angle_y);
    angle_z = 0.96 * (angle_z + gy_angle_z) + (0.04 * acc_angle_z);

    std::cout << "AngleX: "<< angle_x << std::endl;
    std::cout << "AngleY: " << angle_y << std::endl;
    //std::cout << "Temp: " << temp << std::endl;
}
*/


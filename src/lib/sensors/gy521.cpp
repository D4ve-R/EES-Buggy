#include "sensors/gy521.h"

/**
 * Constructor for gy521
 * @param uint8_t i2cAddress : i2c address of device
 */
GY521::GY521(uint8_t i2cAddress, AFS_SEL acc_mode, FS_SEL gy_mode):
    device {i2cAddress},
    initial {true},
    acc_x {0.0}, acc_y {0.0}, acc_z {0.0}, 
    gy_x {0.0}, gy_y {0.0}, gy_z{0.0},
    temp {0.0},
    angle_x {0.0}, angle_y {0.0}, angle_z {0.0},
    acc_x_off {0.0}, acc_y_off {0.0}, acc_z_off {0.0},
    gy_x_off {0.0}, gy_y_off {0.0}, gy_z_off {0.0},
    gy_angle_x {0.0}, gy_angle_y {0.0}, gy_angle_z {0.0}
{

    configAccel(acc_mode);
    configGyro(gy_mode);
    configTemp();
    

    // disable sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x00);

    delay(5);

    this->calcOffset();

    t = millis();
}

/**
 * Destroy the GY521::GY521 object
 * 
 */
GY521::~GY521()
{
    reset();
    // set sleep bit
    device.write8(GY521_PWR_MGMT_1, 0x40);
}

/**
 * read raw acceleration register x, y, z
 * correct with mean error, and divide by scale
 */
void GY521::readAccel()
{
    acc_x = (int16_t)(device.read8(GY521_ACCEL_X) << 8 | device.read8(GY521_ACCEL_X + 1));
    acc_y = (int16_t)(device.read8(GY521_ACCEL_Y) << 8 | device.read8(GY521_ACCEL_Y + 1));
    acc_z = (int16_t)(device.read8(GY521_ACCEL_Z) << 8 | device.read8(GY521_ACCEL_Z + 1));
}

/**
 * read gyroscope register x, y, z
 * correct with mean error and divide by scale
 */
void GY521::readGyro()
{
    gy_x = (int16_t)(device.read8(GY521_GYRO_X) << 8 | device.read8(GY521_GYRO_X + 1));
    gy_y = (int16_t)(device.read8(GY521_GYRO_Y) << 8 | device.read8(GY521_GYRO_Y + 1));
    gy_z = (int16_t)(device.read8(GY521_GYRO_Z) << 8 | device.read8(GY521_GYRO_Z + 1));
}

/**
 * read temperature register and scale value
 */
void GY521::readTemp()
{
    temp = (int16_t)(device.read8(GY521_TEMP) << 8 | device.read8(GY521_TEMP + 1)) / 340.0 + 36.53;
}

/**
 * updates calculated values from new data
 */
void GY521::update()
{
    readTemp();
    
    readAccel();
    acc_x = (acc_x - acc_x_off) / acc_scale;
    acc_y = (acc_y - acc_y_off) / acc_scale;
    acc_z = (acc_z - acc_z_off) / acc_scale;
    // acc_z is always 1 because of gravity
    acc_z -= 1.0;

    float acc_angle_x = atan(acc_y / sqrt(pow(acc_z, 2) + pow(acc_x, 2))) * rad_to_deg();
    float acc_angle_y = atan(-1 * acc_x /  sqrt(pow(acc_z, 2) + pow(acc_y, 2))) * rad_to_deg();

    if(initial)
    {
        initial = false;
        t = millis();
    }

    readGyro();
    gy_x = (gy_x - gy_x_off) / gy_scale;
    gy_y = (gy_y - gy_y_off) / gy_scale;
    gy_z = (gy_z - gy_z_off) / gy_scale;

    uint32_t tmp = millis();
    float period = (tmp - t) / 1000.0;
    t = tmp;

    gy_angle_x += std::abs(gy_x) > 0.1 ? gy_x * period : 0;
    gy_angle_y += std::abs(gy_y) > 0.1 ? gy_y * period : 0;
    gy_angle_z += std::abs(gy_z) > 0.1 ? gy_z * period : 0;

    angle_x = 0.95 * gy_angle_x + 0.05 * acc_angle_x;
    angle_y = 0.95 * gy_angle_y + 0.05 * acc_angle_y;
    angle_z += gy_z * period;

}

/**
 * set ACCEL_CONFIG register,
 * to change mode and set scale factors
 * @param mode AFS_SEL mode, see datasheet
 */
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

/**
 * set GYRO_CONFIG register,
 * to change mode and set scale factors
 * @param mode FS_SEL mode, see datasheet
 */
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

/**
 * set AFS or FS register to mode
 * @param reg to choose AFS or FS
 * @param mode 
 */
void GY521::configFullScaleRange(CONFIG_REG reg, uint8_t mode)
{
    device.write8(reg, (mode << 3));
}

/**
 * turn temeprature measurement on/off
 * @param on : turns on temp measurement
 */
void GY521::configTemp(bool on)
{
    if(on)
    {    
        device.write8(GY521_PWR_MGMT_1, (device.read8(GY521_PWR_MGMT_1) & ~(1 << 3)));
        return;
    }

    device.write8(GY521_PWR_MGMT_1, (device.read8(GY521_PWR_MGMT_1) | (1 << 3)));
}

/**
 * set reset bit in power managment register 1
 */
void GY521::reset()
{
    device.write8(GY521_PWR_MGMT_1, (device.read8(GY521_PWR_MGMT_1) | (1 << 7)));
}

/**
 * calculate mean offset error
 * !!! no movement or vibration while calculating !!!
 */
void GY521::calcOffset()
{
    uint8_t cnt = 200;
    float sum[6] {0};
    
    uint8_t i = 0;
    for(; i < cnt; i++)
    {
        readAccel();
        sum[0] += acc_x;
        sum[1] += acc_y;
        sum[2] += acc_z;

        readGyro();
        sum[3] += gy_x;
        sum[4] += gy_y;
        sum[5] += gy_z;
        delay(1);
    }

    acc_x_off = sum[0] / (float)cnt;
    acc_y_off = sum[1] / (float)cnt;
    acc_z_off = sum[2] / (float)cnt;
    gy_x_off  = sum[3] / (float)cnt;
    gy_y_off  = sum[4] / (float)cnt;
    gy_z_off  = sum[5] / (float)cnt;
}

/**
 * read who-am-i register
 * @return int : i2c address from who-am-i register
 */
int GY521::getI2CAddr()
{
    return device.read8(GY521_WHOAMI);
}

float GY521::getZRotation()
{
    update();
    return gy_angle_z;
}

void GY521::setZRotation(int8_t deg)
{
    gy_angle_z = deg;
}

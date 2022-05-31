#include "gy521.h"

/**
 * Constructor for gy521
 * uint8_t i2cAddress : i2c address of device
 */
GY521::GY521(uint8_t i2cAddress):
    device {i2cAddress},
    acc_x {0}, acc_y {0}, acc_z {0}, 
    gy_x {0}, gy_y {0}, gy_z{0},
    temp {0},
{
    device.write8(GY521_PWR_MGMT_1, 0);
}



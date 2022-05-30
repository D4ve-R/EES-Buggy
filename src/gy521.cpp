#include "gy521.h"

GY521::GY521(uint8_t i2cAddress):
    device {i2cAddress},
{
    device.write8(GY521_PWR_MGMT_1, 0);
}

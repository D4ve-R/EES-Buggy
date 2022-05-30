#pragma once

class GY521{
    GY521(uint8_t i2cAddress = 0x69);

    private:
    uint8_t address;
}

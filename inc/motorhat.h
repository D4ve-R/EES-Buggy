#pragma once

// address can range from 0x60 to 0x80
// default is 0x60
#define MOTORHAT_ADDR   0x60

/**
 * This class implements the motorhat in software
 */
class Motorhat {
    Motorhat(int address = MOTORHAT_ADDR, int freq = 1600);
    ~Motorhat();

    private:

}

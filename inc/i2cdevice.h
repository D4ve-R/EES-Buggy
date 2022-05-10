#pragma once

// uncomment according to your rev version
// rpi rev 1
//#define I2C_BUS_NO 0
// rpi rev 2
#define I2C_BUS_NO 1

/**
 * This class implements an i2c device driver in userspace,
 * by handling access to i2c device file /dev/i2c-X
 * 
 */
class I2CDevice {
    I2CDevice(int address);
    ~I2cDevice();

    void openRef();
    void closeRef();
    void writeData(int address, int data);
    int readData(int address);
    bool isReady();

    private:
    // i2c address
    int deviceAddress;
    // i2c bus number
    int busNumber;
    // file ref to /dev/i2c-X
    int ref;
}

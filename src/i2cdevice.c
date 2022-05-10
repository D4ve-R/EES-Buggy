#include <i2cdevice.h>

#include <linux/i2c-dev.h>  // I2C_SLAVE
#include <unistd.h>         // read(), write()
#include <sys/ioctl.h>      // ioctl()
#include <fcntl.h>          // open(), O_RDWR = 2

// for readability
#define byte char

/**
 * Constructor method
 * 
 */
I2CDevice::I2CDevice(int address):
    deviceAddress {address},
    ref {-1},
{
    openRef();
}

I2CDevice::~I2CDevice()
{
    closeRef();
}

/**
 *  open /dev/i2c-X
 *  X = I2C Bus Number
 */
void I2CDevice::openRef(){
    closeRef();

    std::string filename = "/dev/i2c-" + std::to_string(I2C_BUS_NO);
    
    // https://man7.org/linux/man-pages/man2/ioctl.2.html#NOTES
    ref = open(filename, O_RDWR | O_NONBLOCK);
    if(ref < 0){
        // error couldnt open file
        exit(1);
    }
}

/**
 *  close /dev/i2c-X
 */
void I2C::closeRef(){
    if(ref >= 0){
        if(close(ref) < 0){
            // error couldnt close file
            exit(1);
        }
    }
}

/**
 *  select device to communicate
 *  change i2c slave address to deviceAddress
 */
void I2CDevice::selectDevice(){
    // deviceAddress must be <= 127, max. i2c clients
    if (ioctl(ref, I2C_SLAVE, deviceAddress & 0x7F) < 0) {
        // error couldnt manipulate file
        exit(1);
    }
}

/**
 *  write 1 byte of data to device
 *  int devAddress : slave address to communicate with
 *  int data : data to be send
 */
void I2CDevice::writeData(int devAddress, int data){
    if(ref < 0)
        return;
    else{
        selectDevice();
        byte buffer[2];
        buffer[0] = (byte) (devAddress & 0xFF);
        buffer[1] = (byte) (data & 0xFF);
        if(write(ref, buffer, 2) != 2){
            // error couldnt write to device
        }
    }
}

/**
 *  read 1 byte of data from device
 *  int devAddress : device address to communicate with
 *  return 1 byte data from device
 */
int I2CDevice::readData(int devAddress){
   if(ref < 0)
       return ref;
   else{
        selectDevice();
        byte buffer[1];
        if(read(ref, &buffer, 1) != 1){
            // error couldnt read device
            return -1;
        }
        else
            return (int)buffer[0]
   }
}

/**
 *  check if device is ready
 */
bool isReady(){
    return ref != -1;
}

#include "sensors/gy521.h"
#include "adafruit/util.h"

#include <iostream>
#include <string>

GY521_LOG::GY521_LOG(bool _verbose):
    GY521 {},
    verbose {_verbose}
{
    if(verbose)
    {
        logger::output("MPU6050");
        char s[25]{'\0'};
        sprintf(s, "WHOAMI : %#04X", getI2CAddr());
        logger::output(s);

        sprintf(s, "acc error x: %4.2f", acc_x_off);
        logger::output(s);
        sprintf(s, "acc error y: %4.2f", acc_y_off);
        logger::output(s);
        sprintf(s, "acc error z: %4.2f", acc_z_off);
        logger::output(s);
        sprintf(s, "gy error x: %4.2f", gy_x_off);
        logger::output(s);
        sprintf(s, "gy error y: %4.2f", gy_y_off);
        logger::output(s);
        sprintf(s, "gy error z: %4.2f", gy_z_off);
        logger::output(s);
    } 
}

void GY521_LOG::update()
{
    GY521::update();

    //std::cout << "Pitch: "<< angle_x << std::endl;
    //std::cout << "Roll: " << angle_y << std::endl;
    //std::cout << "Yaw: " << angle_z << std::endl;
    if(verbose)
    {
//        logger::output("Temp: " + std::to_string(temp));
//        logger::output("ACC_X: " + std::to_string(acc_x) + " ACC_Y: " + std::to_string(acc_y) + " ACC_Z: " + std::to_string(acc_z));
        logger::output("GY_X: " + std::to_string(gy_x) + " GY_Y: " + std::to_string(gy_y) + " GY_Z: " + std::to_string(gy_z));
        logger::output("gy_angle_x: " + std::to_string(gy_angle_x) + " gy_angle_y: " + std::to_string(gy_angle_y) + " gy_angle_z: " + std::to_string(gy_angle_z));
    }
}

void GY521_LOG::calcOffset()
{
    GY521::calcOffset();

    if(verbose)
    {
        char s[25]{'\0'};
        sprintf(s, "acc error x: %4.2f", acc_x_off);
        logger::output(s);
        sprintf(s, "acc error x: %4.2f", acc_y_off);
        logger::output(s);
        sprintf(s, "acc error x: %4.2f", acc_z_off);
        logger::output(s);
        sprintf(s, "gy error x: %4.2f", gy_x_off);
        logger::output(s);
        sprintf(s, "gy error y: %4.2f", gy_y_off);
        logger::output(s);
        sprintf(s, "gy error z: %4.2f", gy_z_off);
        logger::output(s);
    } 
}

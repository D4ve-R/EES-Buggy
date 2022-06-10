#include "sensors/gy521.h"
#include "adafruit/util.h"

#include <iostream>
#include <string>

GY521_LOG::GY521_LOG(bool _verbose):
    GY521 {},
    verbose {_verbose}
{

}

void GY521_LOG::update()
{
    GY521::update();

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

void GY521_LOG::calcOffset()
{
    GY521::calcOffset();

    if(verbose)
        for(i = 0; i < 6; i++)
        {
            char s[25]{'\0'};
            if(i < 3)
            {
                sprintf(s, "acc error %c : %4.2f \n", ((char)(88 + (i%3))), sum[i]);
                logger::output(s);
            }
            else
            {
                sprintf(s, "gyro error %c : %4.2f \n", ((char)(88 + (i%3))), sum[i]);
                logger::output(s);
            }
        }
}

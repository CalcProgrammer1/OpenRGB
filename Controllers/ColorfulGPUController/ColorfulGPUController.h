#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

#pragma once

typedef unsigned char colorful_gpu_dev_id;

#define COLORFUL_PACKET_LENGTH 11

class ColorfulGPUController
{
public:
    ColorfulGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev);
    ~ColorfulGPUController();

    std::string GetDeviceLocation();
    void        SetDirect(RGBColor color);


private:
    i2c_smbus_interface *   bus;
    colorful_gpu_dev_id     dev;
};

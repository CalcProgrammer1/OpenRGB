#include <string>
#include "i2c_smbus.h"
#include "RGBController.h"

#pragma once

typedef unsigned char colorful_gpu_dev_id;

#define COLORFUL_PACKET_LENGTH 8

class ColorfulTuringGPUController
{
public:
    ColorfulTuringGPUController(i2c_smbus_interface* bus, colorful_gpu_dev_id dev);
    ~ColorfulTuringGPUController();

    std::string GetDeviceLocation();
    void        SetDirect(RGBColor color);


private:
    i2c_smbus_interface *   bus;
    colorful_gpu_dev_id     dev;
};

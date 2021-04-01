/*-----------------------------------------*\
|  GainwardGPUController.h                  |
|                                           |
|  Driver for Gainward RGB v1 on GPUs       |
|                                           |
|  TheRogueZeta 11/05/2020                  |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char gainward_gpu_dev_id;

enum
{
    /* RGB Registers */
    GAINWARD_RED_REGISTER                       = 0x03, /* Red Register */
    GAINWARD_GREEN_REGISTER                     = 0x04, /* Green Register */
    GAINWARD_BLUE_REGISTER                      = 0x05, /* Blue Register */
    GAINWARD_06_REGISTER                        = 0x06, /* Unknown (Brightness/Mode?) Register */
};

class GainwardGPUv1Controller
{
public:
    GainwardGPUv1Controller(i2c_smbus_interface* bus, gainward_gpu_dev_id);
    ~GainwardGPUv1Controller();

    std::string   GetDeviceLocation();
    unsigned char GetLEDRed();
    unsigned char GetLEDGreen();
    unsigned char GetLEDBlue();
    void          SetLEDColors(unsigned char red, unsigned char green, unsigned char blue);
    void          SetMode();

    unsigned char GainwardGPURegisterRead(unsigned char reg);
    void          GainwardGPURegisterWrite(unsigned char reg, unsigned char val);

private:
    i2c_smbus_interface *   bus;
    gainward_gpu_dev_id     dev;
};

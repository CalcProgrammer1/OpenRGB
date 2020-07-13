/*-----------------------------------------*\
|  GalaxGPUController.h                     |
|                                           |
|  Driver for Galax / KFA2 RGB on GPUs      |
|                                           |
|  Niels Westphal (crashniels)  12.07.2020  |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"

#pragma once

typedef unsigned char galax_gpu_dev_id;

enum
{
    GALAX_RED_REGISTER                      = 0x02, /* Red Register */
    GALAX_GREEN_REGISTER                    = 0x03, /* Green Register */
    GALAX_BLUE_REGISTER                     = 0x04, /* Blue Register */
    //GALAX_MODE_REGISTER                     = 0x07, /* Mode Register */
};

enum
{
    //GALAX_MODE_STATIC                       = 0x04,
};

class GalaxGPUController
{
public:
    GalaxGPUController(i2c_smbus_interface* bus, galax_gpu_dev_id);
    ~GalaxGPUController();

    std::string   GetDeviceName();
    std::string   GetDeviceLocation();
    unsigned char GetLEDRed();
    unsigned char GetLEDGreen();
    unsigned char GetLEDBlue();
    void          SetLEDColorsDirect(unsigned char red, unsigned char green, unsigned char blue);
    void          SetLEDColorsEffect(unsigned char red, unsigned char green, unsigned char blue);
    void          SetMode(unsigned char mode);

    unsigned char GalaxGPURegisterRead(unsigned char reg);
    void          GalaxGPURegisterWrite(unsigned char reg, unsigned char val);

    bool          direct = false;                                                // Temporary solution to check if we are in "Direct" mode

private:
    char                    device_name[16];
    i2c_smbus_interface *   bus;
    galax_gpu_dev_id        dev;
};

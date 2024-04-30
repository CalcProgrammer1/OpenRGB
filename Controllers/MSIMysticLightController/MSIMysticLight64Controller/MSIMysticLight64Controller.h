/*-----------------------------------------*\
|  MSIMysticLight64Controller.h             |
|                                           |
|  Definitions and types for MSI Mystic     |
|  Light (64-byte) USB lighting controllers |
|                                           |
|  T-bond 3/4/2020                          |
|  Adam Honse 3/6/2021                      |
\*-----------------------------------------*/

#include "MSIMysticLightCommon.h"
#include "RGBController.h"
#include <cstring>
#include <hidapi/hidapi.h>
#include <limits>

#pragma once

enum MSI_64_MODE
{
    MSI_64_OFF             = 0,
    MSI_64_STEADY          = 1,
    MSI_64_BREATHING       = 2,
    MSI_64_PULSE           = 3,
    MSI_64_DOUBLE_PULSE    = 4,
    MSI_64_CYCLE           = 5,
    MSI_64_SMOOTH_CYCLE    = 6,
};

class MSIMysticLight64Controller
{
public:
    MSIMysticLight64Controller
        (
        hid_device*     handle,
        const char      *path
        );
    ~MSIMysticLight64Controller();

    void            SetMode
                        (
                        MSI_64_MODE        mode,
                        MSI_SPEED       speed,
                        MSI_BRIGHTNESS  brightness,
                        unsigned int num_colors, 
                        Color colors[]
                        );


    std::string     GetDeviceName();
    std::string     GetDeviceLocation();
    std::string     GetFWVersion();
    std::string     GetSerial();

private:

    hid_device*                     dev;
    std::string                     location;
};

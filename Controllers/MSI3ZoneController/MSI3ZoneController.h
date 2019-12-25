/*-----------------------------------------*\
|  MSI3ZoneController.h                     |
|                                           |
|  Definitions and types for MSI/Steelseries|
|  3-Zone Keyboard lighting controller      |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/25/2019  |
\*-----------------------------------------*/

#include "RGBController.h"

#include <string>
#include <libusb-1.0/libusb.h>

#pragma once

class MSI3ZoneController
{
public:
    MSI3ZoneController(libusb_device_handle* dev_handle);
    ~MSI3ZoneController();

    char* GetDeviceName();

    void SetLEDs(std::vector<RGBColor> colors);
    
private:
    char                    device_name[32];
    libusb_device_handle*   dev;
};

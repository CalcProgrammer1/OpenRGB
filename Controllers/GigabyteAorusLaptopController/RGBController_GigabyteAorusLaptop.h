/*-----------------------------------------*\
|  RGBController_GigabyteAorusLaptop.h      |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Gigabyte Aorus Laptop USB Driver         |
|                                           |
|  Guimard Morgan (morg) 06/05/2023         |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "GigabyteAorusLaptopController.h"

enum GIGABYTE_AORUS_LAPTOP_DEV_TYPE
{
    GIGABYTE_AORUS_LAPTOP_KEYBOARD_TYPE,
    GIGABYTE_AORUS_LAPTOP_BACKLIGHT_TYPE
};

class RGBController_GigabyteAorusLaptop : public RGBController
{
public:
    RGBController_GigabyteAorusLaptop(GigabyteAorusLaptopController* controller_ptr, GIGABYTE_AORUS_LAPTOP_DEV_TYPE dev_type);
    ~RGBController_GigabyteAorusLaptop();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    GigabyteAorusLaptopController* controller;    
    GIGABYTE_AORUS_LAPTOP_DEV_TYPE dev_type;
};

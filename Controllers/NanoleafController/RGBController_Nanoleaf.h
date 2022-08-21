/*-----------------------------------------*\
|  RGBController_Nanoleaf.h                 |
|                                           |
|  Generic RGB Interface for Nanoleaf       |
|                                           |
|  Nikita Rushmanov 01/13/2022              |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "NanoleafController.h"

class RGBController_Nanoleaf : public RGBController
{

public:
    RGBController_Nanoleaf(std::string a_address, int a_port, std::string a_auth_token);

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    NanoleafController controller;
};

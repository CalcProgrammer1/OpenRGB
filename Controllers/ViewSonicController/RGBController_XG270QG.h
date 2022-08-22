/*------------------------------------*\
|  RGBController_XG270QG.h             |
|  Lanzaa 1/23/2022                    |
\*------------------------------------*/

#pragma once

#include <cstdint>
#include <vector>

#include "VS_XG270QG_Controller.h"
#include "RGBController.h"

class RGBController_XG270QG : public RGBController
{
public:
    RGBController_XG270QG(VS_XG270QG_Controller* controller_ptr);

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    VS_XG270QG_Controller* controller;
};

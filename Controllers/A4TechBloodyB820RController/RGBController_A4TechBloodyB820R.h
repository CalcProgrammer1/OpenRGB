/*-------------------------------------------------------------------*\
|  RGBController_A4TechBloodyB820R.h                                |
|                                                                     |
|  Driver for A4TechBloodyB820R Keyboard Controller                   |
|                                                                     |
|  Zulfikar (o-julfikar)          28 Mar 2024                         |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "LogManager.h"
#include "RGBController.h"
#include "A4TechBloodyB820RController.h"
#include <vector>

class RGBController_A4TechBloodyB820R : public RGBController
{
public:
    RGBController_A4TechBloodyB820R(A4TechBloodyB820RController* controller_ptr);
    ~RGBController_A4TechBloodyB820R();

    void    SetupZones();
    void    ResizeZone(int zone, int new_size);

    void    DeviceUpdateLEDs();
    void    UpdateZoneLEDs(int zone);
    void    UpdateSingleLED(int led);

    void    DeviceUpdateMode();

private:
    A4TechBloodyB820RController* controller;
};

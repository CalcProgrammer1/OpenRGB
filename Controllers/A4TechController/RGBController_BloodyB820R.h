/*-------------------------------------------------------------------*\
|  RGBController_BloodyB820R.h                                |
|                                                                     |
|  Driver for A4Tech Bloody B820R Keyboard Controller                   |
|                                                                     |
|  Mohammed Julfikar Ali Mahbub (o-julfikar)          01 Apr 2024     |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "LogManager.h"
#include "RGBController.h"
#include "BloodyB820RController.h"
#include <vector>

class RGBController_BloodyB820R : public RGBController
{
public:
    RGBController_BloodyB820R(BloodyB820RController* controller_ptr);
    ~RGBController_BloodyB820R();

    void    SetupZones();
    void    ResizeZone(int zone, int new_size);

    void    DeviceUpdateLEDs();
    void    UpdateZoneLEDs(int zone);
    void    UpdateSingleLED(int led);

    void    DeviceUpdateMode();

private:
    BloodyB820RController* controller;
};

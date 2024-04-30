/*-----------------------------------------*\
|  RGBController_RoccatHordeAimo.h          |
|                                           |
|  Generic RGB Interface for Roccat Horde   |
|  Aimo Keyboard controller                 |
|                                           |
|  Morgan Guimard (morg)     2/24/2022      |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatHordeAimoController.h"

class RGBController_RoccatHordeAimo : public RGBController
{
public:
    RGBController_RoccatHordeAimo(RoccatHordeAimoController* controller_ptr);
    ~RGBController_RoccatHordeAimo();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    RoccatHordeAimoController* controller;
};

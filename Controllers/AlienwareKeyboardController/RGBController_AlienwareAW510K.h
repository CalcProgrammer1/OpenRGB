/*-----------------------------------------*\
|  RGBController_AlienwareAW510K.h          |
|                                           |
|  Generic RGB Interface for Alienware      |
|  AW510K keyboard                          |
|                                           |
|  Mohamad Sallal - msallal      05/22/2021 |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "AlienwareAW510KController.h"

class RGBController_AlienwareAW510K : public RGBController
{
public:
    RGBController_AlienwareAW510K(AlienwareAW510KController* controller_ptr);
    ~RGBController_AlienwareAW510K();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AlienwareAW510KController*  controller;
    std::vector<RGBColor>       current_colors;
};

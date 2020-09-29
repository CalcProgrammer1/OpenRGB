/*-----------------------------------------*\
|  RGBController_RedragonK556.h             |
|                                           |
|  Generic RGB Interface for Redragon K556  |
|  Devarajas RGB Keyboard                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/25/2020   |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RedragonK556Controller.h"

class RGBController_RedragonK556 : public RGBController
{
public:
    RGBController_RedragonK556(RedragonK556Controller* redragon_ptr);
    ~RGBController_RedragonK556();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);
    
    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RedragonK556Controller*   redragon;
};

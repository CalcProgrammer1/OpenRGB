/*-----------------------------------------*\
|  RGBController_SteelSeriesQCKMat.h        |
|                                           |
|  Generic RGB Interface SteelSeriesQCK     |
|  Class                                    |
|                                           |
|  Edbgon 22/05/2021                        |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesQCKMatController.h"

class RGBController_SteelSeriesQCKMat : public RGBController
{
public:
    RGBController_SteelSeriesQCKMat(SteelSeriesQCKMatController* qck_ptr);
    ~RGBController_SteelSeriesQCKMat();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    SteelSeriesQCKMatController*     qck;
};

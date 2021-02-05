/*-----------------------------------------*\
|  RGBController_SteelSeriesOldApex.h       |
|                                           |
|  Generic RGB Interface older-style        |
|  SteelSeriesApex Keyboards                |
|  (APEX/APEX Fnatic/Apex 350)              |
|  Class                                    |
|                                           |
|  David Lee (RAMChYLD) 15/11/2020          |
|                                           |
|  Based on work by                         |
|  B Horn (bahorn) 13/05/2020               |
|                                           |
|  Based on findings in ApexCtl by          |
|  Audrius/tuxmark5, et. al                 |
|  https://github.com/tuxmark5/ApexCtl      |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SteelSeriesOldApexController.h"
#include "color32.h"

class RGBController_SteelSeriesOldApex : public RGBController
{
public:
    RGBController_SteelSeriesOldApex(SteelSeriesOldApexController* old_apex_ptr);
    ~RGBController_SteelSeriesOldApex();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    SteelSeriesOldApexController*     OldApex;
};

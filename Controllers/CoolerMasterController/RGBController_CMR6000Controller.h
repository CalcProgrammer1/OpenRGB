/*-------------------------------------------------------------------*\
|  RGBController_CMR6000Controller.h                                  |
|                                                                     |
|  Driver for Coolermaster based AMD Radeon GPU (6000 series)         |
|                                                                     |
|  Eric S (edbgon)          2nd Feb 2021                              |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "CMR6000Controller.h"

class RGBController_CMR6000Controller : public RGBController
{
public:
    RGBController_CMR6000Controller(CMR6000Controller* cmmp_ptr);
    ~RGBController_CMR6000Controller();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    CMR6000Controller* cmr6000;

    int         GetDeviceMode();
};

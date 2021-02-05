/*--------------------------------------------------------------*\
|  RGBController_HoltekA070.h                                    |
|                                                                |
|  Generic RGB Interface for Holtek USB Gaming Mouse [04d9:a070] |
|                                                                |
|  Santeri Pikarinen (santeri3700) 8/01/2020                     |
\*--------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "HoltekA070Controller.h"

class RGBController_HoltekA070 : public RGBController
{
public:
    RGBController_HoltekA070(HoltekA070Controller* holtek_ptr);
    ~RGBController_HoltekA070();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    HoltekA070Controller*   holtek;
};

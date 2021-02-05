/*-----------------------------------------*\
|  RGBController_Sinowealth.h               |
|                                           |
|  Definitions and types for Sinowealth     |
|  mice, including Glorious                 |
|                                           |
|  Niels Westphal (crashniels) 20/5/2020    |
\*-----------------------------------------*/

#pragma once

#include "RGBController.h"
#include "SinowealthController.h"

class RGBController_Sinowealth : public RGBController
{
public:
    RGBController_Sinowealth(SinowealthController* sinowealth_ptr);
    ~RGBController_Sinowealth();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();

    void        DeviceUpdateMode();


private:
    SinowealthController* sinowealth;
};

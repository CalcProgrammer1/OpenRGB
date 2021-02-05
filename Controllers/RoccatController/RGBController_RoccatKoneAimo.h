/*-----------------------------------------*\
|  RGBController_RoccatKoneAimo.h           |
|                                           |
|  Generic RGB Interface for Roccat Kone    |
|  Aimo  controller                         |
|                                           |
|  Thibaud M (enlight3d)     17/11/2020     |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "RoccatKoneAimoController.h"

class RGBController_RoccatKoneAimo : public RGBController
{
public:
    RGBController_RoccatKoneAimo(RoccatKoneAimoController* mouse_ptr);
    ~RGBController_RoccatKoneAimo();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

private:
    RoccatKoneAimoController*                   mouse;
    std::vector<ROCCAT_KONE_AIMO_CHANNEL>       zones_channel;
    std::vector<ROCCAT_KONE_AIMO_CHANNEL>       leds_channel;
};

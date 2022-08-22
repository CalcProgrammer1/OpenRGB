/*-------------------------------------------------------------------*\
|  RGBController_Tecknet.h                                            |
|                                                                     |
|  Driver for Tecknet Devices                                         |
|                                                                     |
|  Chris M (Dr_No)          29th Jul 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#ifndef RGBCONTROLLER_TECKNET_H
#define RGBCONTROLLER_TECKNET_H

#include "RGBController.h"
#include "TecknetController.h"

class RGBController_Tecknet : public RGBController
{
public:
    RGBController_Tecknet(TecknetController* controller_ptr);
    ~RGBController_Tecknet();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    TecknetController* controller;
};

#endif // RGBCONTROLLER_TECKNET_H

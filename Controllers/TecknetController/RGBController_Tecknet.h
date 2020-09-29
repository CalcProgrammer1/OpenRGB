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
    RGBController_Tecknet(TecknetController *_dev);
    ~RGBController_Tecknet();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    TecknetController* Tecknet_dev;
};

#endif // RGBCONTROLLER_TECKNET_H

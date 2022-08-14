/*-------------------------------------------------------------------*\
|  RGBController_LenovoUSB.h                                          |
|                                                                     |
|  interface for various Lenovo Legion Devices                        |
|                                                                     |
|  Cooper Hall (geobot19)          17 Apr 2022                        |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "LenovoDevices.h"
#include "LenovoUSBController.h"
#include "RGBController.h"

#include <vector>

#ifndef RGBCONTROLLER_LENOVOUSB_H
#define RGBCONTROLLER_LENOVOUSB_H

#define NA 0xFFFFFFFF

class RGBController_LenovoUSB : public RGBController
{
public:
    RGBController_LenovoUSB(LenovoUSBController* controller_ptr);
    ~RGBController_LenovoUSB();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    unsigned int            lenovo_size_of_leds;
    LenovoUSBController*    controller;
    const lenovo_led*       lenovo_leds;
};

#endif

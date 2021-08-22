/*-----------------------------------------*\
|  RGBController_CorsairK100.cpp            |
|                                           |
|  Driver for Corsair K100 Keyboard         |
\*-----------------------------------------*/

#ifndef RGBCONTROLLER_CORSAIRK100_H
#define RGBCONTROLLER_CORSAIRK100_H

#include "RGBController.h"
#include "CorsairK100Controller.h"

class RGBController_CorsairK100 : public RGBController
{
public:
    RGBController_CorsairK100(CorsairK100Controller* corsair_ptr);
    ~RGBController_CorsairK100();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();
    void SetCustomMode();

private:
    CorsairK100Controller*  corsair;
    CorsairKeyboardType     logical_layout;
};

#endif // RGBCONTROLLER_CORSAIRK100_H

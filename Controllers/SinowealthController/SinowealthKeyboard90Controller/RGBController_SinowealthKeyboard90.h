/*------------------------------------------*\
|  RGBController_SinowealthKeyboard90.h      |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard with PID:0090,                   |
|  made spefically for Genesis Thor 300      |
|                                            |
|  Jan Baier 30/06/2022                      |
\*-----------------------------------------=*/

#pragma once
#include "RGBController.h"
#include "SinowealthKeyboard90Controller.h"

class RGBController_SinowealthKeyboard90 : public RGBController
{
public:
    RGBController_SinowealthKeyboard90(SinowealthKeyboard90Controller* controller_ptr);
    ~RGBController_SinowealthKeyboard90();

    void            SetupZones();

    void            ResizeZone(int zone, int new_size);

    void            DeviceUpdateLEDs();
    void            UpdateZoneLEDs(int zone);
    void            UpdateSingleLED(int led);

    void            DeviceUpdateMode();

private:
    SinowealthKeyboard90Controller* controller;

    void            AddMode(std::string name, unsigned char value, bool color_support);
    unsigned char   MapRGBToColorEnum(RGBColor color);
};

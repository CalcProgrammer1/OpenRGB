/*------------------------------------------*\
|  RGBController_SinowealthKeyboard16.h      |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard with PID:0016,                   |
|  Hopefully generic for this PID,           |
|  this was made spefically for ZUOYA X51    |
|                                            |
|  Zagorodnikov Aleksey (glooom) 26.07.2021  |
|  based on initial implementation from      |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#pragma once

#include "RGBController.h"
#include "SinowealthKeyboard16Controller.h"

class RGBController_SinowealthKeyboard16 : public RGBController
{
public:
    RGBController_SinowealthKeyboard16(SinowealthKeyboard16Controller* controller_ptr);
    ~RGBController_SinowealthKeyboard16();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    SinowealthKeyboard16Controller* controller;

    mode getModeItem(unsigned int mode_id);
};

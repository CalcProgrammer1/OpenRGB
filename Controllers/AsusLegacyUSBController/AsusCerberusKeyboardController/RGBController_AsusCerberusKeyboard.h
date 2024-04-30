/*-----------------------------------------*\
|  RGBController_AsusCerberusKeyboard.h     |
|                                           |
|  Generic RGB Interface for Asus Cerberus  |
|  USB controller driver                    |
|                                           |
|  Mola19 03/03/2021                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusCerberusKeyboardController.h"

enum
{
    CERBERUS_MECH_BRIGHTNESS_MIN     = 0,
    CERBERUS_MECH_BRIGHTNESS_MAX     = 4,
    CERBERUS_MECH_BRIGHTNESS_DEFAULT = 4
};

class RGBController_AsusCerberusKeyboard : public RGBController
{
public:
    RGBController_AsusCerberusKeyboard(AsusCerberusKeyboardController* controller_ptr);
    ~RGBController_AsusCerberusKeyboard();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

private:
    AsusCerberusKeyboardController*  controller;
};

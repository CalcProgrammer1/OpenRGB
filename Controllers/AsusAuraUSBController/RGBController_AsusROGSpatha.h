/*-----------------------------------------*\
|  RGBController_AsusROGSpatha.h           |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 11/05/2023                        |
\*-----------------------------------------*/

#pragma once
#include "RGBController.h"
#include "AsusAuraMouseGen1Controller.h"

enum
{
    ASUS_ROG_SPATHA_BRIGHTNESS_MIN          = 0,
    ASUS_ROG_SPATHA_BRIGHTNESS_MAX          = 15,
    ASUS_ROG_SPATHA_BRIGHTNESS_DEFAULT      = 15
};

enum
{
    ASUS_ROG_SPATHA_MODE_DIRECT             = 0xFF,
    ASUS_ROG_SPATHA_MODE_STATIC             = 0x01,
    ASUS_ROG_SPATHA_MODE_SPECTRUM_CYCLE     = 0x05,
    ASUS_ROG_SPATHA_MODE_RANDOM             = 0x06,
    ASUS_ROG_SPATHA_MODE_BREATHING          = 0x0A,
    ASUS_ROG_SPATHA_MODE_BATTERY            = 0x0B,
    ASUS_ROG_SPATHA_MODE_REACTIVE           = 0x0C,
};

class RGBController_AsusROGSpatha : public RGBController
{
public:
    RGBController_AsusROGSpatha(AsusAuraMouseGen1Controller* controller_ptr);
    ~RGBController_AsusROGSpatha();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

private:
    AsusAuraMouseGen1Controller*  controller;
};

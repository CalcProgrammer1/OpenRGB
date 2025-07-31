/*---------------------------------------------------------*\
| RGBController_CMR6000Controller.cpp                       |
|                                                           |
|   RGBController for Cooler Master AMD Radeon 6000 series  |
|   GPU                                                     |
|                                                           |
|   Eric S (edbgon)                             02 Feb 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_CMR6000Controller.h"

/**------------------------------------------------------------------*\
    @name AMD Radeon 6000
    @category GPU
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectCoolerMasterGPU
    @comment Similar to the Wraith Spire before it the AMD branded Radeon
        GPUs have an RGB controller provided by Coolermaster.
\*-------------------------------------------------------------------*/

RGBController_CMR6000Controller::RGBController_CMR6000Controller(CMR6000Controller* controller_ptr)
{
    controller                      = controller_ptr;

    name                            = "AMD RX 6xxx GPU";
    vendor                          = "Cooler Master";
    type                            = DEVICE_TYPE_GPU;
    description                     = controller->GetDeviceName();
    serial                          = controller->GetSerial();
    location                        = controller->GetLocation();

    mode Off;
    Off.name                        = "Off";
    Off.flags                       = 0;
    Off.value                       = CM_MR6000_MODE_OFF;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name                     = "Direct";
    Direct.value                    = CM_MR6000_MODE_DIRECT;
    Direct.flags                    = MODE_FLAG_HAS_PER_LED_COLOR| MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode               = MODE_COLORS_PER_LED;
    Direct.speed                    = 0xFF;
    Direct.brightness_min           = 0x00;
    Direct.brightness_max           = 0xFF;
    Direct.brightness               = 0xFF;
    modes.push_back(Direct);

    mode ColorCycle;
    ColorCycle.name                 = "Spectrum Cycle";
    ColorCycle.value                = CM_MR6000_MODE_COLOR_CYCLE;
    ColorCycle.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    ColorCycle.speed_min            = MR6000_CYCLE_SPEED_SLOWEST;
    ColorCycle.speed                = MR6000_CYCLE_SPEED_NORMAL;
    ColorCycle.speed_max            = MR6000_CYCLE_SPEED_FASTEST;
    ColorCycle.color_mode           = MODE_COLORS_NONE;
    ColorCycle.speed                = MR6000_CYCLE_SPEED_NORMAL;
    ColorCycle.brightness_min       = 0x00;
    ColorCycle.brightness_max       = 0xFF;
    ColorCycle.brightness           = 0x7F;
    modes.push_back(ColorCycle);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = CM_MR6000_MODE_BREATHE;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.speed_min             = MR6000_BREATHE_SPEED_SLOWEST;
    Breathing.speed                 = MR6000_BREATHE_SPEED_NORMAL;
    Breathing.speed_max             = MR6000_BREATHE_SPEED_FASTEST;
    Breathing.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors_min            = 1;
    Breathing.colors_max            = 1;
    Breathing.colors.resize(1);
    Breathing.speed                 = MR6000_BREATHE_SPEED_NORMAL;
    modes.push_back(Breathing);

    if(controller->GetPID() == COOLERMASTER_RADEON_6900_PID)
    {
        mode Rainbow;
        Rainbow.name                = "Rainbow Wave";
        Rainbow.value               = CM_MR6000_MODE_RAINBOW;
        Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        Rainbow.speed_min           = MR6000_RAINBOW_SPEED_SLOWEST;
        Rainbow.speed               = MR6000_RAINBOW_SPEED_NORMAL;
        Rainbow.speed_max           = MR6000_RAINBOW_SPEED_FASTEST;
        Rainbow.color_mode          = MODE_COLORS_NONE;
        Rainbow.speed               = MR6000_RAINBOW_SPEED_NORMAL;
        Rainbow.brightness_min      = 0x00;
        Rainbow.brightness_max      = 0xFF;
        Rainbow.brightness          = 0xFF;
        modes.push_back(Rainbow);

        mode Bounce;
        Bounce.name                 = "Bounce";
        Bounce.value                = CM_MR6000_MODE_BOUNCE;
        Bounce.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        Bounce.speed_min            = MR6000_CYCLE_SPEED_SLOWEST;
        Bounce.speed                = MR6000_CYCLE_SPEED_NORMAL;
        Bounce.speed_max            = MR6000_CYCLE_SPEED_FASTEST;
        Bounce.color_mode           = MODE_COLORS_NONE;
        Bounce.speed                = MR6000_CYCLE_SPEED_NORMAL;
        Bounce.brightness_min       = 0x00;
        Bounce.brightness_max       = 0xFF;
        Bounce.brightness           = 0xFF;
        modes.push_back(Bounce);

        mode Chase;
        Chase.name                  = "Chase";
        Chase.value                 = CM_MR6000_MODE_CHASE;
        Chase.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
        Chase.speed_min             = MR6000_CYCLE_SPEED_SLOWEST;
        Chase.speed                 = MR6000_CYCLE_SPEED_NORMAL;
        Chase.speed_max             = MR6000_CYCLE_SPEED_FASTEST;
        Chase.color_mode            = MODE_COLORS_MODE_SPECIFIC;
        Chase.colors_min            = 2;
        Chase.colors_max            = 2;
        Chase.colors.resize(2);
        Chase.speed                 = MR6000_CYCLE_SPEED_NORMAL;
        Chase.brightness_min        = 0;
        Chase.brightness_max        = 0xFF;
        Chase.brightness            = 0xFF;
        modes.push_back(Chase);

        mode Swirl;
        Swirl.name                  = "Swirl";
        Swirl.value                 = CM_MR6000_MODE_SWIRL;
        Swirl.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
        Swirl.speed_min             = MR6000_CYCLE_SPEED_SLOWEST;
        Swirl.speed                 = MR6000_CYCLE_SPEED_NORMAL;
        Swirl.speed_max             = MR6000_CYCLE_SPEED_FASTEST;
        Swirl.color_mode            = MODE_COLORS_MODE_SPECIFIC;
        Swirl.colors_min            = 1;
        Swirl.colors_max            = 1;
        Swirl.colors.resize(1);
        Swirl.speed                 = MR6000_CYCLE_SPEED_NORMAL;
        Swirl.brightness_min        = 0;
        Swirl.brightness_max        = 0xFF;
        Swirl.brightness            = 0xFF;
        modes.push_back(Swirl);
    }

    SetupZones();
    active_mode = 1;
}

RGBController_CMR6000Controller::~RGBController_CMR6000Controller()
{
    delete controller;
}

void RGBController_CMR6000Controller::SetupZones()
{
    zone GP_zone;
    GP_zone.name        = "GPU";
    GP_zone.type        = ZONE_TYPE_SINGLE;
    GP_zone.leds_min    = 1;
    GP_zone.leds_max    = 1;
    GP_zone.leds_count  = 1;
    GP_zone.matrix_map  = NULL;
    zones.push_back(GP_zone);

    led GP_led;
    GP_led.name         = "Logo";
    GP_led.value        = 0;
    leds.push_back(GP_led);

    SetupColors();

}

void RGBController_CMR6000Controller::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMR6000Controller::DeviceUpdateLEDs()
{
    mode new_mode       = modes[active_mode];
    RGBColor color1     = (new_mode.colors.size() > 0) ? new_mode.colors[0] : colors[0];
    RGBColor color2     = (new_mode.colors.size() > 1) ? new_mode.colors[1] : 0;
    unsigned char bri   = (new_mode.flags & MODE_FLAG_HAS_BRIGHTNESS) ? new_mode.brightness : 0xFF;
    unsigned char rnd   = 0x20;

    switch(new_mode.value)
    {
        /*-----------------------------------------------------------------*\
        | Breathing mode requires value 0x20 when in MODE_SPECIFIC_COLOR    |
        \*-----------------------------------------------------------------*/
        case CM_MR6000_MODE_BREATHE:
            if(new_mode.color_mode == MODE_COLORS_RANDOM)
            {
                rnd = 0xA0;
            }
            break;
        case CM_MR6000_MODE_SWIRL:
        case CM_MR6000_MODE_CHASE:
            rnd = new_mode.direction;
            break;
        default:
            rnd = 0;
    }

    controller->SetMode(new_mode.value, new_mode.speed, color1, color2, rnd, bri);
}

void RGBController_CMR6000Controller::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMR6000Controller::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_CMR6000Controller::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

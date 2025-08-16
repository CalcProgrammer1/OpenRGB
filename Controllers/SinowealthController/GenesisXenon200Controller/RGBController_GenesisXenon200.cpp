/*---------------------------------------------------------*\
| RGBController_GenesisXenon200.cpp                         |
|                                                           |
|   RGBController for Genesis Xenon 200 mouse               |
|                                                           |
|   chrabonszcz                                    Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

/**------------------------------------------------------------------*\
    @name Genesis Xenon 200
    @type USB
    @save :white_check_mark:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectSinowealthMouse
    @comment
\*-------------------------------------------------------------------*/

#include "RGBController_GenesisXenon200.h"

RGBController_GenesisXenon200::RGBController_GenesisXenon200(GenesisXenon200Controller* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetNameString();
    vendor                      = "Genesis";
    description                 = "Genesis Xenon 200 Mouse Device";
    type                        = DEVICE_TYPE_MOUSE;
    location                    = controller->GetLocationString();

    mode Static;
    Static.name                 = "Static";
    Static.value                = GENESIS_XENON_200_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness           = 1;
    Static.brightness_min       = 0;
    Static.brightness_max       = 2;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = GENESIS_XENON_200_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed             = 1;
    Breathing.speed_min         = 0;
    Breathing.speed_max         = 2;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = GENESIS_XENON_200_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    SpectrumCycle.speed         = 1;
    SpectrumCycle.speed_min     = 0;
    SpectrumCycle.speed_max     = 2;
    modes.push_back(SpectrumCycle);

    mode Off;
    Off.name                    = "Off";
    Off.value                   = GENESIS_XENON_200_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    SetupColors();
}

RGBController_GenesisXenon200::~RGBController_GenesisXenon200()
{
    delete controller;
}

void RGBController_GenesisXenon200::DeviceUpdateMode()
{
    RGBColor color = 0;
    unsigned char value = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        color = modes[active_mode].colors[0];
    }

    switch(modes[active_mode].value)
    {
    case GENESIS_XENON_200_MODE_STATIC:

        value = GENESIS_XENON_200_STATIC_BRIGHTESS_VALUES[modes[active_mode].brightness];
        break;

    case GENESIS_XENON_200_MODE_BREATHING:

        value = GENESIS_XENON_200_BREATHING_SPEED_VALUES[modes[active_mode].speed];
        break;

    case GENESIS_XENON_200_MODE_SPECTRUM_CYCLE:

        value = GENESIS_XENON_200_SPECTRUM_CYCLE_SPEED_VALUES[modes[active_mode].speed];
        break;

    }

    controller->SaveMode(modes[active_mode].value, value, color);
}

void RGBController_GenesisXenon200::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_GenesisXenon200::DeviceSaveMode()
{

}

void RGBController_GenesisXenon200::SetupZones()
{

}

void RGBController_GenesisXenon200::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_GenesisXenon200::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_GenesisXenon200::UpdateSingleLED(int /*led*/)
{

}

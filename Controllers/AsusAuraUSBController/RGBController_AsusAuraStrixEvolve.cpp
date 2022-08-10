/*-----------------------------------------*\
|  RGBController_AsusAuraStrixEvolve.cpp    |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 11/30/2021                        |
\*-----------------------------------------*/

#include "RGBController_AsusAuraStrixEvolve.h"

/**------------------------------------------------------------------*\
    @name Asus Aura Strix Evolve
    @category Mouse
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectAsusAuraUSBStrixEvolve
    @comment
\*-------------------------------------------------------------------*/

RGBController_AuraStrixEvolve::RGBController_AuraStrixEvolve(AuraStrixEvolveController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "ASUS ROG Strix Evolve";
    vendor                      = "ASUS";
    type                        = DEVICE_TYPE_MOUSE;
    description                 = "ASUS Aura Mouse Device";
    version                     = controller->GetVersion();
    location                    = controller->GetDeviceLocation();
    serial                      = controller->GetSerialString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 1;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min       = AURA_STRIX_EVOLVE_BRIGHTNESS_MIN;
    Direct.brightness_max       = AURA_STRIX_EVOLVE_BRIGHTNESS_MAX;
    Direct.brightness           = AURA_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = 2;
    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.brightness_min    = AURA_STRIX_EVOLVE_BRIGHTNESS_MIN;
    Breathing.brightness_max    = AURA_STRIX_EVOLVE_BRIGHTNESS_MAX;
    Breathing.brightness        = AURA_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Spectrum Cycle";
    ColorCycle.value            = 3;
    ColorCycle.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.brightness_min   = AURA_STRIX_EVOLVE_BRIGHTNESS_MIN;
    ColorCycle.brightness_max   = AURA_STRIX_EVOLVE_BRIGHTNESS_MAX;
    ColorCycle.brightness       = AURA_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = 4;
    Reactive.flags              = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_MANUAL_SAVE | MODE_FLAG_HAS_BRIGHTNESS;
    Reactive.brightness_min     = AURA_STRIX_EVOLVE_BRIGHTNESS_MIN;
    Reactive.brightness_max     = AURA_STRIX_EVOLVE_BRIGHTNESS_MAX;
    Reactive.brightness         = AURA_STRIX_EVOLVE_BRIGHTNESS_DEFAULT;
    Reactive.color_mode         = MODE_COLORS_PER_LED;
    modes.push_back(Reactive);

    SetupZones();
}

RGBController_AuraStrixEvolve::~RGBController_AuraStrixEvolve()
{
    delete controller;
}

void RGBController_AuraStrixEvolve::SetupZones()
{
    zone mouse_zone;

    mouse_zone.name         = "Underglow";
    mouse_zone.type         = ZONE_TYPE_SINGLE;
    mouse_zone.leds_min     = 1;
    mouse_zone.leds_max     = 1;
    mouse_zone.leds_count   = 1;
    mouse_zone.matrix_map   = NULL;

    zones.push_back(mouse_zone);

    led mouse_led;

    mouse_led.name          = "Underglow";
    mouse_led.value         = 1;

    leds.push_back(mouse_led);

    SetupColors();
}

void RGBController_AuraStrixEvolve::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_AuraStrixEvolve::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SendUpdate(0x1C, red);
    controller->SendUpdate(0x1D, grn);
    controller->SendUpdate(0x1E, blu);
}

void RGBController_AuraStrixEvolve::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraStrixEvolve::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraStrixEvolve::DeviceUpdateMode()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SendUpdate(0x19, modes[active_mode].value);
    controller->SendUpdate(0x1A, modes[active_mode].brightness);
    controller->SendUpdate(0x1C, red);
    controller->SendUpdate(0x1D, grn);
    controller->SendUpdate(0x1E, blu);
}

void RGBController_AuraStrixEvolve::DeviceSaveMode()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    unsigned int profile = controller->GetActiveProfile();

    controller->UpdateProfile(0x19, profile, modes[active_mode].value);
    controller->UpdateProfile(0x1A, profile, modes[active_mode].brightness);
    controller->UpdateProfile(0x1C, profile, red);
    controller->UpdateProfile(0x1D, profile, grn);
    controller->UpdateProfile(0x1E, profile, blu);

    controller->SendSavePacket();
}

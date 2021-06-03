/*-----------------------------------------*\
|  RGBController_AsusAuraHeadsetStand.cpp   |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Mola19 06/04/2021                        |
\*-----------------------------------------*/

#include "RGBController_AsusAuraHeadsetStand.h"

RGBController_AuraHeadsetStand::RGBController_AuraHeadsetStand(AuraHeadsetStandController* aura_ptr)
{
    aura        = aura_ptr;

    name        = "ASUS Aura Headset Stand";
    vendor      = "ASUS";
    type        = DEVICE_TYPE_HEADSET_STAND;
    description = "ASUS Aura Headset Stand Device";
    location    = aura->GetDeviceLocation();
    serial      = aura->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = AURA_HEADSET_STAND_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = AURA_HEADSET_STAND_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min = 1;
    Static.colors_max = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = AURA_HEADSET_STAND_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min  = 0;
    Breathing.speed_max  = 255;
    Breathing.speed      = 127;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode Strobing;
    Strobing.name       = "Strobing";
    Strobing.value      = AURA_HEADSET_STAND_MODE_STROBING;
    Strobing.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Strobing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Strobing.colors_min = 1;
    Strobing.colors_max = 1;
    Strobing.colors.resize(1);
    modes.push_back(Strobing);

    mode SpectrumCycle;
    SpectrumCycle.name       = "Spectrum Cycle";
    SpectrumCycle.value      = AURA_HEADSET_STAND_MODE_COLOR_CYCLE;
    SpectrumCycle.flags      = MODE_FLAG_HAS_SPEED;
    SpectrumCycle.speed_min  = 0;
    SpectrumCycle.speed_max  = 255;
    SpectrumCycle.speed      = 127;
    SpectrumCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = AURA_HEADSET_STAND_MODE_RAINBOW;
    Rainbow.flags      = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min  = 0;
    Rainbow.speed_max  = 255;
    Rainbow.speed      = 127;
    modes.push_back(Rainbow);
    SetupZones();
}

RGBController_AuraHeadsetStand::~RGBController_AuraHeadsetStand()
{
	delete aura;
}

void RGBController_AuraHeadsetStand::SetupZones()
{
    zone underglow_zone;

    underglow_zone.name         = "Underglow";
    underglow_zone.type         = ZONE_TYPE_LINEAR;
    underglow_zone.leds_min     = 17;
    underglow_zone.leds_max     = 17;
    underglow_zone.leds_count   = 17;
    underglow_zone.matrix_map   = NULL;

    zones.push_back(underglow_zone);

    for(unsigned int i = 0; i < 17; i++)
    {
        led underglow_led;

        underglow_led.name      = "Underglow LED " + std::to_string(i);

        leds.push_back(underglow_led);
    }

    zone logo_zone;

    logo_zone.name              = "Logo";
    logo_zone.type              = ZONE_TYPE_SINGLE;
    logo_zone.leds_min          = 1;
    logo_zone.leds_max          = 1;
    logo_zone.leds_count        = 1;
    logo_zone.matrix_map        = NULL;

    zones.push_back(logo_zone);

    led logo_led;

    logo_led.name               = "Logo LED";

    leds.push_back(logo_led);

    SetupColors();
}

void RGBController_AuraHeadsetStand::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraHeadsetStand::DeviceUpdateLEDs()
{
    aura->UpdateLeds(std::vector<RGBColor>(colors));
}

void RGBController_AuraHeadsetStand::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraHeadsetStand::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraHeadsetStand::SetCustomMode()
{
    active_mode = 0;
}


void RGBController_AuraHeadsetStand::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;
    switch(modes[active_mode].value)
    {
        case 0:
            aura->UpdateLeds(std::vector<RGBColor>(colors));
            break;
        case 1:
        case 2:
        case 3:
            red = RGBGetRValue(modes[active_mode].colors[0]);
            grn = RGBGetGValue(modes[active_mode].colors[0]);
            blu = RGBGetBValue(modes[active_mode].colors[0]);
            aura->UpdateDevice(modes[active_mode].value, red, grn, blu, modes[active_mode].speed, false);
            break;
        case 4:
        case 5:
            aura->UpdateDevice(modes[active_mode].value, red, grn, blu, modes[active_mode].speed, false);
            break;
    }
}

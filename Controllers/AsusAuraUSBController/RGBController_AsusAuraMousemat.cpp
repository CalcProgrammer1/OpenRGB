/*-----------------------------------------*\
|  RGBController_AsusAuraMousemat.cpp       |
|                                           |
|  Generic RGB Interface for Asus Aura      |
|  USB controller driver                    |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/10/2022   |
\*-----------------------------------------*/

#include "RGBController_AsusAuraMousemat.h"

RGBController_AuraMousemat::RGBController_AuraMousemat(AuraMousematController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "ASUS Aura Mousemat";
    vendor      = "ASUS";
    type        = DEVICE_TYPE_MOUSEMAT;
    description = "ASUS Aura Mousemat Device";
    version     = controller->GetVersion();
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = AURA_MOUSEMAT_MODE_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name           = "Static";
    Static.value          = AURA_MOUSEMAT_MODE_STATIC;
    Static.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Static.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Static.speed_min      = AURA_MOUSEMAT_SPEED_MIN;
    Static.speed_max      = AURA_MOUSEMAT_SPEED_MAX;
    Static.speed          = AURA_MOUSEMAT_SPEED_DEFAULT;
    Static.brightness_min = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Static.brightness_max = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Static.brightness     = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Static.colors_min     = 1;
    Static.colors_max     = 1;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name           = "Breathing";
    Breathing.value          = AURA_MOUSEMAT_MODE_BREATHING;
    Breathing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min      = AURA_MOUSEMAT_SPEED_MIN;
    Breathing.speed_max      = AURA_MOUSEMAT_SPEED_MAX;
    Breathing.speed          = AURA_MOUSEMAT_SPEED_DEFAULT;
    Breathing.brightness_min = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Breathing.brightness_max = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Breathing.brightness     = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Breathing.colors_min     = 1;
    Breathing.colors_max     = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name           = "Color Cycle";
    ColorCycle.value          = AURA_MOUSEMAT_MODE_COLOR_CYCLE;
    ColorCycle.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    ColorCycle.color_mode     = MODE_COLORS_NONE;
    ColorCycle.speed_min      = AURA_MOUSEMAT_SPEED_MIN;
    ColorCycle.speed_max      = AURA_MOUSEMAT_SPEED_MAX;
    ColorCycle.speed          = AURA_MOUSEMAT_SPEED_DEFAULT;
    ColorCycle.brightness_min = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    ColorCycle.brightness_max = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    ColorCycle.brightness     = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    modes.push_back(ColorCycle);

    mode Comet;
    Comet.name           = "Comet";
    Comet.value          = AURA_MOUSEMAT_MODE_COMET;
    Comet.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    Comet.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Comet.speed_min      = AURA_MOUSEMAT_SPEED_MIN;
    Comet.speed_max      = AURA_MOUSEMAT_SPEED_MAX;
    Comet.speed          = AURA_MOUSEMAT_SPEED_DEFAULT;
    Comet.brightness_min = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    Comet.brightness_max = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    Comet.brightness     = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    Comet.colors_min     = 1;
    Comet.colors_max     = 1;
    Comet.colors.resize(1);
    modes.push_back(Comet);

    mode GlowingYoyo;
    GlowingYoyo.name           = "Glowing Yoyo";
    GlowingYoyo.value          = AURA_MOUSEMAT_MODE_GLOWING_YOYO;
    GlowingYoyo.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    GlowingYoyo.color_mode     = MODE_COLORS_NONE;
    GlowingYoyo.speed_min      = AURA_MOUSEMAT_SPEED_MIN;
    GlowingYoyo.speed_max      = AURA_MOUSEMAT_SPEED_MAX;
    GlowingYoyo.speed          = AURA_MOUSEMAT_SPEED_DEFAULT;
    GlowingYoyo.brightness_min = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    GlowingYoyo.brightness_max = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    GlowingYoyo.brightness     = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    modes.push_back(GlowingYoyo);

    SetupZones();
}

RGBController_AuraMousemat::~RGBController_AuraMousemat()
{
    delete controller;
}

void RGBController_AuraMousemat::SetupZones()
{
    zone mousemat_zone;

    mousemat_zone.name         = "Mousemat";
    mousemat_zone.type         = ZONE_TYPE_LINEAR;
    mousemat_zone.leds_min     = 15;
    mousemat_zone.leds_max     = 15;
    mousemat_zone.leds_count   = 15;
    mousemat_zone.matrix_map   = NULL;

    zones.push_back(mousemat_zone);

    for(unsigned int i = 0; i < 15; i++)
    {
        led mousemat_led;

        mousemat_led.name      = "Mousemat LED " + std::to_string(i);

        leds.push_back(mousemat_led);
    }

    SetupColors();
}

void RGBController_AuraMousemat::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AuraMousemat::DeviceUpdateLEDs()
{
    controller->UpdateLeds(std::vector<RGBColor>(colors));
}

void RGBController_AuraMousemat::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraMousemat::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AuraMousemat::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AuraMousemat::DeviceUpdateMode()
{
    unsigned char red = 0;
    unsigned char grn = 0;
    unsigned char blu = 0;

    switch(modes[active_mode].value)
    {
        case AURA_MOUSEMAT_MODE_DIRECT:
            controller->UpdateLeds(std::vector<RGBColor>(colors));
            break;
        case AURA_MOUSEMAT_MODE_STATIC:
        case AURA_MOUSEMAT_MODE_BREATHING:
        case AURA_MOUSEMAT_MODE_COLOR_CYCLE:
        case AURA_MOUSEMAT_MODE_CHASING:
        case AURA_MOUSEMAT_MODE_COMET:
        case AURA_MOUSEMAT_MODE_GLOWING_YOYO:
            red = RGBGetRValue(modes[active_mode].colors[0]);
            grn = RGBGetGValue(modes[active_mode].colors[0]);
            blu = RGBGetBValue(modes[active_mode].colors[0]);
            controller->UpdateDevice(modes[active_mode].value, red, grn, blu, modes[active_mode].speed, modes[active_mode].brightness);
            break;
    }
}

void RGBController_AuraMousemat::DeviceSaveMode()
{
    DeviceUpdateMode();
    controller->SaveMode();
}

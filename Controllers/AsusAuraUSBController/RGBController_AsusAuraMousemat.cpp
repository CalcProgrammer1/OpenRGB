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

    // mode Breathing;
    // Breathing.name           = "Breathing";
    // Breathing.value          = AURA_MOUSEMAT_MODE_BREATHING;
    // Breathing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    // Breathing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    // Breathing.speed_min      = AURA_MOUSEMAT_SPEED_MIN;
    // Breathing.speed_max      = AURA_MOUSEMAT_SPEED_MAX;
    // Breathing.speed          = AURA_MOUSEMAT_SPEED_DEFAULT;
    // Breathing.brightness_min = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    // Breathing.brightness_max = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    // Breathing.brightness     = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    // Breathing.colors_min     = 1;
    // Breathing.colors_max     = 1;
    // Breathing.colors.resize(1);
    // modes.push_back(Breathing);

    // mode Chasing;
    // Chasing.name           = "Chasing";
    // Chasing.value          = AURA_MOUSEMAT_MODE_CHASING;
    // Chasing.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_MANUAL_SAVE;
    // Chasing.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    // Chasing.speed_min      = AURA_MOUSEMAT_SPEED_MIN;
    // Chasing.speed_max      = AURA_MOUSEMAT_SPEED_MAX;
    // Chasing.speed          = AURA_MOUSEMAT_SPEED_DEFAULT;
    // Chasing.brightness_min = AURA_MOUSEMAT_BRIGHTNESS_MIN;
    // Chasing.brightness_max = AURA_MOUSEMAT_BRIGHTNESS_MAX;
    // Chasing.brightness     = AURA_MOUSEMAT_BRIGHTNESS_DEFAULT;
    // Chasing.colors_min     = 1;
    // Chasing.colors_max     = 1;
    // Chasing.colors.resize(1);
    // modes.push_back(Chasing);

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
        case AURA_MOUSEMAT_MODE_BREATHING:
        case AURA_MOUSEMAT_MODE_CHASING:
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

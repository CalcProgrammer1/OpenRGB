/*-----------------------------------------*\
|  RGBController_SteelSeriesApex3TKL.cpp    |
|                                           |
|  Paul K. Gerke - 27.10.2022               |
|  Dr_no (Chris M)                          |
\*-----------------------------------------*/

#include "RGBController_SteelSeriesApex3TKL.h"

/**------------------------------------------------------------------*\
    @name Steel Series Apex 3 TKL Keyboard
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectSteelSeriesApex3TKL
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesApex3TKL::RGBController_SteelSeriesApex3TKL(SteelSeriesApex3TKLController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = "Apex 3 TKL";
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "SteelSeries Apex 3 TKL device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();
    current_brightness      = controller->GetBrightness();

    mode direct;
    direct.name             = "Direct";
    direct.value            = static_cast<int>(APEX3_TKL_MODES::DIRECT);
    direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    direct.color_mode       = MODE_COLORS_PER_LED;
    direct.brightness_min   = STEELSERIES_APEX3TKL_BRIGHTNESS_MIN;
    direct.brightness_max   = STEELSERIES_APEX3TKL_BRIGHTNESS_MAX;
    direct.brightness       = current_brightness;
    modes.push_back(direct);

    mode rainbow;
    rainbow.name             = "Rainbow Wave";
    rainbow.value            = static_cast<int>(APEX3_TKL_MODES::RAINBOW);
    rainbow.flags            = MODE_FLAG_HAS_BRIGHTNESS;
    rainbow.color_mode       = MODE_COLORS_NONE;
    rainbow.brightness_min   = STEELSERIES_APEX3TKL_BRIGHTNESS_MIN;
    rainbow.brightness_max   = STEELSERIES_APEX3TKL_BRIGHTNESS_MAX;
    rainbow.brightness       = current_brightness;
    modes.push_back(rainbow);

    SetupZones();
}

RGBController_SteelSeriesApex3TKL::~RGBController_SteelSeriesApex3TKL()
{
    delete controller;
}


void RGBController_SteelSeriesApex3TKL::SetupZones()
{
    zone curr_zone;
    curr_zone.name          = "Keyboard";
    curr_zone.type          = ZONE_TYPE_LINEAR;
    curr_zone.leds_min      = 8;
    curr_zone.leds_max      = 8;
    curr_zone.leds_count    = 8;
    curr_zone.matrix_map    = NULL;
    zones.push_back(curr_zone);

    for(unsigned int i = 0; i < curr_zone.leds_count; i++)
    {
        led zone_led;
        zone_led.name = "LED " + std::to_string(i);
        leds.push_back(zone_led);
    }

    SetupColors();
}

void RGBController_SteelSeriesApex3TKL::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesApex3TKL::DeviceUpdateLEDs()
{
    controller->SetColor(colors);
    UpdateBrightness();
}

void RGBController_SteelSeriesApex3TKL::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesApex3TKL::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesApex3TKL::DeviceUpdateMode()
{
    switch (static_cast<APEX3_TKL_MODES>(active_mode)) {
        case APEX3_TKL_MODES::DIRECT:
            DeviceUpdateLEDs();
            break;
        case APEX3_TKL_MODES::RAINBOW:
            controller->SetRainbowMode();
            break;
    }
    UpdateBrightness();
}

void RGBController_SteelSeriesApex3TKL::UpdateBrightness()
{
    // Taken from Aerox3 Controller to prevent OpenRGB from automatically
    // overriding user-set brightness values via Mod+F11/F12
    if (current_brightness != modes[active_mode].brightness) {
        controller->SetBrightness(modes[active_mode].brightness);
        current_brightness = modes[active_mode].brightness;
    }
}

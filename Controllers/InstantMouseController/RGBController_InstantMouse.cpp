/*-----------------------------------------*\
|  RGBController_InstantMouse.cpp           |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Instant mouse RGB USB Driver             |
|                                           |
|  Guimard Morgan (morg) 1/19/2024          |
\*-----------------------------------------*/

#include "RGBController_InstantMouse.h"

/**------------------------------------------------------------------*\
    @name Instant mouse
    @category Mouse
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectInstantMouseControllers
    @comment This controller should work with all mouse with this chip.
    Identified devices that work with this controller: Advance Gaming
    GTA 250 (GX72-A725), Anko KM43243952 (GM8-A825), Anko KM43277483
\*-------------------------------------------------------------------*/

RGBController_InstantMouse::RGBController_InstantMouse(InstantMouseController* controller_ptr)
{
    controller                          = controller_ptr;    
    vendor                              = "Instant Microelectronics";
    type                                = DEVICE_TYPE_MOUSE;
    description                         = "Instant USB Gaming Mouse";
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();
    version                             = controller->GetFirmwareVersion();

    mode direct;
    direct.name                         = "Direct";
    direct.value                        = INSTANT_MOUSE_DIRECT_MODE;
    direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    direct.color_mode                   = MODE_COLORS_PER_LED;
    direct.brightness                   = INSTANT_MOUSE_BRIGHTNESS_MAX;
    direct.brightness_min               = INSTANT_MOUSE_BRIGHTNESS_MIN;
    direct.brightness_max               = INSTANT_MOUSE_BRIGHTNESS_MAX;
    modes.push_back(direct);

    mode rainbow;
    rainbow.name                        = "Rainbow wave";
    rainbow.value                       = INSTANT_MOUSE_RAINBOW_WAVE_MODE;
    rainbow.flags                       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    rainbow.color_mode                  = MODE_COLORS_NONE;
    rainbow.speed_min                   = INSTANT_MOUSE_SPEED_MIN;
    rainbow.speed_max                   = INSTANT_MOUSE_SPEED_MAX;
    rainbow.speed                       = INSTANT_MOUSE_SPEED_MAX/2;
    modes.push_back(rainbow);

    mode spectrum;
    spectrum.name                       = "Spectrum cycle";
    spectrum.value                      = INSTANT_MOUSE_SPECTRUM_CYCLE_MODE;
    spectrum.flags                      = MODE_FLAG_HAS_SPEED;
    spectrum.color_mode                 = MODE_COLORS_NONE;
    spectrum.speed_min                  = INSTANT_MOUSE_SPEED_MIN;
    spectrum.speed_max                  = INSTANT_MOUSE_SPEED_MAX;
    spectrum.speed                      = INSTANT_MOUSE_SPEED_MAX/2;
    modes.push_back(spectrum);

    mode breathing;
    breathing.name                      = "Breathing";
    breathing.value                     = INSTANT_MOUSE_BREATHING_MODE;
    breathing.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    breathing.color_mode                = MODE_COLORS_MODE_SPECIFIC;
    breathing.colors.resize(1);
    breathing.colors_min                = 1;
    breathing.colors_max                = 1;
    breathing.speed_min                 = INSTANT_MOUSE_SPEED_MIN;
    breathing.speed_max                 = INSTANT_MOUSE_SPEED_MAX;
    breathing.speed                     = INSTANT_MOUSE_SPEED_MAX/2;
    modes.push_back(breathing);

    mode fill;
    fill.name                           = "Fill";
    fill.value                          = INSTANT_MOUSE_FILL_DRAIN_MODE;
    fill.flags                          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    fill.color_mode                     = MODE_COLORS_NONE;
    fill.speed_min                      = INSTANT_MOUSE_SPEED_MIN;
    fill.speed_max                      = INSTANT_MOUSE_SPEED_MAX;
    fill.speed                          = INSTANT_MOUSE_SPEED_MAX/2;
    fill.brightness                     = INSTANT_MOUSE_BRIGHTNESS_MAX;
    fill.brightness_min                 = INSTANT_MOUSE_BRIGHTNESS_MIN;
    fill.brightness_max                 = INSTANT_MOUSE_BRIGHTNESS_MAX;
    modes.push_back(fill);

    mode loop;
    loop.name                           = "Loop";
    loop.value                          = INSTANT_MOUSE_LOOP_MODE;
    loop.flags                          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_DIRECTION_LR;
    loop.color_mode                     = MODE_COLORS_NONE;
    loop.speed_min                      = INSTANT_MOUSE_SPEED_MIN;
    loop.speed_max                      = INSTANT_MOUSE_SPEED_MAX;
    loop.speed                          = INSTANT_MOUSE_SPEED_MAX/2;
    loop.brightness                     = INSTANT_MOUSE_BRIGHTNESS_MAX;
    loop.brightness_min                 = INSTANT_MOUSE_BRIGHTNESS_MIN;
    loop.brightness_max                 = INSTANT_MOUSE_BRIGHTNESS_MAX;
    modes.push_back(loop);

    mode off;
    off.name                            = "Off";
    off.value                           = INSTANT_MOUSE_OFF_MODE;

    modes.push_back(off);

    SetupZones();
}

RGBController_InstantMouse::~RGBController_InstantMouse()
{
    delete controller;
}

void RGBController_InstantMouse::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Mouse";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = 1;
    new_zone.leds_max   = 1;
    new_zone.leds_count = 1;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(1);
    leds[0].name = "Mouse";

    SetupColors();
}

void RGBController_InstantMouse::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_InstantMouse::DeviceUpdateLEDs()
{
    controller->SendColor(colors[0]);
}

void RGBController_InstantMouse::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_InstantMouse::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_InstantMouse::DeviceUpdateMode()
{
    if(modes[active_mode].value == INSTANT_MOUSE_OFF_MODE)
    {
        controller->SetMode(INSTANT_MOUSE_DIRECT_MODE, 0, 0, 0);
        controller->SendColor(0);
    }
    else
    {
        controller->SetMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].direction);

        if(modes[active_mode].colors.size() == 1)
        {
            controller->SendColor(modes[active_mode].colors[0]);
        }
    }
}

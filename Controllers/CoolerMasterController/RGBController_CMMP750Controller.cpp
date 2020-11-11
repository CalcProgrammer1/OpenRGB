/*-------------------------------------------------------------------*\
|  RGBController_CMMP750Controller.cpp                                |
|                                                                     |
|  Driver for Coolermaster MP750 mousepad                             |
|                                                                     |
|  Chris M (Dr_No)          18th Apr 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_CMMP750Controller.h"

RGBController_CMMP750Controller::RGBController_CMMP750Controller(CMMP750Controller* cmmp_ptr)
{
    cmmp750             = cmmp_ptr;
    unsigned char speed = cmmp750->GetLedSpeed();

    name                = cmmp750->GetDeviceName();
    type                = DEVICE_TYPE_MOUSEMAT;
    description         = cmmp750->GetDeviceName();
    version             = "1.0";
    serial              = cmmp750->GetSerial();
    location            = cmmp750->GetLocation();

    mode Static;
    Static.name       = "Static";
    Static.value      = MP750_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Blink;
    Blink.name       = "Blink";
    Blink.value      = MP750_MODE_BLINK;
    Blink.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Blink.speed_min  = MP750_SPEED_SLOWEST;
    Blink.speed_max  = MP750_SPEED_FASTEST;
    Blink.color_mode = MODE_COLORS_PER_LED;
    Blink.speed      = speed;
    modes.push_back(Blink);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = MP750_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min  = MP750_SPEED_SLOWEST;
    Breathing.speed_max  = MP750_SPEED_FASTEST;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    Breathing.speed      = speed;
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name       = "Color Cycle";
    ColorCycle.value      = MP750_MODE_COLOR_CYCLE;
    ColorCycle.flags      = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min  = MP750_SPEED_SLOWEST;
    ColorCycle.speed_max  = MP750_SPEED_FASTEST;
    ColorCycle.color_mode = MODE_COLORS_NONE;
    ColorCycle.speed      = speed;
    modes.push_back(ColorCycle);

    mode BreathCycle;
    BreathCycle.name       = "Breath Cycle";
    BreathCycle.value      = MP750_MODE_BREATH_CYCLE;
    BreathCycle.flags      = MODE_FLAG_HAS_SPEED;
    BreathCycle.speed_min  = MP750_SPEED_SLOWEST;
    BreathCycle.speed_max  = MP750_SPEED_FASTEST;
    BreathCycle.color_mode = MODE_COLORS_NONE;
    BreathCycle.speed      = speed;
    modes.push_back(BreathCycle);

    SetupZones();
    active_mode = cmmp750->GetMode();
}

RGBController_CMMP750Controller::~RGBController_CMMP750Controller()
{

}

void RGBController_CMMP750Controller::SetupZones()
{
    zone MP_zone;
    MP_zone.name          = "Mousepad";
    MP_zone.type          = ZONE_TYPE_SINGLE;
    MP_zone.leds_min      = 1;
    MP_zone.leds_max      = 1;
    MP_zone.leds_count    = 1;
    MP_zone.matrix_map    = NULL;
    zones.push_back(MP_zone);

    led MP_led;
    MP_led.name = "Mousepad LED";
    leds.push_back(MP_led);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize colors for each LED                            |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned char red = cmmp750->GetLedRed();
        unsigned char grn = cmmp750->GetLedGreen();
        unsigned char blu = cmmp750->GetLedBlue();

        colors[led_idx] = ToRGBColor(red, grn, blu);
    }
}

void RGBController_CMMP750Controller::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_CMMP750Controller::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);
    cmmp750->SetColor(red, grn, blu);
}

void RGBController_CMMP750Controller::UpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    cmmp750->SetColor(red, grn, blu);
}

void RGBController_CMMP750Controller::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_CMMP750Controller::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_CMMP750Controller::DeviceUpdateMode()
{
    cmmp750->SetMode(modes[active_mode].value, modes[active_mode].speed);
}

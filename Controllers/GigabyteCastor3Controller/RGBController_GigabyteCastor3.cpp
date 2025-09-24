/*---------------------------------------------------------*\
|  RGBController_GigabyteCastor3.cpp                        |
|                                                           |
|  RGBController for Gigabyte Aorus Waterforce X II 360     |
|  AIO Cooler (Castor3 controller)                          |
|                                                           |
|  This file is part of the OpenRGB project                 |
|  SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteCastor3.h"

/**------------------------------------------------------------------*\
    @name Gigabyte Aorus Waterforce X II 360
    @category Cooler
    @type USB
    @save :o:
    @direct :o:
    @effects :white_check_mark:
    @detectors DetectGigabyteCastor3Controllers
    @comment Controls the LED ring on the pump head of the
        Gigabyte Aorus Waterforce X II 360 AIO cooler.
        Uses the Castor3 USB HID controller (VID 0x0414, PID 0x7A5E).
        The LED ring and fans share the same controller — they are
        not independently addressable and follow the same effect.
        LCD display control is not implemented.
        No direct/per-LED mode available — all modes are hardware
        effects with a single color or palette.
\*-------------------------------------------------------------------*/

RGBController_GigabyteCastor3::RGBController_GigabyteCastor3(GigabyteCastor3Controller* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Gigabyte Aorus Waterforce X II 360";
    vendor      = "Gigabyte";
    type        = DEVICE_TYPE_COOLER;
    description = "Gigabyte Aorus Waterforce X II 360 AIO Cooler";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();
    version     = controller->GetFirmwareVersion();

    /*---------------------------------------------------------*\
    |  All modes from castor3.py LED_EFFECTS (12 effects + off) |
    |                                                           |
    |  The Castor3 has no direct/per-LED mode. All effects are  |
    |  hardware-driven, either single-color, palette, or        |
    |  firmware-color (no user color).                          |
    |                                                           |
    |  Speed: 1-5 mapped to MODE_FLAG_HAS_SPEED                |
    |  Brightness: 1-10 mapped to MODE_FLAG_HAS_BRIGHTNESS      |
    |                                                           |
    |  mode.value stores the style byte for the c9 command.     |
    |  mode.speed stores 1-5 (scaled to speed*20 on wire).      |
    |  mode.brightness stores 1-10 (direct on wire).            |
    \*---------------------------------------------------------*/

    /*---------------------------------------------------------*\
    |  Off                                                      |
    |  Wire: Style=0x01 (Static) with speed_wire=0              |
    \*---------------------------------------------------------*/
    mode Off;
    Off.name            = "Off";
    Off.value           = 0xFF;     /* sentinel — handled specially */
    Off.flags           = 0;
    Off.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Off);

    /*---------------------------------------------------------*\
    |  Static — single color, no speed                          |
    \*---------------------------------------------------------*/
    mode Static;
    Static.name         = "Static";
    Static.value        = CASTOR3_STYLE_STATIC;
    Static.flags        = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.colors_min   = 1;
    Static.colors_max   = 1;
    Static.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Static.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Static.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Static.brightness   = CASTOR3_BRIGHTNESS_DEFAULT;
    Static.colors.resize(1);
    modes.push_back(Static);

    /*---------------------------------------------------------*\
    |  Pulse — single color, speed + brightness                 |
    \*---------------------------------------------------------*/
    mode Pulse;
    Pulse.name          = "Pulse";
    Pulse.value         = CASTOR3_STYLE_PULSE;
    Pulse.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Pulse.speed_min     = CASTOR3_SPEED_MIN;
    Pulse.speed_max     = CASTOR3_SPEED_MAX;
    Pulse.speed         = CASTOR3_SPEED_DEFAULT;
    Pulse.colors_min    = 1;
    Pulse.colors_max    = 1;
    Pulse.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    Pulse.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Pulse.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Pulse.brightness    = CASTOR3_BRIGHTNESS_DEFAULT;
    Pulse.colors.resize(1);
    modes.push_back(Pulse);

    /*---------------------------------------------------------*\
    |  Flash — single color, speed + brightness                 |
    \*---------------------------------------------------------*/
    mode Flash;
    Flash.name          = "Flash";
    Flash.value         = CASTOR3_STYLE_FLASH;
    Flash.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Flash.speed_min     = CASTOR3_SPEED_MIN;
    Flash.speed_max     = CASTOR3_SPEED_MAX;
    Flash.speed         = CASTOR3_SPEED_DEFAULT;
    Flash.colors_min    = 1;
    Flash.colors_max    = 1;
    Flash.color_mode    = MODE_COLORS_MODE_SPECIFIC;
    Flash.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Flash.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Flash.brightness    = CASTOR3_BRIGHTNESS_DEFAULT;
    Flash.colors.resize(1);
    modes.push_back(Flash);

    /*---------------------------------------------------------*\
    |  Double Flash — single color, speed + brightness          |
    \*---------------------------------------------------------*/
    mode DFlash;
    DFlash.name         = "Double Flash";
    DFlash.value        = CASTOR3_STYLE_DFLASH;
    DFlash.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    DFlash.speed_min    = CASTOR3_SPEED_MIN;
    DFlash.speed_max    = CASTOR3_SPEED_MAX;
    DFlash.speed        = CASTOR3_SPEED_DEFAULT;
    DFlash.colors_min   = 1;
    DFlash.colors_max   = 1;
    DFlash.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    DFlash.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    DFlash.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    DFlash.brightness   = CASTOR3_BRIGHTNESS_DEFAULT;
    DFlash.colors.resize(1);
    modes.push_back(DFlash);

    /*---------------------------------------------------------*\
    |  Cycle — firmware colors, speed only                      |
    \*---------------------------------------------------------*/
    mode Cycle;
    Cycle.name          = "Spectrum Cycle";
    Cycle.value         = CASTOR3_STYLE_CYCLE;
    Cycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.speed_min     = CASTOR3_SPEED_MIN;
    Cycle.speed_max     = CASTOR3_SPEED_MAX;
    Cycle.speed         = CASTOR3_SPEED_DEFAULT;
    Cycle.color_mode    = MODE_COLORS_NONE;
    Cycle.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Cycle.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Cycle.brightness    = CASTOR3_BRIGHTNESS_DEFAULT;
    modes.push_back(Cycle);

    /*---------------------------------------------------------*\
    |  Gradient — single color, speed + brightness              |
    |  Note: b4 = brightness on wire (special case)             |
    \*---------------------------------------------------------*/
    mode Gradient;
    Gradient.name       = "Gradient";
    Gradient.value      = CASTOR3_STYLE_GRADIENT;
    Gradient.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Gradient.speed_min  = CASTOR3_SPEED_MIN;
    Gradient.speed_max  = CASTOR3_SPEED_MAX;
    Gradient.speed      = CASTOR3_SPEED_DEFAULT;
    Gradient.colors_min = 1;
    Gradient.colors_max = 1;
    Gradient.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Gradient.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Gradient.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Gradient.brightness = CASTOR3_BRIGHTNESS_DEFAULT;
    Gradient.colors.resize(1);
    modes.push_back(Gradient);

    /*---------------------------------------------------------*\
    |  Color Shift — 8-color palette, speed + brightness        |
    \*---------------------------------------------------------*/
    mode ColorShift;
    ColorShift.name         = "Color Shift";
    ColorShift.value        = CASTOR3_STYLE_COLORSHIFT;
    ColorShift.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ColorShift.speed_min    = CASTOR3_SPEED_MIN;
    ColorShift.speed_max    = CASTOR3_SPEED_MAX;
    ColorShift.speed        = CASTOR3_SPEED_DEFAULT;
    ColorShift.colors_min   = 1;
    ColorShift.colors_max   = 8;
    ColorShift.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    ColorShift.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    ColorShift.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    ColorShift.brightness   = CASTOR3_BRIGHTNESS_DEFAULT;
    ColorShift.colors.resize(8);
    ColorShift.colors[0] = ToRGBColor(0xFF, 0x00, 0x00);
    ColorShift.colors[1] = ToRGBColor(0xFF, 0x72, 0x00);
    ColorShift.colors[2] = ToRGBColor(0xFF, 0xFF, 0x00);
    ColorShift.colors[3] = ToRGBColor(0x00, 0xFF, 0x00);
    ColorShift.colors[4] = ToRGBColor(0x00, 0xFF, 0xFF);
    ColorShift.colors[5] = ToRGBColor(0x00, 0x00, 0xFF);
    ColorShift.colors[6] = ToRGBColor(0xFF, 0x00, 0xFF);
    ColorShift.colors[7] = ToRGBColor(0xFF, 0x80, 0x80);
    modes.push_back(ColorShift);

    /*---------------------------------------------------------*\
    |  Wave — firmware colors, speed only                       |
    \*---------------------------------------------------------*/
    mode Wave;
    Wave.name           = "Wave";
    Wave.value          = CASTOR3_STYLE_WAVE;
    Wave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Wave.speed_min      = CASTOR3_SPEED_MIN;
    Wave.speed_max      = CASTOR3_SPEED_MAX;
    Wave.speed          = CASTOR3_SPEED_DEFAULT;
    Wave.color_mode     = MODE_COLORS_NONE;
    Wave.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Wave.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Wave.brightness     = CASTOR3_BRIGHTNESS_DEFAULT;
    modes.push_back(Wave);

    /*---------------------------------------------------------*\
    |  Rainbow — firmware colors, speed only                    |
    \*---------------------------------------------------------*/
    mode Rainbow;
    Rainbow.name        = "Rainbow";
    Rainbow.value       = CASTOR3_STYLE_RAINBOW;
    Rainbow.flags       = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.speed_min   = CASTOR3_SPEED_MIN;
    Rainbow.speed_max   = CASTOR3_SPEED_MAX;
    Rainbow.speed       = CASTOR3_SPEED_DEFAULT;
    Rainbow.color_mode  = MODE_COLORS_NONE;
    Rainbow.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Rainbow.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Rainbow.brightness  = CASTOR3_BRIGHTNESS_DEFAULT;
    modes.push_back(Rainbow);

    /*---------------------------------------------------------*\
    |  Tri-Color — 3-color palette, speed + brightness          |
    \*---------------------------------------------------------*/
    mode TriColor;
    TriColor.name       = "Tri-Color";
    TriColor.value      = CASTOR3_STYLE_TRICOLOR;
    TriColor.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    TriColor.speed_min  = CASTOR3_SPEED_MIN;
    TriColor.speed_max  = CASTOR3_SPEED_MAX;
    TriColor.speed      = CASTOR3_SPEED_DEFAULT;
    TriColor.colors_min = 1;
    TriColor.colors_max = 3;
    TriColor.color_mode = MODE_COLORS_MODE_SPECIFIC;
    TriColor.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    TriColor.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    TriColor.brightness = CASTOR3_BRIGHTNESS_DEFAULT;
    TriColor.colors.resize(3);
    TriColor.colors[0]  = ToRGBColor(0x00, 0x00, 0xFF);
    TriColor.colors[1]  = ToRGBColor(0x7D, 0x00, 0xFF);
    TriColor.colors[2]  = ToRGBColor(0xFF, 0x00, 0xFF);
    modes.push_back(TriColor);

    /*---------------------------------------------------------*\
    |  Spin — 3-color palette, speed + brightness               |
    \*---------------------------------------------------------*/
    mode Spin;
    Spin.name           = "Spin";
    Spin.value          = CASTOR3_STYLE_SPIN;
    Spin.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Spin.speed_min      = CASTOR3_SPEED_MIN;
    Spin.speed_max      = CASTOR3_SPEED_MAX;
    Spin.speed          = CASTOR3_SPEED_DEFAULT;
    Spin.colors_min     = 1;
    Spin.colors_max     = 3;
    Spin.color_mode     = MODE_COLORS_MODE_SPECIFIC;
    Spin.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Spin.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Spin.brightness     = CASTOR3_BRIGHTNESS_DEFAULT;
    Spin.colors.resize(3);
    Spin.colors[0]      = ToRGBColor(0xFF, 0x00, 0xFE);
    Spin.colors[1]      = ToRGBColor(0x00, 0xFF, 0xFB);
    Spin.colors[2]      = ToRGBColor(0xFF, 0xFF, 0x00);
    modes.push_back(Spin);

    /*---------------------------------------------------------*\
    |  Switch — 2-color palette, speed + brightness             |
    \*---------------------------------------------------------*/
    mode Switch;
    Switch.name         = "Switch";
    Switch.value        = CASTOR3_STYLE_SWITCH;
    Switch.flags        = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Switch.speed_min    = CASTOR3_SPEED_MIN;
    Switch.speed_max    = CASTOR3_SPEED_MAX;
    Switch.speed        = CASTOR3_SPEED_DEFAULT;
    Switch.colors_min   = 1;
    Switch.colors_max   = 2;
    Switch.color_mode   = MODE_COLORS_MODE_SPECIFIC;
    Switch.brightness_min = CASTOR3_BRIGHTNESS_MIN;
    Switch.brightness_max = CASTOR3_BRIGHTNESS_MAX;
    Switch.brightness   = CASTOR3_BRIGHTNESS_DEFAULT;
    Switch.colors.resize(2);
    Switch.colors[0]    = ToRGBColor(0xFF, 0x00, 0xFE);
    Switch.colors[1]    = ToRGBColor(0x00, 0xFF, 0xFB);
    modes.push_back(Switch);

    SetupZones();
}

RGBController_GigabyteCastor3::~RGBController_GigabyteCastor3()
{
    delete controller;
}

void RGBController_GigabyteCastor3::SetupZones()
{
    /*---------------------------------------------------------*\
    |  Single zone: LED ring + fans (not independently          |
    |  addressable — fans follow the pump ring effect)          |
    |                                                           |
    |  This is a SINGLE zone because the protocol does not      |
    |  expose per-LED addressing. All LEDs display the same     |
    |  hardware effect simultaneously.                          |
    \*---------------------------------------------------------*/
    zone led_ring;
    led_ring.name       = "LED Ring";
    led_ring.type       = ZONE_TYPE_SINGLE;
    led_ring.leds_min   = 1;
    led_ring.leds_max   = 1;
    led_ring.leds_count = 1;
    zones.push_back(led_ring);

    led new_led;
    new_led.name = "LED Ring";
    leds.push_back(new_led);

    SetupColors();
}

void RGBController_GigabyteCastor3::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_GigabyteCastor3::DeviceUpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteCastor3::DeviceUpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_GigabyteCastor3::DeviceUpdateMode()
{
    unsigned char style = (unsigned char)modes[active_mode].value;

    /*---------------------------------------------------------*\
    |  Handle Off mode (sentinel value 0xFF)                    |
    \*---------------------------------------------------------*/
    if(style == 0xFF)
    {
        controller->SetOff();
        return;
    }

    /*---------------------------------------------------------*\
    |  Determine speed, brightness, b4, b5, and color type      |
    |  from the effect lookup table matching castor3.py          |
    \*---------------------------------------------------------*/
    unsigned char speed      = CASTOR3_SPEED_DEFAULT;
    unsigned char brightness = CASTOR3_BRIGHTNESS_DEFAULT;

    if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
    {
        speed = (unsigned char)modes[active_mode].speed;
    }

    if(modes[active_mode].flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        brightness = (unsigned char)modes[active_mode].brightness;
    }

    /*---------------------------------------------------------*\
    |  Look up b4, b5, and color_type per effect                |
    |  These match the LED_EFFECTS dict in castor3.py           |
    \*---------------------------------------------------------*/
    unsigned char b4 = 0x02;
    unsigned char b5 = 0x01;
    castor3_color_type color_type = CASTOR3_COLORS_NONE;

    switch(style)
    {
        case CASTOR3_STYLE_STATIC:
        case CASTOR3_STYLE_PULSE:
        case CASTOR3_STYLE_FLASH:
        case CASTOR3_STYLE_DFLASH:
            b4 = 0x02;
            b5 = 0x01;
            color_type = CASTOR3_COLORS_SINGLE;
            break;

        case CASTOR3_STYLE_GRADIENT:
            /*-------------------------------------------------*\
            |  Gradient: b4 = brightness on wire (special case)  |
            \*-------------------------------------------------*/
            b4 = brightness;
            b5 = 0x01;
            color_type = CASTOR3_COLORS_SINGLE;
            break;

        case CASTOR3_STYLE_CYCLE:
        case CASTOR3_STYLE_WAVE:
        case CASTOR3_STYLE_RAINBOW:
            b4 = 0x02;
            b5 = 0x01;
            color_type = CASTOR3_COLORS_NONE;
            break;

        case CASTOR3_STYLE_COLORSHIFT:
            b4 = 0x08;     /* ClrCount=8 */
            b5 = 0x02;     /* CmbIndex+1=2 */
            color_type = CASTOR3_COLORS_PALETTE;
            break;

        case CASTOR3_STYLE_TRICOLOR:
        case CASTOR3_STYLE_SPIN:
            b4 = 0x02;
            b5 = 0x01;
            color_type = CASTOR3_COLORS_PALETTE;
            break;

        case CASTOR3_STYLE_SWITCH:
            b4 = 0x02;
            b5 = 0x01;
            color_type = CASTOR3_COLORS_PALETTE;
            break;
    }

    controller->SetEffect(style, speed, brightness, b4, b5,
                          color_type, modes[active_mode].colors);
}

/*---------------------------------------------------------*\
| RGBController_EKController.cpp                            |
|                                                           |
|   RGBController for EK Loop Connect                       |
|                                                           |
|   Chris M (Dr_No)                             16 Jul 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_EKController.h"

/**------------------------------------------------------------------*\
    @name EK Loop Connect
    @category LEDStrip
    @type USB
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectEKControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_EKController::RGBController_EKController(EKController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "EK";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = controller->GetDeviceName();
    serial      = controller->GetSerial();
    location    = controller->GetLocation();

    mode Static;
    Static.name                     = "Static";
    Static.value                    = EK_MODE_STATIC;
    Static.flags                    = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode               = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name                  = "Breathing";
    Breathing.value                 = EK_MODE_BREATHING;
    Breathing.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Breathing.speed_min             = EK_SPEED_SLOWEST;
    Breathing.speed_max             = EK_SPEED_FASTEST;
    Breathing.color_mode            = MODE_COLORS_PER_LED;
    Breathing.speed                 = EK_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode Fading;
    Fading.name                     = "Fading";
    Fading.value                    = EK_MODE_FADING;
    Fading.flags                    = MODE_FLAG_HAS_SPEED;
    Fading.speed_min                = EK_SPEED_SLOWEST;
    Fading.speed_max                = EK_SPEED_FASTEST;
    Fading.color_mode               = MODE_COLORS_NONE;
    Fading.speed                    = EK_SPEED_NORMAL;
    modes.push_back(Fading);

    mode Marquee;
    Marquee.name                    = "Marquee";
    Marquee.value                   = EK_MODE_MARQUEE;
    Marquee.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Marquee.speed_min               = EK_SPEED_SLOWEST;
    Marquee.speed_max               = EK_SPEED_FASTEST;
    Marquee.color_mode              = MODE_COLORS_PER_LED;
    Marquee.speed                   = EK_SPEED_NORMAL;
    modes.push_back(Marquee);

    mode Covering_Marquee;
    Covering_Marquee.name           = "Covering Marquee";
    Covering_Marquee.value          = EK_MODE_COVERING_MARQUEE;
    Covering_Marquee.flags          = MODE_FLAG_HAS_SPEED;
    Covering_Marquee.speed_min      = EK_SPEED_SLOWEST;
    Covering_Marquee.speed_max      = EK_SPEED_FASTEST;
    Covering_Marquee.color_mode     = MODE_COLORS_NONE;
    Covering_Marquee.speed          = EK_SPEED_NORMAL;
    modes.push_back(Covering_Marquee);

    mode Pulse;
    Pulse.name                      = "Pulse";
    Pulse.value                     = EK_MODE_PULSE;
    Pulse.flags                     = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Pulse.speed_min                 = EK_SPEED_SLOWEST;
    Pulse.speed_max                 = EK_SPEED_FASTEST;
    Pulse.color_mode                = MODE_COLORS_PER_LED;
    Pulse.speed                     = EK_SPEED_NORMAL;
    modes.push_back(Pulse);

    mode Spectrum_Wave;
    Spectrum_Wave.name              = "Spectrum_Wave";
    Spectrum_Wave.value             = EK_MODE_SPECTRUM_WAVE;
    Spectrum_Wave.flags             = MODE_FLAG_HAS_SPEED;
    Spectrum_Wave.speed_min         = EK_SPEED_SLOWEST;
    Spectrum_Wave.speed_max         = EK_SPEED_FASTEST;
    Spectrum_Wave.color_mode        = MODE_COLORS_NONE;
    Spectrum_Wave.speed             = EK_SPEED_NORMAL;
    modes.push_back(Spectrum_Wave);

    mode Alternating;
    Alternating.name                = "Alternating";
    Alternating.value               = EK_MODE_ALTERNATING;
    Alternating.flags               = MODE_FLAG_HAS_SPEED;
    Alternating.speed_min           = EK_SPEED_SLOWEST;
    Alternating.speed_max           = EK_SPEED_FASTEST;
    Alternating.color_mode          = MODE_COLORS_PER_LED;
    Alternating.speed               = EK_SPEED_NORMAL;
    modes.push_back(Alternating);

    mode Candle;
    Candle.name                    = "Candle";
    Candle.value                   = EK_MODE_CANDLE;
    Candle.flags                   = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Candle.speed_min               = EK_SPEED_SLOWEST;
    Candle.speed_max               = EK_SPEED_FASTEST;
    Candle.color_mode              = MODE_COLORS_PER_LED;
    Candle.speed                   = EK_SPEED_NORMAL;
    modes.push_back(Candle);

    SetupZones();
}

RGBController_EKController::~RGBController_EKController()
{
    delete controller;
}

void RGBController_EKController::SetupZones()
{
    zone EK_zone;
    EK_zone.name        = "Loop Connect";
    EK_zone.type        = ZONE_TYPE_SINGLE;
    EK_zone.leds_min    = 1;
    EK_zone.leds_max    = 1;
    EK_zone.leds_count  = 1;
    zones.push_back(EK_zone);

    led EK_led;
    EK_led.name         = "EK LED";
    leds.push_back(EK_led);

    SetupColors();
}

void RGBController_EKController::DeviceResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | ToDo                                                      |
    \*---------------------------------------------------------*/
}

void RGBController_EKController::DeviceUpdateLEDs()
{
    unsigned char red = RGBGetRValue(colors[0]);
    unsigned char grn = RGBGetGValue(colors[0]);
    unsigned char blu = RGBGetBValue(colors[0]);

    controller->SetColor(red, grn, blu);
}

void RGBController_EKController::DeviceUpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    controller->SetColor(red, grn, blu);
}

void RGBController_EKController::DeviceUpdateSingleLED(int led)
{
    DeviceUpdateZoneLEDs(led);
}

void RGBController_EKController::DeviceUpdateMode()
{
    controller->SetMode(modes[active_mode].value, modes[active_mode].speed);
}

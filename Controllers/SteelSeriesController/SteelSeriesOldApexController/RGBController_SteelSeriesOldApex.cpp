/*---------------------------------------------------------*\
| RGBController_SteelSeriesOldApex.cpp                      |
|                                                           |
|   RGBController for older SteelSeries Apex keyboards      |
|   (Apex/Apex Fnatic/Apex 350)                             |
|                                                           |
|   Based on findings in ApexCtl by Audrius/tuxmark5, et.   |
|   al, https://github.com/tuxmark5/ApexCtl                 |
|                                                           |
|   David Lee (RAMChYLD)                        15 Nov 2020 |
|   Based on work by B Horn (bahorn)            13 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_SteelSeriesOldApex.h"

/**------------------------------------------------------------------*\
    @name Steel Series Apex (Old)
    @category Keyboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectSteelSeriesApexOld
    @comment
\*-------------------------------------------------------------------*/

RGBController_SteelSeriesOldApex::RGBController_SteelSeriesOldApex(SteelSeriesOldApexController* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    vendor                  = "SteelSeries";
    type                    = DEVICE_TYPE_KEYBOARD;
    description             = "SteelSeries Old Apex Device";
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerialString();

    mode direct;
    direct.name             = "Direct";
    direct.value            = STEELSERIES_OLDAPEX_DIRECT;
    direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(direct);

    SetupZones();
}

RGBController_SteelSeriesOldApex::~RGBController_SteelSeriesOldApex()
{
    delete controller;
}

void RGBController_SteelSeriesOldApex::SetupZones()
{
    /* We have 5 zones to work with. Here goes... */
    zone qwerty_zone;
    qwerty_zone.name        = "QWERTY";
    qwerty_zone.type        = ZONE_TYPE_LINEAR;
    qwerty_zone.leds_min    = 1;
    qwerty_zone.leds_max    = 1;
    qwerty_zone.leds_count  = 1;
    qwerty_zone.matrix_map  = NULL;
    zones.push_back(qwerty_zone);

    led qwerty_led;
    qwerty_led.name         = "QWERTY";
    leds.push_back(qwerty_led);

    zone tenkey_zone;
    tenkey_zone.name        = "TenKey";
    tenkey_zone.type        = ZONE_TYPE_LINEAR;
    tenkey_zone.leds_min    = 1;
    tenkey_zone.leds_max    = 1;
    tenkey_zone.leds_count  = 1;
    tenkey_zone.matrix_map  = NULL;
    zones.push_back(tenkey_zone);

    led tenkey_led;
    tenkey_led.name         = "TenKey";
    leds.push_back(tenkey_led);

    zone function_zone;
    function_zone.name        = "FunctionKeys";
    function_zone.type        = ZONE_TYPE_LINEAR;
    function_zone.leds_min    = 1;
    function_zone.leds_max    = 1;
    function_zone.leds_count  = 1;
    function_zone.matrix_map  = NULL;
    zones.push_back(function_zone);

    led function_led;
    function_led.name       = "FunctionKeys";
    leds.push_back(function_led);

    zone mx_zone;
    mx_zone.name            = "MXKeys";
    mx_zone.type            = ZONE_TYPE_LINEAR;
    mx_zone.leds_min        = 1;
    mx_zone.leds_max        = 1;
    mx_zone.leds_count      = 1;
    mx_zone.matrix_map      = NULL;
    zones.push_back(mx_zone);

    led mx_led;
    mx_led.name             = "MXKeys";
    leds.push_back(mx_led);

    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_LINEAR;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    led logo_led;
    logo_led.name           = "Logo";
    leds.push_back(logo_led);

    SetupColors();
}

void RGBController_SteelSeriesOldApex::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SteelSeriesOldApex::DeviceUpdateLEDs()
{
    // Due to the inefficient packet design of the OG Apex
    // All colors must be blasted with each update
    color32 qwerty;
    qwerty.red = RGBGetRValue(colors[0]);
    qwerty.green = RGBGetGValue(colors[0]);
    qwerty.blue = RGBGetBValue(colors[0]);
    qwerty.alpha = modes[active_mode].value;

    color32 tenkey;
    tenkey.red = RGBGetRValue(colors[1]);
    tenkey.green = RGBGetGValue(colors[1]);
    tenkey.blue = RGBGetBValue(colors[1]);
    tenkey.alpha = modes[active_mode].value;

    color32 functionkey;
    functionkey.red = RGBGetRValue(colors[2]);
    functionkey.green = RGBGetGValue(colors[2]);
    functionkey.blue = RGBGetBValue(colors[2]);
    functionkey.alpha = modes[active_mode].value;

    color32 mxkey;
    mxkey.red = RGBGetRValue(colors[3]);
    mxkey.green = RGBGetGValue(colors[3]);
    mxkey.blue = RGBGetBValue(colors[3]);
    mxkey.alpha = modes[active_mode].value;

    color32 logo;
    logo.red = RGBGetRValue(colors[4]);
    logo.green = RGBGetGValue(colors[4]);
    logo.blue = RGBGetBValue(colors[4]);
    logo.alpha = modes[active_mode].value;

    controller->SetColorDetailed(qwerty, tenkey, functionkey, mxkey, logo);
}

void RGBController_SteelSeriesOldApex::UpdateZoneLEDs(int /*zone*/)
{
    // updating for one zone is pointless,
    // all zones have to be blasted anyway
    // so just do a full update
    DeviceUpdateLEDs();
}


void RGBController_SteelSeriesOldApex::UpdateSingleLED(int /*led*/)
{
    // Each zone is one LED, however
    // updating for one zone is pointless,
    // all zones have to be blasted anyway
    // so just do a full update
    DeviceUpdateLEDs();
}

void RGBController_SteelSeriesOldApex::DeviceUpdateMode()
{
    // We are using SetLightingEffect to control the brightness of
    // LEDs. Per-zone brightness is actually possible but we are not
    // doing that for now. Brightness affects whole keyboard.

    // Because at the moment all this code does is change brightness,
    // We just let the new value set in and do a device LED update
    // and blast the brightness value along with the RGB values

    DeviceUpdateLEDs();
}

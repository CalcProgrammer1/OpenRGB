/*---------------------------------------------------------*\
| RGBController_XG270QC.cpp                                 |
|                                                           |
|   RGBController for ViewSonic XG270QC                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <string>
#include "RGBController_XG270QC.h"

/**------------------------------------------------------------------*\
    @name Viewsonic Monitor
    @category Accessory
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectViewSonic
    @comment
\*-------------------------------------------------------------------*/

RGBController_XG270QC::RGBController_XG270QC(VS_XG270QC_Controller* controller_ptr)
{
    controller              = controller_ptr;

    name                    = controller->GetName();
    vendor                  = "ViewSonic";
    type                    = DEVICE_TYPE_MONITOR;
    description             = "ViewSonic Monitor Device";
    location                = controller->GetLocation();
    serial                  = controller->GetSerial();

    mode Off;
    Off.name                = "Off";
    Off.value               = VS_XG270QC_Controller::VS_MODE_OFF;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name             = "Static";
    Static.value            = VS_XG270QC_Controller::VS_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.colors_min       = 2;
    Static.colors_max       = 2;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = VS_XG270QC_Controller::VS_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.colors_min    = 2;
    Breathing.colors_max    = 2;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Rainbow;
    Rainbow.name            = "Rainbow";
    Rainbow.value           = VS_XG270QC_Controller::VS_MODE_RAINBOW;
    Rainbow.flags           = MODE_FLAG_AUTOMATIC_SAVE;
    Rainbow.color_mode      = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode WarpSpeed;
    WarpSpeed.name          = "Warp Speed";
    WarpSpeed.value         = VS_XG270QC_Controller::VS_MODE_WARP_SPEED;
    WarpSpeed.flags         = MODE_FLAG_AUTOMATIC_SAVE;
    WarpSpeed.color_mode    = MODE_COLORS_NONE;
    modes.push_back(WarpSpeed);

    mode Stack;
    Stack.name              = "Stack";
    Stack.value             = VS_XG270QC_Controller::VS_MODE_STACK;
    Stack.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Stack.colors_min        = 2;
    Stack.colors_max        = 2;
    Stack.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Stack);

    //The modes Music and MusicPulse are not supported

    RGBController_XG270QC::SetupZones();
}

void RGBController_XG270QC::SetupZones()
{
    zone base;
    base.name               = "Base";
    base.type               = ZONE_TYPE_SINGLE;
    base.leds_min           = 1;
    base.leds_max           = 1;
    base.leds_count         = 1;
    base.matrix_map         = NULL;
    zones.push_back(base);

    zone rear;
    rear.name               = "Rear";
    rear.type               = ZONE_TYPE_SINGLE;
    rear.leds_min           = 1;
    rear.leds_max           = 1;
    rear.leds_count         = 1;
    rear.matrix_map         = NULL;
    zones.push_back(rear);

    led d;
    d.name                  = "Base";
    d.value                 = 0x00;
    leds.push_back(d);

    led back;
    back.name               = "Rear";
    back.value              = 0x01;
    leds.push_back(back);

    SetupColors();
}

void RGBController_XG270QC::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_XG270QC::DeviceUpdateLEDs()
{
    DeviceUpdateMode();
}

void RGBController_XG270QC::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_XG270QC::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_XG270QC::DeviceUpdateMode()
{
    uint8_t r1 = 0;
    uint8_t g1 = 0;
    uint8_t b1 = 0;
    uint8_t r2 = 0;
    uint8_t g2 = 0;
    uint8_t b2 = 0;

    if(modes[active_mode].flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        r1 = r2 = RGBGetRValue(modes[active_mode].colors[0]);
        g1 = g2 = RGBGetGValue(modes[active_mode].colors[0]);
        b1 = b2 = RGBGetBValue(modes[active_mode].colors[0]);
    }
    else if (modes[active_mode].flags & MODE_FLAG_HAS_PER_LED_COLOR)
    {
        r1 = RGBGetRValue(colors[0]);
        g1 = RGBGetGValue(colors[0]);
        b1 = RGBGetBValue(colors[0]);

        r2 = RGBGetRValue(colors[1]);
        g2 = RGBGetGValue(colors[1]);
        b2 = RGBGetBValue(colors[1]);
    }

    controller->SetMode(modes[active_mode].value, r1, g1, b1, modes[active_mode].value, r2, g2, b2);
}

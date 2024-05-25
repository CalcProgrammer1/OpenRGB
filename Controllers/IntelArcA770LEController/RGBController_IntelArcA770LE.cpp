/*---------------------------------------------------------*\
| RGBController_IntelArcA770LE.cpp                          |
|                                                           |
|   RGBController for Intel Arc A770 LE                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                01 Nov 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_IntelArcA770LE.h"

/**------------------------------------------------------------------*\
    @name Intel Arc A770 Limited Edition
    @category GPU
    @type USB
    @save :o:
    @direct :white_check_mark:
    @effects :tools:
    @detectors DetectIntelArcA770LEControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_IntelArcA770LE::RGBController_IntelArcA770LE(IntelArcA770LEController* controller_ptr)
{
    controller  = controller_ptr;

    name        = "Intel Arc A770 Limited Edition";
    vendor      = "Cooler Master";
    type        = DEVICE_TYPE_GPU;
    description = "Intel Arc A770 Limited Edition";
    version     = controller->GetFirmwareVersionString();
    location    = controller->GetLocationString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min       = 0;
    Direct.brightness_max       = 0;
    Direct.brightness           = 0;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    controller->SendEnableCommand();
    controller->SendApplyCommand();

    SetupZones();
}

RGBController_IntelArcA770LE::~RGBController_IntelArcA770LE()
{
    delete controller;
}

void RGBController_IntelArcA770LE::SetupZones()
{
    const unsigned int fan_1_leds[16] = { 0x01, 0x04, 0x07, 0x0A, 0x0D, 0x10, 0x13, 0x16,
                                          0x19, 0x1C, 0x1F, 0x22, 0x25, 0x28, 0x2B, 0x2E };
    const unsigned int fan_2_leds[16] = { 0x31, 0x34, 0x37, 0x3A, 0x3D, 0x40, 0x43, 0x46,
                                          0x49, 0x4C, 0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E };
    const unsigned int back_leds[8] =   { 0x02, 0x05, 0x08, 0x0B, 0x0E, 0x11, 0x14, 0x17 };
    const unsigned int ring_leds[50] =  { 0x00, 0x03, 0x06, 0x09, 0x0C, 0x0F, 0x12, 0x15,
                                          0x18, 0x1B, 0x1E, 0x21, 0x24, 0x27, 0x2A, 0x2D,
                                          0x30, 0x33, 0x36, 0x39, 0x3C, 0x3F, 0x42, 0x45,
                                          0x48, 0x4B, 0x4E, 0x51, 0x54, 0x57, 0x5A, 0x5D,
                                          0x60, 0x63, 0x66, 0x69, 0x6C, 0x6F, 0x72, 0x75,
                                          0x78, 0x7B, 0x7E, 0x81, 0x84, 0x87, 0x8A, 0x8D,
                                          0x90, 0x93 };
    const unsigned int logo_leds[1] =   { 0x96 };

    zone fan_1_zone;
    fan_1_zone.name         = "Fan 1";
    fan_1_zone.type         = ZONE_TYPE_LINEAR;
    fan_1_zone.leds_min     = 16;
    fan_1_zone.leds_max     = 16;
    fan_1_zone.leds_count   = 16;
    fan_1_zone.matrix_map   = NULL;
    zones.push_back(fan_1_zone);

    zone fan_2_zone;
    fan_2_zone.name         = "Fan 2";
    fan_2_zone.type         = ZONE_TYPE_LINEAR;
    fan_2_zone.leds_min     = 16;
    fan_2_zone.leds_max     = 16;
    fan_2_zone.leds_count   = 16;
    fan_2_zone.matrix_map   = NULL;
    zones.push_back(fan_2_zone);

    zone back;
    back.name               = "Back";
    back.type               = ZONE_TYPE_LINEAR;
    back.leds_min           = 8;
    back.leds_max           = 8;
    back.leds_count         = 8;
    back.matrix_map         = NULL;
    zones.push_back(back);

    zone ring;
    ring.name               = "Ring";
    ring.type               = ZONE_TYPE_LINEAR;
    ring.leds_min           = 50;
    ring.leds_max           = 50;
    ring.leds_count         = 50;
    ring.matrix_map         = NULL;
    zones.push_back(ring);

    zone logo;
    logo.name               = "Logo";
    logo.type               = ZONE_TYPE_SINGLE;
    logo.leds_min           = 1;
    logo.leds_max           = 1;
    logo.leds_count         = 1;
    logo.matrix_map         = NULL;
    zones.push_back(logo);

    for(unsigned int led_idx = 0; led_idx < 16; led_idx++)
    {
        led fan_1_led;
        fan_1_led.name = "Fan 1 LED";
        fan_1_led.value = fan_1_leds[led_idx];
        leds.push_back(fan_1_led);
    }

    for(unsigned int led_idx = 0; led_idx < 16; led_idx++)
    {
        led fan_2_led;
        fan_2_led.name = "Fan 2 LED";
        fan_2_led.value = fan_2_leds[led_idx];
        leds.push_back(fan_2_led);
    }

    for(unsigned int led_idx = 0; led_idx < 8; led_idx++)
    {
        led back_led;
        back_led.name = "Back LED";
        back_led.value = back_leds[led_idx];
        leds.push_back(back_led);
    }

    for(unsigned int led_idx = 0; led_idx < 50; led_idx++)
    {
        led ring_led;
        ring_led.name = "Ring LED";
        ring_led.value = ring_leds[led_idx];
        leds.push_back(ring_led);
    }

    for(unsigned int led_idx = 0; led_idx < 1; led_idx++)
    {
        led logo_led;
        logo_led.name = "Logo LED";
        logo_led.value = logo_leds[led_idx];
        leds.push_back(logo_led);
    }

    SetupColors();
}

void RGBController_IntelArcA770LE::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_IntelArcA770LE::DeviceUpdateLEDs()
{
    unsigned char   led_ids[15];
    RGBColor        color_buf[15];
    unsigned int    leds_count = 0;

    for(unsigned int led_idx = 0; led_idx < colors.size(); led_idx++)
    {
        led_ids[leds_count]     = (unsigned char)leds[led_idx].value;
        color_buf[leds_count]   = colors[led_idx];

        leds_count++;

        if(leds_count >= 15)
        {
            controller->SendDirectPacket(15, led_ids, color_buf);
            leds_count = 0;
        }
    }

    if(leds_count > 0)
    {
        controller->SendDirectPacket(leds_count, led_ids, color_buf);
    }
}

void RGBController_IntelArcA770LE::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_IntelArcA770LE::UpdateSingleLED(int /*led*/)
{
}

void RGBController_IntelArcA770LE::DeviceUpdateMode()
{
}

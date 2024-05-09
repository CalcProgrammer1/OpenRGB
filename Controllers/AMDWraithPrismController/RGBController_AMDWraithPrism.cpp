/*---------------------------------------------------------*\
| RGBController_AMDWraithPrism.cpp                          |
|                                                           |
|   RGBController for AMD Wraith Prism                      |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_AMDWraithPrism.h"

/**------------------------------------------------------------------*\
    @name AMD Wraith Prism
    @category Cooler
    @type USB
    @save :o:
    @direct :white_check_mark:
    @effects :tools:
    @detectors DetectAMDWraithPrismControllers
    @comment The Wraith Prism comes with 2 cables but is only detectable
        and controlable when using the USB cable. `Morse Code` and `Mirage`
        modes have not been implemented. Saving to flash is supported by
        the device but not yet implemented.
\*-------------------------------------------------------------------*/

RGBController_AMDWraithPrism::RGBController_AMDWraithPrism(AMDWraithPrismController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "AMD Wraith Prism";
    vendor                      = "Cooler Master";
    type                        = DEVICE_TYPE_COOLER;
    description                 = "AMD Wraith Prism Device";
    version                     = controller->GetFirmwareVersionString();
    location                    = controller->GetLocationString();

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = AMD_WRAITH_PRISM_EFFECT_CHANNEL_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.brightness_min       = 0;
    Direct.brightness_max       = 0;
    Direct.brightness           = 0;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = AMD_WRAITH_PRISM_EFFECT_CHANNEL_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.speed_min         = AMD_WRAITH_PRISM_SPEED_SLOWEST;
    Breathing.speed_max         = AMD_WRAITH_PRISM_SPEED_FASTEST;
    Breathing.brightness_min    = 0;
    Breathing.brightness_max    = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Breathing.brightness        = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed             = AMD_WRAITH_PRISM_SPEED_NORMAL;
    Breathing.colors.resize(3);
    modes.push_back(Breathing);

    mode ColorCycle;
    ColorCycle.name             = "Spectrum Cycle";
    ColorCycle.value            = AMD_WRAITH_PRISM_EFFECT_CHANNEL_COLOR_CYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    ColorCycle.speed_min        = AMD_WRAITH_PRISM_SPEED_SLOWEST;
    ColorCycle.speed_max        = AMD_WRAITH_PRISM_SPEED_FASTEST;
    ColorCycle.brightness_min   = 0;
    ColorCycle.brightness_max   = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_CYCLE_MAX;
    ColorCycle.brightness       = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_CYCLE_MAX;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    ColorCycle.speed            = AMD_WRAITH_PRISM_SPEED_NORMAL;
    modes.push_back(ColorCycle);

    mode Rainbow;
    Rainbow.name                = "Rainbow Wave";
    Rainbow.value               = AMD_WRAITH_PRISM_EFFECT_CHANNEL_RAINBOW;
    Rainbow.flags               = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Rainbow.speed_min           = AMD_WRAITH_PRISM_SPEED_SLOWEST;
    Rainbow.speed_max           = AMD_WRAITH_PRISM_SPEED_FASTEST;
    Rainbow.brightness_min      = 0;
    Rainbow.brightness_max      = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Rainbow.brightness          = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;    //The Ring zone can not get brighter but Logo / Fan can
    Rainbow.color_mode          = MODE_COLORS_NONE;
    Rainbow.speed               = AMD_WRAITH_PRISM_SPEED_NORMAL;
    modes.push_back(Rainbow);

    mode Bounce;
    Bounce.name                 = "Bounce";
    Bounce.value                = AMD_WRAITH_PRISM_EFFECT_CHANNEL_BOUNCE;
    Bounce.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Bounce.speed_min            = AMD_WRAITH_PRISM_SPEED_SLOWEST;
    Bounce.speed_max            = AMD_WRAITH_PRISM_SPEED_FASTEST;
    Bounce.brightness_min       = 0;
    Bounce.brightness_max       = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Bounce.brightness           = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;    //The Ring zone can not get brighter but Logo / Fan can
    Bounce.color_mode           = MODE_COLORS_NONE;
    Bounce.speed                = AMD_WRAITH_PRISM_SPEED_NORMAL;
    modes.push_back(Bounce);

    mode Chase;
    Chase.name                  = "Chase";
    Chase.value                 = AMD_WRAITH_PRISM_EFFECT_CHANNEL_CHASE;
    Chase.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Chase.speed_min             = AMD_WRAITH_PRISM_SPEED_SLOWEST;
    Chase.speed_max             = AMD_WRAITH_PRISM_SPEED_FASTEST;
    Chase.brightness_min        = 0;
    Chase.brightness_max        = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Chase.brightness            = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Chase.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Chase.speed                 = AMD_WRAITH_PRISM_SPEED_NORMAL;
    Chase.colors.resize(3);
    modes.push_back(Chase);

    mode Swirl;
    Swirl.name                  = "Swirl";
    Swirl.value                 = AMD_WRAITH_PRISM_EFFECT_CHANNEL_SWIRL;
    Swirl.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Swirl.speed_min             = AMD_WRAITH_PRISM_SPEED_SLOWEST;
    Swirl.speed_max             = AMD_WRAITH_PRISM_SPEED_FASTEST;
    Swirl.brightness_min        = 0;
    Swirl.brightness_max        = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Swirl.brightness            = AMD_WRAITH_PRISM_FAN_BRIGHTNESS_DEFAULT_MAX;
    Swirl.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Swirl.speed                 = AMD_WRAITH_PRISM_SPEED_NORMAL;
    Swirl.colors.resize(3);
    modes.push_back(Swirl);

    SetupZones();
}

RGBController_AMDWraithPrism::~RGBController_AMDWraithPrism()
{
    delete controller;
}

void RGBController_AMDWraithPrism::SetupZones()
{
    /*---------------------------------------------------------*\
    | LED maps                                                  |
    \*---------------------------------------------------------*/
    const unsigned int logo_leds[1]  =  { 0x00 };
    const unsigned int fan_leds[1]   =  { 0x01 };
    const unsigned int ring_leds[14] =  { 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x10, 0x0F,
                                          0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09 };

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone logo_zone;
    logo_zone.name          = "Logo";
    logo_zone.type          = ZONE_TYPE_SINGLE;
    logo_zone.leds_min      = 1;
    logo_zone.leds_max      = 1;
    logo_zone.leds_count    = 1;
    logo_zone.matrix_map    = NULL;
    zones.push_back(logo_zone);

    zone fan_zone;
    fan_zone.name           = "Fan";
    fan_zone.type           = ZONE_TYPE_SINGLE;
    fan_zone.leds_min       = 1;
    fan_zone.leds_max       = 1;
    fan_zone.leds_count     = 1;
    fan_zone.matrix_map     = NULL;
    zones.push_back(fan_zone);

    zone ring_zone;
    ring_zone.name          = "Ring";
    ring_zone.type          = ZONE_TYPE_LINEAR;
    ring_zone.leds_min      = 14;
    ring_zone.leds_max      = 14;
    ring_zone.leds_count    = 14;
    ring_zone.matrix_map    = NULL;
    zones.push_back(ring_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    for(unsigned int led_idx = 0; led_idx < 1; led_idx++)
    {
        led logo_led;
        logo_led.name       = "Logo LED";
        logo_led.value      = logo_leds[led_idx];
        leds.push_back(logo_led);
    }

    for(unsigned int led_idx = 0; led_idx < 1; led_idx++)
    {
        led fan_led;
        fan_led.name        = "Fan LED";
        fan_led.value       = fan_leds[led_idx];
        leds.push_back(fan_led);
    }

    for(unsigned int led_idx = 0; led_idx < 14; led_idx++)
    {
        led ring_led;
        ring_led.name       = "Ring LED";
        ring_led.value      = ring_leds[led_idx];
        leds.push_back(ring_led);
    }

    SetupColors();
}

void RGBController_AMDWraithPrism::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AMDWraithPrism::DeviceUpdateLEDs()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
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
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        unsigned char red = RGBGetRValue(modes[active_mode].colors[0]);
        unsigned char grn = RGBGetGValue(modes[active_mode].colors[0]);
        unsigned char blu = RGBGetBValue(modes[active_mode].colors[0]);
        controller->SetLogoColor(red, grn, blu);

        red = RGBGetRValue(modes[active_mode].colors[1]);
        grn = RGBGetGValue(modes[active_mode].colors[1]);
        blu = RGBGetBValue(modes[active_mode].colors[1]);
        controller->SetFanColor(red, grn, blu);

        red = RGBGetRValue(modes[active_mode].colors[2]);
        grn = RGBGetGValue(modes[active_mode].colors[2]);
        blu = RGBGetBValue(modes[active_mode].colors[2]);
        controller->SetRingColor(red, grn, blu);
    }
    else
    {
        controller->SetLogoColor(0, 0, 0);
        controller->SetFanColor(0, 0, 0);
        controller->SetRingColor(0, 0, 0);
    }
}

void RGBController_AMDWraithPrism::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AMDWraithPrism::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_AMDWraithPrism::DeviceUpdateMode()
{
    bool random = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

    switch(modes[active_mode].value)
    {
        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_DIRECT:
            controller->SendEnableCommand(true);
            controller->SendApplyCommand();
            break;

        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_COLOR_CYCLE:
            controller->SendEnableCommand(false);
            controller->SetRingMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].direction, random);
            controller->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[active_mode].speed, modes[active_mode].brightness, random);
            controller->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[active_mode].speed, modes[active_mode].brightness, random);
            break;

        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_RAINBOW:
        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_BOUNCE:
            controller->SendEnableCommand(false);
            controller->SetRingMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].direction, random);
            controller->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[active_mode].speed, (modes[active_mode].brightness >> 1), random);
            controller->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[active_mode].speed, (modes[active_mode].brightness >> 1), random);
            break;

        case AMD_WRAITH_PRISM_EFFECT_CHANNEL_BREATHING:
            controller->SendEnableCommand(false);
            controller->SetRingMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].direction, random);
            controller->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_BREATHING, modes[active_mode].speed, modes[active_mode].brightness, random);
            controller->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_BREATHING, modes[active_mode].speed, modes[active_mode].brightness, random);
            break;

        default:
            if(random)
            {
                controller->SendEnableCommand(false);
                controller->SetRingMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].direction, random);
                controller->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[active_mode].speed, modes[active_mode].brightness, random);
                controller->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_COLOR_CYCLE, modes[active_mode].speed, modes[active_mode].brightness, random);
            }
            else
            {
                controller->SendEnableCommand(false);
                controller->SetRingMode(modes[active_mode].value, modes[active_mode].speed, modes[active_mode].brightness, modes[active_mode].direction, random);
                controller->SetFanMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC, modes[active_mode].speed, modes[active_mode].brightness, random);
                controller->SetLogoMode(AMD_WRAITH_PRISM_FAN_LOGO_MODE_STATIC, modes[active_mode].speed, modes[active_mode].brightness, random);
            }
            break;
    }

    DeviceUpdateLEDs();
}

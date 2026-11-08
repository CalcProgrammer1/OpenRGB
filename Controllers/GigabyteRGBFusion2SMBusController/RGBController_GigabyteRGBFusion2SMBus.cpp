/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2SMBus.cpp                 |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 SMBus     |
|   motherboard                                             |
|                                                           |
|   Matt Harper                                 05 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusion2SMBus.h"
#include "GigabyteFusion2SMBus_Devices.h"
#include "LogManager.h"

/**------------------------------------------------------------------*\
    @name Gigabyte Fusion2 SMBus
    @category Motherboard
    @type I2C
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteRGBFusion2SMBusControllers
    @comment The ITE IT8295 SMBus controller found on Gigabyte
        Z370 / B360 / X470 / B450 / X299 / X399 era boards.  Boards
        listed in GigabyteFusion2SMBus_Devices.cpp expose only their
        populated zones (onboard, 12V RGB and Digital LED headers);
        other boards get the generic 10 slot layout.
\*-------------------------------------------------------------------*/

RGBController_RGBFusion2SMBus::RGBController_RGBFusion2SMBus(RGBFusion2SMBusController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetDeviceName();
    vendor      = "Gigabyte";
    description = "RGB Fusion 2 SMBus";
    location    = controller->GetDeviceLocation();

    type = DEVICE_TYPE_MOTHERBOARD;

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = RGB_FUSION_2_MODE_STATIC;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness_min = RGB_FUSION_2_BRIGHTNESS_MIN;
    Direct.brightness_max = RGB_FUSION_2_BRIGHTNESS_MAX;
    Direct.brightness     = RGB_FUSION_2_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    mode Pulse;
    Pulse.name       = "Pulse";
    Pulse.value      = RGB_FUSION_2_MODE_PULSE;
    Pulse.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Pulse.speed_min  = RGB_FUSION_2_SPEED_SLOW;
    Pulse.speed_max  = RGB_FUSION_2_SPEED_FAST;
    Pulse.speed      = RGB_FUSION_2_SPEED_NORMAL;
    Pulse.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Pulse);

    mode Flashing;
    Flashing.name       = "Flashing";
    Flashing.value      = RGB_FUSION_2_MODE_FLASHING;
    Flashing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    Flashing.speed_min  = 0x01;
    Flashing.speed_max  = 0x04;
    Flashing.speed      = 0x02;
    Flashing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Flashing);

    mode ColorCycle;
    ColorCycle.name           = "Color Cycle";
    ColorCycle.value          = RGB_FUSION_2_MODE_COLOR_CYCLE;
    ColorCycle.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    ColorCycle.speed_min      = RGB_FUSION_2_SPEED_SLOW;
    ColorCycle.speed_max      = RGB_FUSION_2_SPEED_FAST;
    ColorCycle.speed          = RGB_FUSION_2_SPEED_NORMAL;
    ColorCycle.brightness_min = RGB_FUSION_2_BRIGHTNESS_MIN;
    ColorCycle.brightness_max = RGB_FUSION_2_BRIGHTNESS_MAX;
    ColorCycle.brightness     = RGB_FUSION_2_BRIGHTNESS_MAX;
    ColorCycle.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(ColorCycle);

    mode DigitalWave;
    DigitalWave.name           = "Digital Wave";
    DigitalWave.value          = RGB_FUSION_2_MODE_DIGITAL_WAVE;
    DigitalWave.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    DigitalWave.speed_min      = 0xff;
    DigitalWave.speed_max      = 0x49;
    DigitalWave.speed          = 0xc1;
    DigitalWave.brightness_min = RGB_FUSION_2_BRIGHTNESS_MIN;
    DigitalWave.brightness_max = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalWave.brightness     = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalWave.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(DigitalWave);

    mode DigitalA;
    DigitalA.name       = "Star Slide";
    DigitalA.value      = RGB_FUSION_2_MODE_DIGITAL_A;
    DigitalA.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalA.speed_min  = RGB_FUSION_2_DIGITAL_SPEED_MIN;
    DigitalA.speed_max  = RGB_FUSION_2_DIGITAL_SPEED_MAX;
    DigitalA.speed      = RGB_FUSION_2_DIGITAL_SPEED;
    DigitalA.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalA);

    mode DigitalB;
    DigitalB.name       = "Meteor";
    DigitalB.value      = RGB_FUSION_2_MODE_DIGITAL_B;
    DigitalB.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalB.speed_min  = 0xf0;
    DigitalB.speed_max  = 0x3c;
    DigitalB.speed      = 0xa0;
    DigitalB.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalB);

    mode DigitalC;
    DigitalC.name       = "Shaking";
    DigitalC.value      = RGB_FUSION_2_MODE_DIGITAL_C;
    DigitalC.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalC.speed_min  = RGB_FUSION_2_DIGITAL_SPEED_MIN;
    DigitalC.speed_max  = RGB_FUSION_2_DIGITAL_SPEED_MAX;
    DigitalC.speed      = RGB_FUSION_2_DIGITAL_SPEED;
    DigitalC.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalC);

    mode DigitalD;
    DigitalD.name           = "Recover";
    DigitalD.value          = RGB_FUSION_2_MODE_DIGITAL_D;
    DigitalD.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    DigitalD.speed_min      = RGB_FUSION_2_DIGITAL_SPEED_MIN;
    DigitalD.speed_max      = RGB_FUSION_2_DIGITAL_SPEED_MAX;
    DigitalD.speed          = RGB_FUSION_2_DIGITAL_SPEED;
    DigitalD.brightness_min = RGB_FUSION_2_BRIGHTNESS_MIN;
    DigitalD.brightness_max = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalD.brightness     = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalD.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(DigitalD);

    mode DigitalE;
    DigitalE.name       = "Double Meteor";
    DigitalE.value      = RGB_FUSION_2_MODE_DIGITAL_E;
    DigitalE.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR;
    DigitalE.speed_min  = 0x96;
    DigitalE.speed_max  = RGB_FUSION_2_DIGITAL_SPEED_MAX;
    DigitalE.speed      = 0x6e;
    DigitalE.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(DigitalE);

    mode DigitalF;
    DigitalF.name           = "Color Stack";
    DigitalF.value          = RGB_FUSION_2_MODE_DIGITAL_F;
    DigitalF.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    DigitalF.speed_min      = RGB_FUSION_2_DIGITAL_SPEED_MIN;
    DigitalF.speed_max      = RGB_FUSION_2_DIGITAL_SPEED_MAX;
    DigitalF.speed          = RGB_FUSION_2_DIGITAL_SPEED;
    DigitalF.brightness_min = RGB_FUSION_2_BRIGHTNESS_MIN;
    DigitalF.brightness_max = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalF.brightness     = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalF.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(DigitalF);

    mode DigitalG;
    DigitalG.name           = "Color Rain";
    DigitalG.value          = RGB_FUSION_2_MODE_DIGITAL_G;
    DigitalG.flags          = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    DigitalG.speed_min      = 0x8c;
    DigitalG.speed_max      = 0x46;
    DigitalG.speed          = 0x6e;
    DigitalG.brightness_min = RGB_FUSION_2_BRIGHTNESS_MIN;
    DigitalG.brightness_max = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalG.brightness     = RGB_FUSION_2_BRIGHTNESS_MAX;
    DigitalG.color_mode     = MODE_COLORS_PER_LED;
    modes.push_back(DigitalG);

    SetupZones();

    // Initialize active mode
    // TODO - broken. Need to complete GetDeviceMode
    active_mode = GetDeviceMode();
}

RGBController_RGBFusion2SMBus::~RGBController_RGBFusion2SMBus()
{
    Shutdown();

    delete controller;
}

void RGBController_RGBFusion2SMBus::SetupZones()
{
    /*---------------------------------------------------------*\
    | Pick the board layout (generic when the board is unknown) |
    \*---------------------------------------------------------*/
    const gb_fusion2_smbus_layout*  layout      = GetGigabyteFusion2SMBusLayout(name);
    const gb_fusion2_smbus_zone*    zone_map    = layout->zones;
    unsigned int                    zone_count  = layout->zone_count;

    if(zone_count > controller->GetLEDCount())
    {
        zone_count = controller->GetLEDCount();
    }

    if(layout == &gb_fusion2_smbus_generic_layout)
    {
        LOG_INFO("[%s] No zone layout for '%s', using generic %u slot layout", "Gigabyte RGB Fusion 2 SMBus", name.c_str(), zone_count);
    }
    else
    {
        LOG_DEBUG("[%s] Using zone layout for '%s' (%u zones)", "Gigabyte RGB Fusion 2 SMBus", name.c_str(), zone_count);
    }

    zone_division.clear();

    /*---------------------------------------------------------*\
    | Create one single-LED zone per populated division         |
    \*---------------------------------------------------------*/
    for(unsigned int zone_idx = 0; zone_idx < zone_count; zone_idx++)
    {
        zone new_zone;

        new_zone.name          = zone_map[zone_idx].name;
        new_zone.type          = ZONE_TYPE_SINGLE;
        new_zone.leds_min      = 1;
        new_zone.leds_max      = 1;
        new_zone.leds_count    = 1;

        zones.push_back(new_zone);
        zone_division.push_back(zone_map[zone_idx].division);
    }

    for(unsigned int led_idx = 0; led_idx < zones.size(); led_idx++)
    {
        led new_led;

        new_led.name           = zone_map[led_idx].name;

        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_RGBFusion2SMBus::DeviceUpdateLEDs()
{
    for(unsigned int led = 0; led < (unsigned int)colors.size(); led++)
    {
        RGBColor      color     = colors[led];
        unsigned char red       = RGBGetRValue(color);
        unsigned char grn       = RGBGetGValue(color);
        unsigned char blu       = RGBGetBValue(color);

        int          mode       = modes[active_mode].value;
        unsigned int speed      = modes[active_mode].speed;
        unsigned int brightness = modes[active_mode].brightness;

        controller->SetLEDEffect(zone_division[led], mode, brightness, speed, red, grn, blu);
    }

    controller->Apply();
}

void RGBController_RGBFusion2SMBus::DeviceUpdateZoneLEDs(int zone)
{
    RGBColor      color = colors[zone];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    int          mode       = modes[active_mode].value;
    unsigned int speed      = modes[active_mode].speed;
    unsigned int brightness = modes[active_mode].brightness;

    controller->SetLEDEffect(zone_division[zone], mode, brightness, speed, red, grn, blu);
    controller->Apply();
}

void RGBController_RGBFusion2SMBus::DeviceUpdateSingleLED(int led)
{
    DeviceUpdateZoneLEDs(led);
}

void RGBController_RGBFusion2SMBus::DeviceUpdateMode()
{

}

// TODO - Research if possible to read device state
int RGBController_RGBFusion2SMBus::GetDeviceMode()
{
    return(0);
}

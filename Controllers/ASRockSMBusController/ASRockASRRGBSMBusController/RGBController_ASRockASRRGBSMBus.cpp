/*---------------------------------------------------------*\
| RGBController_ASRRGBSMBus.cpp                             |
|                                                           |
|   RGBController for SMBus ASRock ASR LED motherboards     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                15 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_ASRockASRRGBSMBus.h"

#define ASROCK_MAX_ZONES    4
#define ASROCK_MAX_LEDS     22

/**------------------------------------------------------------------*\
    @name ASRock ASR RGB SMBus
    @category Motherboard
    @type SMBus
    @save :robot:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectASRockSMBusControllers
    @comment ASRock ASR RGB LED controllers will save with each update.
        Per ARGB LED support is not possible with these devices.
\*-------------------------------------------------------------------*/

RGBController_ASRockASRRGBSMBus::RGBController_ASRockASRRGBSMBus(ASRockASRRGBSMBusController* controller_ptr)
{
    controller                  = controller_ptr;

    name                        = controller->GetDeviceName();
    vendor                      = "ASRock";
    version                     = controller->GetFirmwareVersion();
    type                        = DEVICE_TYPE_MOTHERBOARD;
    description                 = "ASRock ASR RGB LED Device";
    location                    = controller->GetDeviceLocation();

    mode Off;
    Off.name                    = "Off";
    Off.value                   = ASRLED_MODE_OFF;
    Off.flags                   = MODE_FLAG_AUTOMATIC_SAVE;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Static;
    Static.name                 = "Static";
    Static.value                = ASRLED_MODE_STATIC;
    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = ASRLED_MODE_BREATHING;
    Breathing.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Breathing.speed_min         = ASRLED_SPEED_MIN;
    Breathing.speed_max         = ASRLED_SPEED_MAX;
    Breathing.speed             = ASRLED_SPEED_DEFAULT;
    Breathing.color_mode        = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Strobe;
    Strobe.name                 = "Strobe";
    Strobe.value                = ASRLED_MODE_STROBE;
    Strobe.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Strobe.speed_min            = ASRLED_SPEED_MIN;
    Strobe.speed_max            = ASRLED_SPEED_MAX;
    Strobe.speed                = ASRLED_SPEED_DEFAULT;
    Strobe.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Strobe);

    mode SpectrumCycle;
    SpectrumCycle.name          = "Spectrum Cycle";
    SpectrumCycle.value         = ASRLED_MODE_SPECTRUM_CYCLE;
    SpectrumCycle.flags         = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    SpectrumCycle.speed_min     = ASRLED_SPEED_MIN;
    SpectrumCycle.speed_max     = ASRLED_SPEED_MAX;
    SpectrumCycle.speed         = ASRLED_SPEED_DEFAULT;
    SpectrumCycle.color_mode    = MODE_COLORS_NONE;
    modes.push_back(SpectrumCycle);

    mode Random;
    Random.name                 = "Random";
    Random.value                = ASRLED_MODE_RANDOM;
    Random.flags                = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Random.speed_min            = ASRLED_SPEED_MIN;
    Random.speed_max            = ASRLED_SPEED_MAX;
    Random.speed                = ASRLED_SPEED_DEFAULT;
    Random.color_mode           = MODE_COLORS_NONE;
    modes.push_back(Random);

    mode Music;
    Music.name                  = "Music";
    Music.value                 = ASRLED_MODE_MUSIC;
    Music.flags                 = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Music.color_mode            = MODE_COLORS_PER_LED;
    modes.push_back(Music);

    mode Wave;
    Wave.name                   = "Wave";
    Wave.value                  = ASRLED_MODE_WAVE;
    Wave.flags                  = MODE_FLAG_HAS_SPEED | MODE_FLAG_AUTOMATIC_SAVE;
    Wave.speed_min              = ASRLED_SPEED_MIN;
    Wave.speed_max              = ASRLED_SPEED_MAX;
    Wave.speed                  = ASRLED_SPEED_DEFAULT;
    Wave.color_mode             = MODE_COLORS_NONE;
    modes.push_back(Wave);

    SetupZones();
}

RGBController_ASRockASRRGBSMBus::~RGBController_ASRockASRRGBSMBus()
{
    delete controller;
}

void RGBController_ASRockASRRGBSMBus::SetupZones()
{
    /*---------------------------------------------------------*\
    | ASR LED motherboards only have a single zone/LED          |
    \*---------------------------------------------------------*/

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();

    /*---------------------------------------------------------*\
    | Set single zone name to "Motherboard"                     |
    \*---------------------------------------------------------*/
    new_zone->name          = "Motherboard";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    /*---------------------------------------------------------*\
    | Push new zone to zones vector                             |
    \*---------------------------------------------------------*/
    zones.push_back(*new_zone);

    /*---------------------------------------------------------*\
    | Set up LEDs                                               |
    \*---------------------------------------------------------*/
    led* new_led = new led();

    /*---------------------------------------------------------*\
    | Set single LED name to "Motherboard"                      |
    \*---------------------------------------------------------*/
    new_led->name           = "Motherboard";

    /*---------------------------------------------------------*\
    | Push new LED to LEDs vector                               |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);

    SetupColors();
}

void RGBController_ASRockASRRGBSMBus::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_ASRockASRRGBSMBus::DeviceUpdateLEDs()
{
    for(unsigned int led = 0; led < colors.size(); led++)
    {
        UpdateSingleLED(led);
    }
}

void RGBController_ASRockASRRGBSMBus::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_ASRockASRRGBSMBus::UpdateSingleLED(int led)
{
    unsigned char red = RGBGetRValue(colors[led]);
    unsigned char grn = RGBGetGValue(colors[led]);
    unsigned char blu = RGBGetBValue(colors[led]);

    /*---------------------------------------------------------*\
    | If the LED value is non-zero, this LED overrides the LED  |
    | index                                                     |
    \*---------------------------------------------------------*/
    if(leds[led].value != 0)
    {
        led = leds[led].value;
    }

    controller->SetColorsAndSpeed(led, red, grn, blu);
}

void RGBController_ASRockASRRGBSMBus::DeviceUpdateMode()
{

    controller->SetMode(0, modes[active_mode].value, modes[active_mode].speed);

    DeviceUpdateLEDs();
}

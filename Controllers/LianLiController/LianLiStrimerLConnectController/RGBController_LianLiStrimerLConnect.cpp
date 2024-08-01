/*---------------------------------------------------------*\
| RGBController_LianLiStrimerLConnect.cpp                   |
|                                                           |
|   RGBController for Lian Li Strimer L Connect             |
|                                                           |
|   Chris M (Dr_No)                             03 Jul 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LianLiStrimerLConnect.h"

/**------------------------------------------------------------------*\
    @name Lian Li Strimer L Connect
    @category LEDStrip
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectLianLiStrimerControllers
    @comment The Lian Li Strimer L Connect `Direct` mode stutters at high frame rates and
        and has been rate limited to ~10FPS.
\*-------------------------------------------------------------------*/

RGBController_LianLiStrimerLConnect::RGBController_LianLiStrimerLConnect(LianLiStrimerLConnectController *controller_ptr)
{
    controller                  = controller_ptr;

    name                        = "Lian Li Strimer L Connect";
    vendor                      = "Lian Li";
    type                        = DEVICE_TYPE_LEDSTRIP;
    description                 = controller->GetDeviceName();
    serial                      = controller->GetSerial();
    location                    = controller->GetLocation();

    mode Off;
    Off.name                    = "Off";
    Off.value                   = STRIMERLCONNECT_MODE_DIRECT;
    Off.brightness              = STRIMERLCONNECT_BRIGHTNESS_MIN;
    Off.color_mode              = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = STRIMERLCONNECT_MODE_DIRECT;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Direct.color_mode           = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Breathing              = CreateMode("Breathing", STRIMERLCONNECT_MODE_BREATHING, 0, MODE_COLORS_PER_LED);
    Breathing.flags            |= MODE_FLAG_HAS_PER_LED_COLOR;
    modes.push_back(Breathing);

    mode Flashing               = CreateMode("Flashing", STRIMERLCONNECT_MODE_FLASHING, 0, MODE_COLORS_PER_LED);
    Flashing.flags             |= MODE_FLAG_HAS_PER_LED_COLOR;
    modes.push_back(Flashing);

    mode BreathCycle            = CreateMode("Breathing Cycle", STRIMERLCONNECT_MODE_BREATHCYCLE, 0, MODE_COLORS_NONE);
    modes.push_back(BreathCycle);

    mode Rainbow                = CreateMode("Rainbow", STRIMERLCONNECT_MODE_RAINBOW, 0, MODE_COLORS_NONE);
    Rainbow.flags              |= MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(Rainbow);

    mode RainbowMorph           = CreateMode("Rainbow Morph", STRIMERLCONNECT_MODE_RAINBOWMORPH, 0, MODE_COLORS_NONE);
    modes.push_back(RainbowMorph);

    mode Snooker                = CreateMode("Snooker", STRIMERLCONNECT_MODE_SNOOKER, 6, MODE_COLORS_MODE_SPECIFIC);
    Snooker.flags              |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(Snooker);

    mode Mixing                 = CreateMode("Mixing", STRIMERLCONNECT_MODE_MIXING, 2, MODE_COLORS_MODE_SPECIFIC);
    Mixing.flags               |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(Mixing);

    mode PingPong               = CreateMode("Ping Pong", STRIMERLCONNECT_MODE_PINGPONG, 6, MODE_COLORS_MODE_SPECIFIC);
    PingPong.flags             |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(PingPong);

    mode Runway                 = CreateMode("Runway", STRIMERLCONNECT_MODE_RUNWAY, 2, MODE_COLORS_MODE_SPECIFIC);
    Runway.flags               |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(Runway);

    mode Painting               = CreateMode("Painting", STRIMERLCONNECT_MODE_PAINTING, 6, MODE_COLORS_MODE_SPECIFIC);
    Painting.flags             |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(Painting);

    mode Tide                   = CreateMode("Tide", STRIMERLCONNECT_MODE_TIDE, 6, MODE_COLORS_MODE_SPECIFIC);
    Tide.flags                 |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(Tide);

    mode BlowUp                 = CreateMode("Blow Up", STRIMERLCONNECT_MODE_BLOWUP, 6, MODE_COLORS_MODE_SPECIFIC);
    BlowUp.flags               |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(BlowUp);

    mode Meteor                 = CreateMode("Meteor", STRIMERLCONNECT_MODE_METEOR, 6, MODE_COLORS_MODE_SPECIFIC);
    Meteor.flags               |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(Meteor);

    mode ColorTransfer          = CreateMode("Color Transfer", STRIMERLCONNECT_MODE_COLORTRANSFER, 6, MODE_COLORS_MODE_SPECIFIC);
    ColorTransfer.flags        |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(ColorTransfer);

    mode FadeOut                = CreateMode("Fade Out", STRIMERLCONNECT_MODE_FADEOUT, 6, MODE_COLORS_MODE_SPECIFIC);
    FadeOut.flags              |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(FadeOut);

    mode Contest                = CreateMode("Contest", STRIMERLCONNECT_MODE_CONTEST, 6, MODE_COLORS_MODE_SPECIFIC);
    Contest.flags              |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(Contest);

    mode CrossOver              = CreateMode("Cross Over", STRIMERLCONNECT_MODE_CROSSOVER, 6, MODE_COLORS_MODE_SPECIFIC);
    CrossOver.flags            |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(CrossOver);

    mode BulletStack            = CreateMode("Bullet Stack", STRIMERLCONNECT_MODE_BULLETSTACK, 0, MODE_COLORS_NONE);
    BulletStack.flags          |= MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(BulletStack);

    mode Twinkle                = CreateMode("Twinkle", STRIMERLCONNECT_MODE_TWINKLE, 0, MODE_COLORS_NONE);
    modes.push_back(Twinkle);

    mode Parallel               = CreateMode("Parallel", STRIMERLCONNECT_MODE_PARALLEL, 6, MODE_COLORS_MODE_SPECIFIC);
    Parallel.flags             |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(Parallel);

    mode ShockWave              = CreateMode("Shock Wave", STRIMERLCONNECT_MODE_SHOCKWAVE, 6, MODE_COLORS_MODE_SPECIFIC);
    ShockWave.flags            |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(ShockWave);

    mode Ripple                 = CreateMode("Ripple", STRIMERLCONNECT_MODE_RIPPLE, 6, MODE_COLORS_MODE_SPECIFIC);
    Ripple.flags               |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(Ripple);

    mode Voice                  = CreateMode("Voice", STRIMERLCONNECT_MODE_VOICE, 6, MODE_COLORS_MODE_SPECIFIC);
    Voice.flags                |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    modes.push_back(Voice);

    mode Drizzling              = CreateMode("Drizzling", STRIMERLCONNECT_MODE_DRIZZLING, 6, MODE_COLORS_MODE_SPECIFIC);
    Drizzling.flags            |= MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_DIRECTION_LR;
    modes.push_back(Drizzling);

    Init_Controller();
    SetupZones();
}

RGBController_LianLiStrimerLConnect::~RGBController_LianLiStrimerLConnect()
{
    delete controller;
}

void RGBController_LianLiStrimerLConnect::Init_Controller()
{
    const uint8_t zone_split = STRIMERLCONNECT_STRIP_COUNT / 2;

    /*-------------------------------------------------*\
    | Create the device's controllable zones            |
    \*-------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zone_split; zone_idx++)
    {
        zone new_zone;
        new_zone.name           = "24 Pin ATX Strip ";
        new_zone.name.append(std::to_string(zone_idx));
        new_zone.type           = ZONE_TYPE_LINEAR;
        new_zone.leds_min       = 20;
        new_zone.leds_max       = 20;
        new_zone.leds_count     = new_zone.leds_max;
        new_zone.matrix_map     = NULL;
        zones.push_back(new_zone);
    }

    for(std::size_t zone_idx = zone_split; zone_idx < STRIMERLCONNECT_STRIP_COUNT; zone_idx++)
    {
        zone new_zone;
        new_zone.name           = "8 Pin GPU Strip ";
        new_zone.name.append(std::to_string(zone_idx - zone_split));
        new_zone.type           = ZONE_TYPE_LINEAR;
        new_zone.leds_min       = 27;
        new_zone.leds_max       = 27;
        new_zone.leds_count     = new_zone.leds_max;
        new_zone.matrix_map     = NULL;
        zones.push_back(new_zone);
    }
}

void RGBController_LianLiStrimerLConnect::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        for(unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led new_led;

            new_led.name = zones[zone_idx].name;
            new_led.name.append(" LED " + std::to_string(lp_idx));

            leds.push_back(new_led);
        }
    }

    SetupColors();
}

void RGBController_LianLiStrimerLConnect::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

bool RGBController_LianLiStrimerLConnect::TimeToSend()
{
    /*-----------------------------------------------------*\
    | Rate limit is 1000(ms) / wait_time in Frames Per Sec  |
    \*-----------------------------------------------------*/
    const uint8_t wait_time     = 90;

    return (std::chrono::steady_clock::now() - last_commit_time) > std::chrono::milliseconds(wait_time);
}

void RGBController_LianLiStrimerLConnect::DeviceUpdateLEDs()
{
    if(TimeToSend())
    {

        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            UpdateZoneLEDs((int)zone_idx);
        }

        controller->SendApply();

    /*-----------------------------------------------------*\
    | Update last commit time                               |
    \*-----------------------------------------------------*/
    last_commit_time    = std::chrono::steady_clock::now();
    }
}

void RGBController_LianLiStrimerLConnect::UpdateZoneLEDs(int zone)
{
    mode current_mode = modes[active_mode];

    controller->SetLedsDirect(zone, zones[zone].colors, zones[zone].leds_count);
    controller->SetMode(current_mode.value, zone, current_mode.speed, current_mode.brightness, current_mode.direction, false);
}

void RGBController_LianLiStrimerLConnect::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(GetLED_Zone(led));
    controller->SendApply();
}

void RGBController_LianLiStrimerLConnect::DeviceUpdateMode()
{
    if(TimeToSend())
    {
        mode current_mode       = modes[active_mode];

        if(current_mode.color_mode == MODE_COLORS_PER_LED)
        {
            return;
        }

        bool random_colours     = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);

        if(current_mode.color_mode == MODE_COLORS_NONE)
        {
            for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                controller->SetMode((uint8_t)current_mode.value, (uint8_t)zone_idx, current_mode.speed, current_mode.brightness, current_mode.direction, random_colours);
            }
        }
        else
        {
            for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
            {
                controller->SetLedsDirect((uint8_t)zone_idx, &current_mode.colors[0], (uint8_t)current_mode.colors.size());
                controller->SetMode((uint8_t)current_mode.value, (uint8_t)zone_idx, current_mode.speed, current_mode.brightness, current_mode.direction, random_colours);
            }
        }

        controller->SendApply();

        /*-----------------------------------------------------*\
        | Update last commit time                               |
        \*-----------------------------------------------------*/
        last_commit_time    = std::chrono::steady_clock::now();
    }
}

int RGBController_LianLiStrimerLConnect::GetLED_Zone(int led_idx)
{
    for(size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        int zone_start  = zones[zone_idx].start_idx;
        int zone_end    = zone_start + zones[zone_idx].leds_count - 1;

        if( zone_start <= led_idx && zone_end >= led_idx)
        {
            return((int)zone_idx);
        }
    }

    return(-1);
}

mode RGBController_LianLiStrimerLConnect::CreateMode(std::string name, int value, uint8_t colour_count, uint8_t colour_mode)
{
    mode new_mode;
    new_mode.name               = name;
    new_mode.value              = value;
    new_mode.colors_min         = colour_count;
    new_mode.colors_max         = colour_count;
    new_mode.colors.resize(colour_count);
    new_mode.flags              = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    new_mode.brightness_min     = STRIMERLCONNECT_BRIGHTNESS_MIN;
    new_mode.brightness_max     = STRIMERLCONNECT_BRIGHTNESS_MAX;
    new_mode.brightness         = STRIMERLCONNECT_BRIGHTNESS_MAX;
    new_mode.speed_min          = STRIMERLCONNECT_SPEED_SLOWEST;
    new_mode.speed_max          = STRIMERLCONNECT_SPEED_FASTEST;
    new_mode.speed              = STRIMERLCONNECT_SPEED_NORMAL;
    new_mode.color_mode         = colour_mode;

    return new_mode;
}

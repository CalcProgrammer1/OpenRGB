/*---------------------------------------------------------*\
| RGBController_PhilipsWiz.cpp                              |
|                                                           |
|   RGBController for Philips Wiz                           |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      03 Nov 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_PhilipsWiz.h"

/**------------------------------------------------------------------*\
    @name Philips Wiz
    @category Light
    @type Network
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectPhilipsWizControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_PhilipsWiz::RGBController_PhilipsWiz(PhilipsWizController* controller_ptr)
{
    controller  = controller_ptr;

    name        = controller->GetManufacturer() + " " + controller->GetName();
    vendor      = controller->GetManufacturer();
    type        = DEVICE_TYPE_LIGHT;
    version     = controller->GetVersion();
    description = "Philips Wiz Device";
    serial      = controller->GetUniqueID();
    location    = controller->GetLocation();

    mode Direct;
    Direct.name           = "Direct";
    Direct.value          = PHILLIPSWIZ_MODE_STATIC;
    Direct.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode     = MODE_COLORS_PER_LED;
    Direct.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
    Direct.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
    Direct.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
    modes.push_back(Direct);

    std::string model = controller->GetModuleName();
    if (model.find("RGB") != std::string::npos)
    {
        mode WarmWhite;
        WarmWhite.name           = "Warm White";
        WarmWhite.value          = PHILLIPSWIZ_MODE_WARM_WHITE;
        WarmWhite.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        WarmWhite.color_mode     = MODE_COLORS_PER_LED;
        WarmWhite.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        WarmWhite.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        WarmWhite.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(WarmWhite);

        mode Daylight;
        Daylight.name           = "Daylight";
        Daylight.value          = PHILLIPSWIZ_MODE_DAYLIGHT;
        Daylight.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Daylight.color_mode     = MODE_COLORS_PER_LED;
        Daylight.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Daylight.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Daylight.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Daylight);

        mode CoolWhite;
        CoolWhite.name           = "Cool White";
        CoolWhite.value          = PHILLIPSWIZ_MODE_COOL_WHITE;
        CoolWhite.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        CoolWhite.color_mode     = MODE_COLORS_PER_LED;
        CoolWhite.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        CoolWhite.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        CoolWhite.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(CoolWhite);

        mode Ocean;
        Ocean.name           = "Ocean";
        Ocean.value          = PHILLIPSWIZ_MODE_OCEAN;
        Ocean.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Ocean.color_mode     = MODE_COLORS_PER_LED;
        Ocean.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Ocean.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Ocean.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Ocean);

        mode Romance;
        Romance.name           = "Romance";
        Romance.value          = PHILLIPSWIZ_MODE_ROMANCE;
        Romance.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Romance.color_mode     = MODE_COLORS_PER_LED;
        Romance.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Romance.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Romance.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Romance);

        mode Sunset;
        Sunset.name           = "Sunset";
        Sunset.value          = PHILLIPSWIZ_MODE_SUNSET;
        Sunset.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Sunset.color_mode     = MODE_COLORS_PER_LED;
        Sunset.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Sunset.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Sunset.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Sunset);

        mode Party;
        Party.name           = "Party";
        Party.value          = PHILLIPSWIZ_MODE_PARTY;
        Party.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Party.color_mode     = MODE_COLORS_PER_LED;
        Party.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Party.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Party.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Party);

        mode Fireplace;
        Fireplace.name           = "Fireplace";
        Fireplace.value          = PHILLIPSWIZ_MODE_FIREPLACE;
        Fireplace.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Fireplace.color_mode     = MODE_COLORS_PER_LED;
        Fireplace.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Fireplace.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Fireplace.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Fireplace);

        mode Cozy;
        Cozy.name           = "Cozy";
        Cozy.value          = PHILLIPSWIZ_MODE_COZY;
        Cozy.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Cozy.color_mode     = MODE_COLORS_PER_LED;
        Cozy.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Cozy.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Cozy.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Cozy);

        mode Forest;
        Forest.name           = "Forest";
        Forest.value          = PHILLIPSWIZ_MODE_FOREST;
        Forest.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Forest.color_mode     = MODE_COLORS_PER_LED;
        Forest.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Forest.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Forest.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Forest);

        mode PastelColors;
        PastelColors.name           = "Pastel Colors";
        PastelColors.value          = PHILLIPSWIZ_MODE_PASTEL_COLORS;
        PastelColors.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        PastelColors.color_mode     = MODE_COLORS_PER_LED;
        PastelColors.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        PastelColors.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        PastelColors.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(PastelColors);

        mode WakeUp;
        WakeUp.name           = "Wake up";
        WakeUp.value          = PHILLIPSWIZ_MODE_WAKE_UP;
        WakeUp.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        WakeUp.color_mode     = MODE_COLORS_PER_LED;
        WakeUp.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        WakeUp.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        WakeUp.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(WakeUp);

        mode Bedtime;
        Bedtime.name           = "Bedtime";
        Bedtime.value          = PHILLIPSWIZ_MODE_BEDTIME;
        Bedtime.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Bedtime.color_mode     = MODE_COLORS_PER_LED;
        Bedtime.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Bedtime.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Bedtime.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Bedtime);

        mode NightLight;
        NightLight.name           = "Night light";
        NightLight.value          = PHILLIPSWIZ_MODE_NIGHT_LIGHT;
        NightLight.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        NightLight.color_mode     = MODE_COLORS_PER_LED;
        NightLight.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        NightLight.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        NightLight.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(NightLight);

        mode Focus;
        Focus.name           = "Focus";
        Focus.value          = PHILLIPSWIZ_MODE_FOCUS;
        Focus.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Focus.color_mode     = MODE_COLORS_PER_LED;
        Focus.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Focus.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Focus.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Focus);

        mode Relax;
        Relax.name           = "Relax";
        Relax.value          = PHILLIPSWIZ_MODE_RELAX;
        Relax.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Relax.color_mode     = MODE_COLORS_PER_LED;
        Relax.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Relax.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Relax.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Relax);

        mode TrueColors;
        TrueColors.name           = "True colors";
        TrueColors.value          = PHILLIPSWIZ_MODE_TRUE_COLORS;
        TrueColors.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        TrueColors.color_mode     = MODE_COLORS_PER_LED;
        TrueColors.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        TrueColors.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        TrueColors.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(TrueColors);

        mode TvTime;
        TvTime.name           = "TV time";
        TvTime.value          = PHILLIPSWIZ_MODE_TV_TIME;
        TvTime.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        TvTime.color_mode     = MODE_COLORS_PER_LED;
        TvTime.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        TvTime.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        TvTime.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(TvTime);

        mode Plantgrowth;
        Plantgrowth.name           = "Plantgrowth";
        Plantgrowth.value          = PHILLIPSWIZ_MODE_PLANTGROWTH;
        Plantgrowth.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Plantgrowth.color_mode     = MODE_COLORS_PER_LED;
        Plantgrowth.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Plantgrowth.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Plantgrowth.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Plantgrowth);

        mode Spring;
        Spring.name           = "Spring";
        Spring.value          = PHILLIPSWIZ_MODE_SPRING;
        Spring.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Spring.color_mode     = MODE_COLORS_PER_LED;
        Spring.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Spring.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Spring.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Spring);

        mode Summer;
        Summer.name           = "Summer";
        Summer.value          = PHILLIPSWIZ_MODE_SUMMER;
        Summer.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Summer.color_mode     = MODE_COLORS_PER_LED;
        Summer.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Summer.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Summer.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Summer);

        mode Fall;
        Fall.name           = "Fall";
        Fall.value          = PHILLIPSWIZ_MODE_FALL;
        Fall.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Fall.color_mode     = MODE_COLORS_PER_LED;
        Fall.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Fall.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Fall.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Fall);

        mode Deepdive;
        Deepdive.name           = "Deepdive";
        Deepdive.value          = PHILLIPSWIZ_MODE_DEEPDIVE;
        Deepdive.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Deepdive.color_mode     = MODE_COLORS_PER_LED;
        Deepdive.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Deepdive.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Deepdive.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Deepdive);

        mode Jungle;
        Jungle.name           = "Jungle";
        Jungle.value          = PHILLIPSWIZ_MODE_JUNGLE;
        Jungle.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Jungle.color_mode     = MODE_COLORS_PER_LED;
        Jungle.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Jungle.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Jungle.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Jungle);

        mode Mojito;
        Mojito.name           = "Mojito";
        Mojito.value          = PHILLIPSWIZ_MODE_MOJITO;
        Mojito.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Mojito.color_mode     = MODE_COLORS_PER_LED;
        Mojito.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Mojito.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Mojito.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Mojito);

        mode Club;
        Club.name           = "Club";
        Club.value          = PHILLIPSWIZ_MODE_CLUB;
        Club.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Club.color_mode     = MODE_COLORS_PER_LED;
        Club.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Club.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Club.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Club);

        mode Christmas;
        Christmas.name           = "Christmas";
        Christmas.value          = PHILLIPSWIZ_MODE_CHRISTMAS;
        Christmas.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Christmas.color_mode     = MODE_COLORS_PER_LED;
        Christmas.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Christmas.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Christmas.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Christmas);

        mode Halloween;
        Halloween.name           = "Halloween";
        Halloween.value          = PHILLIPSWIZ_MODE_HALLOWEEN;
        Halloween.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Halloween.color_mode     = MODE_COLORS_PER_LED;
        Halloween.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Halloween.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Halloween.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Halloween);

        mode Candlelight;
        Candlelight.name           = "Candlelight";
        Candlelight.value          = PHILLIPSWIZ_MODE_CANDLELIGHT;
        Candlelight.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Candlelight.color_mode     = MODE_COLORS_PER_LED;
        Candlelight.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Candlelight.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Candlelight.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Candlelight);

        mode GoldenWhite;
        GoldenWhite.name           = "Golden white";
        GoldenWhite.value          = PHILLIPSWIZ_MODE_GOLDEN_WHITE;
        GoldenWhite.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        GoldenWhite.color_mode     = MODE_COLORS_PER_LED;
        GoldenWhite.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        GoldenWhite.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        GoldenWhite.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(GoldenWhite);

        mode Pulse;
        Pulse.name           = "Pulse";
        Pulse.value          = PHILLIPSWIZ_MODE_PULSE;
        Pulse.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Pulse.color_mode     = MODE_COLORS_PER_LED;
        Pulse.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Pulse.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Pulse.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Pulse);

        mode Steampunk;
        Steampunk.name           = "Steampunk";
        Steampunk.value          = PHILLIPSWIZ_MODE_STEAMPUNK;
        Steampunk.flags          = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
        Steampunk.color_mode     = MODE_COLORS_PER_LED;
        Steampunk.brightness_min = PHILIPSWIZ_BRIGHTNESS_MIN;
        Steampunk.brightness_max = PHILIPSWIZ_BRIGHTNESS_MAX;
        Steampunk.brightness     = PHILIPSWIZ_BRIGHTNESS_MAX;
        modes.push_back(Steampunk);

    }

    SetupZones();
}

RGBController_PhilipsWiz::~RGBController_PhilipsWiz()
{
    delete controller;
}

void RGBController_PhilipsWiz::SetupZones()
{
    zone led_zone;
    led_zone.name       = "RGB Light";
    led_zone.type       = ZONE_TYPE_SINGLE;
    led_zone.leds_min   = 1;
    led_zone.leds_max   = 1;
    led_zone.leds_count = 1;
    led_zone.matrix_map = NULL;
    zones.push_back(led_zone);

    led new_led;
    new_led.name = "RGB Light";

    leds.push_back(new_led);

    SetupColors();
}

void RGBController_PhilipsWiz::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_PhilipsWiz::DeviceUpdateLEDs()
{
    if (modes[active_mode].value == PHILLIPSWIZ_MODE_STATIC)
    {
        unsigned char red = RGBGetRValue(colors[0]);
        unsigned char grn = RGBGetGValue(colors[0]);
        unsigned char blu = RGBGetBValue(colors[0]);

        controller->SetColor(red, grn, blu, modes[active_mode].brightness);
    }
}

void RGBController_PhilipsWiz::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsWiz::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_PhilipsWiz::DeviceUpdateMode()
{
    if (modes[active_mode].value != PHILLIPSWIZ_MODE_STATIC)
    {
        controller->SetScene(modes[active_mode].value, modes[active_mode].brightness);
    }
}

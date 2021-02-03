/*-----------------------------------------*\
|  RGBController_SapphireNitroGlowV3.cpp    |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Sapphire Nitro Glow V3 GPU Driver        |
|                                           |
|  K900 2/3/2021                            |
\*-----------------------------------------*/

#include "RGBController_SapphireNitroGlowV3.h"

RGBController_SapphireNitroGlowV3::RGBController_SapphireNitroGlowV3(SapphireNitroGlowV3Controller* sapphire_ptr)
{
    sapphire = sapphire_ptr;

    name        = "Sapphire Nitro Glow V3 Device";
    vendor      = "Sapphire";
    description = "Sapphire Nitro Glow V3 Device";
    location    = sapphire->GetDeviceLocation();

    type        = DEVICE_TYPE_GPU;

    mode Static;
    Static.name       = "Static";
    Static.value      = SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Rainbow;
    Rainbow.name       = "Rainbow";
    Rainbow.value      = SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW;
    Rainbow.flags      = MODE_FLAG_HAS_SPEED;
    Rainbow.speed_min  = 10;
    Rainbow.speed_max  = 250;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode Runway;
    Runway.name       = "Runway";
    Runway.value      = SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY;
    Runway.flags      = MODE_FLAG_HAS_SPEED;
    Runway.speed_min  = 5;
    Runway.speed_max  = 50;
    Runway.color_mode = MODE_COLORS_NONE;
    modes.push_back(Runway);

    mode ColorCycle;
    ColorCycle.name       = "Color Cycle";
    ColorCycle.value      = SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE;
    ColorCycle.flags      = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min  = 30;
    ColorCycle.speed_max  = 1;
    ColorCycle.color_mode = MODE_COLORS_NONE;
    modes.push_back(ColorCycle);

    mode Serial;
    Serial.name       = "Serial";
    Serial.value      = SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL;
    Serial.flags      = MODE_FLAG_HAS_SPEED;
    Serial.speed_min  = 255;
    Serial.speed_max  = 5;
    Serial.color_mode = MODE_COLORS_NONE;
    modes.push_back(Serial);

    mode ExternalControl;
    ExternalControl.name       = "External Control";
    ExternalControl.value      = SAPPHIRE_NITRO_GLOW_V3_MODE_EXTERNAL_CONTROL;
    ExternalControl.flags      = 0;
    ExternalControl.color_mode = MODE_COLORS_NONE;
    modes.push_back(ExternalControl);

    SetupZones();

    ReadConfiguration();
}

RGBController_SapphireNitroGlowV3::~RGBController_SapphireNitroGlowV3()
{
    delete sapphire;
}

void RGBController_SapphireNitroGlowV3::SetupZones()
{
    /*---------------------------------------------------------*\
    | This device only has one LED, so create a single zone and |
    | LED for it                                                |
    \*---------------------------------------------------------*/
    zone* new_zone = new zone();
    led*  new_led  = new led();

    new_zone->name          = "GPU Zone";
    new_zone->type          = ZONE_TYPE_SINGLE;
    new_zone->leds_min      = 1;
    new_zone->leds_max      = 1;
    new_zone->leds_count    = 1;
    new_zone->matrix_map    = NULL;

    new_led->name           = "GPU LED";

    /*---------------------------------------------------------*\
    | Push the zone and LED on to device vectors                |
    \*---------------------------------------------------------*/
    leds.push_back(*new_led);
    zones.push_back(*new_zone);

    SetupColors();
}

void RGBController_SapphireNitroGlowV3::ReadConfiguration()
{
    modes[1].speed = sapphire->GetRainbowAnimationSpeed();
    modes[2].speed = sapphire->GetRunwayAnimationSpeed();
    modes[3].speed = sapphire->GetColorCycleAnimationSpeed();
    modes[4].speed = sapphire->GetSerialAnimationSpeed();

    colors[0] = ToRGBColor(
        sapphire->GetRed(),
        sapphire->GetBlue(),
        sapphire->GetGreen()
    );

    if(sapphire->GetExternalControl())
    {
        active_mode = 5;
        return;
    }

    switch(sapphire->GetMode())
    {
        case SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM:
            active_mode = 0;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW:
            active_mode = 1;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY:
            active_mode = 2;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE:
            active_mode = 3;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL:
            active_mode = 4;
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_OFF:
            active_mode = 0;
            colors[0] = ToRGBColor(0, 0, 0);
            break;

        default:
            active_mode = 0;
            break;
    }
}

void RGBController_SapphireNitroGlowV3::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_SapphireNitroGlowV3::DeviceUpdateLEDs()
{
    RGBColor      color = colors[0];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);

    sapphire->SetColor(red, grn, blu);
}

void RGBController_SapphireNitroGlowV3::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV3::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_SapphireNitroGlowV3::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_SapphireNitroGlowV3::DeviceUpdateMode()
{
    auto mode = modes[active_mode];

    switch(mode.value)
    {
        case SAPPHIRE_NITRO_GLOW_V3_MODE_CUSTOM:
            sapphire->SetExternalControl(false);
            sapphire->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RAINBOW:
            sapphire->SetExternalControl(false);
            sapphire->SetRainbowAnimationSpeed(mode.speed);
            sapphire->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_RUNWAY:
            sapphire->SetExternalControl(false);
            sapphire->SetRunwayAnimationSpeed(mode.speed);
            sapphire->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_COLOR_CYCLE:
            sapphire->SetExternalControl(false);
            sapphire->SetColorCycleAnimationSpeed(mode.speed);
            sapphire->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_SERIAL:
            sapphire->SetExternalControl(false);
            sapphire->SetSerialAnimationSpeed(mode.speed);
            sapphire->SetMode(mode.value);
            break;

        case SAPPHIRE_NITRO_GLOW_V3_MODE_EXTERNAL_CONTROL:
            sapphire->SetExternalControl(true);
            break;
    }
}

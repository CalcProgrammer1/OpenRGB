/*-------------------------------------------------------------------*\
|  RGBController_XPGSpectrixS40G.cpp	                              |
|                                                                     |
|  Driver for XPG's Spectrix S40G NVMe                                |
|                                                                     |
|  NicolasNewman          25th Mar 2021                               |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_XPGSpectrixS40G.h"

RGBController_XPGSpectrixS40G::RGBController_XPGSpectrixS40G(XPGSpectrixS40GController* spectrix_ptr)
{
    xpg = spectrix_ptr;

    name = "XPG Spectrix S40G";
    vendor = "XPG";
    type = DEVICE_TYPE_STORAGE;
    description = "XPG Spectrix S40G RGB NVMe";

    mode Static;
    Static.name = "Static";
    Static.value = XPG_SPECTRIX_S40G_MODE_STATIC;
    Static.flags = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Breathing;
    Breathing.name = "Breathing";
    Breathing.value = XPG_SPECTRIX_S40G_MODE_BREATHING;
    Breathing.flags = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    Breathing.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    Breathing.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    Breathing.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Breathing);

    mode Strobing;
    Strobing.name = "Strobing";
    Strobing.value = XPG_SPECTRIX_S40G_MODE_STROBING;
    Strobing.flags = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Strobing.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    Strobing.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    Strobing.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    Strobing.color_mode = MODE_COLORS_PER_LED;

    mode Cycle;
    Cycle.name = "Cycle";
    Cycle.value = XPG_SPECTRIX_S40G_MODE_CYCLE;
    Cycle.flags = MODE_FLAG_HAS_SPEED; // MODE SPECIFIC COLOR correct...?
    Cycle.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    Cycle.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    Cycle.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    Cycle.color_mode = MODE_COLORS_NONE;

    mode Rainbow;
    Rainbow.name = "Rainbow";
    Rainbow.value = XPG_SPECTRIX_S40G_MODE_RAINBOW;
    Rainbow.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    Rainbow.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    Rainbow.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    Rainbow.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    Rainbow.direction = MODE_DIRECTION_RIGHT;
    Rainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(Rainbow);

    mode BreathingRainbow;
    BreathingRainbow.name = "Breathing Rainbow";
    BreathingRainbow.value = XPG_SPECTRIX_S40G_MODE_BREATHING_RAINBOW;
    BreathingRainbow.flags = MODE_FLAG_HAS_SPEED;
    BreathingRainbow.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    BreathingRainbow.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    BreathingRainbow.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    BreathingRainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(BreathingRainbow);

    // mode Comet;
    // Comet.name = "Comet";
    // Comet.value = XPG_SPECTRIX_S40G_MODE_COMET;
    // Comet.flags =  MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    // Comet.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    // Comet.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    // Comet.colors_min = 1;
    // Comet.colors_max = 1;
    // Comet.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    // Comet.color_mode = MODE_COLORS_MODE_SPECIFIC;
    // Comet.colors.resize(1);
    // modes.push_back(Comet);

    mode CometRainbow;
    CometRainbow.name = "Comet Rainbow";
    CometRainbow.value = XPG_SPECTRIX_S40G_MODE_COMET_RAINBOW;
    CometRainbow.flags = MODE_FLAG_HAS_SPEED;
    CometRainbow.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    CometRainbow.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    CometRainbow.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    CometRainbow.color_mode = MODE_COLORS_NONE;
    modes.push_back(CometRainbow);

    // mode Flash;
    // Flash.name = "Flash";
    // Flash.value = XPG_SPECTRIX_S40G_MODE_FLASH;

    // TODO not functioning properly
    // mode FlashRainbow;
    // FlashRainbow.name = "Flash Rainbow";
    // FlashRainbow.value = XPG_SPECTRIX_S40G_MODE_FLASH_RAINBOW;
    // FlashRainbow.flags = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR;
    // FlashRainbow.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    // FlashRainbow.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    // FlashRainbow.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    // FlashRainbow.direction = MODE_DIRECTION_RIGHT;
    // FlashRainbow.color_mode = MODE_COLORS_NONE;
    // modes.push_back(FlashRainbow);

    // TODO not functioning properly
    // mode Wave;
    // Wave.name = "Wave";
    // Wave.value = XPG_SPECTRIX_S40G_MODE_WAVE;
    // Wave.flags = MODE_FLAG_HAS_SPEED;
    // Wave.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    // Wave.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    // Wave.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    // Wave.color_mode = MODE_COLORS_NONE;
    // modes.push_back(Wave);

    // TODO not functioning properly
    // mode GlowingYoyo;
    // GlowingYoyo.name = "Glowing Yoyo";
    // GlowingYoyo.value = XPG_SPECTRIX_S40G_MODE_YOYO;
    // GlowingYoyo.flags = MODE_FLAG_HAS_SPEED;
    // GlowingYoyo.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    // GlowingYoyo.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    // GlowingYoyo.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    // GlowingYoyo.color_mode = MODE_COLORS_NONE;
    // modes.push_back(GlowingYoyo);

    // TODO not functioning properly
    // mode StarryNight;
    // StarryNight.name = "Starry Night";
    // StarryNight.value = XPG_SPECTRIX_S40G_MODE_STARRY_NIGHT;
    // StarryNight.flags = MODE_FLAG_HAS_SPEED;
    // StarryNight.speed_min = XPG_SPECTRIX_S40G_SPEED_SLOWEST;
    // StarryNight.speed_max = XPG_SPECTRIX_S40G_SPEED_FASTEST;
    // StarryNight.speed = XPG_SPECTRIX_S40G_SPEED_NORMAL;
    // StarryNight.color_mode = MODE_COLORS_NONE;
    // modes.push_back(StarryNight);

    SetupZones();
}

RGBController_XPGSpectrixS40G::~RGBController_XPGSpectrixS40G()
{
    delete xpg;
}

void RGBController_XPGSpectrixS40G::SetupZones()
{
    zone new_zone;
    new_zone.name = "XPG Spectrix S40G Zone";
    new_zone.type = ZONE_TYPE_LINEAR;
    new_zone.leds_min       = xpg->GetLEDCount();
    new_zone.leds_max       = xpg->GetLEDCount();
    new_zone.leds_count     = xpg->GetLEDCount();
    new_zone.matrix_map     = NULL;
    zones.push_back(new_zone);

    for(std::size_t led_indx = 0; led_indx < zones[0].leds_count; led_indx++) 
    {
        led* new_led = new led();
        new_led->name = "XPG Spectrix LED ";
        new_led->name.append(std::to_string(led_indx));
        leds.push_back(*new_led);
    }

    SetupColors();
}

void RGBController_XPGSpectrixS40G::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_XPGSpectrixS40G::DeviceUpdateLEDs() {
    for (std::size_t led = 0; led < colors.size(); led++)
    {
        RGBColor      color = colors[led];
        uint8_t red   = RGBGetRValue(color);
        uint8_t grn   = RGBGetGValue(color);
        uint8_t blu   = RGBGetBValue(color);
        
        xpg->SetLEDColor(led, red, grn, blu);
    }

    xpg->ApplyColors();
}

void RGBController_XPGSpectrixS40G::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_XPGSpectrixS40G::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    uint8_t red   = RGBGetRValue(color);
    uint8_t grn   = RGBGetGValue(color);
    uint8_t blu   = RGBGetBValue(color);

    xpg->SetLEDColor(led, red, grn, blu);
    xpg->ApplyColors();
}

void RGBController_XPGSpectrixS40G::SetCustomMode() // TODO
{
    active_mode = 0;
}

void RGBController_XPGSpectrixS40G::DeviceUpdateMode()
{
    uint8_t direction = 0;
    uint8_t speed = 0x0;
    uint8_t mode_colors[3];

    switch(modes[active_mode].direction)
    {
        case MODE_DIRECTION_RIGHT:
            direction = XPG_SPECTRIX_S40G_DIRECTION_DEFAULT;
            break;
        case MODE_DIRECTION_LEFT:
            direction = XPG_SPECTRIX_S40G_DIRECTION_REVERSE;
            break;
        default:
            direction = 0x00;
    }

    switch(modes[active_mode].speed)
    {
        case XPG_SPECTRIX_S40G_SPEED_SLOWEST:
            speed = 0xFF;
            break;
        case XPG_SPECTRIX_S40G_SPEED_SLOW:
            speed = 0xFE;
            break;
        case XPG_SPECTRIX_S40G_SPEED_NORMAL:
            speed = 0x00;
            break;
        case XPG_SPECTRIX_S40G_SPEED_FAST:
            speed = 0x01;
            break;
        case XPG_SPECTRIX_S40G_SPEED_FASTEST:
            speed = 0x02;
            break;
        default:
            speed = 0x00;
    }

    mode_colors[0] = 0;
    mode_colors[1] = 0;
    mode_colors[2] = 0;

    if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        mode_colors[0] = RGBGetRValue(modes[active_mode].colors[0]);
        mode_colors[1] = RGBGetGValue(modes[active_mode].colors[0]);
        mode_colors[2] = RGBGetBValue(modes[active_mode].colors[0]);
    }

    xpg->SetDirection(0x00);
    xpg->SetSpeed(0x00);
    switch(modes[active_mode].value)
    {
        case XPG_SPECTRIX_S40G_MODE_STATIC:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_STATIC);
            break;
        case XPG_SPECTRIX_S40G_MODE_BREATHING:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_BREATHING);
            xpg->SetSpeed(speed);
            break;
        case XPG_SPECTRIX_S40G_MODE_STROBING:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_STROBING);
            xpg->SetSpeed(speed);
            break;
        case XPG_SPECTRIX_S40G_MODE_CYCLE:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_CYCLE);
            xpg->SetSpeed(speed);
            break;
        case XPG_SPECTRIX_S40G_MODE_RAINBOW:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_RAINBOW);
            xpg->SetSpeed(speed);
            xpg->SetDirection(direction);
            break;
        case XPG_SPECTRIX_S40G_MODE_BREATHING_RAINBOW:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_BREATHING_RAINBOW);
            xpg->SetSpeed(speed);
            break;
        case XPG_SPECTRIX_S40G_MODE_COMET:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_COMET);
            xpg->SetSpeed(speed);
            xpg->SetAllColors(mode_colors[0], mode_colors[1], mode_colors[2]);
            break;
        case XPG_SPECTRIX_S40G_MODE_COMET_RAINBOW:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_COMET_RAINBOW);
            xpg->SetSpeed(speed);
            break;
        case XPG_SPECTRIX_S40G_MODE_FLASH:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_FLASH);
            xpg->SetSpeed(speed);
            xpg->SetDirection(direction);
            xpg->SetAllColors(mode_colors[0], mode_colors[1], mode_colors[2]);
            break;
        case XPG_SPECTRIX_S40G_MODE_FLASH_RAINBOW:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_FLASH_RAINBOW);
            xpg->SetSpeed(speed);
            xpg->SetDirection(direction);
            break;
        case XPG_SPECTRIX_S40G_MODE_WAVE:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_WAVE);
            xpg->SetSpeed(speed);
            break;
        case XPG_SPECTRIX_S40G_MODE_YOYO:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_YOYO);
            xpg->SetSpeed(speed);
            break;
        case XPG_SPECTRIX_S40G_MODE_STARRY_NIGHT:
            xpg->SetMode(XPG_SPECTRIX_S40G_MODE_STARRY_NIGHT);
            xpg->SetSpeed(speed);
            break;
    }
    xpg->ApplyColors();
}

#include "RGBController_RtkBridge.h"

/**------------------------------------------------------------------*\
    @name Realtek Bridge Device
    @category Keyboard,LEDStrip
    @type USB
    @save :x:
    @direct :rotating_light:
    @effects :white_check_mark:
    @detectors DetectRtkBridgeControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RtkBridge::RGBController_RtkBridge(RtkBridgeWrapper *_wrapper)
{
    this->wrapper = _wrapper;

    name        = wrapper->get_product_name();
    vendor      = wrapper->get_manu_name();
    location    = wrapper->get_dev_loc();
    serial      = wrapper->get_sn();
    version     = wrapper->get_fw_ver();
    description = vendor + "Storage Device";
    type        = DEVICE_TYPE_STORAGE;

    SetupStrip();
    SetupZones();
}

void RGBController_RtkBridge::SetupStrip()
{
    int brightness = wrapper->get_argb_brightness() >> 8;

    mode Direct;
    Direct.name                   = "Direct";
    Direct.value                  = LED_EFF_NONE;
    Direct.flags                  = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Direct.color_mode             = MODE_COLORS_PER_LED;
    Direct.brightness_min         = 0;
    Direct.brightness_max         = 255;
    Direct.brightness             = brightness;
    modes.push_back(Direct);

    mode Static;
    Static.name                   = "Static";
    Static.value                  = LED_EFF_ALWAYS;
    Static.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
    Static.colors_min             = 1;
    Static.colors_max             = 1;
    Static.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    Static.brightness_min         = 0;
    Static.brightness_max         = 255;
    Static.brightness             = brightness;
    modes.push_back(Static);

    mode Blink;
    Blink.name                    = "Blink";
    Blink.value                   = LED_EFF_BLINK;
    Blink.flags                   = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Blink.speed_min               = RTK_BRIDGE_SPEED_MIN;
    Blink.speed_max               = RTK_BRIDGE_SPEED_MAX;
    Blink.speed                   = RTK_BRIDGE_SPEED_NORMAL;
    Blink.colors_min              = 1;
    Blink.colors_max              = 1;
    Blink.color_mode              = MODE_COLORS_MODE_SPECIFIC;
    Blink.colors.resize(1);
    Blink.brightness_min          = 0;
    Blink.brightness_max          = 255;
    Blink.brightness              = brightness;
    modes.push_back(Blink);

    mode Breathing;
    Breathing.name                = "Breathing";
    Breathing.value               = LED_EFF_BREATHE;
    Breathing.flags               = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Breathing.speed_min           = RTK_BRIDGE_SPEED_MIN;
    Breathing.speed_max           = RTK_BRIDGE_SPEED_MAX;
    Breathing.speed               = RTK_BRIDGE_SPEED_NORMAL;
    Breathing.colors_min          = 1;
    Breathing.colors_max          = 1;
    Breathing.color_mode          = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.brightness_min      = 0;
    Breathing.brightness_max      = 255;
    Breathing.brightness          = brightness;
    modes.push_back(Breathing);

    mode Spectrum;
    Spectrum.name                 = "Spectrum";
    Spectrum.value                = LED_EFF_SPECTRUM;
    Spectrum.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Spectrum.speed_min            = RTK_BRIDGE_SPEED_MIN;
    Spectrum.speed_max            = RTK_BRIDGE_SPEED_MAX;
    Spectrum.speed                = RTK_BRIDGE_SPEED_NORMAL;
    Spectrum.color_mode           = MODE_COLORS_NONE;
    Spectrum.brightness_min       = 0;
    Spectrum.brightness_max       = 255;
    Spectrum.brightness           = brightness;
    modes.push_back(Spectrum);

    mode Scroll;
    Scroll.name                   = "Scroll";
    Scroll.value                  = LED_EFF_SCROLL;
    Scroll.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Scroll.speed_min              = RTK_BRIDGE_SPEED_MIN;
    Scroll.speed_max              = RTK_BRIDGE_SPEED_MAX;
    Scroll.speed                  = RTK_BRIDGE_SPEED_NORMAL;
    Scroll.colors_min             = 1;
    Scroll.colors_max             = 1;
    Scroll.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Scroll.colors.resize(1);
    Scroll.brightness_min         = 0;
    Scroll.brightness_max         = 255;
    Scroll.brightness             = brightness;
    modes.push_back(Scroll);

    mode RainbowScroll;
    RainbowScroll.name            = "RainbowScroll";
    RainbowScroll.value           = LED_EFF_RAINBOW_SCROLL;
    RainbowScroll.flags           = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RainbowScroll.speed_min       = RTK_BRIDGE_SPEED_MIN;
    RainbowScroll.speed_max       = RTK_BRIDGE_SPEED_MAX;
    RainbowScroll.speed           = RTK_BRIDGE_SPEED_NORMAL;
    RainbowScroll.color_mode      = MODE_COLORS_NONE;
    RainbowScroll.brightness_min  = 0;
    RainbowScroll.brightness_max  = 255;
    RainbowScroll.brightness      = brightness;
    modes.push_back(RainbowScroll);

    mode RunningWater;
    RunningWater.name             = "RunningWater";
    RunningWater.value            = LED_EFF_RUNNING_WATER;
    RunningWater.flags            = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    RunningWater.speed_min        = RTK_BRIDGE_SPEED_MIN;
    RunningWater.speed_max        = RTK_BRIDGE_SPEED_MAX;
    RunningWater.speed            = RTK_BRIDGE_SPEED_NORMAL;
    RunningWater.colors_min       = 1;
    RunningWater.colors_max       = 1;
    RunningWater.color_mode       = MODE_COLORS_MODE_SPECIFIC;
    RunningWater.colors.resize(1);
    RunningWater.brightness_min   = 0;
    RunningWater.brightness_max   = 255;
    RunningWater.brightness       = brightness;
    modes.push_back(RunningWater);

    mode Sliding;
    Sliding.name                   = "Sliding";
    Sliding.value                  = LED_EFF_SLIDING;
    Sliding.flags                  = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Sliding.speed_min              = RTK_BRIDGE_SPEED_MIN;
    Sliding.speed_max              = RTK_BRIDGE_SPEED_MAX;
    Sliding.speed                  = RTK_BRIDGE_SPEED_NORMAL;
    Sliding.color_mode             = MODE_COLORS_NONE;
    Sliding.brightness_min         = 0;
    Sliding.brightness_max         = 255;
    Sliding.brightness             = brightness;
    modes.push_back(Sliding);

    mode NewtonCradle;
    NewtonCradle.name             = "NewtonCradle";
    NewtonCradle.value            = LED_EFF_NEWTON_CRADLE;
    NewtonCradle.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    NewtonCradle.speed_min        = RTK_BRIDGE_SPEED_MIN;
    NewtonCradle.speed_max        = RTK_BRIDGE_SPEED_MAX;
    NewtonCradle.speed            = RTK_BRIDGE_SPEED_NORMAL;
    NewtonCradle.color_mode       = MODE_COLORS_NONE;
    NewtonCradle.brightness_min   = 0;
    NewtonCradle.brightness_max   = 255;
    NewtonCradle.brightness       = brightness;
    modes.push_back(NewtonCradle);

    mode Meteor;
    Meteor.name                   = "Meteor";
    Meteor.value                  = LED_EFF_METEOR;
    Meteor.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    Meteor.speed_min              = RTK_BRIDGE_SPEED_MIN;
    Meteor.speed_max              = RTK_BRIDGE_SPEED_MAX;
    Meteor.speed                  = RTK_BRIDGE_SPEED_NORMAL;
    Meteor.colors_min             = 1;
    Meteor.colors_max             = 1;
    Meteor.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Meteor.colors.resize(1);
    Meteor.brightness_min         = 0;
    Meteor.brightness_max         = 255;
    Meteor.brightness             = brightness;
    modes.push_back(Meteor);
}

RGBController_RtkBridge::~RGBController_RtkBridge()
{
    delete wrapper;
}

void RGBController_RtkBridge::SetupZones()
{
    zone argb_zone;

    argb_zone.name       = "strip";
    argb_zone.type       = ZONE_TYPE_LINEAR;
    argb_zone.leds_min   = wrapper->get_argb_num();
    argb_zone.leds_max   = wrapper->get_argb_num();
    argb_zone.leds_count = wrapper->get_argb_num();
    argb_zone.matrix_map = NULL;

    zones.push_back(argb_zone);

    for(unsigned int led_idx = 0; led_idx < argb_zone.leds_count; led_idx++)
    {
        led StripLED;
        StripLED.name = argb_zone.name + " ";
        StripLED.name.append(std::to_string(led_idx + 1));
        leds.push_back(StripLED);
    }

    SetupColors();
}

void RGBController_RtkBridge::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RtkBridge::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
}

void RGBController_RtkBridge::UpdateZoneLEDs(int /*zone*/)
{
    unsigned short brightness = 0xFF;
    mode& curr_mode = modes[active_mode];

    if(curr_mode.color_mode == MODE_COLORS_PER_LED &&
       curr_mode.value == LED_EFF_NONE)       //direct mode
    {
        if (curr_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
            brightness = curr_mode.brightness;

        wrapper->set_argb_direct(colors, brightness);
    }
    else
        UpdateSingleLED(0);
}

void RGBController_RtkBridge::UpdateSingleLED(int led)
{
    unsigned char speed = RTK_BRIDGE_SPEED_NORMAL;
    unsigned char dir = 0;
    unsigned short brightness = 0xFF;
    mode& curr_mode = modes[active_mode];
    std::vector<RGBColor> rtk_colors = curr_mode.colors;

    if(curr_mode.flags & MODE_FLAG_HAS_SPEED)
    {
        speed = curr_mode.speed;
    }
    if (curr_mode.flags & MODE_FLAG_HAS_BRIGHTNESS)
    {
        brightness = curr_mode.brightness;
    }
    if(curr_mode.flags & MODE_FLAG_HAS_DIRECTION_LR)
    {
        if(curr_mode.direction == MODE_DIRECTION_RIGHT)
        {
            dir = 1;
        }
    }

    if (curr_mode.color_mode == MODE_COLORS_PER_LED)
    {
        rtk_colors = colors;
    }
    else if(curr_mode.color_mode == MODE_COLORS_NONE)
    {
        rtk_colors.clear();
    }

    wrapper->set_argb_effect(curr_mode.value, rtk_colors, speed, brightness);
}

void RGBController_RtkBridge::DeviceUpdateMode()
{
    if(modes[active_mode].value != LED_EFF_NONE)
    {
        DeviceUpdateLEDs();
    }
}

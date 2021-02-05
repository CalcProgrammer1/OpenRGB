/*-----------------------------------------*\
|  RGBController_RoccatKoneAimo.cpp         |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|                                           |
|                                           |
|  Thibaud M (enlight3d) 17/11/2020         |
\*-----------------------------------------*/

#include "RGBController_RoccatKoneAimo.h"

RGBController_RoccatKoneAimo::RGBController_RoccatKoneAimo(RoccatKoneAimoController* mouse_ptr)
{
    name        = "Roccat Kone Aimo";
    vendor      = "Roccat";
    type        = DEVICE_TYPE_MOUSE;
    description = "Roccat Kone Aimo Mouse";
    mouse       = mouse_ptr;
    location    = mouse->GetLocation();
    serial      = mouse->GetSerial();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.speed_min  = 0;
    Direct.speed_max  = 0;
    Direct.speed      = 0;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    active_mode = 0;

    SetupZones();

}

RGBController_RoccatKoneAimo::~RGBController_RoccatKoneAimo()
{
    delete mouse;
}

void RGBController_RoccatKoneAimo::SetupZones()
{
    /*---------------------------------------------------------*\
    | Set up zones and leds per zone                            |
    \*---------------------------------------------------------*/
    zone WHEEL_zone;
    WHEEL_zone.name          = "Scroll Wheel";
    WHEEL_zone.type          = ZONE_TYPE_SINGLE;
    WHEEL_zone.leds_min      = 1;
    WHEEL_zone.leds_max      = 1;
    WHEEL_zone.leds_count    = 1;
    WHEEL_zone.matrix_map    = NULL;
    zones.push_back(WHEEL_zone);
    zones_channel.push_back(SCROLL_WHEEL);

    led WHEEL_led;
    WHEEL_led.name = "Wheel LED";
    WHEEL_led.value = zones.size();
    leds.push_back(WHEEL_led);
    leds_channel.push_back(SCROLL_WHEEL);

    zone STRIP_LEFT_zone;
    STRIP_LEFT_zone.name          = "Strip left";
    STRIP_LEFT_zone.type          = ZONE_TYPE_LINEAR;
    STRIP_LEFT_zone.leds_min      = 4;
    STRIP_LEFT_zone.leds_max      = 4;
    STRIP_LEFT_zone.leds_count    = 4;
    STRIP_LEFT_zone.matrix_map    = NULL;
    zones.push_back(STRIP_LEFT_zone);
    zones_channel.push_back(STRIP_LEFT);

    for(std::size_t led_idx = 0; led_idx < STRIP_LEFT_zone.leds_max; led_idx++)
    {
        led STRIP_LEFT_led;
        STRIP_LEFT_led.name = "Strip left LED "  + std::to_string(led_idx + 1);
        STRIP_LEFT_led.value = zones.size();
        leds.push_back(STRIP_LEFT_led);
        leds_channel.push_back(STRIP_LEFT);
    }

    zone STRIP_RIGHT_zone;
    STRIP_RIGHT_zone.name          = "Strip right";
    STRIP_RIGHT_zone.type          = ZONE_TYPE_LINEAR;
    STRIP_RIGHT_zone.leds_min      = 4;
    STRIP_RIGHT_zone.leds_max      = 4;
    STRIP_RIGHT_zone.leds_count    = 4;
    STRIP_RIGHT_zone.matrix_map    = NULL;
    zones.push_back(STRIP_RIGHT_zone);
    zones_channel.push_back(STRIP_RIGHT);

    for(std::size_t led_idx = 0; led_idx < STRIP_RIGHT_zone.leds_max; led_idx++)
    {
        led STRIP_RIGHT_led;
        STRIP_RIGHT_led.name = "Strip right LED " + std::to_string(led_idx + 1);
        STRIP_RIGHT_led.value = zones.size();
        leds.push_back(STRIP_RIGHT_led);
        leds_channel.push_back(STRIP_RIGHT);
    }

    zone LOWER_LEFT_zone;
    LOWER_LEFT_zone.name          = "Lower left";
    LOWER_LEFT_zone.type          = ZONE_TYPE_SINGLE;
    LOWER_LEFT_zone.leds_min      = 1;
    LOWER_LEFT_zone.leds_max      = 1;
    LOWER_LEFT_zone.leds_count    = 1;
    LOWER_LEFT_zone.matrix_map    = NULL;
    zones.push_back(LOWER_LEFT_zone);
    zones_channel.push_back(LOWER_LEFT);

    led LOWER_LEFT_led;
    LOWER_LEFT_led.name = "Lower left LED";
    LOWER_LEFT_led.value = zones.size();
    leds.push_back(LOWER_LEFT_led);
    leds_channel.push_back(LOWER_LEFT);

    zone LOWER_RIGHT_zone;
    LOWER_RIGHT_zone.name          = "Lower right";
    LOWER_RIGHT_zone.type          = ZONE_TYPE_SINGLE;
    LOWER_RIGHT_zone.leds_min      = 1;
    LOWER_RIGHT_zone.leds_max      = 1;
    LOWER_RIGHT_zone.leds_count    = 1;
    LOWER_RIGHT_zone.matrix_map    = NULL;
    zones.push_back(LOWER_RIGHT_zone);
    zones_channel.push_back(LOWER_RIGHT);

    led LOWER_RIGHT_led;
    LOWER_RIGHT_led.name = "Lower right LED";
    LOWER_RIGHT_led.value = zones.size();
    leds.push_back(LOWER_RIGHT_led);
    leds_channel.push_back(LOWER_RIGHT);

    SetupColors();

    /*---------------------------------------------------------*\
    | Initialize colors for each LED                            |
    \*---------------------------------------------------------*/
    for(std::size_t led_idx = 0; led_idx < leds.size(); led_idx++)
    {
        unsigned char red = 0x00;
        unsigned char grn = 0x00;
        unsigned char blu = 0x00;

        colors[led_idx] = ToRGBColor(red, grn, blu);
    }
}

void RGBController_RoccatKoneAimo::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatKoneAimo::DeviceUpdateLEDs()
{
    /*---------------------------------------------------------*\
    | Set colors for all channel/leds                           |
    \*---------------------------------------------------------*/
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        mouse->SetChannelColors(zones_channel[zone_idx], zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
    /*---------------------------------------------------------*\
    | Apply new colors to the mouse                             |
    \*---------------------------------------------------------*/
    mouse->SendUpdate();
}

void RGBController_RoccatKoneAimo::UpdateZoneLEDs(int zone_idx)
{
    /*---------------------------------------------------------*\
    | Set colors for one channel of leds                        |
    \*---------------------------------------------------------*/
    mouse->SetChannelColors(zones_channel[zone_idx], zones[zone_idx].colors, zones[zone_idx].leds_count);
    /*---------------------------------------------------------*\
    | Apply new colors to the mouse                             |
    \*---------------------------------------------------------*/
    mouse->SendUpdate();
}

void RGBController_RoccatKoneAimo::UpdateSingleLED(int led_idx)
{
    /*---------------------------------------------------------*\
    | Get channel corresponding to led                          |
    \*---------------------------------------------------------*/
    ROCCAT_KONE_AIMO_CHANNEL channel = leds_channel[led_idx];
    /*---------------------------------------------------------*\
    | Update channel corresponding to led                       |
    \*---------------------------------------------------------*/
    mouse->SetChannelColors(channel, zones[leds[led_idx].value].colors, zones[leds[led_idx].value].leds_count);
    /*---------------------------------------------------------*\
    | Apply new colors to the mouse                             |
    \*---------------------------------------------------------*/
    mouse->SendUpdate();
}

void RGBController_RoccatKoneAimo::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_RoccatKoneAimo::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | This device does not support changing mode                |
    \*---------------------------------------------------------*/
}

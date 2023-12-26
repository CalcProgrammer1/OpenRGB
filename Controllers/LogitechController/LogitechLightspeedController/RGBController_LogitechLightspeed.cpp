/*---------------------------------------------------------*\
| RGBController_LogitechLightspeed.cpp                      |
|                                                           |
|   RGBController for Logitech Lightspeed                   |
|                                                           |
|   TheRogueZeta                                05 Aug 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_LogitechLightspeed.h"

/**------------------------------------------------------------------*\
    @name Logitech Lightspeed
    @category Keyboard,Mouse,Mousemat,Headset
    @type USB
    @save :white_check_mark:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectLogitechWireless,DetectLogitechWired
    @comment The Lightspeed controller is the generic RGB Controller
        for all Logitech HID++ devices that support feature page 8070.
\*-------------------------------------------------------------------*/

RGBController_LogitechLightspeed::RGBController_LogitechLightspeed(LogitechLightspeedController* controller_ptr)
{
    controller                      = controller_ptr;
    bool connected                  = controller->lightspeed->connected();

    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_DEVICE_LED_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    if(connected)
    {
        name                        = controller->lightspeed->device_name;
        vendor                      = "Logitech";
        description                 = "Logitech Wireless Lightspeed Device";
        location                    = controller->GetDeviceLocation();
        serial                      = controller->GetSerialString();

        switch(controller->lightspeed->logitech_device_type)
        {
            case LOGITECH_DEVICE_TYPE_KEYBOARD:
                type                = DEVICE_TYPE_KEYBOARD;
                break;

            case LOGITECH_DEVICE_TYPE_MOUSE:
                type                = DEVICE_TYPE_MOUSE;
                break;

            case LOGITECH_DEVICE_TYPE_MOUSEPAD:
                type                = DEVICE_TYPE_MOUSEMAT;
                break;

            case LOGITECH_DEVICE_TYPE_HEADSET:
                type                = DEVICE_TYPE_HEADSET;
                break;

            default:
                type                = DEVICE_TYPE_UNKNOWN;
                LOG_INFO("Logitech device type not known: %i", controller->lightspeed->logitech_device_type);
        }

        logitech_led fx             = controller->lightspeed->getLED_info(0);

        for(uint8_t i = 0; i < fx.fx.size(); i++)
        {
            /*---------------------------------------------------------*\
            | Logitech devices don't have a set order for effects and   |
            |   each device needs to have the effect index mapped       |
            \*---------------------------------------------------------*/
            switch(fx.fx[i].mode)
            {
                case LOGITECH_DEVICE_LED_OFF:
                    /* Do nothing as it's already added */
                    break;

                case LOGITECH_DEVICE_LED_ON:
                {
                    mode Direct;
                    Direct.name                 = "Direct";
                    Direct.value                = i;
                    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
                    Direct.color_mode           = MODE_COLORS_PER_LED;
                    modes.push_back(Direct);

                    mode Static;
                    Static.name                 = "Static";
                    Static.value                = i;
                    Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_BRIGHTNESS;
                    Static.color_mode           = MODE_COLORS_PER_LED;
                    Static.brightness_min       = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MIN;
                    Static.brightness_max       = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MAX;
                    Static.brightness           = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MAX;
                    modes.push_back(Static);
                    LOG_DEBUG("[%s] Adding %s & %s modes at index - %02i", name.c_str(), Direct.name.c_str(), Static.name.c_str(), i);
                    break;
                }

                case LOGITECH_DEVICE_LED_SPECTRUM:
                {
                    mode Cycle;
                    Cycle.name                  = "Spectrum Cycle";
                    Cycle.value                 = i;
                    Cycle.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
                    Cycle.color_mode            = MODE_COLORS_NONE;
                    Cycle.brightness_min        = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MIN;
                    Cycle.brightness_max        = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MAX;
                    Cycle.brightness            = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MAX;
                    Cycle.speed_min             = LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST;
                    Cycle.speed_max             = LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST;
                    Cycle.speed                 = LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL;
                    modes.push_back(Cycle);
                    LOG_DEBUG("[%s] Adding %s mode at index - %02i", name.c_str(), Cycle.name.c_str(), i);
                    break;
                }

                case LOGITECH_DEVICE_LED_BREATHING:
                {
                    mode Breathing;
                    Breathing.name              = "Breathing";
                    Breathing.value             = i;
                    Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
                    Breathing.color_mode        = MODE_COLORS_PER_LED;
                    Breathing.brightness_min    = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MIN;
                    Breathing.brightness_max    = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MAX;
                    Breathing.brightness        = LOGITECH_G_PRO_WIRELESS_BRIGHTNESS_MAX;
                    Breathing.speed_min         = LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST;
                    Breathing.speed_max         = LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST;
                    Breathing.speed             = LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL;
                    modes.push_back(Breathing);
                    LOG_DEBUG("[%s] Adding %s mode at index - %02i", name.c_str(), Breathing.name.c_str(), i);
                    break;
                }

                default:
                    LOG_WARNING("[%s] Effect at index - %02i not added: Value %04X unrecognised", name.c_str(), i, fx.fx[i].mode);
                    break;
            }
        }

        SetupZones();
    }
    else
    {
        name                        = "Idle Lightspeed Device";
        vendor                      = "Logitech";
        type                        = DEVICE_TYPE_UNKNOWN;
        description                 = "Idle Logitech Wireless Lightspeed Device";
        location                    = controller->GetDeviceLocation();
        serial                      = controller->GetSerialString();
    }
}

RGBController_LogitechLightspeed::~RGBController_LogitechLightspeed()
{
    delete controller;
}

void RGBController_LogitechLightspeed::SetupZones()
{
    const std::string zone_string   = "Zone";
    const std::string led_string    = "LED";
    uint8_t led_count               = controller->lightspeed->getLED_count();

    LOG_DEBUG("[%s] Setting up %d LEDs", name.c_str(), led_count);
    if(led_count > 0)
    {
        for(size_t i = 0; i < led_count; i++)
        {
            zone Lightspeed_logo_zone;
            led Lightspeed_logo_led;
            logitech_led new_led                    = controller->lightspeed->getLED_info((uint8_t)i);

            if(new_led.location < NUM_LOGITECH_LED_LOCATIONS )
            {
                Lightspeed_logo_zone.name           = logitech_led_locations[new_led.location];
                Lightspeed_logo_zone.name.append(" ");
                Lightspeed_logo_led.name            = Lightspeed_logo_zone.name;
                Lightspeed_logo_led.name.append(led_string);
                Lightspeed_logo_zone.name.append(zone_string);
            }
            else
            {
                std::string name                    = " " + std::to_string(i);
                Lightspeed_logo_zone.name           = zone_string + name;
                Lightspeed_logo_led.name            = led_string + name;
            }

            Lightspeed_logo_zone.type               = ZONE_TYPE_SINGLE;
            Lightspeed_logo_zone.leds_min           = 1;
            Lightspeed_logo_zone.leds_max           = 1;
            Lightspeed_logo_zone.leds_count         = 1;
            Lightspeed_logo_zone.matrix_map         = NULL;
            zones.push_back(Lightspeed_logo_zone);

            Lightspeed_logo_led.value               = (unsigned int)i;
            leds.push_back(Lightspeed_logo_led);
        }
    }

    SetupColors();
}

void RGBController_LogitechLightspeed::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechLightspeed::DeviceUpdateLEDs()
{
    for(std::vector<led>::iterator led_index = leds.begin(); led_index != leds.end(); led_index++)
    {
        UpdateZoneLEDs(led_index->value);
    }
}

void RGBController_LogitechLightspeed::UpdateZoneLEDs(int zone)
{
    unsigned char red       = RGBGetRValue(colors[zone]);
    unsigned char grn       = RGBGetGValue(colors[zone]);
    unsigned char blu       = RGBGetBValue(colors[zone]);

    controller->SendMouseMode(modes[active_mode].value, modes[active_mode].speed, zone, red, grn, blu, modes[active_mode].brightness);
}

void RGBController_LogitechLightspeed::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LogitechLightspeed::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | If direct mode is true, then sent the packet to put the   |
    | mouse in direct mode.  This code will only be called when |
    | we change modes as to not spam the device.                |
    \*---------------------------------------------------------*/
    controller->lightspeed->setDirectMode(modes[active_mode].name == "Direct");
    DeviceUpdateLEDs();
}

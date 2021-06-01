/*-----------------------------------------*\
|  RGBController_LogitechLightspeed.cpp     |
|                                           |
|  Generic RGB Interface for                |
|  Logitech Lightspeed Gaming Mice          |
|                                           |
|  TheRogueZeta   8/5/2020                  |
\*-----------------------------------------*/

#include "RGBController_LogitechLightspeed.h"

RGBController_LogitechLightspeed::RGBController_LogitechLightspeed(LogitechLightspeedController* logitech_ptr)
{
    logitech                        = logitech_ptr;
    bool connected                  = logitech->lightspeed->connected();

    mode Off;
    Off.name                        = "Off";
    Off.value                       = LOGITECH_G_PRO_WIRELESS_MODE_OFF;
    Off.flags                       = 0;
    Off.color_mode                  = MODE_COLORS_NONE;
    modes.push_back(Off);

    if(connected)
    {
        name                        = logitech->lightspeed->device_name;
        vendor                      = "Logitech";
        description                 = "Logitech Wireless Lightspeed Device";
        location                    = logitech->GetDeviceLocation();
        serial                      = logitech->GetSerialString();

        switch(logitech->lightspeed->logitech_device_type)
        {
            case LOGITECH_DEVICE_TYPE_KEYBOARD:
                type                = DEVICE_TYPE_KEYBOARD;
                break;

            case LOGITECH_DEVICE_TYPE_MOUSE:
                type                = DEVICE_TYPE_MOUSE;
                break;

            default:
                type                = DEVICE_TYPE_UNKNOWN;
                LOG_NOTICE("Logitech device type not known: %i", logitech->lightspeed->logitech_device_type);
        }

        mode Direct;
        Direct.name                 = "Direct";
        Direct.value                = 0xFF;
        Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
        Direct.color_mode           = MODE_COLORS_PER_LED;
        modes.push_back(Direct);

        mode Static;
        Static.name                 = "Static";
        Static.value                = LOGITECH_G_PRO_WIRELESS_MODE_STATIC;
        Static.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
        Static.color_mode           = MODE_COLORS_PER_LED;
        modes.push_back(Static);

        mode Cycle;
        Cycle.name                  = "Spectrum Cycle";
        Cycle.value                 = LOGITECH_G_PRO_WIRELESS_MODE_CYCLE;
        Cycle.flags                 = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        Cycle.color_mode            = MODE_COLORS_NONE;
        Cycle.speed_min             = LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST;
        Cycle.speed_max             = LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST;
        Cycle.speed                 = LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL;
        modes.push_back(Cycle);

        mode Breathing;
        Breathing.name              = "Breathing";
        Breathing.value             = LOGITECH_G_PRO_WIRELESS_MODE_BREATHING;
        Breathing.flags             = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
        Breathing.color_mode        = MODE_COLORS_PER_LED;
        Breathing.speed_min         = LOGITECH_G_PRO_WIRELESS_SPEED_SLOWEST;
        Breathing.speed_max         = LOGITECH_G_PRO_WIRELESS_SPEED_FASTEST;
        Breathing.speed             = LOGITECH_G_PRO_WIRELESS_SPEED_NORMAL;
        modes.push_back(Breathing);

        SetupZones();
    }
    else
    {
        name                        = "Idle Lightspeed Device";
        vendor                      = "Logitech";
        type                        = DEVICE_TYPE_UNKNOWN;
        description                 = "Idle Logitech Wireless Lightspeed Device";
        location                    = logitech->GetDeviceLocation();
        serial                      = logitech->GetSerialString();
    }
}

RGBController_LogitechLightspeed::~RGBController_LogitechLightspeed()
{
    delete logitech;
}

void RGBController_LogitechLightspeed::SetupZones()
{
    uint8_t LED_count = logitech->lightspeed->getLEDinfo();

    if(LED_count > 0)
    {
        for(size_t i = 0; i < LED_count; i++)
        {
            zone Lightspeed_logo_zone;
            Lightspeed_logo_zone.name               = "Zone " + std::to_string(i);
            Lightspeed_logo_zone.type               = ZONE_TYPE_SINGLE;
            Lightspeed_logo_zone.leds_min           = 1;
            Lightspeed_logo_zone.leds_max           = 1;
            Lightspeed_logo_zone.leds_count         = 1;
            Lightspeed_logo_zone.matrix_map         = NULL;
            zones.push_back(Lightspeed_logo_zone);

            led Lightspeed_logo_led;
            Lightspeed_logo_led.name                = "LED " + std::to_string(i);
            Lightspeed_logo_led.value               = i;
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
    unsigned char red = RGBGetRValue(colors[zone]);
    unsigned char grn = RGBGetGValue(colors[zone]);
    unsigned char blu = RGBGetBValue(colors[zone]);

    /*---------------------------------------------------------*\
    | Replace direct mode with static when sending to controller|
    \*---------------------------------------------------------*/
    unsigned char temp_mode = (modes[active_mode].value == 0xFF) ? LOGITECH_G_PRO_WIRELESS_MODE_STATIC : modes[active_mode].value;

    logitech->SendMouseMode(temp_mode, modes[active_mode].speed, zone, red, grn, blu);
}

void RGBController_LogitechLightspeed::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LogitechLightspeed::SetCustomMode()
{
    if(logitech->lightspeed->connected())
    {
        active_mode = 1;
    }
}

void RGBController_LogitechLightspeed::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | If direct mode is true, then sent the packet to put the   |
    | mouse in direct mode.  This code will only be called when |
    | we change modes as to not spam the device.                |
    \*---------------------------------------------------------*/
    logitech->lightspeed->setDirectMode(modes[active_mode].value == 0xFF);
    DeviceUpdateLEDs();
}

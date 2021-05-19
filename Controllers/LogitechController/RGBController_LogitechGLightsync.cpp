/*-----------------------------------------*\
|  RGBController_LogitechGLightsync.cpp     |
|                                           |
|  Generic RGB Interface for                |
|  Logitech G Lightsync Devices             |
|                                           |
|  TheRogueZeta   04/21/2021                |
\*-----------------------------------------*/

#include "RGBController_LogitechGLightsync.h"

RGBController_LogitechGLightsync::RGBController_LogitechGLightsync(LogitechGLightsyncController* logitech_ptr)
{
    logitech                = logitech_ptr;

    name                    = "Logitech G Lightsync Mouse";
    vendor                  = "Logitech";
    type                    = DEVICE_TYPE_MOUSE;
    description             = "Logitech G Lightsync Mouse";
    location                = logitech->GetDeviceLocation();
    serial                  = logitech->GetSerialString();

    mode Off;
    Off.name                = "Off";
    Off.value               = LOGITECH_G_LIGHTSYNC_MODE_OFF;
    Off.flags               = 0;
    Off.color_mode          = MODE_COLORS_NONE;
    modes.push_back(Off);

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0xFF;
    Direct.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name             = "Static";
    Static.value            = LOGITECH_G_LIGHTSYNC_MODE_STATIC;
    Static.flags            = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    mode Cycle;
    Cycle.name              = "Spectrum Cycle";
    Cycle.value             = LOGITECH_G_LIGHTSYNC_MODE_CYCLE;
    Cycle.flags             = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Cycle.color_mode        = MODE_COLORS_NONE;
    Cycle.speed_min         = LOGITECH_G_LIGHTSYNC_SPEED_SLOWEST;
    Cycle.speed_max         = LOGITECH_G_LIGHTSYNC_SPEED_FASTEST;
    Cycle.speed             = LOGITECH_G_LIGHTSYNC_SPEED_NORMAL;
    modes.push_back(Cycle);

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = LOGITECH_G_LIGHTSYNC_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed_min     = LOGITECH_G_LIGHTSYNC_SPEED_SLOWEST;
    Breathing.speed_max     = LOGITECH_G_LIGHTSYNC_SPEED_FASTEST;
    Breathing.speed         = LOGITECH_G_LIGHTSYNC_SPEED_NORMAL;
    modes.push_back(Breathing);

    SetupZones();
}

RGBController_LogitechGLightsync::~RGBController_LogitechGLightsync()
{
    delete logitech;
}

void RGBController_LogitechGLightsync::SetupZones()
{
    zone GLightsync_primary_zone;
    GLightsync_primary_zone.name            = "DPI";
    GLightsync_primary_zone.type            = ZONE_TYPE_SINGLE;
    GLightsync_primary_zone.leds_min        = 1;
    GLightsync_primary_zone.leds_max        = 1;
    GLightsync_primary_zone.leds_count      = 1;
    GLightsync_primary_zone.matrix_map      = NULL;
    zones.push_back(GLightsync_primary_zone);

    led GLightsync_primary_led;
    GLightsync_primary_led.name = "DPI";
    leds.push_back(GLightsync_primary_led);

    zone GLightsync_logo_zone;
    GLightsync_logo_zone.name               = "Logo";
    GLightsync_logo_zone.type               = ZONE_TYPE_SINGLE;
    GLightsync_logo_zone.leds_min           = 1;
    GLightsync_logo_zone.leds_max           = 1;
    GLightsync_logo_zone.leds_count         = 1;
    GLightsync_logo_zone.matrix_map         = NULL;
    zones.push_back(GLightsync_logo_zone);

    led GLightsync_logo_led;
    GLightsync_logo_led.name = "Logo";
    leds.push_back(GLightsync_logo_led);

    SetupColors();
}

void RGBController_LogitechGLightsync::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_LogitechGLightsync::DeviceUpdateLEDs()
{
    UpdateZoneLEDs(0);
    UpdateZoneLEDs(1);
}

void RGBController_LogitechGLightsync::UpdateZoneLEDs(int zone)
{
    unsigned char red = RGBGetRValue(colors[zone]);
    unsigned char grn = RGBGetGValue(colors[zone]);
    unsigned char blu = RGBGetBValue(colors[zone]);

    /*---------------------------------------------------------*\
    | Replace direct mode with static when sending to controller|
    \*---------------------------------------------------------*/
    unsigned char temp_mode = (modes[active_mode].value != 0xFF) ? modes[active_mode].value : LOGITECH_G_LIGHTSYNC_MODE_STATIC;

    logitech->UpdateMouseLED(temp_mode, modes[active_mode].speed, zone, red, grn, blu, /* Brightness */ 0x64);
}

void RGBController_LogitechGLightsync::UpdateSingleLED(int led)
{
    UpdateZoneLEDs(led);
}

void RGBController_LogitechGLightsync::SetCustomMode()
{
    active_mode = 1;
}

void RGBController_LogitechGLightsync::DeviceUpdateMode()
{
    /*---------------------------------------------------------*\
    | If direct mode is true, then sent the packet to put the   |
    | mouse in direct mode.  This code will only be called when |
    | we change modes as to not spam the device.                |
    \*---------------------------------------------------------*/
    logitech->SetDirectMode(modes[active_mode].value == 0xFF);
    DeviceUpdateLEDs();
}
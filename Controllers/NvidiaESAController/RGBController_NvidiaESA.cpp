/*-----------------------------------------*\
|  RGBController_NvidiaESA.cpp              |
|                                           |
|  Generic RGB Interface for OpenRGB        |
|  Nvidia ESA USB Driver                    |
|                                           |
|  Guimard Morgan (morg) 2/18/2022          |
\*-----------------------------------------*/

#include "RGBController_NvidiaESA.h"

#include <thread>
#include <chrono>

/**------------------------------------------------------------------*\
    @name NVidia ESA
    @category Case
    @type USB
    @save :x:
    @direct :x:
    @effects :white_check_mark:
    @detectors DetectNvidiaESAControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_NvidiaESA::RGBController_NvidiaESA(NvidiaESAController* controller_ptr)
{
    controller                          = controller_ptr;
    name                                = "Nvidia ESA USB Device";
    vendor                              = "NVIDIA";
    type                                = DEVICE_TYPE_CASE;
    description                         = name;
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();
    version                             = controller->GetFirmwareVersion();

    mode Static;
    Static.name                         = "Static";
    Static.value                        = 0x00;
    Static.flags                        = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_AUTOMATIC_SAVE;
    Static.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    SetupZones();
}

RGBController_NvidiaESA::~RGBController_NvidiaESA()
{
    delete controller;
}

void RGBController_NvidiaESA::SetupZones()
{
    std::vector<std::string> zone_names =
    {
        "Front Drive Bays",     // 0x42
        "Front USB",            // 0x43
        "Rear",                 // 0x44
        "Internal",             // 0x45
        "Front Audio"           // 0x46
    };

    for(const std::string& zone_name: zone_names)
    {
        zone new_zone;

        new_zone.name       = zone_name;
        new_zone.type       = ZONE_TYPE_LINEAR;
        new_zone.leds_min   = 1;
        new_zone.leds_max   = 1;
        new_zone.leds_count = 1;
        new_zone.matrix_map = nullptr;

        zones.emplace_back(new_zone);

        led new_led;
        new_led.name = "LED";
        leds.push_back(new_led);
    }

    SetupColors();
}

void RGBController_NvidiaESA::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_NvidiaESA::DeviceUpdateLEDs()
{
    for(unsigned int zone = 0; zone < zones.size(); zone++)
    {
        UpdateZoneLEDs(zone);
    }
}

void RGBController_NvidiaESA::UpdateZoneLEDs(int zone)
{
    controller->SetZoneColor(zone, zones[zone].colors[0]);
}

void RGBController_NvidiaESA::UpdateSingleLED(int /*led*/)
{
    UpdateZoneLEDs(0);
}

void RGBController_NvidiaESA::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

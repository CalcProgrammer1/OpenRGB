/*---------------------------------------------------------*\
|  RGBController_AcerMonitor.cpp                            |
|                                                           |
|  RGBController interface implementation for 2020/2021     |
|  Acer monitors with rear RGB strips.                      |
|                                                           |
|  Shenghao Yang (me@shenghaoyang.info), 30/12/2021         |
|  Amended for code style by Adam Honse, 3/13/2022          |
\*---------------------------------------------------------*/

#include "RGBController_AcerMonitor.h"

RGBController_AcerMonitor::RGBController_AcerMonitor(AcerMonitorController* controller_ptr)
{
    controller  = controller_ptr;
    vendor      = "Acer";
    type        = DEVICE_TYPE_LEDSTRIP;
    description = "Acer Monitor Device";
    location    = controller->GetDeviceLocation();
    serial      = controller->GetSerialString();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_AcerMonitor::~RGBController_AcerMonitor()
{
    delete controller;
}

void RGBController_AcerMonitor::SetupZones()
{
    zones.clear();
    leds.clear();

    // for(const auto *const *pz{ctrl->monitor->zones}; *pz; ++pz)
    // {
    //     const Zone *z{*pz};
    //     zone led_zone;

    //     led_zone.leds_count = z->leds;
    //     led_zone.leds_max = led_zone.leds_count;
    //     led_zone.leds_min = led_zone.leds_count;
    //     led_zone.name = z->name;
    //     led_zone.type = z->type;
    //     led_zone.matrix_map = z->mmap;

    //     for (std::size_t i{}; i < z->leds; ++i)
    //     {
    //         led new_led{};

    //         new_led.name = led_zone.name + " LED ";
    //         if (led_zone.leds_count > 1)
    //             new_led.name.append(std::to_string(i + 1));

    //         leds.emplace_back(new_led);
    //     }

    //     zones.emplace_back(led_zone);
    // }

    SetupColors();
}

void RGBController_AcerMonitor::ResizeZone(int, int)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_AcerMonitor::DeviceUpdateLEDs()
{
    // for (std::size_t i{}; i < zones.size(); ++i)
    //     UpdateZoneLEDs(i);
}

void RGBController_AcerMonitor::UpdateZoneLEDs(int zone)
{
    //controller->SetLEDs(zone, zones[zone].colors);
}

void RGBController_AcerMonitor::UpdateSingleLED(int led)
{
    // /*-----------------------------------------------------*\
    // | Find the zone that the LED exists in.                 |
    // \*-----------------------------------------------------*/
    // std::size_t total{};
    // int zone{};
    // while (static_cast<std::size_t>(led) >= total)
    //     total += zones[zone++].leds_count;

    // --zone;
    // ctrl->SetLEDs(zone, zones[zone].colors);
}

void RGBController_AcerMonitor::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_AcerMonitor::DeviceUpdateMode()
{

}

#include "RGBController_RoccatElo.h"

#include <thread>
#include <chrono>

/**------------------------------------------------------------------*\
    @name Roccat Elo 7.1
    @category Headset
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :x:
    @detectors DetectRoccatEloControllers
    @comment
\*-------------------------------------------------------------------*/

RGBController_RoccatElo::RGBController_RoccatElo(RoccatEloController* controller_ptr)
{
    controller                          = controller_ptr;
    name                                = "Roccat Elo 7.1";
    vendor                              = "Roccat";
    type                                = DEVICE_TYPE_HEADSET;
    description                         = name;
    location                            = controller->GetDeviceLocation();
    serial                              = controller->GetSerialString();
    version                             = "";

    mode Direct;
    Direct.name                         = "Direct";
    Direct.flags                        = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode                   = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    SetupZones();
}

RGBController_RoccatElo::~RGBController_RoccatElo()
{
    delete controller;
}

void RGBController_RoccatElo::SetupZones()
{
    zone new_zone;

    new_zone.name       = "Headset";
    new_zone.type       = ZONE_TYPE_LINEAR;
    new_zone.leds_min   = ROCCAT_ELO_LEDS_COUNT;
    new_zone.leds_max   = ROCCAT_ELO_LEDS_COUNT;
    new_zone.leds_count = ROCCAT_ELO_LEDS_COUNT;
    new_zone.matrix_map = nullptr;

    zones.emplace_back(new_zone);

    leds.resize(new_zone.leds_count);

    SetupColors();
}

void RGBController_RoccatElo::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_RoccatElo::DeviceUpdateLEDs()
{
    controller->SendDirect(colors[0]);
}

void RGBController_RoccatElo::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatElo::UpdateSingleLED(int /*led*/)
{
    DeviceUpdateLEDs();
}

void RGBController_RoccatElo::DeviceUpdateMode()
{

}

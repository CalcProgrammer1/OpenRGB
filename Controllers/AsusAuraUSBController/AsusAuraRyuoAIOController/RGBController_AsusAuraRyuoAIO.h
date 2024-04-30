/*-------------------------------------------------------------------*\
|  RGBController_AsusAuraRyuoAIO.h                                    |
|                                                                     |
|  Driver for the ASUS Aura Ryuo AIO                                  |
|    lighting controller                                              |
|                                                                     |
|  Cooper Hall (geobot19 / Geo_bot)          08 Apr 2022              |
|  using snipets from Chris M (Dr.No)                                 |
\*-------------------------------------------------------------------*/

#pragma once
#include "LogManager.h"
#include "RGBController.h"
#include "AsusAuraRyuoAIOController.h"
#include <vector>

class RGBController_AsusAuraRyuoAIO : public RGBController
{
public:
    RGBController_AsusAuraRyuoAIO(AsusAuraRyuoAIOController* controller_ptr);
    ~RGBController_AsusAuraRyuoAIO();

    void SetupZones();
    void ResizeZone(int zone, int new_size);

    void DeviceUpdateLEDs();
    void UpdateZoneLEDs(int zone);
    void UpdateSingleLED(int led);

    void DeviceUpdateMode();

private:
    int  GetDeviceMode();
    int  GetLED_Zone(int led_idx);

    AsusAuraRyuoAIOController* controller;
};


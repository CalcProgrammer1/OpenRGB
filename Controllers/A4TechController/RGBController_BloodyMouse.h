/*---------------------------------------------------------------------*\
|  RGBController_BloodyMouse.h                                          |
|                                                                       |
|  Driver for BloodyMouse USB Controller                                |
|                                                                       |
|  Chris M (Dr_No)          30 Jun 2022                                 |
|                                                                       |
\*---------------------------------------------------------------------*/

#pragma once
#include <vector>
#include "LogManager.h"
#include "RGBController.h"
#include "BloodyMouseController.h"

struct mouse_zone
{
    std::string             name;
    std::vector<uint8_t>    zone_leds;
};

typedef std::vector<mouse_zone> mouse_layout;

class RGBController_BloodyMouse : public RGBController
{
public:
    RGBController_BloodyMouse(BloodyMouseController* controller_ptr);
    ~RGBController_BloodyMouse();

    void                    SetupZones();
    void                    ResizeZone(int zone, int new_size);

    void                    DeviceUpdateLEDs();
    void                    UpdateZoneLEDs(int zone);
    void                    UpdateSingleLED(int led);

    void                    DeviceUpdateMode();

private:
    BloodyMouseController*  controller;
};

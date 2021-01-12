/*---------------------------------------------------------*\
|  Definitions for FanBus RGB Controller Interface          |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/12/2021        |
\*---------------------------------------------------------*/

#pragma once

#include "FanBusInterface.h"
#include "RGBController.h"

#include <vector>

class FanBusController
{
public:
    FanBusController(FanBusInterface* bus_ptr, unsigned char dev_addr);
    ~FanBusController();

    std::string GetLocation();

    void        SetLEDs(std::vector<RGBColor> colors);

private:
    std::string         port_name;
    FanBusInterface*    bus;
    unsigned char       dev;
};

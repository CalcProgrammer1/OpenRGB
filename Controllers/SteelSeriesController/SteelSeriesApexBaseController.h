/*-----------------------------------------*\
|  SteelSeriesApexBaseController.h          |
|                                           |
|  Base controller for SteelSeries Apex     |
|  Keyboard lighting controllers            |
|                                           |
|  Florian Heilmann (FHeilmann) 19/10/2020  |
\*-----------------------------------------*/

#include "RGBController.h"
#include "SteelSeriesGeneric.h"

#include <string>

#pragma once

class SteelSeriesApexBaseController
{
public:

    std::string GetDeviceLocation()
    {
        return(location);
    };

    steelseries_type        proto_type;

    virtual void SetMode
        (
        unsigned char mode,
        std::vector<RGBColor> colors
        ) = 0;

    virtual void SetLEDsDirect(std::vector<RGBColor> colors) = 0;

protected:
    std::string             location;
    hid_device*             dev;
    unsigned char           active_mode;
};

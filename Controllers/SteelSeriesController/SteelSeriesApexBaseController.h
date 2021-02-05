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

    virtual ~SteelSeriesApexBaseController()
    {

    }

    std::string GetDeviceLocation()
    {
        return("HID: " + location);
    };

    std::string GetSerialString()
    {
        wchar_t serial_string[128];
        hid_get_serial_number_string(dev, serial_string, 128);

        std::wstring return_wstring = serial_string;
        std::string return_string(return_wstring.begin(), return_wstring.end());

        return(return_string);
    }

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

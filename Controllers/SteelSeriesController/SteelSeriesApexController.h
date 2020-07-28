/*-----------------------------------------*\
|  SteelSeriesApexController.h              |
|                                           |
|  Definitions and types for SteelSeries    |
|  Apex 7 Keyboard lighting controller      |
|                                           |
|  Eric Samuelson (edbgon) 7/5/2020         |
\*-----------------------------------------*/

#include "RGBController.h"
#include "SteelSeriesGeneric.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    APEX7_PACKET_ID_DIRECT             = 0x3a,     /* Direct mode                */
};

class SteelSeriesApexController
{
public:
    SteelSeriesApexController(hid_device* dev_handle, steelseries_type type);
    ~SteelSeriesApexController();

    steelseries_type        proto_type;

    void SetMode
        (
        unsigned char mode,
        std::vector<RGBColor> colors
        );

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:
    hid_device*             dev;
    unsigned char           active_mode;

    void    SelectProfile
                (
                unsigned char   profile
                );
};
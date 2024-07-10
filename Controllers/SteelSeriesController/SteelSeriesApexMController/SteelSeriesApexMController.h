/*-----------------------------------------*\
|  SteelSeriesApexMController.h             |
|                                           |
|  Definitions and types for SteelSeries    |
|  Apex M750 Keyboard lighting controller   |
|                                           |
|  Florian Heilmann (FHeilmann) 12/10/2020  |
\*-----------------------------------------*/

#include "RGBController.h"
#include "SteelSeriesGeneric.h"
#include "SteelSeriesApexBaseController.h"

#include <string>
#include <hidapi/hidapi.h>

#pragma once

class SteelSeriesApexMController : public SteelSeriesApexBaseController
{
public:
    SteelSeriesApexMController(hid_device* dev_handle, steelseries_type type, const char* path);
    ~SteelSeriesApexMController();

    void SetMode
    (
        unsigned char mode,
        std::vector<RGBColor> colors
    );

    void SetLEDsDirect(std::vector<RGBColor> colors);

private:

    void EnableLEDControl();
    void    SelectProfile
    (
        unsigned char   profile
    );
};

/*---------------------------------------------------------*\
|  Definitions for Philips Hue Entertainment Mode           |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 11/6/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "Bridge.h"
#include "EntertainmentMode.h"
#include "Group.h"

#include <string>
#include <vector>

#pragma once

#define HUE_ENTERTAINMENT_HEADER_SIZE   16
#define HUE_ENTERTAINMENT_LIGHT_SIZE    9

class PhilipsHueEntertainmentController
{
public:
    PhilipsHueEntertainmentController(hueplusplus::Bridge& bridge_ptr, hueplusplus::Group& group_ptr);
    ~PhilipsHueEntertainmentController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();
    unsigned int GetNumLEDs();

    void SetColor(RGBColor* colors);

private:
    hueplusplus::Bridge&            bridge;
    hueplusplus::Group&             group;
    hueplusplus::EntertainmentMode* entertainment;

    std::string                     location;
    unsigned int                    num_leds;
};

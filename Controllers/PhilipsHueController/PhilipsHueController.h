/*---------------------------------------------------------*\
|  Definitions for Philips Hue                              |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 9/15/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "HueDeviceTypes.h"

#include <string>
#include <vector>

#pragma once

class PhilipsHueController
{
public:
    PhilipsHueController(hueplusplus::Light& light_ptr, std::string bridge_ip);
    ~PhilipsHueController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    hueplusplus::Light& light;
    std::string         location;
    bool                dark;
};

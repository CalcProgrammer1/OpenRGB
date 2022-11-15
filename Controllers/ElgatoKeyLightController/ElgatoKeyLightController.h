/*---------------------------------------------------------*\
|  Definitions for Elgato Key Light                         |
|                                                           |
|  Monks (imtherealestmonkey@gmail.com), 11/11/2021         |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "net_port.h"
#include "hsv.h"

#include <cmath>
#include <string>
#include <thread>
#include <vector>

#pragma once

class ElgatoKeyLightController
{
public:
    ElgatoKeyLightController(std::string ip);
    ~ElgatoKeyLightController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(hsv_t hsv_color);

private:
    std::string GetRequest(int brightness, int temperature);
    int HSVToK(int hue);
    std::string         location;
    net_port            port;
};

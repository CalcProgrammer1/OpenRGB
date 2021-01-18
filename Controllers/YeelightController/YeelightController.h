/*---------------------------------------------------------*\
|  Definitions for Yeelight                                 |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/18/2021        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "net_port.h"

#include <string>
#include <thread>
#include <vector>

#pragma once

class YeelightController
{
public:
    YeelightController(std::string ip);
    ~YeelightController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue);

private:
    std::string         location;
    net_port            port;
};

/*---------------------------------------------------------*\
|  GoveeController.h                                        |
|                                                           |
|  Definitions for Govee controller                         |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/1/2023        |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "net_port.h"

#include <string>
#include <thread>
#include <vector>

class GoveeController
{
public:
    GoveeController(std::string ip);
    ~GoveeController();

    std::string GetLocation();
    std::string GetVersion();

    void SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness);

private:
    std::string         firmware_version;
    std::string         module_name;
    std::string         module_mac;
    std::string         location;
    net_port            port;
};

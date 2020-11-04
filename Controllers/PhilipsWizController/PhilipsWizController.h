/*---------------------------------------------------------*\
|  Definitions for Philips Wiz                              |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 11/3/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "net_port.h"

#include <string>
#include <vector>

#pragma once

class PhilipsWizController
{
public:
    PhilipsWizController(std::string ip);
    ~PhilipsWizController();

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

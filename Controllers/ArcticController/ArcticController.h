/*-----------------------------------------*\
|  ArcticController.h                       |
|                                           |
|  Controller Interface for Arctic devices  |
|                                           |
|  Armin Wolf (Wer-Wolf) 01/09/2023         |
\*-----------------------------------------*/

#pragma once
#include <algorithm>

#include "RGBController.h"
#include "serial_port.h"

class ArcticController
{
public:
    ArcticController(const std::string &portname);
    ~ArcticController();

    void SetChannels(std::vector<RGBColor> colors);
    bool IsPresent();

    std::string GetLocation();

private:
    std::string port_name;
    serial_port serialport;
};

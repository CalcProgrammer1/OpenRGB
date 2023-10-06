/*---------------------------------------------------------*\
|  HYTEMousematController.h                                 |
|                                                           |
|  Definitions for HYTE CNVS RGB mousemat controller        |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 7/18/2023        |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "serial_port.h"
#include <vector>

class HYTEMousematController
{
public:
    HYTEMousematController(char* port);
    ~HYTEMousematController();

    std::string     GetLocation();

    void            FirmwareAnimationControl(bool enabled);
    void            StreamingCommand(RGBColor* colors);

private:
    std::string     port_name;
    serial_port *   serialport = nullptr;
};

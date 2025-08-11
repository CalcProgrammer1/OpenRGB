/*---------------------------------------------------------*\
| HYTEMousematController_Windows_MacOS.cpp                  |
|                                                           |
|   Driver for HYTE mousemat (Serial implementation for     |
|   Windows and MacOS)                                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      18 Jul 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "HYTEMousematController_Windows_MacOS.h"

HYTEMousematController::HYTEMousematController(char* port, std::string dev_name)
{
    name        = dev_name;
    port_name   = port;

    /*-----------------------------------------------------*\
    | Open the port                                         |
    | Baud rate doesn't matter for ACM device               |
    \*-----------------------------------------------------*/
    serialport = new serial_port(port_name.c_str(), 2000000);
}

HYTEMousematController::~HYTEMousematController()
{
    serialport->serial_close();
}

std::string HYTEMousematController::GetLocation()
{
    return(port_name);
}

std::string HYTEMousematController::GetName()
{
    return(name);
}

void HYTEMousematController::FirmwareAnimationControl(bool enabled)
{
    unsigned char serial_buf[4];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(serial_buf, 0, sizeof(serial_buf));

    /*-----------------------------------------------------*\
    | Set up Firmware Animation Control packet              |
    \*-----------------------------------------------------*/
    serial_buf[0]   = 0xFF;
    serial_buf[1]   = 0xDC;
    serial_buf[2]   = 0x05;
    serial_buf[3]   = enabled;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    serialport->serial_write((char *)serial_buf, sizeof(serial_buf));
}

void HYTEMousematController::StreamingCommand(RGBColor* colors)
{
    unsigned char serial_buf[157];
    unsigned int max_brightness = 72;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(serial_buf, 0, sizeof(serial_buf));

    /*-----------------------------------------------------*\
    | Set up Streaming packet                               |
    \*-----------------------------------------------------*/
    serial_buf[0]   = 0xFF;
    serial_buf[1]   = 0xEE;
    serial_buf[2]   = 0x02;
    serial_buf[3]   = 0x01;
    serial_buf[4]   = 0x00;
    serial_buf[5]   = 0x32;
    serial_buf[6]   = 0x00;

    /*-----------------------------------------------------*\
    | Copy in colors                                        |
    \*-----------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < 50; color_idx++)
    {
        serial_buf[7 + (color_idx * 3)] = ( max_brightness * RGBGetGValue(colors[color_idx]) ) / 100;
        serial_buf[8 + (color_idx * 3)] = ( max_brightness * RGBGetRValue(colors[color_idx]) ) / 100;
        serial_buf[9 + (color_idx * 3)] = ( max_brightness * RGBGetBValue(colors[color_idx]) ) / 100;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    serialport->serial_write((char *)serial_buf, sizeof(serial_buf));
}

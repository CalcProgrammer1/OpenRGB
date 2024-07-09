/*---------------------------------------------------------*\
| EspurnaController.h                                       |
|                                                           |
|   Driver for Espurna                                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "net_port.h"

#ifndef TRUE
#define TRUE true
#define FALSE false
#endif

#ifndef WIN32
#define LPSTR           char *
#define strtok_s        strtok_r
#endif

class EspurnaController
{
public:
    EspurnaController();
    ~EspurnaController();

    void        Initialize(char* ledstring);
    void        InitializeEspurna(char* clientname, char* port, char * apikey);

    std::string GetLocation();

    void        SetLEDs(std::vector<RGBColor> colors);

private:
    std::string port_name;
    std::string client_name;
    char espurna_apikey[128];

    net_port *tcpport;
};

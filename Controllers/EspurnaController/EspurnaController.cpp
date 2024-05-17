/*---------------------------------------------------------*\
| EspurnaController.cpp                                     |
|                                                           |
|   Driver for Espurna                                      |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      11 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include "EspurnaController.h"

EspurnaController::EspurnaController()
{

}

EspurnaController::~EspurnaController()
{
}

void EspurnaController::Initialize(char* ledstring)
{
    LPSTR   apikey = NULL;
    LPSTR   source = NULL;
    LPSTR   udpport_baud = NULL;
    LPSTR   next = NULL;

    source = strtok_s(ledstring, ",", &next);

    //Check for either the UDP port or the serial baud rate
    if (strlen(next))
    {
        udpport_baud = strtok_s(next, ",", &next);
    }

    //Espurna protocol requires API key
    if (strlen(next))
    {
        apikey = strtok_s(next, ",", &next);
    }

    InitializeEspurna(source, udpport_baud, apikey);
}

void EspurnaController::InitializeEspurna(char * clientname, char * port, char * apikey)
{
    client_name = clientname;
    port_name   = port;

    strcpy(espurna_apikey, apikey);
    tcpport = new net_port;
    tcpport->tcp_client(client_name.c_str(), port_name.c_str());
}

std::string EspurnaController::GetLocation()
{
    return("TCP: " + client_name + ":" + port_name);
}

void EspurnaController::SetLEDs(std::vector<RGBColor> colors)
{
    if (tcpport != NULL)
    {
        RGBColor color = colors[0];

        char get_request[1024];
        snprintf(get_request, 1024, "GET /api/rgb?apikey=%s&value=%%23%02X%02X%02X HTTP/1.1\r\nHost: %s\r\n\r\n", espurna_apikey, RGBGetRValue(color), RGBGetGValue(color), RGBGetBValue(color), client_name.c_str());
        tcpport->tcp_client_connect();
        tcpport->tcp_client_write(get_request, (int)strlen(get_request));
        tcpport->tcp_close();
    }
}

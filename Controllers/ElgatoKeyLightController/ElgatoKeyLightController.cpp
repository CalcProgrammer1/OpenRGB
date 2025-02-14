/*---------------------------------------------------------*\
| ElgatoKeyLightController.cpp                              |
|                                                           |
|   Driver for Elgato Key Light                             |
|                                                           |
|   Monks (imtherealestmonkey@gmail.com),       03 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iostream>
#include "ElgatoKeyLightController.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ElgatoKeyLightController::ElgatoKeyLightController(std::string ip)
{
    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location    = "IP: " + ip;

    /*-----------------------------------------------------------------*\
    | Open a TCP client sending to the device's IP, port 9123          |
    \*-----------------------------------------------------------------*/
    port.tcp_client(ip.c_str(), "9123");
}

ElgatoKeyLightController::~ElgatoKeyLightController()
{
}

std::string ElgatoKeyLightController::GetLocation()
{
    return(location);
}

std::string ElgatoKeyLightController::GetName()
{
    return("Elgato KeyLight");
}

std::string ElgatoKeyLightController::GetVersion()
{
    return("");
}

std::string ElgatoKeyLightController::GetManufacturer()
{
    return("Elgato");
}

std::string ElgatoKeyLightController::GetUniqueID()
{
    return("");
}

void ElgatoKeyLightController::SetColor(hsv_t hsv_color)
{
    // Weird elgato color format
    int k_value = HSVToK(hsv_color.hue);

    port.tcp_client_connect();
    std::string buf = GetRequest(hsv_color.value, k_value);
    port.tcp_client_write((char *)buf.c_str(), (int)buf.length() + 1);

    port.tcp_close();
}

std::string ElgatoKeyLightController::GetRequest(int brightness, int temperature)
{
    json command;

    command["numberOfLights"] = 1;

    auto lights = json::array();
    lights.push_back(json::object({ {"on", 1}, {"temperature", temperature}, {"brightness", brightness}}));
    command["lights"] = lights;

    std::string command_str = command.dump();
    std::string buf = "PUT /elgato/lights HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: " +
                      std::to_string(command_str.length()) +
                      "\r\nConnection: close\r\n\r\n" + command_str + "\r\n\r\n";
    return(buf);
}

int ElgatoKeyLightController::HSVToK(int hue)
{
    int k_value;

    if(hue <= 60 && hue >= 0)
    {
        k_value = 2900;
    }
    else if(hue >= 61 && hue <= 120)
    {
        k_value = 4000;
    }
    else if(hue >= 121 && hue <= 180)
    {
        k_value = 5000;
    }
    else if(hue >= 181 && hue <= 240)
    {
        k_value = 6000;
    }
    else
    {
        k_value = 7000;
    }

    return k_value;
}

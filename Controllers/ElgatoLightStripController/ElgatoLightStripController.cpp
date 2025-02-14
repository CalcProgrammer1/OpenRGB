/*---------------------------------------------------------*\
| ElgatoLightStripController.cpp                            |
|                                                           |
|   Driver for Elgato Light Strip                           |
|                                                           |
|   Monks (@iamtherealestmonkey)                03 Nov 2021 |
|   DomePlaysHD                                 14 Mar 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <sstream>
#include <thread>
#include <vector>
#include "ElgatoLightStripController.h"
#include <nlohmann/json.hpp>
#include "LogManager.h"

using json = nlohmann::json;
using namespace std::chrono_literals;

ElgatoLightStripController::ElgatoLightStripController(std::string ip)
{
    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location = "IP: " + ip;

    /*-----------------------------------------------------------------*\
    | Open a TCP client sending to the device's IP, port 9123           |
    \*-----------------------------------------------------------------*/
    port.tcp_client(ip.c_str(), "9123");

    /*-----------------------------------------------------------*\
    | Handle responses received from the Elgato LightStrip device |
    \*-----------------------------------------------------------*/
    port.tcp_client_connect();
    std::string buf = "GET /elgato/accessory-info HTTP/1.1\r\nContent-Type: application/json\r\nConnection: close\r\n\r\n";
    port.tcp_client_write((char *)buf.c_str(), (int)buf.length() + 1);

    char recv_buf[1024];
    int size = port.tcp_listen(recv_buf, sizeof(recv_buf));
    port.tcp_close();

    if(size > 0)
    {
        /*-----------------------------------------------------------*\
        | Get response body                                           |
        \*-----------------------------------------------------------*/
        std::istringstream recv_stream(recv_buf);
        std::vector<std::string> recv_list;
        std::string current_line;

        while(std::getline(recv_stream, current_line, '\n'))
        {
            recv_list.push_back(current_line);
        }

        std::string result = recv_list[5];
        json elgato_lightstrip_data = json::parse(result);

        firmware_version = elgato_lightstrip_data["firmwareVersion"];
        serialnumber = elgato_lightstrip_data["serialNumber"];
        displayname = elgato_lightstrip_data["displayName"];

        LOG_DEBUG("[ElgatoLightStrip] [%s]", result.data());
    }
}

ElgatoLightStripController::~ElgatoLightStripController()
{
}

std::string ElgatoLightStripController::GetLocation()
{
    return(location);
}

std::string ElgatoLightStripController::GetName()
{
    return(displayname);
}

std::string ElgatoLightStripController::GetVersion()
{
    return(firmware_version);
}

std::string ElgatoLightStripController::GetManufacturer()
{
    return("Elgato");
}

std::string ElgatoLightStripController::GetUniqueID()
{
    return(serialnumber);
}

void ElgatoLightStripController::SetColor(hsv_t hsv_color)
{
    if(hsv_color.hue > 360)
    {
        hsv_color.hue = 360;
    }

    if(hsv_color.saturation > 100)
    {
        hsv_color.saturation = 100;
    }

    /*-------------------------------------------------*\
    | Delay to prevent it from getting stuck on effects |
    \*-------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    port.tcp_client_connect();
    std::string buf = GetRequest(hsv_color.hue, hsv_color.saturation, GetBrightness());
    port.tcp_client_write((char *)buf.c_str(), (int)buf.length() + 1);
    port.tcp_close();
}

std::string ElgatoLightStripController::GetRequest(int hue, int saturation, int brightness)
{
    json command;

    command["numberOfLights"] = 1;

    json lights = json::array();
    lights.push_back(json::object({ {"on", 1}, {"hue", hue}, {"saturation", saturation}, {"brightness", brightness}}));
    command["lights"] = lights;

    std::string command_str = command.dump();
    std::string buf = "PUT /elgato/lights HTTP/1.1\r\nContent-Type: application/json\r\nContent-Length: " +
                      std::to_string(command_str.length()) +
                      "\r\nConnection: close\r\n\r\n" + command_str + "\r\n\r\n";

    return(buf);
}

int ElgatoLightStripController::GetBrightness()
{
    if(device_brightness > 100 || device_brightness < 0)
    {
        device_brightness = 100;
    }

    return device_brightness;
}

void ElgatoLightStripController::SetBrightness(int brightness)
{
    if(brightness > 100 || device_brightness < 0)
    {
        brightness = 100;
    }

    device_brightness = brightness;
}

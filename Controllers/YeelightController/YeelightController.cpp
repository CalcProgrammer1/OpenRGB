/*---------------------------------------------------------*\
| YeelightController.cpp                                    |
|                                                           |
|   Driver for Yeelight                                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Jan 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "YeelightController.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

YeelightController::YeelightController(std::string ip, std::string host_ip, bool music_mode_val)
{
    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location    = "IP: " + ip;
    music_mode  = music_mode_val;
    this->host_ip = host_ip;

    /*-----------------------------------------------------------------*\
    | Open a TCP client sending to the device's IP, port 38899          |
    \*-----------------------------------------------------------------*/
    port.tcp_client(ip.c_str(), "55443");

    SetPower();

    if(music_mode)
    {
        bool port_opened = false;
        char port_string[8];

        /*-----------------------------------------------------------------*\
        | Start searching for open port for music mode at 55444             |
        \*-----------------------------------------------------------------*/
        music_mode_port = 55444;

        while(!port_opened)
        {
            /*-----------------------------------------------------------------*\
            | Convert port to string                                            |
            \*-----------------------------------------------------------------*/
            snprintf(port_string, 8, "%d", music_mode_port);

            /*-----------------------------------------------------------------*\
            | Open a TCP server for music mode if enabled                       |
            \*-----------------------------------------------------------------*/
            port_opened = music_mode_server.tcp_server(port_string);

            if(!port_opened)
            {
                music_mode_port++;

                /*-------------------------------------------------------------*\
                | If we've tested the maximum port value, 65535, and it failed, |
                | give up and don't use music mode                              |
                \*-------------------------------------------------------------*/
                if(music_mode_port >= 65536)
                {
                    music_mode = false;
                    break;
                }

                continue;
            }

            /*-----------------------------------------------------------------*\
            | Command bulb to connect to our TCP server                         |
            \*-----------------------------------------------------------------*/
            SetMusicMode();

            /*-----------------------------------------------------------------*\
            | Get the client socket for the music mode connection               |
            \*-----------------------------------------------------------------*/
            music_mode_sock = music_mode_server.tcp_server_listen();
        }
    }
}

YeelightController::~YeelightController()
{
}

std::string YeelightController::GetLocation()
{
    return(location);
}

std::string YeelightController::GetName()
{
    return("Yeelight");
}

std::string YeelightController::GetVersion()
{
    return("");
}

std::string YeelightController::GetManufacturer()
{
    return("Yeelight");
}

std::string YeelightController::GetUniqueID()
{
    return("");
}

bool YeelightController::GetMusicMode()
{
    return(music_mode);
}

void YeelightController::SetMusicMode()
{
    json command;

    char            hostname[256];
    char*           ip_addr;
    struct hostent* host_entry;

    /*-----------------------------------------------------------------*\
    | The Yeelight bulb requires this PC's local IP address for music   |
    | mode.  Get the first IP address of this computer's hostname, or   |
    | use the one defined                                               |
    \*-----------------------------------------------------------------*/
    if(host_ip.empty())
    {
        gethostname(hostname, 256);
        host_entry = gethostbyname(hostname);
        ip_addr = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));
    }
    else
    {
        ip_addr = &host_ip[0];
    }

    /*-----------------------------------------------------------------*\
    | Fill in the set_rgb command with RGB information.                 |
    | The bulb will not respond to 0, 0, 0, so if all channels are zero,|
    | set the state to off.  Otherwise, set it to on.                   |
    \*-----------------------------------------------------------------*/
    command["id"]               = 1;
    command["method"]           = "set_music";
    command["params"][0]        = 1;
    command["params"][1]        = ip_addr;
    command["params"][2]        = music_mode_port;

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str     = command.dump().append("\r\n");

    port.tcp_client_connect();
    port.tcp_client_write((char *)command_str.c_str(), (int)command_str.length() + 1);
    port.tcp_close();
}

void YeelightController::SetPower()
{
    json command;

    /*-----------------------------------------------------------------*\
    | Fill in the set_rgb command with RGB information.                 |
    | The bulb will not respond to 0, 0, 0, so if all channels are zero,|
    | set the state to off.  Otherwise, set it to on.                   |
    \*-----------------------------------------------------------------*/
    command["id"]               = 1;
    command["method"]           = "set_power";
    command["params"][0]        = "on";
    command["params"][1]        = "sudden";
    command["params"][2]        = 0;
    command["params"][3]        = 2;

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str     = command.dump().append("\r\n");

    port.tcp_client_connect();
    port.tcp_client_write((char *)command_str.c_str(), (int)command_str.length() + 1);
    port.tcp_close();
}

void YeelightController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    json command;

    /*-----------------------------------------------------------------*\
    | Yeelight doesn't seem to support proper RGB, it just uses RGB to  |
    | calculate hue and saturation.  It doesn't affect brightness.  To  |
    | work around this, determine the highest value and scale to 100 to |
    | use as brightness                                                 |
    \*-----------------------------------------------------------------*/
    float bright = red;

    if(green > bright)
    {
        bright          = green;
    }

    if(blue > bright)
    {
        bright          = blue;
    }

    bright = (100.0f * (bright / 255.0f));

    /*-----------------------------------------------------------------*\
    | Calculate the RGB field as 0x00RRGGBB                             |
    \*-----------------------------------------------------------------*/
    unsigned int rgb = (red << 16) | (green << 8) | (blue << 0);

    /*-----------------------------------------------------------------*\
    | Because of Yeelight's weird quirks with true RGB, we have to use  |
    | the Color Flow option but configure only one frame.  Because the  |
    | set_cf option provides both RGB and brightness in one command, it |
    | allows better RGB control than the set_rgb function.              |
    \*-----------------------------------------------------------------*/
    std::string cf = "50,1," + std::to_string(rgb) +"," + std::to_string((int)bright);

    /*-----------------------------------------------------------------*\
    | Fill in the set_cf command with the color flow string.            |
    \*-----------------------------------------------------------------*/
    command["id"]               = 1;
    command["method"]           = "start_cf";
    command["params"][0]        = 1;
    command["params"][1]        = 1;
    command["params"][2]        = cf;

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str     = command.dump().append("\r\n");

    if(music_mode)
    {
        send(*music_mode_sock, (char *)command_str.c_str(), (int)command_str.length(), 0);
    }
    else
    {
        port.tcp_client_connect();
        port.tcp_client_write((char *)command_str.c_str(), (int)command_str.length() + 1);
        port.tcp_close();
    }
}

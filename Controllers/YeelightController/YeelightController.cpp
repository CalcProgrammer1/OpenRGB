/*---------------------------------------------------------*\
|  Driver for Yeelight                                      |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/18/2021        |
\*---------------------------------------------------------*/

#include "YeelightController.h"
#include "json.hpp"

using json = nlohmann::json;

YeelightController::YeelightController(std::string ip)
{
    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location    = "IP: " + ip;

    /*-----------------------------------------------------------------*\
    | Open a TCP client sending to the device's IP, port 38899          |
    \*-----------------------------------------------------------------*/
    port.tcp_client(ip.c_str(), "55443");
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

void YeelightController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    json command;

    unsigned int rgb = (red << 16) | (green << 8) | (blue << 0);

    /*-----------------------------------------------------------------*\
    | Fill in the set_rgb command with RGB information.                 |
    | The bulb will not respond to 0, 0, 0, so if all channels are zero,|
    | set the state to off.  Otherwise, set it to on.                   |
    \*-----------------------------------------------------------------*/
    command["id"]               = 1;
    command["method"]           = "set_rgb";
    command["params"][0]        = rgb;
    command["params"][1]        = "sudden";
    command["params"][2]        = 0;

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str     = command.dump().append("\r\n");

    port.tcp_client_connect();
    port.tcp_client_write((char *)command_str.c_str(), command_str.length() + 1);
    port.tcp_close();
}

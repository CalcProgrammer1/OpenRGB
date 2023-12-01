/*---------------------------------------------------------*\
|  GoveeController.cpp                                      |
|                                                           |
|  Driver for Govee controller                              |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 12/1/2023        |
\*---------------------------------------------------------*/

#include "GoveeController.h"
#include "json.hpp"

using json = nlohmann::json;

GoveeController::GoveeController(std::string ip)
{
    /*-----------------------------------------------------------------*\
    | Fill in location string with device's IP address                  |
    \*-----------------------------------------------------------------*/
    location    = "IP: " + ip;

    /*-----------------------------------------------------------------*\
    | Open a UDP client sending to the device's IP, port 4003           |
    \*-----------------------------------------------------------------*/
    port.udp_client(ip.c_str(), "4003");
}

GoveeController::~GoveeController()
{

}

std::string GoveeController::GetLocation()
{
    return(location);
}

std::string GoveeController::GetVersion()
{
    return(firmware_version);
}

void GoveeController::SetColor(unsigned char red, unsigned char green, unsigned char blue, unsigned char brightness)
{
    json command;

    command["msg"]["cmd"] = "colorwc";
    command["msg"]["data"]["color"]["r"] = red;
    command["msg"]["data"]["color"]["g"] = green;
    command["msg"]["data"]["color"]["b"] = blue;
    command["msg"]["data"]["colorTemInKelvin"] = "0";

    /*-----------------------------------------------------------------*\
    | Convert the JSON object to a string and write it                  |
    \*-----------------------------------------------------------------*/
    std::string command_str     = command.dump();

    port.udp_write((char *)command_str.c_str(), command_str.length() + 1);
}

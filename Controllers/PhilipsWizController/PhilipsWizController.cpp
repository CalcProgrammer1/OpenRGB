/*---------------------------------------------------------*\
|  Driver for Philips Wiz                                   |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 9/15/2020        |
\*---------------------------------------------------------*/

#include "PhilipsWizController.h"

PhilipsWizController::PhilipsWizController(std::string ip)
{
    location    = "IP: " + ip;

    port.udp_client(ip.c_str(), "38899");
}

PhilipsWizController::~PhilipsWizController()
{

}

std::string PhilipsWizController::GetLocation()
{
    return(location);
}

std::string PhilipsWizController::GetName()
{
    return("");
}

std::string PhilipsWizController::GetVersion()
{
    return("");
}

std::string PhilipsWizController::GetManufacturer()
{
    return("");
}

std::string PhilipsWizController::GetUniqueID()
{
    return("");
}

void PhilipsWizController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    if(red == 0 && green == 0 && blue == 0)
    {
        std::string message = "{\"method\":\"setPilot\",\"params\":{\"r\":" + std::to_string(red) + ",\"g\":" + std::to_string(green) + ",\"b\":" + std::to_string(blue) + ",\"state\":0}}";
        port.udp_write((char *)message.c_str(), message.length() + 1);
    }
    else
    {
        std::string message = "{\"method\":\"setPilot\",\"params\":{\"r\":" + std::to_string(red) + ",\"g\":" + std::to_string(green) + ",\"b\":" + std::to_string(blue) + ",\"state\":1}}";
        port.udp_write((char *)message.c_str(), message.length() + 1);
    }

}

/*---------------------------------------------------------*\
|  Driver for Philips Hue Entertainment Mode                |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 11/6/2020        |
\*---------------------------------------------------------*/

#include "PhilipsHueEntertainmentController.h"

static std::vector<char> HexToBytes(const std::string& hex)
{
    std::vector<char> bytes;

    for (unsigned int i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }

    return bytes;
}

PhilipsHueEntertainmentController::PhilipsHueEntertainmentController(hueplusplus::Bridge& bridge_ptr, hueplusplus::Group& group_ptr):bridge(bridge_ptr),group(group_ptr)
{
    /*-------------------------------------------------*\
    | Fill in location string with bridge IP            |
    \*-------------------------------------------------*/
    location                            = "IP: " + bridge.getBridgeIP();
    num_leds                            = group.getLightIds().size();

    /*-------------------------------------------------*\
    | Create Entertainment Mode from bridge and group   |
    \*-------------------------------------------------*/
    entertainment = new hueplusplus::EntertainmentMode(bridge, group);
}

PhilipsHueEntertainmentController::~PhilipsHueEntertainmentController()
{
}

std::string PhilipsHueEntertainmentController::GetLocation()
{
    return(location);
}

std::string PhilipsHueEntertainmentController::GetName()
{
    return(group.getName());
}

std::string PhilipsHueEntertainmentController::GetVersion()
{
    return("");
}

std::string PhilipsHueEntertainmentController::GetManufacturer()
{
    return("");
}

std::string PhilipsHueEntertainmentController::GetUniqueID()
{
    return("");
}

unsigned int PhilipsHueEntertainmentController::GetNumLEDs()
{
    return(num_leds);
}

void PhilipsHueEntertainmentController::SetColor(RGBColor* colors)
{
    /*-------------------------------------------------*\
    | Fill in Entertainment Mode light data             |
    \*-------------------------------------------------*/
    for(unsigned int light_idx = 0; light_idx < num_leds; light_idx++)
    {
        RGBColor color                  = colors[light_idx];
        unsigned char red               = RGBGetRValue(color);
        unsigned char green             = RGBGetGValue(color);
        unsigned char blue              = RGBGetBValue(color);

        entertainment->SetColorRGB(light_idx, red, green, blue);
    }

    entertainment->Update();
}

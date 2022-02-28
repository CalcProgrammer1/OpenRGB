/*---------------------------------------------------------*\
|  Driver for Philips Hue                                   |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 9/15/2020        |
\*---------------------------------------------------------*/

#include "PhilipsHueController.h"
#include "LogManager.h"

PhilipsHueController::PhilipsHueController(hueplusplus::Light light_ptr, std::string bridge_ip):light(light_ptr)
{
    dark        = false;
    location    = "IP: " + bridge_ip;
}

PhilipsHueController::~PhilipsHueController()
{

}

std::string PhilipsHueController::GetLocation()
{
    return(location);
}

std::string PhilipsHueController::GetName()
{
    return(light.getModelId());
}

std::string PhilipsHueController::GetVersion()
{
    return(light.getSwVersion());
}

std::string PhilipsHueController::GetManufacturer()
{
    return(light.getManufacturername());
}

std::string PhilipsHueController::GetUniqueID()
{
    return(light.getUId());
}

void PhilipsHueController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    hueplusplus::RGB rgb;
    rgb.r = red;
    rgb.g = green;
    rgb.b = blue;

    if((red == 0) && (green == 0) && (blue == 0))
    {
        if(!dark)
        {
            try
            {
                light.setColorRGB(rgb, 0);
            }
            catch(const std::exception& e)
            {
                LOG_ERROR("[PhilipsHueController] An error occured while setting the colors: %s", e.what());
            }
        }

        dark = true;    
    }
    else
    {
        dark = false;

        try
        {
            light.setColorRGB(rgb, 0);
        }
        catch(std::exception& e)
        {
            LOG_ERROR("[PhilipsHueController] An error occured while setting the colors: %s", e.what());
        }
    }
}

/*-----------------------------------------*\
|  CorsairWirelessController.cpp            |
|                                           |
|  Driver for Corsair RGB wireless keyboard |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/8/2021    |
\*-----------------------------------------*/

#include "CorsairWirelessController.h"

#include <cstring>

using namespace std::chrono_literals;

CorsairWirelessController::CorsairWirelessController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    type        = DEVICE_TYPE_KEYBOARD;
}

CorsairWirelessController::~CorsairWirelessController()
{
    hid_close(dev);
}

device_type CorsairWirelessController::GetDeviceType()
{
    return type;
}

std::string CorsairWirelessController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string CorsairWirelessController::GetFirmwareString()
{
    return firmware_version;
}

std::string CorsairWirelessController::GetName()
{
    return name;
}

std::string CorsairWirelessController::GetSerialString()
{
    wchar_t serial_string[128];
    hid_get_serial_number_string(dev, serial_string, 128);

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

void CorsairWirelessController::SetLEDs(std::vector<RGBColor>colors)
{
}

void CorsairWirelessController::SetLEDsKeyboardFull(std::vector<RGBColor> colors)
{
}

void CorsairWirelessController::SetName(std::string device_name)
{
    name = device_name;
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

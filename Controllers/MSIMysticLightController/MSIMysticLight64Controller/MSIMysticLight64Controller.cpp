/*-----------------------------------------*\
|  MSIMysticLight64Controller.cpp           |
|                                           |
|  Driver for MSI Mystic Light (64-byte)    |
|  USB lighting controller                  |
|                                           |
|  T-bond 3/4/2020                          |
|  Adam Honse 3/6/2021                      |
|  Elchanan Haas 8/23/2022                  |
\*-----------------------------------------*/

#include "MSIMysticLight64Controller.h"
#include <algorithm>
#include <array>
#include <bitset>

MSIMysticLight64Controller::MSIMysticLight64Controller
(
    hid_device *handle,
    const char *path
)
{
    dev = handle;
    if(dev)
    {
        location = path;
    }
}

MSIMysticLight64Controller::~MSIMysticLight64Controller()
{
    hid_close(dev);
}

void MSIMysticLight64Controller::SetMode
(
    MSI_64_MODE mode,
    MSI_SPEED speed,
    MSI_BRIGHTNESS brightness,
    unsigned int num_colors,
    Color colors[]
)
{
    FeaturePacket_64 data;
    for(int i = 0; i < MSI_64_MAX_COLORS; i++)
    {
        data.colors[i] = colors[i];
    }
    data.speed      = speed;
    data.brightness = brightness;
    data.num_colors = num_colors;
    data.mode       = mode;
    /*-----------------------------------------------------*\
    | Send packet to hardware, return true if successful    |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, (unsigned char *)&data, sizeof(data));
    return;
}

std::string MSIMysticLight64Controller::GetDeviceName()
{
    wchar_t tname[256];

    /*-----------------------------------------------------*\
    | Get the manufacturer string from HID                  |
    \*-----------------------------------------------------*/
    hid_get_manufacturer_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Convert wchar_t into std::wstring into std::string    |
    \*-----------------------------------------------------*/
    std::wstring wname = std::wstring(tname);
    std::string name = std::string(wname.begin(), wname.end());

    /*-----------------------------------------------------*\
    | Get the product string from HID                       |
    \*-----------------------------------------------------*/
    hid_get_product_string(dev, tname, 256);

    /*-----------------------------------------------------*\
    | Append the product string to the manufacturer string  |
    \*-----------------------------------------------------*/
    wname = std::wstring(tname);
    name.append(" ").append(std::string(wname.begin(), wname.end()));
    return name;
}

std::string MSIMysticLight64Controller::GetFWVersion()
{
    /*-----------------------------------------------------*\
    | This device doesn't support firmware version          |
    \*-----------------------------------------------------*/
    std::string firmware_version = "";
    return firmware_version;
}

std::string MSIMysticLight64Controller::GetDeviceLocation()
{
    return ("HID: " + location);
}

std::string MSIMysticLight64Controller::GetSerial()
{
    wchar_t serial[256];

    /*-----------------------------------------------------*\
    | Get the serial number string from HID                 |
    \*-----------------------------------------------------*/
    hid_get_serial_number_string(dev, serial, 256);

    /*-----------------------------------------------------*\
    | Convert wchar_t into std::wstring into std::string    |
    \*-----------------------------------------------------*/
    std::wstring wserial = std::wstring(serial);

    return (std::string(wserial.begin(), wserial.end()));
}

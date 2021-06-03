/*-------------------------------------------*\
|  HyperXPulsefireDartController.cpp          |
|                                             |
|  Driver for HyperX Pulsefire Dart           |
|  lighting controller                        |
|                                             |
|  Santeri Pikarinen (santeri3700) 12/26/2020 |
\*-------------------------------------------*/

#include "HyperXPulsefireDartController.h"

#include <cstring>

HyperXPulsefireDartController::HyperXPulsefireDartController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

HyperXPulsefireDartController::~HyperXPulsefireDartController()
{

}

std::string HyperXPulsefireDartController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string HyperXPulsefireDartController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXPulsefireDartController::SendDirect
    (
    RGBColor        color,
    int             led,
    int             mode,
    int             brightness,
    int             speed
    )
{
    unsigned char buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode packet                             |
    \*-----------------------------------------------------*/
    buf[0x00]   = HYPERX_PULSEFIRE_DART_PACKET_ID_DIRECT;
    buf[0x01]   = led;
    buf[0x02]   = mode;
    buf[0x03]   = 0x08; // 8 bytes after buffer index 0x03
    
    buf[0x04]   = RGBGetRValue(color);
    buf[0x05]   = RGBGetGValue(color);
    buf[0x06]   = RGBGetBValue(color);

    buf[0x07]   = RGBGetRValue(color);
    buf[0x08]   = RGBGetGValue(color);
    buf[0x09]   = RGBGetBValue(color);

    buf[0x0a]   = brightness;
    buf[0x0b]   = speed;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)buf, sizeof(buf));
}

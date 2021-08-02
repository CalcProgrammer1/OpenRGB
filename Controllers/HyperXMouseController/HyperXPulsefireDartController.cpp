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
    hid_close(dev);
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
    unsigned char buf[HYPERX_PULSEFIRE_DART_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode packet                             |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = HYPERX_PULSEFIRE_DART_PACKET_ID_DIRECT;
    buf[0x02]   = led;
    buf[0x03]   = mode;
    buf[0x04]   = 0x08; // 8 bytes after buffer index 0x04
    
    buf[0x05]   = RGBGetRValue(color);
    buf[0x06]   = RGBGetGValue(color);
    buf[0x07]   = RGBGetBValue(color);

    buf[0x08]   = RGBGetRValue(color);
    buf[0x09]   = RGBGetGValue(color);
    buf[0x0a]   = RGBGetBValue(color);

    buf[0x0b]   = brightness;
    buf[0x0c]   = speed;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)buf, sizeof(buf));
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void HyperXPulsefireDartController::Save()
{
    /*-----------------------------------------------------*\
    | Save current settings to the on-board memory          |
    \*-----------------------------------------------------*/
    unsigned char buf[HYPERX_PULSEFIRE_DART_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Save packet                                    |
    \*-----------------------------------------------------*/
    buf[0x00] = 0x00;
    buf[0x01] = 0xde;
    buf[0x02] = 0xff;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)buf, sizeof(buf));
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

/*-----------------------------------------*\
|  HyperXPulsefireHasteController.cpp       |
|                                           |
|  Driver for HyperX Pulsefire Haste        |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/19/2021   |
\*-----------------------------------------*/

#include "HyperXPulsefireHasteController.h"

#include <cstring>

HyperXPulsefireHasteController::HyperXPulsefireHasteController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

HyperXPulsefireHasteController::~HyperXPulsefireHasteController()
{
    hid_close(dev);
}

std::string HyperXPulsefireHasteController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXPulsefireHasteController::GetSerialString()
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

void HyperXPulsefireHasteController::SendDirect
    (
    RGBColor*       color_data
    )
{
    SendDirectSetup();
    SendDirectColor(color_data);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXPulsefireHasteController::SendDirectSetup()
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode Setup packet                       |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = HYPERX_PULSEFIRE_HASTE_PACKET_ID_SETUP;
    buf[0x02]   = 0xF2;

    buf[0x08]   = 0x02;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);
}

void HyperXPulsefireHasteController::SendDirectColor
    (
    RGBColor*       color_data
    )
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode packet                             |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = HYPERX_PULSEFIRE_HASTE_PACKET_ID_COLOR;
    
    buf[0x02]   = RGBGetRValue(color_data[0]);
    buf[0x03]   = RGBGetGValue(color_data[0]);
    buf[0x04]   = RGBGetBValue(color_data[0]);

    buf[0x08]   = 0x02;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);
}

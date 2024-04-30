/*-----------------------------------------*\
|  HyperXPulsefireFPSProController.cpp      |
|                                           |
|  Driver for HyperX Pulsefire FPS Pro      |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 12/26/2020  |
\*-----------------------------------------*/

#include "HyperXPulsefireFPSProController.h"

#include <cstring>

HyperXPulsefireFPSProController::HyperXPulsefireFPSProController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

HyperXPulsefireFPSProController::~HyperXPulsefireFPSProController()
{
    hid_close(dev);
}

std::string HyperXPulsefireFPSProController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXPulsefireFPSProController::GetSerialString()
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

void HyperXPulsefireFPSProController::SendDirect
    (
    RGBColor*       color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct Mode packet                             |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_FPS_PRO_PACKET_ID_DIRECT;
    
    buf[0x02]   = RGBGetRValue(color_data[0]);
    buf[0x03]   = RGBGetGValue(color_data[0]);
    buf[0x04]   = RGBGetBValue(color_data[0]);

    buf[0x08]   = 0xA0;
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

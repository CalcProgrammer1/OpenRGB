/*-----------------------------------------*\
|  HyperXMousematController.cpp             |
|                                           |
|  Driver for HyperX Mousemat lighting      |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 10/25/2020  |
\*-----------------------------------------*/

#include "HyperXMousematController.h"

#include <cstring>

HyperXMousematController::HyperXMousematController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

HyperXMousematController::~HyperXMousematController()
{
    hid_close(dev);
}

std::string HyperXMousematController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXMousematController::GetSerialString()
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

void HyperXMousematController::SendDirect
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
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    buf[0x01]   = 0x04;
    buf[0x02]   = 0xF2;

    buf[0x09]   = 0x02;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    
    for(int i = 0; i < 16; i++)
    {
        buf[(i * 4) + 1] = 0x81;
        buf[(i * 4) + 2] = RGBGetRValue(color_data[i]);
        buf[(i * 4) + 3] = RGBGetGValue(color_data[i]);
        buf[(i * 4) + 4] = RGBGetBValue(color_data[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 65);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x00;
    
    for(int i = 0; i < 16; i++)
    {
        buf[(i * 4) + 1] = 0x81;
        buf[(i * 4) + 2] = RGBGetRValue(color_data[16 + i]);
        buf[(i * 4) + 3] = RGBGetGValue(color_data[16 + i]);
        buf[(i * 4) + 4] = RGBGetBValue(color_data[16 + i]);
    }

    hid_send_feature_report(dev, buf, 65);
}

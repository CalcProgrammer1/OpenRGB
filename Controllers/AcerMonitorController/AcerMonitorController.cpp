/*---------------------------------------------------------*\
|  AcerMonitorController.cpp                                |
|                                                           |
|  Driver for 2020/2021 Acer monitors with rear RGB strips. |
|                                                           |
|  Shenghao Yang (me@shenghaoyang.info), 30/12/2021         |
|  Amended for code style by Adam Honse, 3/13/2022          |
\*---------------------------------------------------------*/

#include "AcerMonitorController.h"
#include <cstring>

AcerMonitorController::AcerMonitorController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;
}

AcerMonitorController::~AcerMonitorController()
{
    hid_close(dev);
}

std::string AcerMonitorController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AcerMonitorController::GetSerialString()
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

void AcerMonitorController::SetColors(RGBColor *colors, std::size_t num_leds)
{
    char usb_buf[374];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Fill in Set Colors packet                             |
    \*-----------------------------------------------------*/
    usb_buf[ACER_MONITOR_OFFSET_REPORT_TYPE]    = ACER_MONITOR_REPORT_TYPE;
    usb_buf[ACER_MONITOR_OFFSET_MAGIC_0]        = ACER_MONITOR_MAGIC_0;
    usb_buf[ACER_MONITOR_OFFSET_MAGIC_1]        = ACER_MONITOR_MAGIC_1;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(unsigned int color_idx = 0; color_idx < num_leds; color_idx++)
    {
        usb_buf[ACER_MONITOR_OFFSET_RGB_DATA + 0 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        usb_buf[ACER_MONITOR_OFFSET_RGB_DATA + 1 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
        usb_buf[ACER_MONITOR_OFFSET_RGB_DATA + 2 + (color_idx * 3)] = RGBGetBValue(colors[color_idx]);
    }
    
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 374);
}

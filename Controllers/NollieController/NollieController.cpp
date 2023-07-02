/*-----------------------------------------*\
|  NollieController.cpp                     |
|                                           |
|  Driver for Nollie32 LED strip controller |
|                                           |
|  Name (cnn1236661)          25th Jun 2023 |
\*-----------------------------------------*/

#include "NollieController.h"
#include <cstring>

using namespace std::chrono_literals;

NollieController::NollieController(hid_device* dev_handle, const char* path, unsigned short pid)
{
    dev         = dev_handle;
    location    = path;
    /*-----------------------------------------------------*\
    | PID may be used in the future, here is to pass       |
    | arguments not to do storage                          |
    \*-----------------------------------------------------*/
}

std::string NollieController::GetLocationString()
{
    return("HID: " + location);
}

std::string NollieController::GetSerialString()
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

void NollieController::SetMos(bool mos)
{
    unsigned char   usb_buf[65];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x01]  = 0x80;
    usb_buf[0x02]  = mos;
    hid_write(dev, usb_buf, 65);
}

void NollieController::SetChannelLEDs(unsigned char channel, RGBColor* colors, unsigned int num_colors)
{
   SendPacket(channel,&colors[0], num_colors);
}

void NollieController::SendPacket(unsigned char channel,RGBColor* colors,unsigned int num_colors)
{

    unsigned char   usb_buf[1025];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[1] = channel;
    usb_buf[2] = 0;
    usb_buf[3] = num_colors / 256;
    usb_buf[4] = num_colors % 256;
    if(!num_colors)
    {
        hid_write(dev, usb_buf, 1025);
        return;
    }
    for(unsigned int color_idx = 0; color_idx < num_colors; color_idx++)
    {
        usb_buf[0x05 + (color_idx * 3)] = RGBGetGValue(colors[color_idx]);
        usb_buf[0x06 + (color_idx * 3)] = RGBGetRValue(colors[color_idx]);
        usb_buf[0x07 + (color_idx * 3)] = RGBGetBValue(colors[color_idx]);
    }
    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 1025);
}


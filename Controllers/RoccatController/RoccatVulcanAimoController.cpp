/*-------------------------------------------------------------------*\
|  RoccatVulcanAimoController.cpp                                     |
|                                                                     |
|  Driver for Roccat Vulcan Aimo Mouse                                |
|                                                                     |
|  Mola19 17/12/2021                                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RoccatVulcanAimoController.h"

#include <cstring>
#include <math.h>

RoccatVulcanAimoController::RoccatVulcanAimoController(hid_device* dev_ctrl_handle, hid_device* dev_led_handle, char *path)
{
    dev_ctrl    = dev_ctrl_handle;
    dev_led     = dev_led_handle;
    location    = path;
}

RoccatVulcanAimoController::~RoccatVulcanAimoController()
{
    hid_close(dev_ctrl);
    hid_close(dev_led);
}

std::string RoccatVulcanAimoController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_ctrl, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);

}

std::string RoccatVulcanAimoController::GetLocation()
{
    return("HID: " + location);
}


device_info RoccatVulcanAimoController::InitDeviceInfo()
{
    unsigned char usb_buf[8] = { 0x0F };
    hid_get_feature_report(dev_ctrl, usb_buf, 8);

    dev_info.version = std::to_string((int) floor(usb_buf[2] / 100)) + "." + std::to_string(usb_buf[2] % 100);

    dev_info.layout_type    = usb_buf[6];
    dev_info.layout_variant = usb_buf[7];

    return dev_info;
}

device_info RoccatVulcanAimoController::GetDeviceInfo()
{
    return dev_info;
}


void RoccatVulcanAimoController::SendColors(std::vector<led_color> colors)
{
    unsigned char bufs[7][65];

    for(int p = 0; p < 7; p++)
    {
        memset(bufs[p], 0x00, 65);
    }

    bufs[0][1] = 0xA1;
    bufs[0][2] = 0x01;
    bufs[0][3] = 0x01;
    bufs[0][4] = 0xB4;

    for(unsigned int i = 0; i < colors.size(); i++)
    {
        int coloumn = floor(colors[i].value / 12);
        int row = colors[i].value % 12;

        int offset = coloumn * 36 + row + 4;

        bufs[offset / 64][offset % 64 + 1] = RGBGetRValue(colors[i].color);

        offset += 12;

        bufs[offset / 64][offset % 64 + 1] = RGBGetGValue(colors[i].color);

        offset += 12;

        bufs[offset / 64][offset % 64 + 1] = RGBGetBValue(colors[i].color);
    }

    for(int p = 0; p < 7; p++)
    {
        hid_write(dev_led, bufs[p], 65);
    }
}

void RoccatVulcanAimoController::SendMode(unsigned int mode, unsigned int speed, unsigned int brightness, std::vector<led_color> colors)
{
	if(speed      == 0) speed      = 1;
	if(brightness == 0) brightness = 1;

    unsigned char buf[443];

    memset(buf, 0x00, 443);

    buf[0] = 0x0D;
    buf[1] = 0xBB;
    buf[2] = 0x01;
    buf[3] = 0x00;
    buf[4] = mode;
    buf[5] = speed;
    buf[6] = 0x05;
    buf[7] = brightness;
    buf[8] = 0x00;

    if(mode == ROCCAT_VULCAN_MODE_STATIC)
    {

        for(unsigned int i = 0; i < colors.size(); i++)
        {
            int coloumn = floor(colors[i].value / 12);
            int row = colors[i].value % 12;

            int offset = coloumn * 36 + row + 9;

            buf[offset] = RGBGetRValue(colors[i].color);

            offset += 12;

            buf[offset] = RGBGetGValue(colors[i].color);

            offset += 12;

            buf[offset] = RGBGetBValue(colors[i].color);
        }
    }

    unsigned short total = 0;
    for(int i = 0; i < 441; i++) total += buf[i];

    buf[441] = total & 0xFF;
    buf[442] = total >> 8;

    hid_send_feature_report(dev_ctrl, buf, 443);
}

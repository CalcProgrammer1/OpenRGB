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

std::string RoccatVulcanAimoController::GetVersion()
{
    unsigned char usb_buf[8] = { 0x0f };
    hid_get_feature_report(dev_ctrl, usb_buf, 8);

    return std::to_string((int) floor(usb_buf[2] / 100)) + "." + std::to_string(usb_buf[2] % 100);
}

void RoccatVulcanAimoController::SendColors(std::vector<RGBColor> colors)
{
    unsigned char bufs[7][65];

    bufs[0][1] = 0xA1;
    bufs[0][2] = 0x01;
    bufs[0][3] = 0x01;
    bufs[0][4] = 0xB4;

    for(int z = 0; z < 11; z++)
    {
        for(int l = 0; l < 12; l++)
        {
            int placeR = (z * 36) + 4 + l +  0;
            bufs[placeR / 64][placeR % 64 + 1] = RGBGetRValue(colors[z * 12 + l]);

            int placeG = (z * 36) + 4 + l + 12;
            bufs[placeG / 64][placeG % 64 + 1] = RGBGetGValue(colors[z * 12 + l]);

            int placeB = (z * 36) + 4 + l + 24;
            bufs[placeB / 64][placeB % 64 + 1] = RGBGetBValue(colors[z * 12 + l]);
        }
    }

    for(int p = 0; p < 7; p++)
    {
        hid_write(dev_led, bufs[p], 65);
    }
}

void RoccatVulcanAimoController::SendMode(unsigned int mode, unsigned int speed, unsigned int brightness, std::vector<RGBColor> colors)
{
	if(speed      == 0) speed      = 1;
	if(brightness == 0) brightness = 1;

    unsigned char buf[443];

    memset(buf, 0x00, 443);

    buf[0] = 0x0d;
    buf[1] = 0xbb;
    buf[2] = 0x01;
    buf[3] = 0x00;
    buf[4] = mode;
    buf[5] = speed;
    buf[6] = 0x05;
    buf[7] = brightness;
    buf[8] = 0x00;

    if(mode == ROCCAT_VULCAN_MODE_STATIC)
    {
        for(int z = 0; z < 11; z++)
        {
            for(int l = 0; l < 12; l++)
            {
                int placeR      = (z * 36) + 8 + l +  0;
                buf[placeR + 1] = RGBGetRValue(colors[z * 12 + l]);

                int placeG      = (z * 36) + 8 + l + 12;
                buf[placeG + 1] = RGBGetGValue(colors[z * 12 + l]);

                int placeB      = (z * 36) + 8 + l + 24;
                buf[placeB + 1] = RGBGetBValue(colors[z * 12 + l]);
            }
        }
    }

    unsigned short total = 0;
    for(int i = 0; i < 441; i++) total += buf[i];

    buf[441] = total & 0xff;
    buf[442] = total >> 8;

    hid_send_feature_report(dev_ctrl, buf, 443);
}

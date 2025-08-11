/*---------------------------------------------------------*\
| HYTEKeyboardController.cpp                                |
|                                                           |
|   Driver for HYTE keyboard                                |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      30 Oct 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HYTEKeyboardController.h"

HYTEKeyboardController::HYTEKeyboardController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
}

HYTEKeyboardController::~HYTEKeyboardController()
{

}

std::string HYTEKeyboardController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HYTEKeyboardController::GetDeviceName()
{
    return(name);
}

void HYTEKeyboardController::LEDStreaming(unsigned int zone, RGBColor* colors)
{
    /*-----------------------------------------------------*\
    | Call the appropriate LEDStreaming function for the    |
    | given zone                                            |
    \*-----------------------------------------------------*/
    switch(zone)
    {
        case HYTE_KEYBOARD_ZONE_KEYBOARD:
            LEDStreaming_Keyboard(colors);
            break;

        case HYTE_KEYBOARD_ZONE_SURROUND:
            LEDStreaming_Surround(colors);
            break;
    }
}

void HYTEKeyboardController::LEDStreaming_Keyboard(RGBColor* colors)
{
    /*-----------------------------------------------------*\
    | LED Streaming - Keyboard RGB                          |
    |                                                       |
    | Set Feature (9 bytes, the first byte is 0x00)         |
    |   0x04 0xF0 0x00 0x00 0x00 0x00 0x00 0x00             |
    |                                                       |
    | EP6 Write (6 pages, 65 bytes/page)                    |
    |   0xRR 0xGG 0xBB 0xRR 0xGG 0xBB ...                   |
    \*-----------------------------------------------------*/
    unsigned char usb_feature_buf[9];
    unsigned char usb_buf[6][65];

    /*-----------------------------------------------------*\
    | Set up feature report                                 |
    \*-----------------------------------------------------*/
    memset(usb_feature_buf, 0, sizeof(usb_feature_buf));

    usb_feature_buf[0]  = 0x00;
    usb_feature_buf[1]  = 0x04;
    usb_feature_buf[2]  = 0xF0;

    /*-----------------------------------------------------*\
    | Set up data packets                                   |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0][0] = 0x00;
    usb_buf[1][0] = 0x00;
    usb_buf[2][0] = 0x00;
    usb_buf[3][0] = 0x00;
    usb_buf[4][0] = 0x00;
    usb_buf[5][0] = 0x00;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    unsigned int color_idx = 0;
    unsigned int channel_idx = 0;

    for(unsigned int pkt_idx = 0; pkt_idx < 6; pkt_idx++)
    {
        for(unsigned int byte_idx = 0; byte_idx < 64; byte_idx++)
        {
            switch(channel_idx)
            {
                case 0:
                    usb_buf[pkt_idx][1 + byte_idx] = RGBGetRValue(colors[color_idx]);
                    break;

                case 1:
                    usb_buf[pkt_idx][1 + byte_idx] = RGBGetGValue(colors[color_idx]);
                    break;

                case 2:
                    usb_buf[pkt_idx][1 + byte_idx] = RGBGetBValue(colors[color_idx]);
                    color_idx++;
                    break;
            }

            channel_idx = ( channel_idx + 1 ) % 3;
        }
    }

    /*-----------------------------------------------------*\
    | Send the data                                         |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_feature_buf, sizeof(usb_feature_buf));
    hid_write(dev, usb_buf[0], sizeof(usb_buf[0]));
    hid_write(dev, usb_buf[1], sizeof(usb_buf[1]));
    hid_write(dev, usb_buf[2], sizeof(usb_buf[2]));
    hid_write(dev, usb_buf[3], sizeof(usb_buf[3]));
    hid_write(dev, usb_buf[4], sizeof(usb_buf[4]));
    hid_write(dev, usb_buf[5], sizeof(usb_buf[5]));
}

void HYTEKeyboardController::LEDStreaming_Surround(RGBColor* colors)
{
    /*-----------------------------------------------------*\
    | LED Streaming - Surround RGB                          |
    |                                                       |
    | Set Feature (9 bytes, the first byte is 0x00)         |
    |   0x04 0xF1 0x00 0x00 0x00 0x00 0x00 0x00             |
    |                                                       |
    | EP6 Write (3 pages, 65 bytes/page)                    |
    |   0xRR 0xGG 0xBB 0xRR 0xGG 0xBB ...                   |
    \*-----------------------------------------------------*/
    unsigned char usb_feature_buf[9];
    unsigned char usb_buf[3][65];

    /*-----------------------------------------------------*\
    | Set up feature report                                 |
    \*-----------------------------------------------------*/
    memset(usb_feature_buf, 0, sizeof(usb_feature_buf));

    usb_feature_buf[0]  = 0x00;
    usb_feature_buf[1]  = 0x04;
    usb_feature_buf[2]  = 0xF1;

    /*-----------------------------------------------------*\
    | Set up data packets                                   |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0][0] = 0x00;
    usb_buf[1][0] = 0x00;
    usb_buf[2][0] = 0x00;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    unsigned int color_idx = 0;
    unsigned int channel_idx = 0;

    for(unsigned int pkt_idx = 0; pkt_idx < 3; pkt_idx++)
    {
        for(unsigned int byte_idx = 0; byte_idx < 64; byte_idx++)
        {
            switch(channel_idx)
            {
                case 0:
                    usb_buf[pkt_idx][1 + byte_idx] = RGBGetRValue(colors[color_idx]);
                    break;

                case 1:
                    usb_buf[pkt_idx][1 + byte_idx] = RGBGetGValue(colors[color_idx]);
                    break;

                case 2:
                    usb_buf[pkt_idx][1 + byte_idx] = RGBGetBValue(colors[color_idx]);
                    color_idx++;
                    break;
            }

            channel_idx = ( channel_idx + 1 ) % 3;
        }
    }

    /*-----------------------------------------------------*\
    | Send the data                                         |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_feature_buf, sizeof(usb_feature_buf));
    hid_write(dev, usb_buf[0], sizeof(usb_buf[0]));
    hid_write(dev, usb_buf[1], sizeof(usb_buf[1]));
    hid_write(dev, usb_buf[2], sizeof(usb_buf[2]));
}


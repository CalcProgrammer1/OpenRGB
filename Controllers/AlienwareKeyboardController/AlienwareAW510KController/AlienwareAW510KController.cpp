/*---------------------------------------------------------*\
| AlienwareAW510KController.cpp                             |
|                                                           |
|   Driver for Alienware AW510K keyboard                    |
|                                                           |
|   Mohamad Sallal (msallal)                    22 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AlienwareAW510KController.h"
#include "StringUtils.h"

AlienwareAW510KController::AlienwareAW510KController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    SendCommit();
}

AlienwareAW510KController::~AlienwareAW510KController()
{
    hid_close(dev);
}

std::string AlienwareAW510KController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AlienwareAW510KController::GetDeviceName()
{
    return(name);
}

std::string AlienwareAW510KController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void AlienwareAW510KController::SendCommit()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x05;
    usb_buf[0x02]           = 0x01;
    usb_buf[0x0A]           = 0x10;
    usb_buf[0x0B]           = 0x0A;
    usb_buf[0x0C]           = 0x01;
    usb_buf[0x0D]           = 0x02;
    usb_buf[0x0E]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Delay 20 milliseconds                                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
}

void AlienwareAW510KController::SendfeatureReport
    (
    unsigned char   first_byte,
    unsigned char   second_byte,
    unsigned char   third_byte,
    unsigned char   forth_byte
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Feature report packet                          |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = first_byte;
    usb_buf[0x02]           = second_byte;
    usb_buf[0x03]           = third_byte;
    usb_buf[0x04]           = forth_byte;

    /*-----------------------------------------------------*\
    | Send Feature report packet                            |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Delay 10 milliseconds                                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void AlienwareAW510KController::SendEdit()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Edit packet                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x01;
    usb_buf[0x02]           = 0x00;
    usb_buf[0x03]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Delay 2 milliseconds                                  |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void AlienwareAW510KController::SendInitialize()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x0E;
    usb_buf[0x02]           = 0x01;
    usb_buf[0x03]           = 0x00;
    usb_buf[0x04]           = 0x01;
    usb_buf[0x05]           = 0xAD;
    usb_buf[0x06]           = 0x80;
    usb_buf[0x07]           = 0x10;
    usb_buf[0x08]           = 0xA5;
    usb_buf[0x0A]           = 0x0A;
    usb_buf[0x12]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Delay 2 milliseconds                                  |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void AlienwareAW510KController::SetDirect
    (
    unsigned char   /*zone*/,
    unsigned char   r,
    unsigned char   g,
    unsigned char   b
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/

    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x05;
    usb_buf[0x02]           = 0x01;
    usb_buf[0x03]           = 0x01;
    usb_buf[0x04]           = r;
    usb_buf[0x05]           = g;
    usb_buf[0x06]           = b;
    usb_buf[0x0B]           = 0x0A;
    usb_buf[0x0D]           = 0x01;
    usb_buf[0x0E]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, (unsigned char *)usb_buf, 65);

    /*-----------------------------------------------------*\
    | Delay 2 milliseconds                                  |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void AlienwareAW510KController::SendDirectOn
    (
    std::vector<SelectedKeys>     &frame_data
    )
{
    SendfeatureReport(0x0E, (unsigned char)frame_data.size(), 0x00, 0x01);

    /*-----------------------------------------------*\
    | To Guarantee the data are always %4 =0 append   |
    | zeros at end of last packet                     |
    \*-----------------------------------------------*/
    for(unsigned int i = 0; i < (frame_data.size() % 4); i++)
    {
        SelectedKeys key;
        key.idx     = 0x00;
        key.red     = 0x00;
        key.green   = 0x00;
        key.blue    = 0x00;

        frame_data.push_back(key);
    }

    unsigned char usb_buf[65];
    unsigned int  frame_idx = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, 65);

    for(unsigned int packet_idx = 0; packet_idx < frame_data.size(); packet_idx++)
    {
        usb_buf[0x00]           = 0x00;
        usb_buf[0x01]           = 0x0E;
        usb_buf[0x02]           = 0x01;
        usb_buf[0x03]           = 0x00;
        usb_buf[0x04]           = ++frame_idx;
        usb_buf[0x05]           = frame_data[packet_idx].idx;
        usb_buf[0x06]           = 0x81;
        usb_buf[0x07]           = 0x00;
        usb_buf[0x08]           = 0xA5;
        usb_buf[0x0A]           = 0x0A;
        usb_buf[0x0B]           = frame_data[packet_idx].red;
        usb_buf[0x0C]           = frame_data[packet_idx].green;
        usb_buf[0x0D]           = frame_data[packet_idx].blue;
        usb_buf[0x12]           = 0x01;

        usb_buf[0x14]           = frame_data[++packet_idx].idx;
        usb_buf[0x15]           = 0x81;
        usb_buf[0x16]           = 0x00;
        usb_buf[0x17]           = 0xA5;
        usb_buf[0x19]           = 0x0A;
        usb_buf[0x1A]           = frame_data[packet_idx].red;
        usb_buf[0x1B]           = frame_data[packet_idx].green;
        usb_buf[0x1C]           = frame_data[packet_idx].blue;
        usb_buf[0x21]           = 0x01;

        usb_buf[0x23]           = frame_data[++packet_idx].idx;
        usb_buf[0x24]           = 0x81;
        usb_buf[0x25]           = 0x00;
        usb_buf[0x26]           = 0xA5;
        usb_buf[0x28]           = 0x0A;
        usb_buf[0x29]           = frame_data[packet_idx].red;
        usb_buf[0x2A]           = frame_data[packet_idx].green;
        usb_buf[0x2B]           = frame_data[packet_idx].blue;
        usb_buf[0x30]           = 0x01;

        usb_buf[0x32]           = frame_data[++packet_idx].idx;
        usb_buf[0x33]           = 0x81;
        usb_buf[0x34]           = 0x00;
        usb_buf[0x35]           = 0xA5;
        usb_buf[0x37]           = 0x0A;
        usb_buf[0x38]           = frame_data[packet_idx].red;
        usb_buf[0x39]           = frame_data[packet_idx].green;
        usb_buf[0x3A]           = frame_data[packet_idx].blue;
        usb_buf[0x3F]           = 0x01;

        hid_write(dev, (unsigned char *)usb_buf, 65);
    }
}


void AlienwareAW510KController::SetMode
    (
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   direction,
    unsigned char   colorMode,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    SendMode(ALIENWARE_AW510K_ZONE_MODE_KEYBOARD, mode, speed, direction, colorMode, red, green, blue);
    SendCommit();
}

void AlienwareAW510KController::UpdateSingleLED
    (
    unsigned char   led,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    SendfeatureReport(0x0E, 0x01, 0x00, 0x01);

    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Single LED packet                              |
    \*-----------------------------------------------------*/

    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x0E;
    usb_buf[0x02]           = 0x01;
    usb_buf[0x03]           = 0x00;
    usb_buf[0x04]           = 0x01;
    usb_buf[0x05]           = led;
    usb_buf[0x06]           = 0x81;
    usb_buf[0x07]           = 0x00;
    usb_buf[0x08]           = 0xA5;
    usb_buf[0x09]           = 0x00;
    usb_buf[0x0A]           = 0x0A;
    usb_buf[0x0B]           = red;
    usb_buf[0x0C]           = green;
    usb_buf[0x0D]           = blue;
    usb_buf[0x12]           = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Delay 20 milliseconds                                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

}
void AlienwareAW510KController::SendMode
    (
    unsigned char   /*zone*/,
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   direction,
    unsigned char   colorMode,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Mode Control packet                            |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x05;
    usb_buf[0x02]           = 0x01;
    usb_buf[0x03]           = mode;
    usb_buf[0x04]           = red;
    usb_buf[0x05]           = green;
    usb_buf[0x06]           = blue;
    usb_buf[0x0A]           = speed;
    usb_buf[0x0B]           = 0x0A;
    usb_buf[0x0D]           = 0x01;
    usb_buf[0x0E]           = colorMode;
    usb_buf[0x0F]           = direction;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

void AlienwareAW510KController::SetMorphMode
    (
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   red1,
    unsigned char   green1,
    unsigned char   blue1,
    unsigned char   red2,
    unsigned char   green2,
    unsigned char   blue2
    )
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Morph Mode packet                              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x00;
    usb_buf[0x01]           = 0x05;
    usb_buf[0x02]           = 0x01;
    usb_buf[0x03]           = mode;
    usb_buf[0x04]           = red1;
    usb_buf[0x05]           = green1;
    usb_buf[0x06]           = blue1;
    usb_buf[0x07]           = red2;
    usb_buf[0x08]           = green2;
    usb_buf[0x09]           = blue2;
    usb_buf[0x0E]           = 0x02;
    usb_buf[0x0A]           = speed;
    usb_buf[0x0B]           = 0x0A;
    usb_buf[0x0D]           = 0x01;
    usb_buf[0x0E]           = ALIENWARE_AW510K_TWO_USER_DEFINED_COLOR_MODE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
}

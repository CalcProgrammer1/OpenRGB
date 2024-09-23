/*---------------------------------------------------------*\
| AirgooLustrousCommanderController.cpp                     |
|                                                           |
|   Driver for Airgoo Lustrous Commander                    |
|                                                           |
|   Zacahry Guinn                               07 Feb 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <iomanip>
#include <iostream>
#include "AirgooLustrousCommanderController.h"

using namespace std::chrono_literals;

AirgooLustrousCommanderController::AirgooLustrousCommanderController(hid_device* dev_handle, const char* path)
{
    dev                     = dev_handle;
    location                = path;

    /*-----------------------------------------------------*\
    | Initialize controller                                 |
    \*-----------------------------------------------------*/
    InitController();
}

AirgooLustrousCommanderController::~AirgooLustrousCommanderController()
{
    /*-----------------------------------------------------*\
    | Close HID device                                      |
    \*-----------------------------------------------------*/
    hid_close(dev);
}

void AirgooLustrousCommanderController::InitController()
{
    unsigned char   usb_buf[AIRGOO_LUSTROUS_COMMANDER_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Send initialization packet                            |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]       = 0x00;
    usb_buf[0x01]       = 0x03;
    usb_buf[0x02]       = 0x55;
    usb_buf[0x03]       = 0xAA;

    hid_write(dev, usb_buf, AIRGOO_LUSTROUS_COMMANDER_PACKET_SIZE);

    //expecting the device to return AA 55 01
    hid_read(dev, usb_buf, AIRGOO_LUSTROUS_COMMANDER_PACKET_SIZE);

    usb_buf[0x00]       = 0x00;
    usb_buf[0x01]       = 0x03;
    usb_buf[0x02]       = 0x55;
    usb_buf[0x03]       = 0xAA;
    usb_buf[0x04]       = 0x01;

    //looks like the client software awknolleges? it returns a 0x01 in pos 4 of the buffer array
    hid_write(dev, usb_buf, AIRGOO_LUSTROUS_COMMANDER_PACKET_SIZE);

    //device will send aa 55 00 back and it seems that we should be ready to send commands at this point
}

std::string AirgooLustrousCommanderController::GetLocationString()
{
    return("HID: " + location);
}

void AirgooLustrousCommanderController::SetMode(unsigned char mode)
{
    active_mode = mode;
}

void AirgooLustrousCommanderController::UpdateDevice
    (
    unsigned char   mode,
    unsigned char   red,
    unsigned char   grn,
    unsigned char   blu,
    unsigned char   speed
    )
{
    unsigned char usb_buf[65];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet with leading 00                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]       = 0x00;

    switch(mode)
    {
        case 0: //off
            usb_buf[0x01]       = 0x09; //number of bytes in package
            usb_buf[0x02]       = 0xFF;
            usb_buf[0x03]       = 0x00;
            usb_buf[0x04]       = 0xF0;
            usb_buf[0x05]       = 0x00; //Red
            usb_buf[0x06]       = 0x00; //Green
            usb_buf[0x07]       = 0x00; //Blue
            usb_buf[0x08]       = 0x05;
            usb_buf[0x09]       = 0x05;
            usb_buf[0x0A]       = 0xFE;
        break;
        case 1: //static
            usb_buf[0x01]       = 0x09; //number of bytes in package
            usb_buf[0x02]       = 0xFF;
            usb_buf[0x03]       = 0x00;
            usb_buf[0x04]       = 0xF0;
            usb_buf[0x05]       = red; //Red
            usb_buf[0x06]       = grn; //Green
            usb_buf[0x07]       = blu; //Blue
            usb_buf[0x08]       = 0x05;
            usb_buf[0x09]       = 0x05;
            usb_buf[0x0A]       = 0xFE;
        break;
        case 2://rainbow
            usb_buf[0x01]       = 0x05; //number of bytes in package
            usb_buf[0x02]       = 0xFF;
            usb_buf[0x03]       = 0x06;
            usb_buf[0x04]       = 0x01; //type like wheel or rolling
            usb_buf[0x05]       = speed; //speed
            usb_buf[0x06]       = 0xFE;
    }

    hid_write(dev, usb_buf, AIRGOO_LUSTROUS_COMMANDER_PACKET_SIZE);
}

void AirgooLustrousCommanderController::SetFanMode()
{

}

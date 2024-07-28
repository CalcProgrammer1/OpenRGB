/*---------------------------------------------------------*\
| AsusAuraCoreController.cpp                                |
|                                                           |
|   Driver for ASUS ROG Aura Core                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                13 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AsusAuraCoreController.h"
#include "StringUtils.h"

#define AURA_CORE_MAX_MESSAGE_SIZE  64

AuraCoreController::AuraCoreController(hid_device* dev_handle, const char* path)
{
    dev                         = dev_handle;
    location                    = path;

    aura_device.aura_type       = AURA_CORE_DEVICE_UNKNOWN;
    aura_device.buff_size       = 0;
    aura_device.report_id       = 0x5D;
    aura_device.num_leds        = 4;
    aura_device.supports_direct = false;

    IdentifyDevice();
    Handshake();
}

AuraCoreController::~AuraCoreController()
{
    hid_close(dev);
}

std::string AuraCoreController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraCoreController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void AuraCoreController::SendBrightness
    (
    unsigned char   brightness
    )
{
    unsigned char usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];

    if(aura_device.aura_type != AURA_CORE_DEVICE_UNKNOWN)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up message packet                                 |
        \*-----------------------------------------------------*/
        usb_buf[0x00]   = 0x5A;
        usb_buf[0x01]   = AURA_CORE_COMMAND_BRIGHTNESS;
        usb_buf[0x02]   = 0xC5;
        usb_buf[0x03]   = 0xC4;
        usb_buf[0x04]   = brightness;

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_send_feature_report(dev, usb_buf, aura_device.buff_size);
    }
}

void AuraCoreController::SendUpdate
    (
    unsigned char   zone,
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   dir,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    unsigned char usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];

    if(aura_device.aura_type != AURA_CORE_DEVICE_UNKNOWN)
    {
        if(aura_device.aura_type == AURA_CORE_DEVICE_KEYBOARD)
        {
            zone += 1;
        }

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up message packet                                 |
        \*-----------------------------------------------------*/
        usb_buf[0x00]   = aura_device.report_id;
        usb_buf[0x01]   = AURA_CORE_COMMAND_UPDATE;
        usb_buf[0x02]   = zone;
        usb_buf[0x03]   = mode;
        usb_buf[0x04]   = red;
        usb_buf[0x05]   = green;
        usb_buf[0x06]   = blue;
        usb_buf[0x07]   = speed;
        usb_buf[0x08]   = dir;

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_send_feature_report(dev, usb_buf, aura_device.buff_size);
    }
}

void AuraCoreController::SendSet()
{
    unsigned char usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];

    if(aura_device.aura_type != AURA_CORE_DEVICE_UNKNOWN)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up message packet                                 |
        \*-----------------------------------------------------*/
        usb_buf[0x00]   = aura_device.report_id;
        usb_buf[0x01]   = AURA_CORE_COMMAND_SET;

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_send_feature_report(dev, usb_buf, aura_device.buff_size);
    }
}

void AuraCoreController::SendApply()
{
    unsigned char usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];

    if(aura_device.aura_type != AURA_CORE_DEVICE_UNKNOWN)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up message packet                                 |
        \*-----------------------------------------------------*/
        usb_buf[0x00]   = aura_device.report_id;
        usb_buf[0x01]   = AURA_CORE_COMMAND_APPLY;

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_send_feature_report(dev, usb_buf, aura_device.buff_size);
    }
}

void AuraCoreController::InitDirectMode()
{
    unsigned char   usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];
    unsigned char   msg_num   = 0;
    int             led_count = aura_device.num_leds;

    if(aura_device.supports_direct)
    {
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up message packet                                 |
        \*-----------------------------------------------------*/
        usb_buf[0x00]   = aura_device.report_id;
        usb_buf[0x01]   = AURA_CORE_COMMAND_DIRECT;
        usb_buf[0x02]   = 0xD0;

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_send_feature_report(dev, usb_buf, aura_device.buff_size);

        while(led_count > 0)
        {
            /*-----------------------------------------------------*\
            | Set up second message packet                         |
            \*-----------------------------------------------------*/
            usb_buf[0x03]   = 0x01;
            usb_buf[0x04]   = 0x02;
            usb_buf[0x05]   = 0x00;
            usb_buf[0x06]   = msg_num++;
            usb_buf[0x07]   = (led_count > aura_device.max_leds_per_message) ? aura_device.max_leds_per_message : led_count;
            usb_buf[0x08]   = 0x00;

            /*-----------------------------------------------------*\
            | Send packet 2                                         |
            \*-----------------------------------------------------*/
            hid_send_feature_report(dev, usb_buf, aura_device.buff_size);

            led_count -= aura_device.max_leds_per_message;
        }
    }
}

void AuraCoreController::UpdateDirect(std::vector<AuraColor>& color_set)
{
    unsigned char usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];
    unsigned char msg_num       = 0;
    unsigned char color_index   = 0;
    unsigned char set_count     = 0;
    int           led_count     = aura_device.num_leds;

    if(aura_device.supports_direct)
    {
        while(led_count > 0)
        {
            unsigned char msg_index = 0x09;

            /*-----------------------------------------------------*\
            | Zero out buffer                                       |
            \*-----------------------------------------------------*/
            memset(usb_buf, 0x00, sizeof(usb_buf));

            /*-----------------------------------------------------*\
            | Set up message packet                                 |
            \*-----------------------------------------------------*/
            usb_buf[0x00]   = aura_device.report_id;
            usb_buf[0x01]   = AURA_CORE_COMMAND_DIRECT;
            usb_buf[0x02]   = 0xD0;
            usb_buf[0x03]   = 0x01;
            usb_buf[0x04]   = 0x02;
            usb_buf[0x05]   = 0x00;
            usb_buf[0x06]   = msg_num++;
            usb_buf[0x07]   = (led_count > aura_device.max_leds_per_message) ? aura_device.max_leds_per_message : led_count;
            usb_buf[0x08]   = 0x00;

            set_count = 0;

            while( (msg_index < sizeof(usb_buf)                 ) &&
                   (led_count > 0                               ) &&
                   (set_count < aura_device.max_leds_per_message)    )
            {
                if(color_index < color_set.size())
                {
                    usb_buf[msg_index++] = color_set[color_index].red;
                    usb_buf[msg_index++] = color_set[color_index].green;
                    usb_buf[msg_index++] = color_set[color_index].blue;
                }
                else
                {
                    usb_buf[msg_index++] = 0;
                    usb_buf[msg_index++] = 0;
                    usb_buf[msg_index++] = 0;
                }

                led_count--;
                color_index++;
                set_count++;
            }

            /*-----------------------------------------------------*\
            | Send packet                                           |
            \*-----------------------------------------------------*/
            hid_send_feature_report(dev, usb_buf, aura_device.buff_size);
        }
    }
}

void AuraCoreController::IdentifyDevice()
{
    unsigned char   usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];
    int             num_bytes       = 0;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | First, attempt to read the report from the Keyboard   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = aura_device.report_id;

    num_bytes = hid_get_feature_report(dev, usb_buf, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Currently, there is no need to attempt to read the    |
    | returned data.  If the device responded,to the report,|
    | we're good.                                           |
    \*-----------------------------------------------------*/
    if(num_bytes > 0)
    {
        aura_device.aura_type                   = AURA_CORE_DEVICE_KEYBOARD;
        aura_device.buff_size                   = 17;
        aura_device.report_id                   = 0x5D;
        aura_device.num_leds                    = 4;
        aura_device.supports_direct             = false;
    }
    else if(num_bytes == -1)
    {
        /*-------------------------------------------------*\
        | First, attempt to read the report from the        |
        | Keyboard                                          |
        \*-------------------------------------------------*/
        usb_buf[0]                              = 0x5E;
        num_bytes                               = hid_get_feature_report(dev, usb_buf, sizeof(usb_buf));

        if(num_bytes > 0)
        {
            /*---------------------------------------------*\
            | Currently, there is no need to attempt to     |
            | read the returned data.  If the device        |
            | responded,to the report, we're good.          |
            \*---------------------------------------------*/
            aura_device.aura_type               = AURA_CORE_DEVICE_GA15DH;
            aura_device.buff_size               = 64;
            aura_device.report_id               = 0x5E;
            aura_device.num_leds                = 20;
            aura_device.max_leds_per_message    = 16;
            aura_device.supports_direct         = true;
        }
    }
}

void AuraCoreController::Handshake()
{
    unsigned char     usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];


    if(aura_device.aura_type == AURA_CORE_DEVICE_GA15DH)
    {
        usb_buf[0] = 0x5E;
        SendIdString();
        hid_get_feature_report(dev, usb_buf, sizeof(usb_buf));
        SendQuery();
        hid_get_feature_report(dev, usb_buf, sizeof(usb_buf));
    }
}

void AuraCoreController::SendIdString()
{
    unsigned char   usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];
    const char      id[] = "ASUS Tech.Inc.";

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = aura_device.report_id;

    /*-----------------------------------------------------*\
    | Copy in string data                                   |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x01], id, sizeof(id));

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, aura_device.buff_size);
}

void AuraCoreController::SendQuery()
{
    unsigned char usb_buf[AURA_CORE_MAX_MESSAGE_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up message packet                                 |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = aura_device.report_id;
    usb_buf[0x01]   = 0x05;
    usb_buf[0x02]   = 0x20;
    usb_buf[0x03]   = 0x31;
    usb_buf[0x04]   = 0x00;
    usb_buf[0x05]   = 0x10;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, usb_buf, aura_device.buff_size);
}

/*---------------------------------------------------------*\
| AsusAuraUSBController.cpp                                 |
|                                                           |
|   Driver for ASUS Aura USB device                         |
|                                                           |
|   Martin Hartl (inlart)                       25 Apr 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "AsusAuraUSBController.h"
#include "LogManager.h"
#include "StringUtils.h"

AuraUSBController::AuraUSBController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;

    GetFirmwareVersion();
    GetConfigTable();
}

AuraUSBController::~AuraUSBController()
{
    hid_close(dev);
}

unsigned int AuraUSBController::GetChannelCount()
{
    return((unsigned int)device_info.size());
}

std::string AuraUSBController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string AuraUSBController::GetDeviceName()
{
    return(name);
}

std::string AuraUSBController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string AuraUSBController::GetDeviceVersion()
{
    return(std::string(version));
}

const std::vector<AuraDeviceInfo>& AuraUSBController::GetAuraDevices() const
{
    return(device_info);
}

void AuraUSBController::GetConfigTable()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_REQUEST_CONFIG_TABLE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Copy the firmware string if the reply ID is correct   |
    \*-----------------------------------------------------*/
    if(usb_buf[1] == 0x30)
    {
        memcpy(config_table, &usb_buf[4], 60);

        LOG_DEBUG("[%s] ASUS Aura USB config table:", version);

        for(int i = 0; i < 60; i+=6)
        {
            LOG_DEBUG("[%s] %02X %02X %02X %02X %02X %02X", version,
                                                            config_table[i + 0],
                                                            config_table[i + 1],
                                                            config_table[i + 2],
                                                            config_table[i + 3],
                                                            config_table[i + 4],
                                                            config_table[i + 5]);
        }
    }
    else
    {
        LOG_INFO("[%s] Could not read config table, can not add device", version);
        delete this;
    }
}

void AuraUSBController::GetFirmwareVersion()
{
    unsigned char usb_buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up firmware version request packet                |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = 0xEC;
    usb_buf[0x01]   = AURA_REQUEST_FIRMWARE_VERSION;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read(dev, usb_buf, 65);

    /*-----------------------------------------------------*\
    | Copy the firmware string if the reply ID is correct   |
    \*-----------------------------------------------------*/
    if(usb_buf[1] == 0x02)
    {
        memcpy(version, &usb_buf[2], 16);
    }
}

void AuraUSBController::SendDirect
    (
    unsigned char   device,
    unsigned char   led_count,
    RGBColor*  colors
    )
{
    unsigned char usb_buf[65];
    unsigned char offset         =      0x00;
    unsigned char sent_led_count =      LEDS_PER_PACKET;
    bool apply                   =      false;
    while(!apply)
    {
        if(offset + sent_led_count > led_count)
        {
            sent_led_count = led_count - offset;
        }


        if(offset + sent_led_count == led_count)
        {
            apply = true;
        }
        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, sizeof(usb_buf));

        /*-----------------------------------------------------*\
        | Set up message packet                                 |
        \*-----------------------------------------------------*/
        usb_buf[0x00]   = 0xEC;
        usb_buf[0x01]   = AURA_CONTROL_MODE_DIRECT;
        usb_buf[0x02]   = (apply ? 0x80 : 0x00) | device;
        usb_buf[0x03]   = offset;
        usb_buf[0x04]   = sent_led_count;

        /*-----------------------------------------------------*\
        | Copy in color data bytes                              |
        \*-----------------------------------------------------*/
        for(unsigned char led_idx = 0; led_idx < sent_led_count; led_idx++)
        {

            usb_buf[0x05 + (led_idx * 3)] = RGBGetRValue(colors[offset + led_idx]);
            usb_buf[0x06 + (led_idx * 3)] = RGBGetGValue(colors[offset + led_idx]);
            usb_buf[0x07 + (led_idx * 3)] = RGBGetBValue(colors[offset + led_idx]);
        }

        /*-----------------------------------------------------*\
        | Send packet                                           |
        \*-----------------------------------------------------*/
        hid_write(dev, usb_buf, 65);

        offset += sent_led_count;
    }
}

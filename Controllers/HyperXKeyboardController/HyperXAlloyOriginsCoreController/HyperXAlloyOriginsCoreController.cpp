/*---------------------------------------------------------*\
| HyperXAlloyOriginsCoreController.cpp                      |
|                                                           |
|   Driver for HyperX Alloy Origins Core keyboard           |
|                                                           |
|   Volodymyr Nazarchuk (Vavooon)               28 Apr 2021 |
|   Mike White (kamaaina)                       09 Jun 2021 |
|   carlos jordao                               15 Mar 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXAlloyOriginsCoreController.h"
#include "StringUtils.h"
#include "LogManager.h"


HyperXAlloyOriginsCoreController::HyperXAlloyOriginsCoreController(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name)
{
    dev             = dev_handle;
    location        = dev_info->path;
    name            = dev_name;

    /*-----------------------------------------------------*\
    | Get the firmware version from the device info         |
    \*-----------------------------------------------------*/
    char fw_version_buf[8];
    memset(fw_version_buf, '\0', sizeof(fw_version_buf));

    unsigned short version = dev_info->release_number;
    snprintf(fw_version_buf, 8, "%.2X.%.2X", (version & 0xFF00) >> 8, version & 0x00FF);

    firmware_version = fw_version_buf;
}

HyperXAlloyOriginsCoreController::~HyperXAlloyOriginsCoreController()
{
    hid_close(dev);
}

std::string HyperXAlloyOriginsCoreController::GetDeviceLocation()
{
    return("HID " + location);
}

std::string HyperXAlloyOriginsCoreController::GetNameString()
{
    return(name);
}

std::string HyperXAlloyOriginsCoreController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string HyperXAlloyOriginsCoreController::GetFirmwareVersion()
{
    return(firmware_version);
}

unsigned int HyperXAlloyOriginsCoreController::GetVariant()
{
    unsigned char packet[65];
    unsigned int variant = 0;
    int actual = 0;

    memset(packet, 0x00, sizeof(packet));

    /*---------------------------------------*\
    | Command 10 asks some data from keyboard |
    | The answer looks like:                  |
    | * command answer header (bytes 0-4)     |
    |   * data length: byte 4                 |
    | * version               (bytes 5-6)     |
    | * Product string        (bytes 9-33)    |
    | * Layout variant        (byte  56)      |
    \*---------------------------------------*/
    packet[1] = 0x10;
    hid_write(dev, packet, 65);
    memset(packet, 0x00, sizeof(packet));
    actual = hid_read(dev, packet, 65);

    if(actual > 0)
        variant = packet[56];
    else
        variant = 0x09;

    LOG_DEBUG("[HyperX Alloy Origins Core] variant: 0x%02X", variant);
    return variant;
}


void HyperXAlloyOriginsCoreController::SetBrightness(unsigned int brightness)
{
    unsigned char packet[65];
    memset(packet, 0x00, sizeof(packet));

    packet[1] = 0xA7;
    packet[4] = 0x01;
    packet[5] = brightness;

    hid_write(dev, packet, 65);
}


void HyperXAlloyOriginsCoreController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    /*------------------------------------------------------------------------------*\
    | * Always send 380 bytes to the keyboard and a total of 94 led indexes.         |
    | The colors are grouped into segments of 48 bytes.                              |
    | Each one is divided into:                                                      |
    |    6 Green + 2 zeroes +  6 Green + 2 zeroes +                                  |
    |    6 Red   + 2 zeroes +  6 Red   + 2 zeroes +                                  |
    |    6 Blue  + 2 zeroes +  6 Blue  + 2 zeroes                                    |
    |    \=---> sector 0       \=--> sector 1                                        |
    | Every 6 colors form a sector. The names are arbitrary, just to make clear how  |
    | to set the colors into the buffer.                                             |
    | So each segment has 2 sectors and 12 colors.                                   |
    | The last 10 colors don't fill completely the last segment.                     |
    | * All 94 colors can be sent even if some of them aren't used by the physical   |
    | keyboard. This allows to lit every key, even if not mapped directly.           |
    \*------------------------------------------------------------------------------*/
    unsigned int segment = 0, sector = 0, sequence = 0;
    unsigned int total_colors = 0;
    unsigned char buf[380];
    memset(buf, 0x00, sizeof(buf));

    /*---------------------------------------------------------------------------*\
    | transfer the colors to the buffer. Max 94 colors to avoid buffer overflow.  |
    \*---------------------------------------------------------------------------*/
    if(colors.size() > 94)
    {
        total_colors = 94;
    }
    else
    {
        total_colors = (unsigned int)colors.size();
    }

    for(unsigned int color_idx = 0; color_idx < total_colors; color_idx++)
    {
        unsigned int pos = 0;
        segment  = (color_idx / 12) * 48;
        sector   = ((color_idx / 6) & 1) * 8;
        sequence = color_idx % 6;

        pos = segment + sector + sequence;

        buf[pos     ] = RGBGetGValue(colors[color_idx]);
        buf[pos + 16] = RGBGetRValue(colors[color_idx]);
        buf[pos + 32] = RGBGetBValue(colors[color_idx]);
    }

    unsigned int sentBytes   = 0;
    unsigned int bytesToSend = sizeof(buf);
    unsigned int payloadSize = 60;
    unsigned int seq         = 0;

    while(sentBytes < bytesToSend)
    {
        if (bytesToSend - sentBytes < payloadSize)
        {
            payloadSize = bytesToSend - sentBytes;
        }

        unsigned char packet[65];
        memset(packet, 0x00, sizeof(packet));

        packet[1] = 0xA2;
        packet[2] = seq++;
        packet[4] = payloadSize;

        memcpy(&packet[5], &buf[sentBytes], payloadSize);
        hid_write(dev, packet, payloadSize + 5);

        sentBytes += payloadSize;
    }
}

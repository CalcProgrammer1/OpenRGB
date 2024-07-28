/*---------------------------------------------------------*\
| HyperXAlloyOriginsCoreController.cpp                      |
|                                                           |
|   Driver for HyperX Alloy Origins Core keyboard           |
|                                                           |
|   Volodymyr Nazarchuk (Vavooon)               28 Apr 2021 |
|   Mike White (kamaaina)                       09 Jun 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "HyperXAlloyOriginsCoreController.h"
#include "StringUtils.h"

// Skip these indices in the color output
static unsigned int skip_idx[] = {6, 7, 14, 15, 22, 23, 30, 31, 38, 39, 44, 46, 47, 54, 55, 58, 60, 61, 62, 63, 70, 71, 78, 79, 86, 87, 94, 95, 101, 102, 103, 109, 110, 111, 118, 119};

HyperXAlloyOriginsCoreController::HyperXAlloyOriginsCoreController(hid_device* dev_handle, hid_device_info* dev_info)
{
    dev              = dev_handle;
    location         = dev_info->path;

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
    for(unsigned int skip_cnt = 0; skip_cnt < (sizeof(skip_idx) / sizeof(skip_idx[0])); skip_cnt++)
    {
        colors.insert(colors.begin() + skip_idx[skip_cnt], 0x00000000);
    }

    unsigned char buf[380];
    memset(buf, 0x00, sizeof(buf));

    int offset = 0;
    int rowPos = 0;

    for(unsigned int color_idx = 0; color_idx < colors.size(); color_idx++)
    {
        if (color_idx > 0 && color_idx % 16 == 0)
        {
            offset += 48;
            rowPos = 0;
        }

        buf[rowPos + offset]      = RGBGetGValue(colors[color_idx]);
        buf[rowPos + offset + 16] = RGBGetRValue(colors[color_idx]);
        buf[rowPos + offset + 32] = RGBGetBValue(colors[color_idx]);

        rowPos++;
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
        hid_write(dev, packet, 65);

        sentBytes += payloadSize;
    }
}

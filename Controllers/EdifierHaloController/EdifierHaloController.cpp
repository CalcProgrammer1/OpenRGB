/*---------------------------------------------------------*\
| EdifierHaloController.cpp                                 |
|                                                           |
|   Driver for Edifier Halo series USB/HID devices          |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  20 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "EdifierHaloController.h"
#include "StringUtils.h"

EdifierHaloController::EdifierHaloController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    wchar_t name_string[256];
    int result;

    result = hid_get_manufacturer_string(dev, name_string, 256);
    if(result == 0)
    {
        device_name = StringUtils::wstring_to_string(name_string);
    }
    else
    {
        device_name = "Edifier";
        LOG_WARNING("[EdifierHaloController] Failed to read manufacturer string for %s", path);
    }

    result = hid_get_product_string(dev, name_string, 256);
    if(result == 0)
    {
        device_name.append(" ").append(StringUtils::wstring_to_string(name_string));
    }
    else
    {
        device_name.append(" Halo PixelBar");
        LOG_WARNING("[EdifierHaloController] Failed to read product string for %s", path);
    }

    wchar_t serial_string[128];
    result = hid_get_serial_number_string(dev, serial_string, 128);
    if(result == 0)
    {
        serial = StringUtils::wstring_to_string(serial_string);
    }
    else
    {
        serial = "";
        LOG_DEBUG("[EdifierHaloController] No serial number available for %s", path);
    }
}

EdifierHaloController::~EdifierHaloController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string EdifierHaloController::GetDeviceName() const
{
    return(device_name);
}

std::string EdifierHaloController::GetSerial() const
{
    return(serial);
}

std::string EdifierHaloController::GetLocation() const
{
    return("HID: " + location);
}

void EdifierHaloController::SetAmbientLightPower(bool enabled)
{
    unsigned char mode = enabled ? 0x01 : 0x00;
    unsigned char payload[] =
    {
        0x00,
        0x00,
        0x00,
        0x00,
        mode,
        0xFF,
        0xFF
    };
    SendEdifierPacket(0x6b, payload, sizeof(payload));
}

void EdifierHaloController::SetAmbientLight(unsigned char effect, unsigned char r, unsigned char g, unsigned char b, unsigned char brightness, unsigned char speed)
{
    unsigned char raw_brightness = 0x3c;
    if (brightness == 1) raw_brightness = 0x14;
    else if (brightness == 2) raw_brightness = 0x28;
    else if (brightness == 3) raw_brightness = 0x3c;

    if (speed < 1) speed = 1;
    if (speed > 10) speed = 10;

    unsigned char payload[] =
    {
        0x13,
        effect,
        r,
        g,
        b,
        raw_brightness,
        speed
    };
    SendEdifierPacket(0x6b, payload, sizeof(payload));
}

void EdifierHaloController::SetPixelScreenColor(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char payload[] =
    {
        0x03,
        r,
        g,
        b
    };
    SendEdifierPacket(0xef, payload, sizeof(payload));
}

void EdifierHaloController::SetScreenUIModel(unsigned char model_id, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char payload[] =
    {
        0x02,
        r,
        g,
        b,
        0x00,
        0x01,
        model_id,
        0xff,
        0xff
    };
    SendEdifierPacket(0xef, payload, sizeof(payload));
}

void EdifierHaloController::SetScreenScene(unsigned char category, unsigned char index, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char payload[] =
    {
        0x01, // group
        r,
        g,
        b,
        0x00,
        0x01,
        category,
        index,
        0xff // option
    };
    SendEdifierPacket(0xef, payload, sizeof(payload));
}

void EdifierHaloController::SendEdifierPacket(unsigned char commandIndex, const unsigned char* payload, unsigned int payload_size)
{
    if(!dev)
    {
        return;
    }

    unsigned char packet[64];
    memset(packet, 0x00, sizeof(packet));

    packet[0] = 0x2e;
    packet[1] = 0xaa;
    packet[2] = 0xec;
    packet[3] = commandIndex;
    packet[4] = (unsigned char)((payload_size >> 8) & 0xff);
    packet[5] = (unsigned char)(payload_size & 0xff);

    if(payload_size > 0 && payload != nullptr)
    {
        memcpy(&packet[6], payload, payload_size);
    }

    // Calculate checksum
    unsigned int sum = 0;
    for(unsigned int i = 1; i < 6 + payload_size; i++)
    {
        sum += packet[i];
    }
    packet[6 + payload_size] = (unsigned char)(sum & 0xff);

    std::lock_guard<std::mutex> guard(device_mutex);
    int bytes_written = hid_write(dev, packet, sizeof(packet));
    if(bytes_written != sizeof(packet))
    {
        LOG_ERROR("[EdifierHaloController] Failed to write HID packet (%d/%d bytes): %ls",
                  bytes_written, (int)sizeof(packet), hid_error(dev));
    }
}

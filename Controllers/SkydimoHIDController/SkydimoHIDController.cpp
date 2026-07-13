/*---------------------------------------------------------*\
| SkydimoHIDController.cpp                                  |
|                                                           |
|   Driver for Skydimo HID devices                          |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  09 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "SkydimoHIDController.h"
#include "StringUtils.h"

static unsigned char CalculateCRC8(const unsigned char* data, unsigned int length)
{
    unsigned char crc = 0x00;

    for(unsigned int byte_idx = 0; byte_idx < length; byte_idx++)
    {
        crc ^= data[byte_idx];

        for(unsigned int bit_idx = 0; bit_idx < SKYDIMO_HID_CRC_BIT_COUNT; bit_idx++)
        {
            if(crc & SKYDIMO_HID_CRC_HIGH_BIT)
            {
                crc = (unsigned char)((crc << 1) ^ SKYDIMO_HID_CRC_POLYNOMIAL);
            }
            else
            {
                crc = (unsigned char)(crc << 1);
            }
        }
    }

    return(crc);
}

SkydimoHIDController::SkydimoHIDController(hid_device* dev_handle, const char* path)
{
    dev         = dev_handle;
    location    = path;

    wchar_t name_string[SKYDIMO_HID_NAME_BUFFER_SIZE];
    int result;

    result = hid_get_manufacturer_string(dev, name_string, SKYDIMO_HID_NAME_BUFFER_SIZE);
    if(result == 0)
    {
        device_name = StringUtils::wstring_to_string(name_string);
    }
    else
    {
        device_name = "Skydimo";
        LOG_WARNING("[SkydimoHIDController] Failed to read manufacturer string for %s", path);
    }

    result = hid_get_product_string(dev, name_string, SKYDIMO_HID_NAME_BUFFER_SIZE);
    if(result == 0)
    {
        device_name.append(" ").append(StringUtils::wstring_to_string(name_string));
    }
    else
    {
        device_name.append(" HID Device");
        LOG_WARNING("[SkydimoHIDController] Failed to read product string for %s", path);
    }

    wchar_t serial_string[SKYDIMO_HID_SERIAL_BUFFER_SIZE];
    result = hid_get_serial_number_string(dev, serial_string, SKYDIMO_HID_SERIAL_BUFFER_SIZE);
    if(result == 0)
    {
        serial = StringUtils::wstring_to_string(serial_string);
    }
    else
    {
        serial = "";
        LOG_DEBUG("[SkydimoHIDController] No serial number available for %s", path);
    }
}

SkydimoHIDController::~SkydimoHIDController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string SkydimoHIDController::GetDeviceName() const
{
    return(device_name);
}

std::string SkydimoHIDController::GetSerial() const
{
    return(serial);
}

std::string SkydimoHIDController::GetLocation() const
{
    return("HID: " + location);
}

bool SkydimoHIDController::SendPacket(unsigned char start_led, const unsigned char* chunk, unsigned int chunk_length)
{
    unsigned char packet[SKYDIMO_HID_PACKET_SIZE];
    memset(packet, 0x00, sizeof(packet));

    packet[0]                                  = SKYDIMO_HID_REPORT_ID;
    packet[SKYDIMO_HID_PACKET_START_LED_INDEX] = start_led;
    packet[SKYDIMO_HID_PACKET_RESERVED_INDEX]  = 0x00;

    if(chunk_length > SKYDIMO_HID_PACKET_DATA_SIZE)
    {
        chunk_length = SKYDIMO_HID_PACKET_DATA_SIZE;
    }

    memcpy(&packet[SKYDIMO_HID_PACKET_DATA_OFFSET], chunk, chunk_length);

    /*-----------------------------------------------------*\
    | Calculate CRC8 over all bytes before the checksum     |
    \*-----------------------------------------------------*/
    packet[SKYDIMO_HID_PACKET_SIZE - 1] = CalculateCRC8(packet, SKYDIMO_HID_PACKET_SIZE - 1);

    int bytes_written = hid_write(dev, packet, SKYDIMO_HID_PACKET_SIZE);

    if(bytes_written != SKYDIMO_HID_PACKET_SIZE)
    {
        LOG_ERROR("[SkydimoHIDController] Failed to write RGB packet at LED %u (%d/%u bytes): %ls",
                  (unsigned int)start_led, bytes_written, (unsigned int)SKYDIMO_HID_PACKET_SIZE, hid_error(dev));
        return(false);
    }

    return(true);
}

void SkydimoHIDController::SetLEDs(const std::vector<RGBColor>& colors)
{
    std::lock_guard<std::mutex> guard(device_mutex);

    unsigned char frame_data[SKYDIMO_HID_FRAME_SIZE];
    memset(frame_data, 0, sizeof(frame_data));

    /*-----------------------------------------------------*\
    | Convert colors to GRB format                          |
    \*-----------------------------------------------------*/
    for(unsigned int led_idx = 0; (led_idx < colors.size()) && (led_idx < SKYDIMO_HID_LED_COUNT); led_idx++)
    {
        unsigned int color_offset = led_idx * SKYDIMO_HID_BYTES_PER_LED;

        frame_data[color_offset + 0] = RGBGetGValue(colors[led_idx]);
        frame_data[color_offset + 1] = RGBGetRValue(colors[led_idx]);
        frame_data[color_offset + 2] = RGBGetBValue(colors[led_idx]);
    }

    /*-----------------------------------------------------*\
    | Send the frame in packet-sized chunks                 |
    \*-----------------------------------------------------*/
    for(unsigned int frame_offset = 0; frame_offset < SKYDIMO_HID_FRAME_SIZE; frame_offset += SKYDIMO_HID_PACKET_DATA_SIZE)
    {
        unsigned int bytes_remaining = SKYDIMO_HID_FRAME_SIZE - frame_offset;
        unsigned int chunk_length    = (bytes_remaining < SKYDIMO_HID_PACKET_DATA_SIZE)
                                     ? bytes_remaining
                                     : SKYDIMO_HID_PACKET_DATA_SIZE;
        unsigned char start_led      = (unsigned char)(frame_offset / SKYDIMO_HID_BYTES_PER_LED);

        if(!SendPacket(start_led, &frame_data[frame_offset], chunk_length))
        {
            return;
        }
    }

    /*-----------------------------------------------------*\
    | Send the frame-end packet                             |
    \*-----------------------------------------------------*/
    unsigned char end_packet[SKYDIMO_HID_PACKET_SIZE];
    memset(end_packet, 0x00, sizeof(end_packet));

    end_packet[0]                                 = SKYDIMO_HID_REPORT_ID;
    end_packet[SKYDIMO_HID_END_COMMAND_INDEX]     = SKYDIMO_HID_END_COMMAND_VALUE;
    end_packet[SKYDIMO_HID_END_SUBCOMMAND_INDEX]  = SKYDIMO_HID_END_SUBCOMMAND_VALUE;
    end_packet[SKYDIMO_HID_END_LED_COUNT_INDEX]   = SKYDIMO_HID_LED_COUNT;
    end_packet[SKYDIMO_HID_END_MARKER_INDEX]      = SKYDIMO_HID_END_MARKER_VALUE;

    int bytes_written = hid_write(dev, end_packet, SKYDIMO_HID_PACKET_SIZE);

    if(bytes_written != SKYDIMO_HID_PACKET_SIZE)
    {
        LOG_ERROR("[SkydimoHIDController] Failed to write frame-end packet (%d/%u bytes): %ls",
                  bytes_written, (unsigned int)SKYDIMO_HID_PACKET_SIZE, hid_error(dev));
    }
}

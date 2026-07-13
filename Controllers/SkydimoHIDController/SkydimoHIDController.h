/*---------------------------------------------------------*\
| SkydimoHIDController.h                                    |
|                                                           |
|   Driver for Skydimo HID devices                          |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  09 Jul 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <mutex>
#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBControllerInterface.h"

#define SKYDIMO_HID_LED_COUNT               49
#define SKYDIMO_HID_MATRIX_SIZE             7
#define SKYDIMO_HID_BYTES_PER_LED           3
#define SKYDIMO_HID_FRAME_SIZE              (SKYDIMO_HID_LED_COUNT * SKYDIMO_HID_BYTES_PER_LED)
#define SKYDIMO_HID_PACKET_SIZE             64
#define SKYDIMO_HID_PACKET_DATA_SIZE        60
#define SKYDIMO_HID_PACKET_DATA_OFFSET      3
#define SKYDIMO_HID_REPORT_ID               0x01
#define SKYDIMO_HID_PACKET_START_LED_INDEX  1
#define SKYDIMO_HID_PACKET_RESERVED_INDEX   2
#define SKYDIMO_HID_MATRIX_NA               0xFFFFFFFF
#define SKYDIMO_HID_CRC_POLYNOMIAL          0x07
#define SKYDIMO_HID_CRC_HIGH_BIT            0x80
#define SKYDIMO_HID_CRC_BIT_COUNT           8
#define SKYDIMO_HID_NAME_BUFFER_SIZE        255
#define SKYDIMO_HID_SERIAL_BUFFER_SIZE      128
#define SKYDIMO_HID_END_MARKER_INDEX        60
#define SKYDIMO_HID_END_MARKER_VALUE        0x1E
#define SKYDIMO_HID_END_COMMAND_INDEX       1
#define SKYDIMO_HID_END_COMMAND_VALUE       0xFF
#define SKYDIMO_HID_END_SUBCOMMAND_INDEX    2
#define SKYDIMO_HID_END_SUBCOMMAND_VALUE    0xFF
#define SKYDIMO_HID_END_LED_COUNT_INDEX     3

class SkydimoHIDController
{
public:
    SkydimoHIDController(hid_device* dev_handle, const char* path);
    ~SkydimoHIDController();

    std::string             GetDeviceName() const;
    std::string             GetSerial() const;
    std::string             GetLocation() const;

    void                    SetLEDs(const std::vector<RGBColor>& colors);

private:
    hid_device*             dev;
    std::string             device_name;
    std::string             serial;
    std::string             location;
    std::mutex              device_mutex;

    bool                    SendPacket(unsigned char start_led, const unsigned char* chunk, unsigned int chunk_length);
};

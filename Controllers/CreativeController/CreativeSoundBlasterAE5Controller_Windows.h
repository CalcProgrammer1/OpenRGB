/*---------------------------------------------------------*\
| CreativeSoundBlasterAE5Controller_Windows.h               |
|                                                           |
|   Driver for Creative SoundBlaster AE-5 (Windows)         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <windows.h>
#include <setupapi.h>
#include "RGBController.h"
#include "CreativeSoundBlasterAE5ControllerBase.h"

// AE-5 LED command structure
#pragma pack(push, 1)
struct AE5_LED_Command
{
    BYTE command;           // 0x03 for internal, 0x02 for external
    BYTE padding1[11];      // 11 zero bytes
    BYTE packet_led_count;  // Number of LEDs in this packet
    BYTE padding2[3];       // 3 zero bytes
    BYTE data_length_low;   // Low byte of data length (LEDs × 4)
    BYTE data_length_high;  // High byte of data length
    BYTE padding3[2];       // 2 zero bytes
    BYTE led_data[400];     // LED data (RGBA), max 100 LEDs × 4 bytes
    BYTE padding4[624];     // Rest filled with zeros (1044 - 420 bytes used)
};
#pragma pack(pop)

class CreativeSoundBlasterAE5Controller_Windows : public CreativeSoundBlasterAE5ControllerBase
{
public:
    CreativeSoundBlasterAE5Controller_Windows();
    ~CreativeSoundBlasterAE5Controller_Windows();

    bool                    Initialize();
    std::string             GetDeviceLocation();
    std::string             GetDeviceName();
    unsigned int            GetLEDCount();
    unsigned int            GetExternalLEDCount();
    void                    SetExternalLEDCount(unsigned int count);

    void                    SetLEDColors(unsigned char led_count, unsigned char* red_values,
                                        unsigned char* green_values, unsigned char* blue_values);

private:
    bool                    FindDevice();
    std::string             FindHDAudioDevicePath();
    bool                    OpenDevice();
    void                    CloseDevice();
    bool                    SendLEDCommand(BYTE command_byte, unsigned int led_count_to_set,
                                          unsigned char* red_values, unsigned char* green_values, unsigned char* blue_values);


    std::string             location;
    std::string             name;
    std::string             hdaudio_device_path;
    bool                    device_found;

    // Device handle for HDAudio communication
    HANDLE                  device_handle;
    bool                    device_opened;

    // Synchronization to prevent conflicts with Creative software
    HANDLE                  led_mutex;

    unsigned int external_led_count;

#define AE5_VENDOR_ID               0x1102
#define AE5_DEVICE_ID               0x0012
#define AE5_INTERNAL_LED_COUNT      5
#define AE5_EXTERNAL_LED_COUNT_MAX  100
};

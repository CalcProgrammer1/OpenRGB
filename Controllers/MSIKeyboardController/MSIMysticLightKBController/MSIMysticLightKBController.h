/*---------------------------------------------------------*\
| MSIMysticLight1565Controller.h                            |
|                                                           |
|   Driver for MSI Mystic Light MS-1565 keyboard leds       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifndef MSIMYSTICLIGHTKBCONTROLLER_H
#define MSIMYSTICLIGHTKBCONTROLLER_H

#include <cstring>
#include <string>
#include <hidapi.h>
#include <vector>

struct Color
{
    unsigned char R;
    unsigned char G;
    unsigned char B;
};

struct ColorKeyFrame
{
    unsigned char time_frame = 0x00;
    Color         color;
};

#define MAX_MS_1565_KEYFRAMES 10

typedef unsigned char MS_1565_SPEED;

// 64 bytes long feature
struct FeaturePacket_MS1565
{
    unsigned char       report_id                               = 0x02; // Report ID
    unsigned char       packet_id                               = 0x02;
    unsigned char       mode                                    = 0x00;
    unsigned char       speed2                                  = 0x00; // Seconds X 100 = duration of animation cycle
    unsigned char       speed1                                  = 0x00; // In little endian
                                                                        // 1 second => 100 = 0x0064, speed2 = 0x64, speed1 = 0x00
    const unsigned char unused                                  = 0x00;
    const unsigned char unused2                                 = 0x00;
    const unsigned char unused3                                 = 0x0F;
    const unsigned char unused4                                 = 0x01;
    unsigned char       wave_dir                                = 0x00;
    ColorKeyFrame       color_keyframes[MAX_MS_1565_KEYFRAMES]  = {};
    const unsigned char padding[14]                             = {}; //pad to make the packet size 64 bytes
};

enum MS_1565_MODE
{
    MS_1565_OFF             = 0,
    MS_1565_STEADY          = 1,
    MS_1565_BREATHING       = 2,
    MS_1565_CYCLE           = 3,
    MS_1565_WAVE            = 4,
};

enum MS_1565_WAVE_DIRECTION
{
    MS_1565_WAVE_DIRECTION_RIGHT_TO_LEFT = 0,
    MS_1565_WAVE_DIRECTION_LEFT_TO_RIGHT = 1
};

enum MS_1565_ZONE
{
    MS_1565_ZONE_1 = 1,
    MS_1565_ZONE_2,
    MS_1565_ZONE_3,
    MS_1565_ZONE_4,
    MS_1565_ZONE_DEVICE
};

class MSIKeyboardController
{
public:
    MSIKeyboardController
        (
        hid_device* handle,
        const char* path
        );
    ~MSIKeyboardController();

    void SetMode
    (
        MS_1565_MODE           mode,
        MS_1565_SPEED          speed1,
        MS_1565_SPEED          speed2,
        MS_1565_WAVE_DIRECTION wave_dir,
        MS_1565_ZONE           zone,
        ColorKeyFrame          color_keyframes[]
    );


    std::string GetDeviceName();
    std::string GetDeviceLocation();
    std::string GetFWVersion();
    std::string GetSerial();

    std::vector<MS_1565_ZONE> mode_zones;

private:
    hid_device* dev;
    std::string location;
};


#endif // MSIMYSTICLIGHTKBCONTROLLER_H

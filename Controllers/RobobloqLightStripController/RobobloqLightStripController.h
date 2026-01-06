/*---------------------------------------------------------*\
| RobobloqLightStripController.h                            |
|                                                           |
|   Detector for Robobloq Monitor Light Strips              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

/*-----------------------------------------*\
| Lighting modes                            |
\*-----------------------------------------*/
enum
{
    /*-------------------------------------*\
    | Dynamic                               |
    \*-------------------------------------*/
    ROBOBLOQ_MODE_DYNAMIC_RAINBOW   = 0x00,
    ROBOBLOQ_MODE_DYNAMIC_BREATHING = 0x01,
    ROBOBLOQ_MODE_DYNAMIC_TWIST     = 0x02,
    ROBOBLOQ_MODE_DYNAMIC_BEAT      = 0x03,
    ROBOBLOQ_MODE_DYNAMIC_TWIRL     = 0x04,
    ROBOBLOQ_MODE_DYNAMIC_LEMON     = 0x05,
    ROBOBLOQ_MODE_DYNAMIC_ELECTRIC  = 0x06,
    /*-------------------------------------*\
    | Rhythm (synced to music)              |
    \*-------------------------------------*/
    ROBOBLOQ_MODE_RHYTHM_CIRCLES    = 0x07,
    ROBOBLOQ_MODE_RHYTHM_TWIRL      = 0x08,
    ROBOBLOQ_MODE_RHYTHM_SPARKLE    = 0x09,
    ROBOBLOQ_MODE_RHYTHM_BUBBLES    = 0x10,
    ROBOBLOQ_MODE_RHYTHM_SPOTLIGHT  = 0x11,
    ROBOBLOQ_MODE_RHYTHM_RAINBOW    = 0x12,
    ROBOBLOQ_MODE_RHYTHM_BLAST      = 0x13,
    /*-------------------------------------*\
    | Virtual modes                         |
    \*-------------------------------------*/
    ROBOBLOQ_MODE_OFF               = 0xFFFD,
    ROBOBLOQ_MODE_STATIC            = 0xFFFE,
    ROBOBLOQ_MODE_DIRECT            = 0xFFFF,
};

#define ROBOBLOQ_IS_DYNAMIC_EFFECT(x) ((x) >= ROBOBLOQ_MODE_DYNAMIC_RAINBOW && (x) <= ROBOBLOQ_MODE_DYNAMIC_ELECTRIC)
#define ROBOBLOQ_IS_RHYTHM_EFFECT(x) ((x) >= ROBOBLOQ_MODE_RHYTHM_CIRCLES && (x) <= ROBOBLOQ_MODE_RHYTHM_BLAST)

/*-----------------------------------------*\
| Commands                                  |
\*-----------------------------------------*/
enum
{
    ROBOBLOQ_CMD_SET_SYNC_SCREEN    = 0x80,
    ROBOBLOQ_CMD_READ_DEVICE_INFO   = 0x82,
    ROBOBLOQ_CMD_SET_EFFECT         = 0x85,
    ROBOBLOQ_CMD_SET_COLOR          = 0x86,
    ROBOBLOQ_CMD_SET_BRIGHTNESS     = 0x87,
    ROBOBLOQ_CMD_SET_DYNAMIC_SPEED  = 0x8A,
    ROBOBLOQ_CMD_SET_OPEN_URL       = 0x93,
    ROBOBLOQ_CMD_TURN_OFF           = 0x97,
};

/*-----------------------------------------*\
| Effect categories                         |
\*-----------------------------------------*/
enum
{
    ROBOBLOQ_EFFECT_DYNAMIC         = 0x02,
    ROBOBLOQ_EFFECT_RHYTHM          = 0x03,
};

#define ROBOBLOQ_DYNAMIC_SPEED_MAX    0x64

/*---------------------------------------------------------*\
| Number of (start, end, R, G, B) tuples that will be sent  |
| in a SetCustom call                                       |
\*---------------------------------------------------------*/
#define ROBOBLOQ_TUPLE_COUNT          34

class RobobloqLightStripController
{
public:
    RobobloqLightStripController(hid_device* dev_handle, const char* path, std::string dev_name);
    ~RobobloqLightStripController();

    std::string GetDeviceLocation();
    std::string GetDeviceName();
    std::string GetSerialString();
    std::string GetFirmwareVersion();
    int         GetLEDCount();
    int         GetLEDsPerSide();
    int         GetPhysicalSizeInInches();

    void        Initialize();
    void        SetColor(RGBColor c);
    void        SetLEDColor(int led, RGBColor c);
    void        SetColorRanges(const std::vector<unsigned char>& ranges);
    void        SetBrightness(unsigned char brightness);
    void        SetDynamicEffect(unsigned char effect);
    void        SetCustom(const std::vector<RGBColor>& colors);
    void        SetDynamicSpeed(unsigned char speed);
    void        TurnOff();


private:
    hid_device*             dev;
    std::string             name;
    std::string             location;
    std::string             id;
    std::string             uuid;
    std::string             firmware_version;
    unsigned char           led_count;
    unsigned char           physical_size;
    unsigned char           packet_index;

    void        SendPacket(const std::vector<unsigned char>& command, bool flush = true);
    std::vector<unsigned char> SendPacketWithReply(const std::vector<unsigned char>& command);
    void        SendMultiPacket(unsigned char command, const std::vector<unsigned char>& payload);
    bool        RequestDeviceInfo();
    void        SendSyncScreen(const std::vector<unsigned char>& color_bytes);
    void        IncPacketIndex();
    void        WriteReport(const unsigned char* data);
};

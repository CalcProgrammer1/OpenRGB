/*-----------------------------------------*\
|  RedragonMouseController.h                |
|                                           |
|  Definitions and types for Redragon Mouse |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 3/15/2020   |
\*-----------------------------------------*/
#pragma once

#include <string>
#include <hidapi/hidapi.h>

#define REDRAGON_MOUSE_REPORT_ID   0x02
#define REDRAGON_MOUSE_REPORT_SIZE 16
#define REDRAGON_MOUSE_LED_COUNT   1

enum
{
    REDRAGON_MOUSE_MODE_WAVE              = 0x00,
    REDRAGON_MOUSE_MODE_RANDOM_BREATHING  = 0x01,
    REDRAGON_MOUSE_MODE_STATIC            = 0x02,
    REDRAGON_MOUSE_MODE_BREATHING         = 0x04,
    REDRAGON_MOUSE_MODE_RAINBOW           = 0x08,
    REDRAGON_MOUSE_MODE_FLASHING          = 0x10
};

class RedragonMouseController
{
public:
    RedragonMouseController(hid_device* dev_handle, const char* path);
    ~RedragonMouseController();

    std::string GetDeviceLocation();
    std::string GetSerialString();

    void        SendMouseApply();

    void        SendMouseColor
                    (
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

    void        SendMouseMode
                    (
                    unsigned char       mode,
                    unsigned char       speed
                    );

    void        SendMouseMode
                    (
                    unsigned char       mode,
                    unsigned char       speed,
                    unsigned char       red,
                    unsigned char       green,
                    unsigned char       blue
                    );

private:
    hid_device*             dev;
    std::string             location;

    void        SendWritePacket
                    (
                    unsigned short      address,
                    unsigned char       data_size,
                    unsigned char *     data
                    );
};

/*---------------------------------------*\
|  VS_XG270QG_Controller.h                |
|                                         |
|  Definitions and types for ViewSonic    |
|  Elite XG270QG Monitor                  |
|                                         |
|  Lanzaa 1/23/2022                       |
\*---------------------------------------*/

#pragma once

#include <cstdint>
#include <mutex>
#include <string>

#include "RGBController.h"

#include <hidapi/hidapi.h>

enum
{
    VS_MODE_OFF         = 0x00,
    VS_MODE_STATIC      = 0x01,
    VS_MODE_BREATHING   = 0x02,
    VS_MODE_RAINBOW     = 0x07,
    VS_MODE_ELITE       = 0x0A,
    VS_MODE_JAZZ        = 0x0C,
    VS_MODE_WATERFALL   = 0x12,
    VS_MODE_ELITEGLOBAL = 0x13,
};

class VS_XG270QG_Controller
{
public:
    VS_XG270QG_Controller(hid_device* device, const char* path);
    ~VS_XG270QG_Controller();

    std::string GetLocation();
    std::string GetSerial();

    void        SetMode(uint8_t mode, uint8_t r, uint8_t g, uint8_t b);

private:
    hid_device* dev;
    std::string location;

    std::string version;
    std::string serial;

    std::string ReadVersion();
    void        SendModeComplete
                    (
                    uint8_t mode1, uint8_t r1, uint8_t g1, uint8_t b1,
                    uint8_t mode2, uint8_t r2, uint8_t g2, uint8_t b2
                    );
    void        SendCommand(uint8_t  *config, size_t length);
};

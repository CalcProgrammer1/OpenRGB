/*-----------------------------------------*\
|  EVGAGP102Controller.h                    |
|                                           |
|  Definitions and types for EVGA           |
|  GP102-based Nvidia GPUs' RGB controller  |
|  GeForce GTX 1080 Ti K|NGP|N and FTW3.    |
|                                           |
|  Fabricio Murta (avengerx) 1/31/2021      |
\*-----------------------------------------*/

#include <string>
#include "i2c_smbus.h"
#include <vector>

#pragma once

#define EVGA_GP102_CONTROLLER_NAME "EVGA GP102 Nvidia GPU"

enum
{
    EVGA_GP102_REG_MODE  = 0x0C,
    EVGA_GP102_REG_CMD   = 0x0E,
    EVGA_GP102_REG_VALID = 0x04
};

enum
{
    EVGA_GP102_MODE_OFF    = 0x00,
    EVGA_GP102_MODE_CUSTOM = 0x01
    // TODO: Other LEDSync modes (rainbow, breath, pulse)
};

enum
{
    EVGA_GP102_CIDX_RED = 0,
    EVGA_GP102_CIDX_GREEN = 1,
    EVGA_GP102_CIDX_BLUE = 2
};

enum
{
    EVGA_GP102_CMD_BEGIN = 0xE5,
    EVGA_GP102_CMD_COLOR = 0xE9,
    EVGA_GP102_CMD_END = 0xE0
};

typedef struct
{
    std::string zone_name;
    s32 dev_addr;
    s32 color_addrs[3];
    s32 resp_ready;
    s32 resp_clear;
} zoneinfo;

const static zoneinfo gpuzoneinfos[]
{
    {
        "Nameplate",
        0x4A,
        {0x09, 0x0A, 0x0B},
        0x03,
        0x00
    },
    {
        "Backplate",
        0x2A,
        {0x30, 0x31, 0x32},
        0xE9,
        0xE0
    }
};

class EVGAGP102Controller
{
public:
    EVGAGP102Controller(i2c_smbus_interface* bus, zoneinfo info);
    ~EVGAGP102Controller();

    bool                            IsValid();
    std::string                     GetDeviceLocation();
    std::string                     GetName();
    unsigned char                   GetMode();

    void                            SetColor(unsigned char red, unsigned char green, unsigned char blue);
    std::array<unsigned char, 3>    GetColor();
    void                            SetMode(unsigned char mode);
    void                            SaveSettings();

private:
    bool                            CommandAcknowledged();
    bool                            CommandCompleted();
    s32                             QueryCommand(s32 command);
    void                            SendCommand(s32 command);
    unsigned char                   GetRed();
    unsigned char                   GetGreen();
    unsigned char                   GetBlue();

    i2c_smbus_interface* bus;
    zoneinfo zi;

};

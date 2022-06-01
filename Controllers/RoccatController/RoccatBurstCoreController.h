/*-------------------------------------------------------------------*\
|  RoccatBurstCoreController.h                                        |
|                                                                     |
|  Driver for Roccat Burst Core Mouse                                 |
|                                                                     |
|  Morgan Guimard (morg)   6/01/2022                                  |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include <hidapi/hidapi.h>

#define ROCCAT_BURST_CORE_CONTROL_MODE_PACKET_LENGTH    6
#define ROCCAT_BURST_CORE_DIRECT_MODE_PACKET_LENGTH     11
#define ROCCAT_BURST_CORE_FLASH_PACKET_LENGTH           63
#define ROCCAT_BURST_CORE_FLASH_REPORT_ID               0x06
#define ROCCAT_BURST_CORE_DIRECT_MODE_REPORT_ID         0x0D
#define ROCCAT_BURST_CORE_DIRECT_MODE_BYTE              0x0B
#define ROCCAT_BURST_CORE_NUMBER_OF_LEDS                1

enum
{
    ROCCAT_BURST_CORE_DIRECT_MODE_VALUE     = 0x00,
    ROCCAT_BURST_CORE_STATIC_MODE_VALUE     = 0x01,
    ROCCAT_BURST_CORE_WAVE_MODE_VALUE       = 0x0A,
    ROCCAT_BURST_CORE_HEARTBEAT_MODE_VALUE  = 0x04,
    ROCCAT_BURST_CORE_BREATHING_MODE_VALUE  = 0x03,
    ROCCAT_BURST_CORE_BLINKING_MODE_VALUE   = 0x02
};

enum
{
    ROCCAT_BURST_CORE_SPEED_MIN             = 0x01,
    ROCCAT_BURST_CORE_SPEED_MAX             = 0x0B,
    ROCCAT_BURST_CORE_BRIGHTNESS_MIN        = 0x00,
    ROCCAT_BURST_CORE_BRIGHTNESS_MAX        = 0xFF
};

class RoccatBurstCoreController
{
public:
    RoccatBurstCoreController(hid_device* dev_handle, const hid_device_info& info);
    ~RoccatBurstCoreController();

    std::string     GetSerialString();
    std::string     GetDeviceLocation();
    std::string     GetFirmwareVersion();

    void            SetupDirectMode();
    void            SendDirect(std::vector<RGBColor> colors);
    void            SetMode(std::vector<RGBColor> colors,
                            unsigned char mode_value,
                            unsigned char speed,
                            unsigned char brightness,
                            unsigned int color_mode,
                            unsigned int mode_flags
                            );
private:
    hid_device*     dev;
    std::string     location;
    std::string     serial_number;
    std::string     version;

    unsigned int    CalculateCRC(unsigned char* bytes);
    void            SwitchControl(bool direct);
};

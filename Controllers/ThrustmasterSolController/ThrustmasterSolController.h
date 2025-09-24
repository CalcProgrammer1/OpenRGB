/*---------------------------------------------------------*\
| ThrustmasterSolController.h                               |
|                                                           |
|   Driver for Thrustmaster Sol series joysticks            |
|                                                           |
|   Ken Sanislo                                 02 Apr 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include "RGBController.h"

#ifdef _WIN32
#include "dependencies/libusb-1.0.27/include/libusb.h"
#else
#include <libusb.h>
#endif

/*-----------------------------------------------------*\
| Thrustmaster vendor ID                                |
\*-----------------------------------------------------*/
#define THRUSTMASTER_VID                        0x044F

/*-----------------------------------------------------*\
| Thrustmaster Sol series product IDs                   |
\*-----------------------------------------------------*/
#define THRUSTMASTER_SOL_R_RIGHT_PID            0x0422
#define THRUSTMASTER_SOL_R_LEFT_PID             0x042A
#define THRUSTMASTER_SOL_F16_RIGHT_PID          0x0420
#define THRUSTMASTER_SOL_F18_RIGHT_PID          0x0421
#define THRUSTMASTER_SOL_F16_LEFT_PID           0x0428
#define THRUSTMASTER_SOL_F18_LEFT_PID           0x0429

/*-----------------------------------------------------*\
| Thrustmaster Sol USB constants                        |
\*-----------------------------------------------------*/
#define THRUSTMASTER_SOL_INTERFACE              1
#define THRUSTMASTER_SOL_ENDPOINT_OUT           0x02
#define THRUSTMASTER_SOL_ENDPOINT_IN            0x82
#define THRUSTMASTER_SOL_PACKET_SIZE            64
#define THRUSTMASTER_SOL_MAX_ENTRIES            15

/*-----------------------------------------------------*\
| Thrustmaster Sol report type identifiers              |
\*-----------------------------------------------------*/
#define THRUSTMASTER_SOL_REPORT_BASE_LO        0x01
#define THRUSTMASTER_SOL_REPORT_BASE_HI        0x08
#define THRUSTMASTER_SOL_REPORT_GRIP_LO        0x01
#define THRUSTMASTER_SOL_REPORT_GRIP_HI        0x88

/*-----------------------------------------------------*\
| Thrustmaster Sol persistence flags                    |
\*-----------------------------------------------------*/
#define THRUSTMASTER_SOL_VOLATILE              0x00
#define THRUSTMASTER_SOL_PERSISTENT            0x80

/*-----------------------------------------------------*\
| Thrustmaster Sol read query report types               |
\*-----------------------------------------------------*/
#define THRUSTMASTER_SOL_READ_BASE_LO          0x02
#define THRUSTMASTER_SOL_READ_BASE_HI          0x00
#define THRUSTMASTER_SOL_READ_GRIP_LO          0x02
#define THRUSTMASTER_SOL_READ_GRIP_HI          0x80

/*-----------------------------------------------------*\
| Thrustmaster Sol zone constants                       |
|                                                       |
| Zone 0x00 is shared: on the base it is a logo LED,   |
| on the grip it is the thumbstick LED. The grip flag   |
| (bit 8) selects the grip report type for zone 0x00.   |
\*-----------------------------------------------------*/
#define THRUSTMASTER_SOL_ZONE_THUMBSTICK       0x00
#define THRUSTMASTER_SOL_GRIP_FLAG             0x100
#define THRUSTMASTER_SOL_MARKER                0xFF

/*-----------------------------------------------------*\
| Thrustmaster Sol zone count for Sol-R                 |
| 19 base zones + thumbstick + logo zone 0x00 = 21     |
\*-----------------------------------------------------*/
#define THRUSTMASTER_SOL_R_ZONE_COUNT          21

class ThrustmasterSolController
{
public:
    ThrustmasterSolController(libusb_device_handle* dev_handle, const char* path, unsigned short pid, std::string dev_name);
    ~ThrustmasterSolController();

    std::string     GetDeviceLocation();
    std::string     GetDeviceName();
    std::string     GetSerialString();
    unsigned short  GetPID();

    void            SetLEDColor(unsigned int zone, RGBColor color);
    void            SetLEDColors(unsigned int* zones, RGBColor* colors,
                                 unsigned int count);
    void            SaveColors(unsigned int* zones, RGBColor* colors,
                               unsigned int count);
    void            ReadColors(std::vector<unsigned int>& zones,
                               std::vector<RGBColor>& colors);

private:
    libusb_device_handle*   dev;
    std::string             location;
    std::string             name;
    std::string             serial;
    unsigned short          pid;

    void    SendPacket(unsigned char* packet, unsigned int size);
    void    BuildAndSendPackets(unsigned int* zones, RGBColor* colors,
                                unsigned int count, bool persistent);
};

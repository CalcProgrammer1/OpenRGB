/*---------------------------------------------------------*\
| EVGACLCController.h                                       |
|                                                           |
|   Driver for EVGA CLC liquid coolers (Asetek 690LC)       |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <libusb.h>
#include "RGBController.h"

enum
{
    EVGA_CLC_MODE_FIXED     = 0,
    EVGA_CLC_MODE_FADING    = 1,
    EVGA_CLC_MODE_BLINKING  = 2,
    EVGA_CLC_MODE_RAINBOW   = 3,
};

class EVGACLCController
{
public:
    EVGACLCController(libusb_device_handle* dev_handle, std::string dev_name);
    ~EVGACLCController();

    std::string     GetFirmwareString();
    std::string     GetLocation();
    std::string     GetNameString();

    void            SetFixed
                        (
                            unsigned char r,
                            unsigned char g,
                            unsigned char b
                        );

    void            SetFading
                        (
                            unsigned char r1,
                            unsigned char g1,
                            unsigned char b1,
                            unsigned char r2,
                            unsigned char g2,
                            unsigned char b2,
                            unsigned char speed
                        );

    void            SetBlinking
                        (
                            unsigned char r,
                            unsigned char g,
                            unsigned char b,
                            unsigned char speed
                        );

    void            SetRainbow
                        (
                            unsigned char speed
                        );

private:
    libusb_device_handle*   dev;
    std::string             firmware_version;
    std::string             location;
    std::string             name;

    void            SendInit();
    void            SendFirmwareRequest();
    void            FlushBuffers();

    void            SendConfigPacket
                        (
                            unsigned char r1,
                            unsigned char g1,
                            unsigned char b1,
                            unsigned char r2,
                            unsigned char g2,
                            unsigned char b2,
                            unsigned char interval1,
                            unsigned char interval2,
                            bool          not_blackout,
                            bool          fading,
                            bool          blinking,
                            bool          alert
                        );

    void            SendRainbowSpeed
                        (
                            unsigned char speed
                        );
};

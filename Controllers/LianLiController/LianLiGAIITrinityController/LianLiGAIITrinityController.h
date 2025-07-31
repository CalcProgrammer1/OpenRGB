/*---------------------------------------------------------*\
| LianLiGAIITrinityController.h                             |
|                                                           |
|   Driver for Lian Li GAII Trinity                         |
|                                                           |
|   Michael Losert                              27 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <hidapi.h>
#include "RGBController.h"

/*----------------------------------------------------------------------------*\
| Definitions related to LED configuration.                                    |
\*----------------------------------------------------------------------------*/

class LianLiGAIITrinityController
{
public:
    struct GAII_Info
    {
        std::string serial;
        std::string version;
    };

    enum GAII_PacketType : unsigned char
    {
        PT_RGB_CONTROL = 0x83,
        PT_FIRMWARE_INFO = 0x86
    };

    enum GAII_Modes : unsigned char
    {
        M_RAINBOW = 0x01,
        M_RAINBOW_MORPH,
        M_STATIC_COLOR,
        M_BREATHING_COLOR,
        M_RUNWAY,
        M_METEOR,
        M_VORTEX,
        M_CROSSING_OVER,
        M_TAI_CHI,
        M_COLORFUL_STARRY_NIGHT,
        M_STATIC_STARRY_NIGHT,
        M_VOICE,
        M_BIG_BANG,
        M_PUMP,
        M_COLORS_MORPH,
        /* M_BOUNCE, */ // TODO: requires zone-specific modes
    };

    enum GAII_Ring : unsigned char
    {
        R_INNER,
        R_OUTER,
        R_BOTH,
    };

    enum GAII_Brightness : unsigned char
    {
        B_OFF,
        B_25,
        B_50,
        B_75,
        B_100,
    };

    enum GAII_Speed : unsigned char
    {
        S_VERY_SLOW,
        S_SLOW,
        S_MODERATE,
        S_FAST,
        S_VERY_FAST,
    };

    enum GAII_Direction : unsigned char
    {
        D_RIGHT,
        D_LEFT
    };

    enum GAII_ByteAddress : unsigned char
    {
        BA_PACKET_TYPE = 0x01,

        BA_RING = 0x06,
        BA_MODE,
        BA_BRIGHTNESS,
        BA_SPEED,

        BA_R0 = 0x0A,
        BA_G0,
        BA_B0,
        BA_R1,
        BA_G1,
        BA_B1,
        BA_R2,
        BA_G2,
        BA_B2,
        BA_R3,
        BA_G3,
        BA_B3,

        BA_DIRECTION = 0x16,
    };

    LianLiGAIITrinityController(hid_device* dev_handle, char* path);
    ~LianLiGAIITrinityController();

    std::string GetLocation();

    GAII_Info GetControllerInfo();

    void SetMode_Rainbow(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction);
    void SetMode_RainbowMorph(GAII_Brightness brightness, GAII_Speed speed);
    void SetMode_StaticColor(GAII_Brightness brightness, RGBColor rgb0, RGBColor rgb1);
    void SetMode_BreathingColor(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1);
    void SetMode_Runway(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1);
    void SetMode_Meteor(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3);
    void SetMode_Vortex(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3);
    void SetMode_CrossingOver(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3);
    void SetMode_TaiChi(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1);
    void SetMode_ColorfulStarryNight(GAII_Brightness brightness, GAII_Speed speed);
    void SetMode_StaticStarryNight(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0);
    void SetMode_Voice(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1);
    void SetMode_BigBang(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3);
    void SetMode_Pump(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1);
    void SetMode_ColorsMorph(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction);

private:
    std::string location;
    unsigned char* GetRGBControlPacketTemplate();
    void SetRGB(unsigned char* usb_buf, RGBColor rgb0) { SetRGB(usb_buf, &rgb0, nullptr, nullptr, nullptr); };
    void SetRGB(unsigned char* usb_buf, RGBColor rgb0, RGBColor rgb1) { SetRGB(usb_buf, &rgb0, &rgb1, nullptr, nullptr); };
    void SetRGB(unsigned char* usb_buf, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2) { SetRGB(usb_buf, &rgb0, &rgb1, &rgb2, nullptr); };
    void SetRGB(unsigned char* usb_buf, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3) { SetRGB(usb_buf, &rgb0, &rgb1, &rgb2, &rgb3); };
    void SetRGB(unsigned char* usb_buf, RGBColor* rgb0, RGBColor* rgb1, RGBColor* rgb2, RGBColor* rgb3);

    hid_device* dev;
};

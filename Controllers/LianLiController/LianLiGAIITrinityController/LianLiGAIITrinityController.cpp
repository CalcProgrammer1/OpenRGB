/*---------------------------------------------------------*\
| LianLiGAIITrinityController.cpp                           |
|                                                           |
|   Driver for Lian Li GAII Trinity                         |
|                                                           |
|   Michael Losert                              27 Oct 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <sstream>
#include "LianLiGAIITrinityController.h"
#include "StringUtils.h"

LianLiGAIITrinityController::LianLiGAIITrinityController(hid_device* dev_handle, char* path)
{
    dev         = dev_handle;
    location    = path;
}

LianLiGAIITrinityController::~LianLiGAIITrinityController()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string LianLiGAIITrinityController::GetLocation()
{
    return("HID: " + location);
}

LianLiGAIITrinityController::GAII_Info LianLiGAIITrinityController::GetControllerInfo()
{
    GAII_Info controllerInfo;

    // get serial number
    const uint8_t sz    = 255;
    wchar_t       tmp[sz];

    hid_get_serial_number_string(dev, tmp, sz);
    controllerInfo.serial = StringUtils::wstring_to_string(tmp);

    // get firmware version
    unsigned char data[64] = "";
    data[0x00] = 0x01;
    data[GAII_ByteAddress::BA_PACKET_TYPE] = GAII_PacketType::PT_FIRMWARE_INFO;
    hid_write(dev, data, sizeof(data));
    memset(data, 0, sizeof(data));
    hid_read(dev, data, sizeof(data));
    data[sizeof(data) - 1] = 0;
    std::string response(reinterpret_cast<char*>(&data[0x06]));
    memset(data, 0, sizeof(data));
    hid_read(dev, data, sizeof(data));
    data[sizeof(data) - 1] = 0;
    response += " (" + std::string(reinterpret_cast<char*>(&data[0x06])) + ")";
    std::replace( response.begin(), response.end(), ',', ' ');
    controllerInfo.version = response.substr(0, 100);

    return controllerInfo;
}

unsigned char* LianLiGAIITrinityController::GetRGBControlPacketTemplate()
{
    static unsigned char usb_buf[64];
    memset(usb_buf, 0, sizeof(usb_buf));

    usb_buf[0x00] = 0x01;
    usb_buf[GAII_ByteAddress::BA_PACKET_TYPE] = GAII_PacketType::PT_RGB_CONTROL;
    usb_buf[0x05] = 0x13;

    return usb_buf;
}

void LianLiGAIITrinityController::SetRGB(unsigned char* usb_buf, RGBColor* rgb0, RGBColor* rgb1, RGBColor* rgb2, RGBColor* rgb3)
{
    if(rgb0)
    {
        usb_buf[GAII_ByteAddress::BA_R0] = RGBGetRValue(*rgb0);
        usb_buf[GAII_ByteAddress::BA_G0] = RGBGetGValue(*rgb0);
        usb_buf[GAII_ByteAddress::BA_B0] = RGBGetBValue(*rgb0);
    }

    if(rgb1)
    {
        usb_buf[GAII_ByteAddress::BA_R1] = RGBGetRValue(*rgb1);
        usb_buf[GAII_ByteAddress::BA_G1] = RGBGetGValue(*rgb1);
        usb_buf[GAII_ByteAddress::BA_B1] = RGBGetBValue(*rgb1);
    }

    if(rgb2)
    {
        usb_buf[GAII_ByteAddress::BA_R2] = RGBGetRValue(*rgb2);
        usb_buf[GAII_ByteAddress::BA_G2] = RGBGetGValue(*rgb2);
        usb_buf[GAII_ByteAddress::BA_B2] = RGBGetBValue(*rgb2);
    }

    if(rgb3)
    {
        usb_buf[GAII_ByteAddress::BA_R3] = RGBGetRValue(*rgb3);
        usb_buf[GAII_ByteAddress::BA_G3] = RGBGetGValue(*rgb3);
        usb_buf[GAII_ByteAddress::BA_B3] = RGBGetBValue(*rgb3);
    }
}

void LianLiGAIITrinityController::SetMode_Rainbow(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_RAINBOW;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    usb_buf[GAII_ByteAddress::BA_DIRECTION] = direction;

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_RainbowMorph(GAII_Brightness brightness, GAII_Speed speed)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_RAINBOW_MORPH;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_StaticColor(GAII_Brightness brightness, RGBColor rgb0, RGBColor rgb1)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_STATIC_COLOR;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    SetRGB(usb_buf, rgb0, rgb1);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_BreathingColor(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_BREATHING_COLOR;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    SetRGB(usb_buf, rgb0, rgb1);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_Runway(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_RUNWAY;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    SetRGB(usb_buf, rgb0, rgb1);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_Meteor(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_METEOR;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    usb_buf[GAII_ByteAddress::BA_DIRECTION] = direction;
    SetRGB(usb_buf, rgb0, rgb1, rgb2, rgb3);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_Vortex(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_VORTEX;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    usb_buf[GAII_ByteAddress::BA_DIRECTION] = direction;
    SetRGB(usb_buf, rgb0, rgb1, rgb2, rgb3);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_CrossingOver(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_CROSSING_OVER;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    usb_buf[GAII_ByteAddress::BA_DIRECTION] = direction;
    SetRGB(usb_buf, rgb0, rgb1, rgb2, rgb3);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_TaiChi(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_TAI_CHI;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    usb_buf[GAII_ByteAddress::BA_DIRECTION] = direction;
    SetRGB(usb_buf, rgb0, rgb1);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_ColorfulStarryNight(GAII_Brightness brightness, GAII_Speed speed)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_COLORFUL_STARRY_NIGHT;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_StaticStarryNight(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_STATIC_STARRY_NIGHT;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    SetRGB(usb_buf, rgb0);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_Voice(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_VOICE;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    SetRGB(usb_buf, rgb0, rgb1);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_BigBang(GAII_Brightness brightness, GAII_Speed speed, RGBColor rgb0, RGBColor rgb1, RGBColor rgb2, RGBColor rgb3)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_BIG_BANG;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    SetRGB(usb_buf, rgb0, rgb1, rgb2, rgb3);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_Pump(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction, RGBColor rgb0, RGBColor rgb1)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_PUMP;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    usb_buf[GAII_ByteAddress::BA_DIRECTION] = direction;
    SetRGB(usb_buf, rgb0, rgb1);

    hid_write(dev, usb_buf, 64);
}

void LianLiGAIITrinityController::SetMode_ColorsMorph(GAII_Brightness brightness, GAII_Speed speed, GAII_Direction direction)
{
    unsigned char *usb_buf = GetRGBControlPacketTemplate();

    usb_buf[GAII_ByteAddress::BA_MODE] = GAII_Modes::M_COLORS_MORPH;
    usb_buf[GAII_ByteAddress::BA_RING] = GAII_Ring::R_BOTH;
    usb_buf[GAII_ByteAddress::BA_BRIGHTNESS] = brightness;
    usb_buf[GAII_ByteAddress::BA_SPEED] = speed;
    usb_buf[GAII_ByteAddress::BA_DIRECTION] = direction;

    hid_write(dev, usb_buf, 64);
}

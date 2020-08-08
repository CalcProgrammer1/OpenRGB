/*-----------------------------------------*\
|  HyperXPulsefireSurgeController.cpp       |
|                                           |
|  Driver for HyperX Pulsefire Surge        |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/25/2020   |
\*-----------------------------------------*/

#include "HyperXPulsefireSurgeController.h"

#include <cstring>

HyperXPulsefireSurgeController::HyperXPulsefireSurgeController(hid_device* dev_handle)
{
    dev = dev_handle;

    SelectProfile(0x01);
    SetProfileBrightness(0x01, 0x64);
}

HyperXPulsefireSurgeController::~HyperXPulsefireSurgeController()
{

}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXPulsefireSurgeController::SelectProfile
    (
    unsigned char   profile
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_SURGE_PACKET_ID_SELECT_PROFILE;
    buf[0x02]   = profile;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXPulsefireSurgeController::SetProfileBrightness
    (
    unsigned char   profile,
    unsigned char   brightness
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_SURGE_PACKET_ID_SET_BRIGHTNESS;
    buf[0x02]   = profile;
    buf[0x03]   = 0x01;
    buf[0x04]   = 0x01;
    buf[0x05]   = 0x01;
    buf[0x06]   = 0x64;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXPulsefireSurgeController::SendData
    (
    unsigned char   mode,
    RGBColor*       color_data
    )
{
    unsigned char buf[264];

    unsigned short dpi_1 = 0x000A;
    unsigned short dpi_2 = 0x0014;
    unsigned short dpi_3 = 0x001E;
    unsigned char dpi_r = 255;
    unsigned char dpi_g = 255;
    unsigned char dpi_b = 255;

    unsigned char rgb1_r = 255;
    unsigned char rgb1_g = 255;
    unsigned char rgb1_b = 255;
    unsigned char rgb2_r = 255;
    unsigned char rgb2_g = 255;
    unsigned char rgb2_b = 255;
    unsigned char rgb3_r = 255;
    unsigned char rgb3_g = 255;
    unsigned char rgb3_b = 255;

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Set Configuration packet                       |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_SURGE_PACKET_ID_SET_CONFIGURATION;
    buf[0x02]   = 0x01;

    /*-----------------------------------------------------*\
    | DPI settings                                          |
    \*-----------------------------------------------------*/
    buf[0x08]   = (dpi_1 & 0xFF);
    buf[0x09]   = dpi_1 >> 8;
    buf[0x0A]   = (dpi_2 & 0xFF);
    buf[0x0B]   = dpi_2 >> 8;
    buf[0x0C]   = (dpi_3 & 0xFF);
    buf[0x0D]   = dpi_3 >> 8;


    buf[0x0E]   = 0x80;
    buf[0x10]   = 0x40;
    buf[0x12]   = 0x01;
    buf[0x13]   = 0x08;
    
    /*-----------------------------------------------------*\
    | DPI settings                                          |
    \*-----------------------------------------------------*/
    buf[0x14]   = (dpi_1 & 0xFF);
    buf[0x15]   = dpi_1 >> 8;
    buf[0x16]   = (dpi_2 & 0xFF);
    buf[0x17]   = dpi_2 >> 8;
    buf[0x18]   = (dpi_3 & 0xFF);
    buf[0x19]   = dpi_3 >> 8;
    
    buf[0x1A]   = 0x80;
    buf[0x1C]   = 0x40;
    buf[0x1D]   = 0x01;
    buf[0x1E]   = 0x08;
    
    buf[0x20]   = 0x01;
    buf[0x21]   = 0x01;
    buf[0x22]   = 0x01;
    buf[0x25]   = 0x02;
    buf[0x26]   = 0xF0;
    buf[0x29]   = 0x02;
    buf[0x2A]   = 0xF1;
    buf[0x2D]   = 0x02;
    buf[0x2E]   = 0xF2;

    buf[0x31]   = 0x71;
    buf[0x32]   = 0xF0;
    buf[0x35]   = 0x02;
    buf[0x36]   = 0xF9;
    buf[0x39]   = 0x02;
    buf[0x3A]   = 0xF8;
    buf[0x3D]   = 0x02;
    buf[0x3E]   = 0xF4;
    buf[0x41]   = 0x02;
    buf[0x42]   = 0xF3;

    buf[0x4E]   = 0x01;
    buf[0x50]   = 0x01;

    buf[0x51]   = mode;
    buf[0x52]   = 0x03;
    buf[0x53]   = 0x03;
    buf[0x54]   = 0x03;

    buf[0x56]   = 0x28;
    buf[0x58]   = 0x28;
    buf[0x5A]   = 0x28;
    buf[0x5C]   = 0x28;
    buf[0x5D]   = 0x28;
    buf[0x60]   = 0xF0;
    buf[0x62]   = 0xF0;

    buf[0x64]   = dpi_r;
    buf[0x65]   = dpi_g;
    buf[0x66]   = dpi_b;

    buf[0x6D]   = rgb1_r;
    buf[0x70]   = rgb1_g;
    buf[0x73]   = rgb1_b;

    /*-----------------------------------------------------*\
    | Copy in direct color data                             |
    |   Red starts at 0x76                                  |
    |   Green starts at 0x96                                |
    |   Blue starts at 0xB6                                 |
    \*-----------------------------------------------------*/
    for(int color_idx = 0; color_idx < 0x20; color_idx++)
    {
        buf[0x76 + color_idx] = RGBGetRValue(color_data[color_idx]);
        buf[0x96 + color_idx] = RGBGetGValue(color_data[color_idx]);
        buf[0xB6 + color_idx] = RGBGetBValue(color_data[color_idx]);
    }

    buf[0xDC]   = rgb2_r;
    buf[0xDF]   = rgb2_g;
    buf[0xE2]   = rgb2_b;
    buf[0xE6]   = rgb3_r;
    buf[0xE9]   = rgb3_g;
    buf[0xEC]   = rgb3_b;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXPulsefireSurgeController::SendDirect
    (
    RGBColor*       color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_SURGE_PACKET_ID_DIRECT;
    buf[0x03]   = 0xA0;

    for(int red_idx = 0; red_idx < 32; red_idx++)
    {
        buf[0x08 + red_idx] = RGBGetRValue(color_data[red_idx]);
    }

    for(int grn_idx = 0; grn_idx < 32; grn_idx++)
    {
        buf[0x28 + grn_idx] = RGBGetGValue(color_data[grn_idx]);
    }

    for(int blu_idx = 0; blu_idx < 32; blu_idx++)
    {
        buf[0x48 + blu_idx] = RGBGetBValue(color_data[blu_idx]);
    }

    buf[0x6C] = RGBGetRValue(color_data[32]);
    buf[0x6D] = RGBGetGValue(color_data[32]);
    buf[0x6E] = RGBGetBValue(color_data[32]);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

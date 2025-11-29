/*---------------------------------------------------------*\
| SteelSeriesApex9Controller.cpp                            |
|                                                           |
|   Driver for SteelSeries Apex 9                           |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "SteelSeriesApex9Controller.h"

using namespace std::chrono_literals;

static unsigned int keys[] = {0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
                              0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, //20
                              0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21,
                              0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, //40
                              0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x32, 0x33, 0x34, 0x35, 0x36,
                              0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, //60
                              0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
                              0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x64, 0xE0, //80
                              0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xF0, 0x31, 0x87,
                              0x88, 0x89, 0x8A, 0x8B, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, //100
                              0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62,
                              0x63 };

SteelSeriesApex9Controller::SteelSeriesApex9Controller(hid_device* dev_handle, steelseries_type type, const char* path, std::string dev_name) : SteelSeriesApexBaseController (dev_handle, path, dev_name)
{
    proto_type  = type;
}

SteelSeriesApex9Controller::~SteelSeriesApex9Controller()
{
    hid_close(dev);
}

void SteelSeriesApex9Controller::SetMode(unsigned char mode /*mode*/, std::vector<RGBColor> /*colors*/ )
{
    unsigned char mode_colors[9];

    active_mode = mode;

    memset(mode_colors, 0x00, sizeof(mode_colors));
}

void SteelSeriesApex9Controller::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char buf[APEX_9_PACKET_LENGTH];
    int num_keys = 0;

    num_keys = sizeof(keys) / sizeof(*keys);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0;
    buf[0x01]   = APEX_9_PACKET_ID_DIRECT;
    buf[0x02]   = num_keys;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0; i < num_keys; i++)
    {
        buf[(i*4)+3] = keys[i];
        buf[(i*4)+4] = RGBGetRValue(colors[i]);
        buf[(i*4)+5] = RGBGetGValue(colors[i]);
        buf[(i*4)+6] = RGBGetBValue(colors[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, APEX_9_PACKET_LENGTH);

}

std::string SteelSeriesApex9Controller::GetSerial()
{
    std::string return_string = "";

    switch(proto_type)
    {
    case APEX_9_TKL:
        return_string = "64847";
        break;
    case APEX_9_MINI:
        return_string = "64837";
        break;
    default:
        return_string = "Apex 9 GetSerial() error";
    }

    return(return_string);
}

std::string SteelSeriesApex9Controller::GetVersion()
{
    std::string return_string = "Unsupported protocol";

    unsigned char   obuf[STEELSERIES_PACKET_OUT_SIZE];
    unsigned char   ibuf[STEELSERIES_PACKET_IN_SIZE];
    int             result;

    memset(obuf, 0x00, sizeof(obuf));
    obuf[0x00] = 0;
    obuf[0x01] = 0x90;
    hid_write(dev, obuf, STEELSERIES_PACKET_OUT_SIZE);
    result = hid_read_timeout(dev, ibuf, STEELSERIES_PACKET_IN_SIZE, 2);

    if(result > 0)
    {
        std::string fwver(ibuf, ibuf+STEELSERIES_PACKET_IN_SIZE);
        fwver = fwver.substr(2, fwver.size());
        fwver = fwver.c_str();

        /*---------------------------------------------*\
        | Find 2 periods in string, if found we can     |
        | form a X.Y.Z revision.                        |
        \*---------------------------------------------*/
        std::size_t majorp = fwver.find('.');
        if(majorp != std::string::npos)
        {
            std::size_t minorp = fwver.find('.', majorp+1);
            if(minorp != std::string::npos)
            {
                std::string major = fwver.substr(0, majorp);
                std::string minor = fwver.substr(majorp+1, (minorp-majorp-1));
                std::string build = fwver.substr(minorp+1);
                return_string = "KBD: " + major + "." + minor + "." + build;
            }
        }
    }

    return(return_string);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void SteelSeriesApex9Controller::SelectProfile(unsigned char profile)
{
    unsigned char buf[65];

    /*-----------------------------------------------------*\
    | Zero out buffer, set up packet and send               |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));
    buf[0x00]   = 0;
    buf[0x01]   = 0x89;
    buf[0x02]   = profile;
    hid_send_feature_report(dev, buf, 65);
}

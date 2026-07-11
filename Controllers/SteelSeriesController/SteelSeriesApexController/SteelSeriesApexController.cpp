/*---------------------------------------------------------*\
| SteelSeriesApexController.cpp                             |
|                                                           |
|   Driver for SteelSeries Apex Keyboards                   |
|                                                           |
|   New driver based on SignalRGB Plugins                   |
|   https://gitlab.com/signalrgb/signal-plugins/            |
|                                                           |
|   Eric Samuelson (edbgon)                     05 Jul 2020 |
|   Filipe S. (filipesn)                         5 Jan 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <cstdio>
#include "SteelSeriesApexController.h"
#include "LogManager.h"

using namespace std::chrono_literals;

static unsigned int keys[] =
{
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
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
    0x63, 0xFB
};

inline static const std::map<unsigned short, protocol_quirk> protocol_map =
{
    {STEELSERIES_APEX_9_TKL_PID,             APEX_GEN2},
    {STEELSERIES_APEX_9_MINI_PID,            APEX_GEN2},
    {STEELSERIES_APEX_PRO_TKL_2023_PID,      APEX_GEN2},
    {STEELSERIES_APEX_PRO_TKL_2023_WL_PID_1, APEX_GEN3},
    {STEELSERIES_APEX_PRO_TKL_2023_WL_PID_2, APEX_GEN3},
    {STEELSERIES_APEX_PRO_TKL_GEN3_PID,      APEX_GEN3},
    {STEELSERIES_APEX_PRO_TKL_GEN3_WL_PID_1, APEX_GEN3},
    {STEELSERIES_APEX_PRO_TKL_GEN3_WL_PID_2, APEX_GEN3},
};

SteelSeriesApexController::SteelSeriesApexController(hid_device* dev_handle, steelseries_type type, const char* path, unsigned short pid, std::string dev_name) : SteelSeriesApexBaseController(dev_handle, path, dev_name)
{
    proto_type = type;
    kbd_quirk = APEX_GEN1;
    device_pid = pid;
    if(protocol_map.count(device_pid) > 0)
    {
        kbd_quirk = protocol_map.at(device_pid);
    }
    SendInitialization();
}

SteelSeriesApexController::~SteelSeriesApexController()
{
    SendDeinitialization();
    hid_close(dev);
}

/*---------------------------------------------------*\
| OpenRGB must clear initialized keyboards during     |
| shutdown as hardware lighting profiles won't apply  |
| otherwise. Keyboards not needing initialization are |
| also cleared, though selecting a hardware profile   |
| will override the leds unlike the former.           |
\*---------------------------------------------------*/
void SteelSeriesApexController::SendDeinitialization()
{
    unsigned char obuf[STEELSERIES_PACKET_OUT_SIZE];
    memset(obuf, 0x00, sizeof(obuf));
    obuf[0x00] = 0;
    obuf[0x01] = reset_cmd;
    hid_write(dev, obuf, STEELSERIES_PACKET_OUT_SIZE);
}

void SteelSeriesApexController::SetMode(unsigned char mode, std::vector<RGBColor>)
{
    if(active_mode != mode)
    {
        if(active_mode == APEX_MODE_ONBOARD && kbd_quirk != APEX_GEN1)
        {
            SendInitialization();
        }
        if(mode == APEX_MODE_ONBOARD)
        {
            SendDeinitialization();
        }
    }
    active_mode = mode;
}

void SteelSeriesApexController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char buf[APEX_PACKET_LENGTH];
    int num_keys = sizeof(keys) / sizeof(*keys);
    unsigned char packet_id = APEX_GEN1_PACKET_ID_DIRECT;

    if(kbd_quirk >= APEX_GEN2)
    {
        /*-------------------------------------------------*\
        | Apparently Gen 3 wireless models reuse this       |
        | protocol, make sure to place their model here and |
        | further below when developing.                    |
        \*-------------------------------------------------*/
        if(device_pid == STEELSERIES_APEX_PRO_TKL_GEN3_WL_PID_1 ||
           device_pid == STEELSERIES_APEX_PRO_TKL_GEN3_WL_PID_2 ||
           device_pid == STEELSERIES_APEX_PRO_TKL_2023_WL_PID_1 ||
           device_pid == STEELSERIES_APEX_PRO_TKL_2023_WL_PID_2)
        {
             packet_id = APEX_2023_PACKET_ID_DIRECT_WIRELESS;
        }
        else
        {
             packet_id = APEX_2023_PACKET_ID_DIRECT;
        }
    }

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0;
    buf[0x01]   = packet_id;
    buf[0x02]   = kbd_quirk ? (unsigned char)colors.size() : num_keys;

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
    hid_send_feature_report(dev, buf, APEX_PACKET_LENGTH);
}

/*---------------------------------------------------------*\
| Private packet sending functions.                         |
\*---------------------------------------------------------*/
void SteelSeriesApexController::SelectProfile(unsigned char profile)
{
    unsigned char buf[STEELSERIES_PACKET_OUT_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer, set up packet and send               |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));
    buf[0x00]   = 0;
    buf[0x01]   = APEX_PACKET_ID_PROFILE;
    buf[0x02]   = profile;
    hid_send_feature_report(dev, buf, STEELSERIES_PACKET_OUT_SIZE);
}

void SteelSeriesApexController::SendInitialization()
{
    /*---------------------------------------------------*\
    | Check models which can be Gen 3 via firmware update |
    \*---------------------------------------------------*/
    if(device_pid == STEELSERIES_APEX_PRO_TKL_PID ||
       device_pid == STEELSERIES_APEX_9_TKL_PID   ||
       device_pid == STEELSERIES_APEX_9_MINI_PID)
    {
        std::string fwver = GetVersion();
        fwver.erase(0,4);
        int major = 0, minor = 0, patch = 0;
        int count = sscanf(fwver.c_str(), "%d.%d.%d", &major, &minor, &patch);
        if(count == 3)
        {
            /*--------------------------------------------*\
            | Assume Gen 3 protocol from 1.19.7 or newer.  |
            \*--------------------------------------------*/
            if(major > 1)
            {
                kbd_quirk = APEX_GEN3;
            }
            else if(major == 1)
            {
                if(minor > 19)
                {
                    kbd_quirk = APEX_GEN3;
                }
                else if(minor == 19 && patch >= 7)
                {
                    kbd_quirk = APEX_GEN3;
                }
            }
        }
    }

    /*-----------------------------------------------------*\
    | Send Initialization packet for Gen 3 protocol. This   |
    | seems to be required for some models and not others,  |
    | regardless there is no harm in sending it.            |
    \*-----------------------------------------------------*/
    if(kbd_quirk >= APEX_GEN3)
    {
        unsigned char buf[STEELSERIES_PACKET_OUT_SIZE];
        memset(buf, 0x00, sizeof(buf));
        reset_cmd = APEX_2023_PACKET_ID_ONBOARD;
        buf[0x00] = 0x00;
        buf[0x01] = APEX_2023_PACKET_ID_INIT;
        hid_send_feature_report(dev, buf, STEELSERIES_PACKET_OUT_SIZE);
        LOG_DEBUG("[%s] Using Apex 2023/Gen3 protocol.", name.c_str());
    }
    else
    {
        reset_cmd = APEX_GEN1_PACKET_ID_ONBOARD;
        LOG_DEBUG("[%s] Using Apex Legacy protocol.", name.c_str());
    }
}

std::string SteelSeriesApexController::GetSerial()
{
    if(kbd_quirk == APEX_GEN1)
    {
        return SteelSeriesApexBaseController::GetSerial();
    }

    /*-------------------------------------------------*\
    | Gen 2+ doesn't expose the serial number in        |
    | firmware. A region code is instead set by the     |
    | user and subsequently read back. This region code |
    | is used by all 5 on-board configs.                |
    | For consistency with other Apex keyboards, this   |
    | region code in tandem with the model is mapped    |
    | to an approximate product number as the region    |
    | patch logic from that point is identical.         |
    | The product number used may not be an exact match |
    | for the keyboard but should reflect the form      |
    | factor, region and RGB layout                     |
    \*-------------------------------------------------*/
    unsigned char obuf[STEELSERIES_PACKET_OUT_SIZE];
    unsigned char ibuf[STEELSERIES_PACKET_IN_SIZE];
    int result;

    memset(obuf, 0x00, sizeof(obuf));

    if(device_pid == STEELSERIES_APEX_PRO_TKL_GEN3_PID)
    {
        obuf[0x00] = 0;
        obuf[0x01] = APEX_2023_PACKET_ID_REGION;
        hid_write(dev, obuf, STEELSERIES_PACKET_OUT_SIZE);
        result = hid_read_timeout(dev, ibuf, STEELSERIES_PACKET_IN_SIZE, 2);

        if(result > 3 && ibuf[0] == APEX_2023_PACKET_ID_REGION)
        {
            switch(ibuf[2])
            {
            case 0x1:
                return "64740";
                break;
            case 0x3:
                return "64741";
                break;
            case 0x4:
                return "64743";
                break;
            case 0x6:
                return "64744";
                break;
            case 0xA:
                return "64742";
                break;
            case 0xD:
                return "64745";
                break;
            default:
                break;
            }
        }
    }
    if(device_pid == STEELSERIES_APEX_9_TKL_PID)
    {
        return "64847";
    }
    if(device_pid == STEELSERIES_APEX_9_MINI_PID)
    {
        return "64837";
    }
    return "64865";
}

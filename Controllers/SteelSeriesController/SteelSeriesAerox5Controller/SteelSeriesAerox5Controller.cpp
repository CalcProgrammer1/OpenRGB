/*---------------------------------------------------------*\
| SteelSeriesAerox5Controller.cpp                           |
|                                                           |
|   Driver for the Steelseries Aerox 5                      |
|                                                           |
|   Bobby Quantum (BobbyQuantum)                19 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "SteelSeriesAerox5Controller.h"
#include "LogManager.h"

SteelSeriesAerox5Controller::SteelSeriesAerox5Controller(hid_device* dev_handle, steelseries_type proto_type, const char* path, std::string dev_name) : SteelSeriesMouseController(dev_handle, proto_type, path, dev_name)
{
    SendInit();
}

SteelSeriesAerox5Controller::~SteelSeriesAerox5Controller()
{
    hid_close(dev);
}

void SteelSeriesAerox5Controller::SendInit()
{
    /*-----------------------------------------------------------------*\
    | This sets sensitivity and allows software mode?? max 5 uint8      |
    |   buffer[2]        = Count eg. 0 thru 5                           |
    |   buffer[4] to [8] = dpi / 50 range = 0x04 - 0xC7 eg. 400 = 0x08  |
    \*-----------------------------------------------------------------*/
    uint8_t buffer[STEELSERIES_AEROX5_PACKET_SIZE] = { 0x00, 0x2D };

    hid_send_feature_report(dev, buffer, STEELSERIES_AEROX5_PACKET_SIZE);
}

std::string SteelSeriesAerox5Controller::GetFirmwareVersion()
{
    uint8_t result      = 0;
    const uint8_t CMD   = 0x90;
    const uint8_t sz    = 16;
    char version[sz + 1];

    uint8_t buffer[STEELSERIES_AEROX5_PACKET_SIZE] = { 0x00, CMD, 0x00 };

    hid_send_feature_report(dev, buffer, STEELSERIES_AEROX5_PACKET_SIZE);
    do
    {
        result = hid_read_timeout(dev, buffer, STEELSERIES_AEROX5_PACKET_SIZE, STEELSERIES_AEROX5_TIMEOUT);
        LOG_DEBUG("[%s] Reading version buffer: Bytes Read %d Buffer %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", STEELSERIES_AEROX5_NAME, result,
                  buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5], buffer[6], buffer[7], buffer[8], buffer[9], buffer[10]);
    } while(result > 0 && buffer[0] != CMD);

    if(buffer[0] == CMD)
    {
        /*-----------------------------------------------------------------*\
        | Read the version from the second character                        |
        \*-----------------------------------------------------------------*/
        memcpy(version, &buffer[1], sz);
        version[sz] = 0;
        std::string tmp = std::string(version);
        LOG_DEBUG("[%s] Version: %s as string %s", STEELSERIES_AEROX5_NAME, version, tmp.c_str());

        return tmp;
    }
    else
    {
        LOG_DEBUG("[%s] Unable to get version: giving up!", STEELSERIES_AEROX5_NAME);
        return "";
    }
}

steelseries_mouse SteelSeriesAerox5Controller::GetMouse()
{
    return aerox_5;
}

void SteelSeriesAerox5Controller::SetLightEffectAll(uint8_t /*effect*/)
{
    /*-----------------------------------------------------------------*\
    | Not used by this device                                           |
    \*-----------------------------------------------------------------*/
}

void SteelSeriesAerox5Controller::SetColor
    (
        unsigned char       zone_id,
        unsigned char       red,
        unsigned char       green,
        unsigned char       blue,
        unsigned char       brightness
        )
{
    uint8_t buffer[STEELSERIES_AEROX5_PACKET_SIZE] = { 0x00, 0x21, 0x01 };

    uint8_t offset     = 0x03;

    switch (zone_id)
    {
    case 0:
        offset         = 0x03;
        break;
    case 1:
        buffer[2]      = 0x02;
        offset         = 0x06;
        break;
    case 2:
        buffer[2]      = 0x04;
        offset         = 0x09;
        break;
    case 3:
        buffer[1]      = 0x26;
        offset         = 0x04;
        break;
    default:
        return;
    }

    buffer[offset]     = red;
    buffer[offset + 1] = green;
    buffer[offset + 2] = blue;

    hid_write(dev, buffer, STEELSERIES_AEROX5_PACKET_SIZE);
    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    if (brightness != current_brightness)
    {
        SetBrightness(brightness);
        current_brightness = brightness;
    }
}

void SteelSeriesAerox5Controller::SetBrightness(uint8_t brightness)
{
    uint8_t buffer[3] = { 0x00, 0x23, brightness };

    hid_write(dev, buffer, STEELSERIES_AEROX5_PACKET_SIZE);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void SteelSeriesAerox5Controller::Save()
{
    uint8_t buffer2[3]  = { 0x00, 0x11, 0x00 };

    hid_write(dev, buffer2, STEELSERIES_AEROX5_PACKET_SIZE);
}

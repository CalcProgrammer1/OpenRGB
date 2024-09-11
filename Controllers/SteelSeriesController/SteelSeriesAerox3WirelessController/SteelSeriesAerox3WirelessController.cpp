/*---------------------------------------------------------*\
| SteelSeriesAerox3WirelessController.cpp                   |
|                                                           |
|   Driver for SteelSeries Aerox 3 Wireless                 |
|                                                           |
|   Ensar S (esensar)                           09 Sep 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "SteelSeriesAerox3WirelessController.h"
#include "LogManager.h"
#include "SteelSeriesGeneric.h"

SteelSeriesAerox3WirelessController::SteelSeriesAerox3WirelessController(hid_device* dev_handle, steelseries_type proto_type, const char* path)
    : SteelSeriesMouseController(dev_handle, proto_type, path)
{
    SendInit();
}

SteelSeriesAerox3WirelessController::~SteelSeriesAerox3WirelessController()
{
    hid_close(dev);
}

void SteelSeriesAerox3WirelessController::SendInit()
{
    /*-----------------------------------------------------------------*\
    | This sets sensitivity and allows software mode?? max 5 uint8      |
    |   buffer[2]        = Count eg. 0 thru 5                           |
    |   buffer[4] to [8] = dpi / 50 range = 0x04 - 0xC7 eg. 400 = 0x08  |
    \*-----------------------------------------------------------------*/
    uint8_t buffer[STEELSERIES_AEROX3_PACKET_SIZE] = { 0x00, 0x2D };

    hid_send_feature_report(dev, buffer, STEELSERIES_AEROX3_PACKET_SIZE);
}

std::string SteelSeriesAerox3WirelessController::GetFirmwareVersion()
{
    uint8_t result      = 0;
    const uint8_t CMD   = 0x90;
    const uint8_t sz    = 16;
    char version[sz + 1];

    uint8_t buffer[STEELSERIES_AEROX3_PACKET_SIZE] = { 0x00, CMD, 0x00 };

    hid_send_feature_report(dev, buffer, STEELSERIES_AEROX3_PACKET_SIZE);
    do
    {
        result = hid_read_timeout(dev, buffer, STEELSERIES_AEROX3_PACKET_SIZE, STEELSERIES_AEROX3_TIMEOUT);
        LOG_DEBUG("[%s] Reading version buffer: Bytes Read %d Buffer %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X", (proto == AEROX_3_WIRELESS) ? STEELSERIES_AEROX3_WIRELESS_NAME : STEELSERIES_AEROX3_WIRELESS_WIRED_NAME, result,
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
        LOG_DEBUG("[%s] Version: %s as string %s", (proto == AEROX_3_WIRELESS) ? STEELSERIES_AEROX3_WIRELESS_NAME : STEELSERIES_AEROX3_WIRELESS_WIRED_NAME, version, tmp.c_str());

        return tmp;
    }
    else
    {
        LOG_DEBUG("[%s] Unable to get version: giving up!", (proto == AEROX_3_WIRELESS) ? STEELSERIES_AEROX3_WIRELESS_NAME : STEELSERIES_AEROX3_WIRELESS_WIRED_NAME);
        return "";
    }
}

steelseries_mouse SteelSeriesAerox3WirelessController::GetMouse()
{
    return aerox_3_wireless;
}

void SteelSeriesAerox3WirelessController::SetLightEffectAll(uint8_t effect)
{
    if(effect == 0x05)
    {
        uint8_t buffer[STEELSERIES_AEROX3_PACKET_SIZE]  = { 0x00, 0x22, 0xFF };

        WriteBuffer(buffer);
    }
}

void SteelSeriesAerox3WirelessController::SetColor
    (
    unsigned char       zone_id,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue,
    unsigned char       brightness
    )
{
    uint8_t buffer[STEELSERIES_AEROX3_PACKET_SIZE] = { 0x00, 0x21, 0x01 };

    buffer[0x03]        = zone_id;
    uint8_t offset      = 0x04;

    buffer[offset]      = red;
    buffer[offset + 1]  = green;
    buffer[offset + 2]  = blue;

    WriteBuffer(buffer);

    // Supports only 10 steps of brightness
    brightness = (uint8_t)(brightness / 10);

    if(brightness != current_brightness)
    {
        SetBrightness(brightness);
        current_brightness = brightness;
    }
}

void SteelSeriesAerox3WirelessController::SetBrightness(uint8_t brightness)
{
    uint8_t buffer[STEELSERIES_AEROX3_PACKET_SIZE]  = { 0x00, 0x23, brightness };

    WriteBuffer(buffer);
}

void SteelSeriesAerox3WirelessController::Save()
{
    /*---------------------------------------------------------------------------------*\
    | Save packet was not confirmed as working but packet is verified as correct.       |
    |   https://github.com/flozz/rivalcfg/blob/master/rivalcfg/devices/aerox3.py#L141   |
    \*---------------------------------------------------------------------------------*/
    uint8_t buffer2[STEELSERIES_AEROX3_PACKET_SIZE]  = { 0x00, 0x11, 0x00 };

    WriteBuffer(buffer2);
}

void SteelSeriesAerox3WirelessController::WriteBuffer(uint8_t* buffer)
{
    if(proto == AEROX_3_WIRELESS)
    {
        buffer[1] |= STEELSERIES_AEROX3_WIRELESS_FLAG;
    }

    hid_write(dev, buffer, STEELSERIES_AEROX3_PACKET_SIZE);

    if(proto == AEROX_3_WIRELESS)
    {
        // Readback required in wireless mode
        hid_read_timeout(dev, buffer, STEELSERIES_AEROX3_PACKET_SIZE, STEELSERIES_AEROX3_TIMEOUT);
    }
}

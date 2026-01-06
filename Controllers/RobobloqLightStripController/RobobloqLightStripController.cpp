/*---------------------------------------------------------*\
| RobobloqLightStripController.cpp                          |
|                                                           |
|   Detector for Robobloq Monitor Light Strip               |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <cstring>
#include <vector>
#include <cstdio>
#include <thread>
#include <numeric>
#include <iomanip>
#include <sstream>
#include "LogManager.h"
#include "RobobloqLightStripController.h"
#include "RobobloqRangeMerger.h"
#include "RGBController.h"
#include <Colors.h>

using namespace std::chrono_literals;

RobobloqLightStripController::RobobloqLightStripController(hid_device* dev_handle, const char* path, std::string dev_name)
{
    dev         = dev_handle;
    location    = path;
    name        = dev_name;
    packet_index = 0x02;
    led_count    = 0;

    RequestDeviceInfo();
    Initialize();
}

RobobloqLightStripController::~RobobloqLightStripController()
{
    hid_close(dev);
}

std::string RobobloqLightStripController::GetDeviceLocation()
{
    return location;
}

std::string RobobloqLightStripController::GetDeviceName()
{
    return name;
}

std::string RobobloqLightStripController::GetSerialString()
{
    return uuid;
}

std::string RobobloqLightStripController::GetFirmwareVersion()
{
    return firmware_version;
}

int RobobloqLightStripController::GetLEDCount()
{
    return led_count;
}

int RobobloqLightStripController::GetLEDsPerSide()
{
    switch(physical_size)
    {
        case 34:
            return 15;
        default:
            return 0;  /* unknown                          */
    }
}


int RobobloqLightStripController::GetPhysicalSizeInInches()
{
    return physical_size;
}

void RobobloqLightStripController::Initialize()
{
    RequestDeviceInfo();

    /*-----------------------------------------------------*\
    | This tells the device (permanently) not to try to use |
    | its keyboard to open a URL to the driver download     |
    | page. Yes, it really does that.                       |
    \*-----------------------------------------------------*/
    SendPacket({ROBOBLOQ_CMD_SET_OPEN_URL, 0x00});

    SetBrightness(0xF9);
    SetDynamicSpeed(0x32);
}

/*---------------------------------------------------------*\
| Set the entire light strip to the specified color.        |
\*---------------------------------------------------------*/
void RobobloqLightStripController::SetColor(RGBColor c)
{
    std::vector<unsigned char> payload = {
        0x86,
        0x01,
        (unsigned char)RGBGetRValue(c),
        (unsigned char)RGBGetGValue(c),
        (unsigned char)RGBGetBValue(c),
        led_count,
        /*-------------------------------------------------*\
        | Add dummy range (matches original application)    |
        \*-------------------------------------------------*/
        (unsigned char)(led_count + 1),
        0x00,
        0x00,
        0x00,
        0xFE,
    };
    SendPacket(payload, false);
}

/*---------------------------------------------------------*\
| Set a single LED to the specified color. LED is 0-indexed |
\*---------------------------------------------------------*/
void RobobloqLightStripController::SetLEDColor(int led, RGBColor c)
{
    std::vector<unsigned char> payload = {
        0x86,
        (unsigned char)(led + 1),
        (unsigned char)RGBGetRValue(c),
        (unsigned char)RGBGetGValue(c),
        (unsigned char)RGBGetBValue(c),
        (unsigned char)(led + 1),
    };
    SendPacket(payload, false);
}

void RobobloqLightStripController::SetColorRanges(const std::vector<unsigned char>& ranges)
{
    /*-----------------------------------------------------*\
    | 6 bytes overhead (header + command + checksum) plus   |
    | ranges must fit in 64 bytes                           |
    \*-----------------------------------------------------*/
    if((ranges.size() + 6) > 64)
    {
        LOG_ERROR("[Robobloq] SetColorRanges: Too many ranges (%d) for packet size", (int)ranges.size());
        return;
    }
    std::vector<unsigned char> payload = { 0x86 };
    payload.insert(payload.end(), ranges.begin(), ranges.end());
    SendPacket(payload, false);
}

void RobobloqLightStripController::SetBrightness(unsigned char brightness)
{
    /*-----------------------------------------------------*\
    | 0 value is interpreted as max (255)                   |
    \*-----------------------------------------------------*/
    if(brightness == 0)
    {
        brightness = 1;
    }

    SendPacket({ROBOBLOQ_CMD_SET_BRIGHTNESS, brightness});
}

void RobobloqLightStripController::SetDynamicEffect(unsigned char effect)
{
    SendPacket({ROBOBLOQ_CMD_SET_EFFECT, ROBOBLOQ_EFFECT_DYNAMIC, effect});
}

void RobobloqLightStripController::SetDynamicSpeed(unsigned char speed)
{
    /*-----------------------------------------------------*\
    | Device expects 0x00 = fast, 0x64 = slow               |
    \*-----------------------------------------------------*/
    speed = ROBOBLOQ_DYNAMIC_SPEED_MAX - speed;

    SendPacket({ROBOBLOQ_CMD_SET_DYNAMIC_SPEED, speed});
}

void RobobloqLightStripController::TurnOff()
{
    /*-----------------------------------------------------*\
    | The device doesn't really turn off: we send a command |
    | to disable any device-side animations, then set all   |
    | LEDs to black.                                        |
    \*-----------------------------------------------------*/
    SendPacket({ROBOBLOQ_CMD_TURN_OFF});
    SetColor(COLOR_BLACK);
}

/*---------------------------------------------------------*\
| Update all LED colors at once.                            |
|                                                           |
| The official app reduces 71 LED values to 34 distinct     |
| ranges before sending them to the device, so we do the    |
| same. SendSyncScreen is capable of sending any number of  |
| ranges (e.g 71 - 1 per pixel) but this has not been       |
| properly tested.                                          |
\*---------------------------------------------------------*/
void RobobloqLightStripController::SetCustom(const std::vector<RGBColor>& colors)
{
    int num_leds = (int)colors.size();
    if(num_leds != this->led_count)
    {
        LOG_ERROR("[Robobloq] SetCustom: Number of colors (%d) does not match LED count (%d), rejecting", num_leds, this->led_count);
        return;
    }

    std::vector<unsigned char> color_bytes = MergeRobobloqRanges(colors, ROBOBLOQ_TUPLE_COUNT);

    SendSyncScreen(color_bytes);
}

/*---------------------------------------------------------*\
| Internal method to send color ranges to the device.       |
\*---------------------------------------------------------*/
void RobobloqLightStripController::SendSyncScreen(const std::vector<unsigned char>& color_bytes)
{
    if(color_bytes.size() % 5 != 0)
    {
        LOG_ERROR("[Robobloq] SendSyncScreen: color_bytes size (%d) is not a multiple of 5, rejecting", (int)color_bytes.size());
        return;
    }

    SendMultiPacket(ROBOBLOQ_CMD_SET_SYNC_SCREEN, color_bytes);
}

/*---------------------------------------------------------*\
| Sends a multi-packet command to the device.               |
|                                                           |
| This allows payloads >64 bytes, e.g. SendSyncScreen       |
\*---------------------------------------------------------*/
void RobobloqLightStripController::SendMultiPacket(unsigned char command, const std::vector<unsigned char>& payload)
{
    /*-----------------------------------------------------*\
    | Allow for 6-byte header and trailing checksum         |
    \*-----------------------------------------------------*/
    unsigned short len = (unsigned short)(payload.size() + 7);

    std::vector<unsigned char> data = {0x53, 0x43, (unsigned char)(len >> 8), (unsigned char)(len & 0xFF), packet_index, command};
    data.insert(data.end(), payload.begin(), payload.end());

    /*-----------------------------------------------------*\
    | Calculate checksum                                    |
    \*-----------------------------------------------------*/
    unsigned int sum = 0;
    for(std::size_t i = 0; i < data.size(); i++)
    {
        sum += data[i];
    }
    data.push_back(sum & 0xFF);

    /*-----------------------------------------------------*\
    | Pad to a multiple of 64 bytes                         |
    \*-----------------------------------------------------*/
    size_t remainder = data.size() % 64;
    if(remainder > 0)
    {
        data.resize(data.size() + 64 - remainder, 0x00);
    }

    /*-----------------------------------------------------*\
    | Send in chunks of 64 bytes                            |
    \*-----------------------------------------------------*/
    for(size_t i = 0; i < data.size(); i += 64)
    {
        WriteReport(&data[i]);

        /*-------------------------------------------------*\
        | Microsleep to avoid overloading the device        |
        \*-------------------------------------------------*/
        std::this_thread::sleep_for(1ms);
    }

    IncPacketIndex();
}

void RobobloqLightStripController::IncPacketIndex()
{
    packet_index = (packet_index + 1) & 0xFF;
}

void RobobloqLightStripController::WriteReport(const unsigned char* data /* 64 bytes*/)
{
    /*-----------------------------------------------------*\
    | Add report ID                                         |
    \*-----------------------------------------------------*/
    std::vector<unsigned char> report(65);
    report[0] = 0x00;
    memcpy(&report[1], data, 64);

    hid_write(dev, report.data(), report.size());
}

void RobobloqLightStripController::SendPacket(const std::vector<unsigned char>& command, bool flush)
{
    unsigned char length = (unsigned char)(command.size() + 5);

    if(length > 64)
    {
        LOG_ERROR("[Robobloq] SendPacket: command size (%d) is too large, rejecting", (int)command.size());
        return;
    }

    std::vector<unsigned char> packet = {0x52, 0x42, length, packet_index};
    packet.insert(packet.end(), command.begin(), command.end());

    unsigned int csum = 0;
    for(std::size_t i = 0; i < packet.size(); i++)
    {
        csum += packet[i];
    }
    packet.push_back(csum & 0xFF);

    packet.resize(64, 0x00);

    WriteReport(packet.data());
    IncPacketIndex();

    if(flush)
    {
        /*-------------------------------------------------*\
        | Flush away any awaiting IN packets                |
        \*-------------------------------------------------*/
        unsigned char buf[64];

        int res = 1;
        while(res > 0)
        {
            res = hid_read_timeout(dev, buf, 64, 0);
        }
    }
}

std::vector<unsigned char> RobobloqLightStripController::SendPacketWithReply(const std::vector<unsigned char>& command)
{
    unsigned char expected_id = packet_index;
    LOG_DEBUG("[Robobloq] WithReply: sending command %02x, expecting reply for packet ID %02x", command[0], expected_id);
    SendPacket(command, false);

    int tries = 3;
    while(tries--)
    {
        unsigned char buf[64];
        int res = 1;
        while(res > 0)
        {
            std::this_thread::sleep_for(10ms);
            res = hid_read_timeout(dev, buf, 64, 1000);
            LOG_DEBUG("[Robobloq] WithReply: read call returned %d packet ID %02x", res, buf[3]);

            if(buf[3] == expected_id)
            {
                return std::vector<unsigned char>(buf, buf + res);
            }
        }

        /*-------------------------------------------------*\
        | Sometimes device gets stuck and we need to send a |
        | new command before we can receive the previous    |
        | command's reply, so send a no-op                  |
        \*-------------------------------------------------*/
        LOG_DEBUG("[Robobloq] No matching reply, sending no-op to try to unstick device");
        SendPacket({ROBOBLOQ_CMD_SET_OPEN_URL, 0x00}, false);
    }

    /*-----------------------------------------------------*\
    | We weren't able to get a reply for this message       |
    \*-----------------------------------------------------*/
    return {};
}

/***
 * Asks device for information and update our attributes. When sent 0x82, the device
 * returns information in the following form:
 *
 *                  header        ID                           uuid                 v1.8.2
 *              /------------\ /======\               /-----------------------\    /------\
 *              52 42 19 02 82 00 05 01 | 22 01 00 47 cd ab c5 74 | 25 bc b7 dc e5 01 08 02
 *                                         |  ?  ?  |                            ?
 *                                         |        led count (71)
 *                                         display size (34")
 *   v1.8.2 is device fw version
 */
bool RobobloqLightStripController::RequestDeviceInfo()
{
    std::vector<unsigned char> data = SendPacketWithReply({ROBOBLOQ_CMD_READ_DEVICE_INFO});
    if(data.size() < 24)
    {
        LOG_DEBUG("[Robobloq] Device Info Data too small! LED count -> 0. Non-Direct modes will work");
        return false;
    }

    this->physical_size = data[8];
    this->led_count = data[11];

    char id_buf[7];
    snprintf(id_buf, sizeof(id_buf), "%02x%02x%02x", data[5], data[6], data[7]);
    this->id = std::string(id_buf);

    char uuid_buf[17];
    for(int i = 0; i < 8; i++)
    {
        sprintf(uuid_buf + (i * 2), "%02x", data[12 + i]);
    }
    this->uuid = std::string(uuid_buf);

    char fw_buf[16];
    snprintf(fw_buf, sizeof(fw_buf), "%d.%d.%d", data[21], data[22], data[23]);
    this->firmware_version = std::string(fw_buf);

    LOG_DEBUG("[Robobloq] Got device uuid: %s, fw: %s, size: %d\", leds: %d",
        this->uuid.c_str(), this->firmware_version.c_str(),
        this->physical_size, this->led_count);

    return true;
}

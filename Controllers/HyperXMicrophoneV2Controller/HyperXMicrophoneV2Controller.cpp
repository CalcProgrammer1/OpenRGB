/*---------------------------------------------------------*\
| HyperXMicrophoneV2Controller.cpp                          |
|                                                           |
|   Driver for HyperX QuadCast 2 S Microphone               |
|                                                           |
|   Morgan Guimard (morg)                                   |
|   Logan Phillips (Eclipse)                    23 Oct 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include <sstream>
#include <iomanip>
#include "HyperXMicrophoneV2Controller.h"
#include "StringUtils.h"
#include "LogManager.h"

HyperXMicrophoneV2Controller::HyperXMicrophoneV2Controller(hid_device* dev_handle, std::string path, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    name                = dev_name;
    errors              = 0;
    last_error_time     = std::chrono::steady_clock::now();
    pause_until         = std::chrono::steady_clock::now();
}

HyperXMicrophoneV2Controller::~HyperXMicrophoneV2Controller()
{
    if(dev)
    {
        hid_close(dev);
    }
}

std::string HyperXMicrophoneV2Controller::GetDeviceLocation()
{
    return(location);
}

std::string HyperXMicrophoneV2Controller::GetNameString()
{
    return(name);
}

std::string HyperXMicrophoneV2Controller::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

bool HyperXMicrophoneV2Controller::ShouldPauseUpdates()
{
    return std::chrono::steady_clock::now() < pause_until;
}

void HyperXMicrophoneV2Controller::FlushInputBuffer()
{
    uint8_t discard[HYPERX_QUADCAST_2S_PACKET_SIZE];
    int flushed_count = 0;

    /*---------------------------------------------------------*\
    | Read and discard all pending responses in the buffer      |
    \*---------------------------------------------------------*/
    while(hid_read_timeout(dev, discard, HYPERX_QUADCAST_2S_PACKET_SIZE, 10) > 0)
    {
        flushed_count++;
    }

    if(flushed_count > 0)
    {
        LOG_DEBUG("[%s] Flushed %d stale response(s) from input buffer", name.c_str(), flushed_count);
    }
}

void HyperXMicrophoneV2Controller::TrackCommunicationError()
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    long long time_since_last_error = std::chrono::duration_cast<std::chrono::seconds>(now - last_error_time).count();

    if(time_since_last_error < 10)
    {
        errors++;
        if(errors >= 5)
        {
            LOG_WARNING("[%s] Multiple communication errors detected (%d). Flushing input buffer to clear stale responses.", name.c_str(), errors);
            FlushInputBuffer();
        }
        if(errors >= 10)
        {
            LOG_ERROR("[%s] Multiple consecutive communication errors detected. Another program (such as HyperX NGENUITY) may be controlling this device. Pausing updates for 5 seconds.", name.c_str());
            pause_until = std::chrono::steady_clock::now() + std::chrono::seconds(5);
            errors = 0;
        }
    }
    else
    {
        errors = 1;
    }

    last_error_time = now;
}

bool HyperXMicrophoneV2Controller::WaitForResponse(const uint8_t* sent_packet, int timeout_ms)
{
    uint8_t response[HYPERX_QUADCAST_2S_PACKET_SIZE];
    memset(response, 0, HYPERX_QUADCAST_2S_PACKET_SIZE);

    int bytes_read = hid_read_timeout(dev, response, HYPERX_QUADCAST_2S_PACKET_SIZE, timeout_ms);

    if(bytes_read <= 0)
    {
        LOG_WARNING("[%s] No response received from device (timeout: %d ms)", name.c_str(), timeout_ms);
        TrackCommunicationError();
        return false;
    }

    /*---------------------------------------------------------*\
    | Verify the response echoes back the command bytes         |
    | Response bytes 14-15 should match sent bytes 0-1          |
    \*---------------------------------------------------------*/
    if(response[14] == sent_packet[0] && response[15] == sent_packet[1])
    {
        return true;
    }

    /*---------------------------------------------------------*\
    | Log validation failure with full response payload         |
    \*---------------------------------------------------------*/
    std::stringstream response_hex;
    for(int i = 0; i < bytes_read; i++)
    {
        response_hex << std::hex << std::setw(2) << std::setfill('0') << (int)response[i];
    }

    LOG_WARNING("[%s] Invalid response from device. Expected echo of bytes [0x%02X 0x%02X] at positions 14-15, but got [0x%02X 0x%02X]. Full response: %s",
                name.c_str(), sent_packet[0], sent_packet[1], response[14], response[15], response_hex.str().c_str());

    TrackCommunicationError();
    return false;
}

void HyperXMicrophoneV2Controller::SendColorPackets(std::vector<RGBColor> colors, uint8_t command_byte)
{
    uint8_t buf[HYPERX_QUADCAST_2S_PACKET_SIZE];
    unsigned int total_leds_sent = 0;

    for(unsigned int packet = 0; packet < 6; packet++)
    {
        memset(buf, 0, HYPERX_QUADCAST_2S_PACKET_SIZE);

        buf[0] = HYPERX_QUADCAST_2S_REPORT_ID;
        buf[1] = command_byte;
        buf[2] = packet;

        unsigned int c = 0;

        while(c < HYPERX_QUADCAST_2S_LEDS_PER_PACKET && total_leds_sent < HYPERX_QUADCAST_2S_TOTAL_LEDS)
        {
            buf[4 + (3 * c)] = RGBGetRValue(colors[total_leds_sent]);
            buf[5 + (3 * c)] = RGBGetGValue(colors[total_leds_sent]);
            buf[6 + (3 * c)] = RGBGetBValue(colors[total_leds_sent]);

            c++;
            total_leds_sent++;
        }

        hid_write(dev, buf, HYPERX_QUADCAST_2S_PACKET_SIZE);
        WaitForResponse(buf);
    }
}

void HyperXMicrophoneV2Controller::SendDirect(std::vector<RGBColor> colors)
{
    lock.lock();

    /*---------------------------------------------------------*\
    | Skip sending if we're in pause mode                       |
    \*---------------------------------------------------------*/
    if(ShouldPauseUpdates())
    {
        lock.unlock();
        return;
    }

    uint8_t buf[HYPERX_QUADCAST_2S_PACKET_SIZE];

    /*---------------------------------------------------------*\
    | Send header packet for direct mode                        |
    \*---------------------------------------------------------*/
    memset(buf, 0, HYPERX_QUADCAST_2S_PACKET_SIZE);
    buf[0] = HYPERX_QUADCAST_2S_REPORT_ID;
    buf[1] = 0x01;
    buf[2] = 0x06;
    hid_write(dev, buf, HYPERX_QUADCAST_2S_PACKET_SIZE);
    WaitForResponse(buf);

    /*---------------------------------------------------------*\
    | Send color data packets                                   |
    \*---------------------------------------------------------*/
    SendColorPackets(colors, 0x02);

    lock.unlock();
}

void HyperXMicrophoneV2Controller::SaveColors(std::vector<RGBColor> colors)
{
    lock.lock();

    /*---------------------------------------------------------*\
    | Skip sending if we're in pause mode                       |
    \*---------------------------------------------------------*/
    if(ShouldPauseUpdates())
    {
        lock.unlock();
        return;
    }

    uint8_t buf[HYPERX_QUADCAST_2S_PACKET_SIZE];

    /*---------------------------------------------------------*\
    | Initiate save to device                                   |
    \*---------------------------------------------------------*/
    memset(buf, 0, HYPERX_QUADCAST_2S_PACKET_SIZE);
    buf[0] = HYPERX_QUADCAST_2S_REPORT_ID;
    buf[1] = 0x03;
    buf[2] = 0x01;
    buf[3] = 0x06;
    hid_write(dev, buf, HYPERX_QUADCAST_2S_PACKET_SIZE);
    WaitForResponse(buf);

    /*---------------------------------------------------------*\
    | Send 6 color data packets                                 |
    \*---------------------------------------------------------*/
    SendColorPackets(colors, 0x04);

    /*---------------------------------------------------------*\
    | Send "Framerate" packet                                   |
    | If someone ever wanted to try and replicate the effects,  |
    | apparently this is the packet to try and change.          |
    | I believe currently this is setting a "static" frame      |
    \*---------------------------------------------------------*/
    memset(buf, 0, HYPERX_QUADCAST_2S_PACKET_SIZE);
    buf[0] = 0x42;
    buf[1] = 0x02;
    buf[5] = 0xE8;
    buf[6] = 0x03;
    hid_write(dev, buf, HYPERX_QUADCAST_2S_PACKET_SIZE);
    WaitForResponse(buf);

    /*---------------------------------------------------------*\
    | Send final packet                                         |
    \*---------------------------------------------------------*/
    memset(buf, 0, HYPERX_QUADCAST_2S_PACKET_SIZE);
    buf[0] = 0x40;
    buf[1] = 0x01;
    buf[4] = 0xFF;
    hid_write(dev, buf, HYPERX_QUADCAST_2S_PACKET_SIZE);
    WaitForResponse(buf);

    lock.unlock();
}

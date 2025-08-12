/*---------------------------------------------------------*\
| HYTENexusController.cpp                                   |
|                                                           |
|   Driver for HYTE Nexus                                   |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "HYTENexusController.h"
#include "LogManager.h"

using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| The protocol for the HYTE NP50 and Q60 are documented     |
| on hackmd.io:                                             |
|   NP50: https://hackmd.io/3X_ojT77Sr-sLt5Fo2CYMQ          |
|   Q60:  https://hackmd.io/7qUhUQfIQReQYNhdGLqO6g          |
|                                                           |
| More information on the HYTE Nexus Playground site:       |
|   https://hyte.com/nexus/nexus-playground                 |
\*---------------------------------------------------------*/

HYTENexusController::HYTENexusController(char* port, unsigned short pid, std::string dev_name)
{
    port_name   = port;
    device_pid  = pid;
    name        = dev_name;

    /*-----------------------------------------------------*\
    | Initialize channels based on PID                      |
    \*-----------------------------------------------------*/
    memset(channels, 0, sizeof(channels));

    switch(pid)
    {
        case HYTE_NEXUS_PORTAL_NP50_PID:
            num_channels = 3;

            channels[0].is_nexus_channel    = true;
            channels[1].is_nexus_channel    = true;
            channels[2].is_nexus_channel    = true;

            channels[0].has_6_led_logo      = true;
            break;

        case HYTE_THICC_Q60_PID:
            num_channels = 4;

            channels[0].is_nexus_channel    = true;
            channels[1].is_nexus_channel    = true;
            channels[2].is_nexus_channel    = false;
            channels[3].is_nexus_channel    = false;

            channels[2].has_lcd_leds        = true;
            channels[3].has_4_led_logo      = true;
            break;

        default:
            num_channels = 0;
            break;
    }

    /*-----------------------------------------------------*\
    | Open the port                                         |
    | Baud rate doesn't matter for ACM device               |
    \*-----------------------------------------------------*/
    serialport = new serial_port(port_name.c_str(), 2000000);

    /*-----------------------------------------------------*\
    | Get controller information and firmware version       |
    \*-----------------------------------------------------*/
    ReadDeviceInfo();
    ReadFirmwareVersion();

    /*-----------------------------------------------------*\
    | Get attached device information for all channels      |
    \*-----------------------------------------------------*/
    for(unsigned int channel = 0; channel < num_channels; channel++)
    {
        if(channels[channel].is_nexus_channel)
        {
            ReadChannelInfo(channel);
        }
    }

    keepalive_thread_run = true;
    keepalive_thread = std::thread(&HYTENexusController::KeepaliveThreadFunction, this);
}

HYTENexusController::~HYTENexusController()
{
    keepalive_thread_run = false;
    keepalive_thread.join();

    serialport->serial_close();
}

std::string HYTENexusController::GetFirmwareVersion()
{
    return(firmware_version);
}

std::string HYTENexusController::GetLocation()
{
    return(port_name);
}

std::string HYTENexusController::GetName()
{
    return(name);
}

void HYTENexusController::KeepaliveThreadFunction()
{
    while(keepalive_thread_run.load())
    {
        if((std::chrono::steady_clock::now() - last_update_time) > 2500ms)
        {
            ReadDeviceInfo();
        }
        std::this_thread::sleep_for(1s);
    }
}

std::string HYTENexusController::GetDeviceName(unsigned int device_type)
{
    std::string device_name = "";

    switch(device_type)
    {
        case HYTE_NEXUS_DEVICE_TYPE_LS10:
            device_name = "LS10";
            break;

        case HYTE_NEXUS_DEVICE_TYPE_LS30:
            device_name = "LS30";
            break;

        case HYTE_NEXUS_DEVICE_TYPE_FP12:
            device_name = "FP12";
            break;

        case HYTE_NEXUS_DEVICE_TYPE_FP12_DUO:
            device_name = "FP12 Duo";
            break;

        case HYTE_NEXUS_DEVICE_TYPE_FP12_TRIO:
            device_name = "FP12 Trio";
            break;

        case HYTE_NEXUS_DEVICE_TYPE_LN4060:
            device_name = "LN4060";
            break;

        case HYTE_NEXUS_DEVICE_TYPE_LN70:
            device_name = "LN70";
            break;
    }

    return(device_name);
}

void HYTENexusController::LEDStreaming(unsigned char channel, unsigned short led_count, RGBColor* colors)
{
    /*-----------------------------------------------------*\
    | Send LED Streaming command                            |
    | Byte 0:   FF                                          |
    | Byte 1:   EE                                          |
    | Byte 2:   01                                          |
    | Byte 3:   Channel (Port1, Port2, or Port3)            |
    | Byte 4:   LEDCount_H                                  |
    | Byte 5:   LEDCount_L                                  |
    | Byte 6:   Reserved                                    |
    | Byte 7:   G                                           |
    | Byte 8:   R                                           |
    | Byte 9:   B                                           |
    | Repeat GRB pattern for remaining bytes                |
    \*-----------------------------------------------------*/
    unsigned char command_buf[750];

    memset(command_buf, 0, sizeof(command_buf));

    command_buf[0] = 0xFF;
    command_buf[1] = 0xEE;
    command_buf[2] = 0x01;
    command_buf[3] = (channel + 1);
    command_buf[4] = (led_count >> 8);
    command_buf[5] = (led_count & 0xFF);

    for(unsigned int led_idx = 0; led_idx < led_count; led_idx++)
    {
        unsigned int offset = (led_idx * 3);

        command_buf[7 + offset] = RGBGetGValue(colors[led_idx]);
        command_buf[8 + offset] = RGBGetRValue(colors[led_idx]);
        command_buf[9 + offset] = RGBGetBValue(colors[led_idx]);
    }

    /*-----------------------------------------------------*\
    | The default data length is (led_count * 3) + 7        |
    \*-----------------------------------------------------*/
    unsigned int bytes_to_send = ((led_count * 3) + 7);

    /*-----------------------------------------------------*\
    | The HYTE THICC Q60 requires 90 bytes to be sent for   |
    | the 4th channel (logo) even though it only has 4 LEDs |
    \*-----------------------------------------------------*/
    if((device_pid == HYTE_THICC_Q60_PID)
    && (channel == 3)
    && (bytes_to_send < 90))
    {
        bytes_to_send = 90;
    }

    /*-----------------------------------------------------*\
    | The HYTE Nexus Portal NP50 requires 750 bytes to be   |
    | sent for the 3rd channel, no matter how many LEDs it  |
    | has connected                                         |
    \*-----------------------------------------------------*/
    if((device_pid == HYTE_NEXUS_PORTAL_NP50_PID)
    && (channel == 2)
    && (bytes_to_send < 750))
    {
        bytes_to_send = 750;
    }

    port_mutex.lock();
    serialport->serial_write((char *)command_buf, bytes_to_send);
    serialport->serial_flush_tx();
    serialport->serial_flush_rx();
    port_mutex.unlock();
}

void HYTENexusController::ReadChannelInfo(unsigned char channel)
{
    /*-----------------------------------------------------*\
    | Send Get Channel Info command                         |
    | Byte 0:   FF                                          |
    | Byte 1:   CC                                          |
    | Byte 2:   01 (Get Status)                             |
    | Byte 3:   Channel (Port1, Port2, or Port3)            |
    \*-----------------------------------------------------*/
    unsigned char command_buf[4];

    command_buf[0] = 0xFF;
    command_buf[1] = 0xCC;
    command_buf[2] = 0x01;
    command_buf[3] = (channel + 1);

    port_mutex.lock();
    serialport->serial_write((char *)command_buf, sizeof(command_buf));
    serialport->serial_flush_tx();

    /*-----------------------------------------------------*\
    | Wait 50ms for device to send response                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(50ms);

    /*-----------------------------------------------------*\
    | Receive Channel Info                                  |
    |           First Device        Additional Devices      |
    | Byte 0:   FF                  00                      |
    | Byte 1:   CC                  00                      |
    | Byte 2:   Device Count                                |
    | Byte 3:   Device Type                                 |
    | Byte 4:   Hardware Version                            |
    | Byte 5:   LED Count                                   |
    | Byte 6:   Fan Temp_H                                  |
    | Byte 7:   Fan Temp_L                                  |
    | Byte 8:   Fan RPM_H                                   |
    | Byte 9:   Fan RPM_L                                   |
    | Byte 10:  Fan Orientation                             |
    | Byte 11:  Touch                                       |
    |                                                       |
    | Format repeats with offset of 12 * n for nth device   |
    | in the list, except for the first two bytes being     |
    | zero.                                                 |
    \*-----------------------------------------------------*/
    unsigned char receive_buf[240];

    serialport->serial_read((char *)receive_buf, sizeof(receive_buf));
    serialport->serial_flush_rx();
    port_mutex.unlock();

    channels[channel].num_devices = 0;
    memset(&channels[channel].devices, 0, sizeof(channels[channel].devices));

    for(unsigned int device = 0; device < 19; device++)
    {
        unsigned int offset = 12 * device;

        if(receive_buf[offset + 2] > 0)
        {
            channels[channel].num_devices++;

            channels[channel].devices[device].device_type       = receive_buf[offset + 3];
            channels[channel].devices[device].hardware_version  = receive_buf[offset + 4];
            channels[channel].devices[device].led_count         = receive_buf[offset + 5];
        }
    }
}

void HYTENexusController::ReadDeviceInfo()
{
    /*-----------------------------------------------------*\
    | Send Get Device Info command                          |
    | Byte 0:   FF                                          |
    | Byte 1:   CC                                          |
    | Byte 2:   01 (Get Status)                             |
    | Byte 3:   00 (NP50/Pump)                              |
    \*-----------------------------------------------------*/
    unsigned char command_buf[4];

    command_buf[0] = 0xFF;
    command_buf[1] = 0xCC;
    command_buf[2] = 0x01;
    command_buf[3] = 0x00;

    port_mutex.lock();
    serialport->serial_write((char *)command_buf, sizeof(command_buf));
    serialport->serial_flush_tx();

    /*-----------------------------------------------------*\
    | Wait 50ms for device to send response                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(50ms);

    /*-----------------------------------------------------*\
    | Receive Device Info                                   |
    |           NP50                    Q60                 |
    | Byte 0:   FF                      FF                  |
    | Byte 1:   CC                      CC                  |
    | Byte 2:   00                      00                  |
    | Byte 3:   Noise_H                 Reserve             |
    | Byte 4:   Noise_L                 Reserve             |
    | Byte 5:   Reserve                 In Liquid Temp_H    |
    | Byte 6:   Reserve                 In Liquid Temp_L    |
    | Byte 7:   Temp_H                  Out Liquid Temp_H   |
    | Byte 8:   Temp_L                  Out Liquid Temp_L   |
    | Byte 9:   RPM_H                   Pump RPM_H          |
    | Byte 10:  RPM_L                   Pump RPM_L          |
    | Byte 11:  Reserve                 Fan Exhaust/Intake  |
    | Byte 12:  Current Cooling Mode    Current Cooling Mode|
    | Byte 13:  Warning                 Warnings            |
    \*-----------------------------------------------------*/
    unsigned char receive_buf[14];

    serialport->serial_read((char *)receive_buf, sizeof(receive_buf));
    serialport->serial_flush_rx();
    port_mutex.unlock();

    /*-----------------------------------------------------*\
    | Update last update time                               |
    \*-----------------------------------------------------*/
    last_update_time = std::chrono::steady_clock::now();
}

void HYTENexusController::ReadFirmwareVersion()
{
    /*-----------------------------------------------------*\
    | Send Get Firmware Version command                     |
    | Byte 0:   FF                                          |
    | Byte 1:   DD                                          |
    | Byte 2:   02                                          |
    | Byte 3:   00 (Reserve)                                |
    \*-----------------------------------------------------*/
    unsigned char command_buf[4];

    command_buf[0] = 0xFF;
    command_buf[1] = 0xDD;
    command_buf[2] = 0x02;
    command_buf[3] = 0x00;

    port_mutex.lock();
    serialport->serial_write((char *)command_buf, sizeof(command_buf));
    serialport->serial_flush_tx();

    /*-----------------------------------------------------*\
    | Wait 50ms for device to send response                 |
    \*-----------------------------------------------------*/
    std::this_thread::sleep_for(50ms);

    /*-----------------------------------------------------*\
    | Receive Firmware Version                              |
    | Byte 0:   FF                                          |
    | Byte 1:   DD                                          |
    | Byte 2:   02                                          |
    | Byte 3:   Large Version                               |
    | Byte 4:   Mid Version                                 |
    | Byte 5:   Small Version                               |
    | Byte 6:   Hardware Version                            |
    \*-----------------------------------------------------*/
    unsigned char receive_buf[7];

    serialport->serial_read((char *)receive_buf, sizeof(receive_buf));
    serialport->serial_flush_rx();
    port_mutex.unlock();

    /*-----------------------------------------------------*\
    | Format Firmware Version string                        |
    \*-----------------------------------------------------*/
    firmware_version = "FW: " + std::to_string(receive_buf[3]) + "." + std::to_string(receive_buf[4]) + "." + std::to_string(receive_buf[5]) + ", HW: " + std::to_string(receive_buf[6]);
}

/*---------------------------------------------------------*\
| AlienwareAW3423DWFController.cpp                          |
|                                                           |
|   Driver for the Alienware AW3423DWF monitor              |
|                                                           |
|   Ferréol DUBOIS COLI (Fefe_du_973)           23 Jan 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <bitset>
#include <chrono>
#include <cstring>
#include <thread>

#include "AlienwareAW3423DWFController.h"
#include "StringUtils.h"

AlienwareAW3423DWFController::AlienwareAW3423DWFController(hid_device *dev_handle, const char *path) : dev(dev_handle), location(path){}

void AlienwareAW3423DWFController::SendControlPacket(const unsigned char *data, size_t length)
{
    unsigned char buffer[256] = {0x00};
    memcpy(buffer + 1, data, length);

    hid_write(dev, buffer, length + 1);
}

std::vector<unsigned char> AlienwareAW3423DWFController::GetReportResponse()
{
    uint8_t buffer[193];

    memset(buffer, 0, 193);

    hid_get_input_report(dev, buffer, 193);

    return std::vector<unsigned char>(buffer + 1, buffer + 18);
}

void AlienwareAW3423DWFController::PerformLogin()
{
    unsigned char init_packet[64] =
    {
        0x40, 0xE1, 0x01
    };
    SendControlPacket(init_packet, 4);

    std::vector<unsigned char> response = GetReportResponse();

    std::vector<unsigned char> key = GenerateKey(response);

    unsigned char login_packet[192] = {0x00};
    login_packet[0] = 0x40;
    login_packet[1] = 0xE1;
    login_packet[2] = 0x02;
    memcpy(login_packet + 64, key.data(), key.size());

    SendControlPacket(login_packet, 192);
}

void AlienwareAW3423DWFController::SendColor(unsigned char led_id, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char led_id_2 = (led_id == 0x01) ? 0xf6 : (led_id == 0x02) ? 0xf5
                                                   : (led_id == 0x08)   ? 0xff
                                                                        : 0xfc;
    PerformLogin();

    unsigned char color_packet[192] = {0x00};

    color_packet[0]  = 0x40;
    color_packet[1]  = 0xC6;
    color_packet[6]  = 0x0A;
    color_packet[8]  = 0x6E;
    color_packet[10] = 0x82;
    color_packet[64] = 0x51;
    color_packet[65] = 0x87;
    color_packet[66] = 0xD0;
    color_packet[67] = 0x04;

    color_packet[68] = led_id;
    color_packet[69] = r;
    color_packet[70] = g;
    color_packet[71] = b;
    color_packet[72] = 0x64;
    color_packet[73] = led_id_2;

    SendControlPacket(color_packet, 192);

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

std::vector<unsigned char> AlienwareAW3423DWFController::GenerateKey(
    const std::vector<unsigned char> &response)
{
    std::vector<unsigned char> syn_key(8, 0);

    const std::vector<unsigned char> oem_key = {
        0xf5, 0x3f, 0xc1, 0x39, 0x44, 0x3a, 0x31, 0x79, 0x0d, 0xb1, 0x82, 0x76
    };

    size_t sk_idx = 0, ok_idx = 0;
    while(ok_idx < oem_key.size() && sk_idx < 8)
    {
        unsigned char ok_sub_len = (oem_key[ok_idx] & 1) + ((oem_key[ok_idx] & 0x10) >> 4);

        for(unsigned int i = 0; i < ok_sub_len && sk_idx < 8; i++)
        {
            syn_key[sk_idx] = oem_key[ok_idx + 1] ^ oem_key[ok_idx];
            ok_idx++;
            sk_idx++;
        }
        ok_idx++;
    }

    std::vector<unsigned char> out_buffer;

    uint16_t v15 = static_cast<uint16_t>(response[15]) |
                   (static_cast<uint16_t>(response[0]) << 8);
    bool parity_odd = (std::bitset<16>(v15).count() % 2 != 0);

    if(parity_odd)
    {
        size_t end = std::min<size_t>(8, response.size());
        out_buffer = std::vector<unsigned char>(response.begin(), response.begin() + end);

        if(response.size() > 14)
        {
            unsigned char idx = response[14] & 0x07;
            if((idx + 8) < (unsigned char)response.size())
            {
                out_buffer[idx] ^= response[idx + 8];
            }
        }
    }
    else
    {
        size_t start = std::min<size_t>(8, response.size());
        size_t end = std::min<size_t>(start + 8, response.size());
        out_buffer = std::vector<unsigned char>(response.begin() + start, response.begin() + end);

        if(response.size() > 6)
        {
            unsigned char idx = response[6] & 0x07;
            if(idx < response.size())
            {
                out_buffer[idx] ^= response[idx];
            }
        }
    }

    for(size_t i = 0; i < 8 && i < out_buffer.size(); i++)
    {
        syn_key[i] ^= out_buffer[i];
    }

    return syn_key;
}

AlienwareAW3423DWFController::~AlienwareAW3423DWFController()
{
    if(dev)
    {
        hid_close(dev);
        dev = nullptr;
    }
}

std::string AlienwareAW3423DWFController::GetLocation()
{
    return "HID: " + location;
}

std::string AlienwareAW3423DWFController::GetSerialString()
{
    wchar_t serial[256];
    if(hid_get_serial_number_string(dev, serial, 256) >= 0)
    {
        std::wstring ws(serial);
        return StringUtils::wstring_to_string(ws);
    }
    return "";
}

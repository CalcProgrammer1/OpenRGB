/*---------------------------------------------------------*\
| LenovoUSBController_Gen7_8.cpp                            |
|                                                           |
|   Driver for Lenovo Gen7 and Gen8 devices                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <iomanip>
#include <sstream>
#include "LenovoUSBController_Gen7_8.h"
#include "StringUtils.h"

using namespace std;

LenovoGen7And8USBController::LenovoGen7And8USBController(hid_device* dev_handle, const char* path, uint16_t in_pid, std::string dev_name)
{
    dev                     = dev_handle;
    location                = path;
    pid                     = in_pid;
    name                    = dev_name;
}

LenovoGen7And8USBController::~LenovoGen7And8USBController()
{
    hid_close(dev);
}

uint16_t LenovoGen7And8USBController::getPid()
{
    return pid;
}

string LenovoGen7And8USBController::getName()
{
    return name;
}

string LenovoGen7And8USBController::getLocation()
{
    return location;
}

void LenovoGen7And8USBController::setLedsByGroup(uint8_t profile_id, vector<led_group> led_groups)
{
    uint8_t buffer[PACKET_SIZE];
    memset(buffer, 0x00, PACKET_SIZE);

    size_t i = 0;
    buffer[i++] = REPORT_ID;
    buffer[i++] = SAVE_PROFILE;
    buffer[i++] = 0xC0;
    buffer[i++] = 0x03;
    buffer[i++] = profile_id;
    buffer[i++] = 0x01;
    buffer[i++] = 0x01;

    for(size_t group = 0; group < led_groups.size() && i < PACKET_SIZE - 21; group++)
    {
        buffer[i++] = (uint8_t)group + 1;  //Group index
        buffer[i++] = 0x06;
        buffer[i++] = 0x01;
        buffer[i++] = led_groups[group].mode;
        buffer[i++] = 0x02;
        buffer[i++] = led_groups[group].speed;
        buffer[i++] = 0x03;
        buffer[i++] = led_groups[group].spin;
        buffer[i++] = 0x04;
        buffer[i++] = led_groups[group].direction;
        buffer[i++] = 0x05;
        buffer[i++] = led_groups[group].color_mode;
        buffer[i++] = 0x06;
        buffer[i++] = 0x00;

        buffer[i++] = (uint8_t)led_groups[group].colors.size();
        for(RGBColor c : led_groups[group].colors)
        {
            buffer[i++] = RGBGetRValue(c);
            buffer[i++] = RGBGetGValue(c);
            buffer[i++] = RGBGetBValue(c);
        }

        vector<uint16_t> leds = led_groups[group].leds;
        size_t led_count = min(leds.size(), (PACKET_SIZE - i)/2);
        buffer[i++] = (uint8_t)led_count;
        uint8_t* byte_ptr = reinterpret_cast<uint8_t*>(leds.data());
        std::copy(byte_ptr, byte_ptr + led_count * sizeof(uint16_t), buffer + i);
        i+= led_count * sizeof(uint16_t);
    }

    buffer[2] = (uint8_t)i;

    sendFeatureReport(buffer, PACKET_SIZE);
}

void LenovoGen7And8USBController::setLedsDirectOn(uint8_t profile_id)
{
    uint8_t buffer[PACKET_SIZE];
    memset(buffer, 0x00, PACKET_SIZE);

    size_t i = 0;
    buffer[i++] = REPORT_ID;
    buffer[i++] = SET_DIRECT_MODE;
    buffer[i++] = 0xC0;
    buffer[i++] = 0x03;
    buffer[i++] = 0x01;
    buffer[i++] = profile_id;

    sendFeatureReport(buffer, PACKET_SIZE);
}

void LenovoGen7And8USBController::setLedsDirectOff(uint8_t profile_id)
{
    uint8_t buffer[PACKET_SIZE];
    memset(buffer, 0x00, PACKET_SIZE);

    size_t i = 0;
    buffer[i++] = REPORT_ID;
    buffer[i++] = SET_DIRECT_MODE;
    buffer[i++] = 0xC0;
    buffer[i++] = 0x03;
    buffer[i++] = 0x02;
    buffer[i++] = profile_id;

    sendFeatureReport(buffer, PACKET_SIZE);
}

void LenovoGen7And8USBController::setLedsDirect(std::vector<led> &leds, std::vector<RGBColor> &colors)
{
    uint8_t buffer[PACKET_SIZE];
    memset(buffer, 0x00, PACKET_SIZE);

    size_t i = 0;
    buffer[i++] = REPORT_ID;
    buffer[i++] = DIRECT_MODE;
    buffer[i++] = 0xC0;
    buffer[i++] = 0x03;

    for(size_t index = 0; index < leds.size() && i < PACKET_SIZE; index++)
    {
        buffer[i++] = leds[index].value & 0xFF;
        buffer[i++] = leds[index].value >> 8 & 0xFF;
        buffer[i++] = RGBGetRValue(colors[index]);
        buffer[i++] = RGBGetGValue(colors[index]);
        buffer[i++] = RGBGetBValue(colors[index]);
    }

    sendFeatureReport(buffer, PACKET_SIZE);
}

void LenovoGen7And8USBController::setLedsAllOff(uint8_t profile_id)
{
    uint8_t buffer[PACKET_SIZE] = {REPORT_ID, SAVE_PROFILE, 0xC0, 0x03, profile_id, 0x01, 0x01};

    sendFeatureReport(buffer, PACKET_SIZE);
}

uint8_t LenovoGen7And8USBController::getCurrentProfileId()
{
    uint8_t buffer[PACKET_SIZE] = {REPORT_ID, GET_ACTIVE_PROFILE, 0xC0, 0x03};

    vector<uint8_t> response = getFeatureReport(buffer, PACKET_SIZE);

    return response.size()>4?response[4]:0x01;
}

uint8_t LenovoGen7And8USBController::getCurrentBrightness()
{
    uint8_t buffer[PACKET_SIZE] = {REPORT_ID, GET_BRIGHTNESS, 0xC0, 0x03};

    vector<uint8_t> response = getFeatureReport(buffer, PACKET_SIZE);

    return response.size()>4?response[4]:0x00;
}


void LenovoGen7And8USBController::setBrightness(uint8_t brightness)
{
    uint8_t buffer[PACKET_SIZE] = {REPORT_ID, SET_BRIGHTNESS, 0xC0, 0x03, brightness};

    sendFeatureReport(buffer, PACKET_SIZE);
}

void LenovoGen7And8USBController::switchProfileTo(uint8_t profile_id)
{
    uint8_t buffer[PACKET_SIZE] = {REPORT_ID, SWITCH_PROFILE, 0xC0, 0x03, profile_id};

    sendFeatureReport(buffer, PACKET_SIZE);
}

std::vector<led_group> LenovoGen7And8USBController::getProfileSettings(uint8_t profile_id)
{
    uint8_t buffer[PACKET_SIZE] = {REPORT_ID, GET_PROFILE, 0xC0, 0x03, profile_id};

    vector<uint8_t> response = getFeatureReport(buffer, PACKET_SIZE);

    vector<led_group> groups;

    size_t i = 7;
    while(i < response.size() && response[i] != 0x00){
        i++;

        led_group group;

        /*-----------------*\
        |read group settings|
        \*-----------------*/

        size_t cnt = response[i++];
        for(size_t j = 0; j < cnt && i < response.size(); j++, i+=2)
        {
            switch(response[i])
            {
            case 0x01:
            group.mode = response[i+1];
            break;
            case 0x02:
            group.speed = response[i+1];
            break;
            case 0x03:
            group.spin = response[i+1];
            break;
            case 0x04:
            group.direction = response[i+1];
            break;
            case 0x05:
            group.color_mode = response[i+1];
            break;
            case 0x06:
            //group.mode = response[i+1];
            break;
            }
        }

        /*-----------------*\
        |read group colors  |
        \*-----------------*/

        cnt = response[i++];
        for(size_t j = 0; j < cnt && i < response.size(); j++, i+=3)
        {
            group.colors.push_back(ToRGBColor(response[i],response[i+1],response[i+2]));
        }

        /*-----------------*\
        |read group LEDs    |
        \*-----------------*/

        cnt = response[i++];
        for(size_t j = 0; j < cnt && i < response.size(); j++, i+=2)
        {
            group.leds.push_back(response[i] | response[i+1] << 8);
        }

        groups.push_back(group);
    }

   return groups;

}

void LenovoGen7And8USBController::sendFeatureReport(uint8_t packet[], size_t packet_size)
{
    hid_send_feature_report(dev, packet, packet_size);
    LOG_TRACE("[Lenovo Gen 7 Controller] Buffer: %s", ConvertBytesToHex(packet, packet_size).c_str());
}

std::vector<uint8_t> LenovoGen7And8USBController::getFeatureReport(uint8_t packet[], size_t packet_size)
{
    sendFeatureReport(packet, packet_size);

    uint8_t read_buffer[PACKET_SIZE] = {REPORT_ID};
    int num_bytes = 0;
    num_bytes = hid_get_feature_report(dev, read_buffer, sizeof(read_buffer));

    vector<uint8_t> response = {};
    if(num_bytes > 0)
    {
        response.insert(response.begin(), read_buffer, read_buffer + num_bytes);
    }

    LOG_TRACE("[Lenovo Gen 7 Controller] Read Buffer: %s", ConvertBytesToHex(response).c_str());
    return response;
}

std::string LenovoGen7And8USBController::ConvertBytesToHex(uint8_t packet[], size_t packet_size)
{
    return ConvertBytesToHex(std::vector<uint8_t>(packet, packet + packet_size));
}

std::string LenovoGen7And8USBController::ConvertBytesToHex(const std::vector<uint8_t> &input)
{
    std::ostringstream temp_stream;
    for(const uint8_t &oneInputByte : input)
    {
        temp_stream << (temp_stream.tellp()==0 ? "" : " ") << std::setw(2) << std::setfill('0') << std::hex << (int)oneInputByte;
    }
    return temp_stream.str();
}

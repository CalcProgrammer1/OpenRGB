/*---------------------------------------------------------*\
| KasaSmartController.h                                     |
|                                                           |
|   Driver for Kasa Smart bulbs                             |
|                                                           |
|   Devin Wendt (umbreon222@gmail.com)          16 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <string>
#include <thread>
#include <vector>
#include "RGBController.h"
#include "net_port.h"

enum
{
    KASA_SMART_MODE_DIRECT  = 0x00,
    KASA_SMART_MODE_OFF     = 0x01,
    KASA_SMART_MODE_RAINBOW = 0x02
};

enum
{
    KASA_SMART_TYPE_KL420           = 0x00,
    KASA_SMART_TYPE_OTHER_LEDSTRIP  = 0x01,
    KASA_SMART_TYPE_LIGHT           = 0x02
};

#define KASA_SMART_INITIALIZATION_VECTOR 0xAB
#define KASA_SMART_RECEIVE_BUFFER_SIZE 4096
#define KASA_SMART_MAX_CONNECTION_ATTEMPTS 3

/*-------------------------*\
| Kasa Smart Light Commands |
\*-------------------------*/
#define KASA_SMART_SYSTEM_INFO_QUERY "{\"system\": {\"get_sysinfo\": {}}}"
#define KASA_SMART_LIGHT_OFF_COMMAND "{\"smartlife.iot.smartbulb.lightingservice\": {\"transition_light_state\": {\"transition_period\": 0, \"on_off\":0, \"mode\":\"normal\"}}}"
const char KASA_SMART_LIGHT_SET_LIGHT_STATE_COMMAND_FORMAT[] = "{\"smartlife.iot.smartbulb.lightingservice\": {\"transition_light_state\": {\"transition_period\": 0, \"on_off\""
                                                         ":1, \"mode\":\"normal\", \"hue\": %u, \"saturation\": %u, \"brightness\": %u, \"color_temp\": 0}}}";
#define KASA_SMART_LEDSTRIP_OFF_COMMAND "{\"smartlife.iot.lightStrip\": {\"set_light_state\": {\"transition\": 0, \"on_off\":0, \"mode\":\"normal\"}}}"
const char KASA_SMART_LEDSTRIP_SET_LIGHT_STATE_COMMAND_FORMAT[] = "{\"smartlife.iot.lightStrip\": {\"set_light_state\": {\"transition\": 0, \"on_off\""
                                                         ":1, \"mode\":\"normal\", \"hue\": %u, \"saturation\": %u, \"brightness\": %u, \"color_temp\": 0}}}";
#define KASA_SMART_EFFECT_RAINBOW_COMMAND "{\"smartlife.iot.lighting_effect\":{\"set_lighting_effect\":{\"custom\":0,\"direction\":1,\"duration\":0,\"enable\":1,\"expansion_strategy\":1,\"name\":\"Rainbow\",\"repeat_times\":0,\"segments\":[0],\"sequence\":[[0,100,100],[100,100,100],[200,100,100],[300,100,100]],\"spread\":12,\"transition\":1500,\"type\":\"sequence\"}}}}"


class KasaSmartController
{
public:
    KasaSmartController(std::string ipAddress, std::string name);
    ~KasaSmartController();

    std::string GetLocation();
    std::string GetName();
    std::string GetVersion();
    std::string GetManufacturer();
    std::string GetUniqueID();
    int GetKasaType();

    bool Initialize();
    void SetColor(unsigned char red, unsigned char green, unsigned char blue, int device_type);
    void SetEffect(std::string effect);
    void TurnOff(int device_type);

private:
    net_port port;
    std::string name;
    bool is_initialized;
    unsigned int retry_count;
    std::string firmware_version;
    std::string module_name;
    std::string device_id;
    std::string location;
    int kasa_type;
    bool SendCommand(std::string command, std::string &response);
    static unsigned char* Encrypt(const std::string request);
    static std::string Decrypt(const unsigned char*, int length, std::string &response);
    static void XorPayload(unsigned char* encrypted, int length);
    static void XorEncryptedPayload(unsigned char* encrypted, int length);
};

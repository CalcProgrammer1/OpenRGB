/*---------------------------------------------------------*\
|  Definitions for Kasa Smart                               |
|                                                           |
|  Devin Wendt (umbreon222@gmail.com), 2/16/2023            |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include "net_port.h"

#include <string>
#include <thread>
#include <vector>

#pragma once

enum
{
    KASA_SMART_MODE_DIRECT = 0x00,
    KASA_SMART_MODE_OFF    = 0x01
};

#define KASA_SMART_INITIALIZATION_VECTOR 0xAB
#define KASA_SMART_RECEIVE_BUFFER_SIZE 4096
#define KASA_SMART_MAX_CONNECTION_ATTEMPTS 3

/*-------------------------*\
| Kasa Smart Light Commands |
\*-------------------------*/
#define KASA_SMART_SYSTEM_INFO_QUERY "{\"system\": {\"get_sysinfo\": {}}}"
#define KASA_SMART_OFF_COMMAND "{\"smartlife.iot.smartbulb.lightingservice\": {\"transition_light_state\": {\"transition_period\": 0, \"on_off\":0, \"mode\":\"normal\"}}}"
const char KASA_SMART_SET_LIGHT_STATE_COMMAND_FORMAT[] = "{\"smartlife.iot.smartbulb.lightingservice\": {\"transition_light_state\": {\"transition_period\": 0, \"on_off\""
                                                         ":1, \"mode\":\"normal\", \"hue\": %u, \"saturation\": %u, \"brightness\": %u, \"color_temp\": 0}}}";

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

    bool Initialize();
    void SetColor(unsigned char red, unsigned char green, unsigned char blue);
    void TurnOff();

private:
    net_port port;
    std::string name;
    bool is_initialized;
    unsigned int retry_count;
    std::string firmware_version;
    std::string module_name;
    std::string device_id;
    std::string location;
    bool SendCommand(std::string command, std::string &response);
    static unsigned char* Encrypt(const std::string request);
    static std::string Decrypt(const unsigned char*, int length, std::string &response);
    static void XorPayload(unsigned char* encrypted, int length);
    static void XorEncryptedPayload(unsigned char* encrypted, int length);
};

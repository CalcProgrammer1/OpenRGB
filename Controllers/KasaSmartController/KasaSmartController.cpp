/*---------------------------------------------------------*\
| KasaSmartController.cpp                                   |
|                                                           |
|   Driver for Kasa Smart bulbs                             |
|                                                           |
|   Devin Wendt (umbreon222@gmail.com)          16 Feb 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstdint>
#include "KasaSmartController.h"
#include <nlohmann/json.hpp>
#include "hsv.h"

using json = nlohmann::json;

KasaSmartController::KasaSmartController(std::string ipAddress, std::string name)
{
    this->name = name;

    /*------------------------------------------------*\
    | Fill in location string with device's IP address |
    \*------------------------------------------------*/
    location = "IP: " + ipAddress;

    /*---------------------------------------------------------*\
    | Create a TCP client sending to the device's IP, port 9999 |
    \*---------------------------------------------------------*/
    port.tcp_client(ipAddress.c_str(), "9999");
}

bool KasaSmartController::Initialize()
{
    is_initialized = false;
    retry_count = 0;

    /*--------------*\
    | Try to connect |
    \*--------------*/
    while(!port.connected && !port.tcp_client_connect() && retry_count < KASA_SMART_MAX_CONNECTION_ATTEMPTS)
    {
        ++retry_count;
    }
    if(!port.connected)
    {
        /*----------------*\
        | Couldn't connect |
        \*----------------*/
        return is_initialized;
    }

    const std::string system_info_query(KASA_SMART_SYSTEM_INFO_QUERY);
    std::string system_info_json;
    bool command_sent = KasaSmartController::SendCommand(system_info_query, system_info_json);
    port.tcp_close();
    if(!command_sent || system_info_json.empty())
    {
        /*---------------------------------------*\
        | Send command failed or no data returned |
        \*---------------------------------------*/
        return is_initialized;
    }

    json system_information;
    try
    {
        system_information = json::parse(system_info_json);
    }
    catch (json::parse_error&)
    {
        /*-----------------------*\
        | Can't parse system info |
        \*-----------------------*/
        return is_initialized;
    }

    std::string device_type;
    if(system_information["system"]["get_sysinfo"].contains("type"))
    {
        device_type = system_information["system"]["get_sysinfo"]["type"];
    }
    else if(system_information["system"]["get_sysinfo"].contains("mic_type"))
    {
        device_type = system_information["system"]["get_sysinfo"]["mic_type"];
    }
    else
    {
        /*----------------------*\
        | Can't find device type |
        \*----------------------*/
        return is_initialized;
    }

    std::transform(device_type.begin(), device_type.end(), device_type.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    if(device_type.find("smartbulb") == std::string::npos)
    {
        /*----------------------------*\
        | Device type not a smart bulb |
        \*----------------------------*/
        return is_initialized;
    }

    if(system_information["system"]["get_sysinfo"].contains("is_color") && system_information["system"]["get_sysinfo"]["is_color"] != 1)
    {
        /*--------------------------------*\
        | Smart bulb doesn't support color |
        \*--------------------------------*/
        return is_initialized;
    }

    std::string model;
    if(system_information["system"]["get_sysinfo"].contains("model"))
    {
        model = system_information["system"]["get_sysinfo"]["model"];
    }
    else
    {
        /*-----------------------*\
        | Can't find device model |
        \*-----------------------*/
        return is_initialized;
    }

    if(model.find("KL420") != std::string::npos)
    {
        kasa_type = KASA_SMART_TYPE_KL420;
    }
    else if(model.find("KL4") != std::string::npos)
    {
        kasa_type = KASA_SMART_TYPE_OTHER_LEDSTRIP;
    }
    else
    {
        kasa_type = KASA_SMART_TYPE_LIGHT;
    }

    firmware_version = system_information["system"]["get_sysinfo"]["sw_ver"];
    module_name      = system_information["system"]["get_sysinfo"]["model"];
    device_id        = system_information["system"]["get_sysinfo"]["deviceId"];

    is_initialized = true;
    return is_initialized;
}

KasaSmartController::~KasaSmartController()
{
    if(port.connected)
    {
        port.tcp_close();
    }
}

std::string KasaSmartController::GetLocation()
{
    return(location);
}

std::string KasaSmartController::GetName()
{
    return(name);
}

std::string KasaSmartController::GetVersion()
{
    return(module_name + " " + firmware_version);
}

std::string KasaSmartController::GetManufacturer()
{
    return("Kasa Smart");
}

std::string KasaSmartController::GetUniqueID()
{
    return(device_id);
}

int KasaSmartController::GetKasaType()
{
    return(kasa_type);
}

void KasaSmartController::SetColor(unsigned char red, unsigned char green, unsigned char blue, int device_type)
{
    if(!is_initialized)
    {
        return;
    }

    RGBColor color = ToRGBColor(red, green, blue);
    hsv_t hsv;
    rgb2hsv(color, &hsv);

    /*------------------------------------------*\
    | Normalize case where hue is "-1" undefined |
    \*------------------------------------------*/
    unsigned int normalized_hue = hsv.hue;
    if(hsv.hue == (unsigned int)-1)
    {
        normalized_hue = 0;
    }
    /*--------------------------------------------------*\
    | Kasa smart lights take values out of 100 for these |
    \*--------------------------------------------------*/
    unsigned int normalized_saturation = hsv.saturation * 100 / 255;
    unsigned int normalized_value      = hsv.value * 100 / 255;


    /*-------------------*\
    | Open TCP connection |
    \*-------------------*/
    if(!port.connected && !port.tcp_client_connect() && ++retry_count >= KASA_SMART_MAX_CONNECTION_ATTEMPTS)
    {
        is_initialized = false;
        return;
    }

    /*----------------------------*\
    | Hack to handle/emulate black |
    \*----------------------------*/
    if(normalized_saturation == 0 && normalized_value == 0)
    {
        TurnOff(device_type);
        return;
    }

    /*------------------------------*\
    | Format set light state command |
    \*------------------------------*/
    std::string set_lightstate_command_format;
    if(device_type == DEVICE_TYPE_LIGHT)
    {
        set_lightstate_command_format = KASA_SMART_LIGHT_SET_LIGHT_STATE_COMMAND_FORMAT;
    }
    else if(device_type == DEVICE_TYPE_LEDSTRIP)
    {
        set_lightstate_command_format = KASA_SMART_LEDSTRIP_SET_LIGHT_STATE_COMMAND_FORMAT;
    }
    int size = std::snprintf(nullptr, 0, set_lightstate_command_format.c_str(), normalized_hue, normalized_saturation, normalized_value) + 1;
    if(size <= 0)
    {
        port.tcp_close();
        return;
    }
    char* buf = new char[size];
    std::snprintf(buf, size, set_lightstate_command_format.c_str(), normalized_hue, normalized_saturation, normalized_value);
    std::string set_lightstate_command(buf, buf + size - 1);
    delete[] buf;

    /*-----------------------------*\
    | Send command, ignore response |
    \*-----------------------------*/
    std::string response;
    KasaSmartController::SendCommand(set_lightstate_command, response);
    port.tcp_close();
}

void KasaSmartController::SetEffect(std::string effect)
{
    if(!is_initialized)
    {
        return;
    }

    /*-------------------*\
    | Open TCP connection |
    \*-------------------*/
    if(!port.connected && !port.tcp_client_connect() && ++retry_count >= KASA_SMART_MAX_CONNECTION_ATTEMPTS)
    {
        is_initialized = false;
        return;
    }

    std::string response;
    KasaSmartController::SendCommand(effect, response);
    port.tcp_close();
}

void KasaSmartController::TurnOff(int device_type)
{
    if(!is_initialized)
    {
        return;
    }

    std::string turn_off_command;
    if(device_type == DEVICE_TYPE_LIGHT)
    {
        turn_off_command = KASA_SMART_LIGHT_OFF_COMMAND;
    }
    else if(device_type == DEVICE_TYPE_LEDSTRIP)
    {
        turn_off_command = KASA_SMART_LEDSTRIP_OFF_COMMAND;
    }

    if(!port.connected && !port.tcp_client_connect() && ++retry_count >= KASA_SMART_MAX_CONNECTION_ATTEMPTS)
    {
        is_initialized = false;
        return;
    }
    std::string response;
    KasaSmartController::SendCommand(turn_off_command, response);
    port.tcp_close();
}

bool KasaSmartController::SendCommand(std::string command, std::string &response)
{
    const unsigned char* encrypted_payload = KasaSmartController::Encrypt(command);
    port.tcp_client_write((char*)encrypted_payload, (int)(command.length() + sizeof(unsigned long)));
    delete[] encrypted_payload;

    unsigned char* receive_buffer = new unsigned char[KASA_SMART_RECEIVE_BUFFER_SIZE];
    int response_length = port.tcp_listen((char*)receive_buffer, KASA_SMART_RECEIVE_BUFFER_SIZE);
    if(response_length > KASA_SMART_RECEIVE_BUFFER_SIZE || response_length <= 0) {
        /*-------------------------------------------------------------*\
        | Small fail safes to prevent decrypting bad or empty responses |
        \*-------------------------------------------------------------*/
        return false;
    }

    unsigned long received_length = response_length;
    unsigned long response_full_length = 0;
    if(response_length > 0)
    {
        response_full_length = ntohl(*(uint32_t*)receive_buffer);
    }

    if(response_full_length > KASA_SMART_RECEIVE_BUFFER_SIZE) {
        return false;
    }

    /*--------------------------*\
    | Fetch entirety of response |
    \*--------------------------*/
    while(received_length < response_full_length)
    {
        received_length += port.tcp_listen((char*)receive_buffer + received_length, KASA_SMART_RECEIVE_BUFFER_SIZE - received_length);
    }

    if(received_length > 0)
    {
        /*------------------------------------------------*\
        | Decrypt payload data preceeding the payload size |
        \*------------------------------------------------*/
        KasaSmartController::Decrypt(receive_buffer + sizeof(uint32_t), received_length - sizeof(uint32_t), response);
    }
    delete[] receive_buffer;
    return true;
}

unsigned char* KasaSmartController::Encrypt(const std::string request)
{
    /*----------------------------------------------------------------*\
    | "Encrypted" payload consists of size as a uint32 + XOR'd payload |
    \*----------------------------------------------------------------*/
    uint32_t size          = htonl((uint32_t)request.length());
    int payload_size       = (int)(request.length() + sizeof(size));
    unsigned char* payload = new unsigned char[payload_size];
    memcpy(payload, &size, sizeof(size));
    unsigned char* request_data = new unsigned char[request.length()];
    memcpy(request_data, request.data(), request.length());
    KasaSmartController::XorPayload(request_data, (int)request.length());
    memcpy(payload + sizeof(size), request_data, request.length());
    delete[] request_data;
    return payload;
}

std::string KasaSmartController::Decrypt(const unsigned char* encrypted, int length, std::string &response)
{
    unsigned char* temp_encrypted = new unsigned char[length];
    memcpy(temp_encrypted, encrypted, length);
    KasaSmartController::XorEncryptedPayload(temp_encrypted, length);
    for(int i = 0; i < length; ++i)
    {
        response += temp_encrypted[i];
    }
    delete[] temp_encrypted;
    return response;
}

void KasaSmartController::XorPayload(unsigned char* encrypted, int length)
{
    unsigned char key = KASA_SMART_INITIALIZATION_VECTOR;
    for(int i = 0; i < length; ++i)
    {
        key ^= encrypted[i];
        encrypted[i] = key;
    }
}

void KasaSmartController::XorEncryptedPayload(unsigned char* encrypted, int length)
{
    unsigned char key = KASA_SMART_INITIALIZATION_VECTOR;
    for(int i = 0; i < length; ++i)
    {
        unsigned char plain_byte = key ^ encrypted[i];
        key = encrypted[i];
        encrypted[i] = plain_byte;
    }
}

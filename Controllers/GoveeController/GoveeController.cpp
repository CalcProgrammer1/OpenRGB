/*---------------------------------------------------------*\
| GoveeController.cpp                                       |
|                                                           |
|   Driver for Govee wireless lighting devices              |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      01 Dec 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <exception>
#include <nlohmann/json.hpp>
#include "base64.hpp"
#include "GoveeController.h"
#include "JsonUtils.h"
#include "LogManager.h"

using json = nlohmann::json;
using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| Port the Govee LAN API listens on for scan requests       |
\*---------------------------------------------------------*/
#define GOVEE_SCAN_PORT                             "4001"

base64::byte CalculateXorChecksum(std::vector<base64::byte> packet)
{
    base64::byte checksum = 0;

    for(unsigned int i = 0; i < packet.size(); i++)
    {
        checksum ^= packet[i];
    }

    return(checksum);
}

GoveeController::GoveeController(std::string ip)
{
    /*-----------------------------------------------------*\
    | Fill in location string with device's IP address      |
    \*-----------------------------------------------------*/
    ip_address  = ip;

    broadcast_received      = false;
    razer_status_received   = false;
    razer_status_supported  = false;
    razer_initially_armed   = false;
    device_status_received  = false;
    initial_power           = false;
    initial_brightness      = 100;
    initial_color           = 0;
    port_initialized        = false;

    /*-----------------------------------------------------*\
    | Register callback for receiving broadcasts            |
    \*-----------------------------------------------------*/
    RegisterReceiveBroadcastCallback(this);

    /*-----------------------------------------------------*\
    | Request device information                            |
    \*-----------------------------------------------------*/
    SendScan();

    /*-----------------------------------------------------*\
    | Wait up to 5s for device information to be received   |
    \*-----------------------------------------------------*/
    for(unsigned int wait_count = 0; broadcast_initialized && wait_count < 500; wait_count++)
    {
        if(broadcast_received)
        {
            break;
        }

        std::this_thread::sleep_for(10ms);
    }

    if(!broadcast_received.load())
    {
        LOG_WARNING("[Govee] No scan response from %s; check LAN Control and UDP port 4002", ip_address.c_str());
    }

    /*-----------------------------------------------------*\
    | Query read-only capabilities and initial light state. |
    | B2 support proves a status endpoint, not LED count.   |
    \*-----------------------------------------------------*/
    if(broadcast_initialized)
    {
        QueryStatus("status");
        for(unsigned int wait_count = 0; wait_count < 75 && !razer_status_received.load(); wait_count++)
        {
            std::this_thread::sleep_for(10ms);
        }

        QueryStatus("devStatus");
        for(unsigned int wait_count = 0; wait_count < 75 && !device_status_received.load(); wait_count++)
        {
            std::this_thread::sleep_for(10ms);
        }
    }

    if(razer_status_supported.load())
    {
        LOG_INFO("[Govee] Valid Razer B2 status response from %s", ip_address.c_str());
    }

    /*-----------------------------------------------------*\
    | Open a UDP client sending to the Govee device IP,     |
    | port 4003                                             |
    \*-----------------------------------------------------*/
    port_initialized = port.udp_client(ip_address.c_str(), "4003");
    if(!port_initialized)
    {
        LOG_WARNING("[Govee] Could not create UDP client for %s", ip_address.c_str());
    }
}

GoveeController::~GoveeController()
{
    UnregisterReceiveBroadcastCallback(this);
    if(port_initialized)
    {
        port.tcp_close();
    }
}

std::string GoveeController::GetLocation()
{
    return("IP: " + ip_address);
}

std::string GoveeController::GetSku()
{
    return(sku);
}

std::string GoveeController::GetVersion()
{
    return("BLE Hardware Version: "  + bleVersionHard  + "\r\n" +
           "BLE Software Version: "  + bleVersionSoft  + "\r\n" +
           "WiFi Hardware Version: " + wifiVersionHard + "\r\n" +
           "WiFi Software Version: " + wifiVersionSoft + "\r\n");
}

bool GoveeController::SupportsRazerStatus() const
{
    return(razer_status_supported.load());
}

bool GoveeController::RazerInitiallyArmed() const
{
    return(razer_status_supported.load() && razer_initially_armed.load());
}

std::string GoveeController::GetDeviceId() const
{
    return(module_mac);
}

bool GoveeController::GetInitialState(bool& on, unsigned int& brightness, RGBColor& color) const
{
    if(!device_status_received.load())
    {
        return(false);
    }

    on         = initial_power;
    brightness = initial_brightness;
    color      = initial_color;
    return(true);
}

static std::string GetGoveeStringField(const json& data, const char* key)
{
    if(data.contains(key) && data[key].is_string())
    {
        return(data[key].get<std::string>());
    }

    return("");
}

void GoveeController::ReceiveBroadcast(char* recv_buf, int size, const std::string& source_ip)
{
    if((size <= 0) || (source_ip != ip_address))
    {
        return;
    }

    while((size > 0) && (recv_buf[size - 1] == '\0'))
    {
        size--;
    }

    json response;
    if((size == 0) ||
       !JsonUtils::JsonParse(std::string(recv_buf, size), response) ||
       !response.is_object() ||
       !response.contains("msg") ||
       !response["msg"].is_object())
    {
        return;
    }

    const json& msg = response["msg"];
    if(!msg.contains("cmd") ||
       !msg["cmd"].is_string() ||
       !msg.contains("data") ||
       !msg["data"].is_object())
    {
        return;
    }

    const json& data = msg["data"];
    const std::string command = msg["cmd"].get<std::string>();

    if((command == "scan") && !broadcast_received.load())
    {
        if(!data.contains("ip") || !data["ip"].is_string() || data["ip"] != ip_address)
        {
            return;
        }

        sku             = GetGoveeStringField(data, "sku");
        module_mac      = GetGoveeStringField(data, "device");
        bleVersionHard  = GetGoveeStringField(data, "bleVersionHard");
        bleVersionSoft  = GetGoveeStringField(data, "bleVersionSoft");
        wifiVersionHard = GetGoveeStringField(data, "wifiVersionHard");
        wifiVersionSoft = GetGoveeStringField(data, "wifiVersionSoft");
        broadcast_received.store(true);
    }
    else if((command == "status") && !razer_status_received.load())
    {
        if(data.contains("pt") && data["pt"].is_string())
        {
            try
            {
                const std::vector<base64::byte> pt = base64::decode(data["pt"].get<std::string>());
                if((pt.size() == 6) &&
                   (pt[0] == 0xBB) &&
                   (pt[1] == 0x00) &&
                   (pt[2] == 0x01) &&
                   (pt[3] == 0xB2) &&
                   (pt[4] <= 1))
                {
                    base64::byte checksum = 0;
                    for(unsigned int idx = 0; idx < 5; idx++)
                    {
                        checksum ^= pt[idx];
                    }
                    const bool valid = (checksum == pt[5]);
                    razer_initially_armed.store(valid && (pt[4] == 1));
                    razer_status_supported.store(valid);
                }
            }
            catch(const std::exception&)
            {
            }
        }
        razer_status_received.store(true);
    }
    else if((command == "devStatus") && !device_status_received.load())
    {
        if(data.contains("onOff") &&
           data["onOff"].is_number_integer() &&
           data.contains("brightness") &&
           data["brightness"].is_number_integer() &&
           data.contains("color") &&
           data["color"].is_object())
        {
            const json& rgb = data["color"];
            if(rgb.contains("r") &&
               rgb["r"].is_number_integer() &&
               rgb.contains("g") &&
               rgb["g"].is_number_integer() &&
               rgb.contains("b") &&
               rgb["b"].is_number_integer())
            {
                try
                {
                    // Read wide integers before narrowing, so unexpected
                    // 64-bit values cannot wrap into seemingly valid RGB data.
                    const long long r = rgb["r"].get<long long>();
                    const long long g = rgb["g"].get<long long>();
                    const long long b = rgb["b"].get<long long>();
                    const long long brightness = data["brightness"].get<long long>();
                    const long long on_off = data["onOff"].get<long long>();
                    if((r >= 0) && (r <= 255) &&
                       (g >= 0) && (g <= 255) &&
                       (b >= 0) && (b <= 255) &&
                       (brightness >= 0) && (brightness <= 100) &&
                       (on_off >= 0) && (on_off <= 1))
                    {
                        initial_power      = (on_off != 0);
                        initial_brightness = (unsigned int)brightness;
                        initial_color      = ToRGBColor((unsigned char)r,
                                                        (unsigned char)g,
                                                        (unsigned char)b);
                        device_status_received.store(true);
                    }
                }
                catch(const std::exception&)
                {
                    // Ignore invalid or out-of-range status values; allow another response.
                }
            }
        }
    }
}

void GoveeController::SetColor(unsigned char red, unsigned char green, unsigned char blue)
{
    json command;

    command["msg"]["cmd"]                       = "colorwc";
    command["msg"]["data"]["color"]["r"]        = red;
    command["msg"]["data"]["color"]["g"]        = green;
    command["msg"]["data"]["color"]["b"]        = blue;
    command["msg"]["data"]["colorTemInKelvin"]  = 0;

    /*-----------------------------------------------------*\
    | Convert the JSON object to a string and write it      |
    \*-----------------------------------------------------*/
    std::string command_str                     = command.dump();

    SendCommand(command_str);
}

void GoveeController::SendRazerData(RGBColor* colors, unsigned int size, bool gradient)
{
    /*-----------------------------------------------------*\
    | Do not send an empty frame (this was producing        |
    | length=2, count=0)                                    |
    \*-----------------------------------------------------*/
    if(size == 0)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | PT payload: BB [len_hi] [len_lo] B0 [gradient=0]      |
    | [led_count] (RGB * N) [xor]                           |
    | length = 2 + 3*N  (bytes after 0xB0: gradient +       |
    | led_count + RGB*count)                                |
    \*-----------------------------------------------------*/
    const unsigned int count = std::min(size, 255u);
    const unsigned int payload_len = 2 + (3 * count);

    /*-----------------------------------------------------*\
    | Create buffer with fixed size and fill sequentially   |
    \*-----------------------------------------------------*/

    std::vector<base64::byte> pkt;
    pkt.reserve(7 + (3 * count));

    pkt.push_back(0xBB);
    pkt.push_back((base64::byte)((payload_len >> 8) & 0xFF)); /* len_hi */
    pkt.push_back((base64::byte)(payload_len & 0xFF));        /* len_lo */
    pkt.push_back(0xB0);                                     /* subcommand */
    pkt.push_back(gradient ? 0x01 : 0x00);                    /* gradient */
    pkt.push_back((base64::byte)count);                       /* led_count */

    for(std::size_t led_idx = 0; led_idx < count; led_idx++)
    {
        pkt.push_back(RGBGetRValue(colors[led_idx]));
        pkt.push_back(RGBGetGValue(colors[led_idx]));
        pkt.push_back(RGBGetBValue(colors[led_idx]));
    }

    pkt.push_back(CalculateXorChecksum(pkt));

    json command;
    command["msg"]["cmd"]                       = "razer";
    command["msg"]["data"]["pt"]                = base64::encode(pkt);

    /*-----------------------------------------------------*\
    | Convert the JSON object to a string and write it      |
    \*-----------------------------------------------------*/
    std::string command_str                     = command.dump();

    SendCommand(command_str);
}

void GoveeController::SendRazerDisable()
{
    const std::vector<base64::byte> pkt = { 0xBB, 0x00, 0x01, 0xB1, 0x00, 0x0B };
    json command;

    command["msg"]["cmd"]                       = "razer";
    command["msg"]["data"]["pt"]                = base64::encode(pkt);

    /*-----------------------------------------------------*\
    | Convert the JSON object to a string and write it      |
    \*-----------------------------------------------------*/
    std::string command_str                     = command.dump();

    SendCommand(command_str);
}

void GoveeController::SendRazerEnable()
{
    const std::vector<base64::byte> pkt = { 0xBB, 0x00, 0x01, 0xB1, 0x01, 0x0A };
    json command;

    command["msg"]["cmd"]                       = "razer";
    command["msg"]["data"]["pt"]                = base64::encode(pkt);

    /*-----------------------------------------------------*\
    | Convert the JSON object to a string and write it      |
    \*-----------------------------------------------------*/
    std::string command_str                     = command.dump();

    SendCommand(command_str);
}

void GoveeController::SetBrightness(unsigned int brightness)
{
    json command;

    command["msg"]["cmd"]                       = "brightness";
    command["msg"]["data"]["value"]             = std::max(1u, std::min(brightness, 100u));

    /*-----------------------------------------------------*\
    | Convert the JSON object to a string and write it      |
    \*-----------------------------------------------------*/
    std::string command_str                     = command.dump();

    SendCommand(command_str);
}

void GoveeController::SetPower(bool enabled)
{
    json command;

    command["msg"]["cmd"]                       = "turn";
    command["msg"]["data"]["value"]             = enabled ? 1 : 0;

    std::string command_str                     = command.dump();

    SendCommand(command_str);
}

void GoveeController::SendScan()
{
    if(!broadcast_initialized)
    {
        return;
    }

    json command;

    command["msg"]["cmd"]                       = "scan";
    command["msg"]["data"]["account_topic"]     = "reserve";

    /*-----------------------------------------------------*\
    | Send the documented multicast scan and a unicast      |
    | scan to the manually configured IP.  Neither assumes  |
    | a particular subnet mask.                             |
    \*-----------------------------------------------------*/
    std::string command_str                     = command.dump();

    int multicast_result = broadcast_port.udp_write((char *)command_str.c_str(), (int)command_str.length());
    int unicast_result   = broadcast_port.udp_write_to(ip_address.c_str(), GOVEE_SCAN_PORT, (char *)command_str.c_str(), (int)command_str.length());

    if((multicast_result < 0) && (unicast_result < 0))
    {
        LOG_WARNING("[Govee] Could not send LAN scan to %s", ip_address.c_str());
    }
}

void GoveeController::QueryStatus(const char* command_name)
{
    json command;
    command["msg"]["cmd"]  = command_name;
    command["msg"]["data"] = json::object();

    const std::string payload = command.dump();
    if(broadcast_port.udp_write_to(ip_address.c_str(), "4003", (char*)payload.c_str(), (int)payload.length()) < 0)
    {
        LOG_WARNING("[Govee] Failed to query %s from %s", command_name, ip_address.c_str());
    }
}

void GoveeController::SendCommand(const std::string& command)
{
    std::lock_guard<std::mutex> lock(command_mutex);

    if(!port_initialized)
    {
        LOG_WARNING("[Govee] UDP client is not initialized for %s", ip_address.c_str());
        return;
    }

    const int sent = port.udp_write((char *)command.c_str(), (int)command.length());
    if(sent != (int)command.length())
    {
        LOG_WARNING("[Govee] Could not send complete LAN command to %s", ip_address.c_str());
    }
}

/*---------------------------------------------------------*\
| Static class members for shared broadcast receiver        |
\*---------------------------------------------------------*/
net_port                        GoveeController::broadcast_port;
bool                            GoveeController::broadcast_initialized = false;
std::vector<GoveeController*>   GoveeController::callbacks;
std::mutex                      GoveeController::callbacks_mutex;
std::thread*                    GoveeController::ReceiveThread;
std::atomic<bool>               GoveeController::ReceiveThreadRun;

void GoveeController::ReceiveBroadcastThreadFunction()
{
    char recv_buf[1024];

    broadcast_port.set_receive_timeout(1, 0);

    while(ReceiveThreadRun.load())
    {
        /*-------------------------------------------------*\
        | Receive up to 1024 bytes from the device with a   |
        | 1s timeout                                        |
        \*-------------------------------------------------*/
        sockaddr_in source = {};
#ifdef _WIN32
        int source_len = sizeof(source);
#else
        socklen_t source_len = sizeof(source);
#endif
        int size = recvfrom(broadcast_port.sock, recv_buf, sizeof(recv_buf), 0,
                            (sockaddr*)&source, &source_len);

        if(size > 0)
        {
            /*-------------------------------------------------*\
            | Each controller validates the response IP.        |
            | Hold this lock to prevent callback destruction.   |
            \*-------------------------------------------------*/
            std::lock_guard<std::mutex> lock(callbacks_mutex);

            for(unsigned int callback_idx = 0; callback_idx < callbacks.size(); callback_idx++)
            {
                callbacks[callback_idx]->ReceiveBroadcast(recv_buf, size, inet_ntoa(source.sin_addr));
            }
        }
    }
}

void GoveeController::RegisterReceiveBroadcastCallback(GoveeController* controller_ptr)
{
    std::lock_guard<std::mutex> lock(callbacks_mutex);
    callbacks.push_back(controller_ptr);
}

void GoveeController::UnregisterReceiveBroadcastCallback(GoveeController* controller_ptr)
{
    std::lock_guard<std::mutex> lock(callbacks_mutex);
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), controller_ptr), callbacks.end());
}

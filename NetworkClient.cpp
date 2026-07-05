/*---------------------------------------------------------*\
| NetworkClient.cpp                                         |
|                                                           |
|   OpenRGB SDK network client                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "NetworkClient.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "RGBController_Network.h"
#include "StringUtils.h"

#ifdef _WIN32
#include <Windows.h>
#define MSG_NOSIGNAL 0
#endif

#ifdef __APPLE__
#include <unistd.h>
#endif

#ifdef __linux__
#include <unistd.h>
#include <sys/select.h>
#include <netinet/tcp.h>
#endif

#ifdef __linux__
const int yes = 1;
#else
const char yes = 1;
#endif

using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| Macros for copying data fields from set descriptor buffer |
| while ensuring we don't access out of bounds              |
\*---------------------------------------------------------*/
#define COPY_DATA_FIELD(data_ptr, data_start, field)                                \
    if((unsigned)(data_ptr + sizeof(field) - data_start) <= (unsigned)data_size)    \
    {                                                                               \
        memcpy(&field, data_ptr, sizeof(field));                                    \
        data_ptr += sizeof(field);                                                  \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        goto COPY_DATA_ERROR;                                                       \
    }                                                                               \

#define COPY_STRING_FIELD(data_ptr, data_start, length, field)                      \
    if((unsigned)(data_ptr + length - data_start) <= (unsigned)data_size)           \
    {                                                                               \
        field.assign((char *)data_ptr, length);                                     \
        field = StringUtils::remove_null_terminating_chars(field);                  \
        data_ptr += length;                                                         \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        goto COPY_DATA_ERROR;                                                       \
    }                                                                               \

/*---------------------------------------------------------*\
| NetworkClient name for log entries                        |
\*---------------------------------------------------------*/
const char* NETWORKCLIENT = "NetworkClient";

NetworkClient::NetworkClient()
{
    port_ip                             = "127.0.0.1";
    port_num                            = OPENRGB_SDK_PORT;
    client_flags                        = NET_CLIENT_FLAG_SUPPORTS_RGBCONTROLLER
                                        | NET_CLIENT_FLAG_SUPPORTS_LOGMANAGER
                                        | NET_CLIENT_FLAG_SUPPORTS_PROFILEMANAGER
                                        | NET_CLIENT_FLAG_SUPPORTS_SETTINGSMANAGER;
    client_flags_sent                   = false;
    client_is_local_client              = false;
    client_string_sent                  = false;
    client_sock                         = -1;
    detection_percent                   = 100;
    detection_string                    = "";
    protocol_initialized                = false;
    protocol_version                    = 0;
    server_connected                    = false;
    server_controller_ids_requested     = false;
    server_controller_ids_received      = false;
    server_flags                        = 0;
    server_protocol_version             = 0;
    server_reinitialize                 = false;
    change_in_progress                  = false;

    ListenThread                        = NULL;
    ConnectionThread                    = NULL;
}

NetworkClient::~NetworkClient()
{
    StopClient();
}

/*---------------------------------------------------------*\
| Client Information functions                              |
\*---------------------------------------------------------*/
bool NetworkClient::GetConnected()
{
    return(server_connected);
}

std::string NetworkClient::GetIP()
{
    return(port_ip);
}

bool NetworkClient::GetLocal()
{
    return(client_is_local_client);
}

unsigned short NetworkClient::GetPort()
{
    return(port_num);
}

unsigned int NetworkClient::GetProtocolVersion()
{
    return(protocol_version);
}

bool NetworkClient::GetOnline()
{
    return(server_connected && client_string_sent && protocol_initialized && server_flags_initialized && server_initialized);
}

std::string NetworkClient::GetServerName()
{
    return(server_name);
}

bool NetworkClient::GetSupportsRGBControllerAPI()
{
    return(server_flags & NET_SERVER_FLAG_SUPPORTS_RGBCONTROLLER);
}

bool NetworkClient::GetSupportsLogManagerAPI()
{
    return(server_flags & NET_SERVER_FLAG_SUPPORTS_LOGMANAGER);
}

bool NetworkClient::GetSupportsProfileManagerAPI()
{
    return(server_flags & NET_SERVER_FLAG_SUPPORTS_PROFILEMANAGER);
}

bool NetworkClient::GetSupportsPluginManagerAPI()
{
    return(server_flags & NET_SERVER_FLAG_SUPPORTS_PLUGINMANAGER);
}

bool NetworkClient::GetSupportsSettingsManagerAPI()
{
    return(server_flags & NET_SERVER_FLAG_SUPPORTS_SETTINGSMANAGER);
}

bool NetworkClient::GetSupportsDetectionAPI()
{
    return(server_flags & NET_SERVER_FLAG_SUPPORTS_DETECTION);
}

bool NetworkClient::GetSupportsDeviceInfoAPI()
{
    return(server_flags & NET_SERVER_FLAG_SUPPORTS_DEVICE_INFO);
}

/*---------------------------------------------------------*\
| Client Control functions                                  |
\*---------------------------------------------------------*/
void NetworkClient::RequestLocalClient(bool request_local)
{
    /*-----------------------------------------------------*\
    | Set the request local client flag                     |
    \*-----------------------------------------------------*/
    if(request_local)
    {
        client_flags |= NET_CLIENT_FLAG_REQUEST_LOCAL_CLIENT;
    }
    else
    {
        client_flags &= ~NET_CLIENT_FLAG_REQUEST_LOCAL_CLIENT;
    }

    /*-----------------------------------------------------*\
    | If we have already sent the flags, send again         |
    \*-----------------------------------------------------*/
    if(client_flags_sent)
    {
        SendData_ClientFlags();
    }
}

void NetworkClient::SetIP(std::string new_ip)
{
    if(server_connected == false)
    {
        port_ip = new_ip;
    }
}

void NetworkClient::SetName(std::string new_name)
{
    client_name = new_name;

    if(server_connected == true)
    {
        SendData_ClientString();
    }
}

void NetworkClient::SetPort(unsigned short new_port)
{
    if(server_connected == false)
    {
        port_num = new_port;
    }
}

void NetworkClient::StartClient()
{
    /*-----------------------------------------------------*\
    | Start a TCP server and launch threads                 |
    \*-----------------------------------------------------*/
    char port_str[6];
    snprintf(port_str, 6, "%d", port_num);

    port.tcp_client(port_ip.c_str(), port_str);

    client_active = true;

    /*-----------------------------------------------------*\
    | Start the connection thread                           |
    \*-----------------------------------------------------*/
    ConnectionThread = new std::thread(&NetworkClient::ConnectionThreadFunction, this);

    /*-----------------------------------------------------*\
    | Start the ProfileManager listen thread                |
    \*-----------------------------------------------------*/
    profilemanager_thread                           = new NetworkClientListenerThread;

    profilemanager_thread->id                       = 0;
    profilemanager_thread->index                    = 0;
    profilemanager_thread->online                   = true;
    profilemanager_thread->thread                   = new std::thread(&NetworkClient::ProfileManagerListenThread, this, profilemanager_thread);

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_CLIENT_STARTED);
}

void NetworkClient::StopClient()
{
    /*-----------------------------------------------------*\
    | Disconnect the server and set it as inactive          |
    \*-----------------------------------------------------*/
    server_connected = false;
    client_active    = false;

    /*-----------------------------------------------------*\
    | Shut down and close the client socket                 |
    \*-----------------------------------------------------*/
    if(server_connected)
    {
        shutdown(client_sock, SD_RECEIVE);
        closesocket(client_sock);
    }

    client_active    = false;
    server_connected = false;

    /*-----------------------------------------------------*\
    | Close the listen thread                               |
    \*-----------------------------------------------------*/
    if(ListenThread)
    {
        ListenThread->join();
        delete ListenThread;
        ListenThread = nullptr;
    }

    /*-----------------------------------------------------*\
    | Close the ProfileManager listen thread                |
    \*-----------------------------------------------------*/
    if(profilemanager_thread->thread)
    {
        profilemanager_thread->online = false;
        profilemanager_thread->start_cv.notify_all();
        profilemanager_thread->thread->join();
        delete profilemanager_thread->thread;
        profilemanager_thread->thread = nullptr;
        delete profilemanager_thread;
    }

    /*-----------------------------------------------------*\
    | Close the connection thread                           |
    \*-----------------------------------------------------*/
    if(ConnectionThread)
    {
        connection_cv.notify_all();
        ConnectionThread->join();
        delete ConnectionThread;
        ConnectionThread = nullptr;
    }

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_CLIENT_STOPPED);
}

void NetworkClient::SendRequest_ControllerData(unsigned int dev_id)
{
    NetPacketHeader request_hdr;

    /*-----------------------------------------------------*\
    | Clear the controller data received flag               |
    \*-----------------------------------------------------*/
    controller_data_received = false;

    /*-----------------------------------------------------*\
    | Protocol version 0 sends no data, all other protocols |
    | send the protocol version                             |
    \*-----------------------------------------------------*/
    if(protocol_version == 0)
    {
        InitNetPacketHeader(&request_hdr, dev_id, NET_PACKET_ID_REQUEST_CONTROLLER_DATA, 0);
    }
    else
    {
        InitNetPacketHeader(&request_hdr, dev_id, NET_PACKET_ID_REQUEST_CONTROLLER_DATA, sizeof(protocol_version));
    }

    /*-----------------------------------------------------*\
    | Send the packet, including the data field if protocol |
    | is greater than 0.                                    |
    \*-----------------------------------------------------*/
    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);

    if(protocol_version > 0)
    {
        send(client_sock, (char *)&protocol_version, sizeof(unsigned int), MSG_NOSIGNAL);
    }

    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RescanDevices()
{
    if(GetProtocolVersion() >= 5)
    {
        NetPacketHeader request_hdr;

        InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_REQUEST_RESCAN_DEVICES, 0);

        send_in_progress.lock();
        send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();
    }
}

/*---------------------------------------------------------*\
| Client Callback functions                                 |
\*---------------------------------------------------------*/
void NetworkClient::ClearCallbacks()
{
    NetworkClientCallbacks.clear();
    NetworkClientCallbackArgs.clear();
}

void NetworkClient::RegisterNetworkClientCallback(NetworkClientCallback new_callback, void* new_callback_arg)
{
    NetworkClientCallbacks.push_back(new_callback);
    NetworkClientCallbackArgs.push_back(new_callback_arg);
}

/*---------------------------------------------------------*\
| Device Info Functions                                     |
\*---------------------------------------------------------*/
std::vector<HIDDeviceInfo> NetworkClient::GetHIDDeviceInfo()
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    std::vector<HIDDeviceInfo>  device_info;
    NetPacketHeader             reply_hdr;

    if(GetSupportsDeviceInfoAPI())
    {
        InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_GET_HID_DEVICE_INFO, 0);

        send_in_progress.lock();
        send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();

        /*-------------------------------------------------*\
        | Wait for response                                 |
        \*-------------------------------------------------*/
        std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
        waiting_on_response_cv.wait(wait_lock);

        /*-------------------------------------------------*\
        | Parse response into device list                   |
        \*-------------------------------------------------*/
        if(response_header.pkt_id == NET_PACKET_ID_GET_HID_DEVICE_INFO && response_data_ptr != NULL)
        {
            unsigned int            device_count    = 0;
            unsigned char*          data_ptr        = response_data_ptr;
            unsigned int&           data_size       = response_header.pkt_size;
            unsigned int            data_size_pkt;

            COPY_DATA_FIELD(data_ptr, response_data_ptr, data_size_pkt);

            if(data_size_pkt == data_size)
            {
                COPY_DATA_FIELD(data_ptr, response_data_ptr, device_count);

                for(unsigned int device_idx = 0; device_idx < device_count; device_idx++)
                {
                    HIDDeviceInfo   device;

                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.vendor_id);
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.product_id);
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.release_number);
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.usage_page);
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.usage);
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.interface_number);

                    unsigned short serial_number_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, serial_number_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, serial_number_size, device.serial_number);

                    unsigned short manufacturer_string_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, manufacturer_string_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, manufacturer_string_size, device.manufacturer_string);

                    unsigned short product_string_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, product_string_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, product_string_size, device.product_string);

                    unsigned short path_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, path_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, path_size, device.path);

                    device_info.push_back(device);
                }
            }

            COPY_DATA_ERROR:
            delete[] response_data_ptr;
            response_data_ptr = NULL;
        }
    }

    return(device_info);
}

std::vector<i2c_smbus_info> NetworkClient::GetI2CBusInfo()
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    std::vector<i2c_smbus_info> bus_info;
    NetPacketHeader             reply_hdr;

    if(GetSupportsDeviceInfoAPI())
    {
        InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_GET_I2C_BUS_INFO, 0);

        send_in_progress.lock();
        send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();

        /*-------------------------------------------------*\
        | Wait for response                                 |
        \*-------------------------------------------------*/
        std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
        waiting_on_response_cv.wait(wait_lock);

        /*-------------------------------------------------*\
        | Parse response into bus list                      |
        \*-------------------------------------------------*/
        if(response_header.pkt_id == NET_PACKET_ID_GET_I2C_BUS_INFO && response_data_ptr != NULL)
        {
            unsigned int            bus_count       = 0;
            unsigned char*          data_ptr        = response_data_ptr;
            unsigned int&           data_size       = response_header.pkt_size;
            unsigned int            data_size_pkt;

            COPY_DATA_FIELD(data_ptr, response_data_ptr, data_size_pkt);

            if(data_size_pkt == data_size)
            {
                COPY_DATA_FIELD(data_ptr, response_data_ptr, bus_count);

                for(unsigned int bus_idx = 0; bus_idx < bus_count; bus_idx++)
                {
                    i2c_smbus_info  bus;

                    COPY_DATA_FIELD(data_ptr, response_data_ptr, bus);

                    bus_info.push_back(bus);
                }
            }

            COPY_DATA_ERROR:
            delete[] response_data_ptr;
            response_data_ptr = NULL;
        }
    }

    return(bus_info);
}

std::vector<std::string> NetworkClient::GetSerialPorts()
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    std::vector<std::string>    serial_ports;
    NetPacketHeader             reply_hdr;

    if(GetSupportsDeviceInfoAPI())
    {
        InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_GET_SERIAL_PORTS, 0);

        send_in_progress.lock();
        send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();

        /*-------------------------------------------------*\
        | Wait for response                                 |
        \*-------------------------------------------------*/
        std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
        waiting_on_response_cv.wait(wait_lock);

        /*-------------------------------------------------*\
        | Parse response into device list                   |
        \*-------------------------------------------------*/
        if(response_header.pkt_id == NET_PACKET_ID_GET_SERIAL_PORTS && response_data_ptr != NULL)
        {
            unsigned int            port_count      = 0;
            unsigned char*          data_ptr        = response_data_ptr;
            unsigned int&           data_size       = response_header.pkt_size;
            unsigned int            data_size_pkt;

            COPY_DATA_FIELD(data_ptr, response_data_ptr, data_size_pkt);

            if(data_size_pkt == data_size)
            {
                COPY_DATA_FIELD(data_ptr, response_data_ptr, port_count);

                for(unsigned int port_idx = 0; port_idx < port_count; port_idx++)
                {
                    std::string port_string;
                    unsigned short port_string_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, port_string_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, port_string_size, port_string);

                    serial_ports.push_back(port_string);
                }
            }

            COPY_DATA_ERROR:
            delete[] response_data_ptr;
            response_data_ptr = NULL;
        }
    }

    return(serial_ports);
}

std::vector<USBDeviceInfo> NetworkClient::GetUSBDeviceInfo()
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    std::vector<USBDeviceInfo>  device_info;
    NetPacketHeader             reply_hdr;

    if(GetSupportsDeviceInfoAPI())
    {
        InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_GET_USB_DEVICE_INFO, 0);

        send_in_progress.lock();
        send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();

        /*-------------------------------------------------*\
        | Wait for response                                 |
        \*-------------------------------------------------*/
        std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
        waiting_on_response_cv.wait(wait_lock);

        /*-------------------------------------------------*\
        | Parse response into device list                   |
        \*-------------------------------------------------*/
        if(response_header.pkt_id == NET_PACKET_ID_GET_USB_DEVICE_INFO && response_data_ptr != NULL)
        {
            unsigned int            device_count    = 0;
            unsigned char*          data_ptr        = response_data_ptr;
            unsigned int&           data_size       = response_header.pkt_size;
            unsigned int            data_size_pkt;

            COPY_DATA_FIELD(data_ptr, response_data_ptr, data_size_pkt);

            if(data_size_pkt == data_size)
            {
                COPY_DATA_FIELD(data_ptr, response_data_ptr, device_count);

                for(unsigned int device_idx = 0; device_idx < device_count; device_idx++)
                {
                    USBDeviceInfo   device;

                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.vendor_id);
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, device.product_id);

                    unsigned short serial_number_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, serial_number_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, serial_number_size, device.serial_number);

                    unsigned short manufacturer_string_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, manufacturer_string_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, manufacturer_string_size, device.manufacturer_string);

                    unsigned short product_string_size;
                    COPY_DATA_FIELD(data_ptr, response_data_ptr, product_string_size);
                    COPY_STRING_FIELD(data_ptr, response_data_ptr, product_string_size, device.product_string);

                    device_info.push_back(device);
                }
            }

            COPY_DATA_ERROR:
            delete[] response_data_ptr;
            response_data_ptr = NULL;
        }
    }

    return(device_info);
}

/*---------------------------------------------------------*\
| DetectionManager functions                                |
\*---------------------------------------------------------*/
unsigned int NetworkClient::DetectionManager_GetDetectionPercent()
{
    return(detection_percent);
}

std::string NetworkClient::DetectionManager_GetDetectionString()
{
    return(detection_string);
}

/*---------------------------------------------------------*\
| LogManager functions                                      |
\*---------------------------------------------------------*/
void NetworkClient::LogManager_ClearLogBuffer()
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_LOGMANAGER_CLEAR_LOG_BUFFER, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::LogManager_GetLogBuffer()
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_LOGMANAGER_GET_LOG_BUFFER, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

unsigned int NetworkClient::LogManager_GetLogLevel()
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    unsigned int    log_level = 0;
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_LOGMANAGER_GET_LOG_LEVEL, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();

    /*-----------------------------------------------------*\
    | Wait for response                                     |
    \*-----------------------------------------------------*/
    std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
    waiting_on_response_cv.wait(wait_lock);

    /*-----------------------------------------------------*\
    | Parse response into bus list                          |
    \*-----------------------------------------------------*/
    if(response_header.pkt_id == NET_PACKET_ID_LOGMANAGER_GET_LOG_LEVEL && response_data_ptr != NULL)
    {
        if(response_header.pkt_size >= sizeof(log_level))
        {
            memcpy(&log_level, response_data_ptr, sizeof(log_level));
        }

        delete[] response_data_ptr;
        response_data_ptr = NULL;
    }

    return(log_level);
}

void NetworkClient::LogManager_SetLogLevel(unsigned int log_level)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_LOGMANAGER_SET_LOG_LEVEL, sizeof(log_level));

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char*)&log_level, sizeof(log_level), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

/*---------------------------------------------------------*\
| ProfileManager functions                                  |
\*---------------------------------------------------------*/
void NetworkClient::ProfileManager_GetProfileList()
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::ProfileManager_LoadProfile(std::string profile_name)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_LOAD_PROFILE, (unsigned int)strlen(profile_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::ProfileManager_SaveProfile(std::string profile_name)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_SAVE_PROFILE, (unsigned int)strlen(profile_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::ProfileManager_DeleteProfile(std::string profile_name)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_DELETE_PROFILE, (unsigned int)strlen(profile_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::ProfileManager_UploadProfile(std::string profile_json_str)
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_UPLOAD_PROFILE, (unsigned int)strlen(profile_json_str.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_json_str.c_str(), request_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

std::string NetworkClient::ProfileManager_DownloadProfile(std::string profile_name)
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    NetPacketHeader request_hdr;
    std::string     response_string;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE, (unsigned int)strlen(profile_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_name.c_str(), request_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();

    /*-----------------------------------------------------*\
    | Wait for response                                     |
    \*-----------------------------------------------------*/
    std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
    waiting_on_response_cv.wait(wait_lock);

    /*-----------------------------------------------------*\
    | Parse response into bus list                          |
    \*-----------------------------------------------------*/
    if(response_header.pkt_id == NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE && response_data_ptr != NULL)
    {
        unsigned int&   data_size   = response_header.pkt_size;
        unsigned char*  data_ptr    = response_data_ptr;

        COPY_STRING_FIELD(data_ptr, response_data_ptr, data_size, response_string);

        COPY_DATA_ERROR:
        delete[] response_data_ptr;
        response_data_ptr = NULL;
    }

    return(response_string);
}

std::string NetworkClient::ProfileManager_GetActiveProfile()
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    NetPacketHeader request_hdr;
    std::string     response_string;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();

    /*-----------------------------------------------------*\
    | Wait for response                                     |
    \*-----------------------------------------------------*/
    std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
    waiting_on_response_cv.wait(wait_lock);

    /*-----------------------------------------------------*\
    | Parse response into bus list                          |
    \*-----------------------------------------------------*/
    if(response_header.pkt_id == NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE && response_data_ptr != NULL)
    {
        unsigned int&   data_size   = response_header.pkt_size;
        unsigned char*  data_ptr    = response_data_ptr;

        COPY_STRING_FIELD(data_ptr, response_data_ptr, data_size, response_string);

        COPY_DATA_ERROR:
        delete[] response_data_ptr;
        response_data_ptr = NULL;
    }

    return(response_string);
}

void NetworkClient::ProfileManager_ClearActiveProfile()
{
    NetPacketHeader pkt_hdr;

    InitNetPacketHeader(&pkt_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_CLEAR_ACTIVE_PROFILE, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&pkt_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

/*---------------------------------------------------------*\
| SettingsManager functions                                 |
\*---------------------------------------------------------*/
std::string NetworkClient::SettingsManager_GetSettings(std::string settings_key)
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    NetPacketHeader request_hdr;
    std::string     response_string;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS, (unsigned int)strlen(settings_key.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)settings_key.c_str(), request_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();

    /*-----------------------------------------------------*\
    | Wait for response                                     |
    \*-----------------------------------------------------*/
    std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
    waiting_on_response_cv.wait(wait_lock);

    /*-----------------------------------------------------*\
    | Parse response into bus list                          |
    \*-----------------------------------------------------*/
    if(response_header.pkt_id == NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS && response_data_ptr != NULL)
    {
        unsigned int&   data_size   = response_header.pkt_size;
        unsigned char*  data_ptr    = response_data_ptr;

        COPY_STRING_FIELD(data_ptr, response_data_ptr, data_size, response_string);

        COPY_DATA_ERROR:
        response_data_ptr = NULL;
    }

    return(response_string);
}

std::string NetworkClient::SettingsManager_GetSettingsSchema(std::string settings_key)
{
    /*-----------------------------------------------------*\
    | Send request                                          |
    \*-----------------------------------------------------*/
    NetPacketHeader request_hdr;
    std::string     response_string;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS_SCHEMA, (unsigned int)strlen(settings_key.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)settings_key.c_str(), request_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();

    /*-----------------------------------------------------*\
    | Wait for response                                     |
    \*-----------------------------------------------------*/
    std::unique_lock<std::mutex> wait_lock(waiting_on_response_mutex);
    waiting_on_response_cv.wait(wait_lock);

    /*-----------------------------------------------------*\
    | Parse response into bus list                          |
    \*-----------------------------------------------------*/
    if(response_header.pkt_id == NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS_SCHEMA && response_data_ptr != NULL)
    {
        unsigned int&   data_size   = response_header.pkt_size;
        unsigned char*  data_ptr    = response_data_ptr;

        COPY_STRING_FIELD(data_ptr, response_data_ptr, data_size, response_string);

        COPY_DATA_ERROR:
        delete[] response_data_ptr;
        response_data_ptr = NULL;
    }

    return(response_string);
}

void NetworkClient::SettingsManager_ModifySettings(std::string settings_json_str)
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_SETTINGSMANAGER_MODIFY_SETTINGS, (unsigned int)strlen(settings_json_str.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)settings_json_str.c_str(), request_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SettingsManager_SaveSettings()
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_SETTINGSMANAGER_SAVE_SETTINGS, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SettingsManager_SetSettings(std::string settings_json_str)
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_SETTINGSMANAGER_SET_SETTINGS, (unsigned int)strlen(settings_json_str.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)settings_json_str.c_str(), request_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

/*---------------------------------------------------------*\
| RGBController functions                                   |
\*---------------------------------------------------------*/
std::vector<RGBController*>& NetworkClient::GetRGBControllers()
{
    return(server_controllers);
}

void NetworkClient::SendRequest_RGBController_ClearSegments(unsigned int dev_idx, int zone)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;
    int             request_data[1];

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_CLEARSEGMENTS, sizeof(request_data));

    request_data[0]          = zone;

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)&request_data, sizeof(request_data), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_AddSegment(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, 0);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_ConfigureZone(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_CONFIGUREZONE, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, 0);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_ResizeZone(unsigned int dev_idx, int zone, int new_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;
    int             request_data[2];

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE, sizeof(request_data));

    request_data[0]          = zone;
    request_data[1]          = new_size;

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)&request_data, sizeof(request_data), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_ConfigureDevice(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_CONFIGUREDEVICE, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, 0);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_SetHidden(unsigned int dev_idx, bool hidden)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_SETHIDDEN, sizeof(hidden));

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)&hidden, sizeof(hidden), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_UpdateLEDs(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_UpdateZoneLEDs(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_UpdateSingleLED(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_SetCustomMode(unsigned int dev_idx)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_UpdateMode(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_UpdateZoneMode(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATEZONEMODE, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_SaveMode(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_SAVEMODE, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_SetDeviceSpecificConfiguration(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICCONFIGURATION, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_SetDeviceSpecificZoneConfiguration(unsigned int dev_idx, unsigned char* data_ptr, unsigned int data_size)
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_SETDEVICESPECIFICZONECONFIGURATION, data_size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data_ptr, data_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::WaitOnControllerData()
{
    for(int i = 0; i < 1000; i++)
    {
        if(controller_data_received)
        {
            break;
        }
        std::this_thread::sleep_for(1ms);
    }

    return;
}

/*---------------------------------------------------------*\
| Client callback signal functions                          |
\*---------------------------------------------------------*/
void NetworkClient::SignalNetworkClientUpdate(unsigned int update_reason)
{
    NetworkClientCallbackMutex.lock();

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < NetworkClientCallbacks.size(); callback_idx++)
    {
        NetworkClientCallbacks[callback_idx](NetworkClientCallbackArgs[callback_idx], update_reason);
    }

    NetworkClientCallbackMutex.unlock();
}

void NetworkClient::ProfileManagerListenThread(NetworkClientListenerThread* this_thread)
{
    while(this_thread->online == true)
    {
        std::unique_lock<std::mutex> start_lock(this_thread->start_mutex);
        this_thread->start_cv.wait(start_lock);

        while(this_thread->queue.size() > 0)
        {
            NetworkClientListenerThreadQueueEntry   queue_entry;

            this_thread->queue_mutex.lock();
            queue_entry = this_thread->queue.front();
            this_thread->queue.pop();
            this_thread->queue_mutex.unlock();

            switch(queue_entry.header.pkt_id)
            {
                case NET_PACKET_ID_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED:
                    ProcessRequest_ProfileManager_ActiveProfileChanged(queue_entry.header.pkt_size, queue_entry.data);
                    break;

                case NET_PACKET_ID_PROFILEMANAGER_PROFILE_LOADED:
                    ProcessRequest_ProfileManager_ProfileLoaded(queue_entry.header.pkt_size, queue_entry.data);
                    break;

                case NET_PACKET_ID_PROFILEMANAGER_PROFILE_ABOUT_TO_LOAD:
                    ProcessRequest_ProfileManager_ProfileAboutToLoad();
                    break;

                case NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST:
                case NET_PACKET_ID_PROFILEMANAGER_PROFILE_LIST_UPDATED:
                    ProcessRequest_ProfileManager_ProfileListUpdated(queue_entry.header.pkt_size, queue_entry.data);
                    break;

                case NET_PACKET_ID_RGBCONTROLLER_SIGNALUPDATE:
                    ProcessRequest_RGBController_SignalUpdate(queue_entry.header.pkt_size, queue_entry.data, queue_entry.header.pkt_dev_id);
                    break;
            }

            delete[] queue_entry.data;
        }
    }
}

/*---------------------------------------------------------*\
| Client thread functions                                   |
\*---------------------------------------------------------*/
void NetworkClient::ConnectionThreadFunction()
{
    std::unique_lock<std::mutex> lock(connection_mutex);

    /*-----------------------------------------------------*\
    | This thread manages the connection to the server      |
    \*-----------------------------------------------------*/
    while(client_active == true)
    {
        if(server_connected == false)
        {
            /*---------------------------------------------*\
            | Connect to server and reconnect if the        |
            | connection is lost                            |
            \*---------------------------------------------*/
            server_initialized = false;

            /*---------------------------------------------*\
            | Try to connect to server                      |
            \*---------------------------------------------*/
            if(port.tcp_client_connect() == true)
            {
                client_sock = port.sock;
                LOG_INFO("[%s] Connected to server", NETWORKCLIENT);

                /*-----------------------------------------*\
                | Server is now connected                   |
                \*-----------------------------------------*/
                server_connected = true;

                /*-----------------------------------------*\
                | Start the listener thread                 |
                \*-----------------------------------------*/
                ListenThread = new std::thread(&NetworkClient::ListenThreadFunction, this);

                /*-----------------------------------------*\
                | Server is not initialized                 |
                \*-----------------------------------------*/
                server_initialized = false;

                /*-----------------------------------------*\
                | Client info has changed, call the         |
                | callbacks                                 |
                \*-----------------------------------------*/
                SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_CLIENT_CONNECTED);
            }
            else
            {
                LOG_INFO("[%s] Connection attempt failed", NETWORKCLIENT);
            }
        }

        /*-------------------------------------------------*\
        | Double-check client_active as it could have       |
        | changed                                           |
        \*-------------------------------------------------*/
        if(client_active && ( protocol_initialized == false || client_string_sent == false || server_initialized == false ) && server_connected == true)
        {
            /*---------------------------------------------*\
            | Initialize protocol version if it hasn't      |
            | already been initialized                      |
            \*---------------------------------------------*/
            if(!protocol_initialized)
            {
                /*-----------------------------------------*\
                | Request protocol version                  |
                \*-----------------------------------------*/
                server_protocol_version_received = false;

                SendRequest_ProtocolVersion();

                /*-----------------------------------------*\
                | Wait up to 1s for protocol version reply  |
                \*-----------------------------------------*/
                unsigned int timeout_counter = 0;

                while(!server_protocol_version_received)
                {
                    connection_cv.wait_for(lock, 5ms);
                    if(!client_active)
                    {
                        break;
                    }

                    timeout_counter++;

                    /*-------------------------------------*\
                    | If no protocol version received       |
                    | within 1s, assume the server doesn't  |
                    | support protocol versioning and use   |
                    | protocol version 0                    |
                    \*-------------------------------------*/
                    if(timeout_counter > 200)
                    {
                        server_protocol_version          = 0;
                        server_protocol_version_received = true;
                    }
                }

                /*-----------------------------------------*\
                | Limit the protocol version to the highest |
                | supported by both the client and the      |
                | server.                                   |
                \*-----------------------------------------*/
                if(server_protocol_version > OPENRGB_SDK_PROTOCOL_VERSION)
                {
                    protocol_version = OPENRGB_SDK_PROTOCOL_VERSION;
                }
                else
                {
                    protocol_version = server_protocol_version;
                }

                /*-----------------------------------------*\
                | If the protocol version is less than 6,   |
                | feature flags were not part of this       |
                | protocol, initialize the server flags to  |
                | a default value that represents what that |
                | protocol version supported.  Protocol 5   |
                | introduced rescan so set the supports     |
                | detection flag.                           |
                \*-----------------------------------------*/
                if(protocol_version < 6)
                {
                    server_flags = NET_SERVER_FLAG_SUPPORTS_RGBCONTROLLER;

                    if(protocol_version >= 5)
                    {
                        server_flags |= NET_SERVER_FLAG_SUPPORTS_DETECTION;
                    }

                    server_flags_initialized = true;
                }

                SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_PROTOCOL_NEGOTIATED);

                protocol_initialized = true;
            }

            /*---------------------------------------------*\
            | Send client string if it hasn't already been  |
            | sent                                          |
            \*---------------------------------------------*/
            if(!client_string_sent)
            {
                /*-----------------------------------------*\
                | Once server is connected, send client     |
                | string                                    |
                \*-----------------------------------------*/
                SendData_ClientString();

                client_string_sent = true;
            }

            if((!client_flags_sent) && (protocol_version >= 6))
            {
                /*-----------------------------------------*\
                | Once server is connected, send client     |
                | string                                    |
                \*-----------------------------------------*/
                SendData_ClientFlags();

                client_flags_sent = true;
            }

            /*---------------------------------------------*\
            | Initialize the server device list if it       |
            | hasn't already been initialized               |
            \*---------------------------------------------*/
            if(!server_initialized)
            {
                /*-----------------------------------------*\
                | Request the server controller count       |
                \*-----------------------------------------*/
                if(!server_controller_ids_requested)
                {
                    SendRequest_ControllerIDs();

                    server_controller_ids_requested = true;
                }
                else
                {
                    /*-------------------------------------*\
                    | Wait for the server controller count  |
                    | to be received                        |
                    \*-------------------------------------*/
                    if(server_controller_ids_received)
                    {
                        /*---------------------------------*\
                        | Once count is received, request   |
                        | controllers                       |
                        | When data is received, increment  |
                        | count of requested controllers    |
                        | until all controllers have been   |
                        | received                          |
                        \*---------------------------------*/
                        if(requested_controller_index < server_controller_ids.size())
                        {
                            if(!controller_data_requested)
                            {
                                /*-------------------------*\
                                | Check if this ID already  |
                                | exists, if not, request it|
                                \*-------------------------*/
                                unsigned int    id              = server_controller_ids[requested_controller_index];
                                RGBController * rgb_controller = controller_from_id(id);

                                if(rgb_controller == NULL)
                                {
                                    LOG_INFO("[%s] Requesting controller ID %d", NETWORKCLIENT, server_controller_ids[requested_controller_index]);

                                    controller_data_received    = false;
                                    SendRequest_ControllerData(id);
                                    controller_data_requested   = true;
                                }
                                else
                                {
                                    controller_data_received = true;
                                }
                            }

                            if(controller_data_received)
                            {
                                requested_controller_index++;
                                controller_data_requested = false;
                            }
                        }
                        else
                        {
                            server_initialized = true;
                        }
                    }
                }
            }

            /*---------------------------------------------*\
            | Wait 1 ms or until the thread is requested to |
            | stop                                          |
            \*---------------------------------------------*/
            connection_cv.wait_for(lock, 1ms);
        }
        else
        {
            /*---------------------------------------------*\
            | Wait 1 sec or until the thread is requested   |
            | to stop                                       |
            \*---------------------------------------------*/
            connection_cv.wait_for(lock, 1s);
        }
    }
}

void NetworkClient::ListenThreadFunction()
{
    LOG_INFO("[%s] Listener thread started", NETWORKCLIENT);

    /*-----------------------------------------------------*\
    | This thread handles messages received from the server |
    \*-----------------------------------------------------*/
    while(server_connected == true)
    {
        NetPacketHeader header;
        int             bytes_read  = 0;
        unsigned char*  data        = NULL;
        bool            delete_data = true;

        for(unsigned int i = 0; i < 4; i++)
        {
            /*---------------------------------------------*\
            | Read byte of magic                            |
            \*---------------------------------------------*/
            bytes_read = recv_select(client_sock, &header.pkt_magic[i], 1, 0);

            if(bytes_read <= 0)
            {
                LOG_ERROR("[%s] recv_select failed receiving magic, closing listener", NETWORKCLIENT);
                goto listen_done;
            }

            /*---------------------------------------------*\
            | Test characters of magic "ORGB"               |
            \*---------------------------------------------*/
            if(header.pkt_magic[i] != openrgb_sdk_magic[i])
            {
                LOG_ERROR("[%s] Invalid magic received, closing listener", NETWORKCLIENT);
                goto listen_done;
            }
        }

        /*-------------------------------------------------*\
        | If we get to this point, the magic is correct.    |
        | Read the rest of the header                       |
        \*-------------------------------------------------*/
        bytes_read = 0;
        do
        {
            int tmp_bytes_read = 0;

            tmp_bytes_read = recv_select(client_sock, (char *)&header.pkt_dev_id + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read, 0);

            bytes_read += tmp_bytes_read;

            if(tmp_bytes_read <= 0)
            {
                goto listen_done;
            }

        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        /*-------------------------------------------------*\
        | Header received, now receive the data             |
        \*-------------------------------------------------*/
        if(header.pkt_size > 0)
        {
            bytes_read = 0;

            data = new unsigned char[header.pkt_size];

            do
            {
                int tmp_bytes_read = 0;

                tmp_bytes_read = recv_select(client_sock, (char*)&data[(unsigned int)bytes_read], header.pkt_size - bytes_read, 0);

                if(tmp_bytes_read <= 0)
                {
                    goto listen_done;
                }
                bytes_read += tmp_bytes_read;

            } while ((unsigned int)bytes_read < header.pkt_size);
        }

        /*-------------------------------------------------*\
        | Entire request received, select functionality     |
        | based on request ID                               |
        \*-------------------------------------------------*/
        switch(header.pkt_id)
        {
            case NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                ProcessReply_ControllerIDs(header.pkt_size, data);
                break;

            case NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                ProcessReply_ControllerData(header.pkt_size, data, header.pkt_dev_id);
                break;

            case NET_PACKET_ID_REQUEST_PROTOCOL_VERSION:
                ProcessReply_ProtocolVersion(header.pkt_size, data);
                break;

            case NET_PACKET_ID_SET_SERVER_NAME:
                ProcessRequest_ServerString(header.pkt_size, data);
                break;

            case NET_PACKET_ID_SET_SERVER_FLAGS:
                ProcessRequest_ServerFlags(header.pkt_size, data);
                break;

            case NET_PACKET_ID_DEVICE_LIST_UPDATED:
                ProcessRequest_DeviceListChanged();
                break;

            case NET_PACKET_ID_DETECTION_STARTED:
                SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_DETECTION_STARTED);
                break;

            case NET_PACKET_ID_DETECTION_PROGRESS_CHANGED:
                ProcessRequest_DetectionProgressChanged(header.pkt_size, data);
                break;

            case NET_PACKET_ID_DETECTION_COMPLETE:
                SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_DETECTION_COMPLETE);
                break;

            case NET_PACKET_ID_LOGMANAGER_LOGGED_ENTRY:
                ProcessRequest_LogManager_LoggedEntry(header.pkt_size, data);
                break;

            case NET_PACKET_ID_GET_I2C_BUS_INFO:
            case NET_PACKET_ID_GET_HID_DEVICE_INFO:
            case NET_PACKET_ID_GET_USB_DEVICE_INFO:
            case NET_PACKET_ID_GET_SERIAL_PORTS:
            case NET_PACKET_ID_LOGMANAGER_GET_LOG_LEVEL:
            case NET_PACKET_ID_PROFILEMANAGER_DOWNLOAD_PROFILE:
            case NET_PACKET_ID_PROFILEMANAGER_GET_ACTIVE_PROFILE:
            case NET_PACKET_ID_SETTINGSMANAGER_GET_SETTINGS:
                {
                    std::unique_lock<std::mutex> lock(waiting_on_response_mutex);

                    response_header     = header;
                    response_data_ptr   = data;
                    delete_data         = false;

                    lock.unlock();
                    waiting_on_response_cv.notify_all();
                }
                break;

        /*-------------------------------------------------*\
        | ProfileManager functions are handled in a         |
        | separate thread, queue the messages               |
        \*-------------------------------------------------*/
            case NET_PACKET_ID_PROFILEMANAGER_PROFILE_ABOUT_TO_LOAD:
            case NET_PACKET_ID_PROFILEMANAGER_ACTIVE_PROFILE_CHANGED:
            case NET_PACKET_ID_PROFILEMANAGER_PROFILE_LOADED:
            case NET_PACKET_ID_PROFILEMANAGER_GET_PROFILE_LIST:
            case NET_PACKET_ID_PROFILEMANAGER_PROFILE_LIST_UPDATED:
            case NET_PACKET_ID_RGBCONTROLLER_SIGNALUPDATE:
                {
                    profilemanager_thread->queue_mutex.lock();

                    NetworkClientListenerThreadQueueEntry new_entry;
                    new_entry.data                      = data;
                    new_entry.header                    = header;

                    profilemanager_thread->queue.push(new_entry);
                    profilemanager_thread->queue_mutex.unlock();
                    profilemanager_thread->start_cv.notify_all();

                    delete_data = false;
                }
                break;
        }

        if(delete_data)
        {
            delete[] data;
        }
    }

listen_done:
    LOG_INFO("[%s] Client socket has been closed", NETWORKCLIENT);
    client_flags                        = NET_CLIENT_FLAG_SUPPORTS_RGBCONTROLLER
                                        | NET_CLIENT_FLAG_SUPPORTS_PROFILEMANAGER
                                        | NET_CLIENT_FLAG_SUPPORTS_SETTINGSMANAGER;
    client_flags_sent                   = false;
    client_is_local_client              = false;
    client_string_sent                  = false;
    controller_data_requested           = false;
    controller_data_received            = false;
    protocol_initialized                = false;
    requested_controller_index          = 0;
    server_controller_ids_requested     = false;
    server_controller_ids_received      = false;
    server_flags                        = 0;
    server_flags_initialized            = false;
    server_initialized                  = false;
    server_connected                    = false;

    ControllerListMutex.lock();

    std::vector<RGBController *> server_controllers_copy = server_controllers;

    server_controllers.clear();

    for(size_t server_controller_idx = 0; server_controller_idx < server_controllers_copy.size(); server_controller_idx++)
    {
        delete server_controllers_copy[server_controller_idx];
    }

    ControllerListMutex.unlock();

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_CLIENT_DISCONNECTED);
}

/*---------------------------------------------------------*\
| Private Client functions                                  |
\*---------------------------------------------------------*/
void NetworkClient::ProcessReply_ControllerData(unsigned int data_size, unsigned char* data_ptr, unsigned int dev_id)
{
    unsigned int            data_size_pkt;
    unsigned char*          data_start          = data_ptr;
    RGBController*          existing_controller = controller_from_id(dev_id);
    RGBController_Network*  new_controller      = NULL;

    COPY_DATA_FIELD(data_ptr, data_start, data_size_pkt);

    if(data_size != data_size_pkt)
    {
        return;
    }

    /*-------------------------------------------------*\
    | Create a new controller from the received data    |
    \*-------------------------------------------------*/
    new_controller = new RGBController_Network(this, dev_id);
    RGBController::SetDeviceDescription(data_ptr, data_size - (unsigned int)(data_ptr - data_start), new_controller, GetProtocolVersion());

    /*-------------------------------------------------*\
    | Mark this controller as remote owned              |
    \*-------------------------------------------------*/
    new_controller->flags &= ~CONTROLLER_FLAG_LOCAL;
    new_controller->flags |= CONTROLLER_FLAG_REMOTE;

    /*-------------------------------------------------*\
    | If no controller exists with this ID, add it to   |
    | the list                                          |
    \*-------------------------------------------------*/
    if(existing_controller == NULL)
    {
        UpdateDeviceList(new_controller);
    }

    /*-------------------------------------------------*\
    | Otherwise, update the existing controller with    |
    | the new controller's data                         |
    \*-------------------------------------------------*/
    else
    {
        existing_controller->active_mode        = new_controller->active_mode;
        existing_controller->modes.clear();
        existing_controller->modes              = new_controller->modes;
        existing_controller->leds.clear();
        existing_controller->leds               = new_controller->leds;
        existing_controller->colors.clear();
        existing_controller->colors             = new_controller->colors;
        existing_controller->zones.clear();
        existing_controller->zones              = new_controller->zones;
        existing_controller->SetupColors();

        delete new_controller;
    }

    controller_data_received = true;

    COPY_DATA_ERROR:
    return;
}

void NetworkClient::ProcessReply_ControllerIDs(unsigned int data_size, unsigned char* data_ptr)
{
    unsigned int    controller_count    = 0;
    unsigned char*  data_start          = data_ptr;

    /*-----------------------------------------------------*\
    | Copy out controller count from data                   |
    \*-----------------------------------------------------*/
    COPY_DATA_FIELD(data_ptr, data_start, controller_count);

    /*-----------------------------------------------------*\
    | On protocol versions >= 6, the server sends a list of |
    | unique 32-bit IDs, one per controller.                |
    \*-----------------------------------------------------*/
    if(protocol_version >= 6)
    {
        /*-------------------------------------------------*\
        | Validate protocol >= 6 packet size                |
        \*-------------------------------------------------*/
        if(data_size == (sizeof(controller_count) + (controller_count * sizeof(unsigned int))))
        {
            server_controller_ids.clear();
            server_controller_ids.resize(controller_count);

            for(unsigned int controller_id_idx = 0; controller_id_idx < controller_count; controller_id_idx++)
            {
                COPY_DATA_FIELD(data_ptr, data_start, server_controller_ids[controller_id_idx]);
            }
        }
        else
        {
            server_controller_ids.clear();
            LOG_INFO("[%s] Received incorrect packet size for controller list, list cleared", NETWORKCLIENT);
        }
    }
    /*-------------------------------------------------*\
    | On protocol versions < 6, the server only sends   |
    | the controller count, using the index as the ID.  |
    \*-------------------------------------------------*/
    else
    {
        server_controller_ids.clear();
        server_controller_ids.resize(controller_count);

        for(unsigned int controller_id_idx = 0; controller_id_idx < controller_count; controller_id_idx++)
        {
            server_controller_ids[controller_id_idx] = controller_id_idx;
        }
    }

    /*-------------------------------------------------*\
    | Update the device list to remove any controllers  |
    | no longer in the list                             |
    \*-------------------------------------------------*/
    UpdateDeviceList(NULL);

    /*-------------------------------------------------*\
    | Reset the controller data requested index and     |
    | flag to begin the controller request cycle        |
    \*-------------------------------------------------*/
    server_controller_ids_received      = true;
    requested_controller_index          = 0;
    controller_data_requested           = false;

    COPY_DATA_ERROR:
    return;
}

void NetworkClient::ProcessReply_ProtocolVersion(unsigned int data_size, unsigned char* data_ptr)
{
    if(data_size == sizeof(unsigned int))
    {
        memcpy(&server_protocol_version, data_ptr, sizeof(unsigned int));
        server_protocol_version_received = true;
    }
}

void NetworkClient::ProcessRequest_DetectionProgressChanged(unsigned int data_size, unsigned char* data_ptr)
{
    unsigned int    data_size_pkt;
    unsigned char*  data_start      = data_ptr;
    unsigned short  string_length;

    COPY_DATA_FIELD(data_ptr, data_start, data_size_pkt);

    if(data_size != data_size_pkt)
    {
        return;
    }

    COPY_DATA_FIELD(data_ptr, data_start, detection_percent);
    COPY_DATA_FIELD(data_ptr, data_start, string_length);
    COPY_STRING_FIELD(data_ptr, data_start, string_length, detection_string);

    SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_DETECTION_PROGRESS_CHANGED);

    COPY_DATA_ERROR:
    return;
}

void NetworkClient::ProcessRequest_DeviceListChanged()
{
    /*-----------------------------------------------------*\
    | For protocol versions < 6, we need to redownload the  |
    | entire device list, so clear the server IDs and       |
    | update the device list to erase it.                   |
    \*-----------------------------------------------------*/
    if(protocol_version < 6)
    {
        server_controller_ids.clear();

        UpdateDeviceList(NULL);
    }

    /*-----------------------------------------------------*\
    | Mark server as uninitialized and reset server         |
    | initialization state so that it restarts the list     |
    | requesting process                                    |
    \*-----------------------------------------------------*/
    controller_data_requested           = false;
    controller_data_received            = false;
    requested_controller_index          = 0;
    server_controller_ids_requested     = false;
    server_controller_ids_received      = false;
    server_initialized                  = false;
}

void NetworkClient::ProcessRequest_LogManager_LoggedEntry(unsigned int data_size, unsigned char* data_ptr)
{
    unsigned char*  data_start      = data_ptr;
    unsigned int    data_size_pkt;
    unsigned short  filename_size;
    PLogMessage     message         = std::make_shared<LogMessage>();
    unsigned short  text_size;

    COPY_DATA_FIELD(data_ptr, data_start, data_size_pkt);

    if(data_size != data_size_pkt)
    {
        return;
    }

    COPY_DATA_FIELD(data_ptr, data_start, message->level);
    COPY_DATA_FIELD(data_ptr, data_start, message->line);
    COPY_DATA_FIELD(data_ptr, data_start, message->timestamp);
    COPY_DATA_FIELD(data_ptr, data_start, filename_size);
    COPY_STRING_FIELD(data_ptr, data_start, filename_size, message->filename);
    COPY_DATA_FIELD(data_ptr, data_start, text_size);
    COPY_STRING_FIELD(data_ptr, data_start, text_size, message->text);

    LogManager::get()->LogEntry_message(message);

    COPY_DATA_ERROR:
    return;
}

void NetworkClient::ProcessRequest_ProfileManager_ActiveProfileChanged(unsigned int data_size, unsigned char* data_ptr)
{
    /*-----------------------------------------------------*\
    | Validate inputs                                       |
    \*-----------------------------------------------------*/
    if((data_size == 0) || (data_ptr == NULL))
    {
        return;
    }

    std::string profile_name;
    profile_name.assign((char*)data_ptr, data_size);
    profile_name = StringUtils::remove_null_terminating_chars(profile_name);

    ResourceManager::get()->GetProfileManager()->SetActiveProfile(profile_name);
}

void NetworkClient::ProcessRequest_ProfileManager_ProfileAboutToLoad()
{
    ResourceManager::get()->GetProfileManager()->OnProfileAboutToLoad();

    NetPacketHeader pkt_hdr;

    InitNetPacketHeader(&pkt_hdr, 0, NET_PACKET_ID_PROFILEMANAGER_PROFILE_ABOUT_TO_LOAD, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&pkt_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::ProcessRequest_ProfileManager_ProfileListUpdated(unsigned int data_size, unsigned char* data_ptr)
{
    ResourceManager::get()->GetProfileManager()->SetProfileListFromDescription(data_size, (char*)data_ptr);
}

void NetworkClient::ProcessRequest_ProfileManager_ProfileLoaded(unsigned int data_size, unsigned char* data_ptr)
{
    /*-----------------------------------------------------*\
    | Validate inputs                                       |
    \*-----------------------------------------------------*/
    if((data_size == 0) || (data_ptr == NULL))
    {
        return;
    }

    std::string profile_json_str;
    profile_json_str.assign((char*)data_ptr, data_size);
    profile_json_str = StringUtils::remove_null_terminating_chars(profile_json_str);

    ResourceManager::get()->GetProfileManager()->OnProfileLoaded(profile_json_str);
}

void NetworkClient::ProcessRequest_RGBController_SignalUpdate(unsigned int data_size, unsigned char* data_ptr, unsigned int dev_id)
{
    RGBController*  controller;
    unsigned int    data_size_pkt;
    unsigned int    update_reason;

    /*-----------------------------------------------------*\
    | Get the controller pointer by ID and ensure it is     |
    | valid                                                 |
    \*-----------------------------------------------------*/
    controller = controller_from_id(dev_id);

    if(controller == NULL)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Create data buffer                                    |
    \*-----------------------------------------------------*/
    unsigned char*  data_start                    = data_ptr;

    /*-----------------------------------------------------*\
    | Copy in data size                                     |
    \*-----------------------------------------------------*/
    COPY_DATA_FIELD(data_ptr, data_start, data_size_pkt);

    if(data_size != data_size_pkt)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Copy in update reason                                 |
    \*-----------------------------------------------------*/
    COPY_DATA_FIELD(data_ptr, data_start, update_reason);

    /*-----------------------------------------------------*\
    | Process received data according to update reason      |
    \*-----------------------------------------------------*/
    switch(update_reason)
    {
        /*-------------------------------------------------*\
        | UpdateLEDs() sends color description              |
        \*-------------------------------------------------*/
        case RGBCONTROLLER_UPDATE_REASON_UPDATELEDS:
            RGBController::SetColorDescription(data_ptr, data_size - (unsigned int)(data_ptr - data_start), controller, GetProtocolVersion());
            break;

        /*-------------------------------------------------*\
        | Everything else send controller                   |
        | description                                       |
        \*-------------------------------------------------*/
        case RGBCONTROLLER_UPDATE_REASON_UPDATEMODE:
        case RGBCONTROLLER_UPDATE_REASON_SAVEMODE:
        case RGBCONTROLLER_UPDATE_REASON_CONFIGUREZONE:
        case RGBCONTROLLER_UPDATE_REASON_CLEARSEGMENTS:
        case RGBCONTROLLER_UPDATE_REASON_ADDSEGMENT:
        case RGBCONTROLLER_UPDATE_REASON_HIDDEN:
        case RGBCONTROLLER_UPDATE_REASON_UNHIDDEN:
        case RGBCONTROLLER_UPDATE_REASON_SETDEVICESPECIFICCONFIGURATION:
        case RGBCONTROLLER_UPDATE_REASON_SETDEVICESPECIFICZONECONFIGURATION:
        default:
            RGBController::SetDeviceDescription(data_ptr, data_size - (unsigned int)(data_ptr - data_start), controller, GetProtocolVersion());

            /*---------------------------------------------*\
            | Mark this controller as remote owned          |
            \*---------------------------------------------*/
            controller->flags &= ~CONTROLLER_FLAG_LOCAL;
            controller->flags |= CONTROLLER_FLAG_REMOTE;
            break;
    }

    controller->SignalUpdate(update_reason);

    COPY_DATA_ERROR:
    return;
}

void NetworkClient::ProcessRequest_ServerFlags(unsigned int data_size, unsigned char* data_ptr)
{
    /*-----------------------------------------------------*\
    | Validate inputs                                       |
    \*-----------------------------------------------------*/
    if((data_size < sizeof(unsigned int)) || (data_ptr == NULL))
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Store received server flags                           |
    \*-----------------------------------------------------*/
    server_flags = *(unsigned int *)data_ptr;

    /*-----------------------------------------------------*\
    | Update the local client status based on the server's  |
    | response                                              |
    \*-----------------------------------------------------*/
    if(server_flags & NET_SERVER_FLAG_LOCAL_CLIENT)
    {
        client_is_local_client = true;
    }
    else
    {
        client_is_local_client = false;
    }

    server_flags_initialized = true;
}

void NetworkClient::ProcessRequest_ServerString(unsigned int data_size, unsigned char* data_ptr)
{
    /*-----------------------------------------------------*\
    | Validate inputs                                       |
    \*-----------------------------------------------------*/
    if((data_size == 0) || (data_ptr == NULL))
    {
        return;
    }

    server_name.assign((char*)data_ptr, data_size);
    server_name = StringUtils::remove_null_terminating_chars(server_name);

    /*-----------------------------------------------------*\
    | Client info has changed, call the callbacks           |
    \*-----------------------------------------------------*/
    SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_SERVER_STRING_RECEIVED);
}

void NetworkClient::SendData_ClientFlags()
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_SET_CLIENT_FLAGS, sizeof(client_flags));

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)&client_flags, reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendData_ClientString()
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_SET_CLIENT_NAME, (unsigned int)strlen(client_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)client_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_ControllerIDs()
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_REQUEST_CONTROLLER_COUNT, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_ProtocolVersion()
{
    NetPacketHeader request_hdr;
    unsigned int    request_data;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_REQUEST_PROTOCOL_VERSION, sizeof(unsigned int));

    request_data             = OPENRGB_SDK_PROTOCOL_VERSION;

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)&request_data, sizeof(unsigned int), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::UpdateDeviceList(RGBController* new_controller)
{
    /*-----------------------------------------------------*\
    | Lock the controller list mutex                        |
    \*-----------------------------------------------------*/
    ControllerListMutex.lock();

    /*-----------------------------------------------------*\
    | Make a copy of the current controllers list           |
    \*-----------------------------------------------------*/
    std::vector<RGBController*> rgb_controllers_copy = server_controllers;

    /*-----------------------------------------------------*\
    | If adding a new controller, add it to the copied list |
    \*-----------------------------------------------------*/
    if(new_controller != NULL)
    {
        rgb_controllers_copy.push_back(new_controller);
    }

    /*-----------------------------------------------------*\
    | Clear the controllers list                            |
    \*-----------------------------------------------------*/
    server_controllers.clear();

    /*-----------------------------------------------------*\
    | Loop through all of the controller IDs                |
    \*-----------------------------------------------------*/
    for(std::size_t controller_id_idx = 0; controller_id_idx < server_controller_ids.size(); controller_id_idx++)
    {
        /*-------------------------------------------------*\
        | Loop through all of the controllers in the copied |
        | list and see if any match the ID                  |
        \*-------------------------------------------------*/
        std::size_t controller_idx  = 0;
        bool        match           = false;

        for(; controller_idx < rgb_controllers_copy.size(); controller_idx++)
        {
            if(server_controller_ids[controller_id_idx] == ((RGBController_Network*)rgb_controllers_copy[controller_idx])->GetID())
            {
                match = true;
                break;
            }
        }

        /*-------------------------------------------------*\
        | If a match was found, push it into the new        |
        | controller list and remove it from the copy       |
        \*-------------------------------------------------*/
        if(match)
        {
            server_controllers.push_back(rgb_controllers_copy[controller_idx]);
            rgb_controllers_copy.erase(rgb_controllers_copy.begin() + controller_idx);
        }
    }

    /*-----------------------------------------------------*\
    | The list has been updated, unlock the mutex           |
    \*-----------------------------------------------------*/
    ControllerListMutex.unlock();

    /*-----------------------------------------------------*\
    | Signal list updated callback                          |
    \*-----------------------------------------------------*/
    SignalNetworkClientUpdate(NETWORKCLIENT_UPDATE_REASON_DEVICE_LIST_UPDATED);

    /*-----------------------------------------------------*\
    | At this point, the copied list should contain only    |
    | controllers no longer in the current list of IDs      |
    | Delete them                                           |
    \*-----------------------------------------------------*/
    for(std::size_t controller_idx = 0; controller_idx < rgb_controllers_copy.size(); controller_idx++)
    {
        delete rgb_controllers_copy[controller_idx];
    }
}

/*---------------------------------------------------------*\
| Private ProfileManager functions                          |
\*---------------------------------------------------------*/
std::vector<std::string>* NetworkClient::ProcessReply_ProfileList(unsigned int data_size, unsigned char* data_ptr)
{
    unsigned int                data_size_pkt;
    unsigned char*              data_start      = data_ptr;
    unsigned short              num_profiles;
    std::vector<std::string>*   profile_list    = new std::vector<std::string>;

    COPY_DATA_FIELD(data_ptr, data_start, data_size_pkt);

    if(data_size != data_size_pkt)
    {
        return(profile_list);
    }

    COPY_DATA_FIELD(data_ptr, data_start, num_profiles);

    for(unsigned short profile_idx = 0; profile_idx < num_profiles; profile_idx++)
    {
        std::string             profile_name;
        unsigned short          profile_name_len;

        COPY_DATA_FIELD(data_ptr, data_start, profile_name_len);
        COPY_STRING_FIELD(data_ptr, data_start, profile_name_len, profile_name);

        profile_list->push_back(profile_name);
    }

    COPY_DATA_ERROR:
    return(profile_list);
}

/*---------------------------------------------------------*\
| Private helper functions                                  |
\*---------------------------------------------------------*/
RGBController * NetworkClient::controller_from_id(unsigned int id)
{
    RGBController * rgb_controller = NULL;

    for(std::size_t controller_idx = 0; controller_idx < server_controllers.size(); controller_idx++)
    {
        if(((RGBController_Network*)server_controllers[controller_idx])->GetID() == id)
        {
            rgb_controller = server_controllers[controller_idx];
            break;
        }
    }

    return(rgb_controller);
}

int NetworkClient::recv_select(SOCKET s, char *buf, int len, int flags)
{
    fd_set              set;
    struct timeval      timeout;

    while(1)
    {
        timeout.tv_sec      = 5;
        timeout.tv_usec     = 0;

        FD_ZERO(&set);
        FD_SET(s, &set);

        int rv = select((int)s + 1, &set, NULL, NULL, &timeout);

        if(rv == SOCKET_ERROR || server_connected == false)
        {
            return 0;
        }
        else if(rv == 0)
        {
            continue;
        }
        else
        {
        /*-------------------------------------------------*\
        | Set QUICKACK socket option on Linux to improve    |
        | performance                                       |
        \*-------------------------------------------------*/
#ifdef __linux__
            setsockopt(s, IPPROTO_TCP, TCP_QUICKACK, &yes, sizeof(yes));
#endif
            return(recv(s, buf, len, flags));
        }

    }
}

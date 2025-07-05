/*---------------------------------------------------------*\
| NetworkServer.h                                           |
|                                                           |
|   OpenRGB SDK network server                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <mutex>
#include <thread>
#include <chrono>
#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"
#include "ProfileManager.h"
#include "ResourceManager.h"

#define MAXSOCK 32
#define TCP_TIMEOUT_SECONDS 5

typedef void (*NetServerCallback)(void *);
typedef unsigned char* (*NetPluginCallback)(void *, unsigned int, unsigned char*, unsigned int*);

struct NetworkPlugin
{
    std::string name;
    std::string description;
    std::string version;
    NetPluginCallback callback;
    void* callback_arg;
    unsigned int protocol_version;
};

class NetworkClientInfo
{
public:
    NetworkClientInfo();
    ~NetworkClientInfo();

    SOCKET          client_sock;
    std::thread *   client_listen_thread;
    std::string     client_string;
    unsigned int    client_protocol_version;
    std::string     client_ip;
};

class NetworkServer
{
public:
    NetworkServer(std::vector<RGBController *>& control);
    ~NetworkServer();

    std::string                         GetHost();
    unsigned short                      GetPort();
    bool                                GetOnline();
    bool                                GetListening();
    unsigned int                        GetNumClients();
    const char *                        GetClientString(unsigned int client_num);
    const char *                        GetClientIP(unsigned int client_num);
    unsigned int                        GetClientProtocolVersion(unsigned int client_num);

    void                                ClientInfoChanged();
    void                                DeviceListChanged();
    void                                RegisterClientInfoChangeCallback(NetServerCallback, void * new_callback_arg);

    void                                ServerListeningChanged();
    void                                RegisterServerListeningChangeCallback(NetServerCallback, void * new_callback_arg);

    void                                SetHost(std::string host);
    void                                SetLegacyWorkaroundEnable(bool enable);
    void                                SetPort(unsigned short new_port);

    void                                StartServer();
    void                                StopServer();

    void                                ConnectionThreadFunction(int socket_idx);
    void                                ListenThreadFunction(NetworkClientInfo * client_sock);

    void                                ProcessRequest_ClientProtocolVersion(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_ClientString(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_RescanDevices();

    void                                SendReply_ControllerCount(SOCKET client_sock);
    void                                SendReply_ControllerData(SOCKET client_sock, unsigned int dev_idx, unsigned int protocol_version);
    void                                SendReply_ProtocolVersion(SOCKET client_sock);

    void                                SendRequest_DeviceListChanged(SOCKET client_sock);
    void                                SendReply_ProfileList(SOCKET client_sock);
    void                                SendReply_PluginList(SOCKET client_sock);
    void                                SendReply_PluginSpecific(SOCKET client_sock, unsigned int pkt_type, unsigned char* data, unsigned int data_size);

    void                                SetProfileManager(ProfileManagerInterface* profile_manager_pointer);

    void                                RegisterPlugin(NetworkPlugin plugin);
    void                                UnregisterPlugin(std::string plugin_name);

protected:
    std::string                         host;
    unsigned short                      port_num;
    std::atomic<bool>                   server_online;
    std::atomic<bool>                   server_listening;

    std::vector<RGBController *>&       controllers;

    std::mutex                          ServerClientsMutex;
    std::vector<NetworkClientInfo *>    ServerClients;
    std::thread *                       ConnectionThread[MAXSOCK];

    std::mutex                          ClientInfoChangeMutex;
    std::vector<NetServerCallback>      ClientInfoChangeCallbacks;
    std::vector<void *>                 ClientInfoChangeCallbackArgs;

    std::mutex                          ServerListeningChangeMutex;
    std::vector<NetServerCallback>      ServerListeningChangeCallbacks;
    std::vector<void *>                 ServerListeningChangeCallbackArgs;

    ProfileManagerInterface*            profile_manager;

    std::vector<NetworkPlugin>          plugins;

    std::mutex                          send_in_progress;

private:
#ifdef WIN32
    WSADATA     wsa;
#endif

    bool            legacy_workaround_enabled;
    int             socket_count;
    SOCKET          server_sock[MAXSOCK];

    int             accept_select(int sockfd);
    int             recv_select(SOCKET s, char *buf, int len, int flags);
};

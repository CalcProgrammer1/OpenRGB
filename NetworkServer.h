/*---------------------------------------------------------*\
| NetworkServer.h                                           |
|                                                           |
|   OpenRGB SDK network server                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"
#include "PluginManagerInterface.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

#define MAXSOCK 32
#define TCP_TIMEOUT_SECONDS 5

typedef void (*NetServerCallback)(void *);

typedef struct
{
    char *                      data;
    unsigned int                id;
    unsigned int                size;
    unsigned int                client_protocol_version;
} NetworkServerControllerThreadQueueEntry;

typedef struct
{
    unsigned int                                        index;
    std::queue<NetworkServerControllerThreadQueueEntry> queue;
    std::mutex                                          queue_mutex;
    std::mutex                                          start_mutex;
    std::condition_variable                             start_cv;
    std::thread *                                       thread;
    std::atomic<bool>                                   online;
} NetworkServerControllerThread;

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

    /*-----------------------------------------------------*\
    | Server Information functions                          |
    \*-----------------------------------------------------*/
    std::string                         GetHost();
    unsigned short                      GetPort();
    bool                                GetOnline();
    bool                                GetListening();
    unsigned int                        GetNumClients();
    const char *                        GetClientString(unsigned int client_num);
    const char *                        GetClientIP(unsigned int client_num);
    unsigned int                        GetClientProtocolVersion(unsigned int client_num);

    /*-----------------------------------------------------*\
    | Callback functions                                    |
    \*-----------------------------------------------------*/
    void                                RegisterClientInfoChangeCallback(NetServerCallback, void * new_callback_arg);
    void                                RegisterServerListeningChangeCallback(NetServerCallback, void * new_callback_arg);

    /*-----------------------------------------------------*\
    | Functions for forwarding callback sigals over network |
    \*-----------------------------------------------------*/
    void                                SignalResourceManagerUpdate(unsigned int );

    /*-----------------------------------------------------*\
    | Server Configuration functions                        |
    \*-----------------------------------------------------*/
    void                                SetHost(std::string host);
    void                                SetLegacyWorkaroundEnable(bool enable);
    void                                SetName(std::string new_name);
    void                                SetPort(unsigned short new_port);

    /*-----------------------------------------------------*\
    | Server Control functions                              |
    \*-----------------------------------------------------*/
    void                                StartServer();
    void                                StopServer();

    /*-----------------------------------------------------*\
    | Server Interface functions                            |
    \*-----------------------------------------------------*/
    void                                SetPluginManager(PluginManagerInterface* plugin_manager_pointer);
    void                                SetProfileManager(ProfileManagerInterface* profile_manager_pointer);
    void                                SetSettingsManager(SettingsManagerInterface* settings_manager_pointer);

    void                                SendRequest_RGBController_SignalUpdate(RGBController * controller_ptr, unsigned int update_reason);

private:
    /*-----------------------------------------------------*\
    | Server variables                                      |
    \*-----------------------------------------------------*/
    std::string                         host;
    bool                                legacy_workaround_enabled;
    unsigned short                      port_num;
    std::mutex                          send_in_progress;
    std::string                         server_name;
    std::atomic<bool>                   server_online;
    std::atomic<bool>                   server_listening;
    SOCKET                              server_sock[MAXSOCK];
    int                                 socket_count;

    /*-----------------------------------------------------*\
    | Server controller list                                |
    \*-----------------------------------------------------*/
    std::vector<RGBController *>&                   controllers;
    std::vector<NetworkServerControllerThread *>    controller_threads;

    /*-----------------------------------------------------*\
    | Server clients                                        |
    \*-----------------------------------------------------*/
    std::mutex                          ServerClientsMutex;
    std::vector<NetworkClientInfo *>    ServerClients;
    std::thread *                       ConnectionThread[MAXSOCK];

    /*-----------------------------------------------------*\
    | Client information change callbacks                   |
    \*-----------------------------------------------------*/
    std::mutex                          ClientInfoChangeMutex;
    std::vector<NetServerCallback>      ClientInfoChangeCallbacks;
    std::vector<void *>                 ClientInfoChangeCallbackArgs;

    /*-----------------------------------------------------*\
    | Server listening change callbacks                     |
    \*-----------------------------------------------------*/
    std::mutex                          ServerListeningChangeMutex;
    std::vector<NetServerCallback>      ServerListeningChangeCallbacks;
    std::vector<void *>                 ServerListeningChangeCallbackArgs;

    /*-----------------------------------------------------*\
    | Pointers to components that integrate with server     |
    \*-----------------------------------------------------*/
    PluginManagerInterface*             plugin_manager;
    ProfileManagerInterface*            profile_manager;
    SettingsManagerInterface*           settings_manager;

private:
#ifdef WIN32
    /*-----------------------------------------------------*\
    | Windows-specific WSA data                             |
    \*-----------------------------------------------------*/
    WSADATA                             wsa;
#endif

    /*-----------------------------------------------------*\
    | Server callback signal functions                      |
    \*-----------------------------------------------------*/
    void                                SignalClientInfoChanged();
    void                                SignalDetectionCompleted();
    void                                SignalDetectionProgress();
    void                                SignalDetectionStarted();
    void                                SignalDeviceListUpdated();
    void                                SignalServerListeningChanged();

    /*-----------------------------------------------------*\
    | Server Thread functions                               |
    \*-----------------------------------------------------*/
    void                                ConnectionThreadFunction(int socket_idx);
    void                                ControllerListenThread(NetworkServerControllerThread * this_thread);
    void                                ListenThreadFunction(NetworkClientInfo * client_sock);

    /*-----------------------------------------------------*\
    | Server Protocol functions                             |
    \*-----------------------------------------------------*/
    void                                ProcessRequest_ClientProtocolVersion(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_ClientString(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_RescanDevices();

    void                                ProcessRequest_RGBController_AddSegment(std::size_t controller_idx, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_UpdateLEDs(std::size_t controller_idx, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_UpdateSaveMode(std::size_t controller_idx, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_UpdateSingleLED(std::size_t controller_idx, unsigned char * data_ptr);
    void                                ProcessRequest_RGBController_UpdateZoneLEDs(std::size_t controller_idx, unsigned char* data_ptr);
    void                                ProcessRequest_RGBController_UpdateZoneMode(std::size_t controller_idx, unsigned char * data_ptr, unsigned int protocol_version);

    void                                SendReply_ControllerCount(SOCKET client_sock);
    void                                SendReply_ControllerData(SOCKET client_sock, unsigned int dev_idx, unsigned int protocol_version);
    void                                SendReply_ProtocolVersion(SOCKET client_sock);
    void                                SendReply_ServerString(SOCKET client_sock);

    void                                SendReply_ProfileList(SOCKET client_sock);
    void                                SendReply_PluginList(SOCKET client_sock);
    void                                SendReply_PluginSpecific(SOCKET client_sock, unsigned int pkt_type, unsigned char* data, unsigned int data_size);

    void                                SendRequest_DetectionCompleted(SOCKET client_sock, unsigned int protocol_version);
    void                                SendRequest_DetectionProgress(SOCKET client_sock, unsigned int protocol_version, unsigned int detection_percent, std::string detection_string);
    void                                SendRequest_DetectionStarted(SOCKET client_sock, unsigned int protocol_version);
    void                                SendRequest_DeviceListChanged(SOCKET client_sock);

    /*-----------------------------------------------------*\
    | Private helper functions                              |
    \*-----------------------------------------------------*/
    int                                 accept_select(int sockfd);
    int                                 recv_select(SOCKET s, char *buf, int len, int flags);
};

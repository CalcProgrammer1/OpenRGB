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
    NetPacketHeader             header;
    char *                      data;
    SOCKET                      client_sock;
    unsigned int                client_protocol_version;
} NetworkServerControllerThreadQueueEntry;

typedef struct
{
    unsigned int                                        id;
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
    unsigned int    client_flags;
    std::thread *   client_listen_thread;
    std::string     client_string;
    unsigned int    client_protocol_version;
    std::string     client_ip;
    bool            client_is_local;
    bool            client_is_local_client;
};

typedef struct
{
    RGBController * controller;
    unsigned int    id;
} NetworkControllerID;

class NetworkServer
{
public:
    NetworkServer();
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
    | Signal that device list has been updated              |
    \*-----------------------------------------------------*/
    void                                DeviceListUpdated();

    /*-----------------------------------------------------*\
    | Callback functions                                    |
    \*-----------------------------------------------------*/
    void                                RegisterClientInfoChangeCallback(NetServerCallback, void * new_callback_arg);
    void                                RegisterServerListeningChangeCallback(NetServerCallback, void * new_callback_arg);

    /*-----------------------------------------------------*\
    | Functions for forwarding callback sigals over network |
    \*-----------------------------------------------------*/
    void                                SignalProfileManagerUpdate(unsigned int update_reason);
    void                                SignalResourceManagerUpdate(unsigned int update_reason);

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
    void                                SetControllers(std::vector<RGBController *>);
    void                                SetPluginManager(PluginManagerInterface* plugin_manager_pointer);
    void                                SetProfileManager(ProfileManagerInterface* profile_manager_pointer);
    void                                SetSettingsManager(SettingsManagerInterface* settings_manager_pointer);

    void                                ProfileManager_ProfileAboutToLoad();

    void                                SendRequest_ProfileManager_ActiveProfileChanged(std::string profile_name);
    void                                SendRequest_ProfileManager_ProfileLoaded(std::string profile_json_string);
    void                                SendRequest_RGBController_SignalUpdate(RGBController * controller_ptr, unsigned int update_reason);

private:
    /*-----------------------------------------------------*\
    | Server variables                                      |
    \*-----------------------------------------------------*/
    std::string                         host;
    bool                                legacy_workaround_enabled;
    unsigned short                      port_num;
    std::mutex                          send_in_progress;
    unsigned int                        server_flags;
    std::string                         server_name;
    std::atomic<bool>                   server_online;
    std::atomic<bool>                   server_listening;
    SOCKET                              server_sock[MAXSOCK];
    int                                 socket_count;

    /*-----------------------------------------------------*\
    | Server controller list                                |
    \*-----------------------------------------------------*/
    std::vector<NetworkControllerID>                controller_ids;
    std::shared_mutex                               controller_ids_mutex;
    unsigned int                                    controller_next_idx;
    std::vector<RGBController *>                    controllers;
    std::vector<NetworkServerControllerThread *>    controller_threads;
    std::shared_mutex                               controller_threads_mutex;
    bool                                            controller_updating;

    NetworkServerControllerThread *                 profilemanager_thread;

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

    unsigned int                        profile_about_to_load_acks;
    unsigned int                        profile_about_to_load_count;
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
    void                                SignalActiveProfileChanged();
    void                                SignalClientInfoChanged();
    void                                SignalDetectionCompleted();
    void                                SignalDetectionProgress();
    void                                SignalDetectionStarted();
    void                                SignalDeviceListUpdated();
    void                                SignalProfileListUpdated();
    void                                SignalServerListeningChanged();

    /*-----------------------------------------------------*\
    | Server Thread functions                               |
    \*-----------------------------------------------------*/
    void                                ConnectionThreadFunction(int socket_idx);
    void                                ControllerListenThread(NetworkServerControllerThread * this_thread);
    void                                ListenThreadFunction(NetworkClientInfo * client_sock);
    void                                ProfileManagerListenThread(NetworkServerControllerThread * this_thread);

    /*-----------------------------------------------------*\
    | Server Protocol functions                             |
    \*-----------------------------------------------------*/
    void                                ProcessRequest_ClientFlags(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_ClientProtocolVersion(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_ClientString(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_RescanDevices();

    void                                ProcessRequest_ProfileManager_ClearActiveProfile();
    void                                ProcessRequest_ProfileManager_DeleteProfile(unsigned int data_size, char * data);
    void                                ProcessRequest_ProfileManager_DownloadProfile(SOCKET client_sock, unsigned int data_size, char * data);
    void                                ProcessRequest_ProfileManager_GetActiveProfile(SOCKET client_sock);
    void                                ProcessRequest_ProfileManager_LoadProfile(unsigned int data_size, char * data);
    void                                ProcessRequest_ProfileManager_SaveProfile(unsigned int data_size, char * data);
    void                                ProcessRequest_ProfileManager_UploadProfile(unsigned int data_size, char * data);

    void                                ProcessRequest_RGBController_AddSegment(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_ClearSegments(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_ResizeZone(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_SetCustomMode(unsigned int controller_id, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_UpdateLEDs(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_UpdateSaveMode(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version, bool save_mode);
    void                                ProcessRequest_RGBController_UpdateSingleLED(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_UpdateZoneLEDs(unsigned int controller_id, unsigned char* data_ptr, unsigned int protocol_version);
    void                                ProcessRequest_RGBController_UpdateZoneMode(unsigned int controller_id, unsigned char * data_ptr, unsigned int protocol_version);

    void                                SendReply_ControllerCount(SOCKET client_sock, unsigned int protocol_version);
    void                                SendReply_ControllerData(SOCKET client_sock, unsigned int dev_id, unsigned int protocol_version);
    void                                SendReply_ProtocolVersion(SOCKET client_sock);
    void                                SendReply_ServerFlags(SOCKET client_sock);
    void                                SendReply_ServerString(SOCKET client_sock);

    void                                SendReply_ProfileList(SOCKET client_sock);
    void                                SendReply_PluginList(SOCKET client_sock);
    void                                SendReply_PluginSpecific(SOCKET client_sock, unsigned int pkt_type, unsigned char* data, unsigned int data_size);

    void                                SendRequest_DetectionCompleted(SOCKET client_sock, unsigned int protocol_version);
    void                                SendRequest_DetectionProgress(SOCKET client_sock, unsigned int protocol_version, unsigned int detection_percent, std::string detection_string);
    void                                SendRequest_DetectionStarted(SOCKET client_sock, unsigned int protocol_version);
    void                                SendRequest_DeviceListChanged(SOCKET client_sock);

    void                                SendRequest_ProfileManager_ActiveProfileChanged(SOCKET client_sock, std::string active_profile);
    void                                SendRequest_ProfileManager_ProfileAboutToLoad();
    void                                SendRequest_ProfileManager_ProfileListChanged(SOCKET client_sock, unsigned char *profile_list_description);

    /*-----------------------------------------------------*\
    | Private helper functions                              |
    \*-----------------------------------------------------*/
    int                                 accept_select(int sockfd);
    unsigned int                        index_from_id(unsigned int id, unsigned int protocol_version, bool* index_valid);
    int                                 recv_select(SOCKET s, char *buf, int len, int flags);
};

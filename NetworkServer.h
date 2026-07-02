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
#include "LogManager.h"
#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"
#include "PluginManagerInterface.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

#define MAXSOCK 32
#define TCP_TIMEOUT_SECONDS 5

typedef void (*NetServerCallback)(void*);

class NetworkClientInfo
{
public:
    NetworkClientInfo();
    ~NetworkClientInfo();

    SOCKET          client_sock;
    unsigned int    client_flags;
    std::thread*    client_listen_thread;
    std::string     client_string;
    unsigned int    client_protocol_version;
    std::string     client_ip;
    bool            client_is_local;
    bool            client_is_local_client;
};

typedef struct
{
    RGBController*  controller;
    unsigned int    id;
} NetworkControllerID;

typedef struct
{
    NetPacketHeader             header;
    unsigned char*              data;
    NetworkClientInfo*          client_info;
} NetworkServerControllerThreadQueueEntry;

typedef struct
{
    unsigned int                                        id;
    unsigned int                                        index;
    std::queue<NetworkServerControllerThreadQueueEntry> queue;
    std::mutex                                          queue_mutex;
    std::mutex                                          start_mutex;
    std::condition_variable                             start_cv;
    std::thread*                                        thread;
    std::atomic<bool>                                   online;
} NetworkServerControllerThread;

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
    void                                RegisterClientInfoChangeCallback(NetServerCallback, void* new_callback_arg);
    void                                RegisterServerListeningChangeCallback(NetServerCallback, void* new_callback_arg);

    /*-----------------------------------------------------*\
    | Functions for forwarding callback sigals over network |
    \*-----------------------------------------------------*/
    void                                SignalLogManagerLoggedEntry(LogMessage& logged_entry);
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
    void                                SetControllers(std::vector<RGBController*>);
    void                                SetPluginManager(PluginManagerInterface* plugin_manager_pointer);
    void                                SetProfileManager(ProfileManagerInterface* profile_manager_pointer);
    void                                SetSettingsManager(SettingsManagerInterface* settings_manager_pointer);

    void                                ProfileManager_ProfileAboutToLoad();

    void                                SendRequest_ProfileManager_ActiveProfileChanged(std::string profile_name);
    void                                SendRequest_ProfileManager_ProfileLoaded(std::string profile_json_string);
    void                                SendRequest_RGBController_SignalUpdate(RGBController* controller_ptr, unsigned int update_reason);

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
    std::vector<RGBController*>                     controllers;
    std::vector<NetworkServerControllerThread*>     controller_threads;
    std::shared_mutex                               controller_threads_mutex;
    bool                                            controller_updating;

    NetworkServerControllerThread*                  profilemanager_thread;

    /*-----------------------------------------------------*\
    | Server clients                                        |
    \*-----------------------------------------------------*/
    std::mutex                          ServerClientsMutex;
    std::vector<NetworkClientInfo*>     ServerClients;
    std::thread*                        ConnectionThread[MAXSOCK];

    /*-----------------------------------------------------*\
    | Client information change callbacks                   |
    \*-----------------------------------------------------*/
    std::mutex                          ClientInfoChangeMutex;
    std::vector<NetServerCallback>      ClientInfoChangeCallbacks;
    std::vector<void*>                  ClientInfoChangeCallbackArgs;

    /*-----------------------------------------------------*\
    | Server listening change callbacks                     |
    \*-----------------------------------------------------*/
    std::mutex                          ServerListeningChangeMutex;
    std::vector<NetServerCallback>      ServerListeningChangeCallbacks;
    std::vector<void*>                  ServerListeningChangeCallbackArgs;

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
    void                                ControllerListenThread(NetworkServerControllerThread* this_thread);
    void                                ListenThreadFunction(NetworkClientInfo* client_info);
    void                                ProfileManagerListenThread(NetworkServerControllerThread* this_thread);

    /*-----------------------------------------------------*\
    | Server Protocol functions                             |
    \*-----------------------------------------------------*/
    NetPacketStatus                     ProcessRequest_ClientFlags(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_ClientProtocolVersion(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_ClientString(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_RescanDevices();

    NetPacketStatus                     ProcessRequest_GetHIDDeviceInfo(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_GetI2CBusInfo(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_GetUSBDeviceInfo(NetworkClientInfo* client_info);

    NetPacketStatus                     ProcessRequest_LogManager_ClearLogBuffer(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_LogManager_GetLogBuffer(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_LogManager_GetLogLevel(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_LogManager_SetLogLevel(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);

    NetPacketStatus                     ProcessRequest_ProfileManager_ClearActiveProfile(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_ProfileManager_DeleteProfile(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_ProfileManager_DownloadProfile(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_ProfileManager_GetActiveProfile(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_ProfileManager_GetProfileList(NetworkClientInfo* client_info);
    NetPacketStatus                     ProcessRequest_ProfileManager_LoadProfile(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_ProfileManager_SaveProfile(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_ProfileManager_UploadProfile(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);

    NetPacketStatus                     ProcessRequest_SettingsManager_GetSettings(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_SettingsManager_GetSettingsSchema(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_SettingsManager_ModifySettings(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_SettingsManager_SetSettings(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);
    NetPacketStatus                     ProcessRequest_SettingsManager_SaveSettings(NetworkClientInfo* client_info);

    NetPacketStatus                     ProcessRequest_RGBController_AddSegment(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_ClearSegments(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_ConfigureZone(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_ResizeZone(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_ConfigureDevice(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_SetCustomMode(NetworkClientInfo* client_info, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_SetDeviceSpecificConfiguration(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_SetDeviceSpecificZoneConfiguration(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_SetHidden(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_UpdateLEDs(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_UpdateSaveMode(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id, bool save_mode);
    NetPacketStatus                     ProcessRequest_RGBController_UpdateSingleLED(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_UpdateZoneLEDs(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);
    NetPacketStatus                     ProcessRequest_RGBController_UpdateZoneMode(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int controller_id);

    void                                SendAck(NetworkClientInfo* client_info, unsigned int acked_pkt_dev_id, unsigned int acked_pkt_id, NetPacketStatus status);

    void                                SendReply_ControllerCount(NetworkClientInfo* client_info);
    void                                SendReply_ControllerData(NetworkClientInfo* client_info, unsigned int dev_id, unsigned int protocol_version);
    void                                SendReply_ProtocolVersion(NetworkClientInfo* client_info);
    void                                SendReply_ServerFlags(NetworkClientInfo* client_info);
    void                                SendReply_ServerString(NetworkClientInfo* client_info);

    void                                SendReply_PluginList(NetworkClientInfo* client_info);
    void                                SendReply_PluginSpecific(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr, unsigned int pkt_id);

    void                                SendRequest_DetectionCompleted(NetworkClientInfo* client_info);
    void                                SendRequest_DetectionProgress(NetworkClientInfo* client_info, unsigned int detection_percent, std::string detection_string);
    void                                SendRequest_DetectionStarted(NetworkClientInfo* client_info);
    void                                SendRequest_DeviceListChanged(NetworkClientInfo* client_info);

    void                                SendRequest_LoggedEntry(NetworkClientInfo* client_info, unsigned int data_size, unsigned char* data_ptr);

    void                                SendRequest_ProfileManager_ActiveProfileChanged(NetworkClientInfo* client_info, std::string active_profile);
    void                                SendRequest_ProfileManager_ProfileAboutToLoad();
    void                                SendRequest_ProfileManager_ProfileListChanged(NetworkClientInfo* client_info, unsigned char* profile_list_description);

    /*-----------------------------------------------------*\
    | Private helper functions                              |
    \*-----------------------------------------------------*/
    int                                 accept_select(int sockfd);
    unsigned int                        index_from_id(unsigned int id, unsigned int protocol_version, bool* index_valid);
    int                                 recv_select(SOCKET s, char *buf, int len, int flags);
};

/*---------------------------------------------------------*\
| NetworkClient.h                                           |
|                                                           |
|   OpenRGB SDK network client                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <condition_variable>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

/*---------------------------------------------------------*\
| Callback Types                                            |
\*---------------------------------------------------------*/
typedef void (*NetworkClientCallback)(void *, unsigned int);

/*---------------------------------------------------------*\
| NetworkClient Update Reason Codes                         |
\*---------------------------------------------------------*/
enum
{
    NETWORKCLIENT_UPDATE_REASON_CLIENT_STARTED,                 /* Client started                   */
    NETWORKCLIENT_UPDATE_REASON_CLIENT_STOPPED,                 /* Client stopped                   */
    NETWORKCLIENT_UPDATE_REASON_CLIENT_CONNECTED,               /* Client connectedd                */
    NETWORKCLIENT_UPDATE_REASON_CLIENT_DISCONNECTED,            /* Client disconnected              */
    NETWORKCLIENT_UPDATE_REASON_SERVER_STRING_RECEIVED,         /* Server string received           */
    NETWORKCLIENT_UPDATE_REASON_PROTOCOL_NEGOTIATED,            /* Protocol version negotiated      */
    NETWORKCLIENT_UPDATE_REASON_DEVICE_LIST_UPDATED,            /* Device list updated              */
    NETWORKCLIENT_UPDATE_REASON_DETECTION_STARTED,              /* Detection started                */
    NETWORKCLIENT_UPDATE_REASON_DETECTION_PROGRESS_CHANGED,     /* Detection progress changed       */
    NETWORKCLIENT_UPDATE_REASON_DETECTION_COMPLETE,             /* Detection completed              */
};



class NetworkClient
{
public:
    NetworkClient();
    ~NetworkClient();

    /*-----------------------------------------------------*\
    | Client Information functions                          |
    \*-----------------------------------------------------*/
    bool                                GetConnected();
    std::string                         GetIP();
    unsigned short                      GetPort();
    unsigned int                        GetProtocolVersion();
    bool                                GetOnline();
    std::string                         GetServerName();

    /*-----------------------------------------------------*\
    | Client Control functions                              |
    \*-----------------------------------------------------*/
    void                                SetIP(std::string new_ip);
    void                                SetName(std::string new_name);
    void                                SetPort(unsigned short new_port);

    void                                StartClient();
    void                                StopClient();

    void                                SendRequest_ControllerData(unsigned int dev_idx);
    void                                SendRequest_RescanDevices();

    /*-----------------------------------------------------*\
    | Client Callback functions                             |
    \*-----------------------------------------------------*/
    void                                ClearCallbacks();
    void                                RegisterNetworkClientCallback(NetworkClientCallback new_callback, void * new_callback_arg);

    /*-----------------------------------------------------*\
    | DetectionManager functions                            |
    \*-----------------------------------------------------*/
    unsigned int                        DetectionManager_GetDetectionPercent();
    std::string                         DetectionManager_GetDetectionString();

    /*-----------------------------------------------------*\
    | ProfileManager functions                              |
    \*-----------------------------------------------------*/
    char *                              ProfileManager_GetProfileList();
    void                                ProfileManager_LoadProfile(std::string profile_name);
    void                                ProfileManager_SaveProfile(std::string profile_name);
    void                                ProfileManager_DeleteProfile(std::string profile_name);
    void                                ProfileManager_UploadProfile(std::string profile_json_str);
    std::string                         ProfileManager_DownloadProfile(std::string profile_name);
    std::string                         ProfileManager_GetActiveProfile();

    /*-----------------------------------------------------*\
    | SettingsManager functions                             |
    \*-----------------------------------------------------*/
    std::string                         SettingsManager_GetSettings(std::string settings_key);
    void                                SettingsManager_SaveSettings();
    void                                SettingsManager_SetSettings(std::string settings_json_str);

    /*-----------------------------------------------------*\
    | RGBController functions                               |
    \*-----------------------------------------------------*/
    std::vector<RGBController*>&        GetRGBControllers();

    void                                SendRequest_RGBController_ClearSegments(unsigned int dev_idx, int zone);
    void                                SendRequest_RGBController_AddSegment(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void                                SendRequest_RGBController_ResizeZone(unsigned int dev_idx, int zone, int new_size);

    void                                SendRequest_RGBController_UpdateLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void                                SendRequest_RGBController_UpdateZoneLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void                                SendRequest_RGBController_UpdateSingleLED(unsigned int dev_idx, unsigned char * data, unsigned int size);

    void                                SendRequest_RGBController_SetCustomMode(unsigned int dev_idx);

    void                                SendRequest_RGBController_UpdateMode(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void                                SendRequest_RGBController_UpdateZoneMode(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void                                SendRequest_RGBController_SaveMode(unsigned int dev_idx, unsigned char * data, unsigned int size);

    void                                WaitOnControllerData();

private:
    /*-----------------------------------------------------*\
    | Client state variables                                |
    \*-----------------------------------------------------*/
    std::atomic<bool>                   client_active;
    bool                                client_string_sent;
    bool                                controller_data_received;
    bool                                controller_data_requested;
    bool                                protocol_initialized;
    bool                                change_in_progress;
    unsigned int                        requested_controllers;
    std::mutex                          send_in_progress;

    NetPacketHeader                     response_header;
    char *                              response_data_ptr;
    std::mutex                          waiting_on_response_mutex;
    std::condition_variable             waiting_on_response_cv;

    /*-----------------------------------------------------*\
    | Client information                                    |
    \*-----------------------------------------------------*/
    std::string                         client_name;
    SOCKET                              client_sock;
    net_port                            port;
    std::string                         port_ip;
    unsigned short                      port_num;

    /*-----------------------------------------------------*\
    | Server information                                    |
    \*-----------------------------------------------------*/
    std::string                         server_name;
    bool                                server_connected;
    bool                                server_initialized;
    bool                                server_reinitialize;
    unsigned int                        server_controller_count;
    bool                                server_controller_count_requested;
    bool                                server_controller_count_received;
    unsigned int                        server_protocol_version;
    bool                                server_protocol_version_received;

    /*-----------------------------------------------------*\
    | Client threads                                        |
    \*-----------------------------------------------------*/
    std::mutex                          connection_mutex;
    std::condition_variable             connection_cv;
    std::thread *                       ConnectionThread;
    std::thread *                       ListenThread;

    /*-----------------------------------------------------*\
    | Callbacks                                             |
    \*-----------------------------------------------------*/
    std::mutex                          NetworkClientCallbackMutex;
    std::vector<NetworkClientCallback>  NetworkClientCallbacks;
    std::vector<void *>                 NetworkClientCallbackArgs;

    /*-----------------------------------------------------*\
    | Controller list                                       |
    \*-----------------------------------------------------*/
    std::mutex                          ControllerListMutex;
    std::vector<RGBController *>        server_controllers;

    /*-----------------------------------------------------*\
    | Detection variables                                   |
    \*-----------------------------------------------------*/
    unsigned int                        detection_percent;
    std::string                         detection_string;

    /*-----------------------------------------------------*\
    | Client callback signal functions                      |
    \*-----------------------------------------------------*/
    void                                SignalNetworkClientUpdate(unsigned int update_reason);

    /*-----------------------------------------------------*\
    | Client thread functions                               |
    \*-----------------------------------------------------*/
    void                                ConnectionThreadFunction();
    void                                ListenThreadFunction();

    /*-----------------------------------------------------*\
    | Private Client functions                              |
    \*-----------------------------------------------------*/
    void                                ProcessReply_ControllerCount(unsigned int data_size, char * data);
    void                                ProcessReply_ControllerData(unsigned int data_size, char * data, unsigned int dev_idx);
    void                                ProcessReply_ProtocolVersion(unsigned int data_size, char * data);
    void                                ProcessRequest_DetectionProgressChanged(unsigned int data_size, char * data);
    void                                ProcessRequest_DeviceListChanged();
    void                                ProcessRequest_RGBController_SignalUpdate(unsigned int data_size, char * data, unsigned int dev_idx);
    void                                ProcessRequest_ServerString(unsigned int data_size, char * data);

    void                                SendData_ClientString();
    void                                SendRequest_ControllerCount();
    void                                SendRequest_ProtocolVersion();

    /*-----------------------------------------------------*\
    | Private ProfileManager functions                      |
    \*-----------------------------------------------------*/
    std::vector<std::string> *          ProcessReply_ProfileList(unsigned int data_size, char * data);

    /*-----------------------------------------------------*\
    | Private helper functions                              |
    \*-----------------------------------------------------*/
    int                                 recv_select(SOCKET s, char *buf, int len, int flags);
};

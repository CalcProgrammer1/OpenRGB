/*---------------------------------------------------------*\
| NetworkClient.h                                           |
|                                                           |
|   OpenRGB SDK network client                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <mutex>
#include <thread>
#include <condition_variable>
#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

typedef void (*NetClientCallback)(void *);

class NetworkClient
{
public:
    NetworkClient(std::vector<RGBController *>& control);
    ~NetworkClient();

    void            ClientInfoChanged();

    bool            GetConnected();
    std::string     GetIP();
    unsigned short  GetPort();
    unsigned int    GetProtocolVersion();
    bool            GetOnline();

    void            ClearCallbacks();
    void            RegisterClientInfoChangeCallback(NetClientCallback new_callback, void * new_callback_arg);

    void            SetIP(std::string new_ip);
    void            SetName(std::string new_name);
    void            SetPort(unsigned short new_port);

    void            StartClient();
    void            StopClient();

    void            ConnectionThreadFunction();
    void            ListenThreadFunction();

    void            WaitOnControllerData();

    void        ProcessReply_ControllerCount(unsigned int data_size, char * data);
    void        ProcessReply_ControllerData(unsigned int data_size, char * data, unsigned int dev_idx);
    void        ProcessReply_ProtocolVersion(unsigned int data_size, char * data);

    void        ProcessRequest_DeviceListChanged();

    void        SendData_ClientString();

    void        SendRequest_ControllerCount();
    void        SendRequest_ControllerData(unsigned int dev_idx);
    void        SendRequest_ProtocolVersion();

    void        SendRequest_RescanDevices();

    void        SendRequest_RGBController_ClearSegments(unsigned int dev_idx, int zone);
    void        SendRequest_RGBController_AddSegment(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_ResizeZone(unsigned int dev_idx, int zone, int new_size);

    void        SendRequest_RGBController_UpdateLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_UpdateZoneLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_UpdateSingleLED(unsigned int dev_idx, unsigned char * data, unsigned int size);

    void        SendRequest_RGBController_SetCustomMode(unsigned int dev_idx);

    void        SendRequest_RGBController_UpdateMode(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_SaveMode(unsigned int dev_idx, unsigned char * data, unsigned int size);


    std::vector<std::string> * ProcessReply_ProfileList(unsigned int data_size, char * data);

    void        SendRequest_GetProfileList();
    void        SendRequest_LoadProfile(std::string profile_name);
    void        SendRequest_SaveProfile(std::string profile_name);
    void        SendRequest_DeleteProfile(std::string profile_name);

    std::vector<RGBController *>  server_controllers;

    std::mutex                          ControllerListMutex;

protected:
    std::vector<RGBController *>& controllers;


private:
    SOCKET          client_sock;
    std::string     client_name;
    net_port        port;
    std::string     port_ip;
    unsigned short  port_num;
    std::atomic<bool> client_active;
    bool            client_string_sent;
    bool            controller_data_received;
    bool            controller_data_requested;
    bool            protocol_initialized;
    bool            server_connected;
    bool            server_initialized;
    bool            server_reinitialize;
    unsigned int    server_controller_count;
    bool            server_controller_count_requested;
    bool            server_controller_count_received;
    unsigned int    server_protocol_version;
    bool            server_protocol_version_received;
    bool            change_in_progress;
    unsigned int    requested_controllers;
    std::mutex      send_in_progress;

    std::mutex      connection_mutex;
    std::condition_variable connection_cv;

    std::thread *   ConnectionThread;
    std::thread *   ListenThread;

    std::mutex                          ClientInfoChangeMutex;
    std::vector<NetClientCallback>      ClientInfoChangeCallbacks;
    std::vector<void *>                 ClientInfoChangeCallbackArgs;

    int recv_select(SOCKET s, char *buf, int len, int flags);
};

/*-----------------------------------------*\
|  NetworkClient.h                          |
|                                           |
|  Client header for OpenRGB SDK            |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/9/2020    |
\*-----------------------------------------*/

#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#include <mutex>
#include <thread>

#pragma once

typedef void (*NetClientCallback)(void *);

class NetworkClient
{
public:
    NetworkClient(std::vector<RGBController *>& control);
    ~NetworkClient();

    void            ClientInfoChanged();

    bool            GetConnected();
    const char *    GetIP();
    unsigned short  GetPort();
    unsigned int    GetProtocolVersion();
    bool            GetOnline();

    void            ClearCallbacks();
    void            RegisterClientInfoChangeCallback(NetClientCallback new_callback, void * new_callback_arg);

    void            SetIP(const char *new_ip);
    void            SetName(const char *new_name);
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

    void        SendRequest_RGBController_ResizeZone(unsigned int dev_idx, int zone, int new_size);

    void        SendRequest_RGBController_UpdateLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_UpdateZoneLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_UpdateSingleLED(unsigned int dev_idx, unsigned char * data, unsigned int size);

    void        SendRequest_RGBController_SetCustomMode(unsigned int dev_idx);

    void        SendRequest_RGBController_UpdateMode(unsigned int dev_idx, unsigned char * data, unsigned int size);


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
    char            port_ip[20];
    unsigned short  port_num;
    bool            client_active;
    bool            controller_data_received;
    bool            server_connected;
    bool            server_initialized;
    unsigned int    server_controller_count;
    bool            server_controller_count_received;
    unsigned int    server_protocol_version;
    bool            server_protocol_version_received;
    bool            change_in_progress;

    std::thread *   ConnectionThread;
    std::thread *   ListenThread;

    std::mutex                          ClientInfoChangeMutex;
    std::vector<NetClientCallback>      ClientInfoChangeCallbacks;
    std::vector<void *>                 ClientInfoChangeCallbackArgs;

    int recv_select(SOCKET s, char *buf, int len, int flags);
};

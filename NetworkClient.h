#include "RGBController.h"
#include "NetworkProtocol.h"
#include "net_port.h"

#include <thread>

#pragma once

class NetworkClient
{
public:
    NetworkClient(std::vector<RGBController *>& control);

    const char *    GetIP();
    unsigned short  GetPort();
    bool            GetOnline();

    void            SetIP(const char *new_ip);
    void            SetName(const char *new_name);
    void            SetPort(unsigned short new_port);

    void            StartClient();
    void            StopClient();

    void            ConnectionThreadFunction();
    void            ListenThreadFunction();

    void        ProcessReply_ControllerCount(unsigned int data_size, char * data);
    void        ProcessReply_ControllerData(unsigned int data_size, char * data, unsigned int dev_idx);
    
    void        SendData_ClientString();

    void        SendRequest_ControllerCount();
    void        SendRequest_ControllerData(unsigned int dev_idx);

    void        SendRequest_RGBController_ResizeZone(unsigned int dev_idx, int zone, int new_size);

    void        SendRequest_RGBController_UpdateLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_UpdateZoneLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size);
    void        SendRequest_RGBController_UpdateSingleLED(unsigned int dev_idx, unsigned char * data, unsigned int size);

    void        SendRequest_RGBController_SetCustomMode(unsigned int dev_idx);

    void        SendRequest_RGBController_UpdateMode(unsigned int dev_idx, unsigned char * data, unsigned int size);

protected:
    std::vector<RGBController *>& controllers;
    std::vector<RGBController *>  server_controllers;

private:
    std::string     client_name;
    net_port        port;
    char            port_ip[20];
    unsigned short  port_num;
    bool            server_connected;
    unsigned int    server_controller_count;

    std::thread *   ConnectionThread;
    std::thread *   ListenThread;
};

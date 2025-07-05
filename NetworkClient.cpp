/*---------------------------------------------------------*\
| NetworkClient.cpp                                         |
|                                                           |
|   OpenRGB SDK network client                              |
|                                                           |
|   Adam Honse (CalcProgrammer1)                09 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "NetworkClient.h"
#include "RGBController_Network.h"

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
#endif

using namespace std::chrono_literals;

NetworkClient::NetworkClient(std::vector<RGBController *>& control) : controllers(control)
{
    port_ip                             = "127.0.0.1";
    port_num                            = OPENRGB_SDK_PORT;
    client_string_sent                  = false;
    client_sock                         = -1;
    protocol_initialized                = false;
    server_connected                    = false;
    server_controller_count             = 0;
    server_controller_count_requested   = false;
    server_controller_count_received    = false;
    server_protocol_version             = 0;
    server_reinitialize                 = false;
    change_in_progress                  = false;

    ListenThread            = NULL;
    ConnectionThread        = NULL;
}

NetworkClient::~NetworkClient()
{
    StopClient();
}

void NetworkClient::ClearCallbacks()
{
    ClientInfoChangeCallbacks.clear();
    ClientInfoChangeCallbackArgs.clear();
}

void NetworkClient::ClientInfoChanged()
{
    ClientInfoChangeMutex.lock();
    ControllerListMutex.lock();

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < ClientInfoChangeCallbacks.size(); callback_idx++)
    {
        ClientInfoChangeCallbacks[callback_idx](ClientInfoChangeCallbackArgs[callback_idx]);
    }

    ControllerListMutex.unlock();
    ClientInfoChangeMutex.unlock();
}

std::string NetworkClient::GetIP()
{
    return port_ip;
}

unsigned short NetworkClient::GetPort()
{
    return port_num;
}

unsigned int NetworkClient::GetProtocolVersion()
{
    unsigned int protocol_version = 0;

    if(server_protocol_version > OPENRGB_SDK_PROTOCOL_VERSION)
    {
        protocol_version = OPENRGB_SDK_PROTOCOL_VERSION;
    }
    else
    {
        protocol_version = server_protocol_version;
    }

    return(protocol_version);
}

bool NetworkClient::GetConnected()
{
    return(server_connected);
}

bool NetworkClient::GetOnline()
{
    return(server_connected && client_string_sent && protocol_initialized && server_initialized);
}

void NetworkClient::RegisterClientInfoChangeCallback(NetClientCallback new_callback, void * new_callback_arg)
{
    ClientInfoChangeCallbacks.push_back(new_callback);
    ClientInfoChangeCallbackArgs.push_back(new_callback_arg);
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
    /*---------------------------------------------------------*\
    | Start a TCP server and launch threads                     |
    \*---------------------------------------------------------*/
    char port_str[6];
    snprintf(port_str, 6, "%d", port_num);

    port.tcp_client(port_ip.c_str(), port_str);

    client_active = true;

    /*---------------------------------------------------------*\
    | Start the connection thread                               |
    \*---------------------------------------------------------*/
    ConnectionThread = new std::thread(&NetworkClient::ConnectionThreadFunction, this);

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    ClientInfoChanged();
}

void NetworkClient::StopClient()
{
    /*---------------------------------------------------------*\
    | Disconnect the server and set it as inactive              |
    \*---------------------------------------------------------*/
    server_connected = false;
    client_active    = false;

    /*---------------------------------------------------------*\
    | Shut down and close the client socket                     |
    \*---------------------------------------------------------*/
    if(server_connected)
    {
        shutdown(client_sock, SD_RECEIVE);
        closesocket(client_sock);
    }

    client_active    = false;
    server_connected = false;

    /*---------------------------------------------------------*\
    | Close the listen thread                                   |
    \*---------------------------------------------------------*/
    if(ListenThread)
    {
        ListenThread->join();
        delete ListenThread;
        ListenThread = nullptr;
    }

    /*---------------------------------------------------------*\
    | Close the connection thread                               |
    \*---------------------------------------------------------*/
    if(ConnectionThread)
    {
        connection_cv.notify_all();
        ConnectionThread->join();
        delete ConnectionThread;
        ConnectionThread = nullptr;
    }

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    ClientInfoChanged();
}

void NetworkClient::ConnectionThreadFunction()
{
    std::unique_lock<std::mutex> lock(connection_mutex);

    /*---------------------------------------------------------*\
    | This thread manages the connection to the server          |
    \*---------------------------------------------------------*/
    while(client_active == true)
    {
        if(server_connected == false)
        {
            /*---------------------------------------------------------*\
            | Connect to server and reconnect if the connection is lost |
            \*---------------------------------------------------------*/
            server_initialized = false;

            /*---------------------------------------------------------*\
            | Try to connect to server                                  |
            \*---------------------------------------------------------*/
            if(port.tcp_client_connect() == true)
            {
                client_sock = port.sock;
                printf( "Connected to server\n" );

                /*---------------------------------------------------------*\
                | Server is now connected                                   |
                \*---------------------------------------------------------*/
                server_connected = true;

                /*---------------------------------------------------------*\
                | Start the listener thread                                 |
                \*---------------------------------------------------------*/
                ListenThread = new std::thread(&NetworkClient::ListenThreadFunction, this);

                /*---------------------------------------------------------*\
                | Server is not initialized                                 |
                \*---------------------------------------------------------*/
                server_initialized = false;

                /*---------------------------------------------------------*\
                | Client info has changed, call the callbacks               |
                \*---------------------------------------------------------*/
                ClientInfoChanged();
            }
            else
            {
                printf( "Connection attempt failed\n" );
            }
        }

        /*-------------------------------------------------------------*\
        | Double-check client_active as it could have changed           |
        \*-------------------------------------------------------------*/
        if(client_active && ( protocol_initialized == false || client_string_sent == false || server_initialized == false ) && server_connected == true)
        {
            /*---------------------------------------------------------*\
            | Initialize protocol version if it hasn't already been     |
            | initialized                                               |
            \*---------------------------------------------------------*/
            if(!protocol_initialized)
            {
                /*-----------------------------------------------------*\
                | Request protocol version                              |
                \*-----------------------------------------------------*/
                SendRequest_ProtocolVersion();

                /*-----------------------------------------------------*\
                | Wait up to 1s for protocol version reply              |
                \*-----------------------------------------------------*/
                unsigned int timeout_counter = 0;

                while(!server_protocol_version_received)
                {
                    connection_cv.wait_for(lock, 5ms);
                    if(!client_active)
                    {
                        break;
                    }

                    timeout_counter++;

                    /*-------------------------------------------------*\
                    | If no protocol version received within 1s, assume |
                    | the server doesn't support protocol versioning    |
                    | and use protocol version 0                        |
                    \*-------------------------------------------------*/
                    if(timeout_counter > 200)
                    {
                        server_protocol_version          = 0;
                        server_protocol_version_received = true;
                    }
                }

                protocol_initialized = true;
            }

            /*---------------------------------------------------------*\
            | Send client string if it hasn't already been sent         |
            \*---------------------------------------------------------*/
            if(!client_string_sent)
            {
                /*-----------------------------------------------------*\
                | Once server is connected, send client string          |
                \*-----------------------------------------------------*/
                SendData_ClientString();

                client_string_sent = true;
            }

            /*---------------------------------------------------------*\
            | Initialize the server device list if it hasn't already    |
            | been initialized                                          |
            \*---------------------------------------------------------*/
            if(!server_initialized)
            {
                /*-----------------------------------------------------*\
                | Request the server controller count                   |
                \*-----------------------------------------------------*/
                if(!server_controller_count_requested)
                {
                    SendRequest_ControllerCount();

                    server_controller_count_requested = true;
                }
                else
                {
                    /*-------------------------------------------------*\
                    | Wait for the server controller count to be        |
                    | received                                          |
                    \*-------------------------------------------------*/
                    if(server_controller_count_received)
                    {
                        /*---------------------------------------------*\
                        | Once count is received, request controllers   |
                        | When data is received, increment count of     |
                        | requested controllers until all controllers   |
                        | have been received                            |
                        \*---------------------------------------------*/
                        if(requested_controllers < server_controller_count)
                        {
                            if(!controller_data_requested)
                            {
                                printf("Client: Requesting controller %d\r\n", requested_controllers);

                                controller_data_received = false;
                                SendRequest_ControllerData(requested_controllers);

                                controller_data_requested = true;
                            }

                            if(controller_data_received)
                            {
                                requested_controllers++;
                                controller_data_requested = false;
                            }
                        }
                        else
                        {
                            ControllerListMutex.lock();

                            /*-----------------------------------------*\
                            | All controllers received, add them to     |
                            | master list                               |
                            \*-----------------------------------------*/
                            printf("Client: All controllers received, adding them to master list\r\n");
                            for(std::size_t controller_idx = 0; controller_idx < server_controllers.size(); controller_idx++)
                            {
                                controllers.push_back(server_controllers[controller_idx]);
                            }

                            ControllerListMutex.unlock();

                            /*-----------------------------------------*\
                            | Client info has changed, call the         |
                            | callbacks                                 |
                            \*-----------------------------------------*/
                            ClientInfoChanged();

                            server_initialized = true;
                        }
                    }
                }
            }

            /*---------------------------------------------------------*\
            | Wait 1 ms or until the thread is requested to stop        |
            \*---------------------------------------------------------*/
            connection_cv.wait_for(lock, 1ms);
        }
        else
        {
            /*---------------------------------------------------------*\
            | Wait 1 sec or until the thread is requested to stop       |
            \*---------------------------------------------------------*/
            connection_cv.wait_for(lock, 1s);
        }
    }
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
            return(recv(s, buf, len, flags));
        }

    }
}

void NetworkClient::ListenThreadFunction()
{
    printf("Network client listener started\n");

    /*---------------------------------------------------------*\
    | This thread handles messages received from the server     |
    \*---------------------------------------------------------*/
    while(server_connected == true)
    {
        NetPacketHeader header;
        int             bytes_read  = 0;
        char *          data        = NULL;

        for(unsigned int i = 0; i < 4; i++)
        {
            /*---------------------------------------------------------*\
            | Read byte of magic                                        |
            \*---------------------------------------------------------*/
            bytes_read = recv_select(client_sock, &header.pkt_magic[i], 1, 0);

            if(bytes_read <= 0)
            {
                goto listen_done;
            }

            /*---------------------------------------------------------*\
            | Test characters of magic "ORGB"                           |
            \*---------------------------------------------------------*/
            if(header.pkt_magic[i] != openrgb_sdk_magic[i])
            {
                continue;
            }
        }

        /*---------------------------------------------------------*\
        | If we get to this point, the magic is correct.  Read the  |
        | rest of the header                                        |
        \*---------------------------------------------------------*/
        bytes_read = 0;
        do
        {
            int tmp_bytes_read = 0;

            tmp_bytes_read = recv_select(client_sock, (char *)&header.pkt_dev_idx + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read, 0);

            bytes_read += tmp_bytes_read;

            if(tmp_bytes_read <= 0)
            {
                goto listen_done;
            }

        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        /*---------------------------------------------------------*\
        | Header received, now receive the data                     |
        \*---------------------------------------------------------*/
        if(header.pkt_size > 0)
        {
            bytes_read = 0;

            data = new char[header.pkt_size];

            do
            {
                int tmp_bytes_read = 0;

                tmp_bytes_read = recv_select(client_sock, &data[(unsigned int)bytes_read], header.pkt_size - bytes_read, 0);

                if(tmp_bytes_read <= 0)
                {
                    goto listen_done;
                }
                bytes_read += tmp_bytes_read;

            } while ((unsigned int)bytes_read < header.pkt_size);
        }

        /*---------------------------------------------------------*\
        | Entire request received, select functionality based on    |
        | request ID                                                |
        \*---------------------------------------------------------*/
        switch(header.pkt_id)
        {
            case NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                ProcessReply_ControllerCount(header.pkt_size, data);
                break;

            case NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                ProcessReply_ControllerData(header.pkt_size, data, header.pkt_dev_idx);
                break;

            case NET_PACKET_ID_REQUEST_PROTOCOL_VERSION:
                ProcessReply_ProtocolVersion(header.pkt_size, data);
                break;

            case NET_PACKET_ID_DEVICE_LIST_UPDATED:
                ProcessRequest_DeviceListChanged();
                break;
        }

        delete[] data;
    }

listen_done:
    printf( "Client socket has been closed");
    client_string_sent                  = false;
    controller_data_requested           = false;
    controller_data_received            = false;
    protocol_initialized                = false;
    requested_controllers               = 0;
    server_controller_count             = 0;
    server_controller_count_requested   = false;
    server_controller_count_received    = false;
    server_initialized                  = false;
    server_connected                    = false;

    ControllerListMutex.lock();

    for(size_t server_controller_idx = 0; server_controller_idx < server_controllers.size(); server_controller_idx++)
    {
        for(size_t controller_idx = 0; controller_idx < controllers.size(); controller_idx++)
        {
            if(controllers[controller_idx] == server_controllers[server_controller_idx])
            {
                controllers.erase(controllers.begin() + controller_idx);
                break;
            }
        }
    }

    std::vector<RGBController *> server_controllers_copy = server_controllers;

    server_controllers.clear();

    for(size_t server_controller_idx = 0; server_controller_idx < server_controllers_copy.size(); server_controller_idx++)
    {
        delete server_controllers_copy[server_controller_idx];
    }

    ControllerListMutex.unlock();

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    ClientInfoChanged();
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

void NetworkClient::ProcessReply_ControllerCount(unsigned int data_size, char * data)
{
    if(data_size == sizeof(unsigned int))
    {
        memcpy(&server_controller_count, data, sizeof(unsigned int));

        server_controller_count_received    = true;
        requested_controllers               = 0;
        controller_data_requested           = false;

        printf("Client: Received controller count from server: %d\r\n", server_controller_count);
    }
}

void NetworkClient::ProcessReply_ControllerData(unsigned int data_size, char * data, unsigned int dev_idx)
{
    /*---------------------------------------------------------*\
    | Verify the controller description size (first 4 bytes of  |
    | data) matches the packet size in the header               |
    \*---------------------------------------------------------*/
    if(data_size == *((unsigned int*)data))
    {
        RGBController_Network * new_controller   = new RGBController_Network(this, dev_idx);

        new_controller->ReadDeviceDescription((unsigned char *)data, GetProtocolVersion());

        /*-----------------------------------------------------*\
        | Mark this controller as remote owned                  |
        \*-----------------------------------------------------*/
        new_controller->flags &= ~CONTROLLER_FLAG_LOCAL;
        new_controller->flags |= CONTROLLER_FLAG_REMOTE;

        ControllerListMutex.lock();

        if(dev_idx >= server_controllers.size())
        {
            server_controllers.push_back(new_controller);
        }
        else
        {
            server_controllers[dev_idx]->active_mode = new_controller->active_mode;
            server_controllers[dev_idx]->leds.clear();
            server_controllers[dev_idx]->leds        = new_controller->leds;
            server_controllers[dev_idx]->colors.clear();
            server_controllers[dev_idx]->colors      = new_controller->colors;
            for(unsigned int i = 0; i < server_controllers[dev_idx]->zones.size(); i++)
            {
                server_controllers[dev_idx]->zones[i].leds_count = new_controller->zones[i].leds_count;
                server_controllers[dev_idx]->zones[i].segments.clear();
                server_controllers[dev_idx]->zones[i].segments = new_controller->zones[i].segments;
            }
            server_controllers[dev_idx]->SetupColors();

            delete new_controller;
        }

        ControllerListMutex.unlock();

        controller_data_received = true;
    }
}

void NetworkClient::ProcessReply_ProtocolVersion(unsigned int data_size, char * data)
{
    if(data_size == sizeof(unsigned int))
    {
        memcpy(&server_protocol_version, data, sizeof(unsigned int));
        server_protocol_version_received = true;
    }
}

void NetworkClient::ProcessRequest_DeviceListChanged()
{
    change_in_progress = true;

    /*---------------------------------------------------------*\
    | Delete all controllers from the server's controller list  |
    \*---------------------------------------------------------*/
    ControllerListMutex.lock();

    for(size_t server_controller_idx = 0; server_controller_idx < server_controllers.size(); server_controller_idx++)
    {
        for(size_t controller_idx = 0; controller_idx < controllers.size(); controller_idx++)
        {
            if(controllers[controller_idx] == server_controllers[server_controller_idx])
            {
                controllers.erase(controllers.begin() + controller_idx);
                break;
            }
        }
    }

    std::vector<RGBController *> server_controllers_copy = server_controllers;

    server_controllers.clear();

    for(size_t server_controller_idx = 0; server_controller_idx < server_controllers_copy.size(); server_controller_idx++)
    {
        delete server_controllers_copy[server_controller_idx];
    }

    ControllerListMutex.unlock();

    /*---------------------------------------------------------*\
    | Client info has changed, call the callbacks               |
    \*---------------------------------------------------------*/
    ClientInfoChanged();

    /*---------------------------------------------------------*\
    | Mark server as uninitialized and reset server             |
    | initialization state so that it restarts the list         |
    | requesting process                                        |
    \*---------------------------------------------------------*/
    controller_data_requested           = false;
    controller_data_received            = false;
    requested_controllers               = 0;
    server_controller_count             = 0;
    server_controller_count_requested   = false;
    server_controller_count_received    = false;
    server_initialized                  = false;

    change_in_progress = false;
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

void NetworkClient::SendRequest_ControllerCount()
{
    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, 0, NET_PACKET_ID_REQUEST_CONTROLLER_COUNT, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_ControllerData(unsigned int dev_idx)
{
    NetPacketHeader request_hdr;
    unsigned int    protocol_version;

    controller_data_received = false;

    memcpy(request_hdr.pkt_magic, openrgb_sdk_magic, sizeof(openrgb_sdk_magic));

    request_hdr.pkt_dev_idx  = dev_idx;
    request_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_DATA;

    if(server_protocol_version == 0)
    {
        request_hdr.pkt_size     = 0;

        send_in_progress.lock();
        send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send_in_progress.unlock();
    }
    else
    {
        request_hdr.pkt_size     = sizeof(unsigned int);

        /*-------------------------------------------------------------*\
        | Limit the protocol version to the highest supported by both   |
        | the client and the server.                                    |
        \*-------------------------------------------------------------*/
        if(server_protocol_version > OPENRGB_SDK_PROTOCOL_VERSION)
        {
            protocol_version = OPENRGB_SDK_PROTOCOL_VERSION;
        }
        else
        {
            protocol_version = server_protocol_version;
        }

        send_in_progress.lock();
        send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
        send(client_sock, (char *)&protocol_version, sizeof(unsigned int), MSG_NOSIGNAL);
        send_in_progress.unlock();
    }
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

void NetworkClient::SendRequest_RGBController_AddSegment(unsigned int dev_idx, unsigned char * data, unsigned int size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_ADDSEGMENT, size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data, size, 0);
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

void NetworkClient::SendRequest_RGBController_UpdateLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS, size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data, size, 0);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_UpdateZoneLEDs(unsigned int dev_idx, unsigned char * data, unsigned int size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS, size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data, size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_UpdateSingleLED(unsigned int dev_idx, unsigned char * data, unsigned int size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED, size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data, size, MSG_NOSIGNAL);
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

void NetworkClient::SendRequest_RGBController_UpdateMode(unsigned int dev_idx, unsigned char * data, unsigned int size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE, size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data, size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_RGBController_SaveMode(unsigned int dev_idx, unsigned char * data, unsigned int size)
{
    if(change_in_progress)
    {
        return;
    }

    NetPacketHeader request_hdr;

    InitNetPacketHeader(&request_hdr, dev_idx, NET_PACKET_ID_RGBCONTROLLER_SAVEMODE, size);

    send_in_progress.lock();
    send(client_sock, (char *)&request_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)data, size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_LoadProfile(std::string profile_name)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_LOAD_PROFILE, (unsigned int)strlen(profile_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_SaveProfile(std::string profile_name)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_SAVE_PROFILE, (unsigned int)strlen(profile_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_DeleteProfile(std::string profile_name)
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_DELETE_PROFILE, (unsigned int)strlen(profile_name.c_str()) + 1);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send(client_sock, (char *)profile_name.c_str(), reply_hdr.pkt_size, MSG_NOSIGNAL);
    send_in_progress.unlock();
}

void NetworkClient::SendRequest_GetProfileList()
{
    NetPacketHeader reply_hdr;

    InitNetPacketHeader(&reply_hdr, 0, NET_PACKET_ID_REQUEST_PROFILE_LIST, 0);

    send_in_progress.lock();
    send(client_sock, (char *)&reply_hdr, sizeof(NetPacketHeader), MSG_NOSIGNAL);
    send_in_progress.unlock();
}

std::vector<std::string> * NetworkClient::ProcessReply_ProfileList(unsigned int data_size, char * data)
{
    std::vector<std::string> * profile_list;

    if(data_size > 0)
    {
        profile_list = new std::vector<std::string>(data_size);

        /*---------------------------------------------------------*\
        | Skip 4 first bytes (data length, unused)                  |
        \*---------------------------------------------------------*/
        unsigned short data_ptr = sizeof(unsigned short);
        unsigned short num_profile;

        memcpy(&num_profile, data, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        for(int i = 0; i < num_profile; i++)
        {
            unsigned short name_len;

            memcpy(&name_len, data, sizeof(unsigned short));
            data_ptr += sizeof(unsigned short);

            std::string profile_name(data, name_len);
            profile_list->push_back(profile_name);

            data_ptr += name_len;
        }

        server_controller_count_received = true;
    }
    else
    {
        profile_list = new std::vector<std::string>(0);
    }

    return profile_list;
}

/*---------------------------------------------------------*\
| net_port.h                                                |
|                                                           |
|   Cross Platform Network Library for Windows and Linux    |
|   This library provides access to TCP and UDP ports with  |
|   a common API for both Windows and Linux systems         |
|                                                           |
|   Adam Honse (calcprogrammer1@gmail.com)      15 Dec 2016 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <vector>

#ifdef WIN32
/*---------------------------------------------------------*\
| Windows interferes with std::max unless NOMINMAX defined  |
\*---------------------------------------------------------*/
#define NOMINMAX
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <net/if.h>
#endif

#ifndef WIN32
#define SOCKET int
#define ioctlsocket ioctl
#define closesocket close
#define WSACleanup()
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define SD_RECEIVE SHUT_RD
#endif

//Network Port Class
//The reason for this class is that network ports are treated differently
//on Windows and Linux.  By creating a class, those differences can be
//made invisible to the program and make cross-platform usage easy

class net_port
{
public:
    net_port();
    net_port(const char * client_name, const char * port);

    ~net_port();

    //Function to open the port
    bool udp_client(const char* client_name, const char * port);
    bool udp_client(const char * client_name, const char * send_port, const char * recv_port);
    bool tcp_client(const char* client_name, const char * port);
    bool tcp_client_connect();

    //Function to open a server
    bool        tcp_server(const char * port);
    std::size_t tcp_server_num_clients();
    SOCKET *    tcp_server_get_client(std::size_t client_idx);
    SOCKET *    tcp_server_listen();

    void udp_join_multicast_group(const char * group_name);

    int udp_listen(char * recv_data, int length);
    int tcp_listen(char * recv_data, int length);

    //Function to write data to the serial port
    int udp_write(char * buffer, int length);
    int tcp_write(char * buffer, int length);
    int tcp_client_write(char * buffer, int length);

    void tcp_close();

    void set_receive_timeout(int sec, int usec);

    bool connected;
    SOCKET sock;

private:
#ifdef WIN32
    WSADATA     wsa;
#endif

    std::vector<SOCKET *> clients;

    sockaddr addrDest;
    addrinfo*   result_list;
};

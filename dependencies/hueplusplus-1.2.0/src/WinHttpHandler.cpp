/**
    \file WinHttpHandler.cpp
    Copyright Notice\n
    Copyright (C) 2017  Jan Rogall		- developer\n
    Copyright (C) 2017  Moritz Wirger	- developer\n

    This file is part of hueplusplus.

    hueplusplus is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    hueplusplus is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with hueplusplus.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "hueplusplus/WinHttpHandler.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <system_error>

#include <stdio.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

namespace hueplusplus
{
namespace
{
class AddrInfoFreer
{
public:
    explicit AddrInfoFreer(addrinfo* p) : p(p) { }
    ~AddrInfoFreer() { freeaddrinfo(p); }

private:
    addrinfo* p;
};
class SocketCloser
{
public:
    explicit SocketCloser(SOCKET s) : s(s) { }
    ~SocketCloser() { closesocket(s); }

private:
    SOCKET s;
};
} // namespace

WinHttpHandler::WinHttpHandler()
{
    // Initialize Winsock
    int return_code = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (return_code != 0)
    {
        std::cerr << "WinHttpHandler: Failed to open socket: " << return_code << std::endl;
        throw(std::system_error(return_code, std::system_category(), "WinHttpHandler: Failed to open socket"));
    }
}

WinHttpHandler::~WinHttpHandler()
{
    WSACleanup();
}

std::string WinHttpHandler::send(const std::string& msg, const std::string& adr, int port) const
{
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    struct addrinfo* result = nullptr;
    if (getaddrinfo(adr.c_str(), std::to_string(port).c_str(), &hints, &result) != 0)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: getaddrinfo failed: " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: getaddrinfo failed"));
    }
    SOCKET connect_socket = INVALID_SOCKET;
    int connectError = 0;
    {
        AddrInfoFreer freeResult(result);

        // Attempt to connect to the first address returned by
        // the call to getaddrinfo
        struct addrinfo* ptr = result;

        // Create a SOCKET for connecting to server
        connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

        if (connect_socket == INVALID_SOCKET)
        {
            int err = WSAGetLastError();
            std::cerr << "WinHttpHandler: Error at socket(): " << err << std::endl;
            throw(std::system_error(err, std::system_category(), "WinHttpHandler: Error at socket()"));
        }

        // Connect to server.
        if (connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
        {
            connectError = WSAGetLastError();
            closesocket(connect_socket);
            connect_socket = INVALID_SOCKET;
        }

        // Should really try the next address returned by getaddrinfo
        // if the connect call failed
        // But for this simple example we just free the resources
        // returned by getaddrinfo and print an error message
    }

    if (connect_socket == INVALID_SOCKET)
    {
        std::cerr << "WinHttpHandler: Unable to connect to server!" << std::endl;
        throw std::system_error(connectError, std::system_category(), "WinHttpHandler: Unable to connect to server!");
    }
    SocketCloser closeSocket(connect_socket);

    // Send an initial buffer
    if (::send(connect_socket, msg.c_str(), msg.size(), 0) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: send failed: " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: send failed"));
    }

    const int recvbuflen = 128;
    char recvbuf[recvbuflen];

    // Receive data until the server closes the connection
    std::string response;
    int res;
    do
    {
        res = recv(connect_socket, recvbuf, recvbuflen, 0);
        if (res > 0)
        {
            // std::cout << "WinHttpHandler: Bytes received: " << res << std::endl;
            response.append(recvbuf, res);
        }
        else if (res == 0)
        {
            // std::cout << "WinHttpHandler: Connection closed " << std::endl;
        }
        else
        {
            int err = WSAGetLastError();
            std::cerr << "WinHttpHandler: recv failed: " << err << std::endl;
            throw(std::system_error(err, std::system_category(), "WinHttpHandler: recv failed"));
        }
    } while (res > 0);

    // shutdown the connection
    if (shutdown(connect_socket, SD_BOTH) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: shutdown failed: " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: shutdown failed"));
    }

    return response;
}

std::vector<std::string> WinHttpHandler::sendMulticast(
    const std::string& msg, const std::string& adr, int port, std::chrono::steady_clock::duration timeout) const
{
    struct addrinfo hints = {};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    struct addrinfo* result = nullptr;
    if (getaddrinfo(adr.c_str(), std::to_string(port).c_str(), &hints, &result) != 0)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: sendMulticast: getaddrinfo failed: " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: sendMulticast: getaddrinfo failed"));
    }
    AddrInfoFreer freeResult(result);

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    struct addrinfo* ptr = result;

    // Create a SOCKET for connecting to server
    SOCKET connect_socket = socket(ptr->ai_family, ptr->ai_socktype, 0);
    if (connect_socket == INVALID_SOCKET)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: sendMulticast: Error at socket(): " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: sendMulticast: Error at socket()"));
    }
    SocketCloser closeSocket(connect_socket);

    // Fill out source socket's address information.
    SOCKADDR_IN source_sin;
    source_sin.sin_family = AF_INET;
    source_sin.sin_port = htons(0);
    source_sin.sin_addr.s_addr = htonl(INADDR_ANY);

    // Associate the source socket's address with the socket, Sock.
    if (bind(connect_socket, (struct sockaddr FAR*)&source_sin, sizeof(source_sin)) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: sendMulticast: Binding socket failed: " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: sendMulticast: Binding socket failed"));
    }

    u_long sock_mode = 1;
    ioctlsocket(connect_socket, FIONBIO, &sock_mode);

    BOOL bOptVal = TRUE;
    setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOptVal, sizeof(bOptVal));

    // Set the Time-to-Live of the multicast.
    int iOptVal = 1; // for same subnet, but might be increased to 16
    if (setsockopt(connect_socket, IPPROTO_IP, IP_MULTICAST_TTL, (char FAR*)&iOptVal, sizeof(int)) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: sendMulticast: setsockopt failed: " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: sendMulticast: setsockopt failed"));
    }

    // Fill out the desination socket's address information.
    SOCKADDR_IN dest_sin;
    dest_sin.sin_family = AF_INET;
    dest_sin.sin_port = htons(port);
    dest_sin.sin_addr.s_addr = inet_addr((const char*)ptr->ai_addr);

    // Send a message to the multicasting address.
    if (sendto(connect_socket, msg.c_str(), msg.size(), 0, (struct sockaddr FAR*)&dest_sin, sizeof(dest_sin))
        == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: sendMulticast: sendto failed: " << WSAGetLastError() << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: sendMulticast: sendto failed"));
    }

    // shutdown the connection for sending since no more data will be sent
    // the client can still use the ConnectSocket for receiving data (no issue here because this is a UDP socket)
    if (shutdown(connect_socket, SD_SEND) == SOCKET_ERROR)
    {
        int err = WSAGetLastError();
        std::cerr << "WinHttpHandler: sendMulticast: shutdown failed: " << err << std::endl;
        throw(std::system_error(err, std::system_category(), "WinHttpHandler: sendMulticast: shutdown failed"));
    }

    std::string response;
    const int recvbuflen = 2048;
    char recvbuf[recvbuflen] = {};
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < timeout)
    {
        int res = recv(connect_socket, recvbuf, recvbuflen, 0);
        if (res > 0)
        {
            // std::cout << "WinHttpHandler: sendMulticast: Bytes received: " << res
            // << std::endl;
            response.append(recvbuf, res);
        }
        else if (res == 0)
        {
            // std::cout << "WinHttpHandler: sendMulticast: Connection closed " <<
            // std::endl;
        }
        else
        {
            // No exception here due to non blocking socket
            // std::cerr << "sendMulticast: recv failed: " << WSAGetLastError() <<
            // std::endl; throw(std::runtime_error("recv failed"));
        }
    }

    // construct return vector
    std::vector<std::string> returnString;
    size_t pos = response.find("\r\n\r\n");
    size_t prevpos = 0;
    while (pos != std::string::npos)
    {
        returnString.push_back(response.substr(prevpos, pos - prevpos));
        pos += 4;
        prevpos = pos;
        pos = response.find("\r\n\r\n", pos);
    }

    return returnString;
}
} // namespace hueplusplus

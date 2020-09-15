/**
    \file LinHttpHandler.cpp
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

#include "hueplusplus/LinHttpHandler.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <system_error>

#include <arpa/inet.h>
#include <netdb.h> // struct hostent, gethostbyname
#include <netinet/in.h> // struct sockaddr_in, struct sockaddr
#include <stdio.h> // printf, sprintf
#include <stdlib.h> // exit
#include <string.h> // functions for C style null-terminated strings
#include <sys/socket.h> // socket, connect
#include <unistd.h> // read, write, close

namespace hueplusplus
{
class SocketCloser
{
public:
    explicit SocketCloser(int sockFd) : s(sockFd) {}
    ~SocketCloser() { close(s); }

private:
    int s;
};

std::string LinHttpHandler::send(const std::string& msg, const std::string& adr, int port) const
{
    // create socket
    int socketFD = socket(AF_INET, SOCK_STREAM, 0);

    SocketCloser closeMySocket(socketFD);
    if (socketFD < 0)
    {
        int errCode = errno;
        std::cerr << "LinHttpHandler: Failed to open socket: " << std::strerror(errCode) << "\n";
        throw(std::system_error(errCode, std::generic_category(), "LinHttpHandler: Failed to open socket"));
    }

    // lookup ip address
    hostent* server;
    server = gethostbyname(adr.c_str());
    if (server == NULL)
    {
        int errCode = errno;
        std::cerr << "LinHttpHandler: Failed to find host with address " << adr << ": " << std::strerror(errCode)
                  << "\n";
        throw(std::system_error(errCode, std::generic_category(), "LinHttpHandler: gethostbyname"));
    }

    // fill in the structure
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // connect the socket
    if (connect(socketFD, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        int errCode = errno;
        std::cerr << "LinHttpHandler: Failed to connect socket: " << std::strerror(errCode) << "\n";
        throw(std::system_error(errCode, std::generic_category(), "LinHttpHandler: Failed to connect socket"));
    }

    // send the request
    size_t total = msg.length();
    size_t sent = 0;
    do
    {
        ssize_t bytes = write(socketFD, msg.c_str() + sent, total - sent);
        if (bytes < 0)
        {
            int errCode = errno;
            std::cerr << "LinHttpHandler: Failed to write message to socket: " << std::strerror(errCode) << "\n";
            throw(std::system_error(
                errCode, std::generic_category(), "LinHttpHandler: Failed to write message to socket"));
        }
        else if (bytes == 0)
        {
            break;
        }
        else
        {
            sent += bytes;
        }
    } while (sent < total);

    // receive the response
    std::string response;
    char buffer[128] = {};
    do
    {
        ssize_t bytes = read(socketFD, buffer, 127);
        if (bytes < 0)
        {
            int errCode = errno;
            std::cerr << "LinHttpHandler: Failed to read response from socket: " << std::strerror(errCode) << std::endl;
            throw(std::system_error(
                errCode, std::generic_category(), "LinHttpHandler: Failed to read response from socket"));
        }
        else if (bytes == 0)
        {
            break;
        }
        else
        {
            response.append(buffer, bytes);
        }
    } while (true);

    return response;
}

std::vector<std::string> LinHttpHandler::sendMulticast(
    const std::string& msg, const std::string& adr, int port, std::chrono::steady_clock::duration timeout) const
{
    hostent* server; // host information
    sockaddr_in server_addr; // server address

    // fill in the server's address and data
    memset((char*)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // look up the address of the server given its name
    server = gethostbyname(adr.c_str());
    if (!server)
    {
        int errCode = errno;
        std::cerr << "LinHttpHandler: sendMulticast: Failed to obtain address of " << msg << ": "
                  << std::strerror(errCode) << "\n";
        throw(std::system_error(
            errCode, std::generic_category(), "LinHttpHandler: sendMulticast: Failed to obtain address of host"));
    }

    // put the host's address into the server address structure
    memcpy((void*)&server_addr.sin_addr, server->h_addr_list[0], server->h_length);

    // create the socket
    int socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    SocketCloser closeMySendSocket(socketFD);
    if (socketFD < 0)
    {
        int errCode = errno;
        std::cerr << "LinHttpHandler: sendMulticast: Failed to open socket: " << std::strerror(errCode) << "\n";
        throw(std::system_error(
            errCode, std::generic_category(), "LinHttpHandler: sendMulticast: Failed to open socket"));
    }

    // send a message to the server
    if (sendto(socketFD, msg.c_str(), strlen(msg.c_str()), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        int errCode = errno;
        std::cerr << "LinHttpHandler: sendMulticast: Failed to send message: " << std::strerror(errCode) << "\n";
        throw(std::system_error(
            errCode, std::generic_category(), "LinHttpHandler: sendMulticast: Failed to send message"));
    }

    std::string response;
    char buffer[2048] = {}; // receive buffer

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - start < timeout)
    {
        ssize_t bytesReceived = recv(socketFD, &buffer, 2048, MSG_DONTWAIT);
        if (bytesReceived < 0)
        {
            int errCode = errno;
            if (errCode != EAGAIN && errCode != EWOULDBLOCK)
            {
                std::cerr << "LinHttpHandler: sendMulticast: Failed to read response "
                             "from socket: "
                          << std::strerror(errCode) << "\n";
                throw(std::system_error(errCode, std::generic_category(),
                    "LinHttpHandler: sendMulticast: Failed to read "
                    "response from socket"));
            }
            continue;
        }
        if (bytesReceived)
        {
            response.append(buffer, bytesReceived);
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

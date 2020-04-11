#include "NetworkServer.h"

NetworkServer::NetworkServer(std::vector<RGBController *>& control) : controllers(control)
{
    //Start a TCP server and launch threads
    port.tcp_server("1337");
}

void NetworkServer::ConnectionThread()
{
    //This thread handles client connections
    while(1)
    {
        port.tcp_server_listen();
    }
}

void NetworkServer::ListenThread()
{
    //This thread handles messages received from clients
    while(1)
    {
        //Listen for request

        //Request received, select functionality based on request ID
    }
}
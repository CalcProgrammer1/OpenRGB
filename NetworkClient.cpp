#include "NetworkClient.h"

NetworkClient::NetworkClient(std::vector<RGBController *>& control) : controllers(control)
{
    //Start the connection thread
}

void NetworkClient::ConnectionThread()
{
    //This thread manages the connection to the server
    while(1)
    {
        //Connect to server and reconnect if the connection is lost
    }
}

void NetworkClient::ListenThread()
{
    //This thread handles messages received from the server
    while(1)
    {
        //Listen for request

        //Request received, select functionality based on request ID
    }
}
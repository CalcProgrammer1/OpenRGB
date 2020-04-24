#include "NetworkClient.h"
#include "RGBController_Dummy.h"
#include <cstring>

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

//Thread functions have different types in Windows and Linux
#ifdef WIN32
#define THREAD static void
#define THREADRETURN
#else
#define THREAD static void*
#define THREADRETURN return(NULL);
#endif

THREAD connection_thread(void *param)
{
    NetworkClient* client = static_cast<NetworkClient*>(param);
    client->ConnectionThread();
    THREADRETURN
}

THREAD listen_thread(void *param)
{
    NetworkClient* client = static_cast<NetworkClient*>(param);
    client->ListenThread();
    THREADRETURN
}

NetworkClient::NetworkClient(std::vector<RGBController *>& control) : controllers(control)
{
    unsigned int requested_controllers;

    port.tcp_client("127.0.0.1", "1337");

    server_connected        = false;
    server_controller_count = 0;
    requested_controllers   = 0;

    //Start the connection thread
#ifdef WIN32
    _beginthread(connection_thread, 0, this);
#else
    pthread_t thread;
    pthread_create(&thread, NULL, &connection_thread, this);
    pthread_create(&thread, NULL, &listen_thread, this);
#endif

    //Wait for server to connect
    while(!server_connected)
    {
        Sleep(100);
    }

    //Once server is connected, request number of controllers
    SendRequest_ControllerCount();

    //Wait for server controller count
    while(server_controller_count == 0)
    {
        Sleep(100);
    }

    printf("Client: Received controller count from server: %d\r\n", server_controller_count);

    //Once count is received, request controllers
    while(requested_controllers < server_controller_count)
    {
        printf("Client: Requesting controller %d\r\n", requested_controllers);

        SendRequest_ControllerData(requested_controllers);

        //Wait until controller is received
        while(server_controllers.size() == requested_controllers)
        {
            Sleep(100);
        }

        requested_controllers++;
    }

    //All controllers received, add them to master list
    printf("Client: All controllers received, adding them to master list\r\n");
    for(std::size_t controller_idx = 0; controller_idx < server_controllers.size(); controller_idx++)
    {
        controllers.push_back(server_controllers[controller_idx]);
    }
}

void NetworkClient::ConnectionThread()
{
    //This thread manages the connection to the server
    while(1)
    {
        //Connect to server and reconnect if the connection is lost

        //Try to connect to server
        port.tcp_client_connect();

        printf( "Connected to server\n" );

        server_connected = true;
        break;

        //Wait 1 second between tries;
        Sleep(1000);
    }
}

void NetworkClient::ListenThread()
{
    printf("Network client listener started\n");
    //This thread handles messages received from the server
    while(1)
    {
        NetPacketHeader header;
        char *          data        = NULL;
        int             bytes_read  = 0;

        //Read first byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[0], 1);
        } while(bytes_read == 0);

        //Test first character of magic - 'O'
        if(header.pkt_magic[0] != 'O')
        {
            continue;
        }

        //Read second byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[1], 1);
        } while(bytes_read == 0);

        //Test second character of magic - 'R'
        if(header.pkt_magic[1] != 'R')
        {
            continue;
        }

        //Read third byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[2], 1);
        } while(bytes_read == 0);

        //Test third character of magic - 'G'
        if(header.pkt_magic[2] != 'G')
        {
            continue;
        }

        //Read fourth byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[3], 1);
        } while(bytes_read == 0);

        //Test fourth character of magic - 'B'
        if(header.pkt_magic[3] != 'B')
        {
            continue;
        }

        //If we get to this point, the magic is correct.  Read the rest of the header
        bytes_read = 0;
        do
        {
            bytes_read += port.tcp_listen((char *)&header.pkt_dev_idx + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read);
        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        printf( "Client: Received header, now receiving data of size %d\r\n", header.pkt_size);

        //Header received, now receive the data
        if(header.pkt_size > 0)
        {
            unsigned int bytes_read = 0;

            data = new char[header.pkt_size];

            do
            {
                bytes_read += port.tcp_listen(&data[bytes_read], 128);
            } while (bytes_read < header.pkt_size);
        }

        printf( "Client: Received header and data\r\n" );
        printf( "Client: Packet ID: %d \r\n", header.pkt_id);

        //Entire request received, select functionality based on request ID
        switch(header.pkt_id)
        {
            case NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                printf( "Client: NET_PACKET_ID_REQUEST_CONTROLLER_COUNT\r\n" );
                
                ProcessReply_ControllerCount(header.pkt_size, data);
                break;

            case NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                printf( "Client: NET_PACKET_ID_REQUEST_CONTROLLER_DATA\r\n");
                
                ProcessReply_ControllerData(header.pkt_size, data);
                break;
        }
    }
}

void NetworkClient::ProcessReply_ControllerCount(unsigned int data_size, char * data)
{
    if(data_size == sizeof(unsigned int))
    {
        memcpy(&server_controller_count, data, sizeof(unsigned int));
    }
}

void NetworkClient::ProcessReply_ControllerData(unsigned int data_size, char * data)
{
    RGBController_Dummy * new_controller = new RGBController_Dummy();

    new_controller->ReadDeviceDescription((unsigned char *)data);

    printf("Received controller: %s\r\n", new_controller->name.c_str());

    server_controllers.push_back(new_controller);
}

void NetworkClient::SendRequest_ControllerCount()
{
    NetPacketHeader reply_hdr;

    reply_hdr.pkt_magic[0] = 'O';
    reply_hdr.pkt_magic[1] = 'R';
    reply_hdr.pkt_magic[2] = 'G';
    reply_hdr.pkt_magic[3] = 'B';

    reply_hdr.pkt_dev_idx  = 0;
    reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_COUNT;
    reply_hdr.pkt_size     = 0;

    port.tcp_client_write((char *)&reply_hdr, sizeof(NetPacketHeader));
}

void NetworkClient::SendRequest_ControllerData(unsigned int idx)
{
    NetPacketHeader reply_hdr;
    unsigned int    reply_data;
    
    reply_hdr.pkt_magic[0] = 'O';
    reply_hdr.pkt_magic[1] = 'R';
    reply_hdr.pkt_magic[2] = 'G';
    reply_hdr.pkt_magic[3] = 'B';

    reply_hdr.pkt_dev_idx  = idx;
    reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_DATA;
    reply_hdr.pkt_size     = 0;

    port.tcp_client_write((char *)&reply_hdr, sizeof(NetPacketHeader));
}
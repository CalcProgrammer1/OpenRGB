#include "NetworkServer.h"
#include <cstring>

//Include thread libraries for Windows or Linux
#ifdef WIN32
#include <process.h>
#else
#include "pthread.h"
#include "unistd.h"
#endif

//Thread functions have different types in Windows and Linux
#ifdef WIN32
#define THREAD static void
#define THREADRETURN
#else
#define THREAD static void*
#define THREADRETURN return(NULL);
#endif

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

typedef struct listen_thread_param_type
{
    NetworkServer * this_ptr;
    SOCKET *        sock_ptr;
};

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif

THREAD connection_thread(void *param)
{
    NetworkServer* server = static_cast<NetworkServer*>(param);
    server->ConnectionThread();
    THREADRETURN
}

THREAD listen_thread(void *param)
{
    NetworkServer* server = static_cast<listen_thread_param_type*>(param)->this_ptr;
    SOCKET*        sock   = static_cast<listen_thread_param_type*>(param)->sock_ptr;
    server->ListenThread(sock);
    THREADRETURN
}

NetworkServer::NetworkServer(std::vector<RGBController *>& control) : controllers(control)
{
    //Start a TCP server and launch threads
    port.tcp_server("1337");

    //Start the connection thread
#ifdef WIN32
    _beginthread(connection_thread, 0, this);
#else
    pthread_t thread;
    pthread_create(&thread, NULL, &connection_thread, this);
#endif
}

void NetworkServer::ConnectionThread()
{
    //This thread handles client connections

    printf("Network connection thread started\n");
    while(1)
    {
        SOCKET * client_sock = port.tcp_server_listen();
        //Start a listener thread for the new client socket
#ifdef WIN32
        _beginthread(listen_thread, 0, this);
#else
        pthread_t thread;

        listen_thread_param_type new_thread_param;
        new_thread_param.sock_ptr = client_sock;
        new_thread_param.this_ptr = this;
        pthread_create(&thread, NULL, &listen_thread, &new_thread_param);
#endif
    }
}

void NetworkServer::ListenThread(SOCKET * client_sock)
{
    printf("Network server started\n");
    //This thread handles messages received from clients
    while(1)
    {
        NetPacketHeader header;
        char *          data        = NULL;
        int             bytes_read  = 0;

        //Read first byte of magic
        do
        {
            bytes_read = read(*client_sock, &header.pkt_magic[0], 1);
        } while(bytes_read == 0);

        //Test first character of magic - 'O'
        if(header.pkt_magic[0] != 'O')
        {
            continue;
        }

        printf("Magic: 'O'\r\n");

        //Read second byte of magic
        do
        {
            bytes_read = read(*client_sock, &header.pkt_magic[1], 1);
        } while(bytes_read == 0);

        //Test second character of magic - 'R'
        if(header.pkt_magic[1] != 'R')
        {
            continue;
        }

        printf("Magic: 'R'\r\n");

        //Read third byte of magic
        do
        {
            bytes_read = read(*client_sock, &header.pkt_magic[2], 1);
        } while(bytes_read == 0);

        //Test third character of magic - 'G'
        if(header.pkt_magic[2] != 'G')
        {
            continue;
        }

        printf("Magic: 'G'\r\n");

        //Read fourth byte of magic
        do
        {
            bytes_read = read(*client_sock, &header.pkt_magic[3], 1);
        } while(bytes_read == 0);

        //Test fourth character of magic - 'B'
        if(header.pkt_magic[3] != 'B')
        {
            continue;
        }

        printf("Magic: 'B'\r\n");

        //If we get to this point, the magic is correct.  Read the rest of the header
        bytes_read = 0;
        do
        {
            bytes_read += read(*client_sock, (char *)&header.pkt_dev_idx + bytes_read, sizeof(header) - sizeof(header.pkt_magic) - bytes_read);
        } while(bytes_read != sizeof(header) - sizeof(header.pkt_magic));

        printf( "Received header, now receiving data of size %d\r\n", header.pkt_size);

        //Header received, now receive the data
        if(header.pkt_size > 0)
        {
            unsigned int bytes_read = 0;

            data = new char[header.pkt_size];

            do
            {
                bytes_read += read(*client_sock, &data[bytes_read], 128);
            } while (bytes_read < header.pkt_size);
        }

        printf( "Received header and data\r\n" );
        printf( "Packet ID: %d \r\n", header.pkt_id);

        //Entire request received, select functionality based on request ID
        switch(header.pkt_id)
        {
            case NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                printf( "NET_PACKET_ID_REQUEST_CONTROLLER_COUNT\r\n" );

                NetPacketHeader reply_hdr;
                unsigned int    reply_data;

                reply_hdr.pkt_magic[0] = 'O';
                reply_hdr.pkt_magic[1] = 'R';
                reply_hdr.pkt_magic[2] = 'G';
                reply_hdr.pkt_magic[3] = 'B';

                reply_hdr.pkt_dev_idx  = 0;
                reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_COUNT;
                reply_hdr.pkt_size     = sizeof(unsigned int);

                reply_data             = controllers.size();

                write(*client_sock, &reply_hdr, sizeof(NetPacketHeader));
                write(*client_sock, &reply_data, sizeof(unsigned int));
                break;

            case NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                printf( "NET_PACKET_ID_REQUEST_CONTROLLER_DATA\r\n" );

                if(header.pkt_dev_idx < controllers.size())
                {
                    NetPacketHeader reply_hdr;
                    unsigned char *reply_data = controllers[header.pkt_dev_idx]->GetDeviceDescription();
                    unsigned int   reply_size;

                    memcpy(&reply_size, reply_data, sizeof(reply_size));
                    
                    reply_hdr.pkt_magic[0] = 'O';
                    reply_hdr.pkt_magic[1] = 'R';
                    reply_hdr.pkt_magic[2] = 'G';
                    reply_hdr.pkt_magic[3] = 'B';

                    reply_hdr.pkt_dev_idx  = 0;
                    reply_hdr.pkt_id       = NET_PACKET_ID_REQUEST_CONTROLLER_COUNT;
                    reply_hdr.pkt_size     = reply_size;

                    write(*client_sock, &reply_hdr, sizeof(NetPacketHeader));
                    write(*client_sock, reply_data, reply_size);
                }
                break;

            case NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE:
                printf( "NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE\r\n" );
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS:
                printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS\r\n" );
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
                printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS\r\n" );
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED:
                printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED\r\n" );
                break;

            case NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE:
                printf( "NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE\r\n" );
                break;

            case NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE:
                printf( "NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE\r\n" );
                break;
        }
    }
}
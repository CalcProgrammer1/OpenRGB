#include "NetworkServer.h"

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
    while(1)
    {
        port.tcp_server_listen();

        //Start a listener thread for the new client socket
#ifdef WIN32
        _beginthread(listen_thread, 0, this);
#else
        pthread_t thread;
        pthread_create(&thread, NULL, &listen_thread, this);
#endif
    }
}

void NetworkServer::ListenThread(SOCKET * client_sock)
{
    //This thread handles messages received from clients
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

        printf("Magic: 'O'\r\n");

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

        printf("Magic: 'R'\r\n");

        //Read third byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[2], 1);
        } while(bytes_read == 0);

        //Test third character of magic - 'G'
        if(header.pkt_magic[2] != 'O')
        {
            continue;
        }

        printf("Magic: 'G'\r\n");

        //Read fourth byte of magic
        do
        {
            bytes_read = port.tcp_listen(&header.pkt_magic[3], 1);
        } while(bytes_read == 0);

        //Test fourth character of magic - 'B'
        if(header.pkt_magic[3] != 'O')
        {
            continue;
        }

        printf("Magic: 'B'\r\n");

        //If we get to this point, the magic is correct.  Read the rest of the header
        do
        {
            bytes_read = port.tcp_listen((char *)&header.pkt_dev_idx, sizeof(header) - sizeof(header.pkt_magic));
        } while(bytes_read == sizeof(header) - sizeof(header.pkt_magic));

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

        printf( "Received header and data\r\n" );

        //Entire request received, select functionality based on request ID
        switch(header.pkt_id)
        {
            NET_PACKET_ID_REQUEST_CONTROLLER_COUNT:
                break;

            NET_PACKET_ID_REQUEST_CONTROLLER_DATA:
                break;

            NET_PACKET_ID_RGBCONTROLLER_RESIZEZONE:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATELEDS:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATEZONELEDS:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATESINGLELED:
                break;

            NET_PACKET_ID_RGBCONTROLLER_SETCUSTOMMODE:
                break;

            NET_PACKET_ID_RGBCONTROLLER_UPDATEMODE:
                break;
        }
    }
}
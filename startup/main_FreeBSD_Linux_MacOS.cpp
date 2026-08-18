/*---------------------------------------------------------*\
| main_FreeBSD_Linux_MacOS.cpp                              |
|                                                           |
|   Entry point for the OpenRGB application                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "cli.h"
#include "DetectionManager.h"
#include "ResourceManager.h"
#include "NetworkServer.h"
#include "LogManager.h"
#include "startup.h"

#ifdef _MACOSX_X86_X64
#include "macUSPCIOAccess.h"
io_connect_t macUSPCIO_driver_connection;
#endif

#ifdef __APPLE__
#include "macutils.h"
#endif

#include <condition_variable>
#include <mutex>

using namespace std::chrono_literals;

#include <csignal>

static volatile bool service_stop_requested = false;
static std::mutex service_stop_mutex;
static std::condition_variable service_stop_cv;

static void sigHandler(int s)
{
    service_stop_requested = true;
    service_stop_cv.notify_one();
}

/*---------------------------------------------------------*\
| WaitWhileServerOnline                                     |
|                                                           |
|   Wait while NetworkServer is online and return only when |
|   it has shut down                                        |
\*---------------------------------------------------------*/
void WaitWhileServerOnline(NetworkServer* srv)
{
    std::unique_lock<std::mutex> lock(service_stop_mutex);
    while(srv->GetOnline())
    {
        if(service_stop_requested)
        {
            srv->StopServer();
            break;
        }
        service_stop_cv.wait_for(lock, 1s);
    };
}

/*---------------------------------------------------------*\
| main                                                      |
|                                                           |
|   Entry point, calls the startup processing               |
\*---------------------------------------------------------*/
int main(int argc, char* argv[])
{
    /*-----------------------------------------------------*\
    | Mac x86/x64 only - Install SMBus Driver macUSPCIO     |
    \*-----------------------------------------------------*/
#ifdef _MACOSX_X86_X64
    InitMacUSPCIODriver();
#endif

    /*-----------------------------------------------------*\
    | Perform CLI pre-detection processing to get return    |
    | flags                                                 |
    \*-----------------------------------------------------*/
    unsigned int ret_flags = cli_pre_detection(argc, argv);

    /*-----------------------------------------------------*\
    | Initialize ResourceManager                            |
    \*-----------------------------------------------------*/
    ResourceManager::get()->Initialize(
        !(ret_flags & RET_FLAG_NO_AUTO_CONNECT),
        !(ret_flags & RET_FLAG_NO_DETECT),
        ret_flags & RET_FLAG_START_SERVER,
        ret_flags & RET_FLAG_CLI_POST_DETECTION,
        ret_flags & RET_FLAG_START_GUI);

    /*-----------------------------------------------------*\
    | If running as a headless server, register signal      |
    | handler for stopping server                           |
    \*-----------------------------------------------------*/
    if((ret_flags & RET_FLAG_START_SERVER) && !(ret_flags & RET_FLAG_START_GUI))
    {
        std::signal(SIGINT,  sigHandler);
        std::signal(SIGTERM, sigHandler);
    }

    /*-----------------------------------------------------*\
    | Perform application startup and run the application.  |
    | This call returns only when the GUI application is    |
    | closing or if not running the GUI.                    |
    \*-----------------------------------------------------*/
    int exitval = startup(argc, argv, ret_flags);

    /*-----------------------------------------------------*\
    | If started in headless server mode, wait until server |
    | shuts down before closing application.                |
    \*-----------------------------------------------------*/
    if((ret_flags & RET_FLAG_START_SERVER) && !(ret_flags & RET_FLAG_START_GUI))
    {
        NetworkServer* server = ResourceManager::get()->GetServer();
        if(server)
        {
            WaitWhileServerOnline(server);
        }
    }

    /*-----------------------------------------------------*\
    | Call ServiceShutdown to allow operations before       |
    | controllers are closed and deleted. Only runs when    |
    | running as a background service (headless server).    |
    \*-----------------------------------------------------*/
    ResourceManager::get()->ServiceShutdown();

    /*-----------------------------------------------------*\
    | Clean up detected devices so destructors can run.     |
    \*-----------------------------------------------------*/
    DetectionManager::get()->Cleanup();

    LOG_TRACE("OpenRGB finishing with exit code %d", exitval);

    /*-----------------------------------------------------*\
    | Mac x86/x64 only - Uninstall SMBus Driver macUSPCIO   |
    \*-----------------------------------------------------*/
#ifdef _MACOSX_X86_X64
    CloseMacUSPCIODriver();
#endif

    return exitval;
}

/*---------------------------------------------------------*\
| main_Linux_MacOS.cpp                                      |
|                                                           |
|   Entry point for the OpenRGB application                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "cli.h"
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

using namespace std::chrono_literals;

/*---------------------------------------------------------*\
| WaitWhileServerOnline                                     |
|                                                           |
|   Wait while NetworkServer is online and return only when |
|   it has shut down                                        |
\*---------------------------------------------------------*/
void WaitWhileServerOnline(NetworkServer* srv)
{
    while(srv->GetOnline())
    {
        std::this_thread::sleep_for(1s);
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
        ret_flags & RET_FLAG_CLI_POST_DETECTION);

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
    | Perform ResourceManager cleanup before exiting        |
    \*-----------------------------------------------------*/
    ResourceManager::get()->Cleanup();

    LOG_TRACE("OpenRGB finishing with exit code %d", exitval);

    /*-----------------------------------------------------*\
    | Mac x86/x64 only - Uninstall SMBus Driver macUSPCIO   |
    \*-----------------------------------------------------*/
#ifdef _MACOSX_X86_X64
    CloseMacUSPCIODriver();
#endif

    return exitval;
}

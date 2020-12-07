/******************************************************************************************\
*                                                                                          *
*   main.cpp                                                                               *
*                                                                                          *
*       Main function for OpenAuraSDK GUI project                                          *
*                                                                                          *
\******************************************************************************************/

#include "ResourceManager.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "OpenRGB.h"
#include "ProfileManager.h"
#include "RGBController.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include "OpenRGBDialog2.h"

using namespace std::chrono_literals;

/*-------------------------------------------------------------*\
| Command line functionality and return flags                   |
\*-------------------------------------------------------------*/
extern unsigned int cli_pre_detection(int argc, char *argv[]);
extern unsigned int cli_post_detection(int argc, char *argv[]);

enum
{
    RET_FLAG_PRINT_HELP         = 1,
    RET_FLAG_START_GUI          = 2,
    RET_FLAG_I2C_TOOLS          = 4,
    RET_FLAG_START_MINIMIZED    = 8,
    RET_FLAG_NO_DETECT          = 16,
    RET_FLAG_CLI_POST_DETECTION = 32,
    RET_FLAG_START_SERVER       = 64,
};

/******************************************************************************************\
*                                                                                          *
*   InitializeTimerResolution (Win32)                                                      *
*                                                                                          *
*       On Windows, the default timer resolution is 15.6ms.  For higher accuracy delays,   *
*       the timer resolution should be set to a shorter interval.  The shortest interval   *
*       that can be set is 0.5ms.                                                          *
*                                                                                          *
\******************************************************************************************/
#ifdef _WIN32
typedef unsigned int NTSTATUS;
typedef NTSTATUS (*NTSETTIMERRESOLUTION)(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);

void InitializeTimerResolution()
{
    NTSETTIMERRESOLUTION NtSetTimerResolution;
    HMODULE              NtDllHandle;
    ULONG                CurrentResolution;

    NtDllHandle = LoadLibrary("ntdll.dll");

    NtSetTimerResolution = (NTSETTIMERRESOLUTION)GetProcAddress(NtDllHandle, "NtSetTimerResolution");

    NtSetTimerResolution(5000, TRUE, &CurrentResolution);
}

void InitializeTimerResolutionThreadFunction()
{
    while(1)
    {
        InitializeTimerResolution();

        std::this_thread::sleep_for(500ms);
    }
}
#endif

/******************************************************************************************\
*                                                                                          *
*   AttemptLocalConnection                                                                 *
*                                                                                          *
*       Attempts an SDK connection to the local server.  Returns true if success           *
*                                                                                          *
\******************************************************************************************/

bool AttemptLocalConnection()
{
    bool success = false;

    NetworkClient * client = new NetworkClient(ResourceManager::get()->GetRGBControllers());

    std::string titleString = "OpenRGB ";
    titleString.append(VERSION_STRING);

    client->SetName(titleString.c_str());
    client->StartClient();

    for(int timeout = 0; timeout < 10; timeout++)
    {
        if(client->GetConnected())
        {
            break;
        }
        std::this_thread::sleep_for(5ms);
    }

    if(!client->GetConnected())
    {
        client->StopClient();

        delete client;

        client = NULL;
    }
    else
    {
        ResourceManager::get()->GetClients().push_back(client);

        success = true;
    }

    return success;
}

/******************************************************************************************\
*                                                                                          *
*   main                                                                                   *
*                                                                                          *
*       Main function.  Detects busses and Aura controllers, then opens the main window    *
*                                                                                          *
\******************************************************************************************/

int main(int argc, char* argv[])
{
#ifdef _WIN32
    /*---------------------------------------------------------*\
    | Windows only - Attach console output                      |
    \*---------------------------------------------------------*/
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        /*---------------------------------------------------------*\
        | We are running under some terminal context; otherwise     |
        | leave the GUI and CRT alone                               |
        \*---------------------------------------------------------*/
        freopen("CONIN$",  "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    /*---------------------------------------------------------*\
    | Windows only - Start timer resolution correction thread   |
    \*---------------------------------------------------------*/
    std::thread * InitializeTimerResolutionThread;
    InitializeTimerResolutionThread = new std::thread(InitializeTimerResolutionThreadFunction);
    InitializeTimerResolutionThread->detach();
#endif

    /*---------------------------------------------------------*\
    | Process command line arguments before detection           |
    \*---------------------------------------------------------*/
    unsigned int ret_flags = RET_FLAG_START_GUI;
    ret_flags |= cli_pre_detection(argc, argv);

    /*---------------------------------------------------------*\
    | Perform local connection and/or hardware detection if not |
    | disabled from CLI                                         |
    \*---------------------------------------------------------*/
    if(!(ret_flags & RET_FLAG_NO_DETECT))
    {
        printf("Attempting to connect to local OpenRGB server.\r\n");

        if(!AttemptLocalConnection())
        {
            printf("Local OpenRGB server unavailable, running standalone.\r\n");

            ResourceManager::get()->DetectDevices();
        }
        else
        {
            printf("Local OpenRGB server connected, running in client mode\r\n");
            
            ResourceManager::get()->DisableDetection();
        }
    }

    /*---------------------------------------------------------*\
    | Start the server if requested from CLI (this must be done |
    | after attempting local connection!)                       |
    \*---------------------------------------------------------*/
    if(ret_flags & RET_FLAG_START_SERVER)
    {
        ResourceManager::get()->GetServer()->StartServer();

        if(!ResourceManager::get()->GetServer()->GetOnline())
        {
            printf("Server failed to start\r\n");
        }
    }

    /*---------------------------------------------------------*\
    | Process command line arguments after detection only if the|
    | pre-detection parsing indicated it should be run          |
    \*---------------------------------------------------------*/
    if(ret_flags & RET_FLAG_CLI_POST_DETECTION)
    {
        ret_flags |= cli_post_detection(argc, argv);
    }

    /*---------------------------------------------------------*\
    | If the command line parser indicates that the GUI should  |
    | run, or if there were no command line arguments, start the|
    | GUI.                                                      |
    \*---------------------------------------------------------*/
    if(ret_flags & RET_FLAG_START_GUI)
    {
        QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication a(argc, argv);

        Ui::OpenRGBDialog2 dlg;

        if(ret_flags & RET_FLAG_I2C_TOOLS)
        {
            dlg.AddI2CToolsPage();
        }

        dlg.AddClientTab();
        
        if(ret_flags & RET_FLAG_START_MINIMIZED)
        {
            dlg.hide();
        }
        else
        {
            dlg.show();
        }
        
        return a.exec();
    }
    else
    {
        return 0;
    }
}

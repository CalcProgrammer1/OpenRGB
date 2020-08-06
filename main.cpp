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

std::vector<NetworkClient*> clients;

/*-------------------------------------------------------------*\
| Command line functionality and return flags                   |
\*-------------------------------------------------------------*/
extern unsigned int cli_main(int argc, char *argv[], std::vector<RGBController *> &rgb_controllers, ProfileManager* profile_manager_in, NetworkServer* network_server_in, std::vector<NetworkClient*> &clients);

enum
{
    RET_FLAG_PRINT_HELP         = 1,
    RET_FLAG_START_GUI          = 2,
    RET_FLAG_I2C_TOOLS          = 4,
    RET_FLAG_START_MINIMIZED    = 8,
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

bool AttemptLocalConnection(std::vector<RGBController*> &rgb_controllers)
{
    bool success = false;

    NetworkClient * client = new NetworkClient(rgb_controllers);
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
        clients.push_back(client);

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

    std::thread * InitializeTimerResolutionThread;
    InitializeTimerResolutionThread = new std::thread(InitializeTimerResolutionThreadFunction);
    InitializeTimerResolutionThread->detach();
#endif

    std::vector<i2c_smbus_interface*> &busses    = ResourceManager::get()->GetI2CBusses();
    std::vector<RGBController*> &rgb_controllers = ResourceManager::get()->GetRGBControllers();

    ProfileManager profile_manager(rgb_controllers);
    NetworkServer server(rgb_controllers);
    
    if(!AttemptLocalConnection(rgb_controllers))
    {
        ResourceManager::get()->DetectDevices();
    }

    /*---------------------------------------------------------*\
    | Process command line arguments                            |
    \*---------------------------------------------------------*/
    unsigned int ret_flags = RET_FLAG_START_GUI;
    if(argc > 1)
    {
        ret_flags = cli_main(argc, argv, rgb_controllers, &profile_manager, &server, clients);
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

        Ui::OpenRGBDialog2 dlg(busses, rgb_controllers, &profile_manager);

        if(ret_flags & RET_FLAG_I2C_TOOLS)
        {
            dlg.AddI2CToolsPage();
        }

        if(clients.size() == 0)
        {
            dlg.AddServerTab(&server);
        }

        dlg.AddClientTab();

        for(int client_idx = 0; client_idx < clients.size(); client_idx++)
        {
            dlg.AddClient(clients[client_idx]);
        }
        
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

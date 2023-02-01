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
#include "ProfileManager.h"
#include "RGBController.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

#ifdef _MACOSX_X86_X64
#include "macUSPCIOAccess.h"
io_connect_t macUSPCIO_driver_connection;
#endif

#include "OpenRGBDialog2.h"

#ifdef __APPLE__
#include "macutils.h"
#endif

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
    RET_FLAG_NO_AUTO_CONNECT    = 128,
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

void WaitWhileServerOnline(NetworkServer* srv)
{
    while (srv->GetOnline())
    {
        std::this_thread::sleep_for(1s);
    };
}

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
        ResourceManager::get()->RegisterNetworkClient(client);

        success = true;

        /*-----------------------------------------------------*\
        | Wait up to 5 seconds for the client connection to     |
        | retrieve all controllers                              |
        \*-----------------------------------------------------*/
        for(int timeout = 0; timeout < 1000; timeout++)
        {
            if(client->GetOnline())
            {
                break;
            }
            std::this_thread::sleep_for(5ms);
        }
    }

    return success;
}

/******************************************************************************************\
*                                                                                          *
*   Install SMBus Driver WinRing0, If not already installed (Win32)                        *
*                                                                                          *
\******************************************************************************************/
#ifdef _WIN32
void InstallWinRing0()
{
    TCHAR winring0_install_location[MAX_PATH]; // driver final location usually C:\windows\system32\drivers\WinRing0x64.sys
    uint system_path_length = GetSystemDirectory(winring0_install_location, MAX_PATH);
    std::string winring0_filename = "WinRing0.sys";
    BOOL bIsWow64 = false;
#if _WIN64
    winring0_filename = "WinRing0x64.sys";
#else
    BOOL (*fnIsWow64Process)(HANDLE, PBOOL) = (BOOL (__cdecl *)(HANDLE, PBOOL))GetProcAddress(GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
    if (fnIsWow64Process)
    {
        fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
    }
    if(bIsWow64)
    {
        winring0_filename = "WinRing0x64.sys";
    }
#endif
    std::strncat(winring0_install_location, "\\drivers\\", MAX_PATH - system_path_length - 1);
    std::strncat(winring0_install_location, winring0_filename.c_str(), MAX_PATH - system_path_length - 10);

    std::string driver_name = winring0_filename.substr(0, winring0_filename.size() - 4); // driver name: WinRing0 or WinRing0x64
    SC_HANDLE manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (manager)
    {
        PVOID wow64_fsredirection_OldValue = NULL;
        if(bIsWow64)
        {
            Wow64DisableWow64FsRedirection(&wow64_fsredirection_OldValue);
        }
        if(INVALID_FILE_ATTRIBUTES == GetFileAttributes(winring0_install_location) && GetLastError()==ERROR_FILE_NOT_FOUND)
        {
            char module_path_buffer[MAX_PATH];
            GetModuleFileNameA(NULL, module_path_buffer, MAX_PATH);
            std::string::size_type exe_loc = std::string(module_path_buffer).find_last_of("\\/");
            std::string driver_source_path = std::string(module_path_buffer).substr(0, exe_loc + 1) + winring0_filename;
            CopyFile(driver_source_path.c_str(), winring0_install_location, true);
        }
        if(bIsWow64)
        {
            Wow64RevertWow64FsRedirection(wow64_fsredirection_OldValue);
        }

        SC_HANDLE service = OpenService(manager, driver_name.c_str(), SERVICE_ALL_ACCESS);
        if(!service)
        {
            std::string service_sys_path = "System32\\Drivers\\" + winring0_filename;
            service = CreateService(manager,
               driver_name.c_str(),
               driver_name.c_str(),
               SERVICE_ALL_ACCESS,
               SERVICE_KERNEL_DRIVER,
               SERVICE_AUTO_START,
               SERVICE_ERROR_NORMAL,
               service_sys_path.c_str(),
               NULL,
               NULL,
               NULL,
               NULL,
               NULL);
        }
        CloseServiceHandle(service);
        CloseServiceHandle(manager);
    }
}
#endif

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

    /*---------------------------------------------------------*\
    | Windows only - Install SMBus Driver WinRing0              |
    \*---------------------------------------------------------*/
    InstallWinRing0();
#endif

    /*---------------------------------------------------------*\
    | Mac x86/x64 only - Install SMBus Driver macUSPCIO         |
    \*---------------------------------------------------------*/
#ifdef _MACOSX_X86_X64
    InitMacUSPCIODriver();
#endif

    /*---------------------------------------------------------*\
    | Process command line arguments before detection           |
    \*---------------------------------------------------------*/
    unsigned int ret_flags = cli_pre_detection(argc, argv);

    /*---------------------------------------------------------*\
    | Perform local connection and/or hardware detection if not |
    | disabled from CLI                                         |
    \*---------------------------------------------------------*/
    if(!(ret_flags & RET_FLAG_NO_AUTO_CONNECT))
    {
        printf("Attempting to connect to local OpenRGB server.\r\n");

        if(!AttemptLocalConnection())
        {
            printf("Local OpenRGB server unavailable.\r\n");
        }
        else
        {
            printf("Local OpenRGB server connected, running in client mode\r\n");

            ResourceManager::get()->DisableDetection();
        }
    }

    /*---------------------------------------------------------*\
    | Perform hardware detection if not disabled from CLI       |
    \*---------------------------------------------------------*/
    if(!(ret_flags & RET_FLAG_NO_DETECT))
    {
        if(ResourceManager::get()->GetDetectionEnabled())
        {
            printf("Running standalone.\r\n");
        }

        ResourceManager::get()->DetectDevices();
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

        /*---------------------------------------------------------*\
        | Main UI widget                                            |
        \*---------------------------------------------------------*/
        Ui::OpenRGBDialog2 dlg;

        if(ret_flags & RET_FLAG_I2C_TOOLS)
        {
            dlg.AddI2CToolsPage();
        }

        dlg.AddClientTab();

        if(ret_flags & RET_FLAG_START_MINIMIZED)
        {
#ifdef __APPLE__
            MacUtils::ToggleApplicationDocklessState(false);
#endif
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
        if(ret_flags & RET_FLAG_START_SERVER)
        {
            if(!ResourceManager::get()->GetServer()->GetOnline())
            {
#ifdef _MACOSX_X86_X64
                CloseMacUSPCIODriver();
#endif
                return 1;
            }
            else
            {
                WaitWhileServerOnline(ResourceManager::get()->GetServer());
#ifdef _MACOSX_X86_X64
                CloseMacUSPCIODriver();
#endif
            }
        }
        else
        {
#ifdef _MACOSX_X86_X64
            CloseMacUSPCIODriver();
#endif
            return 0;
        }
    }
}

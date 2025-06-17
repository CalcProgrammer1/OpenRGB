/*---------------------------------------------------------*\
| main.cpp                                                  |
|                                                           |
|   Entry point for the OpenRGB application                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include "cli.h"
#include "ResourceManager.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "ProfileManager.h"
#include "RGBController.h"
#include "i2c_smbus.h"
#include "LogManager.h"

#ifdef _MACOSX_X86_X64
#include "macUSPCIOAccess.h"
io_connect_t macUSPCIO_driver_connection;
#endif

#include "OpenRGBDialog.h"

#ifdef __APPLE__
#include "macutils.h"
#endif

#ifdef __linux__
#include <csignal>
#endif

using namespace std::chrono_literals;

/******************************************************************************************\
*                                                                                          *
*   InitializeTimerResolutionThreadFunction (Win32)                                        *
*                                                                                          *
*       On Windows, the default timer resolution is 15.6ms.  For higher accuracy delays,   *
*       the timer resolution should be set to a shorter interval.  The shortest interval   *
*       that can be set is 0.5ms.                                                          *
*                                                                                          *
\******************************************************************************************/
#ifdef _WIN32
void InitializeTimerResolutionThreadFunction()
{
    /*-----------------------------------------------------*\
    | NtSetTimerResolution function pointer type            |
    \*-----------------------------------------------------*/
    typedef unsigned int NTSTATUS;
    typedef NTSTATUS (*NTSETTIMERRESOLUTION)(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);

    /*-----------------------------------------------------*\
    | PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION = 4, |
    | isn't defined in Win10 headers                        |
    \*-----------------------------------------------------*/
    PROCESS_POWER_THROTTLING_STATE PowerThrottlingState { PROCESS_POWER_THROTTLING_CURRENT_VERSION, 4, 0 };

    ULONG                CurrentResolution;
    HMODULE              NtDllHandle;
    NTSETTIMERRESOLUTION NtSetTimerResolution;

    /*-----------------------------------------------------*\
    | Load ntdll.dll and get pointer to NtSetTimerResolution|
    \*-----------------------------------------------------*/
    NtDllHandle             = LoadLibrary("ntdll.dll");
    NtSetTimerResolution    = (NTSETTIMERRESOLUTION)GetProcAddress(NtDllHandle, "NtSetTimerResolution");

    /*-----------------------------------------------------*\
    | Windows 11 requires                                   |
    | PROCESS_POWER_THROTTLING_IGNORE_TIMER_RESOLUTION      |
    \*-----------------------------------------------------*/
    SetProcessInformation(GetCurrentProcess(), ProcessPowerThrottling, &PowerThrottlingState, sizeof(PowerThrottlingState));

    /*-----------------------------------------------------*\
    | Call NtSetTimerResolution to set timer resolution to  |
    | 0.5ms every 500ms                                     |
    \*-----------------------------------------------------*/
    while(1)
    {
        NtSetTimerResolution(5000, TRUE, &CurrentResolution);

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
*   Linux signal handler                                                                   *
*                                                                                          *
\******************************************************************************************/
#ifdef __linux__
void sigHandler(int s)
{
    std::signal(s, SIG_DFL);
    qApp->quit();
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
    int exitval = EXIT_SUCCESS;
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

    ResourceManager::get()->Initialize(
        !(ret_flags & RET_FLAG_NO_AUTO_CONNECT),
        !(ret_flags & RET_FLAG_NO_DETECT),
        ret_flags & RET_FLAG_START_SERVER,
        ret_flags & RET_FLAG_CLI_POST_DETECTION);

    /*---------------------------------------------------------*\
    | If the command line parser indicates that the GUI should  |
    | run, or if there were no command line arguments, start the|
    | GUI.                                                      |
    \*---------------------------------------------------------*/
    if(ret_flags & RET_FLAG_START_GUI)
    {
        LOG_TRACE("[main] initializing GUI");

        /*-----------------------------------------------------*\
        | Enable high DPI scaling support                       |
        \*-----------------------------------------------------*/
        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps,    true);
            QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
        #endif

        /*-----------------------------------------------------*\
        | Enable high DPI fractional scaling support on Windows |
        \*-----------------------------------------------------*/
        #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0) && defined(Q_OS_WIN)
            QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
        #endif

        QApplication a(argc, argv);
        QGuiApplication::setDesktopFileName("org.openrgb.OpenRGB");
        LOG_TRACE("[main] QApplication created");

        /*---------------------------------------------------------*\
        | Main UI widget                                            |
        \*---------------------------------------------------------*/
        OpenRGBDialog dlg;
        LOG_TRACE("[main] Dialog created");

        if(ret_flags & RET_FLAG_I2C_TOOLS)
        {
            dlg.AddI2CToolsPage();
        }

        dlg.AddClientTab();

        if(ret_flags & RET_FLAG_START_MINIMIZED)
        {
#ifdef _WIN32
            /*---------------------------------------------------------*\
            | Show the window always, even if it will immediately be    |
            | hidden.  On Windows, events are not delivered to          |
            | nativeEventFilter (for SuspendResume) until the window    |
            | has been shown once.                                      |
            \*---------------------------------------------------------*/
            dlg.showMinimized();
#endif
#ifdef __APPLE__
            MacUtils::ToggleApplicationDocklessState(false);
#endif
            dlg.hide();
        }
        else
        {
            dlg.show();
        }

        LOG_TRACE("[main] Ready to exec() the dialog");
#ifdef __linux__
        std::signal(SIGINT,  sigHandler);
        std::signal(SIGTERM, sigHandler);
#endif
        exitval = a.exec();
    }
    else
    {
        /*---------------------------------------------------------*\
        | If no GUI is needed, we let the background threads run    |
        | as long as they need, but we need to AT LEAST wait for    |
        | initialization to finish                                  |
        \*---------------------------------------------------------*/
        ResourceManager::get()->WaitForInitialization();

        if(ret_flags & RET_FLAG_START_SERVER)
        {
            NetworkServer* server = ResourceManager::get()->GetServer();

            if(!server->GetOnline())
            {
                exitval = EXIT_FAILURE;
            }
            else
            {
                WaitWhileServerOnline(server);
            }
        }
    }
    ResourceManager::get()->Cleanup();
#ifdef _MACOSX_X86_X64
    CloseMacUSPCIODriver();
#endif
    LOG_TRACE("OpenRGB finishing with exit code %d", exitval);
    return exitval;
}

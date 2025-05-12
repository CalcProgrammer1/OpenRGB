/*---------------------------------------------------------*\
| main.cpp                                                  |
|                                                           |
|   Entry point for the OpenRGB application                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ResourceManager.h"
#include "NetworkServer.h"
#include "LogManager.h"
#include "cli.h"

#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <thread>

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
    /*-----------------------------------------------------*\
    | Windows only - Attach console output                  |
    \*-----------------------------------------------------*/
    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        /*-------------------------------------------------*\
        | We are running under some terminal context;       |
        | otherwise leave the GUI and CRT alone             |
        \*-------------------------------------------------*/
        freopen("CONIN$",  "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }

    /*-----------------------------------------------------*\
    | Windows only - Start timer resolution correction      |
    | thread                                                |
    \*-----------------------------------------------------*/
    std::thread * InitializeTimerResolutionThread;
    InitializeTimerResolutionThread = new std::thread(InitializeTimerResolutionThreadFunction);
    InitializeTimerResolutionThread->detach();
#endif

    /*-----------------------------------------------------*\
    | Mac x86/x64 only - Install SMBus Driver macUSPCIO     |
    \*-----------------------------------------------------*/
#ifdef _MACOSX_X86_X64
    InitMacUSPCIODriver();
#endif

    /*-----------------------------------------------------*\
    | Process command line arguments before detection       |
    \*-----------------------------------------------------*/
    unsigned int ret_flags = cli_pre_detection(argc, argv);

    ResourceManager::get()->Initialize(
        !(ret_flags & RET_FLAG_NO_AUTO_CONNECT),
        !(ret_flags & RET_FLAG_NO_DETECT),
        ret_flags & RET_FLAG_START_SERVER,
        ret_flags & RET_FLAG_CLI_POST_DETECTION);

    /*-----------------------------------------------------*\
    | If the command line parser indicates that the GUI     |
    | should run, or if there were no command line          |
    | arguments, start the GUI.                             |
    \*-----------------------------------------------------*/
    if(ret_flags & RET_FLAG_START_GUI)
    {
        LOG_TRACE("[main] initializing GUI");

        /*-------------------------------------------------*\
        | Enable high DPI scaling support                   |
        \*-------------------------------------------------*/
        #if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps,    true);
            QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
        #endif

        /*-------------------------------------------------*\
        | Enable high DPI fractional scaling support on     |
        | Windows                                           |
        \*-------------------------------------------------*/
        #if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0) && defined(Q_OS_WIN)
            QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
        #endif

        QApplication a(argc, argv);
        QGuiApplication::setDesktopFileName("org.openrgb.OpenRGB");
        LOG_TRACE("[main] QApplication created");

        /*-------------------------------------------------*\
        | Main UI widget                                    |
        \*-------------------------------------------------*/
        OpenRGBDialog dlg;
        LOG_TRACE("[main] Dialog created");

        if(ret_flags & RET_FLAG_I2C_TOOLS)
        {
            dlg.AddI2CToolsPage();
        }

        if(ret_flags & RET_FLAG_START_MINIMIZED)
        {
#ifdef _WIN32
            /*---------------------------------------------*\
            | Show the window always, even if it will       |
            | immediately be hidden.  On Windows, events    |
            | are not delivered to nativeEventFilter (for   |
            | SuspendResume) until the window has been      |
            | shown once.                                   |
            \*---------------------------------------------*/
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
        /*-------------------------------------------------*\
        | If no GUI is needed, we let the background        |
        | threads run as long as they need, but we need to  |
        | AT LEAST wait for initialization to finish        |
        \*-------------------------------------------------*/
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

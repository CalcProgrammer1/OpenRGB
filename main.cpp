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

using namespace std::chrono_literals;

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
        QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps,    true);
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

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
        Ui::OpenRGBDialog dlg;
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

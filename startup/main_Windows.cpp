/*---------------------------------------------------------*\
| main_Windows.cpp                                          |
|                                                           |
|   Entry point for the OpenRGB application on Windows      |
|   Differentiate between service and application startup   |
|   Housekeeping for Windows service management             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <thread>

#include <QApplication>

#include "cli.h"
#include "startup.h"
#include "LogManager.h"
#include "NetworkServer.h"
#include "ResourceManager.h"

using namespace std::chrono_literals;

static int common_main(int argc, char* argv[]);

static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);
static void ReportServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);

static char                  service_name[]             = "OpenRGB";
static SERVICE_TABLE_ENTRY   service_dispatch_table[]   = { { service_name, ServiceMain }, { NULL, NULL } };
static DWORD                 service_checkpoint         = 1;
static SERVICE_STATUS_HANDLE service_status_handle;
static SERVICE_STATUS        service_status;

static bool                  started_as_service;
static volatile bool         service_stop_requested;
static bool                  have_console;

static std::mutex            service_status_mutex;

/*---------------------------------------------------------*\
| Detection progress goes from 0 to 100 twice, passing an   |
| estimate based on that confuses the service manager       |
\*---------------------------------------------------------*/
static int                   detection_pass;
static unsigned int          lastpercent                = 101;

/*---------------------------------------------------------*\
| ServiceStartupProgress                                    |
|                                                           |
|   Report detection progress when running as a service     |
\*---------------------------------------------------------*/
static void ServiceStartupProgress(void*)
{
    unsigned int percent = ResourceManager::get()->GetDetectionPercent();
    unsigned int estimate;

    percent = std::clamp(percent, 0u, 100u);

    if(lastpercent > percent)
    {
        detection_pass += 1;
    }

    lastpercent = percent;

    switch(detection_pass)
    {
        case 0:
            percent = 0;
            break;
        case 1:
            percent = percent * 4 / 5;
            break;
        case 2:
            percent = percent / 5 + 80;
            break;
        default:
            percent = 100;
            break;
    }

    estimate = (100 - percent) / 5 + 10;

    ReportServiceStatus(SERVICE_START_PENDING, NO_ERROR, estimate * 1000);
}

/*---------------------------------------------------------*\
| ReportServiceStatus                                       |
|                                                           |
|   As of writing this (24H2) there are 7 possible service  |
|   states:                                                 |
|       SERVICE_START_PENDING, SERVICE_RUNNING,             |
|       SERVICE_STOP_PENDING, SERVICE_STOPPED,              |
|       SERVICE_PAUSE_PENDING, SERVICE_PAUSED,              |
|       SERVICE_CONTINUE_PENDING                            |
|                                                           |
|   We don't accept pause requests, so only start_pending   |
|   through stopped are possible.                           |
|                                                           |
|   Control requests can arrive asynchronously so we take   |
|   some precautions here not to report an out of order     |
|   status.                                                 |
\*---------------------------------------------------------*/
static void ReportServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
    static DWORD last_state = SERVICE_START_PENDING;

    LOG_TRACE("ReportServiceStatus(%lu, %lu, %lu), PID %lu Thread %lu", dwCurrentState, dwWin32ExitCode, dwWaitHint, GetCurrentProcessId(), GetCurrentThreadId());

    service_status_mutex.lock();

    switch(last_state)
    {
        case SERVICE_RUNNING:
            /*---------------------------------------------*\
            | Don't go back to SERVICE_START_PENDING        |
            \*---------------------------------------------*/
            if(dwCurrentState == SERVICE_START_PENDING)
            {
                service_status_mutex.unlock();
                return;
            }
            break;

        case SERVICE_STOP_PENDING:
            /*---------------------------------------------*\
            | Don't go back to starting or running          |
            \*---------------------------------------------*/
            if((dwCurrentState != SERVICE_STOP_PENDING) && (dwCurrentState != SERVICE_STOPPED))
            {
                service_status_mutex.unlock();
                return;
            }
            break;

        case SERVICE_STOPPED:
            /*---------------------------------------------*\
            | Don't call SetServiceStatus anymore after     |
            | SERVICE_STOPPED has been reported             |
            \*---------------------------------------------*/
            service_status_mutex.unlock();
            return;
    }

    switch(dwCurrentState)
    {
        case SERVICE_START_PENDING:
        case SERVICE_STOP_PENDING:
            service_status.dwControlsAccepted = 0;
            service_status.dwCheckPoint = service_checkpoint;
            service_checkpoint += 1;
            break;

        case SERVICE_RUNNING:
        case SERVICE_STOPPED:
            service_status.dwCheckPoint = 0;
            service_status.dwControlsAccepted =
                // SERVICE_ACCEPT_POWEREVENT   |
                SERVICE_ACCEPT_STOP         |
                SERVICE_ACCEPT_PRESHUTDOWN;
            break;

        default:
            /*---------------------------------------------*\
            | Shouldn't happen                              |
            \*---------------------------------------------*/
            service_status_mutex.unlock();
            return;
    }

    last_state                      = dwCurrentState;
    service_status.dwCurrentState   = dwCurrentState;
    service_status.dwWin32ExitCode  = dwWin32ExitCode;
    service_status.dwWaitHint       = dwWaitHint;

    SetServiceStatus(service_status_handle, &service_status);

    service_status_mutex.unlock();

    return;
}

/*---------------------------------------------------------*\
| ServiceControlHandler                                     |
|                                                           |
|   Handler function to register with                       |
|   RegisterServiceControlHandlerEx, to handle service      |
|   control operations (stop/shutdown).                     |
\*---------------------------------------------------------*/
static DWORD WINAPI ServiceControlHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData, LPVOID lpContext)
{
    DWORD retval = NO_ERROR;

    LOG_TRACE("ServiceControlHandler(%lu, %lu, %p, %p) called, PID %lu Thread %lu", dwControl, dwEventType, lpEventData, lpContext, GetCurrentProcessId(), GetCurrentThreadId());

    switch(dwControl)
    {
        /*-------------------------------------------------*\
        | TODO either this or                               |
        | RegisterSuspendResumeNotification somewhere       |
        \*-------------------------------------------------*/
        // case SERVICE_CONTROL_POWEREVENT:
        case SERVICE_CONTROL_INTERROGATE:
            /*---------------------------------------------*\
            | Should do nothing but return NO_ERROR         |
            \*---------------------------------------------*/
            break;

        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_PRESHUTDOWN:
            ReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 10000);
            service_stop_requested = true;
            break;

        default:
            retval = ERROR_CALL_NOT_IMPLEMENTED;
            break;
    }

    return retval;
}

/*---------------------------------------------------------*\
| LogEvent                                                  |
|                                                           |
|   Log an event to the service log when running as a       |
|   service.  Only used for startup and shutdown events.    |
\*---------------------------------------------------------*/
static void LogEvent(const char *fmt, ...)
{
    va_list args1;
    va_list args2;

    /*-----------------------------------------------------*\
    | Handle variable arguments                             |
    \*-----------------------------------------------------*/
    va_start(args1, fmt);
    va_copy(args2, args1);

    /*-----------------------------------------------------*\
    | Determine string length and allocate buffer           |
    \*-----------------------------------------------------*/
    int     len = vsnprintf(NULL, 0, fmt, args1);
    char *  buf = (char *)malloc(len + 1);

    /*-----------------------------------------------------*\
    | Fill the string buffer                                |
    \*-----------------------------------------------------*/
    va_end(args1);
    vsnprintf(buf, len + 1, fmt, args2);
    va_end(args2);

    /*-----------------------------------------------------*\
    | If we somehow ended up here with a working console    |
    | then just print it                                    |
    \*-----------------------------------------------------*/
    if(have_console)
    {
        printf("%s\n", buf);
        free(buf);
        return;
    }

    /*-----------------------------------------------------*\
    | Otherwise, register event source with our service     |
    | name and report event to the event log                |
    \*-----------------------------------------------------*/
    HANDLE event_source = RegisterEventSource(NULL, service_name);

    if(event_source)
    {
        LPCSTR strings[2] = { service_name, buf };
        ReportEvent(
            event_source,
            EVENTLOG_ERROR_TYPE,
            0,
            1, // error #
            NULL,
            2,
            0,
            strings,
            NULL);
        DeregisterEventSource(event_source);
    }
    else
    {
        /*-------------------------------------------------*\
        | No console, no logmanager, no eventlog, no more   |
        | ideas. Sometimes it's OK to give up.              |
        \*-------------------------------------------------*/
    }

    /*-----------------------------------------------------*\
    | Free the buffer                                       |
    \*-----------------------------------------------------*/
    free(buf);
}

/*---------------------------------------------------------*\
| LogError                                                  |
|                                                           |
|   Log an error event when running as a service            |
\*---------------------------------------------------------*/
static void LogError(const char *fn, DWORD lasterror)
{
    std::string message = std::system_category().message(lasterror);
    LogEvent("%s failed with error code %lu \"%s\"", fn, lasterror, message.c_str());
}

/*---------------------------------------------------------*\
| ServiceMain                                               |
|                                                           |
|   Figures out whether we are started as a service, calls  |
|   common_main one way or another.                         |
\*---------------------------------------------------------*/
static void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    /*-----------------------------------------------------*\
    | Initialize service status                             |
    \*-----------------------------------------------------*/
    service_status.dwServiceType                = SERVICE_WIN32_OWN_PROCESS;
    service_status.dwServiceSpecificExitCode    = 0;

    started_as_service                          = true;
    service_status_handle                       = RegisterServiceCtrlHandlerEx(service_name, ServiceControlHandler, NULL);

    /*-----------------------------------------------------*\
    | Exit if service status handle is invalid              |
    \*-----------------------------------------------------*/
    if(service_status_handle == 0)
    {
        LogError("RegisterServiceCtrlHandlerEx", GetLastError());
        return;
    }

    /*-----------------------------------------------------*\
    | Report service status start pending                   |
    \*-----------------------------------------------------*/
    ReportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 30000);

    /*-----------------------------------------------------*\
    | Perform common main processing                        |
    \*-----------------------------------------------------*/
    int exitval = common_main(dwArgc, lpszArgv);

    /*-----------------------------------------------------*\
    | Log if exit was unsuccessful                          |
    \*-----------------------------------------------------*/
    if(exitval != EXIT_SUCCESS)
    {
        LogEvent("OpenRGB finishing with exit code %d", exitval);
    }

    /*-----------------------------------------------------*\
    | Report service status stopped                         |
    \*-----------------------------------------------------*/
    ReportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
}

/*---------------------------------------------------------*\
| main                                                      |
|                                                           |
|   Entry point, checks if started as a service and then    |
|   calls the common main processing                        |
\*---------------------------------------------------------*/
int main(int argc, char* argv[])
{
    started_as_service = false;

    /*-----------------------------------------------------*\
    | This will call ServiceMain() if we are started as a   |
    | service                                               |
    \*-----------------------------------------------------*/
    bool sscd = StartServiceCtrlDispatcher(service_dispatch_table);

    /*-----------------------------------------------------*\
    | ... returns a specific error code otherwise           |
    \*-----------------------------------------------------*/
    DWORD lasterror = GetLastError();

    if(sscd)
    {
        if(started_as_service)
        {
            /*---------------------------------------------*\
            | StartServiceCtrlDispatcher reported success,  |
            | ServiceMain was reached. Don't log anything,  |
            | it's the normal exit path when a service is   |
            | stopped.                                      |
            \*---------------------------------------------*/
            return EXIT_SUCCESS;
        }
        else
        {
            /*---------------------------------------------*\
            | Should not happen                             |
            \*---------------------------------------------*/
            LogEvent("StartServiceCtrlDispatcher returned true, but ServiceMain has not been reached.");
            return EXIT_FAILURE;
        }
    }

    /*-----------------------------------------------------*\
    | StartServiceCtrlDispatcher reported failure.          |
    | First, check that the error isn't the one returned    |
    | when it's called from an app                          |
    \*-----------------------------------------------------*/
    if(lasterror != ERROR_FAILED_SERVICE_CONTROLLER_CONNECT)
    {
        LogError("StartServiceCtrlDispatcher", lasterror);
        return EXIT_FAILURE;
    }

    /*-----------------------------------------------------*\
    | Now we are quite sure we are not started as a service |
    \*-----------------------------------------------------*/
    if(started_as_service)
    {
        /*-------------------------------------------------*\
        | ... so this should not happen                     |
        \*-------------------------------------------------*/
        LogEvent("StartServiceCtrlDispatcher returned false, but ServiceMain has been reached.");
        return EXIT_FAILURE;
    }

    /*-----------------------------------------------------*\
    | Code that needs to be run at startup when we are      |
    | started as an app and not as a service goes here.     |
    \*-----------------------------------------------------*/

    /*-----------------------------------------------------*\
    | Attach console output                                 |
    \*-----------------------------------------------------*/
    if(AttachConsole(ATTACH_PARENT_PROCESS))
    {
        /*-------------------------------------------------*\
        | We are running under some terminal context;       |
        | otherwise leave the GUI and CRT alone             |
        \*-------------------------------------------------*/
        freopen("CONIN$",  "r", stdin);
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        have_console = true;
    }

    return common_main(argc, argv);
}

/*---------------------------------------------------------*\
| InitializeTimerResolutionThreadFunction (Win32)           |
|                                                           |
|   On Windows, the default timer resolution is 15.6ms.     |
|   For higher accuracy delays, the timer resolution should |
|   be set to a shorter interval.  The shortest interval    |
|   that can be set is 0.5ms.                               |
\*---------------------------------------------------------*/
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

/*---------------------------------------------------------*\
| WaitWhileServerOnline                                     |
|                                                           |
|   Wait while NetworkServer is online and return only when |
|   it has shut down                                        |
\*---------------------------------------------------------*/
static void WaitWhileServerOnline(NetworkServer* srv)
{
    while(srv->GetOnline())
    {
        std::this_thread::sleep_for(1s);
        if(service_stop_requested)
        {
            srv->StopServer();
        }
    };
}

/*---------------------------------------------------------*\
| common_main                                               |
|                                                           |
|   Common entry functionality after determining whether we |
|   were started as a service or not.                       |
\*---------------------------------------------------------*/
static int common_main(int argc, char* argv[])
{
    unsigned int ret_flags;

    if(started_as_service)
    {
        /*-------------------------------------------------*\
        | Passing command line arguments to a service is    |
        | difficult, can cause all kinds of trouble and     |
        | doesn't have a way to warn about them             |
        \*-------------------------------------------------*/
        ret_flags = RET_FLAG_START_SERVER | RET_FLAG_NO_AUTO_CONNECT;
    }
    else
    {
        /*-------------------------------------------------*\
        | Perform CLI pre-detection processing to get       |
        | return flags                                      |
        \*-------------------------------------------------*/
        ret_flags = cli_pre_detection(argc, argv);
    }

    /*-----------------------------------------------------*\
    | Start timer resolution correction thread              |
    \*-----------------------------------------------------*/
    std::thread * InitializeTimerResolutionThread;
    InitializeTimerResolutionThread = new std::thread(InitializeTimerResolutionThreadFunction);
    InitializeTimerResolutionThread->detach();

    /*-----------------------------------------------------*\
    | Initialize ResourceManager                            |
    \*-----------------------------------------------------*/
    ResourceManager::get()->Initialize(
        !(ret_flags & RET_FLAG_NO_AUTO_CONNECT),
        !(ret_flags & RET_FLAG_NO_DETECT),
        ret_flags & RET_FLAG_START_SERVER,
        ret_flags & RET_FLAG_CLI_POST_DETECTION);

    /*-----------------------------------------------------*\
    | If running as a service, register the service startup |
    | progress callback with ResourceManager to report      |
    | startup progress based on detection progress.         |
    \*-----------------------------------------------------*/
    if(started_as_service)
    {
        ResourceManager::get()->RegisterDetectionProgressCallback(ServiceStartupProgress, NULL);
    }

    /*-----------------------------------------------------*\
    | Perform application startup and run the application.  |
    | This call returns only when the GUI application is    |
    | closing or if not running the GUI.                    |
    \*-----------------------------------------------------*/
    int exitval = startup(argc, argv, ret_flags);

    /*-----------------------------------------------------*\
    | If running as a service, unregister the service       |
    | startup progress callback when shutting down.         |
    \*-----------------------------------------------------*/
    if(started_as_service)
    {
        ResourceManager::get()->UnregisterDetectionProgressCallback(ServiceStartupProgress, NULL);
    }

    /*-----------------------------------------------------*\
    | If started in headless server mode, wait until server |
    | shuts down before closing application.                |
    \*-----------------------------------------------------*/
    if((ret_flags & RET_FLAG_START_SERVER) && !(ret_flags & RET_FLAG_START_GUI))
    {
        NetworkServer* server = ResourceManager::get()->GetServer();
        if(server && server->GetOnline())
        {
            if(started_as_service)
            {
                ReportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);
            }
            WaitWhileServerOnline(server);
        }
    }

    /*-----------------------------------------------------*\
    | Perform ResourceManager cleanup before exiting        |
    \*-----------------------------------------------------*/
    ResourceManager::get()->Cleanup();

    LOG_TRACE("OpenRGB finishing with exit code %d", exitval);

    return exitval;
}

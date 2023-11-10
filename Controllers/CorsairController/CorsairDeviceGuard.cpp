/*---------------------------------------------------------*\
|  CorsairDeviceGuard.h                                     |
|                                                           |
|  A DeviceGuard implementation for Corsair devices.        |
|                                                           |
|  On Windows platforms, a global mutex is used.            |
|                                                           |
|  Evan Mulawski, 2023-09-04                                |
|                                                           |
\*---------------------------------------------------------*/

#include "CorsairDeviceGuard.h"

CorsairDeviceGuard::CorsairDeviceGuard() : DeviceGuard()
{
#ifdef _WIN32
    mutex_handle = CreateWindowsMutex();
#endif
}

void CorsairDeviceGuard::Acquire()
{
#ifdef _WIN32
    while(true)
    {
        DWORD result = WaitForSingleObject(mutex_handle, INFINITE);

        if(result == WAIT_OBJECT_0)
        {
            break;
        }

        if(result == WAIT_ABANDONED)
        {
            ReleaseMutex(mutex_handle);
        }
    }
#endif
}

void CorsairDeviceGuard::Release()
{
#ifdef _WIN32
    ReleaseMutex(mutex_handle);
#endif
}

#ifdef _WIN32

HANDLE CorsairDeviceGuard::CreateWindowsMutex()
{
    SECURITY_DESCRIPTOR sd;
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = FALSE;

    return CreateMutex(&sa, FALSE, "Global\\CorsairLinkReadWriteGuardMutex");
}

#endif

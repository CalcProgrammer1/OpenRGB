/*---------------------------------------------------------*\
| CorsairDeviceGuard.cpp                                    |
|                                                           |
|   DeviceGuard for Corsair devices                         |
|                                                           |
|   Evan Mulawski                               04 Sep 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

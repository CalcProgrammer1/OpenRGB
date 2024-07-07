/*---------------------------------------------------------*\
| RazerDeviceGuard.cpp                                      |
|                                                           |
|   DeviceGuard for Razer devices                           |
|                                                           |
|   Aytac Kayadelen                             18 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RazerDeviceGuard.h"

RazerDeviceGuard::RazerDeviceGuard() : DeviceGuard()
{
#ifdef _WIN32
    mutex_handle = CreateWindowsMutex();
#endif
}

void RazerDeviceGuard::Acquire()
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

void RazerDeviceGuard::Release()
{
#ifdef _WIN32
    ReleaseMutex(mutex_handle);
#endif
}

#ifdef _WIN32

HANDLE RazerDeviceGuard::CreateWindowsMutex()
{
    SECURITY_DESCRIPTOR sd;
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = &sd;
    sa.bInheritHandle = FALSE;

    return CreateMutex(&sa, FALSE, "Global\\RazerLinkReadWriteGuardMutex");
}

#endif

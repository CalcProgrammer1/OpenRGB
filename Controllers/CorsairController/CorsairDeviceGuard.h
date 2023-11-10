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

#pragma once

#include "DeviceGuard.h"

#ifdef _WIN32
#include <Windows.h>
#endif

class CorsairDeviceGuard : public DeviceGuard
{
public:
    CorsairDeviceGuard();

    void Acquire() override;
    void Release() override;

private:
#ifdef _WIN32
    HANDLE mutex_handle;

    HANDLE CreateWindowsMutex();
#endif
};

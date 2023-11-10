/*---------------------------------------------------------*\
|  DeviceGuardManager.h                                     |
|                                                           |
|  Responsible for managing a DeviceGuard implementation,   |
|  allowing clients to wait for exclusive access to a       |
|  device using the DeviceGuardLock it provides.            |
|                                                           |
|  Evan Mulawski, 2023-09-05                                |
|                                                           |
\*---------------------------------------------------------*/

#pragma once

#include "DeviceGuardLock.h"

class DeviceGuardManager
{
public:
    DeviceGuardManager(DeviceGuard* guard_ptr);
    ~DeviceGuardManager();

    DeviceGuardLock AwaitExclusiveAccess();

private:
    DeviceGuard* guard;
};

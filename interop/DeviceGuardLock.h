/*---------------------------------------------------------*\
|  DeviceGuardLock.h                                        |
|                                                           |
|  Represents a lock for a device guard, ensuring           |
|  exclusive access to a device when acquired               |
|  and releasing it when destroyed.                         |
|                                                           |
|  Evan Mulawski, 2023-09-05                                |
|                                                           |
\*---------------------------------------------------------*/

#pragma once

#include "DeviceGuard.h"

class DeviceGuardLock
{
public:
    DeviceGuardLock(DeviceGuard& guard_ref);
    ~DeviceGuardLock();

private:
    DeviceGuard& guard;
};

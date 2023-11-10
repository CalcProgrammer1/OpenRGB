/*---------------------------------------------------------*\
|  DeviceGuardLock.cpp                                      |
|                                                           |
|  Represents a lock for a device guard, ensuring           |
|  exclusive access to a device when acquired               |
|  and releasing it when destroyed.                         |
|                                                           |
|  Evan Mulawski, 2023-09-05                                |
|                                                           |
\*---------------------------------------------------------*/

#include "DeviceGuardLock.h"

DeviceGuardLock::DeviceGuardLock(DeviceGuard& guard_ref) : guard(guard_ref)
{
    guard.Acquire();
}

DeviceGuardLock::~DeviceGuardLock()
{
    guard.Release();
}

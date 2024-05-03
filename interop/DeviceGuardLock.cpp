/*---------------------------------------------------------*\
| DeviceGuardLock.cpp                                       |
|                                                           |
|   Represents a lock for a device guard, ensuring          |
|   exclusive access to a device when acquired and          |
|   releasing it when destroyed                             |
|                                                           |
|   Evan Mulawski                               09 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

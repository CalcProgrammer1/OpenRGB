/*---------------------------------------------------------*\
| DeviceGuardManager.h                                      |
|                                                           |
|   Responsible for managing a DeviceGuard implementation,  |
|   allowing clients to wait for exclusive access to a      |
|   device using the DeviceGuardLock it provides            |
|                                                           |
|   Evan Mulawski                               09 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
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

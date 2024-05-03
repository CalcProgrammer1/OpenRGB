/*---------------------------------------------------------*\
| DeviceGuard.h                                             |
|                                                           |
|   An abstract interface for acquiring and releasing       |
|   a guard on a device, serving as a foundation for        |
|   implementing device-specific synchronization mechanisms |
|                                                           |
|   Evan Mulawski                               09 May 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

class DeviceGuard
{
public:
    DeviceGuard();
    virtual ~DeviceGuard();

    virtual void Acquire() = 0;
    virtual void Release() = 0;
};

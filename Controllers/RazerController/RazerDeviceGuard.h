/*---------------------------------------------------------*\
| RazerDeviceGuard.h                                      |
|                                                           |
|   DeviceGuard for Razer devices                           |
|                                                           |
|   Aytac Kayadelen                             18 May 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "DeviceGuard.h"

#ifdef _WIN32
#include <Windows.h>
#endif

class RazerDeviceGuard : public DeviceGuard
{
public:
    RazerDeviceGuard();

    void Acquire() override;
    void Release() override;

private:
#ifdef _WIN32
    HANDLE mutex_handle;

    HANDLE CreateWindowsMutex();
#endif
};

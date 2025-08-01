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

#pragma once

#include "DeviceGuard.h"

#ifdef _WIN32
/*---------------------------------------------------------*\
| Windows interferes with std::max unless NOMINMAX defined  |
\*---------------------------------------------------------*/
#define NOMINMAX
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

/*---------------------------------------------------------*\
| SuspendResume.h                                           |
|                                                           |
|   Suspend/resume common implementation                    |
|                                                           |
|   Zach Deibert (zachdeibert)                  12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

class SuspendResumeListenerBase
{
protected:
    virtual void OnSuspend() = 0;
    virtual void OnResume() = 0;
};

#ifdef _WIN32
#include "SuspendResume_Windows.h"
#endif

#ifdef __APPLE__
#include "SuspendResume_MacOS.h"
#endif

#if defined(__linux__) || defined(__FreeBSD__)
#include "SuspendResume_Linux_FreeBSD.h"
#endif

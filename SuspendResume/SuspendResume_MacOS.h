/*---------------------------------------------------------*\
| SuspendResume_MacOS.h                                     |
|                                                           |
|   Suspend/resume MacOS implementation                     |
|                                                           |
|   Zach Deibert (zachdeibert)                  12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <stdint.h>
#include "SuspendResume.h"
#include "IOKit/pwr_mgt/IOPMLib.h"
#include "IOKit/IOMessage.h"

class SuspendResumeListener : public SuspendResumeListenerBase
{
protected:
    SuspendResumeListener();
    virtual ~SuspendResumeListener();

private:
    static void SystemPowerCallback(void *refcon, io_service_t service, uint32_t message_type, void *message_argument);

    io_connect_t root_port;
    IONotificationPortRef port_ref;
    io_object_t notifier;
};

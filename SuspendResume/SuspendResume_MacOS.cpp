/*---------------------------------------------------------*\
| SuspendResume_MacOS.cpp                                   |
|                                                           |
|   Suspend/resume MacOS implementation                     |
|                                                           |
|   Zach Deibert (zachdeibert)                  12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <stdint.h>
#include "SuspendResume.h"
#include "IOKit/pwr_mgt/IOPMLib.h"
#include "IOKit/IOMessage.h"

SuspendResumeListener::SuspendResumeListener()
{
    root_port = IORegisterForSystemPower(this, &port_ref, &SuspendResumeListener::SystemPowerCallback, &notifier);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), IONotificationPortGetRunLoopSource(port_ref), kCFRunLoopCommonModes);
}

SuspendResumeListener::~SuspendResumeListener()
{
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), IONotificationPortGetRunLoopSource(port_ref), kCFRunLoopCommonModes);
    IODeregisterForSystemPower(&notifier);
    IOServiceClose(root_port);
    IONotificationPortDestroy(port_ref);
}

void SuspendResumeListener::SystemPowerCallback(void *refcon, io_service_t service, uint32_t message_type, void *message_argument)
{
    (void)service;
    SuspendResumeListener *spl = (SuspendResumeListener *)refcon;
    switch(message_type)
    {
        case kIOMessageSystemWillSleep:
            spl->OnSuspend();
            IOAllowPowerChange(spl->root_port, (intptr_t)message_argument);
            break;
        case kIOMessageSystemHasPoweredOn:
            spl->OnResume();
            break;
    }
}

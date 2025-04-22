/*---------------------------------------------------------*\
| SuspendResume_Linux_FreeBSD.cpp                           |
|                                                           |
|   Suspend/resume Linux/FreeBSD implementation             |
|                                                           |
|   Zach Deibert (zachdeibert)                  12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QDBusConnection>
#include "SuspendResume.h"

SuspendResumeLoginManager::SuspendResumeLoginManager(SuspendResumeListener *srl) : srl(srl), bus(QDBusConnection::systemBus())
{
    bus.connect("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", "PrepareForSleep", this, SLOT(PrepareForSleep(bool)));
}

SuspendResumeLoginManager::~SuspendResumeLoginManager()
{
    bus.disconnect("org.freedesktop.login1", "/org/freedesktop/login1", "org.freedesktop.login1.Manager", "PrepareForSleep", this, SLOT(PrepareForSleep(bool)));
}

void SuspendResumeLoginManager::PrepareForSleep(bool mode)
{
    if(mode)
    {
        srl->OnSuspend();
    }
    else
    {
        srl->OnResume();
    }
}

SuspendResumeListener::SuspendResumeListener() : login_manager(this)
{
}

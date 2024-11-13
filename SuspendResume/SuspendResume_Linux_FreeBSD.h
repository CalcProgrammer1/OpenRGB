/*---------------------------------------------------------*\
| SuspendResume_Linux_FreeBSD.h                             |
|                                                           |
|   Suspend/resume Linux/FreeBSD implementation             |
|                                                           |
|   Zach Deibert (zachdeibert)                  12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QDBusConnection>
#include <QObject>
#include "SuspendResume.h"

class SuspendResumeListener;

class SuspendResumeLoginManager : public QObject
{
    Q_OBJECT

public:
    SuspendResumeLoginManager(SuspendResumeListener *srl);
    ~SuspendResumeLoginManager();

public slots:
    void PrepareForSleep(bool mode);

private:
    SuspendResumeListener *srl;
    QDBusConnection bus;
};

class SuspendResumeListener : public SuspendResumeListenerBase
{
    friend class SuspendResumeLoginManager;

protected:
    SuspendResumeListener();

private:
    SuspendResumeLoginManager login_manager;
};

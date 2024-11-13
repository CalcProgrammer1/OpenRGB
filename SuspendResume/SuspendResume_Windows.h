/*---------------------------------------------------------*\
| SuspendResume_Windows.h                                   |
|                                                           |
|   Suspend/resume Windows implementation                   |
|                                                           |
|   Zach Deibert (zachdeibert)                  12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <QAbstractNativeEventFilter>
#include <QByteArray>
#include "SuspendResume.h"

class SuspendResumeListener : public SuspendResumeListenerBase, private QAbstractNativeEventFilter
{
protected:
    SuspendResumeListener();
    virtual ~SuspendResumeListener();

private:
    bool nativeEventFilter(const QByteArray &event_type, void *message, long *result);
};

/*---------------------------------------------------------*\
| SuspendResume_Windows.cpp                                 |
|                                                           |
|   Suspend/resume Windows implementation                   |
|                                                           |
|   Zach Deibert (zachdeibert)                  12 Nov 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <QByteArray>
#include <QCoreApplication>
#include "SuspendResume.h"
#include "windows.h"

SuspendResumeListener::SuspendResumeListener()
{
    QCoreApplication::instance()->installNativeEventFilter(this);
}

SuspendResumeListener::~SuspendResumeListener()
{
    QCoreApplication::instance()->removeNativeEventFilter(this);
}

bool SuspendResumeListener::nativeEventFilter(const QByteArray &event_type, void *message, NEFResultType *result)
{
    (void)result;
    if(event_type == "windows_generic_MSG")
    {
        switch(((MSG *)message)->message)
        {
            case WM_POWERBROADCAST:
                switch(((MSG *)message)->wParam)
                {
                    case PBT_APMSUSPEND:
                        OnSuspend();
                        break;
                    case PBT_APMRESUMEAUTOMATIC:
                        OnResume();
                        break;
                }
                break;
        }
    }
    return false;
}

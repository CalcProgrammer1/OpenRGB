/*---------------------------------------------------------*\
| ResourceManagerCallback.h                                 |
|                                                           |
|   Callback type and update reason codes for               |
|   ResourceManager                                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

/*---------------------------------------------------------*\
| Callback Types                                            |
\*---------------------------------------------------------*/
typedef void (*ResourceManagerCallback)(void *, unsigned int);

/*---------------------------------------------------------*\
| ResourceManager Update Reason Codes                       |
\*---------------------------------------------------------*/
enum
{
    RESOURCEMANAGER_UPDATE_REASON_DETECTION_STARTED,            /* Detection started                */
    RESOURCEMANAGER_UPDATE_REASON_DETECTION_PROGRESS_CHANGED,   /* Detection progress changed       */
    RESOURCEMANAGER_UPDATE_REASON_DETECTION_COMPLETE,           /* Detection completed              */
    RESOURCEMANAGER_UPDATE_REASON_CLIENT_INFO_UPDATED,          /* NetworkClient info updated       */
    RESOURCEMANAGER_UPDATE_REASON_I2C_BUS_LIST_UPDATED,         /* I2C bus list updated             */
    RESOURCEMANAGER_UPDATE_REASON_DEVICE_LIST_UPDATED,          /* Device list updated              */
};

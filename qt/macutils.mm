/*---------------------------------------------------------*\
| macutils.mm                                               |
|                                                           |
|   Utility function for dockless mode in MacOS             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#import <Cocoa/Cocoa.h>
#include "macutils.h"

void MacUtils::ToggleApplicationDocklessState(bool showDock)
{
    ProcessApplicationTransformState transformState = showDock ? ProcessApplicationTransformState(kProcessTransformToForegroundApplication) : ProcessApplicationTransformState(kProcessTransformToUIElementApplication);
    ProcessSerialNumber psn = { 0, kCurrentProcess };
    TransformProcessType(&psn, transformState);
}

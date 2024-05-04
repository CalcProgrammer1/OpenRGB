/*---------------------------------------------------------*\
| macutils.h                                                |
|                                                           |
|   Utility function for dockless mode in MacOS             |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#ifndef MACUTILS_H
#define MACUTILS_H

class MacUtils
{
public:
    static void ToggleApplicationDocklessState(bool showDock);
};

#endif // MACUTILS_H

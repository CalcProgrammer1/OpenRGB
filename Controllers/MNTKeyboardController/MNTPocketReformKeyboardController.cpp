/*---------------------------------------------------------*\
| MNTPocketReformKeyboardController.cpp                     |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "MNTPocketReformKeyboardController.h"

MNTPocketReformKeyboardController::MNTPocketReformKeyboardController(hid_device *dev_handle, const char *path)
{
    dev      = dev_handle;
    location = path;
    kbd_cols = KBD_COLS_POCKET_REFORM;
}

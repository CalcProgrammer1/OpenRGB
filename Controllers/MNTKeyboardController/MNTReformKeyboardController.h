/*---------------------------------------------------------*\
| MNTReformKeyboardController.h                             |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "MNTKeyboardController.h"

#define KBD_COLS_REFORM 14

class MNTReformKeyboardController : public MNTKeyboardController
{
  public:
    MNTReformKeyboardController(hid_device *dev_handle, const char *path);
};

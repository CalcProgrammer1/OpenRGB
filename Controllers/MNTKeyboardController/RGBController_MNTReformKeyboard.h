/*---------------------------------------------------------*\
| RGBController_MNTReformKeyboard.h                         |
|                                                           |
|   Driver for the MNT Reform keyboards                     |
|                                                           |
|   Christian Heller <c.heller@plomlompom.de>  7 Aug 2024   |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController_MNTKeyboard.h"
#include "MNTReformKeyboardController.h"

class RGBController_MNTReformKeyboard : public RGBController_MNTKeyboard
{
  public:
    RGBController_MNTReformKeyboard(MNTReformKeyboardController *controller_ptr);
};

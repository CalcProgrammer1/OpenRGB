/*---------------------------------------------------------*\
| RGBController_MNTPocketReformKeyboard.h                   |
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
#include "MNTPocketReformKeyboardController.h"

class RGBController_MNTPocketReformKeyboard : public RGBController_MNTKeyboard
{
  public:
    RGBController_MNTPocketReformKeyboard(MNTPocketReformKeyboardController *controller_ptr);
};

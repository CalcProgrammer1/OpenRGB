/*---------------------------------------------------------*\
| EpomakerController.cpp                                    |
|                                                           |
|   Compatibility wrapper for ROYUAN-based keyboards        |
|                                                           |
|   Alvaro Munoz (alvaromunoz)                  05 Jun 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "EpomakerController.h"

EpomakerController::EpomakerController(hid_device* dev_handle, char* path)
    : RoyuanKeyboardController(dev_handle, path, RoyuanKeyboardProfile::EpomakerLegacy())
{
}

EpomakerController::~EpomakerController()
{
}

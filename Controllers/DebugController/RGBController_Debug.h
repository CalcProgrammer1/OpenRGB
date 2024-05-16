/*---------------------------------------------------------*\
| RGBController_Debug.h                                     |
|                                                           |
|   RGBController for debug devices                         |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController_Dummy.h"

class RGBController_Debug : public RGBController_Dummy
{
public:
    RGBController_Debug();
    void ResizeZone(int zone, int newSize) override;
};

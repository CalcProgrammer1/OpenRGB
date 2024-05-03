/*---------------------------------------------------------*\
| RGBController_Dummy.cpp                                   |
|                                                           |
|   Dummy RGBController that can mimic various devices for  |
|   development and test purposes                           |
|                                                           |
|   Adam Honse (CalcProgrammer1)                25 Feb 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_Dummy.h"

/**------------------------------------------------------------------*\
    @name Dummy
    @category Dummy
    @type I2C or Serial or WMI or USB
    @save :white_check_mark: or :robot: or :o: or :x:
    @direct :white_check_mark: or :rotating_light: or :o: or :x:
    @effects :white_check_mark: or :rotating_light: or :tools: or :o: or :x:
    @detectors DetectDummy,DetectDummy2
    @comment Insert multiline dummy comment here

        | Symbol | Meaning |
        | :---: | :--- |
        | :white_check_mark: | Fully supported by OpenRGB |
        | :rotating_light: | Support is problematic |
        | :robot: | Feature is automatic and can not be turned off |
        | :tools: | Partially supported by OpenRGB |
        | :o: | Not currently supported by OpenRGB |
        | :x: | Not applicable for this device |
*/

RGBController_Dummy::RGBController_Dummy()
{

}

void RGBController_Dummy::SetupZones()
{

}

void RGBController_Dummy::ResizeZone(int /*zone*/, int /*new_size*/)
{

}

void RGBController_Dummy::DeviceUpdateLEDs()
{

}

void RGBController_Dummy::UpdateZoneLEDs(int /*zone*/)
{

}

void RGBController_Dummy::UpdateSingleLED(int /*led*/)
{

}

void RGBController_Dummy::SetCustomMode()
{

}

void RGBController_Dummy::DeviceUpdateMode()
{

}

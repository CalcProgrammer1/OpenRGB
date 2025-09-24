/*---------------------------------------------------------*\
| RGBController_LogitechHIDPP20.h                           |
|                                                           |
|   RGBController for unified Logitech HID++ 2.0 devices    |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "LogitechHIDPP20Controller.h"

class RGBController_LogitechHIDPP20 : public RGBController
{
public:
    RGBController_LogitechHIDPP20(LogitechHIDPP20Controller* controller_ptr);
    ~RGBController_LogitechHIDPP20();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    bool        ReapplyActiveMode();

private:
    LogitechHIDPP20Controller*    controller;

    /*---------------------------------------------------------*\
    | Repaint callback handler. Registered with the controller  |
    | as request_repaint_fn and invoked from the power thread   |
    | for dim/wake when no animation is driving updates.        |
    \*---------------------------------------------------------*/
    void                        OnRepaintRequest();

    /*---------------------------------------------------------*\
    | When true, the next DeviceUpdateMode cycle sends its      |
    | SetZoneEffect calls with persist=true instead of the      |
    | default ephemeral write. Used by DeviceSaveMode to replay |
    | the active mode as a NVM-committed effect on 0x8070       |
    | devices, which default to non-persistent live writes.     |
    \*---------------------------------------------------------*/
    bool                        save_pending = false;

    /*---------------------------------------------------------*\
    | Maps OpenRGB LED index -> HID++ per-key zone ID           |
    \*---------------------------------------------------------*/
    std::vector<uint16_t>       led_to_zone_id;

    /*---------------------------------------------------------*\
    | Reverse map: zone_id -> LED index (-1 if no LED).         |
    | Indexed 0..255 (zone IDs are bytes). Built once in        |
    | SetupZones to avoid scanning led_to_zone_id at commit     |
    | time, which would be O(N) per acked zone.                 |
    \*---------------------------------------------------------*/
    std::vector<int>            zone_id_to_led_idx;

    /*---------------------------------------------------------*\
    | Last successfully committed colors for delta updates.     |
    | An entry of HIDPP20_UNCOMMITTED (0xFF000000) marks an LED |
    | whose last write didn't ACK and which therefore needs to  |
    | be re-pushed in the next frame regardless of color delta. |
    | The high byte (0xFF) is impossible for any value produced |
    | by ToRGBColor() so it never collides with a real color.   |
    \*---------------------------------------------------------*/
    std::vector<RGBColor>       sent_colors;
    uint32_t                    last_init_gen = 0;
};

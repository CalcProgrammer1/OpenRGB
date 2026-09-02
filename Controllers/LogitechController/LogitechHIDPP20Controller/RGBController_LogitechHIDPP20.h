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
    RGBController_LogitechHIDPP20(LogitechHIDPP20Controller* controller_ptr, std::function<void()> shutdown_callback);
    ~RGBController_LogitechHIDPP20();

    void        SetupZones();
    void        SetupZones8080();
    bool        PerKey8080Capable() const;

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

    void        DeviceUpdateMode();
    void        DeviceSaveMode();

    bool        ReapplyActiveMode();

    LogitechHIDPP20Controller*  GetController() const   { return controller; }

private:
    LogitechHIDPP20Controller*    controller;

    /*-----------------------------------------------------*\
    | Repaint callback handler. Registered with             |
    | the controller as request_repaint_fn and              |
    | invoked from the power thread for dim/wake            |
    | when no animation is driving updates.                 |
    \*-----------------------------------------------------*/
    void                        OnRepaintRequest();

    /*-----------------------------------------------------*\
    | Per-key frame body. Registered with the               |
    | controller as the frame sender callback and           |
    | invoked from its sender thread with the newest        |
    | submitted snapshot: diffs it against sent_colors      |
    | and puts the differences on the wire.                 |
    \*-----------------------------------------------------*/
    void                        SendPerKeyFrame(std::vector<RGBColor>& snapshot);

    /*-----------------------------------------------------*\
    | Build one matrix (map != nullptr) or linear           |
    | zone from a name + geometry + (led name, wire         |
    | value) list. Shared by the static-table layout        |
    | paths (mouse, headset).                               |
    \*-----------------------------------------------------*/
    void                        AddLayoutZone(const std::string&                                   zone_name,
                                              zone_type                                            type,
                                              unsigned int                                         rows,
                                              unsigned int                                         cols,
                                              const unsigned int*                                  map,
                                              const std::vector<std::pair<std::string, uint16_t>>& zone_leds);

    /*-----------------------------------------------------*\
    | When true, the next DeviceUpdateMode cycle sends      |
    | its SetZoneEffect calls with persist=true instead     |
    | of the default ephemeral write. Used by               |
    | DeviceSaveMode to replay the active mode as a         |
    | NVM-committed effect on 0x8070 devices, which         |
    | default to non-persistent live writes.                |
    \*-----------------------------------------------------*/
    bool                        save_pending = false;

    /*-----------------------------------------------------*\
    | Maps OpenRGB LED index -> HID++ per-key zone ID       |
    \*-----------------------------------------------------*/
    std::vector<uint16_t>       led_to_zone_id;

    /*-----------------------------------------------------*\
    | Reverse map: zone_id -> LED index (-1 if no LED).     |
    | Indexed 0..255 (zone IDs are bytes). Built once in    |
    | SetupZones to avoid scanning led_to_zone_id at commit |
    | time, which would be O(N) per acked zone.             |
    \*-----------------------------------------------------*/
    std::vector<int>            zone_id_to_led_idx;

    /*-----------------------------------------------------*\
    | Last successfully committed colors for delta          |
    | updates. An entry of HIDPP20_UNCOMMITTED              |
    | (0xFF000000) marks an LED whose last write didn't     |
    | ACK and which therefore needs to be re-pushed in the  |
    | next frame regardless of color delta. The high byte   |
    | (0xFF) is impossible for any value produced by        |
    | ToRGBColor() so it never collides with a real color.  |
    \*-----------------------------------------------------*/
    std::vector<RGBColor>       sent_colors;
    uint32_t                    last_init_gen = 0;

    /*-----------------------------------------------------*\
    | Rolling resync cursor (see HIDPP20_RESYNC_KEYS_MIN).  |
    | Walks the LED list one frame at a time so every key   |
    | is repainted within a bounded number of frames.       |
    \*-----------------------------------------------------*/
    size_t                      resync_cursor = 0;

    /*-----------------------------------------------------*\
    | 0x8080 addressing: LED index -> (keyType,             |
    | keyId), built in SetupZones from the                  |
    | enumeration. keyType 0 + keyId 0 marks a              |
    | matrix LED the device did not enumerate.              |
    | Parallel to led_to_zone_id; 0x8080 path only.         |
    \*-----------------------------------------------------*/
    std::vector<std::pair<uint16_t, uint8_t>> led_to_keytype_keyid_8080;

    std::function<void()> shutdown_callback;
};

/*---------------------------------------------------------*\
| RGBController_CMARGBGen2A1Controller.h                    |
|                                                           |
|   Driver for Cooler Master ARGB Gen 2 A1 controller       |
|                                                           |
|   Morgan Guimard (morg)                       26 Jun 2022 |
|   Fabian R (kderazorback)                     11 Aug 2023 |
|   Daniel Clark (megadjc)                      14 Oct 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <vector>
#include "RGBController.h"
#include "CMARGBGen2A1Controller.h"

class RGBController_CMARGBGen2A1Controller : public RGBController
{
public:
    RGBController_CMARGBGen2A1Controller(CMARGBGen2A1controller* controller_ptr);
    ~RGBController_CMARGBGen2A1Controller();

    void                        SetupZones();
    void                        DeviceConfigureZone(int zone_idx);
    void                        DeviceUpdateLEDs(std::vector<RGBColor>& update_colors);
    void                        DeviceUpdateZoneLEDs(int zone);
    void                        DeviceUpdateSingleLED(int led);
    void                        DeviceUpdateMode();
    void                        DeviceUpdateZoneMode(int zone) override;
    void                        DeviceSaveMode();
    void                        SetDeviceSpecificConfiguration(nlohmann::json configuration_json) override;
    void                        DeviceConfigureDevice() override;
    void                        DeviceUpdateDeviceSpecificConfiguration() override;

private:
    struct ZoneTarget
    {
        unsigned int            port      = 0;
        unsigned char           subtarget = CM_ARGB_GEN2_A1_GEN1_SUBTARGET;
        bool                    gen2      = false;
    };

    struct ZoneState
    {
        ZoneTarget              target;
        int                     mode_value = -1;
        std::vector<RGBColor>   colors;
    };

    CMARGBGen2A1controller*     controller;
    unsigned int                gen1_zone_led_counts[CM_ARGB_GEN2_A1_CHANNEL_COUNT] = {};
    bool                        gen1_zone_size_configured[CM_ARGB_GEN2_A1_CHANNEL_COUNT] = {};
    std::vector<ZoneTarget>     zone_targets;
    std::vector<int>            multilayer_zone_mode_values;
    unsigned char               multilayer_mode = 0xFF;
    int                         last_global_effect_value = CM_ARGB_GEN2_A1_STATIC_MODE;
    bool                        overlay_enabled = false;
    bool                        overlay_gate_active = false;
    unsigned char               overlay_interval = CM_ARGB_GEN2_A1_OVERLAY_INTERVAL_DEFAULT;
    unsigned char               overlay_tables[CM_ARGB_GEN2_A1_CHANNEL_COUNT][CM_ARGB_GEN2_A1_OVERLAY_TABLE_LENGTH] = {};
    bool                        overlay_table_dirty[CM_ARGB_GEN2_A1_CHANNEL_COUNT] = {};

    void                        InitDeviceSpecificConfiguration();
    void                        ImportDeviceLightingState();
    void                        ImportEffectRecord(mode& target_mode, const CMARGBGen2A1EffectRecord& effect);
    int                         FindGlobalModeIndex(int mode_value) const;
    void                        RebuildDeviceSpecificConfiguration();
    void                        ApplyDeviceSpecificConfiguration(bool setup_zones);
    bool                        ApplyOverlayGate(bool enabled, unsigned char interval);
    bool                        ApplyPendingOverlayTables();
    void                        RebuildLEDConfiguration();
    std::vector<ZoneState>      SnapshotZoneStates() const;
    std::vector<std::vector<RGBColor>> SnapshotZoneColors() const;
    void                        RestoreZoneColors(const std::vector<std::vector<RGBColor>>& saved_colors);
    bool                        SetGen1ZoneLEDCount(unsigned int port, unsigned int led_count);
    bool                        EnsureMultilayerMode(unsigned char mode);
    const ZoneState*            FindPreviousZoneState(const std::vector<ZoneState>& states, const ZoneTarget& target) const;
    int                         FindZoneModeIndex(const zone& current_zone, int mode_value) const;
    void                        PrepareMultilayerModes();
    bool                        ApplyGlobalMode(const mode& selected_mode);
    void                        ApplyModeToZone(unsigned int zone_idx, const mode& selected_mode);
    void                        ApplyMultilayerModes();
    void                        UpdateZoneCustomColors(unsigned int zone_idx);
    std::vector<CMARGBGen2A1RealtimeTarget> BuildRealtimeTargets() const;
    int                         GetLEDZone(int led_idx) const;
};

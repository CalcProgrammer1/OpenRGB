/*---------------------------------------------------------*\
| RGBController_QMKOpenRGBRevB.h                            |
|                                                           |
|   RGBController for OpenRGB QMK Keyboard Protocol         |
|   Revision B                                              |
|                                                           |
|   Kasper                                      28 Jun 2021 |
|   Jath03                                      28 May 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <algorithm>
#include <cmath>
#include <map>
#include <set>
#include "RGBController.h"
#include "QMKOpenRGBRevBController.h"

#define NO_LED 0xFFFFFFFF

typedef std::vector<std::vector<unsigned int>> VectorMatrix;

class RGBController_QMKOpenRGBRevB : public RGBController
{
public:
    RGBController_QMKOpenRGBRevB(QMKOpenRGBRevBController* controller_ptr, bool save);
    ~RGBController_QMKOpenRGBRevB();

    void                                    SetupZones();
    void                                    ResizeZone(int zone, int new_size);

    void                                    DeviceUpdateLEDs();
    void                                    UpdateZoneLEDs(int zone);
    void                                    UpdateSingleLED(int led);

    void                                    DeviceUpdateMode();
    void                                    DeviceSaveMode();

private:
    QMKOpenRGBRevBController*   controller;
    std::vector<unsigned int>   flat_matrix_map;
    std::vector<unsigned int>   flat_underglow_map;

    void                                    InitializeMode
                                            (
                                            std::string name,
                                            unsigned int &current_mode,
                                            unsigned int flags,
                                            unsigned int color_mode,
                                            bool save
                                            );

    unsigned int                            CalculateDivisor
                                            (
                                            std::vector<point_t> led_points,
                                            std::set<int> rows,
                                            std::set<int> columns
                                            );

    void                                    CountKeyTypes
                                            (
                                                std::vector<unsigned int>   led_flags,
                                                unsigned int                total_led_count,
                                                unsigned int&               key_leds,
                                                unsigned int&               underglow_leds
                                            );

    void                                    PlaceLEDsInMaps
                                            (
                                                std::set<int>               unique_rows,
                                                std::set<int>               unique_cols,
                                                unsigned int                divisor,
                                                std::vector<point_t>        led_points,
                                                std::vector<unsigned int>   led_flags,
                                                VectorMatrix&               matrix_map_xl,
                                                VectorMatrix&               underglow_map_xl
                                            );

    VectorMatrix                            MakeEmptyMatrixMap
                                            (
                                                std::size_t height,
                                                std::size_t width
                                            );

    void                                    CleanMatrixMaps
                                            (
                                            VectorMatrix&   matrix_map,
                                            VectorMatrix&   underglow_map,
                                            unsigned int    height,
                                            bool            has_underglow
                                            );

    std::vector<unsigned int>               FlattenMatrixMap
                                            (
                                            VectorMatrix matrix_map
                                            );
};

/*-------------------------------------------------------------------*\
|  RGBController_QMKOpenRGB.h                                         |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol                    |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "QMKOpenRGBController.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#define NO_LED 0xFFFFFFFF

typedef std::vector<std::vector<unsigned int>> VectorMatrix;

class RGBController_QMKOpenRGB : public RGBController
{
public:
    RGBController_QMKOpenRGB(QMKOpenRGBController* controller_ptr);
    ~RGBController_QMKOpenRGB();

    void                                    SetupZones();
    void                                    ResizeZone(int zone, int new_size);

    void                                    DeviceUpdateLEDs();
    void                                    UpdateZoneLEDs(int zone);
    void                                    UpdateSingleLED(int led);

    void                                    SetCustomMode();
    void                                    DeviceUpdateMode();

private:
    QMKOpenRGBController*       controller;
    std::vector<unsigned int>   flat_matrix_map;
    std::vector<unsigned int>   flat_underglow_map;

    void                                    InitializeMode
                                            (
                                            std::string name,
                                            unsigned int &current_mode,
                                            unsigned int flags,
                                            unsigned int color_mode
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
                                            unsigned int height,
                                            unsigned int width
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

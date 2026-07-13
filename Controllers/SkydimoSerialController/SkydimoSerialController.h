/*---------------------------------------------------------*\
| SkydimoSerialController.h                                 |
|                                                           |
|   Driver for Skydimo serial devices                       |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  30 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <mutex>
#include <string>
#include <vector>
#include "RGBController.h"
#include "serial_port.h"

#define SKYDIMO_SERIAL_BAUD_RATE               115200
#define SKYDIMO_SERIAL_QUERY                    "Moni-A"
#define SKYDIMO_SERIAL_QUERY_LENGTH             6
#define SKYDIMO_SERIAL_MODEL_ID_LENGTH          6
#define SKYDIMO_SERIAL_READ_BUFFER_SIZE         64
#define SKYDIMO_SERIAL_MAX_RESPONSE_SIZE        63
#define SKYDIMO_SERIAL_QUERY_ATTEMPTS           10
#define SKYDIMO_SERIAL_DEFAULT_LED_COUNT        0
#define SKYDIMO_SERIAL_DEFAULT_MAX_LED_COUNT    150
#define SKYDIMO_SERIAL_BYTES_PER_LED            3
#define SKYDIMO_SERIAL_FRAME_COUNT_SIZE         2
#define SKYDIMO_SERIAL_MATRIX_NA                0xFFFFFFFF

class SkydimoSerialController
{
public:
    SkydimoSerialController(const std::string& port_name_arg);
    ~SkydimoSerialController();

    bool                    IsPresent();

    void                    SetLEDs(const std::vector<RGBColor>& colors);
    void                    SetBlack();
    void                    SetLEDCount(unsigned int led_count);

    std::string             GetName() const;
    std::string             GetSerial() const;
    std::string             GetLocation() const;

    bool                    IsResizable() const;
    unsigned int            GetLEDCount() const;
    unsigned int            GetMinLEDCount() const;
    unsigned int            GetMaxLEDCount() const;

    unsigned int            GetMatrixWidth() const;
    unsigned int            GetMatrixHeight() const;
    const std::vector<unsigned int>& GetMatrixMap() const;

private:
    bool                    QueryInfo();
    void                    ConfigureOutput();
    void                    SetMatrixCell(unsigned int y, unsigned int x, unsigned int& led_index);
    bool                    WriteFrame(const std::vector<RGBColor>& colors, unsigned int count);

    std::string             port_name;
    std::string             model_name;
    std::string             model_id;
    std::string             serial_id;

    bool                    port_open;
    bool                    present;
    bool                    resizable;

    unsigned int            leds_count;
    unsigned int            leds_min;
    unsigned int            leds_max;
    unsigned int            matrix_width;
    unsigned int            matrix_height;

    std::vector<unsigned int>   matrix_map;
    serial_port                 serial_port_interface;
    std::mutex                  device_mutex;
};

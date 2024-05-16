/*---------------------------------------------------------*\
| RGBController_E131.h                                      |
|                                                           |
|   RGBController for E1.31 devices                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Oct 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <thread>
#include <e131.h>
#include "RGBController.h"

typedef unsigned int e131_rgb_order;

enum
{
    E131_RGB_ORDER_RGB,
    E131_RGB_ORDER_RBG,
    E131_RGB_ORDER_GRB,
    E131_RGB_ORDER_GBR,
    E131_RGB_ORDER_BRG,
    E131_RGB_ORDER_BGR
};

enum
{
    E131_MATRIX_ORDER_HORIZONTAL_TOP_LEFT,
    E131_MATRIX_ORDER_HORIZONTAL_TOP_RIGHT,
    E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_LEFT,
    E131_MATRIX_ORDER_HORIZONTAL_BOTTOM_RIGHT,
    E131_MATRIX_ORDER_VERTICAL_TOP_LEFT,
    E131_MATRIX_ORDER_VERTICAL_TOP_RIGHT,
    E131_MATRIX_ORDER_VERTICAL_BOTTOM_LEFT,
    E131_MATRIX_ORDER_VERTICAL_BOTTOM_RIGHT
};

typedef unsigned int e131_matrix_order;

struct E131Device
{
    std::string name;
    std::string ip;
    unsigned int num_leds;
    unsigned int start_universe;
    unsigned int start_channel;
    unsigned int keepalive_time;
    e131_rgb_order rgb_order;
    zone_type type;
    unsigned int matrix_width;
    unsigned int matrix_height;
    unsigned int universe_size;
    e131_matrix_order matrix_order;
};

class RGBController_E131 : public RGBController
{
public:
    RGBController_E131(std::vector<E131Device> device_list);
    ~RGBController_E131();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
	std::vector<E131Device> 	devices;
    std::vector<e131_packet_t> 	packets;
	std::vector<e131_addr_t> 	dest_addrs;
	std::vector<unsigned int> 	universes;
	int 						sockfd;
    std::thread *               keepalive_thread;
    std::atomic<bool>           keepalive_thread_run;
    std::chrono::milliseconds                           keepalive_delay;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};

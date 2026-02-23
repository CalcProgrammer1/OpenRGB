/*---------------------------------------------------------*\
| RGBController_E131.h                                      |
|                                                           |
|   RGBController for E1.31 devices                         |
|                                                           |
|   Adam Honse (CalcProgrammer1)                18 Oct 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#pragma once

#include <chrono>
#include <thread>
#include <e131.h>
#include "RGBController.h"

struct E131Device
{
    std::string         ip;
    std::string         name;
    unsigned int        keepalive_time;
    unsigned int        num_leds;
    unsigned int        start_channel;
    unsigned int        start_universe;
    unsigned int        universe_size;
};

class RGBController_E131 : public RGBController
{
public:
    RGBController_E131(std::vector<E131Device> device_list);
    ~RGBController_E131();

    void        SetupZones();

    void        DeviceUpdateLEDs();
    void        DeviceUpdateZoneLEDs(int zone);
    void        DeviceUpdateSingleLED(int led);

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

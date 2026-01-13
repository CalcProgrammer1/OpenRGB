/*---------------------------------------------------------*\
| HyperXDRAMControllerDetect.cpp                            |
|                                                           |
|   Driver for HyperX/Kingston Fury RAM                     |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 Jun 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <vector>
#include "DetectionManager.h"
#include "HyperXDRAMController.h"
#include "i2c_smbus.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "RGBController_HyperXDRAM.h"

using namespace std::chrono_literals;

#define HYPERX_CONTROLLER_NAME "HyperX DRAM"

bool TestForHyperXDRAMController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Writing at address %02X, res=%02X", HYPERX_CONTROLLER_NAME, address, res);

    if (res >= 0)
    {
        pass = true;

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != i)
            {
                pass = false;
            }
        }
    }

    return(pass);
}

DetectedControllers DetectHyperXDRAMControllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &/*name*/)
{
    DetectedControllers detected_controllers;
    bool                fury_detected = false;
    bool                pred_detected = false;
    unsigned char       slots_valid   = 0x00;

    // Check for HyperX controller at 0x27
    LOG_DEBUG("[%s] Testing bus %d at address 0x27", HYPERX_CONTROLLER_NAME, bus->port_id);

    if(TestForHyperXDRAMController(bus, 0x27))
    {
        for(SPDWrapper *slot : slots)
        {
            LOG_DEBUG("[%s] SPD check success", HYPERX_CONTROLLER_NAME);

            slots_valid |= (1 << (slot->index()));

            if(slot->manufacturer_data(0x06) == 0x01)
            {
                fury_detected = true;
            }
            else
            {
                pred_detected = true;
            }

            std::this_thread::sleep_for(1ms);
        }

        LOG_DEBUG("[%s] slots_valid=%d fury_detected=%d pred_detected=%d",
                  HYPERX_CONTROLLER_NAME, slots_valid, fury_detected, pred_detected);

        if(slots_valid != 0)
        {
            std::string name = "HyperX DRAM";

            if(fury_detected && !pred_detected)
            {
                name = "HyperX Fury RGB";
            }
            else if(!fury_detected && pred_detected)
            {
                name = "HyperX Predator RGB";
            }

            HyperXDRAMController*     controller     = new HyperXDRAMController(bus, 0x27, slots_valid, name);
            RGBController_HyperXDRAM* rgb_controller = new RGBController_HyperXDRAM(controller);

            detected_controllers.push_back(rgb_controller);
        }
    }

    return(detected_controllers);
}

REGISTER_I2C_DRAM_DETECTOR("HyperX DRAM", DetectHyperXDRAMControllers, JEDEC_KINGSTON, SPD_DDR4_SDRAM);

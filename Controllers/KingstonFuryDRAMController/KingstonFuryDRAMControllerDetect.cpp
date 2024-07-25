/*---------------------------------------------------------*\
|  KingstonFuryDRAMControllerDetect.cpp                     |
|                                                           |
|   Detection of Kingston Fury DDR4/5 RAM modules           |
|                                                           |
|   Geofrey Mon (geofbot)                       14 Jul 2024 |
|   Milan Cermak (krysmanta)                                |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <bitset>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Detector.h"
#include "KingstonFuryDRAMController.h"
#include "LogManager.h"
#include "RGBController.h"
#include "RGBController_KingstonFuryDRAM.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

using namespace std::chrono_literals;

typedef enum
{
    RESULT_PASS  = 0,
    RESULT_FAIL  = 1,
    RESULT_ERROR = 2
} TestResult;

TestResult TestForFurySlot(i2c_smbus_interface *bus, unsigned int slot_addr, bool (*modelChecker)(char))
{
    char model_code = 0;
    int res = bus->i2c_smbus_write_quick(slot_addr, I2C_SMBUS_WRITE);

    LOG_DEBUG("[%s] Probing address %02X, res=%02X", FURY_CONTROLLER_NAME, slot_addr, res);
    if(res < 0)
    {
        return RESULT_FAIL;
    }

    // Get the model code
    res = bus->i2c_smbus_read_word_data(slot_addr, FURY_REG_MODEL);
    if(res < 0)
    {
        return RESULT_ERROR;
    }
    model_code = res >> 8;
    std::this_thread::sleep_for(FURY_DELAY);
    LOG_DEBUG("[%s] Reading model code at address %02X register %02X, res=%02X",
              FURY_CONTROLLER_NAME, slot_addr, FURY_REG_MODEL, model_code);

    if(!modelChecker(model_code))
    {
        LOG_DEBUG("[%s] Unknown model code 0x%02X", FURY_CONTROLLER_NAME, model_code);
        return RESULT_FAIL;
    }
    return RESULT_PASS;
}

bool TestDDR4Models(char code)
{
    return (code == FURY_MODEL_BEAST_WHITE_DDR4 ||
            code == FURY_MODEL_BEAST_DDR4);
}

bool TestDDR5Models(char code)
{
    return (code == FURY_MODEL_BEAST_DDR5 ||
            code == FURY_MODEL_RENEGADE_DDR5 ||
            code == FURY_MODEL_BEAST_RGB_WHITE_DDR5);
}

// Checking Fury signature in the RGB address space
TestResult TestForFurySignature(i2c_smbus_interface *bus, unsigned int slot_addr)
{
    bool passed = true;
    char test_str[] = "FURY";
    int res;

    // Start transaction
    res = bus->i2c_smbus_write_byte_data(slot_addr, FURY_REG_APPLY, FURY_BEGIN_TRNSFER);
    if(res < 0)
    {
        return RESULT_ERROR;
    }

    std::this_thread::sleep_for(FURY_DELAY);
    LOG_DEBUG("[%s] %02X beginning transaction; res=%02X",
              FURY_CONTROLLER_NAME, slot_addr, res);

    // Read and check the signature
    for(int i = 1; i <= 4; i++)
    {
        for(int retry = 3; retry > 0; retry--)
        {
            res = bus->i2c_smbus_read_word_data(slot_addr, i);
            std::this_thread::sleep_for(FURY_DELAY);
            if(res >= 0)
            {
                break;
            }
        }
        if(res < 0)
        {
            return RESULT_ERROR;
        }

        char shifted = (res >> 8) & 0xff;
        LOG_DEBUG("[%s] Testing address %02X register %02X, res=%02X",
                  FURY_CONTROLLER_NAME, slot_addr, i, shifted);
        if(shifted != test_str[i-1])
        {
            passed = false;
            break;
        }
    }

    // Close transaction
    res = bus->i2c_smbus_write_byte_data(slot_addr, FURY_REG_APPLY, FURY_END_TRNSFER);
    if(res < 0)
    {
        return RESULT_ERROR;
    }
    std::this_thread::sleep_for(FURY_DELAY);
    LOG_DEBUG("[%s] %02X ending transaction; res=%02X",
              FURY_CONTROLLER_NAME, slot_addr, res);

    if(!passed)
    {
        return RESULT_FAIL;
    }
    return RESULT_PASS;
}

/******************************************************************************************\
*                                                                                          *
*   DetectKingstonFuryDRAMControllers                                                      *
*                                                                                          *
*       Detect Kingston Fury DDR4/5 DRAM controllers on the enumerated I2C busses.           *
*                                                                                          *
\******************************************************************************************/

void DetectKingstonFuryDRAMControllers(std::vector<i2c_smbus_interface*> &busses)
{
    enum { FURY_UNKNOWN, FURY_DDR4, FURY_DDR5 } fury_type = FURY_UNKNOWN;

    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            std::vector<int> slots;
            for(int slot_index = 0; slot_index < FURY_MAX_SLOTS; slot_index++)
            {
                int retries = 0;
                TestResult result = RESULT_ERROR;
                while(retries < 3 && result == RESULT_ERROR)
                {
                    // Check for DDR4 module (no point, if we already found DDR5 module)
                    if(fury_type != FURY_DDR5)
                    {
                        result = TestForFurySlot(busses[bus],
                                     FURY_BASE_ADDR_DDR4 + slot_index, TestDDR4Models);
                        if(result == RESULT_PASS)
                        {
                            result = TestForFurySignature(busses[bus],
                                         FURY_BASE_ADDR_DDR4 + slot_index);
                        }
                        if(result == RESULT_PASS)
                        {
                            fury_type = FURY_DDR4;
                            break;
                        }
                    }
                    // Check for DDR5 module (no point, if we already found DDR4 module)
                    if(fury_type != FURY_DDR4 && result != RESULT_PASS)
                    {
                        result = TestForFurySlot(busses[bus],
                                     FURY_BASE_ADDR_DDR5 + slot_index, TestDDR5Models);
                        if(result == RESULT_PASS)
                        {
                            result = TestForFurySignature(busses[bus],
                                         FURY_BASE_ADDR_DDR5 + slot_index);
                        }
                        if(result == RESULT_PASS)
                        {
                            fury_type = FURY_DDR5;
                            break;
                        }
                    }
                    if(result == RESULT_ERROR)
                    {
                        // I/O error - wait for a bit and retry
                        retries++;
                        std::this_thread::sleep_for(100ms);
                    }
                }

                // RAM module successfully detected in the slot 'slot_index'
                if(result == RESULT_PASS)
                {
                    LOG_DEBUG("[%s] detected at slot index %d",
                              FURY_CONTROLLER_NAME, slot_index);
                    slots.push_back(slot_index);
                }
            }

            if(!slots.empty() && fury_type != FURY_UNKNOWN)
            {
                unsigned char base_addr =
                    (fury_type == FURY_DDR4) ? FURY_BASE_ADDR_DDR4 : FURY_BASE_ADDR_DDR5;
                KingstonFuryDRAMController* controller =
                    new KingstonFuryDRAMController(busses[bus], base_addr, slots);
                RGBController_KingstonFuryDRAM* rgb_controller =
                    new RGBController_KingstonFuryDRAM(controller);
                rgb_controller->name =
                    (fury_type == FURY_DDR4) ? "Kingston Fury DDR4 RGB" : "Kingston Fury DDR5 RGB";
                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }
}

REGISTER_I2C_DETECTOR("Kingston Fury DDR4/5 DRAM", DetectKingstonFuryDRAMControllers);

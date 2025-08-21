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

#include <vector>
#include "Detector.h"
#include "KingstonFuryDRAMController.h"
#include "LogManager.h"
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

bool TestDDR4Models(char code)
{
    return (code == FURY_MODEL_BEAST_WHITE_DDR4 ||
            code == FURY_MODEL_BEAST_DDR4);
}

bool TestDDR5Models(char code)
{
    return (code == FURY_MODEL_BEAST_DDR5 ||
            code == FURY_MODEL_BEAST2_DDR5 ||
            code == FURY_MODEL_RENEGADE_DDR5 ||
            code == FURY_MODEL_BEAST_RGB_WHITE_DDR5);
}

// Checking Fury signature in the RGB address space
TestResult TestForFurySignature(i2c_smbus_interface *bus, unsigned int slot_addr, bool (*modelChecker)(char))
{
    bool passed = true;
    char test_str[] = "FURY";
    int res;

    LOG_DEBUG("[%s] looking at 0x%02X",
              FURY_CONTROLLER_NAME, slot_addr);

    // Start transaction
    res = bus->i2c_smbus_write_byte_data(slot_addr, FURY_REG_APPLY, FURY_BEGIN_TRNSFER);
    if(res < 0)
    {
        LOG_DEBUG("[%s] DIMM not present at 0x%02X",
                  FURY_CONTROLLER_NAME, slot_addr);
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
            LOG_DEBUG("[%s] Testing address %02X register %02X, res=%04X",
                      FURY_CONTROLLER_NAME, slot_addr, i, res);
            // retry when there is an error or the returned value is 0xFFFF
            if((res >= 0) && (res < 0xFFFF))
            {
                break;
            }
        }
        if(res < 0)
        {
            return RESULT_ERROR;
        }

        char shifted = (res >> 8) & 0xFF;
        if(shifted != test_str[i-1])
        {
            passed = false;
            break;
        }
    }

    if(passed)
    {
        // Get the model code
        res = bus->i2c_smbus_read_word_data(slot_addr, FURY_REG_MODEL);
        int model_code = res >> 8;
        std::this_thread::sleep_for(FURY_DELAY);
        LOG_DEBUG("[%s] Reading model code at address %02X register %02X, res=%02X",
                  FURY_CONTROLLER_NAME, slot_addr, FURY_REG_MODEL, res);

        if(!modelChecker(model_code))
        {
            LOG_INFO("[%s] Unknown model code 0x%02X", FURY_CONTROLLER_NAME, model_code);
            passed = false;
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

    return passed ? RESULT_PASS : RESULT_FAIL;
}

void DetectKingstonFuryDRAMControllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots,
                                       uint8_t fury_base_addr, bool (*modelChecker)(char), std::vector<int> &fury_slots)
{
    // Are these the Kingston Fury DRAMs
    for(SPDWrapper *slot : slots)
    {
        TestResult result;
        int retries = 0;

        result = RESULT_ERROR;
        while(retries < 3 && result == RESULT_ERROR)
        {
            result = TestForFurySignature(bus, fury_base_addr + slot->index(), modelChecker);
            if(result == RESULT_PASS)
            {
                break;
            }
            if(result == RESULT_ERROR)
            {
                // I/O error - wait for a bit and retry
                retries++;
                std::this_thread::sleep_for(FURY_DELAY);
            }
        }

        // RAM module successfully detected in the slot 'slot_index'
        if(result == RESULT_PASS)
        {
            LOG_DEBUG("[%s] detected at slot index %d",
                      FURY_CONTROLLER_NAME, slot->index());
            fury_slots.push_back(slot->index());
        }
    }
}

/******************************************************************************************\
*                                                                                          *
*   DetectKingstonFuryDRAMControllers                                                      *
*                                                                                          *
*       Detect Kingston Fury DDR4/5 DRAM controllers on the enumerated I2C busses.           *
*                                                                                          *
\******************************************************************************************/

void DetectKingstonFuryDDR4Controllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &name)
{
    std::vector<int> fury_slots;

    DetectKingstonFuryDRAMControllers(bus, slots, FURY_BASE_ADDR_DDR4, TestDDR4Models, fury_slots);

    if(!fury_slots.empty())
    {
        KingstonFuryDRAMController*     controller     = new KingstonFuryDRAMController(bus, FURY_BASE_ADDR_DDR4, fury_slots, name);
        RGBController_KingstonFuryDRAM* rgb_controller = new RGBController_KingstonFuryDRAM(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

void DetectKingstonFuryDDR5Controllers(i2c_smbus_interface* bus, std::vector<SPDWrapper*> &slots, const std::string &name)
{
    std::vector<int> fury_slots;

    DetectKingstonFuryDRAMControllers(bus, slots, FURY_BASE_ADDR_DDR5, TestDDR5Models, fury_slots);

    if(!fury_slots.empty())
    {
        KingstonFuryDRAMController*     controller     = new KingstonFuryDRAMController(bus, FURY_BASE_ADDR_DDR5, fury_slots, name);
        RGBController_KingstonFuryDRAM* rgb_controller = new RGBController_KingstonFuryDRAM(controller);

        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
}

REGISTER_I2C_DIMM_DETECTOR("Kingston Fury DDR4 DRAM",   DetectKingstonFuryDDR4Controllers,  JEDEC_KINGSTON,     SPD_DDR4_SDRAM);
REGISTER_I2C_DIMM_DETECTOR("Kingston Fury DDR4 DRAM",   DetectKingstonFuryDDR4Controllers,  JEDEC_KINGSTON_2,   SPD_DDR4_SDRAM);
REGISTER_I2C_DIMM_DETECTOR("Kingston Fury DDR5 DRAM",   DetectKingstonFuryDDR5Controllers,  JEDEC_KINGSTON,     SPD_DDR5_SDRAM);
REGISTER_I2C_DIMM_DETECTOR("Kingston Fury DDR5 DRAM",   DetectKingstonFuryDDR5Controllers,  JEDEC_KINGSTON_2,   SPD_DDR5_SDRAM);
REGISTER_I2C_DIMM_DETECTOR("Kingston Fury DDR5 DRAM",   DetectKingstonFuryDDR5Controllers,  JEDEC_KINGSTON_3,   SPD_DDR5_SDRAM);

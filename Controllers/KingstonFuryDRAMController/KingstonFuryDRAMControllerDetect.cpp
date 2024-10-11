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

typedef enum
{
    SPD_RESERVED      =  0,
    SPD_FPM_DRAM      =  1,
    SPD_EDO           =  2,
    SPD_NIBBLE        =  3,
    SPD_SDR_SDRAM     =  4,
    SPD_MUX_ROM       =  5,
    SPD_DDR_SGRAM     =  6,
    SPD_DDR_SDRAM     =  7,
    SPD_DDR2_SDRAM    =  8,
    SPD_FB_DIMM       =  9,
    SPD_FB_PROBE      = 10,
    SPD_DDR3_SDRAM    = 11,
    SPD_DDR4_SDRAM    = 12,
    SPD_RESERVED2     = 13,
    SPD_DDR4E_SDRAM   = 14,
    SPD_LPDDR3_SDRAM  = 15,
    SPD_LPDDR4_SDRAM  = 16,
    SPD_LPDDR4X_SDRAM = 17,
    SPD_DDR5_SDRAM    = 18,
    SPD_LPDDR5_SDRAM  = 19
} SPDMemoryType;

// DDR5 SPD hub detection
TestResult TestForSPDHub(i2c_smbus_interface *bus, int spd_address, int &mem_type)
{
    int ddr5Magic = bus->i2c_smbus_read_byte_data(spd_address, 0x00);
    int ddr5Sensor = bus->i2c_smbus_read_byte_data(spd_address, 0x01);
    std::this_thread::sleep_for(1ms);

    if(ddr5Magic < 0 || ddr5Sensor < 0)
    {
        return RESULT_ERROR;
    }

    if(ddr5Magic == 0x51 && (ddr5Sensor & 0xEF) == 0x08)
    {
        // These values are invalid for any other memory type
        mem_type = SPD_DDR5_SDRAM;
        return RESULT_PASS;
    }
    return RESULT_FAIL;
}

TestResult TestSPDForKingston(i2c_smbus_interface *bus, SPDMemoryType &fury_type, std::vector<int> &slots)
{
    int mem_type = -1;

    for(int slot_index = 0; slot_index < FURY_MAX_SLOTS; slot_index++)
    {
        int spd_address = 0x50 + slot_index;

        TestResult result = TestForSPDHub(bus, spd_address, mem_type);
        if(result == RESULT_ERROR)
        {
            LOG_TRACE("[%s] SPD Hub check [0x%02X] failed. No device.",
                      FURY_CONTROLLER_NAME, spd_address);
            continue;
        }

        if(mem_type != SPD_DDR4_SDRAM && mem_type != SPD_DDR5_SDRAM)
        {
            // Get memory type from SPD for DDR4 or older
            bus->i2c_smbus_write_byte_data(0x36, 0x00, 0xFF);
            std::this_thread::sleep_for(1ms);
            mem_type = bus->i2c_smbus_read_byte_data(spd_address, 0x02);
        }

        if(mem_type != SPD_DDR4_SDRAM && mem_type != SPD_DDR5_SDRAM)
        {
            LOG_TRACE("[%s] SPD check [0x%02X] - wrong memory type => %02X",
                      FURY_CONTROLLER_NAME, spd_address, mem_type);
            continue;
        }
        fury_type = static_cast<SPDMemoryType>(mem_type);

        LOG_TRACE("[%s] SPD check [0x%02X]: memory type => %02X, ",
                  FURY_CONTROLLER_NAME, spd_address, mem_type);
        // Switch SPD page
        if(mem_type == SPD_DDR4_SDRAM)
        {
            bus->i2c_smbus_write_byte_data(0x37, 0x00, 0xFF);
        }
        else
        {
            bus->i2c_smbus_write_byte_data(spd_address, 0x0B, 4);
        }
        std::this_thread::sleep_for(1ms);

        // Get ManufacturerID
        int vendorHi, vendorLo;
        if(mem_type == SPD_DDR4_SDRAM)
        {
            vendorHi = bus->i2c_smbus_read_byte_data(spd_address, 0x40);
            vendorLo = bus->i2c_smbus_read_byte_data(spd_address, 0x41);
        }
        else
        {
            vendorHi = bus->i2c_smbus_read_byte_data(spd_address, 0x80);
            vendorLo = bus->i2c_smbus_read_byte_data(spd_address, 0x81);
        }
        std::this_thread::sleep_for(1ms);

        LOG_DEBUG("[%s] SPD check [0x%02X]: vendorHi => %02X, vendorLo => %02X, ",
                  FURY_CONTROLLER_NAME, spd_address, vendorHi, vendorLo);

        // Switch SPD page back to 0
        if(mem_type == SPD_DDR4_SDRAM)
        {
            bus->i2c_smbus_write_byte_data(0x36, 0x00, 0xFF);
        }
        else
        {
            bus->i2c_smbus_write_byte_data(spd_address, 0x0B, 0);
        }
        std::this_thread::sleep_for(1ms);

        if(vendorHi == 0x01 && vendorLo == 0x98)
        {
            slots.push_back(slot_index);
        }
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
TestResult TestForFurySignature(i2c_smbus_interface *bus, unsigned int slot_addr, bool (*modelChecker)(char))
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

        char shifted = (res >> 8) & 0xFF;
        LOG_DEBUG("[%s] Testing address %02X register %02X, res=%02X",
                  FURY_CONTROLLER_NAME, slot_addr, i, shifted);
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

/******************************************************************************************\
*                                                                                          *
*   DetectKingstonFuryDRAMControllers                                                      *
*                                                                                          *
*       Detect Kingston Fury DDR4/5 DRAM controllers on the enumerated I2C busses.           *
*                                                                                          *
\******************************************************************************************/

void DetectKingstonFuryDRAMControllers(std::vector<i2c_smbus_interface*> &busses)
{
    SPDMemoryType fury_type = SPD_RESERVED;

    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_DRAM_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            TestResult result;
            std::vector<int> occupied_slots, fury_slots;
            int fury_base_addr;
            bool (*modelChecker)(char);

            // Do we have Kingston DRAMs?
            result = TestSPDForKingston(busses[bus], fury_type, occupied_slots);
            if(result != RESULT_PASS || (fury_type != SPD_DDR4_SDRAM && fury_type != SPD_DDR5_SDRAM))
            {
                continue;
            }

            if(fury_type == SPD_DDR4_SDRAM)
            {
                fury_base_addr = FURY_BASE_ADDR_DDR4;
                modelChecker = TestDDR4Models;
            }
            else
            {
                fury_base_addr = FURY_BASE_ADDR_DDR5;
                modelChecker = TestDDR5Models;
            }

            // Are these the Kingston Fury DRAMs
            for(int slot_index : occupied_slots)
            {
                int retries = 0;
                result = RESULT_ERROR;

                while(retries < 3 && result == RESULT_ERROR)
                {
                    result = TestForFurySignature(busses[bus],
                                 fury_base_addr + slot_index, modelChecker);
                    if(result == RESULT_PASS)
                    {
                        break;
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
                    fury_slots.push_back(slot_index);
                }
            }

            if(!fury_slots.empty())
            {
                unsigned char base_addr =
                    (fury_type == SPD_DDR4_SDRAM) ? FURY_BASE_ADDR_DDR4 : FURY_BASE_ADDR_DDR5;
                KingstonFuryDRAMController* controller =
                    new KingstonFuryDRAMController(busses[bus], base_addr, fury_slots);
                RGBController_KingstonFuryDRAM* rgb_controller =
                    new RGBController_KingstonFuryDRAM(controller);
                rgb_controller->name =
                    (fury_type == SPD_DDR4_SDRAM) ? "Kingston Fury DDR4 RGB" : "Kingston Fury DDR5 RGB";
                ResourceManager::get()->RegisterRGBController(rgb_controller);
            }
        }
    }
}

REGISTER_I2C_DETECTOR("Kingston Fury DDR4/5 DRAM", DetectKingstonFuryDRAMControllers);

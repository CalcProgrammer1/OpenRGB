/*---------------------------------------------------------*\
| ASRockSMBusControllerDetect.cpp                           |
|                                                           |
|   Detector for SMBus ASRock ASR RGB and Polychrome        |
|   motherboards                                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Dec 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include "Detector.h"
#include "ASRockASRRGBSMBusController.h"
#include "ASRockPolychromeV1SMBusController.h"
#include "ASRockPolychromeV2SMBusController.h"
#include "LogManager.h"
#include "RGBController_ASRockASRRGBSMBus.h"
#include "RGBController_ASRockPolychromeV1SMBus.h"
#include "RGBController_ASRockPolychromeV2SMBus.h"
#include "i2c_smbus.h"
#include "pci_ids.h"

/*******************************************************************************************\
*                                                                                           *
*   TestForPolychromeSMBusController                                                        *
*                                                                                           *
*       Tests the given address to see if an ASRock RGB controller exists there.            *
*       First does a quick write to test for a response                                     *
*                                                                                           *
\*******************************************************************************************/

#define ASROCK_DETECTOR_NAME        "ASRock SMBus Detectector"
#define VENDOR_NAME                 "ASRock"
#define SMBUS_ADDRESS               0x6A

enum
{
    ASROCK_TYPE_UNKNOWN                     = 0x00,     /* Unknown Type or Not ASRock Device    */
    ASROCK_TYPE_ASRLED                      = 0x01,     /* ASRock Firmware 1.x - ASR LED        */
    ASROCK_TYPE_POLYCHROME_V1               = 0x02,     /* ASRock Firmware 2.x - Polychrome V1  */
    ASROCK_TYPE_POLYCHROME_V2               = 0x03,     /* ASRock Firmware 3.x - Polychrome V2  */
    ASROCK_REG_FIRMWARE_VER                 = 0x00,     /* Firmware version Major.Minor         */
};

union u16_to_u8
{
    uint16_t   u16;
    struct
    {
        uint8_t lsb;
        uint8_t msb;
    };
};

bool TestForPolychromeSMBusController(i2c_smbus_interface* bus, uint8_t address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);

    if (res >= 0)
    {
        pass = true;
    }

    return(pass);

}   /* TestForPolychromeController() */

uint16_t GetFirmwareVersion(i2c_smbus_interface* bus, uint8_t address)
{
    // The firmware register holds two bytes, so the first read should return 2
    // If not, report invalid firmware revision
    LOG_DEBUG("[%s] Reading back device firmware version", ASROCK_DETECTOR_NAME);
    u16_to_u8 asrock_version_u16;

    // Version response array needs to be 32 bytes to prevent non ASRock board from stack smashing
    uint8_t asrock_version[I2C_SMBUS_BLOCK_MAX] = { 0x00, 0x00 };

    if (bus->i2c_smbus_read_block_data(address, ASROCK_REG_FIRMWARE_VER, asrock_version) == 0x02)
    {
        asrock_version_u16.msb = asrock_version[0];
        asrock_version_u16.lsb = asrock_version[1];

        LOG_DEBUG("[%s] Device firmware version: v%02d.%02d", ASROCK_DETECTOR_NAME, asrock_version_u16.msb, asrock_version_u16.lsb);
        return(asrock_version_u16.u16);
    }
    else
    {
        LOG_WARNING("[%s] Firmware readback failed; Returning Unknown Version", ASROCK_DETECTOR_NAME);
        return(ASROCK_TYPE_UNKNOWN);
    }
}

/******************************************************************************************\
*                                                                                          *
*   DetectPolychromeControllers                                                            *
*                                                                                          *
*       Detect ASRock Polychrome RGB SMBus controllers on the enumerated I2C busses at     *
*       address 0x6A.                                                                      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Polychrome device is connected      *
*           dev - I2C address of Polychrome device                                         *
*                                                                                          *
\******************************************************************************************/

void DetectASRockSMBusControllers(std::vector<i2c_smbus_interface*>& busses)
{
    for(unsigned int bus = 0; bus < busses.size(); bus++)
    {
        IF_MOBO_SMBUS(busses[bus]->pci_vendor, busses[bus]->pci_device)
        {
            if(busses[bus]->pci_subsystem_vendor == ASROCK_SUB_VEN)
            {
                LOG_DEBUG(SMBUS_CHECK_DEVICE_MESSAGE_EN, ASROCK_DETECTOR_NAME, bus, VENDOR_NAME, SMBUS_ADDRESS);
                // Check for Polychrome controller at 0x6A
                if(TestForPolychromeSMBusController(busses[bus], SMBUS_ADDRESS))
                {
                    LOG_DEBUG("[%s] Detected a device at address 0x%02X, testing for a known controller",  ASROCK_DETECTOR_NAME, SMBUS_ADDRESS);

                    u16_to_u8 version;
                    version.u16 = GetFirmwareVersion(busses[bus], SMBUS_ADDRESS);

                    switch (version.msb)
                    {
                        case ASROCK_TYPE_ASRLED:
                            {
                                LOG_DEBUG("[%s] Found a ASR RGB LED Controller", ASROCK_DETECTOR_NAME);
                                ASRockASRRGBSMBusController* controller = new ASRockASRRGBSMBusController(busses[bus], SMBUS_ADDRESS);
                                controller-> fw_version = version.u16;
                                RGBController_ASRockASRRGBSMBus* rgb_controller = new RGBController_ASRockASRRGBSMBus(controller);
                                ResourceManager::get()->RegisterRGBController(rgb_controller);
                            }
                            break;

                            case ASROCK_TYPE_POLYCHROME_V1:
                            {
                                LOG_DEBUG("[%s] Found a Polychrome v1 Controller", ASROCK_DETECTOR_NAME);
                                ASRockPolychromeV1SMBusController* controller = new ASRockPolychromeV1SMBusController(busses[bus], SMBUS_ADDRESS);
                                controller-> fw_version = version.u16;
                                RGBController_ASRockPolychromeV1SMBus* rgb_controller = new RGBController_ASRockPolychromeV1SMBus(controller);
                                ResourceManager::get()->RegisterRGBController(rgb_controller);
                            }
                            break;

                            case ASROCK_TYPE_POLYCHROME_V2:
                            {
                                LOG_DEBUG("[%s] Found a Polychrome v2 Controller", ASROCK_DETECTOR_NAME);
                                ASRockPolychromeV2SMBusController* controller = new ASRockPolychromeV2SMBusController(busses[bus], SMBUS_ADDRESS);
                                controller-> fw_version = version.u16;
                                RGBController_ASRockPolychromeV2SMBus* rgb_controller = new RGBController_ASRockPolychromeV2SMBus(controller);
                                ResourceManager::get()->RegisterRGBController(rgb_controller);
                            }
                            break;

                            default:
                                LOG_DEBUG("[%s] Not a Polychrome device or unknown type", ASROCK_DETECTOR_NAME);
                                break;
                    }
                }
                else
                {
                    LOG_DEBUG("[%s] Bus %02d has no response at 0x%02X", ASROCK_DETECTOR_NAME, bus, SMBUS_ADDRESS);
                }
            }
            else
            {
                LOG_DEBUG(SMBUS_CHECK_DEVICE_FAILURE_EN, ASROCK_DETECTOR_NAME, bus, VENDOR_NAME);
            }
        }
    }

}   /* DetectSMBusPolychromeControllers() */

REGISTER_I2C_DETECTOR("ASRock Motherboard SMBus Controllers", DetectASRockSMBusControllers);

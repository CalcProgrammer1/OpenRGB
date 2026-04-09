/*---------------------------------------------------------*\
| ZotacBlackwellGPUController.cpp                           |
|                                                           |
|   Driver for ZOTAC Blackwell (RTX 50 series) GPU          |
|                                                           |
|   Eder Sánchez                                27 Mar 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <unistd.h>
#include "ZotacBlackwellGPUController.h"
#include "LogManager.h"

ZotacBlackwellGPUController::ZotacBlackwellGPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;

    ReadVersion();
}

ZotacBlackwellGPUController::~ZotacBlackwellGPUController()
{
}

std::string ZotacBlackwellGPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string ZotacBlackwellGPUController::GetName()
{
    return(name);
}

std::string ZotacBlackwellGPUController::GetVersion()
{
    return(version);
}

void ZotacBlackwellGPUController::ReadVersion()
{
    /*---------------------------------------------------------*\
    | Read version via raw I2C block read.  The first bytes     |
    | returned contain the ASCII version string (e.g.           |
    | "N762A-2008e").  If the raw read fails or returns empty,  |
    | fall back to reading register 0x2F as an identifier.      |
    \*---------------------------------------------------------*/
    u8  rdata_pkt[I2C_SMBUS_BLOCK_MAX] = { 0x00 };
    int rdata_len                      = sizeof(rdata_pkt);

    if(bus->i2c_read_block(dev, &rdata_len, rdata_pkt) >= 0 && rdata_pkt[0] != 0x00)
    {
        version = std::string((char*)rdata_pkt);
    }
    else
    {
        version = "Unknown";
    }

    LOG_INFO("[%s] Firmware version: %s", name.c_str(), version.c_str());
}

void ZotacBlackwellGPUController::SetMode
(
    unsigned int    zone,
    unsigned int    mode_val,
    RGBColor        color1,
    RGBColor        color2,
    unsigned int    brightness,
    unsigned int    speed,
    unsigned int    direction
)
{
    /*---------------------------------------------------------*\
    | Write all 16 registers 0x20 - 0x2F via individual         |
    | i2c_smbus_write_byte_data calls with 3ms delay between    |
    | each transaction.  Partial writes are not supported.      |
    \*---------------------------------------------------------*/

    u8 regs[16];
    regs[0x00]  = 0x00;                                 /* 0x20 - Fixed = 0x00              */
    regs[0x01]  = (u8)zone;                             /* 0x21 - Zone index                */
    regs[0x02]  = (u8)mode_val;                         /* 0x22 - Mode                      */
    regs[0x03]  = (u8)RGBGetRValue(color1);             /* 0x23 - Red 1                     */
    regs[0x04]  = (u8)RGBGetGValue(color1);             /* 0x24 - Green 1                   */
    regs[0x05]  = (u8)RGBGetBValue(color1);             /* 0x25 - Blue 1                    */
    regs[0x06]  = (u8)RGBGetRValue(color2);             /* 0x26 - Red 2                     */
    regs[0x07]  = (u8)RGBGetGValue(color2);             /* 0x27 - Green 2                   */
    regs[0x08]  = (u8)RGBGetBValue(color2);             /* 0x28 - Blue 2                    */
    regs[0x09]  = (u8)brightness;                       /* 0x29 - Brightness (0-100)        */
    regs[0x0A]  = (u8)speed;                            /* 0x2A - Speed (0-100)             */
    regs[0x0B]  = (u8)direction;                        /* 0x2B - Direction                 */
    regs[0x0C]  = 0x00;                                 /* 0x2C - Reserved                  */
    regs[0x0D]  = 0x00;                                 /* 0x2D - Reserved                  */
    regs[0x0E]  = 0x00;                                 /* 0x2E - Reserved                  */
    regs[0x0F]  = 0x00;                                 /* 0x2F - Reserved                  */

    for(int i = 0; i < 16; i++)
    {
        bus->i2c_smbus_write_byte_data(dev, (u8)(ZOTAC_BLACKWELL_GPU_REG_FIXED + i), regs[i]);
        usleep(ZOTAC_BLACKWELL_GPU_DELAY_US);
    }
}

void ZotacBlackwellGPUController::Commit()
{
    /*---------------------------------------------------------*\
    | Write commit register to apply staged changes.            |
    | A longer delay is needed after commit to allow the        |
    | firmware to process the change — without this, some       |
    | effects don't visually update until a parameter changes.  |
    \*---------------------------------------------------------*/
    bus->i2c_smbus_write_byte_data(dev, ZOTAC_BLACKWELL_GPU_REG_COMMIT, 0x01);
    usleep(ZOTAC_BLACKWELL_GPU_COMMIT_DELAY_US);
}

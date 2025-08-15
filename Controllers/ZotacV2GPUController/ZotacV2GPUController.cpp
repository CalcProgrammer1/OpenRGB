/*---------------------------------------------------------*\
| ZotacV2GPUController.cpp                                  |
|                                                           |
|   Driver for Zotac V2 GPU                                 |
|                                                           |
|   Krzysztof Haładyn (krzys_h)                 16 Mar 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "ZotacV2GPUController.h"
#include "LogManager.h"

ZotacV2GPUController::ZotacV2GPUController(i2c_smbus_interface* bus, u8 dev, std::string dev_name)
{
    this->bus   = bus;
    this->dev   = dev;
    this->name  = dev_name;

    if(dev)
    {
        ReadVersion();
    }
}

ZotacV2GPUController::~ZotacV2GPUController()
{
}

std::string ZotacV2GPUController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return ("I2C: " + return_string);
}

std::string ZotacV2GPUController::GetName()
{
    return(name);
}

std::string ZotacV2GPUController::GetVersion()
{
    return(version);
}

bool ZotacV2GPUController::ReadVersion()
{
    u8 data_pkt[] = { ZOTAC_V2_GPU_REG_RGB, 0xF1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    if(bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt) < 0)
    {
        return false;
    }

    u8 rdata_pkt[I2C_SMBUS_BLOCK_MAX] = { 0x00 };
    int rdata_len = sizeof(rdata_pkt);
    if(bus->i2c_read_block(dev, &rdata_len, rdata_pkt) < 0)
    {
        return false;
    }

    version = std::string((char*)rdata_pkt);

    return true;
}

bool ZotacV2GPUController::TurnOnOff(bool on)
{
    return SendCommand(on, false, 0, 0, 0, ZotacV2GPUZone());
}

bool ZotacV2GPUController::ResetToDefaults()
{
    return SendCommand(true, true, 0, 0, 0, ZotacV2GPUZone());
}

bool ZotacV2GPUController::SetMode(int zone, int idleActive, int syncMode, ZotacV2GPUZone zoneConfig)
{
    // NOTE: This only works if the device is in the ON state. Otherwise, the SetMode command will behave
    // like TurnOnOff(true), and the change will be ignored.

    // NOTE: syncMode is per idleActive, NOT per (zone, idleActive) pair like zoneConfig is - as in,
    // you can have ACTIVE in INDIVIDUAL mode and IDLE in SYNCHRONIZED mode, but you can't have
    // different syncModes in different zones. The last written value is always applied, so make
    // sure you don't change it accidentally between writes.
    // TODO: Verify what I said above - it doesn't match the GUI, but it seems to match the NvAPISpy traces
    // From the GUI it seems like syncMode should be global, period.

    return SendCommand(true, false, zone, idleActive, syncMode, zoneConfig);
}

bool ZotacV2GPUController::GetMode(int zone, int idleActive, int& syncMode, ZotacV2GPUZone& zoneConfig, bool& on)
{
    u8 data_pkt[] =
    {
        ZOTAC_V2_GPU_REG_RGB,
        0xF0,
        0x00,
        0x00,
        0x00,
        (u8)idleActive,
        (u8)zone,
        0x00,
    };

    if(bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt) < 0)
    {
        return false;
    }


    u8 rdata_pkt[I2C_SMBUS_BLOCK_MAX] = { 0x00 };
    int rdata_len = sizeof(rdata_pkt);
    if(bus->i2c_read_block(dev, &rdata_len, rdata_pkt) < 0)
    {
        return false;
    }

    bool readReset;
    int readZone;
    int readIdleActive;
    if(!ParseCommand(on, readReset, readZone, readIdleActive, syncMode, zoneConfig))
    {
        return false;
    }

    if(readReset != 0)
    {
        LOG_WARNING("Reset byte was not 0?!");
    }

    if(readZone != zone || readIdleActive != idleActive)
    {
        LOG_WARNING("Got unexpected data - expected to recieve data for (%d, %d) but got for (%d, %d)", zone, idleActive, readZone, readIdleActive);
        return false;
    }

    return true;
}

bool ZotacV2GPUController::SendCommand(bool on, bool reset, int zone, int idleActive, int syncMode, ZotacV2GPUZone zoneConfig)
{
    u8 data_pkt[] =
    {
        ZOTAC_V2_GPU_REG_RGB,
        on ? (u8)0x01 : (u8)0x00,
        reset ? (u8)0x01 : (u8)0x00,
        0x00,
        0x00,
        (u8)idleActive,
        (u8)zone,
        (u8)zoneConfig.mode,
        (u8)RGBGetRValue(zoneConfig.color1),
        (u8)RGBGetGValue(zoneConfig.color1),
        (u8)RGBGetBValue(zoneConfig.color1),
        (u8)zoneConfig.speed,
        (u8)zoneConfig.brightness,
        (u8)zoneConfig.direction,
        0x00,
        (u8)syncMode,
        (u8)zoneConfig.circuit,
        (u8)RGBGetRValue(zoneConfig.color2),
        (u8)RGBGetGValue(zoneConfig.color2),
        (u8)RGBGetBValue(zoneConfig.color2),
        (u8)zoneConfig.colorPreset,
    };

    if(bus->i2c_write_block(dev, sizeof(data_pkt), data_pkt) < 0)
    {
        return false;
    }
    return true;
}

bool ZotacV2GPUController::ParseCommand(bool& on, bool& reset, int& zone, int& idleActive, int& syncMode, ZotacV2GPUZone& zoneConfig)
{
    u8 rdata_pkt[I2C_SMBUS_BLOCK_MAX] = { 0x00 };
    int rdata_len = sizeof(rdata_pkt);

    if(bus->i2c_read_block(dev, &rdata_len, rdata_pkt) < 0)
    {
        return false;
    }

    on                     = rdata_pkt[0] != 0x00;
    reset                  = rdata_pkt[1] != 0x00;
    idleActive             = rdata_pkt[4];
    zone                   = rdata_pkt[5];
    zoneConfig.mode        = rdata_pkt[6];
    zoneConfig.color1      = ToRGBColor(rdata_pkt[7], rdata_pkt[8], rdata_pkt[9]);
    zoneConfig.speed       = rdata_pkt[10];
    zoneConfig.brightness  = rdata_pkt[11];
    zoneConfig.direction   = rdata_pkt[12];
    syncMode               = rdata_pkt[14];
    zoneConfig.circuit     = rdata_pkt[15];
    zoneConfig.color2      = ToRGBColor(rdata_pkt[16], rdata_pkt[17], rdata_pkt[18]);
    zoneConfig.colorPreset = rdata_pkt[19];
    return true;
}

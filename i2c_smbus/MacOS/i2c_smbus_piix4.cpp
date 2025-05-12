/*---------------------------------------------------------*\
| i2c_smbus_piix4.cpp                                       |
|                                                           |
|   PIIX4 SMBUS driver for MacOS                            |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Aug 2018 |
|   Portions based on Linux source code                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <unistd.h>
#include "macUSPCIOAccess.h"

#include "Detector.h"
#include "i2c_smbus_piix4.h"
#include "LogManager.h"
#include "pci_ids.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

i2c_smbus_piix4::i2c_smbus_piix4()
{
    json drivers_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Drivers");

    bool amd_smbus_reduce_cpu = false;
    if(drivers_settings.contains("amd_smbus_reduce_cpu"))
    {
        amd_smbus_reduce_cpu = drivers_settings["amd_smbus_reduce_cpu"].get<bool>();
    }

    delay_timer = amd_smbus_reduce_cpu;
}

i2c_smbus_piix4::~i2c_smbus_piix4()
{
}

//Logic adapted from piix4_transaction() in i2c-piix4.c
int i2c_smbus_piix4::piix4_transaction()
{
    int result = 0;
    int temp;
    int timeout = 0;

    /* Make sure the SMBus host is ready to start transmitting */
    temp = ReadIoPortByte(SMBHSTSTS);

    if (temp != 0x00)
    {
        WriteIoPortByte(SMBHSTSTS, temp);

        temp = ReadIoPortByte(SMBHSTSTS);

        if (temp != 0x00)
        {
            return -EBUSY;
        }
    }

    /* start the transaction by setting bit 6 */
    temp = ReadIoPortByte(SMBHSTCNT);
    WriteIoPortByte(SMBHSTCNT, temp | 0x040);

    /* We will always wait for a fraction of a second! (See PIIX4 docs errata) */
    temp = 0;

    if(delay_timer)
    {
        while ((++timeout < MAX_TIMEOUT) && temp <= 1)
        {
            temp = ReadIoPortByte(SMBHSTSTS);
            usleep(250);
        }
    }
    else
    {
        while ((++timeout < MAX_TIMEOUT) && temp <= 1)
        {
            temp = ReadIoPortByte(SMBHSTSTS);
        }
    }

    /* If the SMBus is still busy, we give up */
    if (timeout == MAX_TIMEOUT)
    {
        result = -ETIMEDOUT;
    }

    if (temp & 0x10)
    {
        result = -EIO;
    }

    if (temp & 0x08)
    {
        result = -EIO;
    }

    if (temp & 0x04)
    {
        result = -ENXIO;
    }

    temp = ReadIoPortByte(SMBHSTSTS);
    if (temp != 0x00)
    {
        WriteIoPortByte(SMBHSTSTS, temp);
    }

    return result;
}

//Logic adapted from piix4_access() in i2c-piix4.c
s32 i2c_smbus_piix4::piix4_access(u16 addr, char read_write, u8 command, int size, i2c_smbus_data *data)
{
	int i, len, status;

	switch (size)
	{
	case I2C_SMBUS_QUICK:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
		size = PIIX4_QUICK;
		break;
    case I2C_SMBUS_BYTE:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        if (read_write == I2C_SMBUS_WRITE)
        {
            WriteIoPortByte(SMBHSTCMD, command);
        }
        size = PIIX4_BYTE;
        break;
	case I2C_SMBUS_BYTE_DATA:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        WriteIoPortByte(SMBHSTCMD, command);
		if (read_write == I2C_SMBUS_WRITE)
		{
            WriteIoPortByte(SMBHSTDAT0, data->byte);
		}
		size = PIIX4_BYTE_DATA;
		break;
	case I2C_SMBUS_WORD_DATA:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        WriteIoPortByte(SMBHSTCMD, command);
		if (read_write == I2C_SMBUS_WRITE)
		{
            WriteIoPortByte(SMBHSTDAT0, data->word & 0xFF);
            WriteIoPortByte(SMBHSTDAT1, (data->word & 0xFF00) >> 8);
		}
		size = PIIX4_WORD_DATA;
		break;
	case I2C_SMBUS_BLOCK_DATA:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        WriteIoPortByte(SMBHSTCMD, command);
		if (read_write == I2C_SMBUS_WRITE)
		{
            len = data->block[0];
			if (len == 0 || len > I2C_SMBUS_BLOCK_MAX)
			{
				return -EINVAL;
			}
            WriteIoPortByte(SMBHSTDAT0, len);
            ReadIoPortByte(SMBHSTCNT);
			for (i = 1; i <= len; i++)
			{
                WriteIoPortByte(SMBBLKDAT, data->block[i]);
			}
		}
		size = PIIX4_BLOCK_DATA;
		break;
	default:
		return -EOPNOTSUPP;
	}

    WriteIoPortByte(SMBHSTCNT, (size & 0x1C));

    status = piix4_transaction();

    if (status)
        return status;

    if ((read_write == I2C_SMBUS_WRITE) || (size == PIIX4_QUICK))
		return 0;

	switch (size)
	{
	case PIIX4_BYTE:
	case PIIX4_BYTE_DATA:
        data->byte = (u8)ReadIoPortByte(SMBHSTDAT0);
		break;
	case PIIX4_WORD_DATA:
        data->word = ReadIoPortByte(SMBHSTDAT0) + (ReadIoPortByte(SMBHSTDAT1) << 8);
		break;
	case PIIX4_BLOCK_DATA:
        data->block[0] = (u8)ReadIoPortByte(SMBHSTDAT0);
		if (data->block[0] == 0 || data->block[0] > I2C_SMBUS_BLOCK_MAX)
		{
			return -EPROTO;
		}
        ReadIoPortByte(SMBHSTCNT);
		for (i = 1; i <= data->block[0]; i++)
		{
            data->block[i] = (u8)ReadIoPortByte(SMBBLKDAT);
		}
		break;
	}

	return 0;
}

s32 i2c_smbus_piix4::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    s32 result = piix4_access(addr, read_write, command, size, data);

    return result;
}

s32 i2c_smbus_piix4::i2c_xfer(u8 /*addr*/, char /*read_write*/, int* /*size*/, u8* /*data*/)
{
    return -1;
}

bool i2c_smbus_piix4_detect()
{
    if(!GetMacUSPCIODriverStatus())
    {
        LOG_INFO("macUSPCIO is not loaded, piix4 I2C bus detection aborted");
        return(false);
    }

    // addresses are referenced from: https://opensource.apple.com/source/IOPCIFamily/IOPCIFamily-146/IOKit/pci/IOPCIDevice.h.auto.html
    uint16_t vendor_id = ReadConfigPortWord(0x00);
    uint16_t device_id = ReadConfigPortWord(0x02);
    uint16_t subsystem_vendor_id = ReadConfigPortWord(0x2c);
    uint16_t subsystem_device_id = ReadConfigPortWord(0x2e);

    if(vendor_id != AMD_VEN || !device_id || !subsystem_vendor_id || !subsystem_device_id)
    {
        return(false);
    }

    i2c_smbus_interface * bus;

    bus                         = new i2c_smbus_piix4();
    bus->pci_vendor             = vendor_id;
    bus->pci_device             = device_id;
    bus->pci_subsystem_vendor   = subsystem_vendor_id;
    bus->pci_subsystem_device   = subsystem_device_id;
    strcpy(bus->device_name, "Advanced Micro Devices, Inc PIIX4 SMBus at 0x0B00");
    ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B00;
    ResourceManager::get()->RegisterI2CBus(bus);

    bus                         = new i2c_smbus_piix4();
    bus->pci_vendor             = vendor_id;
    bus->pci_device             = device_id;
    bus->pci_subsystem_vendor   = subsystem_vendor_id;
    bus->pci_subsystem_device   = subsystem_device_id;
    ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B20;
    strcpy(bus->device_name, "Advanced Micro Devices, Inc PIIX4 SMBus at 0x0B20");
    ResourceManager::get()->RegisterI2CBus(bus);

    return(true);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_piix4_detect);

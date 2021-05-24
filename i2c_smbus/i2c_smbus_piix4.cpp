/*-----------------------------------------*\
|  i2c_smbus_piix4.cpp                      |
|                                           |
|  PIIX4 SMBUS driver for Windows           |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/8/2018    |
|  Portions based on Linux source code      |
|  GNU GPL v2                               |
\*-----------------------------------------*/

#include "i2c_smbus_piix4.h"
#include <Windows.h>
#include "inpout32.h"
#include "LogManager.h"

//Logic adapted from piix4_transaction() in i2c-piix4.c
int i2c_smbus_piix4::piix4_transaction()
{
    int result = 0;
    int temp;
    int timeout = 0;

    /* Make sure the SMBus host is ready to start transmitting */
    temp = Inp32(SMBHSTSTS);

    if (temp != 0x00)
    {
        Out32(SMBHSTSTS, temp);

        temp = Inp32(SMBHSTSTS);

        if (temp != 0x00)
        {
            return -EBUSY;
        }
    }

    /* start the transaction by setting bit 6 */
    temp = Inp32(SMBHSTCNT);
    Out32(SMBHSTCNT, temp | 0x040);

    /* We will always wait for a fraction of a second! (See PIIX4 docs errata) */
    temp = 0;
    while ((++timeout < MAX_TIMEOUT) && temp <= 1)
    {
        temp = Inp32(SMBHSTSTS);
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

    temp = Inp32(SMBHSTSTS);
    if (temp != 0x00)
    {
        Out32(SMBHSTSTS, temp);
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
		Out32(SMBHSTADD, (addr << 1) | read_write);
		size = PIIX4_QUICK;
		break;
    case I2C_SMBUS_BYTE:
        Out32(SMBHSTADD, (addr << 1) | read_write);
        if (read_write == I2C_SMBUS_WRITE)
        {
            Out32(SMBHSTCMD, command);
        }
        size = PIIX4_BYTE;
        break;
	case I2C_SMBUS_BYTE_DATA:
		Out32(SMBHSTADD, (addr << 1) | read_write);
		Out32(SMBHSTCMD, command);
		if (read_write == I2C_SMBUS_WRITE)
		{
			Out32(SMBHSTDAT0, data->byte);
		}
		size = PIIX4_BYTE_DATA;
		break;
	case I2C_SMBUS_WORD_DATA:
		Out32(SMBHSTADD, (addr << 1) | read_write);
		Out32(SMBHSTCMD, command);
		if (read_write == I2C_SMBUS_WRITE)
		{
			Out32(SMBHSTDAT0, data->word & 0xFF);
			Out32(SMBHSTDAT1, (data->word & 0xFF00) >> 8);
		}
		size = PIIX4_WORD_DATA;
		break;
	case I2C_SMBUS_BLOCK_DATA:
		Out32(SMBHSTADD, (addr << 1) | read_write);
		Out32(SMBHSTCMD, command);
		if (read_write == I2C_SMBUS_WRITE)
		{
            len = data->block[0];
			if (len == 0 || len > I2C_SMBUS_BLOCK_MAX)
			{
				return -EINVAL;
			}
			Out32(SMBHSTDAT0, len);
			Inp32(SMBHSTCNT);
			for (i = 1; i <= len; i++)
			{
				Out32(SMBBLKDAT, data->block[i]);
			}
		}
		size = PIIX4_BLOCK_DATA;
		break;
	default:
		return -EOPNOTSUPP;
	}

	Out32(SMBHSTCNT, (size & 0x1C));

    status = piix4_transaction();

    if (status)
        return status;

	if ((read_write == I2C_SMBUS_WRITE) || (size == PIIX4_QUICK))
		return 0;

	switch (size)
	{
	case PIIX4_BYTE:
	case PIIX4_BYTE_DATA:
		data->byte = (u8)Inp32(SMBHSTDAT0);
		break;
	case PIIX4_WORD_DATA:
		data->word = Inp32(SMBHSTDAT0) + (Inp32(SMBHSTDAT1) << 8);
		break;
	case PIIX4_BLOCK_DATA:
		data->block[0] = (u8)Inp32(SMBHSTDAT0);
		if (data->block[0] == 0 || data->block[0] > I2C_SMBUS_BLOCK_MAX)
		{
			return -EPROTO;
		}
		Inp32(SMBHSTCNT);
		for (i = 1; i <= data->block[0]; i++)
		{
			data->block[i] = (u8)Inp32(SMBBLKDAT);
		}
		break;
	}

	return 0;
}

s32 i2c_smbus_piix4::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    return piix4_access(addr, read_write, command, size, data);
}

#include "Detector.h"
#include "wmi.h"

void i2c_smbus_piix4_detect()
{
    if(!IsInpOutDriverOpen())
    {
        LOG_NOTICE("inpout32 is not loaded, piix4 I2C bus detection aborted");
        return;
    }

    i2c_smbus_interface * bus;
    HRESULT hres;
    Wmi wmi;
    wmi.init();

    // Query WMI for Win32_PnPSignedDriver entries with names matching "SMBUS" or "SM BUS"
    // These devices may be browsed under Device Manager -> System Devices
    std::vector<QueryObj> q_res_PnPSignedDriver;
    hres = wmi.query("SELECT * FROM Win32_PnPSignedDriver WHERE Description LIKE '\%SMBUS\%' OR Description LIKE '\%SM BUS\%'", q_res_PnPSignedDriver);

    if (hres)
    {
        LOG_NOTICE("WMI query failed, piix4 I2C bus detection aborted");
        return;
    }

    // For each detected SMBus adapter, try enumerating it as either AMD or Intel
    for (QueryObj &i : q_res_PnPSignedDriver)
    {
        // AMD SMBus controllers do not show any I/O resources allocated in Device Manager
        // Analysis of many AMD boards has shown that AMD SMBus controllers have two adapters with fixed I/O spaces at 0x0B00 and 0x0B20
        // AMD SMBus adapters use the PIIX4 driver
        if (i["Manufacturer"].find("Advanced Micro Devices, Inc") != std::string::npos)
        {
            std::string pnp_str = i["DeviceID"];

            std::size_t ven_loc = pnp_str.find("VEN_");
            std::size_t dev_loc = pnp_str.find("DEV_");
            std::size_t sub_loc = pnp_str.find("SUBSYS_");

            std::string ven_str = pnp_str.substr(ven_loc + 4, 4);
            std::string dev_str = pnp_str.substr(dev_loc + 4, 4);
            std::string sbv_str = pnp_str.substr(sub_loc + 11, 4);
            std::string sbd_str = pnp_str.substr(sub_loc + 7, 4);

            int ven_id = (int)std::stoul(ven_str, nullptr, 16);
            int dev_id = (int)std::stoul(dev_str, nullptr, 16);
            int sbv_id = (int)std::stoul(sbv_str, nullptr, 16);
            int sbd_id = (int)std::stoul(sbd_str, nullptr, 16);

            bus                         = new i2c_smbus_piix4();
            bus->pci_vendor             = ven_id;
            bus->pci_device             = dev_id;
            bus->pci_subsystem_vendor   = sbv_id;
            bus->pci_subsystem_device   = sbd_id;
            strcpy(bus->device_name, i["Description"].c_str());
            strcat(bus->device_name, " at 0x0B00");
            ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B00;
            ResourceManager::get()->RegisterI2CBus(bus);

            bus                         = new i2c_smbus_piix4();
            bus->pci_vendor             = ven_id;
            bus->pci_device             = dev_id;
            bus->pci_subsystem_vendor   = sbv_id;
            bus->pci_subsystem_device   = sbd_id;
            ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B20;
            strcpy(bus->device_name, i["Description"].c_str());
            strcat(bus->device_name, " at 0x0B20");
            ResourceManager::get()->RegisterI2CBus(bus);
        }
    }
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_piix4_detect);

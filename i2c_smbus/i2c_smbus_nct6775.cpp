/*-----------------------------------------*\
|  i2c_smbus_nct6775.cpp                    |
|                                           |
|  Nuvoton NCT67xx SMBUS driver for Windows |
|                                           |
|  Adam Honse (CalcProgrammer1) 5/19/2019   |
\*-----------------------------------------*/

#include "i2c_smbus_nct6775.h"
#include <Windows.h>
#include "inpout32.h"
#include "LogManager.h"

using namespace std::chrono_literals;

s32 i2c_smbus_nct6775::nct6775_access(u16 addr, char read_write, u8 command, int size, i2c_smbus_data *data)
{
    int i, len, status, cnt;
    i2c_smbus_data tmp_data;
    int timeout = 0;

    tmp_data.word = 0;
    cnt = 0;
    len = 0;

    Out32(SMBHSTCTL, NCT6775_SOFT_RESET);

    switch (size)
    {
    case I2C_SMBUS_QUICK:
        Out32(SMBHSTADD, (addr << 1) | read_write);
        break;
    case I2C_SMBUS_BYTE_DATA:
        tmp_data.byte = data->byte;
    case I2C_SMBUS_BYTE:
        Out32(SMBHSTADD, (addr << 1) | read_write);
        Out32(SMBHSTIDX, command);
        if (read_write == I2C_SMBUS_WRITE)
        {
            Out32(SMBHSTDAT, tmp_data.byte);
            Out32(SMBHSTCMD, NCT6775_WRITE_BYTE);
        }
        else
        {
            Out32(SMBHSTCMD, NCT6775_READ_BYTE);
        }
        break;
    case I2C_SMBUS_WORD_DATA:
        Out32(SMBHSTADD, (addr << 1) | read_write);
        Out32(SMBHSTIDX, command);
        if (read_write == I2C_SMBUS_WRITE)
        {
			Out32(SMBHSTDAT, data->word & 0xFF);
			Out32(SMBHSTDAT, (data->word & 0xFF00) >> 8);
            Out32(SMBHSTCMD, NCT6775_WRITE_WORD);
        }
        else
        {
            Out32(SMBHSTCMD, NCT6775_READ_WORD);
        }
        break;
    case I2C_SMBUS_BLOCK_DATA:
        Out32(SMBHSTADD, (addr << 1) | read_write);
        Out32(SMBHSTIDX, command);
        if (read_write == I2C_SMBUS_WRITE)
        {
            len = data->block[0];
            if (len == 0 || len > I2C_SMBUS_BLOCK_MAX)
            {
                return -EINVAL;
            }
            Out32(SMBBLKSZ, len);

			//Load 4 bytes into FIFO
			cnt = 1;
			if (len >= 4)
			{
				for (i = cnt; i <= 4; i++)
				{
					Out32(SMBHSTDAT, data->block[i]);
				}

				len -= 4;
				cnt += 4;
			}
			else
			{
				for (i = cnt; i <= len; i++)
				{
					Out32(SMBHSTDAT, data->block[i]);
				}

				len = 0;
			}
            
            Out32(SMBHSTCMD, NCT6775_WRITE_BLOCK);
        }
        else
        {
            return -EOPNOTSUPP;
            //Out32(SMBHSTCMD, NCT6775_READ_BLOCK);
        }
        break;
    default:
        return -EOPNOTSUPP;
    }

    Out32(SMBHSTCTL, NCT6775_MANUAL_START);

	while ((size == I2C_SMBUS_BLOCK_DATA) && (len > 0))
	{
		if (read_write == I2C_SMBUS_WRITE)
		{
            timeout = 0;
            while ((Inp32(SMBHSTSTS) & NCT6775_FIFO_EMPTY) == 0)
            {
                if(timeout > NCT6775_MAX_RETRIES)
                {
                    return -ETIMEDOUT;
                }
                std::this_thread::sleep_for(1ms);;
                timeout++;
            }

			//Load more bytes into FIFO
			if (len >= 4)
			{
				for (i = cnt; i <= (cnt + 4); i++)
				{
					Out32(SMBHSTDAT, data->block[i]);
				}

				len -= 4;
				cnt += 4;
			}
			else
			{
				for (i = cnt; i <= (cnt + len); i++)
				{
					Out32(SMBHSTDAT, data->block[i]);
				}

				len = 0;
			}
		}
        else
        {
            return -ENOTSUP;
        }
	}

	//wait for manual mode to complete
    timeout = 0;
    while ((Inp32(SMBHSTSTS) & NCT6775_MANUAL_ACTIVE) != 0)
    {
        if(timeout > NCT6775_MAX_RETRIES)
        {
            return -ETIMEDOUT;
        }
        std::this_thread::sleep_for(1ms);;
        timeout++;
    }

	if ((Inp32(SMBHSTERR) & NCT6775_NO_ACK) != 0)
	{
		return -EPROTO;
	}
	else if ((read_write == I2C_SMBUS_WRITE) || (size == I2C_SMBUS_QUICK))
	{
		return 0;
	}

    switch (size)
    {
    case I2C_SMBUS_QUICK:
    case I2C_SMBUS_BYTE_DATA:
        data->byte = (u8)Inp32(SMBHSTDAT);
        break;
    case I2C_SMBUS_WORD_DATA:
        data->word = Inp32(SMBHSTDAT) + (Inp32(SMBHSTDAT) << 8);
        break;
    }

    return 0;
}

s32 i2c_smbus_nct6775::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    return nct6775_access(addr, read_write, command, size, data);
}

#include "Detector.h"
#include "super_io.h"

void i2c_smbus_nct6775_detect()
{
    if(!IsInpOutDriverOpen())
    {
        LOG_NOTICE("inpout32 is not loaded, nct6775 I2C bus detection aborted");
        return;
    }

    i2c_smbus_interface* bus;
    int sioaddr = 0x2E;
    superio_enter(sioaddr);

    int val = (superio_inb(sioaddr, SIO_REG_DEVID) << 8) | superio_inb(sioaddr, SIO_REG_DEVID + 1);

    switch (val & SIO_ID_MASK)
    {
    case SIO_NCT5577_ID:
    case SIO_NCT6102_ID:
    case SIO_NCT6793_ID:
    case SIO_NCT6796_ID:
    case SIO_NCT6798_ID:
        // Create new nct6775 bus and zero out the PCI ID information
        bus                         = new i2c_smbus_nct6775();
        bus->pci_vendor             = 0;
        bus->pci_device             = 0;
        bus->pci_subsystem_vendor   = 0;
        bus->pci_subsystem_device   = 0;

        // Set logical device register to get SMBus base address
        superio_outb(sioaddr, SIO_REG_LOGDEV, SIO_LOGDEV_SMBUS);

        // Get SMBus base address from configuration register
        int smba = (superio_inb(sioaddr, SIO_REG_SMBA) << 8) | superio_inb(sioaddr, SIO_REG_SMBA + 1);
        ((i2c_smbus_nct6775*)bus)->nct6775_smba = smba;

        // Set device name string
        switch (val & SIO_ID_MASK)
        {
        case SIO_NCT5577_ID:
            sprintf(bus->device_name, "Nuvoton NCT5577D SMBus at %X", smba);
            break;
        case SIO_NCT6102_ID:
            sprintf(bus->device_name, "Nuvoton NCT6102D/NCT6106D SMBus at %X", smba);
            break;
        case SIO_NCT6793_ID:
            sprintf(bus->device_name, "Nuvoton NCT6793D SMBus at %X", smba);
            break;
        case SIO_NCT6796_ID:
            sprintf(bus->device_name, "Nuvoton NCT6796D SMBus at %X", smba);
            break;
        case SIO_NCT6798_ID:
            sprintf(bus->device_name, "Nuvoton NCT6798D SMBus at %X", smba);
            break;
        }

        ResourceManager::get()->RegisterI2CBus(bus);
    }
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_nct6775_detect);

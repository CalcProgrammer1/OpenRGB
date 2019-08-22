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

#pragma comment(lib, "inpout32.lib")

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
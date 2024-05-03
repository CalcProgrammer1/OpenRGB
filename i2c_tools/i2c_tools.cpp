/*---------------------------------------------------------*\
| i2c_tools.cpp                                             |
|                                                           |
|   Utility functions for communicating with and reverse    |
|   engineering i2c devices                                 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "i2c_tools.h"

/******************************************************************************************\
*                                                                                          *
*   i2c_detect                                                                             *
*                                                                                          *
*       Prints a list of all detected I2C addresses on the given bus                       *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface to scan                                   *
*           mode - one of AUTO, QUICK, READ, FUNC - method of access                       *
*                                                                                          *
*       Code adapted from i2cdetect.c from i2c-tools Linux package                         *
*                                                                                          *
\******************************************************************************************/

std::string i2c_detect(i2c_smbus_interface * bus, int mode)
{
    int i, j;
    int first = 0x03, last = 0x77;
    int res;
    int slave_addr;
    char line[128];
    std::string text;

    snprintf(line, 128, "     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    text.append(line);

    for (i = 0; i < 128; i += 16)
    {
        snprintf(line, 128, "%02x: ", i);
        text.append(line);

        for (j = 0; j < 16; j++)
        {
            /* Skip unwanted addresses */
			if (i+j < first || i+j > last)
            {
                snprintf(line, 128, "   ");
                text.append(line);
				continue;
			}

            /* Set slave address */
            slave_addr = i + j;

            /* Probe this address */
            switch (mode)
            {
            case MODE_QUICK:
                res = bus->i2c_smbus_write_quick(slave_addr, I2C_SMBUS_WRITE);
                break;
            case MODE_READ:
                res = bus->i2c_smbus_read_byte(slave_addr);
                break;
            default:
                if ((i + j >= 0x30 && i + j <= 0x37)
                 || (i + j >= 0x50 && i + j <= 0x5F))
                    res = bus->i2c_smbus_read_byte(slave_addr);
                else
                    res = bus->i2c_smbus_write_quick(slave_addr, I2C_SMBUS_WRITE);
                break;
            }

            if (res < 0)
            {
                snprintf(line, 128, "-- ");
                text.append(line);
            }
            else
            {
                snprintf(line, 128, "%02x ", i + j);
                text.append(line);
            }
        }
        snprintf(line, 128, "\r\n");
        text.append(line);
    }

    return text;

}   /* i2c_detect() */

/******************************************************************************************\
*                                                                                          *
*   i2c_dump                                                                               *
*                                                                                          *
*       Prints the values at each address of a given SMBus device                          *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface to scan                                   *
*           address - SMBus device address to scan                                         *
*                                                                                          *
\******************************************************************************************/

std::string i2c_dump(i2c_smbus_interface * bus, unsigned char address)
{
    int i, j;

    int start = 0x0000;

    char line[128];
    std::string text;

    snprintf(line, 128, "       0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    text.append(line);

    for (i = 0; i < 0xFF; i += 16)
    {
        snprintf(line, 128, "%04x: ", i + start);
        text.append(line);

        for (j = 0; j < 16; j++)
        {
            snprintf(line, 128, "%02x ", (unsigned char )bus->i2c_smbus_read_byte_data(address, start + i + j));
            text.append(line);
        }

        snprintf(line, 128, "\r\n");
        text.append(line);
    }

    return text;

}   /* i2c_dump() */

/******************************************************************************************\
*                                                                                          *
*   i2c_read                                                                               *
*                                                                                          *
*       Prints <size> values read from register address regaddr of a given SMBus device    *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface to scan                                   *
*           address - SMBus device address to scan                                         *
*           regaddr - register address to read from                                        *
*           size - number of bytes to read                                                 *
*                                                                                          *
\******************************************************************************************/

std::string i2c_read(i2c_smbus_interface * bus, unsigned char address, unsigned char regaddr, unsigned char size)
{
    int i;

    bus->i2c_smbus_write_byte(address, regaddr);

    char line[128];
    std::string text;

    for(i = 0; i < size; i++)
    {
        snprintf(line, 128, "%02x ", (unsigned char)bus->i2c_smbus_read_byte(address));
        text.append(line);
    }

    return text;

}   /* i2c_read() */

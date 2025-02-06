/*---------------------------------------------------------*\
| i2c_smbus_nct6775.cpp                                     |
|                                                           |
|   Nuvoton NCT67xx SMBUS driver for Windows                |
|                                                           |
|   Adam Honse (CalcProgrammer1)                19 May 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "i2c_smbus_nct6775.h"
#include "LogManager.h"
#ifdef _WIN32
#include "OlsApi.h"
#elif _MACOSX_X86_X64
#include "macUSPCIOAccess.h"
#endif
#include "ResourceManager.h"
#include "SettingsManager.h"
#include "super_io.h"

using namespace std::chrono_literals;

i2c_smbus_nct6775::i2c_smbus_nct6775()
{
#ifdef _WIN32
    json drivers_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("Drivers");

    bool shared_smbus_access = true;
    if(drivers_settings.contains("shared_smbus_access"))
    {
        shared_smbus_access = drivers_settings["shared_smbus_access"].get<bool>();
    }
    if(shared_smbus_access)
    {
        global_smbus_access_handle = CreateMutexA(NULL, FALSE, GLOBAL_SMBUS_MUTEX_NAME);
    }
#endif
}

i2c_smbus_nct6775::~i2c_smbus_nct6775()
{
#ifdef _WIN32
    if(global_smbus_access_handle != NULL)
    {
        CloseHandle(global_smbus_access_handle);
    }
#endif
}

s32 i2c_smbus_nct6775::nct6775_access(u16 addr, char read_write, u8 command, int size, i2c_smbus_data *data)
{
    int i, len, cnt;
    i2c_smbus_data tmp_data;
    int timeout = 0;

    tmp_data.word = 0;
    cnt = 0;
    len = 0;

    WriteIoPortByte(SMBHSTCTL, NCT6775_SOFT_RESET);

    switch (size)
    {
    case I2C_SMBUS_QUICK:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        break;
    case I2C_SMBUS_BYTE_DATA:
        tmp_data.byte = data->byte;
    case I2C_SMBUS_BYTE:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        WriteIoPortByte(SMBHSTIDX, command);
        if (read_write == I2C_SMBUS_WRITE)
        {
            WriteIoPortByte(SMBHSTDAT, tmp_data.byte);
            WriteIoPortByte(SMBHSTCMD, NCT6775_WRITE_BYTE);
        }
        else
        {
            WriteIoPortByte(SMBHSTCMD, NCT6775_READ_BYTE);
        }
        break;
    case I2C_SMBUS_WORD_DATA:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        WriteIoPortByte(SMBHSTIDX, command);
        if (read_write == I2C_SMBUS_WRITE)
        {
            WriteIoPortByte(SMBHSTDAT, data->word & 0xFF);
            WriteIoPortByte(SMBHSTDAT, (data->word & 0xFF00) >> 8);
            WriteIoPortByte(SMBHSTCMD, NCT6775_WRITE_WORD);
        }
        else
        {
            WriteIoPortByte(SMBHSTCMD, NCT6775_READ_WORD);
        }
        break;
    case I2C_SMBUS_BLOCK_DATA:
        WriteIoPortByte(SMBHSTADD, (addr << 1) | read_write);
        WriteIoPortByte(SMBHSTIDX, command);
        if (read_write == I2C_SMBUS_WRITE)
        {
            len = data->block[0];
            if (len == 0 || len > I2C_SMBUS_BLOCK_MAX)
            {
                return -EINVAL;
            }
            WriteIoPortByte(SMBBLKSZ, len);

            //Load 4 bytes into FIFO
            cnt = 1;
            if (len >= 4)
            {
                for (i = cnt; i <= 4; i++)
                {
                    WriteIoPortByte(SMBHSTDAT, data->block[i]);
                }

                len -= 4;
                cnt += 4;
            }
            else
            {
                for (i = cnt; i <= len; i++)
                {
                    WriteIoPortByte(SMBHSTDAT, data->block[i]);
                }

                len = 0;
            }

            WriteIoPortByte(SMBHSTCMD, NCT6775_WRITE_BLOCK);
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

    WriteIoPortByte(SMBHSTCTL, NCT6775_MANUAL_START);

    while ((size == I2C_SMBUS_BLOCK_DATA) && (len > 0))
    {
        if (read_write == I2C_SMBUS_WRITE)
        {
            timeout = 0;
            while ((ReadIoPortByte(SMBHSTSTS) & NCT6775_FIFO_EMPTY) == 0)
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
                    WriteIoPortByte(SMBHSTDAT, data->block[i]);
                }

                len -= 4;
                cnt += 4;
            }
            else
            {
                for (i = cnt; i <= (cnt + len); i++)
                {
                    WriteIoPortByte(SMBHSTDAT, data->block[i]);
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
    while ((ReadIoPortByte(SMBHSTSTS) & NCT6775_MANUAL_ACTIVE) != 0)
    {
        if(timeout > NCT6775_MAX_RETRIES)
        {
            return -ETIMEDOUT;
        }
        std::this_thread::sleep_for(1ms);;
        timeout++;
    }

    if ((ReadIoPortByte(SMBHSTERR) & NCT6775_NO_ACK) != 0)
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
        data->byte = (u8)ReadIoPortByte(SMBHSTDAT);
        break;
    case I2C_SMBUS_WORD_DATA:
        data->word = ReadIoPortByte(SMBHSTDAT) + (ReadIoPortByte(SMBHSTDAT) << 8);
        break;
    }

    return 0;
}

s32 i2c_smbus_nct6775::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
#ifdef _WIN32
    if(global_smbus_access_handle != NULL)
    {
        WaitForSingleObject(global_smbus_access_handle, INFINITE);
    }
#endif

    s32 result = nct6775_access(addr, read_write, command, size, data);

#ifdef _WIN32
    if(global_smbus_access_handle != NULL)
    {
        ReleaseMutex(global_smbus_access_handle);
    }
#endif

    return result;
}

s32 i2c_smbus_nct6775::i2c_xfer(u8 /*addr*/, char /*read_write*/, int* /*size*/, u8* /*data*/)
{
    return -1;
}

bool i2c_smbus_nct6775_detect()
{
#ifdef _WIN32
    if(!InitializeOls() || GetDllStatus())
    {
        LOG_INFO("WinRing0 is not loaded, nct6775 I2C bus detection aborted");
        return(false);
    }
#elif _MACOSX_X86_X64
    if(!GetMacUSPCIODriverStatus())
    {
        LOG_INFO("macUSPCIO is not loaded, nct6775 I2C bus detection aborted");
        return(false);
    }
#endif

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
            snprintf(bus->device_name, 512, "Nuvoton NCT5577D SMBus at %X", smba);
            break;
        case SIO_NCT6102_ID:
            snprintf(bus->device_name, 512, "Nuvoton NCT6102D/NCT6106D SMBus at %X", smba);
            break;
        case SIO_NCT6793_ID:
            snprintf(bus->device_name, 512, "Nuvoton NCT6793D SMBus at %X", smba);
            break;
        case SIO_NCT6796_ID:
            snprintf(bus->device_name, 512, "Nuvoton NCT6796D SMBus at %X", smba);
            break;
        case SIO_NCT6798_ID:
            snprintf(bus->device_name, 512, "Nuvoton NCT6798D SMBus at %X", smba);
            break;
        }

        ResourceManager::get()->RegisterI2CBus(bus);
    }

    return(true);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_nct6775_detect);

/*---------------------------------------------------------*\
| i2c_smbus_piix4.cpp                                       |
|                                                           |
|   PIIX4 SMBUS driver for Windows                          |
|                                                           |
|   Adam Honse (CalcProgrammer1)                08 Aug 2018 |
|   Portions based on Linux source code                     |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "i2c_smbus_piix4.h"
#include "LogManager.h"
#ifdef _WIN32
#include "OlsApi.h"
#include "wmi.h"
#elif _MACOSX_X86_X64
#include <unistd.h>
#include "macUSPCIOAccess.h"
#include "pci_ids.h"
#endif
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
#ifdef _WIN32
    if(amd_smbus_reduce_cpu)
    {
        delay_timer = CreateWaitableTimerExW(NULL, NULL, CREATE_WAITABLE_TIMER_MANUAL_RESET | CREATE_WAITABLE_TIMER_HIGH_RESOLUTION, TIMER_ALL_ACCESS);
        if(delay_timer == NULL) // high resolution timer not supported
        {
            delay_timer = CreateWaitableTimer(NULL, TRUE, NULL); // create regular timer instead
        }
    }

    bool shared_smbus_access = true;
    if(drivers_settings.contains("shared_smbus_access"))
    {
        shared_smbus_access = drivers_settings["shared_smbus_access"].get<bool>();
    }
    if(shared_smbus_access)
    {
        global_smbus_access_handle = CreateMutexA(NULL, FALSE, GLOBAL_SMBUS_MUTEX_NAME);
    }
#else
    delay_timer = amd_smbus_reduce_cpu;
#endif
}

i2c_smbus_piix4::~i2c_smbus_piix4()
{
#ifdef _WIN32
    if(delay_timer != NULL)
    {
        CloseHandle(delay_timer);
    }

    if(global_smbus_access_handle != NULL)
    {
        CloseHandle(global_smbus_access_handle);
    }
#endif
}

//Logic adapted from piix4_transaction() in i2c-piix4.c
int i2c_smbus_piix4::piix4_transaction()
{
    int result = 0;
    int temp;
    int timeout = 0;

    /* start the transaction by setting bit 6 */
    temp = ReadIoPortByte(SMBHSTCNT);
    WriteIoPortByte(SMBHSTCNT, temp | 0x040);

    /* We will always wait for a fraction of a second! (See PIIX4 docs errata) */
    /*---------------------------------------------------------------------------------------------------------------*\
    | The above comment from 2002 or earlier is still relevant for the Zen 4 architecture.                            |
    | AMD takes bug-for-bug compatibility seriously. It's the least they can do since they've classified Ryzen        |
    | system programming documentation as trade secret.                                                               |
    |                                                                                                                 |
    | Instead of just waiting for an unspecified amount of time, we try to follow exactly what the                    |
    | Intel 83271 Specification Update says about SMBHSTSTS.                                                          |
    |                                                                                                                 |
    |-----------------------------------------------------------------------------------------------------------------|
    | [Bit] 1  SMBus Interrupt/Host Completion (INTER)—R/WC.                                                          |
    | 1 = Indicates that the host transaction has completed or that the source of an SMBus interrupt was the          |
    | completion of the last host command.                                                                            |
    | 0 = Host transaction has not completed or that an SMBus interrupt was not caused by host command completion.    |
    | This bit is only set by hardware and can only be reset by writing a 1 to this bit position.                     |
    |-----------------------------------------------------------------------------------------------------------------|
    | [Bit] 0  Host Busy (HOST_BUSY)—RO.                                                                              |
    | 1 = Indicates that the SMBus controller host interface is in the process of completing a command.               |
    | 0 = SMBus controller host interface is not processing a command. None of the other registers should be accessed |
    | if this bit is set. Note that there may be moderate latency before the transaction begins and the Host Busy bit |
    | gets set.                                                                                                       |
    \*---------------------------------------------------------------------------------------------------------------*/
#ifdef _WIN32
    if(delay_timer != NULL)
    {
        LARGE_INTEGER retry_delay;
        retry_delay.QuadPart = RETRY_DELAY_US * -10;

        do
        {
            SetWaitableTimer(delay_timer, &retry_delay, 0, NULL, NULL, FALSE);
            WaitForSingleObject(delay_timer, INFINITE);
            temp = ReadIoPortByte(SMBHSTSTS);
        }
        while((++timeout < MAX_TIMEOUT) && ((temp & 0x03) != 0x02));
    }
#else
    if(delay_timer)
    {
        do
        {
            usleep(RETRY_DELAY_US);
            temp = ReadIoPortByte(SMBHSTSTS);
        }
        while((++timeout < MAX_TIMEOUT) && ((temp & 0x03) != 0x02));
    }
#endif
    else
    {
        std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
        do
        {
            temp = ReadIoPortByte(SMBHSTSTS);
        }
        while((std::chrono::steady_clock::now() < deadline) && ((temp & 0x03) != 0x02));
        temp = ReadIoPortByte(SMBHSTSTS); // we can be preempted between reading the register and checking for the timeout
    }

    /* If the SMBus is still busy, we give up */
    if (temp & 0x01)
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
    int i, len, status, temp;

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
#ifdef _WIN32
    if(global_smbus_access_handle != NULL)
    {
        WaitForSingleObject(global_smbus_access_handle, INFINITE);
    }
#endif

    s32 result = piix4_access(addr, read_write, command, size, data);

#ifdef _WIN32
    if(global_smbus_access_handle != NULL)
    {
        ReleaseMutex(global_smbus_access_handle);
    }
#endif

    return result;
}

s32 i2c_smbus_piix4::i2c_xfer(u8 /*addr*/, char /*read_write*/, int* /*size*/, u8* /*data*/)
{
    return -1;
}
#ifdef _WIN32
bool i2c_smbus_piix4_detect()
{
    if(!InitializeOls() || GetDllStatus())
    {
        LOG_INFO("WinRing0 is not loaded, piix4 I2C bus detection aborted");
        return(false);
    }

    i2c_smbus_interface * bus;
    HRESULT hres;
    Wmi wmi;

    // Query WMI for Win32_PnPSignedDriver entries with names matching "SMBUS" or "SM BUS"
    // These devices may be browsed under Device Manager -> System Devices
    std::vector<QueryObj> q_res_PnPSignedDriver;
    hres = wmi.query("SELECT * FROM Win32_PnPSignedDriver WHERE Description LIKE '%SMBUS%' OR Description LIKE '%SM BUS%'", q_res_PnPSignedDriver);

    if (hres)
    {
        LOG_INFO("WMI query failed, piix4 I2C bus detection aborted");
        return(false);
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

    return(true);
}
#elif _MACOSX_X86_X64
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
#endif

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_piix4_detect);

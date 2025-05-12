/*---------------------------------------------------------*\
| i2c_smbus_linux.cpp                                       |
|                                                           |
|   Linux i2c/smbus driver                                  |
|                                                           |
|   Adam Honse (CalcProgrammer1)                14 Feb 2019 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <cstring>
#include "LogManager.h"
#include "i2c_smbus.h"
#include "i2c_smbus_linux.h"

s32 i2c_smbus_linux::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, union i2c_smbus_data* data)
{

    struct i2c_smbus_ioctl_data args;

    //Tell I2C host which slave address to transfer to
    ioctl(handle, I2C_SLAVE, addr);

    args.read_write = read_write;
    args.command = command;
    args.size = size;
    args.data = data;

    return ioctl(handle, I2C_SMBUS, &args);
}

s32 i2c_smbus_linux::i2c_xfer(u8 addr, char read_write, int* size, u8* data)
{
    i2c_rdwr_ioctl_data rdwr;
    i2c_msg msg;
    s32 ret_val;

    msg.addr  = addr;
    msg.flags = read_write;
    msg.len   = *size;
    msg.buf   = (u8*)malloc(*size);
    memcpy(msg.buf, data, *size);

    rdwr.msgs  = &msg;
    rdwr.nmsgs = 1;

    ret_val = ioctl(handle, I2C_RDWR, &rdwr);

    /*-------------------------------------------------*\
    | If operation was a read, copy read data and size  |
    \*-------------------------------------------------*/
    if(read_write == I2C_SMBUS_READ)
    {
        *size = msg.len;
        memcpy(data, msg.buf, *size);
    }

    free(msg.buf);

    return ret_val;
}

#include "Detector.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

bool i2c_smbus_linux_detect()
{
    i2c_smbus_linux *       bus;
    char                    device_string[1024];
    DIR *                   dir;
    char                    driver_path[512];
    struct dirent *         ent;
    int                     test_fd;
    int                     ret = true;
    char path[1024];
    char buff[100];
    unsigned short pci_device, pci_vendor, pci_subsystem_device, pci_subsystem_vendor;
    unsigned short port_id, bus_id;
    char *ptr;

    // Start looking for I2C adapters in /sys/bus/i2c/devices/
    strcpy(driver_path, "/sys/bus/i2c/devices/");
    dir = opendir(driver_path);

    if(dir == NULL)
    {
        return(false);
    }

    // Loop through all entries in i2c-adapter list
    while((ent = readdir(dir)) != NULL)
    {
        if(ent->d_type == DT_DIR || ent->d_type == DT_LNK)
        {
            if(strncmp(ent->d_name, "i2c-", 4) == 0)
            {
                strcpy(device_string, driver_path);
                strcat(device_string, ent->d_name);
                strcat(device_string, "/name");
                test_fd = open(device_string, O_RDONLY);

                if(test_fd)
                {
                    memset(device_string, 0x00, sizeof(device_string));

                    if(read(test_fd, device_string, sizeof(device_string)) < 0)
                    {
                        LOG_WARNING("[i2c_smbus_linux] Failed to read i2c device name");
                    }

                    device_string[strlen(device_string) - 1] = 0x00;

                    close(test_fd);

                    // Clear PCI Information
                    pci_vendor              = 0;
                    pci_device              = 0;
                    pci_subsystem_vendor    = 0;
                    pci_subsystem_device    = 0;
                    port_id                 = 0;
                    bus_id                  = 0;

                    // Get port ID for Nvidia GPUs
                    sscanf(device_string, "NVIDIA i2c adapter %hu at", &port_id);

                    // Get the Linux Bus ID
                    sscanf(ent->d_name, "i2c-%hu", &bus_id);

                    // Get device path
                    strcpy(path, driver_path);
                    strcat(path, ent->d_name);
                    if(ent->d_type == DT_LNK)
                    {
                        ptr = realpath(path, NULL);
                        if(ptr == NULL)
                            continue;

                        strcpy(path, ptr);
                        strcat(path, "/..");
                        free(ptr);
                    }
                    else
                    {
                        strcat(path, "/device");
                    }
                    ptr = path + strlen(path);

                    // Get PCI Vendor
                    strcpy(ptr, "/vendor");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));

                        if(read(test_fd, buff, sizeof(buff)) < 0)
                        {
                            LOG_WARNING("[i2c_smbus_linux] Failed to read i2c device PCI vendor ID");
                        }

                        buff[strlen(buff) - 1] = 0x00;
                        pci_vendor = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }

                    // Get PCI Device
                    strcpy(ptr, "/device");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));

                        if(read(test_fd, buff, sizeof(buff)) < 0)
                        {
                            LOG_WARNING("[i2c_smbus_linux] Failed to read i2c device PCI device ID");
                        }

                        buff[strlen(buff) - 1] = 0x00;
                        pci_device = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }

                    // Get PCI Subsystem Vendor
                    strcpy(ptr, "/subsystem_vendor");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));

                        if(read(test_fd, buff, sizeof(buff)) < 0)
                        {
                            LOG_WARNING("[i2c_smbus_linux] Failed to read i2c device PCI subvendor ID");
                        }

                        buff[strlen(buff) - 1] = 0x00;
                        pci_subsystem_vendor = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }

                    // Get PCI Subsystem Device
                    strcpy(ptr, "/subsystem_device");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));

                        if(read(test_fd, buff, sizeof(buff)) < 0)
                        {
                            LOG_WARNING("[i2c_smbus_linux] Failed to read i2c device PCI subdevice ID");
                        }

                        buff[strlen(buff) - 1] = 0x00;
                        pci_subsystem_device = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }

                    strcpy(device_string, "/dev/");
                    strcat(device_string, ent->d_name);
                    test_fd = open(device_string, O_RDWR);

                    if (test_fd < 0)
                    {
                        ret = false;
                    }

                    bus = new i2c_smbus_linux();
                    strcpy(bus->device_name, device_string);
                    bus->handle               = test_fd;
                    bus->pci_device           = pci_device;
                    bus->pci_vendor           = pci_vendor;
                    bus->pci_subsystem_device = pci_subsystem_device;
                    bus->pci_subsystem_vendor = pci_subsystem_vendor;
                    bus->port_id              = port_id;
                    bus->bus_id               = bus_id;
                    ResourceManager::get()->RegisterI2CBus(bus);
                }
                else
                {
                    ret = false;
                }
            }
        }
    }
    closedir(dir);

    return(ret);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_linux_detect);

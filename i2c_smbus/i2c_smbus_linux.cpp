/*-----------------------------------------*\
|  i2c_smbus_linux.cpp                      |
|                                           |
|  Linux i2c/smbus driver                   |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/14/2019   |
\*-----------------------------------------*/

#include "i2c_smbus.h"
#include "i2c_smbus_linux.h"

#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <sys/ioctl.h>

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

#include "Detector.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

void i2c_smbus_linux_detect()
{
    i2c_smbus_linux *       bus;
    char                    device_string[1024];
    DIR *                   dir;
    char                    driver_path[512];
    struct dirent *         ent;
    int                     test_fd;
    char path[1024];
    char buff[100];
    unsigned short pci_device, pci_vendor, pci_subsystem_device, pci_subsystem_vendor;
    unsigned short port_id;

    // Start looking for I2C adapters in /sys/bus/i2c/devices/
    strcpy(driver_path, "/sys/bus/i2c/devices/");
    dir = opendir(driver_path);

    if(dir == NULL)
    {
        return;
    }

    // Loop through all entries in i2c-adapter list
    ent = readdir(dir);
    while(ent != NULL)
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
                    read(test_fd, device_string, sizeof(device_string));
                    device_string[strlen(device_string) - 1] = 0x00;

                    close(test_fd);

                    // Clear PCI Information
                    pci_vendor              = 0;
                    pci_device              = 0;
                    pci_subsystem_vendor    = 0;
                    pci_subsystem_device    = 0;
                    port_id                 = 0;

                    // Get PCI Vendor
                    snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/vendor");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_vendor = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }

                    // Get port ID for NVidia GPUs
                    sscanf(device_string, "NVIDIA i2c adapter %hu at", &port_id);

                    // Get PCI Device
                    snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/device");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_device = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }

                    // Get PCI Subsystem Vendor
                    snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/subsystem_vendor");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_subsystem_vendor = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }

                    // Get PCI Subsystem Device
                    snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/subsystem_device");
                    test_fd = open(path, O_RDONLY);
                    if (test_fd >= 0)
                    {
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_subsystem_device = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }
                    
                    strcpy(device_string, "/dev/");
                    strcat(device_string, ent->d_name);
                    test_fd = open(device_string, O_RDWR);

                    if (test_fd < 0)
                    {
                        ent = readdir(dir);
                        continue;
                    }

                    bus = new i2c_smbus_linux();
                    strcpy(bus->device_name, device_string);
                    bus->handle               = test_fd;
                    bus->pci_device           = pci_device;
                    bus->pci_vendor           = pci_vendor;
                    bus->pci_subsystem_device = pci_subsystem_device;
                    bus->pci_subsystem_vendor = pci_subsystem_vendor;
                    bus->port_id              = port_id;
                    ResourceManager::get()->RegisterI2CBus(bus);
                }
            }
        }
        ent = readdir(dir);
    }
    closedir(dir);
}

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_linux_detect);

/******************************************************************************************\
*                                                                                          *
*   OpenAuraSDK.cpp                                                                        *
*                                                                                          *
*       Functions for communicating with Asus Aura devices on Windows and Linux            *
*                                                                                          *
\******************************************************************************************/

#include "AuraController.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32

#include "i2c_smbus_piix4.h"
#include "i2c_smbus_i801.h"
#include "wmi.h"

#else /* WIN32 */

#include "i2c_smbus_linux.h"
#include <regex>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#endif /* WIN32 */

std::vector<AuraController *> controllers;
std::vector<i2c_smbus_interface *> busses;


/******************************************************************************************\
*                                                                                          *
*   CreateAuraDevice                                                                       *
*                                                                                          *
*       Enumerates an ASUS Aura compatibler I2C device with the given address on the given *
*       bus                                                                                *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

AuraController * CreateAuraDevice(i2c_smbus_interface * bus, aura_dev_id dev)
{
    AuraController * aura;

    aura = new AuraController();
    aura->bus = bus;
    aura->dev = dev;

    return(aura);

}   /* CreateAuraDevice() */

#ifdef WIN32
/******************************************************************************************\
*                                                                                          *
*   DetectI2CBusses (Windows)                                                              *
*                                                                                          *
*       Detects available AMD and Intel SMBUS adapters and enumerates i2c_smbus_interface  *
*       objects for them                                                                   *
*                                                                                          *
\******************************************************************************************/

void DetectI2CBusses()
{
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
            bus = new i2c_smbus_piix4();
            ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B00;
            busses.push_back(bus);

            bus = new i2c_smbus_piix4();
            ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B20;
            busses.push_back(bus);
        }

        // Intel SMBus controllers do show I/O resources in Device Manager
        // Analysis of many Intel boards has shown that Intel SMBus adapter I/O space varies between boards
        // We can query Win32_PnPAllocatedResource entries and look up the PCI device ID to find the allocated I/O space
        // Intel SMBus adapters use the i801 driver
        else if (i["Manufacturer"].find("Intel") != std::string::npos)
        {
            std::string rgx1 = ".+" + q_res_PnPSignedDriver[0]["DeviceID"].substr(4, 33) + ".+";

            AdditionalFilters filters;
            filters.emplace("Dependent", rgx1);
            filters.emplace("Antecedent", ".*Port.*");

            std::vector<QueryObj> q_res_PNPAllocatedResource;
            hres = wmi.query("SELECT * FROM Win32_PnPAllocatedResource", q_res_PNPAllocatedResource, &filters);

            std::regex rgx2(".*StartingAddress=\"(\\d+)\".*");
            std::smatch matches;

            // Query the StartingAddress for the matching device ID and use it to enumerate the bus
            if (std::regex_search(q_res_PNPAllocatedResource[0]["Antecedent"], matches, rgx2))
            {
                unsigned int IORangeStart = std::stoi(matches[1].str());

                bus = new i2c_smbus_i801();
                ((i2c_smbus_i801 *)bus)->i801_smba = IORangeStart;
                busses.push_back(bus);
            }
        }
    }

}   /* DetectI2CBusses() */

#else /* WIN32 */

/******************************************************************************************\
*                                                                                          *
*   DetectI2CBusses (Linux)                                                                *
*                                                                                          *
*       Detects available SMBUS adapters and enumerates i2c_smbus_interface objects for    *
*       them                                                                               *
*                                                                                          *
\******************************************************************************************/

void DetectI2CBusses()
{
    i2c_smbus_linux *       bus;
    char                    device_string[1024];
    DIR *                   dir;
    char                    driver_path[512];
    struct dirent *         ent;
    int                     test_fd;

    // Start looking for I2C adapters in /sys/class/i2c-adapter/
    strcpy(driver_path, "/sys/class/i2c-adapter/");
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
                    read(test_fd, device_string, sizeof(device_string));
                    close(test_fd);

                    //ignore nvidia adapters for now
                    if(strncmp(device_string, "NVIDIA", 6) == 0)
                    {
                        ent = readdir(dir);
                        continue;
                    }

                    bus = new i2c_smbus_linux();
                    strcpy(bus->device_name, device_string);

                    strcpy(device_string, "/dev/");
                    strcat(device_string, ent->d_name);
                    test_fd = open(device_string, O_RDWR);

                    if (test_fd < 0)
                    {
                        delete bus;
                        ent = readdir(dir);
                        continue;
                    }

                    bus->handle = test_fd;
                    busses.push_back(bus);
                }
            }
        }
        ent = readdir(dir);
    }

}   /* DetectI2CBusses() */

#endif  /* WIN32 */

/******************************************************************************************\
*                                                                                          *
*   DetectAuraControllers                                                                  *
*                                                                                          *
*       Detect Aura controllers on the enumerated I2C busses.  Searches for Aura-enabled   *
*       RAM at 0x77 and tries to initialize their slot addresses, then searches for them   *
*       at their correct initialized addresses.  Also looks for motherboard controller at  *
*       address 0x4E.                                                                      *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface where Aura device is connected            *
*           dev - I2C address of Aura device                                               *
*                                                                                          *
\******************************************************************************************/

void DetectAuraControllers()
{
    for (unsigned int bus = 0; bus < busses.size(); bus++)
    {
        // Remap Aura-enabled RAM modules on 0x77
        for (unsigned int slot = 0; slot < 8; slot++)
        {
            int res = busses[bus]->i2c_smbus_write_quick(0x77, I2C_SMBUS_WRITE);

            if (res < 0)
            {
                break;
            }

            AuraController * temp_controller = CreateAuraDevice(busses[bus], 0x77);

            temp_controller->AuraRegisterWrite(AURA_REG_SLOT_INDEX, slot);
            temp_controller->AuraRegisterWrite(AURA_REG_I2C_ADDRESS, 0xE0 + (slot << 1));

            delete temp_controller;
        }

        // Add Aura-enabled controllers at their remapped addresses
        for (unsigned int slot = 0; slot < 8; slot++)
        {
            int res = busses[bus]->i2c_smbus_write_quick(0x70 + slot, I2C_SMBUS_WRITE);

            if (res >= 0)
            {
                AuraController * new_controller = CreateAuraDevice(busses[bus], 0x70 + slot);
                controllers.push_back(new_controller);
            }
        }

        // Check for Aura controller at 0x4E
        int res = busses[bus]->i2c_smbus_write_quick(0x4E, I2C_SMBUS_WRITE);

        if (res >= 0)
        {
            AuraController * new_controller = CreateAuraDevice(busses[bus], 0x4E);
            controllers.push_back(new_controller);
        }
    }

}   /* DetectAuraControllers() */


/******************************************************************************************\
*                                                                                          *
*   DetectI2C                                                                              *
*                                                                                          *
*       Prints a list of all detected I2C addresses on the given bus                       *
*                                                                                          *
*           bus - pointer to i2c_smbus_interface to scan                                   *
*           mode - one of AUTO, QUICK, READ, FUNC - method of access                       *
*                                                                                          *
*       Code adapted from i2cdetect.c from i2c-tools Linux package                         *
*                                                                                          *
\******************************************************************************************/

#define MODE_AUTO   0
#define MODE_QUICK  1
#define MODE_READ   2
#define MODE_FUNC   3

void DetectI2C(i2c_smbus_interface * bus, int mode)
{
    int i, j;
    int res;
    int slave_addr;

    freopen("i2cdetect.txt", "a", stdout);

    printf("     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");

    for (i = 0; i < 128; i += 16)
    {
        printf("%02x: ", i);

        for (j = 0; j < 16; j++)
        {
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
                printf("-- ");
            }
            else
            {
                printf("%02x ", i + j);
            }
        }
        printf("\r\n");
    }

}   /* DetectI2C() */


/******************************************************************************************\
*                                                                                          *
*   DumpAuraRegisters                                                                      *
*                                                                                          *
*       Dumps register values from an Aura device                                          *
*                                                                                          *
\******************************************************************************************/

void DumpAuraRegisters(AuraController * controller)
{
    int i, j;

    int start = 0x8000;

    freopen("auradump.txt", "a", stdout);

    printf("       0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");

    for (i = 0; i < 512; i += 16)
    {
        printf("%04x: ", i + start);

        for (j = 0; j < 16; j++)
        {
            printf("%02x ", controller->AuraRegisterRead(start + i + j));
        }

        printf("\r\n");
    }

}   /* DumpAuraRegisters() */


/******************************************************************************************\
*                                                                                          *
*   main/WinMain                                                                           *
*                                                                                          *
*       Main function.  Has no defined purpose yet, but is where you can call the other    *
*       functions in this project to test them out                                         *
*                                                                                          *
*       Currently, this program will enumerate all detected SMBus adapters and then detect *
*       all attached devices, similar to i2cdetect on Linux                                *
*                                                                                          *
\******************************************************************************************/

#ifdef WIN32
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else /* WIN32 */
int main()
#endif /* WIN32 */
{
    // Colors Array              R    B    G
    unsigned char colors[15] = { 255, 0,   0,
                                 0,   255, 0,
                                 0,   0,   255,
                                 255, 0,   255,
                                 255, 255, 255  };
    DetectI2CBusses();

    DetectAuraControllers();

    for (unsigned int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_DIRECT, 1);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }

    for (unsigned int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWriteBlock(AURA_REG_COLORS_DIRECT, colors, 15);
    }

    return 1;
}

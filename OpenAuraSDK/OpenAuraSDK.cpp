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

#include <tchar.h>
#include <regex>
#include "OpenAuraSDKDialog.h"
#include "I2CDetectDialog.h"
#include "i2c_smbus_piix4.h"
#include "i2c_smbus_i801.h"
#include "wmi.h"

#else /* WIN32 */

#include "OpenAuraSDKQtDialog.h"
#include "i2c_smbus_linux.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

#endif /* WIN32 */

std::vector<AuraController *> controllers;
std::vector<i2c_smbus_interface *> busses;

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
            strcpy(bus->device_name, i["Description"].c_str());
            strcat(bus->device_name, " at 0x0B00");
            ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B00;
            busses.push_back(bus);

            bus = new i2c_smbus_piix4();
            ((i2c_smbus_piix4 *)bus)->piix4_smba = 0x0B20;
            strcpy(bus->device_name, i["Description"].c_str());
            strcat(bus->device_name, " at 0x0B20");
            busses.push_back(bus);
        }

        // Intel SMBus controllers do show I/O resources in Device Manager
        // Analysis of many Intel boards has shown that Intel SMBus adapter I/O space varies between boards
        // We can query Win32_PnPAllocatedResource entries and look up the PCI device ID to find the allocated I/O space
        // Intel SMBus adapters use the i801 driver
        else if ((i["Manufacturer"].find("Intel") != std::string::npos)
              || (i["Manufacturer"].find("INTEL") != std::string::npos))
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
                strcpy(bus->device_name, i["Description"].c_str());
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
*   TestForAuraController                                                                  *
*                                                                                          *
*       Tests the given address to see if an Aura controller exists there.  First does a   *
*       quick write to test for a response, and if so does a simple read at 0xA0 to test   *
*       for incrementing values 0...F which was observed at this location during data dump *
*                                                                                          *
\******************************************************************************************/

bool TestForAuraController(i2c_smbus_interface* bus, unsigned char address)
{
    bool pass = false;

    int res = bus->i2c_smbus_write_quick(address, I2C_SMBUS_WRITE);
    
    if (res >= 0)
    {
        pass = true;

        for (int i = 0xA0; i < 0xB0; i++)
        {
            res = bus->i2c_smbus_read_byte_data(address, i);

            if (res != (i - 0xA0))
            {
                pass = false;
            }
        }
    }

    return(pass);

}   /* TestForAuraController() */


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
    AuraController* new_controller;

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

            AuraController temp_controller(busses[bus], 0x77);

            temp_controller.AuraRegisterWrite(AURA_REG_SLOT_INDEX, slot);
            temp_controller.AuraRegisterWrite(AURA_REG_I2C_ADDRESS, 0xE0 + (slot << 1));
        }

        // Add Aura-enabled controllers at their remapped addresses
        for (unsigned int slot = 0; slot < 8; slot++)
        {
            if (TestForAuraController(busses[bus], 0x70 + slot))
            {
                new_controller = new AuraController(busses[bus], 0x70 + slot);
                controllers.push_back(new_controller);
            }
        }

        // Check for Aura controller at 0x40
        if (TestForAuraController(busses[bus], 0x40))
        {
            new_controller = new AuraController(busses[bus], 0x40);
            controllers.push_back(new_controller);
        }

        // Check for Aura controller at 0x4E
        if (TestForAuraController(busses[bus], 0x4E))
        {
            new_controller = new AuraController(busses[bus], 0x4E);
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

std::string DetectI2C(i2c_smbus_interface * bus, int mode)
{
    int i, j;
    int res;
    int slave_addr;
    char line[128];
    std::string text;

    freopen("i2cdetect.txt", "a", stdout);

    sprintf(line, "     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");
    text.append(line);

    for (i = 0; i < 128; i += 16)
    {
        sprintf(line, "%02x: ", i);
        text.append(line);

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
                sprintf(line, "-- ");
                text.append(line);
            }
            else
            {
                sprintf(line, "%02x ", i + j);
                text.append(line);
            }
        }
        sprintf(line, "\r\n");
        text.append(line);
    }

    return text;

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

    int start = 0x0000;

    FILE* file = freopen("auradump.txt", "a", stdout);

    printf("       0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f\r\n");

    for (i = 0; i < 0xFFFF; i += 16)
    {
        printf("%04x: ", i + start);

        for (j = 0; j < 16; j++)
        {
            printf("%02x ", controller->AuraRegisterRead(start + i + j));
        }

        printf("\r\n");
    }

    fclose(file);

}   /* DumpAuraRegisters() */


/******************************************************************************************\
*                                                                                          *
*   main                                                                                   *
*                                                                                          *
*       Main function.  Detects busses and Aura controllers, then opens the main window    *
*                                                                                          *
\******************************************************************************************/

int main(int argc, char *argv[])
{
    DetectI2CBusses();

    DetectAuraControllers();

#if WIN32
    OpenAuraSDKDialog dlg(busses, controllers);
    dlg.DoModal();

    return 0;

#else
    QApplication a(argc, argv);

    Ui::OpenAuraSDKQtDialog dlg(busses, controllers);
    dlg.show();

    return a.exec();
#endif
}

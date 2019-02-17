// OpenAuraSDK.cpp : Defines the exported functions for the DLL application.
//

#include "i2c_smbus.h"
//#include "i2c_smbus_piix4.h"
//#include "i2c_smbus_i801.h"
#include "i2c_smbus_linux.h"
#include "AuraController.h"
#include "OpenAuraSDK.h"
#include <vector>
#include <regex>

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <unistd.h>
#include <dirent.h>

//#include "wmi.h"

std::vector<AuraController *> controllers;
std::vector<i2c_smbus_interface *> busses;
#if 0
// DetectI2CBusses
//
//  Detects available AMD and Intel SMBUS adapters and enumerates i2c_smbus_interface objects for them
//
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
        {#include <dirent.h>
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
}
#endif

// CreateAuraDevice
//
//  Enumerates an ASUS Aura compatibler I2C device with the given address on the given bus
//
//      bus - pointer to i2c_smbus_interface where Aura device is connected
//      dev - I2C address of Aura device
//
AuraController * CreateAuraDevice(i2c_smbus_interface * bus, aura_dev_id dev)
{
    AuraController * aura;

    aura = new AuraController();
    aura->bus = bus;
    aura->dev = dev;

    return(aura);
}

void DeleteAuraDevice(AuraController * controller)
{
    delete controller;
}

// DetectI2C
//
//  Prints a list of all detected I2C addresses on the given bus
//
//      bus - pointer to i2c_smbus_interface to scan
//      mode - one of AUTO, QUICK, READ, FUNC - method of access
//
//  Code adapted from i2cdetect.c from i2c-tools Linux package
//
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

            if (slave_addr == 0x08)
            {
                int a = 0;
            }
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
}

// DumpAuraDevices
//
//  Dumps known register values from all enumerated Aura devices
//
void DumpAuraDevices()
{
    freopen("auradump.txt", "w", stdout);

    for (unsigned int i = 0; i < controllers.size(); i++)
    {
        unsigned char dcolor0 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x00 );
        unsigned char dcolor1 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x01 );
        unsigned char dcolor2 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x02 );
        unsigned char dcolor3 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x03 );
        unsigned char dcolor4 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x04 );
        unsigned char dcolor5 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x05 );
        unsigned char dcolor6 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x06 );
        unsigned char dcolor7 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x07 );
        unsigned char dcolor8 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x08 );
        unsigned char dcolor9 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x09 );
        unsigned char dcolorA = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x0A );
        unsigned char dcolorB = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x0B );
        unsigned char dcolorC = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x0C );
        unsigned char dcolorD = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x0D );
        unsigned char dcolorE = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_DIRECT + 0x0E );

        unsigned char ecolor0 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x00 );
        unsigned char ecolor1 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x01 );
        unsigned char ecolor2 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x02 );
        unsigned char ecolor3 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x03 );
        unsigned char ecolor4 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x04 );
        unsigned char ecolor5 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x05 );
        unsigned char ecolor6 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x06 );
        unsigned char ecolor7 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x07 );
        unsigned char ecolor8 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x08 );
        unsigned char ecolor9 = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x09 );
        unsigned char ecolorA = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x0A );
        unsigned char ecolorB = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x0B );
        unsigned char ecolorC = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x0C );
        unsigned char ecolorD = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x0D );
        unsigned char ecolorE = controllers[i]->AuraRegisterRead( AURA_REG_COLORS_EFFECT + 0x0E );

        unsigned char direct  = controllers[i]->AuraRegisterRead( AURA_REG_DIRECT               );
        unsigned char mode    = controllers[i]->AuraRegisterRead( AURA_REG_MODE                 );

        printf("Controller %d\r\n", i);
        printf("Direct Mode:    %d \r\n", direct  );
        printf("Mode Value:     %d \r\n", mode    );
        printf("Direct Color 0: %d \r\n", dcolor0);
        printf("Direct Color 1: %d \r\n", dcolor1);
        printf("Direct Color 2: %d \r\n", dcolor2);
        printf("Direct Color 3: %d \r\n", dcolor3);
        printf("Direct Color 4: %d \r\n", dcolor4);
        printf("Direct Color 5: %d \r\n", dcolor5);
        printf("Direct Color 6: %d \r\n", dcolor6);
        printf("Direct Color 7: %d \r\n", dcolor7);
        printf("Direct Color 8: %d \r\n", dcolor8);
        printf("Direct Color 9: %d \r\n", dcolor9);
        printf("Direct Color A: %d \r\n", dcolorA);
        printf("Dirsmbus_adapter_devect Color B: %d \r\n", dcolorB);
        printf("Direct Color C: %d \r\n", dcolorC);
        printf("Direct Color D: %d \r\n", dcolorD);
        printf("Direct Color E: %d \r\n", dcolorE);
        printf("Effect Color 0: %d \r\n", ecolor0);
        printf("Effect Color 1: %d \r\n", ecolor1);
        printf("Effect Color 2: %d \r\n", ecolor2);
        printf("Effect Color 3: %d \r\n", ecolor3);
        printf("Effect Color 4: %d \r\n", ecolor4);
        printf("Effect Color 5: %d \r\n", ecolor5);
        printf("Effect Color 6: %d \r\n", ecolor6);
        printf("Effect Color 7: %d \r\n", ecolor7);
        printf("Effect Color 8: %d \r\n", ecolor8);
        printf("Effect Color 9: %d \r\n", ecolor9);
        printf("Effect Color A: %d \r\n", ecolorA);
        printf("Effect Color B: %d \r\n", ecolorB);
        printf("Effect Color C: %d \r\n", ecolorC);
        printf("Effect Color D: %d \r\n", ecolorD);
        printf("Effect Color E: %d \r\n", ecolorE);
    }
}

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
}

// WinMain
//
//  Main function.  Has no defined purpose yet, but is where you can call the other functions
//  in this project to test them out
//
//  Currently, this program will enumerate all detected SMBus adapters and then detect
//  all attached devices, similar to i2cdetect on Linux
//
//INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
int main()
{
    //DetectI2CBusses();

    char driver_path[512];
    DIR *dir;
    struct dirent *ent;

    strcpy(driver_path, "/sys/class/i2c-adapter/");

    dir = opendir(driver_path);

    if(dir == NULL)
    {
    }

    ent = readdir(dir);

    while(ent != NULL)
    {
        if(ent->d_type == DT_DIR || ent->d_type == DT_LNK)
        {
            if(strncmp(ent->d_name, "i2c-", 4) == 0)
            {
                int test_fd;
                char device_string[1024];
                strcpy(device_string, driver_path);
                strcat(device_string, ent->d_name);
                strcat(device_string, "/name");
                test_fd = open(device_string, O_RDONLY);

                if(test_fd)
                {
                    read(test_fd, device_string, sizeof(device_string));
                    close(test_fd);

                    i2c_smbus_interface *bus;
                    bus = new i2c_smbus_linux();
                    strcpy(bus->device_name, device_string);

                    //ignore nvidia adapters for now
                    if(strncmp(device_string, "NVIDIA", 6) == 0)
                    {
                        ent = readdir(dir);
                        continue;
                    }

                    strcpy(device_string, "/dev/");
                    strcat(device_string, ent->d_name);

                    test_fd = open(device_string, O_RDWR);

                    if (test_fd < 0)
                    {
                        ent = readdir(dir);
                        continue;
                    }

                    ((i2c_smbus_linux *)bus)->handle = test_fd;
                    busses.push_back(bus);
                }
            }
        }
        ent = readdir(dir);
    }

    for( int bus = 0; bus < busses.size(); bus++ )
    {
        // Remap Aura-enabled RAM modules on 0x77
        for(int slot = 0; slot < 8; slot++)
        {
            int res = busses[bus]->i2c_smbus_write_quick(0x77, I2C_SMBUS_WRITE);

            if(res < 0)
            {
                break;
            }

            AuraController * temp_controller = CreateAuraDevice(busses[bus], 0x77);

            controllers[0]->AuraRegisterWrite(AURA_REG_SLOT_INDEX,  slot);
            controllers[0]->AuraRegisterWrite(AURA_REG_I2C_ADDRESS, 0xE0 + ( slot << 1 ) );

            delete temp_controller;
        }

        // Add Aura-enabled controllers at their remapped addresses
        for(int slot = 0; slot < 8; slot++)
        {
            int res = busses[bus]->i2c_smbus_write_quick(0x70 + slot, I2C_SMBUS_WRITE);

            if(res >= 0)
            {
               AuraController * new_controller = CreateAuraDevice(busses[bus], 0x70 + slot);
               controllers.push_back(new_controller);
            }
        }

        // Check for Aura controller at 0x4E
        int res = busses[bus]->i2c_smbus_write_quick(0x4E, I2C_SMBUS_WRITE);

        if(res >= 0)
        {
            AuraController * new_controller = CreateAuraDevice(busses[bus], 0x4E);
            controllers.push_back(new_controller);
        }
    }

    for (int i = 0; i < controllers.size(); i++)
    {
        controllers[i]->AuraRegisterWrite(AURA_REG_DIRECT, 1);
        controllers[i]->AuraRegisterWrite(AURA_REG_APPLY, AURA_APPLY_VAL);
    }

    for (int i = 0; i < controllers.size(); i++)
    {
        for(int j = 0; j < 5; j++)
        {
            controllers[i]->AuraRegisterWrite(AURA_REG_COLORS_DIRECT + ( j * 3 ) + 0, 0);
            controllers[i]->AuraRegisterWrite(AURA_REG_COLORS_DIRECT + ( j * 3 ) + 1, 255  );
            controllers[i]->AuraRegisterWrite(AURA_REG_COLORS_DIRECT + ( j * 3 ) + 2, 0  );
        }
    }

    return 1;
}

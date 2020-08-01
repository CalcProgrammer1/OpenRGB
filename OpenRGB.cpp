/******************************************************************************************\
*                                                                                          *
*   OpenRGB.cpp                                                                            *
*                                                                                          *
*       Functions for communicating with RGBController API devices on Windows and Linux    *
*                                                                                          *
\******************************************************************************************/

#include "RGBController.h"
#include "i2c_smbus.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32
#include <regex>
#include "i2c_smbus_piix4.h"
#include "i2c_smbus_i801.h"
#include "i2c_smbus_nct6775.h"
#include "i2c_smbus_nvapi.h"
#include "i2c_smbus_amdadl.h"
#include "super_io.h"
#include "wmi.h"
#else /* WIN32 */

#include "i2c_smbus_linux.h"
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#endif /* WIN32 */

std::vector<i2c_smbus_interface*> busses;
std::vector<RGBController*> rgb_controllers;

#ifdef WIN32
/******************************************************************************************\
*                                                                                          *
*   DetectNuvotonI2CBusses (Windows)                                                       *
*                                                                                          *
*       Detects available Nuvoton Super IO SMBUS adapters and enumerates                   *
*       i2c_smbus_interface objects for them                                               *
*                                                                                          *
\******************************************************************************************/

void DetectNuvotonI2CBusses()
{
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
        bus = new i2c_smbus_nct6775();

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

        busses.push_back(bus);
    }

}   /* DetectNuvotonI2CBusses() */

/******************************************************************************************\
*                                                                                          *
*   DetectNvAPII2CBusses (Windows)                                                         *
*                                                                                          *
*       Detects available NVidia NvAPI I2C adapters and enumerates                         *
*       i2c_smbus_interface objects for them.  Only enumerates the first bus for each GPU  *
*                                                                                          *
\******************************************************************************************/

void DetectNvAPII2CBusses()
{
    static NV_PHYSICAL_GPU_HANDLE   gpu_handles[64];
    static NV_S32                   gpu_count = 0;
    NV_U32                          device_id;
    NV_U32                          ext_device_id;
    NV_STATUS                       res;
    NV_U32                          revision_id;
    NV_U32                          sub_system_id;

    NV_STATUS initialize = NvAPI_Initialize();

    NvAPI_EnumPhysicalGPUs(gpu_handles, &gpu_count);

    for(NV_S32 gpu_idx = 0; gpu_idx < gpu_count; gpu_idx++)
    {
        i2c_smbus_nvapi * nvapi_bus = new i2c_smbus_nvapi(gpu_handles[gpu_idx]);

        sprintf(nvapi_bus->device_name, "NVidia NvAPI I2C on GPU %d", gpu_idx);

        res = NvAPI_GPU_GetPCIIdentifiers(gpu_handles[gpu_idx], &device_id, &sub_system_id, &revision_id, &ext_device_id);

        if (res == 0)
        {
            nvapi_bus->pci_device           = device_id >> 16;
            nvapi_bus->pci_vendor           = device_id & 0xffff;
            nvapi_bus->pci_subsystem_device = sub_system_id >> 16;
            nvapi_bus->pci_subsystem_vendor = sub_system_id & 0xffff;
            nvapi_bus->port_id              = 1;
        }

        busses.push_back(nvapi_bus);
    }
}   /* DetectNvAPII2CBusses() */

/******************************************************************************************\
*                                                                                          *
*   DetectAMDADLI2CBusses (Windows)                                                        *
*                                                                                          *
*       Detects available AMD ADL I2C adapters and enumerates                              *
*       i2c_smbus_interface objects for them.  Only enumerates Bus Nr 1                    *
*                                                                                          *
\******************************************************************************************/

void DetectADLI2C()
{
    int adl_status;
    int gpu_count = 0;
    ADL_CONTEXT_HANDLE gpu_handle;

    i2c_smbus_amdadl * adl_bus = new i2c_smbus_amdadl(gpu_handle);

    adl_status = adl_bus->ADL_Initialize();

    if(0 != adl_status)
    {
        printf_s("ADL Status %d \n", adl_status);
    }
    else
    {
        sprintf(adl_bus->device_name, "AMD ADL I2C on GPU %d", gpu_count);
        busses.push_back(adl_bus);
    }
}   /* DetectAMDADLI2CBusses() */

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

    // Detect Nuvoton Super IO SMBus adapters
    DetectNuvotonI2CBusses();

    // Detect NVidia NvAPI I2C adapters
    DetectNvAPII2CBusses();

    // Detect AMD ADL I2C adadpters
    DetectADLI2C();

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
    char path[1024];
    char buff[100];
    unsigned short pci_device, pci_vendor, pci_subsystem_device, pci_subsystem_vendor;
    unsigned short port_id;
    bool info;

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

                    // For now, only get PCI information from nVidia GPUs
                    // PCI IDs are not currently obtained from the Nouveau driver
                    // and GPUs using PCI IDs for detection will not work with it.
                    if (sscanf(device_string, "NVIDIA i2c adapter %hu at", &port_id) == 1)
                    {
                        info = true;

                        // Get PCI Device
                        snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/device");
                        test_fd = open(path, O_RDONLY);
                        if (test_fd < 0)
                        {
                            ent = readdir(dir);
                            continue;
                        }
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_device = strtoul(buff, NULL, 16);
                        close(test_fd);

                        // Get PCI Vendor
                        snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/vendor");
                        test_fd = open(path, O_RDONLY);
                        if (test_fd < 0)
                        {
                            ent = readdir(dir);
                            continue;
                        }
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_vendor = strtoul(buff, NULL, 16);
                        close(test_fd);

                        // Get PCI Subsystem Device
                        snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/subsystem_device");
                        test_fd = open(path, O_RDONLY);
                        if (test_fd < 0)
                        {
                            ent = readdir(dir);
                            continue;
                        }
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_subsystem_device = strtoul(buff, NULL, 16);
                        close(test_fd);

                        // Get PCI Subsystem Vendor
                        snprintf(path, sizeof(path), "%s%s%s", driver_path, ent->d_name, "/device/subsystem_vendor");
                        test_fd = open(path, O_RDONLY);
                        if (test_fd < 0)
                        {
                            ent = readdir(dir);
                            continue;
                        }
                        memset(buff, 0x00, sizeof(buff));
                        read(test_fd, buff, sizeof(buff));
                        buff[strlen(buff) - 1] = 0x00;
                        pci_subsystem_vendor = strtoul(buff, NULL, 16);
                        close(test_fd);
                    }
                    else
                    {
                        info = false;
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
                    bus->handle = test_fd;
                    if (info) {
                        bus->pci_device = pci_device;
                        bus->pci_vendor = pci_vendor;
                        bus->pci_subsystem_device = pci_subsystem_device;
                        bus->pci_subsystem_vendor = pci_subsystem_vendor;
                        bus->port_id = port_id;
                    }
                    busses.push_back(bus);
                }
            }
        }
        ent = readdir(dir);
    }

}   /* DetectI2CBusses() */

#endif  /* WIN32 */

void DetectAuraSMBusControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers);
void DetectAuraGPUControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers);
void DetectCorsairVengeanceControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers);
void DetectCorsairVengeanceProControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers);
void DetectCrucialControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers);
void DetectHyperXDRAMControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers);
void DetectPatriotViperControllers(std::vector<i2c_smbus_interface*> &busses, std::vector<RGBController*> &rgb_controllers);
void DetectPolychromeControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectRGBFusionControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectRGBFusionGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectRGBFusion2SMBusControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectRGBFusion2DRAMControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectMSIGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectSapphireGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectGalaxGPUControllers(std::vector<i2c_smbus_interface*>& busses, std::vector<RGBController*>& rgb_controllers);
void DetectMSIMysticLightControllers(std::vector<RGBController*> &rgb_controllers);
void DetectMSIRGBControllers(std::vector<RGBController*> &rgb_controllers);
void DetectAuraUSBControllers(std::vector<RGBController*> &rgb_controllers);
void DetectAuraCoreControllers(std::vector<RGBController*> &rgb_controllers);
void DetectLEDStripControllers(std::vector<RGBController*> &rgb_controllers);
void DetectNZXTHue2Controllers(std::vector<RGBController*> &rgb_controllers);
void DetectHuePlusControllers(std::vector<RGBController*> &rgb_controllers);
void DetectOpenRazerControllers(std::vector<RGBController*> &rgb_controllers);
void DetectE131Controllers(std::vector<RGBController*> &rgb_controllers);
void DetectAMDWraithPrismControllers(std::vector<RGBController*>& rgb_controllers);
void DetectCoolerMasterControllers(std::vector<RGBController*>& rgb_controllers);
void DetectMSI3ZoneControllers(std::vector<RGBController*>& rgb_controllers);
void DetectPoseidonZRGBControllers(std::vector<RGBController*>& rgb_controllers);
void DetectCorsairPeripheralControllers(std::vector<RGBController*>& rgb_controllers);
void DetectCorsairLightingNodeControllers(std::vector<RGBController*> &rgb_controllers);
void DetectFaustusControllers(std::vector<RGBController*> &rgb_controllers);
void DetectHyperXKeyboardControllers(std::vector<RGBController*>& rgb_controllers);
void DetectHyperXMouseControllers(std::vector<RGBController*>& rgb_controllers);
void DetectThermaltakeRiingControllers(std::vector<RGBController*>& rgb_controllers);
void DetectRGBFusion2USBControllers(std::vector<RGBController*> &rgb_controllers);
void DetectRedragonControllers(std::vector<RGBController*>& rgb_controllers);
void DetectLogitechControllers(std::vector<RGBController*>& rgb_controllers);
void DetectNZXTKrakenControllers(std::vector<RGBController*>& rgb_controllers);
void DetectSteelSeriesControllers(std::vector<RGBController*>& rgb_controllers);
void DetectGloriousModelOControllers(std::vector<RGBController*>& rgb_controllers);
void DetectHoltekControllers(std::vector<RGBController*>& rgb_controllers);
void DetectDuckyKeyboardControllers(std::vector<RGBController*>& rgb_controllers);
void DetectEKControllers(std::vector<RGBController*>& rgb_controllers);

/******************************************************************************************\
*                                                                                          *
*   DetectRGBConrollers                                                                    *
*                                                                                          *
*       Detect and populate RGB Controllers vector                                         *
*                                                                                          *
\******************************************************************************************/

void DetectRGBControllers(void)
{
    DetectI2CBusses();

    DetectAuraSMBusControllers(busses, rgb_controllers);
    DetectAuraGPUControllers(busses, rgb_controllers);
    DetectCorsairVengeanceControllers(busses, rgb_controllers);
    DetectCorsairVengeanceProControllers(busses, rgb_controllers);
    DetectCrucialControllers(busses, rgb_controllers);
    DetectGalaxGPUControllers(busses, rgb_controllers);
    DetectHyperXDRAMControllers(busses, rgb_controllers);
    DetectPatriotViperControllers(busses, rgb_controllers);
    DetectPolychromeControllers(busses, rgb_controllers);
    DetectRGBFusionGPUControllers(busses, rgb_controllers);
    DetectMSIGPUControllers(busses, rgb_controllers);
    DetectSapphireGPUControllers(busses, rgb_controllers);

    //TODO: Implement better detection before enabling these controllers
    //DetectRGBFusion2SMBusControllers(busses, rgb_controllers);
    //DetectRGBFusion2DRAMControllers(busses, rgb_controllers);

    DetectRGBFusionControllers(busses, rgb_controllers);
    //DetectMSIMysticLightControllers(rgb_controllers);
    DetectMSIRGBControllers(rgb_controllers);

    DetectAuraUSBControllers(rgb_controllers);
    DetectAuraCoreControllers(rgb_controllers);
    DetectLEDStripControllers(rgb_controllers);
    DetectNZXTHue2Controllers(rgb_controllers);
    DetectHuePlusControllers(rgb_controllers);

    DetectAMDWraithPrismControllers(rgb_controllers);
    DetectCoolerMasterControllers(rgb_controllers);
    DetectMSI3ZoneControllers(rgb_controllers);
    DetectPoseidonZRGBControllers(rgb_controllers);
    DetectHyperXKeyboardControllers(rgb_controllers);
    DetectHyperXMouseControllers(rgb_controllers);
    DetectCorsairPeripheralControllers(rgb_controllers);
    DetectCorsairLightingNodeControllers(rgb_controllers);
    DetectThermaltakeRiingControllers(rgb_controllers);
    DetectRGBFusion2USBControllers(rgb_controllers);
    DetectRedragonControllers(rgb_controllers);
    DetectLogitechControllers(rgb_controllers);
    DetectNZXTKrakenControllers(rgb_controllers);
    DetectSteelSeriesControllers(rgb_controllers);
    DetectGloriousModelOControllers(rgb_controllers);
    DetectHoltekControllers(rgb_controllers);
    DetectDuckyKeyboardControllers(rgb_controllers);
    DetectEKControllers(rgb_controllers);

    DetectE131Controllers(rgb_controllers);

    DetectOpenRazerControllers(rgb_controllers);

    /*-------------------------------------*\
    | Windows-only devices                  |
    \*-------------------------------------*/
#ifdef WIN32
    /*-------------------------------------*\
    | Linux-only devices                    |
    \*-------------------------------------*/
#else
    DetectFaustusControllers(rgb_controllers);
#endif

}   /* DetectRGBControllers() */

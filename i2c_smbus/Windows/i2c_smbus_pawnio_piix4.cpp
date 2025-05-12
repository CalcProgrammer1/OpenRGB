/*---------------------------------------------------------*\
| i2c_smbus_pawnio_piix4.cpp                                |
|                                                           |
|   PawnIO PIIX4 SMBUS driver for Windows                   |
|                                                           |
|   Stephen Horvath (Steve-Tech)                04 May 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "Detector.h"
#include "i2c_smbus_pawnio.h"
#include "LogManager.h"
#ifdef _WIN32
#include "PawnIOLib.h"
#include "wmi.h"
#endif
#include "ResourceManager.h"

#ifdef _WIN32
bool i2c_smbus_piix4_pawnio_detect()
{
    ULONG dll_version;
    if(pawnio_version(&dll_version))
    {
        LOG_INFO("PawnIO is not loaded, PawnIO piix4 I2C bus detection aborted");
        return(false);
    }

    i2c_smbus_interface * bus;
    HRESULT hres;
    Wmi wmi;
    HANDLE pawnio_handle;

    // Query WMI for Win32_PnPSignedDriver entries with names matching "SMBUS" or "SM BUS"
    // These devices may be browsed under Device Manager -> System Devices
    std::vector<QueryObj> q_res_PnPSignedDriver;
    hres = wmi.query("SELECT * FROM Win32_PnPSignedDriver WHERE Description LIKE '%SMBUS%' OR Description LIKE '%SM BUS%'", q_res_PnPSignedDriver);

    if(hres)
    {
        LOG_INFO("WMI query failed, PawnIO piix4 I2C bus detection aborted");
        return(false);
    }

    // For each detected SMBus adapter, try enumerating it as either AMD or Intel
    for (QueryObj &i : q_res_PnPSignedDriver)
    {
        // AMD SMBus adapters use the PIIX4 driver
        if(i["Manufacturer"].find("Advanced Micro Devices, Inc") != std::string::npos)
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

            if(i2c_smbus_pawnio::start_pawnio("SmbusPIIX4.bin", &pawnio_handle) != S_OK)
            {
                return(false);
            }

            bus                         = new i2c_smbus_pawnio(pawnio_handle, "piix4", 0);
            bus->pci_vendor             = ven_id;
            bus->pci_device             = dev_id;
            bus->pci_subsystem_vendor   = sbv_id;
            bus->pci_subsystem_device   = sbd_id;
            strncpy(bus->device_name, i["Description"].c_str(), sizeof bus->device_name);
            strncat(bus->device_name, " port 0", sizeof bus->device_name);
            ResourceManager::get()->RegisterI2CBus(bus);

            bus                         = new i2c_smbus_pawnio(pawnio_handle, "piix4", 1);
            bus->pci_vendor             = ven_id;
            bus->pci_device             = dev_id;
            bus->pci_subsystem_vendor   = sbv_id;
            bus->pci_subsystem_device   = sbd_id;
            strncpy(bus->device_name, i["Description"].c_str(), sizeof bus->device_name);
            strncat(bus->device_name, " port 1", sizeof bus->device_name);
            ResourceManager::get()->RegisterI2CBus(bus);
        }
    }

    return(true);
}
#endif

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_piix4_pawnio_detect);

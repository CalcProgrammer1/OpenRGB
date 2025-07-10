/*---------------------------------------------------------*\
| i2c_smbus_amdadl.cpp                                      |
|                                                           |
|   Definitions and types for AMD ADL I2C functions         |
|                                                           |
|   Niels Westphal (crashniels)                 30 May 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <string>
#include "Detector.h"
#include "i2c_smbus_amdadl.h"
#include "LogManager.h"
#include "pci_ids.h"

typedef int ( *ADL2_MAIN_CONTROL_CREATE )(ADL_MAIN_MALLOC_CALLBACK, int, ADL_CONTEXT_HANDLE*);
typedef int ( *ADL2_MAIN_CONTROL_DESTROY )(ADL_CONTEXT_HANDLE);
typedef int ( *ADL2_ADAPTER_NUMBEROFADAPTERS_GET ) ( ADL_CONTEXT_HANDLE , int* );
typedef int ( *ADL2_ADAPTER_PRIMARY_GET) (ADL_CONTEXT_HANDLE, int* lpPrimaryAdapterIndex);
typedef int ( *ADL2_ADAPTER_ADAPTERINFOX2_GET) (ADL_CONTEXT_HANDLE, AdapterInfo**);
typedef int ( *ADL2_ADAPTER_ADAPTERINFOX4_GET) (ADL_CONTEXT_HANDLE, int iAdapterIndex, int* numAdapters, AdapterInfoX2** lppAdapterInfoX2);
typedef int ( *ADL2_DISPLAY_WRITEANDREADI2C) (ADL_CONTEXT_HANDLE, int iAdapterIndex, ADLI2C* plI2C);

ADL2_MAIN_CONTROL_CREATE          ADL2_Main_Control_Create;
ADL2_MAIN_CONTROL_DESTROY         ADL2_Main_Control_Destroy;
ADL2_ADAPTER_NUMBEROFADAPTERS_GET ADL2_Adapter_NumberOfAdapters_Get;
ADL2_ADAPTER_PRIMARY_GET          ADL2_Adapter_Primary_Get;
ADL2_ADAPTER_ADAPTERINFOX2_GET    ADL2_Adapter_AdapterInfoX2_Get;
ADL2_ADAPTER_ADAPTERINFOX4_GET    ADL2_Adapter_AdapterInfoX4_Get;
ADL2_DISPLAY_WRITEANDREADI2C      ADL2_Display_WriteAndReadI2C;

int LoadLibraries()
{
    HINSTANCE hDLL;

    hDLL = LoadLibrary("atiadlxx.dll");

    if(hDLL == NULL)
    {
        /*---------------------------------------------------------------------*\
        | A 32 bit calling application on 64 bit OS will fail to LoadLibrary.   |
        | Try to load the 32 bit library (atiadlxy.dll) instead                 |
        \*---------------------------------------------------------------------*/
        hDLL = LoadLibrary("atiadlxy.dll");
        return ADL_ERR;
    }
    else
    {
        ADL2_Main_Control_Create            = (ADL2_MAIN_CONTROL_CREATE)GetProcAddress(hDLL, "ADL2_Main_Control_Create");
        ADL2_Main_Control_Destroy           = (ADL2_MAIN_CONTROL_DESTROY)GetProcAddress(hDLL, "ADL2_Main_Control_Destroy");
        ADL2_Adapter_NumberOfAdapters_Get   = (ADL2_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(hDLL, "ADL2_Adapter_NumberOfAdapters_Get");
        ADL2_Adapter_Primary_Get            = (ADL2_ADAPTER_PRIMARY_GET)GetProcAddress(hDLL, "ADL2_Adapter_Primary_Get");
        ADL2_Adapter_AdapterInfoX2_Get      = (ADL2_ADAPTER_ADAPTERINFOX2_GET)GetProcAddress(hDLL, "ADL2_Adapter_AdapterInfoX2_Get");
        ADL2_Adapter_AdapterInfoX4_Get      = (ADL2_ADAPTER_ADAPTERINFOX4_GET)GetProcAddress(hDLL, "ADL2_Adapter_AdapterInfoX4_Get");
        ADL2_Display_WriteAndReadI2C        = (ADL2_DISPLAY_WRITEANDREADI2C)GetProcAddress(hDLL, "ADL2_Display_WriteAndReadI2C");

        /*---------------------------------------------------------------------*\
        | Only return OK if all function pointers are valid                     |
        \*---------------------------------------------------------------------*/
        if( ADL2_Main_Control_Create
         && ADL2_Main_Control_Destroy
         && ADL2_Adapter_NumberOfAdapters_Get
         && ADL2_Adapter_Primary_Get
         && ADL2_Adapter_AdapterInfoX2_Get
         && ADL2_Adapter_AdapterInfoX4_Get
         && ADL2_Display_WriteAndReadI2C)
        {
            return ADL_OK;
        }
        else
        {
            return ADL_ERR;
        }
    }
}

// Memory allocation function
void* __stdcall ADL_Main_Memory_Alloc ( int iSize )
{
    void* lpBuffer = malloc ( iSize );
    return lpBuffer;
}

// Optional Memory de-allocation function
void __stdcall ADL_Main_Memory_Free ( void* lpBuffer )
{
    if ( NULL != lpBuffer )
    {
        free ( lpBuffer );
        lpBuffer = NULL;
    }
}

i2c_smbus_amdadl::i2c_smbus_amdadl(ADL_CONTEXT_HANDLE context, int adapter_index)
{
    int num_of_devices;
    AdapterInfoX2* info;

    this->context = context;

    if (ADL_OK != ADL2_Adapter_AdapterInfoX4_Get(context, adapter_index, &num_of_devices, &info))
    {
        printf("Cannot get Adapter Info!\n");
    }
    else
    {
        std::string pnp_str = info->strPNPString;

        std::size_t ven_loc = pnp_str.find("VEN_");
        std::size_t dev_loc = pnp_str.find("DEV_");
        std::size_t sub_loc = pnp_str.find("SUBSYS_");

        if((ven_loc != std::string::npos) && (dev_loc != std::string::npos) && (sub_loc != std::string::npos))
        {
            std::string ven_str = pnp_str.substr(ven_loc + 4, 4);
            std::string dev_str = pnp_str.substr(dev_loc + 4, 4);
            std::string sbv_str = pnp_str.substr(sub_loc + 11, 4);
            std::string sbd_str = pnp_str.substr(sub_loc + 7, 4);

            int ven_id = (int)std::stoul(ven_str, nullptr, 16);
            int dev_id = (int)std::stoul(dev_str, nullptr, 16);
            int sbv_id = (int)std::stoul(sbv_str, nullptr, 16);
            int sbd_id = (int)std::stoul(sbd_str, nullptr, 16);

            this->pci_vendor           = ven_id;
            this->pci_device           = dev_id;
            this->pci_subsystem_vendor = sbv_id;
            this->pci_subsystem_device = sbd_id;
            this->port_id              = 1;
            strcpy(this->device_name, "AMD ADL");
        }
    }
}

s32 i2c_smbus_amdadl::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    int PrimaryDisplay;
    int ret;
    int data_size = 0;
    char* data_ptr;

    ADLI2C* pI2C;
    ADLI2C I2Cstore;
    pI2C = &I2Cstore;

    char i2c_buf[I2C_SMBUS_BLOCK_MAX + 8];

    pI2C->iSize = sizeof(ADLI2C);
    pI2C->iSpeed = 100;
    pI2C->iLine = 1; //location of the Aura chip
    pI2C->iAddress = addr << 1;
    pI2C->iOffset = 0;
    pI2C->pcData = (char*)data;

    if (ADL_OK != ADL2_Adapter_Primary_Get(context, &PrimaryDisplay))
    {
        printf("Cannot get Display!\n");
        return ADL_ERR;
    }

    switch (size)
    {
    case I2C_SMBUS_QUICK:
        //garbo data
        //pI2C->iDataSize = 0;
        return -1;
        break;

    case I2C_SMBUS_BYTE:
        //pI2C->iDataSize = 1;
        //break;

    case I2C_SMBUS_BYTE_DATA:
        data_size = 1;
        data_ptr = (char*)data;
        break;

    case I2C_SMBUS_WORD_DATA:
        data_size = 2;
        data_ptr = (char*)data;
        break;

    case I2C_SMBUS_BLOCK_DATA:
        data_size = data->block[0] + 1;
        data_ptr = (char*)&data->block[0];
        break;

    case I2C_SMBUS_I2C_BLOCK_DATA:
        data_size = data->block[0];
        data_ptr = (char*)&data->block[1];
        break;

    default:
        return -1;
    }

    if (read_write == I2C_SMBUS_READ)
    {
        /* An SMBus read can be achieved by setting the offset to the command (register address) */
        pI2C->iOffset = command;
        pI2C->iAction = ADL_DL_I2C_ACTIONREAD;
        pI2C->iDataSize = data_size;
        pI2C->pcData = (char *)data_ptr;

        ret = ADL2_Display_WriteAndReadI2C(context, PrimaryDisplay, pI2C);
    }
    else
    {
        /* An SMBus write has one extra byte, the register address, before the data */
        pI2C->iAction = ADL_DL_I2C_ACTIONWRITE;
        pI2C->iDataSize = data_size + 1;
        pI2C->pcData = i2c_buf;

        i2c_buf[0] = command;
        memcpy(&i2c_buf[1], data_ptr, data_size);

        ret = ADL2_Display_WriteAndReadI2C(context, PrimaryDisplay, pI2C);
    }

    return(ret);
};

s32 i2c_smbus_amdadl::i2c_xfer(u8 addr, char read_write, int* size, u8* data)
{
    int PrimaryDisplay;
    int ret;
    int data_size = *size;

    ADLI2C* pI2C;
    ADLI2C I2Cstore;
    pI2C = &I2Cstore;

    char i2c_buf[I2C_SMBUS_BLOCK_MAX];

    pI2C->iSize = sizeof(ADLI2C);
    pI2C->iSpeed = 100;
    pI2C->iLine = 1; //location of the Aura chip
    pI2C->iAddress = addr << 1;
    pI2C->iOffset = 0;
    pI2C->iDataSize = data_size;

    if (ADL_OK != ADL2_Adapter_Primary_Get(context, &PrimaryDisplay))
    {
        printf("Cannot get Display!\n");
        return ADL_ERR;
    }

    if (read_write == I2C_SMBUS_READ)
    {
        pI2C->iAction = ADL_DL_I2C_ACTIONREAD;
        pI2C->pcData = (char*)data;
        ret = ADL2_Display_WriteAndReadI2C(context, PrimaryDisplay, pI2C);
    }
    else
    {
        pI2C->iAction = ADL_DL_I2C_ACTIONWRITE;
        pI2C->pcData = i2c_buf;

        memcpy(i2c_buf, data, data_size);

        ret = ADL2_Display_WriteAndReadI2C(context, PrimaryDisplay, pI2C);
    }

    return(ret);
}

bool i2c_smbus_amdadl_detect()
{
    ADL_CONTEXT_HANDLE context;

    if(ADL_OK == LoadLibraries())
    {
        if (ADL_OK != ADL2_Main_Control_Create(::ADL_Main_Memory_Alloc, 1, &context))
        {
            printf("Cannot get handle!\n");
            return(false);
        }
        else
        {
            int num_of_devices;
            AdapterInfoX2* info;
            if (ADL_OK == ADL2_Adapter_AdapterInfoX4_Get(context, -1, &num_of_devices, &info))
            {
                int last_bus_number = -1;
                for(int i = 0; i < num_of_devices; i++)
                {
                    AdapterInfoX2 current = *(info + i);
                    if(last_bus_number == current.iBusNumber)
                    {
                        continue;
                    }
                    last_bus_number = current.iBusNumber;
                    i2c_smbus_amdadl * adl_bus = new i2c_smbus_amdadl(context, current.iAdapterIndex);

                    if(adl_bus->pci_vendor != AMD_GPU_VEN)
                    {
                        delete adl_bus;
                        continue;
                    }

                    LOG_INFO("ADL GPU Device %04X:%04X Subsystem: %04X:%04X", adl_bus->pci_vendor, adl_bus->pci_device,adl_bus->pci_subsystem_vendor,adl_bus->pci_subsystem_device);
                    ResourceManager::get()->RegisterI2CBus(adl_bus);
                }
            }
        }
    }

    return(true);
}   /* DetectAMDADLI2CBusses() */

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_amdadl_detect);

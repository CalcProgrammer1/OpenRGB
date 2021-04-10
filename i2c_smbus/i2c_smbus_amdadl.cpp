/*-----------------------------------------*\
|  i2c_smbus_amdadl.cpp                     |
|                                           |
|  Definitions and types for AMD ADL I2C    |
|  functions                                |
|                                           |
|  Niels Westphal (crashniels) 05/30/2020   |
\*-----------------------------------------*/

#include "i2c_smbus_amdadl.h"
#include <string>

typedef int ( *ADL2_MAIN_CONTROL_CREATE )(ADL_MAIN_MALLOC_CALLBACK, int, ADL_CONTEXT_HANDLE*);
typedef int ( *ADL2_MAIN_CONTROL_DESTROY )(ADL_CONTEXT_HANDLE);
typedef int ( *ADL2_ADAPTER_NUMBEROFADAPTERS_GET ) ( ADL_CONTEXT_HANDLE , int* );
typedef int ( *ADL2_ADAPTER_PRIMARY_GET) (ADL_CONTEXT_HANDLE, int* lpPrimaryAdapterIndex);
typedef int ( *ADL2_ADAPTER_ADAPTERINFOX2_GET) (ADL_CONTEXT_HANDLE, AdapterInfo**);
typedef int ( *ADL2_DISPLAY_WRITEANDREADI2C) (ADL_CONTEXT_HANDLE, int iAdapterIndex, ADLI2C* plI2C);

ADL2_MAIN_CONTROL_CREATE          ADL2_Main_Control_Create;
ADL2_MAIN_CONTROL_DESTROY         ADL2_Main_Control_Destroy;
ADL2_ADAPTER_NUMBEROFADAPTERS_GET ADL2_Adapter_NumberOfAdapters_Get;
ADL2_ADAPTER_PRIMARY_GET          ADL2_Adapter_Primary_Get;
ADL2_ADAPTER_ADAPTERINFOX2_GET    ADL2_Adapter_AdapterInfoX2_Get;
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
        ADL2_Display_WriteAndReadI2C        = (ADL2_DISPLAY_WRITEANDREADI2C)GetProcAddress(hDLL, "ADL2_Display_WriteAndReadI2C");

        /*---------------------------------------------------------------------*\
        | Only return OK if all function pointers are valid                     |
        \*---------------------------------------------------------------------*/
        if( ADL2_Main_Control_Create 
         && ADL2_Main_Control_Destroy
         && ADL2_Adapter_NumberOfAdapters_Get
         && ADL2_Adapter_Primary_Get
         && ADL2_Adapter_AdapterInfoX2_Get
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

i2c_smbus_amdadl::i2c_smbus_amdadl(ADL_CONTEXT_HANDLE context)
{
    AdapterInfo * info;

    this->context = context;

    if (ADL_OK != ADL2_Adapter_AdapterInfoX2_Get(context, &info))
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
            strcpy(this->device_name, "AMD ADL");
        }
    }
}

s32 i2c_smbus_amdadl::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    int PrimaryDisplay;
    int ret;

    ADLI2C* pI2C;
    ADLI2C I2Cstore;
    pI2C = &I2Cstore;

    pI2C->iSize = sizeof(ADLI2C);
    pI2C->iSpeed = 100;
    pI2C->iLine = 1; //location of the Aura chip
    pI2C->iAddress = addr << 1;
    pI2C->iOffset = command;
    pI2C->pcData = (char*)data;


    if (ADL_OK != ADL2_Main_Control_Create(::ADL_Main_Memory_Alloc, 1, &context))
    {
        printf("Cannot get handle!\n");
        return ADL_ERR;
    }

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
        pI2C->iDataSize = 1;
        break;

    case I2C_SMBUS_WORD_DATA:
        return -1;
        break;

    case I2C_SMBUS_BLOCK_DATA:
        return -1;
        break;

    default:
        return -1;
    }

    if (read_write == I2C_SMBUS_READ)
    {
        pI2C->iAction = ADL_DL_I2C_ACTIONREAD;
        ret = ADL2_Display_WriteAndReadI2C(context, PrimaryDisplay, pI2C);
    }
    else
    {
        pI2C->iAction = ADL_DL_I2C_ACTIONWRITE;
        ret = ADL2_Display_WriteAndReadI2C(context, PrimaryDisplay, pI2C);
    }

    return (ret);
};

#include "Detector.h"

void i2c_smbus_amdadl_detect()
{
    int adl_status;
    int gpu_count = 0;
    ADL_CONTEXT_HANDLE context;

    if(ADL_OK == LoadLibraries())
    {
        if (ADL_OK != ADL2_Main_Control_Create(::ADL_Main_Memory_Alloc, 1, &context))
        {
            printf("Cannot get handle!\n");
        }
        else
        {
            i2c_smbus_amdadl * adl_bus = new i2c_smbus_amdadl(context);
            ResourceManager::get()->RegisterI2CBus(adl_bus);
        }
    }
}   /* DetectAMDADLI2CBusses() */

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_amdadl_detect);

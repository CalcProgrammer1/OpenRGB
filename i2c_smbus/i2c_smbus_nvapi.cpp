/*-----------------------------------------*\
|  i2c_smbus_nvapi.cpp                      |
|                                           |
|  NVidia NvAPI I2C driver for Windows      |
|                                           |
|  Adam Honse (CalcProgrammer1) 2/21/2020   |
\*-----------------------------------------*/

#include "i2c_smbus_nvapi.h"

i2c_smbus_nvapi::i2c_smbus_nvapi(NV_PHYSICAL_GPU_HANDLE handle)
{
    this->handle = handle;
}

s32 i2c_smbus_nvapi::i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data)
{
    NV_STATUS ret;
    unsigned int unknown = 0;
    NV_I2C_INFO_V3 i2c_data;
    uint8_t data_buf[8];
    uint8_t chip_addr;
	
    // Set up chip register address to command, one byte in length
    chip_addr = command;
    i2c_data.i2c_reg_address = &chip_addr;
    i2c_data.reg_addr_size = 1;

    // Set up data buffer, zero bytes in length
	i2c_data.data = data_buf;
	i2c_data.size = 0;

    // Always use GPU port 1 - this is where RGB controllers are attached
	i2c_data.is_ddc_port = 0;
    i2c_data.port_id = 1;
	i2c_data.is_port_id_set = 1;

    // Use default speed
	i2c_data.i2c_speed = 0xFFFF;
	i2c_data.i2c_speed_khz = NV_I2C_SPEED::NVAPI_I2C_SPEED_DEFAULT;

    // Load device address
    i2c_data.i2c_dev_address = (addr << 1);

	switch (size)
	{
    case I2C_SMBUS_BYTE:
        // One byte of data with no register address
        i2c_data.reg_addr_size = 0;
        data_buf[0] = command;
        i2c_data.size = 1;
        break;

    case I2C_SMBUS_BYTE_DATA:
        // One byte of data with one byte of register address
        data_buf[0] = data->byte;
        i2c_data.size = 1;
        break;

    case I2C_SMBUS_WORD_DATA:
        // One word of data with one byte of register address
        data_buf[0] = (data->word & 0x00ff);
        data_buf[1] = (data->word & 0xff00) >> 8;
        i2c_data.size = 2;
        break;

    // Not supported
    case I2C_SMBUS_QUICK:
    case I2C_SMBUS_BLOCK_DATA:
        return -1;
        break;

	default:
        return -1;
    }

    // Perform read or write
    if(read_write == I2C_SMBUS_WRITE)
    {
        ret = NvAPI_I2CWriteEx(handle, &i2c_data, &unknown);
    }
    else
    {
        ret = NvAPI_I2CReadEx(handle, &i2c_data, &unknown);

        if(i2c_data.size == 1)
        {
            data->byte = i2c_data.data[0];
        }
        else
        {
            data->word = (i2c_data.data[0] | (i2c_data.data[1] << 8));
        }
    }
    
    return(ret);
}

#include "Detector.h"

void i2c_smbus_nvapi_detect()
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

        ResourceManager::get()->RegisterI2CBus(nvapi_bus);
    }
}   /* DetectNvAPII2CBusses() */

REGISTER_I2C_BUS_DETECTOR(i2c_smbus_nvapi_detect);

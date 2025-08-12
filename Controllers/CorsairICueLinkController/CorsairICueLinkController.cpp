/*---------------------------------------------------------*\
| CorsairICueLinkController.cpp                             |
|                                                           |
|   Driver for Corsair iCue Link System Hub                 |
|                                                           |
|   Aiden Vigue (acvigue)                       02 Mar 2025 |
|   Adam Honse <calcprogrammer1@gmail.com>      01 Aug 2025 |
|   Nikola Jurkovic (jurkovic.nikola)           11 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include <LogManager.h>

#include "CorsairDeviceGuard.h"
#include "CorsairICueLinkController.h"
#include "CorsairICueLinkProtocol.h"

using namespace std::chrono_literals;

CorsairICueLinkController::CorsairICueLinkController(hid_device* dev_handle, const char* path, std::string name)
{
    dev                     = dev_handle;
    location                = path;
    this->name              = name;

    guard_manager_ptr       = new DeviceGuardManager(new CorsairDeviceGuard());

    GetControllerFirmware();        // Firmware
    SetControllerSoftwareMode();    // Software mode
    GetControllerDevices();         // Get connected devices
}

CorsairICueLinkController::~CorsairICueLinkController()
{
    SetControllerHardwareMode();    // Release device back to hardware mode
    hid_close(dev);
    delete guard_manager_ptr;
}

void CorsairICueLinkController::SetControllerSoftwareMode()
{
    SendCommand(CORSAIR_ICUE_LINK_CMD_SOFTWARE_MODE, { }, { });
}

void CorsairICueLinkController::SetControllerHardwareMode()
{
    SendCommand(CORSAIR_ICUE_LINK_CMD_HARDWARE_MODE, { }, { });
}

void CorsairICueLinkController::GetControllerFirmware()
{
    /*-----------------------------------------------------*\
    | Get the firmware version                              |
    \*-----------------------------------------------------*/
    std::vector<unsigned char> firmware_data    = SendCommand(CORSAIR_ICUE_LINK_CMD_GET_FIRMWARE, { }, { });
    version = 
    {
        firmware_data[4],
        firmware_data[5],
        static_cast<unsigned short>(firmware_data[6] | (firmware_data[7] << 8))
    };
}

void CorsairICueLinkController::GetControllerDevices()
{
    /*-----------------------------------------------------*\
    | Get the endpoints data                                |
    \*-----------------------------------------------------*/
    std::vector<unsigned char>  endpoint_data           = Read(CORSAIR_ICUE_LINK_MODE_GET_DEVICES, CORSAIR_ICUE_LINK_DATA_TYPE_GET_DEVICES);
    unsigned char               channel                 = endpoint_data[6];
    std::vector<unsigned char>  index                   = std::vector<unsigned char>(endpoint_data.begin() + 7, endpoint_data.end());
    std::size_t                 pos                     = 0;

    /*-----------------------------------------------------*\
    | Process each channel                                  |
    \*-----------------------------------------------------*/
    for(std::size_t channel_idx = 1; channel_idx < (std::size_t)(channel + 1); channel_idx++)
    {
        std::size_t device_id_length = index[pos + 7];

        if(device_id_length == 0)
        {
            pos += 8;
            continue;
        }

        /*-------------------------------------------------*\
        | Extract endpoint metadata and ID from data        |
        \*-------------------------------------------------*/
        std::vector<unsigned char> endpoint_metadata    = std::vector<unsigned char>(index.begin() + pos, index.begin() + pos + 8);
        std::vector<unsigned char> endpoint_id          = std::vector<unsigned char>(index.begin() + pos + 8, index.begin() + pos + 8 + device_id_length);

        /*-------------------------------------------------*\
        | Get device information for this endpoint          |
        \*-------------------------------------------------*/
        unsigned char type                              = endpoint_metadata[2];
        unsigned char model                             = endpoint_metadata[3];
        const CorsairICueLinkDevice * device            = FindCorsairICueLinkDevice(type, model);

        if(device == nullptr)
        {
            pos += 8 + device_id_length;
            LOG_WARNING("[CorsairICueLinkController] Unknown device type: 0x%02x, model: 0x%02x", type, model);
            continue;
        }

        /*-------------------------------------------------*\
        | Dont process internal device due to duplication   |
        \*-------------------------------------------------*/
        if(device->internal == true)
        {
            pos += 8 + device_id_length;
            continue;
        }

        if(device->led_channels == 0)
        {
            LOG_WARNING("[CorsairICueLinkController] Device type %s has 0 LEDs, please open issue", device->display_name.c_str());
            pos += 8 + device_id_length;
            continue;
        }

        /*-------------------------------------------------*\
        | Append this endpoint's serial number to the       |
        | device's serial number string                     |
        \*-------------------------------------------------*/
        std::string endpoint_id_str(endpoint_id.begin(), endpoint_id.end());
        serial += "\r\n" + endpoint_id_str; // Why do we need endpoint IDs when colors are managed via endpoint channel IDs ?

        /*-------------------------------------------------*\
        | Add endpoint device to list                       |
        \*-------------------------------------------------*/
        endpoints.push_back(device);

        pos += 8 + device_id_length;
    }
}

std::string CorsairICueLinkController::GetFirmwareString()
{
    char buffer[20];
    std::snprintf(buffer, sizeof(buffer), "v%d.%d.%d", version[0], version[1], version[2]);
    return std::string(buffer);
}

std::string CorsairICueLinkController::GetNameString()
{
    return(name);
}

std::string CorsairICueLinkController::GetLocationString()
{
    return("HID: " + location);
}

std::string CorsairICueLinkController::GetSerialString()
{
    return(serial);
}

std::vector<const CorsairICueLinkDevice *> CorsairICueLinkController::GetEndpoints()
{
    return(endpoints);
}

void CorsairICueLinkController::UpdateLights(RGBColor* colors, std::size_t num_colors)
{
    /*-------------------------------------------------*\
    | Send color buffer, packed RGBRGBRGB               |
    \*-------------------------------------------------*/
    std::vector<unsigned char> color_data;
    for(std::size_t i = 0; i < num_colors; i++)
    {
        color_data.push_back(RGBGetRValue(colors[i]));
        color_data.push_back(RGBGetGValue(colors[i]));
        color_data.push_back(RGBGetBValue(colors[i]));
    }

    Write(CORSAIR_ICUE_LINK_MODE_SET_COLOR, CORSAIR_ICUE_LINK_DATA_TYPE_SET_COLOR, color_data, CORSAIR_ICUE_ENDPOINT_TYPE_COLOR);
}

std::vector<std::vector<unsigned char>> CorsairICueLinkController::ProcessMultiChunkPacket(const std::vector<unsigned char>& data, size_t max_chunk_size)
{
    std::vector<std::vector<unsigned char>> result;
    size_t offset = 0;

    while(offset < data.size())
    {
        size_t end = std::min(max_chunk_size, data.size() - offset);
        std::vector<unsigned char> chunk(data.begin() + offset, data.begin() + offset + end);
        result.push_back(chunk);
        offset += end;
    }

    return result;
}

std::vector<unsigned char> CorsairICueLinkController::SendCommand(std::vector<unsigned char> command, std::vector<unsigned char> data, std::vector<unsigned char> waitForDataType)
{
    DeviceGuardLock lock = guard_manager_ptr->AwaitExclusiveAccess();
    
    std::vector<unsigned char> write_buf(CORSAIR_ICUE_LINK_BUFFER_WRITE_LENGTH);
    write_buf[2] = 0x01;

    size_t command_size = command.size();
    size_t data_size = data.size();

    for(size_t i = 0; i < command_size; i++)
    {
        write_buf[3 + i] = command[i];
    }

    for(size_t i = 0; i < data_size; i++)
    {
        write_buf[3 + command_size + i] = data[i];
    }

    std::vector<unsigned char> read_buf(CORSAIR_ICUE_LINK_BUFFER_READ_LENGTH);

    hid_write(dev, write_buf.data(), CORSAIR_ICUE_LINK_BUFFER_WRITE_LENGTH);
    hid_read_timeout(dev, read_buf.data(), CORSAIR_ICUE_LINK_BUFFER_READ_LENGTH, 1000);

    if(waitForDataType.size() != 2)
    {
        return read_buf;
    }

    int tries = 0;
    while((read_buf[4] != waitForDataType[0]) && tries < 5)
    {
        std::fill(read_buf.begin(), read_buf.end(), 0);  // Clear the buffer before reading again
        hid_read_timeout(dev, read_buf.data(), CORSAIR_ICUE_LINK_BUFFER_READ_LENGTH, 1000);
        tries++;
    }

    return read_buf;
}

std::vector<unsigned char> CorsairICueLinkController::Read(std::vector<unsigned char> endpoint, std::vector<unsigned char> data_type)
{
    /*-----------------------------------------------------*\
    | Private function to read data from an endpoint        |
    \*-----------------------------------------------------*/
    DeviceGuardLock lock = guard_manager_ptr->AwaitExclusiveAccess();

    SendCommand(CORSAIR_ICUE_LINK_CMD_CLOSE_ENDPOINT, endpoint, { });
    SendCommand(CORSAIR_ICUE_LINK_CMD_OPEN_ENDPOINT, endpoint, { });
    std::vector<unsigned char> res = SendCommand(CORSAIR_ICUE_LINK_CMD_READ, { }, data_type);
    SendCommand(CORSAIR_ICUE_LINK_CMD_CLOSE_ENDPOINT, endpoint, { });

    return res;
}

void CorsairICueLinkController::Write(std::vector<unsigned char> endpoint, std::vector<unsigned char> data_type, std::vector<unsigned char> data, CORSAIR_ICUE_ENDPOINT_TYPE endpoint_type)
{
    DeviceGuardLock lock = guard_manager_ptr->AwaitExclusiveAccess();

    std::vector<unsigned char> buf(data_type.size() + data.size() + CORSAIR_ICUE_LINK_WRITE_HEADER_SIZE);

    unsigned short data_len = (unsigned short)(data.size() + 2);
    buf[0]                  = (unsigned char)(data_len & 0xFF);
    buf[1]                  = (unsigned char)((data_len >> 8) & 0xFF);

    /*-----------------------------------------------------*\
    | Pack data into next bytes                             |
    \*-----------------------------------------------------*/
    for(size_t i = 0; i < data_type.size(); i++)
    {
        buf[CORSAIR_ICUE_LINK_WRITE_HEADER_SIZE + i] = data_type[i];
    }

    /*-----------------------------------------------------*\
    | Pack data into next bytes                             |
    \*-----------------------------------------------------*/
    for(size_t i = 0; i < data.size(); i++)
    {
        buf[CORSAIR_ICUE_LINK_WRITE_HEADER_SIZE + data_type.size() + i] = data[i];
    }

    SendCommand(CORSAIR_ICUE_LINK_CMD_CLOSE_ENDPOINT, endpoint, { });

    if(endpoint_type == CORSAIR_ICUE_ENDPOINT_TYPE_DEFAULT)
    {
        SendCommand(CORSAIR_ICUE_LINK_CMD_OPEN_ENDPOINT, endpoint, { });
        SendCommand(CORSAIR_ICUE_LINK_CMD_WRITE, buf, { });
        SendCommand(CORSAIR_ICUE_LINK_CMD_CLOSE_ENDPOINT, endpoint, { });
        return;
    }

    SendCommand(CORSAIR_ICUE_LINK_CMD_OPEN_COLOR_ENDPOINT, endpoint, { });
    std::vector<std::vector<unsigned char>> chunks = ProcessMultiChunkPacket(buf, CORSAIR_ICUE_LINK_MAXIMUM_BUFFER_PER_REQUEST);

    for(size_t i = 0; i < chunks.size(); i++)
    {
        if(i == 0)
        {
            /*-----------------------------------------------------*\
            | Initial color packet                                  |
            \*-----------------------------------------------------*/
            SendCommand(CORSAIR_ICUE_LINK_CMD_WRITE_COLOR, chunks[i], {});
        }
        else
        {
            /*-----------------------------------------------------*\
            | Everything else follows 0x07, 0x00                    |
            \*-----------------------------------------------------*/
            SendCommand(CORSAIR_ICUE_LINK_CMD_WRITE_COLOR_NEXT, chunks[i], {});
        }
    }

    SendCommand(CORSAIR_ICUE_LINK_CMD_CLOSE_ENDPOINT, endpoint, { });
}

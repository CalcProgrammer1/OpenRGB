/*---------------------------------------------------------*\
| CorsairDRAMController.cpp                                 |
|                                                           |
|   Driver for Corsair DRAM RGB controllers                 |
|                                                           |
|   Adam Honse (CalcProgrammer1)                30 Jun 2019 |
|   Erik Gilling (konkers)                      25 Sep 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include "CRC.h"
#include "CorsairDRAMController.h"
#include "LogManager.h"

#define CORSAIR_DRAM_NAME "Corsair DRAM"

using namespace std::chrono_literals;

CorsairDRAMController::CorsairDRAMController(i2c_smbus_interface *bus, corsair_dev_id dev)
{
    /*-----------------------------------------------------*\
    | Initialize class variables                            |
    \*-----------------------------------------------------*/
    this->bus           = bus;
    this->dev           = dev;
    device_index        = 0;
    pid                 = 0;
    vid                 = 0;
    protocol_version    = 0;

    /*-----------------------------------------------------*\
    | Read device information                               |
    \*-----------------------------------------------------*/
    ReadDeviceInfo();
}

CorsairDRAMController::~CorsairDRAMController()
{
}

unsigned int CorsairDRAMController::GetLEDCount()
{
    return(corsair_dram_device_list[device_index]->led_count);
}

unsigned char CorsairDRAMController::GetProtocolVersion()
{
    return(protocol_version);
}

std::string CorsairDRAMController::GetDeviceLocation()
{
    std::string return_string(bus->device_name);
    char addr[5];
    snprintf(addr, 5, "0x%02X", dev);
    return_string.append(", address ");
    return_string.append(addr);
    return("I2C: " + return_string);
}

std::string CorsairDRAMController::GetDeviceName()
{
    return(corsair_dram_device_list[device_index]->name);
}

std::string CorsairDRAMController::GetDeviceVersion()
{
    return(firmware_version);
}

void CorsairDRAMController::SetColorsPerLED(RGBColor* colors)
{
    /*-----------------------------------------------------*\
    | Get LED count from device list                        |
    \*-----------------------------------------------------*/
    unsigned int        led_count           = corsair_dram_device_list[device_index]->led_count;

    if(direct_mode)
    {
        /*-------------------------------------------------*\
        | Sanity check - Direct mode can only be used on    |
        | protocol 4+                                       |
        \*-------------------------------------------------*/
        if(protocol_version < 4)
        {
            LOG_ERROR("[%s] Protocol version %d tried to use direct mode, ignoring", CORSAIR_DRAM_NAME, protocol_version);
            return;
        }

        /*-------------------------------------------------*\
        | Packet format:                                    |
        |   Size n is (LED count * 3) + 2                   |
        |   0:          Command byte (0x0A or 0x0C)         |
        |   1 to (n-2): LED color data in R/G/B order       |
        |   (n-1):      CRC8 of bytes 0 to (n-2)            |
        \*-------------------------------------------------*/
        unsigned int    direct_packet_size  = (led_count * 3) + 2;
        unsigned char*  direct_packet       = new unsigned char[direct_packet_size];

        /*-------------------------------------------------*\
        | First byte in packet is LED count                 |
        \*-------------------------------------------------*/
        direct_packet[0]                    = led_count;

        /*-------------------------------------------------*\
        | Fill in LED data                                  |
        \*-------------------------------------------------*/
        for(unsigned int led_idx = 0; led_idx < led_count; led_idx++)
        {
            unsigned int color_index        = led_idx;
            unsigned int offset             = (led_idx * 3) + 1;

            if(corsair_dram_device_list[device_index]->reverse)
            {
                color_index                 = (led_count -1) - led_idx;
            }

            direct_packet[offset + 0]       = RGBGetRValue(colors[color_index]);
            direct_packet[offset + 1]       = RGBGetGValue(colors[color_index]);
            direct_packet[offset + 2]       = RGBGetBValue(colors[color_index]);
        }

        /*-------------------------------------------------*\
        | Last byte in packet is CRC of all data up to it   |
        \*-------------------------------------------------*/
        direct_packet[direct_packet_size - 1] = CRCPP::CRC::Calculate(direct_packet, (direct_packet_size - 1), CRCPP::CRC::CRC_8());

        /*-------------------------------------------------*\
        | Write using block writes, if packet exceeds 32    |
        | bytes, use a second block write to the second     |
        | block write address for the remaining data        |
        \*-------------------------------------------------*/
        bus->i2c_smbus_write_block_data(dev, CORSAIR_DRAM_REG_COLOR_BUFFER_BLOCK_1, 32, direct_packet);

        if(direct_packet_size > 32)
        {
            bus->i2c_smbus_write_block_data(dev, CORSAIR_DRAM_REG_COLOR_BUFFER_BLOCK_2, direct_packet_size - 32, direct_packet + 32);
        }

        /*-------------------------------------------------*\
        | Remember to delete the data buffer                |
        \*-------------------------------------------------*/
        delete[] direct_packet;
    }
    else
    {
        /*-------------------------------------------------*\
        | Local variables                                   |
        \*-------------------------------------------------*/
        unsigned char   device_crc;
        unsigned char   calc_crc;

        /*-------------------------------------------------*\
        | Packet format:                                    |
        |   Size n is (LED count * 4)                       |
        |   Format is 0xRR, 0xGG, 0xBB, 0xFF for each LED   |
        \*-------------------------------------------------*/
        unsigned int    color_data_size     = (led_count * 4);
        unsigned char*  color_data_packet   = new unsigned char[color_data_size];

        for(unsigned int led_idx = 0; led_idx < led_count; led_idx++)
        {
            unsigned int color_index        = led_idx;

            if(corsair_dram_device_list[device_index]->reverse)
            {
                color_index                 = (led_count -1) - led_idx;
            }

            color_data_packet[(led_idx * 4) + 0]  = RGBGetRValue(colors[color_index]);
            color_data_packet[(led_idx * 4) + 1]  = RGBGetGValue(colors[color_index]);
            color_data_packet[(led_idx * 4) + 2]  = RGBGetBValue(colors[color_index]);
            color_data_packet[(led_idx * 4) + 3]  = 0xFF;
        }

        /*-------------------------------------------------*\
        | Write LED color data packet                       |
        \*-------------------------------------------------*/
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_RESET_BUFFER, 0x00);
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_BINARY_START, 0x00);

        for(unsigned int i = 0; i < color_data_size; i++)
        {
            bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_SET_BINARY_DATA, color_data_packet[i]);
        }

        /*-------------------------------------------------*\
        | Calculate CRC and read CRC reported by device     |
        \*-------------------------------------------------*/
        calc_crc    = CRCPP::CRC::Calculate(color_data_packet, color_data_size, CRCPP::CRC::CRC_8());
        device_crc  = bus->i2c_smbus_read_byte_data(dev, CORSAIR_DRAM_REG_GET_CHECKSUM);

        /*-------------------------------------------------*\
        | Write effect configuration only if CRCs match     |
        \*-------------------------------------------------*/
        if(calc_crc == device_crc)
        {
            bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_WRITE_CONFIGURATION, CORSAIR_DRAM_ID_COLOR_DATA);
            WaitReady();
        }

        /*-------------------------------------------------*\
        | Remember to delete the data buffer                |
        \*-------------------------------------------------*/
        delete[] color_data_packet;
    }
}

void CorsairDRAMController::SetDirect(bool direct)
{
    direct_mode = direct;
}

void CorsairDRAMController::SetEffect
    (
    unsigned char mode,
    unsigned char speed,
    unsigned char direction,
    bool          random,
    unsigned char brightness,
    unsigned char red1,
    unsigned char grn1,
    unsigned char blu1,
    unsigned char red2,
    unsigned char grn2,
    unsigned char blu2
    )
{
    /*-----------------------------------------------------*\
    | Local variables                                       |
    \*-----------------------------------------------------*/
    unsigned char   effect_data[20];
    unsigned char   device_crc;
    unsigned char   calc_crc;
    unsigned char   random_byte;

    /*-----------------------------------------------------*\
    | If mode is direct (which is a dummy value not         |
    | understood by the hardware), return.  Direct mode is  |
    | not set in the effect configuration.                  |
    \*-----------------------------------------------------*/
    direct_mode = (mode == CORSAIR_DRAM_MODE_DIRECT);

    if(direct_mode)
    {
        return;
    }

    /*-----------------------------------------------------*\
    | Determine random byte                                 |
    \*-----------------------------------------------------*/
    if(random)
    {
        random_byte = CORSAIR_DRAM_EFFECT_RANDOM_COLORS;
    }
    else
    {
        random_byte = CORSAIR_DRAM_EFFECT_CUSTOM_COLORS;
    }

    /*-----------------------------------------------------*\
    | Fill in effect packet                                 |
    \*-----------------------------------------------------*/
    effect_data[0]  = mode;         // Mode
    effect_data[1]  = speed;        // Speed
    effect_data[2]  = random_byte;  // Custom color
    effect_data[3]  = direction;    // Direction
    effect_data[4]  = red1;         // Custom color 1 red
    effect_data[5]  = grn1;         // Custom color 1 green
    effect_data[6]  = blu1;         // Custom color 1 blue
    effect_data[7]  = brightness;
    effect_data[8]  = red2;         // Custom color 2 red
    effect_data[9]  = grn2;         // Custom color 2 green
    effect_data[10] = blu2;         // Custom color 2 blue
    effect_data[11] = brightness;
    effect_data[12] = 0x00;
    effect_data[13] = 0x00;
    effect_data[14] = 0x00;
    effect_data[15] = 0x00;
    effect_data[16] = 0x00;
    effect_data[17] = 0x00;
    effect_data[18] = 0x00;
    effect_data[19] = 0x00;

    /*-----------------------------------------------------*\
    | Write effect packet                                   |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_RESET_BUFFER, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_BINARY_START, 0x00);

    for(unsigned int i = 0; i < 20; i++)
    {
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_SET_BINARY_DATA, effect_data[i]);
    }

    /*-----------------------------------------------------*\
    | Calculate CRC and read CRC reported by device         |
    \*-----------------------------------------------------*/
    calc_crc    = CRCPP::CRC::Calculate(effect_data, sizeof(effect_data), CRCPP::CRC::CRC_8());
    device_crc  = bus->i2c_smbus_read_byte_data(dev, CORSAIR_DRAM_REG_GET_CHECKSUM);

    /*-----------------------------------------------------*\
    | Write effect configuration only if CRCs match         |
    \*-----------------------------------------------------*/
    if(calc_crc == device_crc)
    {
        bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_WRITE_CONFIGURATION, CORSAIR_DRAM_ID_EFFECT_CONFIGURATION);
        WaitReady();
    }
}

bool CorsairDRAMController::WaitReady()
{
    /*-----------------------------------------------------*\
    | Poll status register 0x30; bit 3 (0x08) = busy.       |
    | Device is ready when bit 3 is clear.                  |
    \*-----------------------------------------------------*/
    for(int retry = 0; retry < 5; retry++)
    {
        int status = bus->i2c_smbus_read_byte_data(dev, CORSAIR_DRAM_REG_STATUS);

        if(status >= 0 && (status & 0x08) == 0)
        {
            return true;
        }

        std::this_thread::sleep_for(10ms);
    }

    return false;
}

void CorsairDRAMController::ReadDeviceInfo()
{
    unsigned char   device_information_data[32];
    unsigned char   device_crc;
    unsigned char   calc_crc;

    /*-----------------------------------------------------*\
    | Request Device Information Data                       |
    \*-----------------------------------------------------*/
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_GET_DEVICE_INFO, 0x00);
    bus->i2c_smbus_write_byte_data(dev, CORSAIR_DRAM_REG_BINARY_START,    0x00);

    /*-----------------------------------------------------*\
    | Read Device Information Data                          |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < 32; i++)
    {
        device_information_data[i] = bus->i2c_smbus_read_byte_data(dev, CORSAIR_DRAM_REG_GET_BINARY_DATA);
    }

    /*-----------------------------------------------------*\
    | Compare CRC                                           |
    \*-----------------------------------------------------*/
    calc_crc    = CRCPP::CRC::Calculate(device_information_data, sizeof(device_information_data), CRCPP::CRC::CRC_8());
    device_crc  = bus->i2c_smbus_read_byte_data(dev, CORSAIR_DRAM_REG_GET_CHECKSUM);

    if(calc_crc != device_crc)
    {
        LOG_ERROR("[%s] ReadDeviceInfo CRC Mismatch", CORSAIR_DRAM_NAME);
    }

    /*-----------------------------------------------------*\
    | Log Device Information Data                           |
    \*-----------------------------------------------------*/
    if(LogManager::get()->getLoglevel() >= LL_TRACE)
    {
        char            device_info_buf[256];
        unsigned int    pos;

        pos = snprintf(device_info_buf, sizeof(device_info_buf), "%02X: ", dev);

        for(unsigned int i = 0; i < 32; i++)
        {
            pos += snprintf(&device_info_buf[pos], sizeof(device_info_buf) - pos, "%02X ", device_information_data[i]);
        }

        LOG_TRACE("[%s] Device Info: %s", CORSAIR_DRAM_NAME, device_info_buf);
    }

    /*-----------------------------------------------------*\
    | Read VID                                              |
    \*-----------------------------------------------------*/
    vid = (device_information_data[1] << 8) | device_information_data[0];

    /*-----------------------------------------------------*\
    | Read PID                                              |
    \*-----------------------------------------------------*/
    pid = (device_information_data[3] << 8) | device_information_data[2];

    /*-----------------------------------------------------*\
    | Format Firwmare Version                               |
    \*-----------------------------------------------------*/
    firmware_version = std::to_string(device_information_data[9]) + "." + std::to_string(device_information_data[8]) + "." + std::to_string((device_information_data[11] << 8) | device_information_data[10]);

    /*-----------------------------------------------------*\
    | Read Protocol Version                                 |
    \*-----------------------------------------------------*/
    protocol_version = device_information_data[28];

    /*-----------------------------------------------------*\
    | Loop through all known devices to look for a PID      |
    | match                                                 |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < CORSAIR_DRAM_NUM_DEVICES; i++)
    {
        for(unsigned int j = 0; j < CORSAIR_DRAM_MAX_PIDS; j++)
        {
            if(corsair_dram_device_list[i]->pids[j] == pid)
            {
                /*-----------------------------------------*\
                | Set device ID                             |
                \*-----------------------------------------*/
                device_index = i;
                break;
            }
        }
    }
}

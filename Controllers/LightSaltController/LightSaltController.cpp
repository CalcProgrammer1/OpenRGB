/*---------------------------------------------------------*\
| LightSaltController.cpp                                   |
|                                                           |
|   Driver for LightSalt Peripherals                        |
|                                                           |
|   James Buren (braewoods)                     23 Jul 2024 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LightSaltController.h"
#include "StringUtils.h"

static const int mode_map[LIGHTSALT_MODE_MAXIMUM][6] =
{
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0400, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0700, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0A00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0D00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0E00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0F00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x1100, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000, 0x0000, 0x3301, 0x3302},
    {0x3402, 0x3401, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x3502, 0x3501, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x3602, 0x3601, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x3802, 0x3801, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x3901, 0x3902, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x3A01, 0x3A02, 0x0000, 0x0000},
    {0x0000, 0x0000, 0x0000, 0x0000, 0x3702, 0x3701},
    {0x3B02, 0x3B01, 0x0000, 0x0000, 0x0000, 0x0000}
};

LightSaltController::LightSaltController(hid_device* dev_handle, const hid_device_info& info)
{
    wchar_t usb_string[128];

    dev             = dev_handle;
    device_location = info.path;

    if(hid_get_manufacturer_string(dev, usb_string, 128) == 0)
    {
        manufacturer = StringUtils::wstring_to_string(usb_string);
    }
    else
    {
        manufacturer = "";
    }

    if(hid_get_product_string(dev, usb_string, 128) == 0)
    {
        product = StringUtils::wstring_to_string(usb_string);
    }
    else
    {
        product = "";
    }

    if(strstr(manufacturer.c_str(), "Light&Salt") && strstr(product.c_str(), "Light&Salt"))
    {
        QueryDeviceModel();
        QueryDeviceClass();
        SetDeviceType();
    }
    else
    {
        device_model     = "";
        device_class     = "";
        firmware_version = "";
        device_type      = LIGHTSALT_TYPE_MAXIMUM;
    }
}

LightSaltController::~LightSaltController()
{
    hid_close(dev);
}

std::string LightSaltController::GetDeviceLocation()
{
    return("HID: " + device_location);
}

std::string LightSaltController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string LightSaltController::GetManufacturer()
{
    return(manufacturer);
}

std::string LightSaltController::GetProduct()
{
    return(product);
}

std::string LightSaltController::GetDeviceModel()
{
    return(device_model);
}

std::string LightSaltController::GetDeviceClass()
{
    return(device_class);
}

std::string LightSaltController::GetFirmwareVersion()
{
    return(firmware_version);
}

int LightSaltController::GetDeviceType()
{
    return(device_type);
}

void LightSaltController::SetColors(RGBColor* colors, int sets, int rows, int columns)
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];
    int color_idx = 0;

    write_buffer[0] = 0x00;

    for(int set_idx = 0; set_idx < sets; set_idx++)
    {
        write_buffer[1] = 0x03 + set_idx;

        for(int row_idx = 0; row_idx < rows; row_idx++)
        {
            int write_idx = 2;

            write_buffer[write_idx++] = 0x01 + row_idx * columns;

            for(int column_idx = 0; column_idx < columns; column_idx++)
            {
                RGBColor color = colors[color_idx++];
                write_buffer[write_idx++] = RGBGetRValue(color);
                write_buffer[write_idx++] = RGBGetGValue(color);
                write_buffer[write_idx++] = RGBGetBValue(color);
            }

            hid_write(dev, write_buffer, sizeof(write_buffer));
        }
    }
}

void LightSaltController::ApplyColors(int sets)
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];
    int write_idx = 1;

    memset(write_buffer, 0x00, sizeof(write_buffer));
    write_buffer[write_idx++] = 0xFE;
    write_buffer[write_idx++] = 0x01;

    for(int set_idx = 0; set_idx < sets; set_idx++)
    {
        write_buffer[write_idx++] = 0x03 + set_idx;
    }

    hid_write(dev, write_buffer, sizeof(write_buffer));
}

void LightSaltController::SaveColors(int sets)
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];
    int write_idx = 1;

    memset(write_buffer, 0x00, sizeof(write_buffer));
    write_buffer[write_idx++] = 0xFE;
    write_buffer[write_idx++] = 0xF0;

    for(int set_idx = 0; set_idx < sets; set_idx++)
    {
        write_buffer[write_idx++] = 0x03 + set_idx;
    }

    hid_write(dev, write_buffer, sizeof(write_buffer));
}

void LightSaltController::SetMode(int mode, int direction, int speed)
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];
    int actual_mode = mode_map[mode][direction];

    memset(write_buffer, 0x00, sizeof(write_buffer));
    write_buffer[1] = 0xFE;
    write_buffer[2] = 0x03;
    write_buffer[3] = (actual_mode & 0xFF00) >> 8;
    write_buffer[4] = (actual_mode & 0x00FF) >> 0;
    write_buffer[5] = speed;

    hid_write(dev, write_buffer, sizeof(write_buffer));
}

void LightSaltController::SetFilter(RGBColor color)
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];

    memset(write_buffer, 0x00, sizeof(write_buffer));
    write_buffer[1] = 0xFE;
    write_buffer[2] = 0x04;
    write_buffer[3] = RGBGetRValue(color);
    write_buffer[4] = RGBGetGValue(color);
    write_buffer[5] = RGBGetBValue(color);

    hid_write(dev, write_buffer, sizeof(write_buffer));
}

void LightSaltController::SetBrightness(uint8_t brightness)
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];

    memset(write_buffer, 0x00, sizeof(write_buffer));
    write_buffer[1] = 0xFE;
    write_buffer[2] = 0x05;
    write_buffer[3] = brightness;

    hid_write(dev, write_buffer, sizeof(write_buffer));
}

void LightSaltController::QueryDeviceModel()
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];
    uint8_t read_buffer[LIGHTSALT_READ_LENGTH];

    memset(write_buffer, 0x00, sizeof(write_buffer));
    write_buffer[1] = 0xFE;
    write_buffer[2] = 0xF5;
    write_buffer[3] = 0x06;

    if(hid_write(dev, write_buffer, sizeof(write_buffer)) != sizeof(write_buffer))
    {
        device_model = "";
        return;
    }

    if(hid_read_timeout(dev, read_buffer, sizeof(read_buffer), 100) != sizeof(read_buffer))
    {
        device_model = "";
        return;
    }

    device_model = (char *)(read_buffer + 3);
}

void LightSaltController::QueryDeviceClass()
{
    uint8_t write_buffer[LIGHTSALT_WRITE_LENGTH];
    uint8_t read_buffer[LIGHTSALT_READ_LENGTH];

    memset(write_buffer, 0x00, sizeof(write_buffer));
    write_buffer[1] = 0xFE;
    write_buffer[2] = 0xFF;

    if(hid_write(dev, write_buffer, sizeof(write_buffer)) != sizeof(write_buffer))
    {
        device_class = "";
        firmware_version = "";
        return;
    }

    if(hid_read_timeout(dev, read_buffer, sizeof(read_buffer), 100) != sizeof(read_buffer))
    {
        device_class = "";
        firmware_version = "";
        return;
    }

    device_class     = (char *)(read_buffer                          );
    firmware_version = (char *)(read_buffer + device_class.size() + 1);
}

void LightSaltController::SetDeviceType()
{
    struct device_class_table
    {
        std::string device_class;
        int device_type;
    };
    static const device_class_table table[] =
    {
        {"Light&Salt_keyboard1", LIGHTSALT_TYPE_KEYBOARD},
        {"Light&Salt_KEY_PAD",   LIGHTSALT_TYPE_KEYPAD  }
    };

    device_type = LIGHTSALT_TYPE_MAXIMUM;

    for(const device_class_table& match : table)
    {
        if(match.device_class == device_class)
        {
            device_type = match.device_type;
            break;
        }
    }
}

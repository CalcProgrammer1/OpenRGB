/*---------------------------------------------------------*\
| EVisionV2KeyboardController.cpp                           |
|                                                           |
|   Driver for EVision V2 keyboard                          |
|                                                           |
|   Le Philousophe                              25 Dec 2022 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogManager.h"
#include "EVisionV2KeyboardController.h"
#include "StringUtils.h"

#define BLANK_SPACE 6
#define query_check_buffer(c) \
    do                        \
    {                         \
        if(!(c))              \
        {                     \
            return -256;      \
        }                     \
    } while(0)

using namespace std::chrono_literals;

static uint8_t evisionv2_map[EVISION_V2_MATRIX_WIDTH * EVISION_V2_MATRIX_HEIGHT] =
{
 /*   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   19   19   20   */
      0,       12,  18,  24,  30,  36,  42,  48,  54,  60,  66,  72,  78,  84,  90,  96,
      1,   7,  13,  19,  25,  31,  37,  43,  49,  55,  61,  67,  73,  79,  85,  91,  97, 103, 109, 115, 121,
      2,   8,  14,  20,  26,  32,  38,  44,  50,  56,  62,  68,  74,  80,  86,  92,  98, 104, 110, 116, 122,
      3,   9,  15,  21,  27,  33,  39,  45,  51,  57,  63,  69,  75,  81,                105, 111, 117,
      4,  10,  16,  22,  28,  34,  40,  46,  52,  58,  64,  70,       82,       94,      106, 112, 118, 124,
      5,  11,  17,                 41,                 65,  71,  77,  83,  89,  95, 101,      113, 119,
};

static uint8_t endorfy_map[EVISION_V2_MATRIX_WIDTH * EVISION_V2_MATRIX_HEIGHT] =
{
 /*   0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   19   19   20   */
      0,        2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,
     21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,
     42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
     63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,       76,                 80,  81,  82,
     84,       86,  87,  88,  89,  90,  91,  92,  93,  94,  95,       97,       99,      101, 102, 103, 104,
    105, 106, 107,                111,                115, 116, 117, 118, 119, 120, 121, 123,      124,
};

EVisionV2KeyboardController::EVisionV2KeyboardController(hid_device* dev_handle, const char* path, EVisionV2KeyboardLayout dev_layout, std::string dev_name)
{
    dev                 = dev_handle;
    location            = path;
    name                = dev_name;
    layout              = dev_layout;

    /*---------------------------------------------------------*\
    | Get capabilities and layout                               |
    \*---------------------------------------------------------*/
    uint8_t buffer[7];
    if(Read(EVISION_V2_CMD_READ_CAPABILITIES, 0, sizeof(buffer), buffer) < 0)
    {
        return;
    }
    if(buffer[0] != 0xAA && buffer[1] != 0x55)
    {
        return;
    }

    map_size    = buffer[5];
    macros_size = buffer[6] * 0x80;

    switch(layout)
    {
        case EVISION_V2_KEYBOARD_LAYOUT:
            keyvalue_map = evisionv2_map;
            led_count = 106;
            break;

        case ENDORFY_KEYBOARD_LAYOUT:
            keyvalue_map = endorfy_map;
            led_count = 104;
            break;
    }
}

EVisionV2KeyboardController::~EVisionV2KeyboardController()
{
    hid_close(dev);
}

std::string EVisionV2KeyboardController::GetName()
{
    return(name);
}

std::string EVisionV2KeyboardController::GetSerial()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string EVisionV2KeyboardController::GetLocation()
{
    return("HID: " + location);
}

int EVisionV2KeyboardController::Query(uint8_t cmd, uint16_t offset, const uint8_t* idata, uint8_t size, uint8_t* odata)
{
    uint8_t buffer[EVISION_V2_PACKET_SIZE];
    memset(buffer, 0, sizeof(buffer));

    buffer[0] = EVISION_V2_REPORT_ID;
    buffer[3] = cmd;
    buffer[4] = size;
    buffer[5] = offset & 0xff;
    buffer[6] = (offset >> 8) & 0xff;

    if(idata)
    {
        memcpy(buffer + 8, idata, size);
    }

    uint16_t chksum = 0;
    for(uint8_t* p = &buffer[3]; p != &buffer[EVISION_V2_PACKET_SIZE]; p++)
    {
        chksum += *p;
    }
    buffer[1] = chksum & 0xff;
    buffer[2] = (chksum >> 8) & 0xff;

    int bytes_read;
    {
        const std::lock_guard<std::mutex> lock(query_mutex);

        hid_write(dev, buffer, sizeof(buffer));

        do
        {
            bytes_read = hid_read(dev, buffer, sizeof(buffer));
        } while(bytes_read != 0 && buffer[0] != EVISION_V2_REPORT_ID);
    }
    query_check_buffer(bytes_read == sizeof(buffer));

    query_check_buffer(buffer[0] == EVISION_V2_REPORT_ID);
    query_check_buffer(buffer[1] == (chksum & 0xff));
    query_check_buffer(buffer[2] == ((chksum >> 8) & 0xff));
    query_check_buffer(buffer[3] == cmd);
    query_check_buffer(buffer[5] == (offset & 0xff));
    query_check_buffer(buffer[6] == ((offset >> 8) & 0xff));

    if(buffer[7] != 0)
    {
        return -buffer[7];
    }

    size = buffer[4];
    if(size > EVISION_V2_PACKET_SIZE - 8)
    {
        return -256;
    }

    if(odata)
    {
        memcpy(odata, buffer + 8, size);
    }

    return size;
}

int EVisionV2KeyboardController::BeginConfigure()
{
    return Query(EVISION_V2_CMD_BEGIN_CONFIGURE);
}

int EVisionV2KeyboardController::EndConfigure()
{
    return Query(EVISION_V2_CMD_END_CONFIGURE);
}

int EVisionV2KeyboardController::Read(uint8_t cmd, uint16_t offset, uint16_t size, uint8_t* odata)
{
    while(size > 0)
    {
        uint8_t pktsz = (uint8_t)std::min<size_t>(size, EVISION_V2_PACKET_SIZE - 8);
        int result = Query(cmd, offset, nullptr, pktsz, odata);
        if(result <= 0)
        {
            return result;
        }
        else if(result > size)
        {
            return -256;
        }
        offset += result;
        odata += result;
        size -= result;
    }
    return 0;
}

int EVisionV2KeyboardController::Write(uint8_t cmd, uint16_t offset, const uint8_t* idata, uint16_t size)
{
    while(size > 0)
    {
        uint8_t pktsz = (uint8_t)std::min<size_t>(size, EVISION_V2_PACKET_SIZE - 8);
        int result = Query(cmd, offset, idata, pktsz);
        if(result <= 0)
        {
            return result;
        }
        offset += pktsz;
        idata += pktsz;
        size -= pktsz;
    }
    return 0;
}

int EVisionV2KeyboardController::GetMode(EVisionV2KeyboardPart part, EvisionV2ModeConfig& config)
{
    uint8_t buffer[18];
    memset(buffer, 0, sizeof(buffer));

    uint8_t current_profile;
    int ret = Read(EVISION_V2_CMD_READ_CONFIG, EVISION_V2_OFFSET_CURRENT_PROFILE, 1, &current_profile);
    if(ret < 0)
    {
        return ret;
    }
    if(current_profile > 2)
    {
        current_profile = 0;
    }

    uint16_t offset;
    uint8_t size;

    offset = current_profile * 0x40 + EVISION_V2_OFFSET_FIRST_PROFILE;

    switch(part)
    {
        case EVISION_V2_KEYBOARD_PART_KEYBOARD:
            size = sizeof(buffer);
            break;
        case EVISION_V2_KEYBOARD_PART_LOGO:
        case ENDORFY_KEYBOARD_PART_EDGE:
            offset += EVISION_V2_PARAMETER_LOGO;
            size = EVISION_V2_PARAMETER_LOGO_ON_OFF - EVISION_V2_PARAMETER_LOGO + 1;
            break;
        case EVISION_V2_KEYBOARD_PART_EDGE:
            offset += EVISION_V2_PARAMETER_EDGE;
            size = EVISION_V2_PARAMETER_END - EVISION_V2_PARAMETER_EDGE;
            break;
        default:
            size = 0;
            break;
    }

    ret = Read(EVISION_V2_CMD_READ_CONFIG, offset, size, buffer);
    if(ret < 0)
    {
        return ret;
    }

    config.mode = buffer[EVISION_V2_PARAMETER_MODE];
    config.brightness = buffer[EVISION_V2_PARAMETER_BRIGHTNESS];
    config.speed = buffer[EVISION_V2_PARAMETER_SPEED];
    config.direction = buffer[EVISION_V2_PARAMETER_DIRECTION];
    config.random_colours = buffer[EVISION_V2_PARAMETER_RANDOM_COLOR_FLAG] != 0;
    config.colour = ToRGBColor(buffer[EVISION_V2_PARAMETER_MODE_COLOR + 0],
        buffer[EVISION_V2_PARAMETER_MODE_COLOR + 1], buffer[EVISION_V2_PARAMETER_MODE_COLOR + 2]);
    if(part == EVISION_V2_KEYBOARD_PART_KEYBOARD)
    {
        config.ledmode = buffer[EVISION_V2_PARAMETER_LED_MODE_COLOR];
        if(config.mode == EVISION_V2_MODE_CUSTOM)
        {
            ret = Read(EVISION_V2_CMD_READ_CONFIG, current_profile * 0x40 + EVISION_V2_OFFSET_FIRST_PROFILE + EVISION_V2_PARAMETER_CURRENT_CUSTOM_MODE, sizeof(config.ledmode), &config.ledmode);
            if(ret < 0)
            {
                return ret;
            }
        }
    }
    else if(part == EVISION_V2_KEYBOARD_PART_LOGO || part ==  ENDORFY_KEYBOARD_PART_EDGE)
    {
        // Use ledmode for logo on/off
        config.ledmode = buffer[EVISION_V2_PARAMETER_LOGO_ON_OFF - EVISION_V2_PARAMETER_LOGO];
    }

    return 0;
}

void EVisionV2KeyboardController::SetMode(EVisionV2KeyboardPart part, const EvisionV2ModeConfig& config)
{
    uint8_t buffer[18];
    memset(buffer, 0, sizeof(buffer));

    buffer[EVISION_V2_PARAMETER_MODE] = config.mode;
    buffer[EVISION_V2_PARAMETER_BRIGHTNESS] = config.brightness;
    buffer[EVISION_V2_PARAMETER_SPEED] = config.speed;
    buffer[EVISION_V2_PARAMETER_DIRECTION] = config.direction;
    buffer[EVISION_V2_PARAMETER_RANDOM_COLOR_FLAG] = (config.random_colours) ? 255 : 0;
    buffer[EVISION_V2_PARAMETER_MODE_COLOR + 0] = RGBGetRValue(config.colour);
    buffer[EVISION_V2_PARAMETER_MODE_COLOR + 1] = RGBGetGValue(config.colour);
    buffer[EVISION_V2_PARAMETER_MODE_COLOR + 2] = RGBGetBValue(config.colour);
    if(part == EVISION_V2_KEYBOARD_PART_KEYBOARD)
    {
        buffer[EVISION_V2_PARAMETER_COLOR_OFFSET] = 0;
        if(config.mode != EVISION_V2_MODE_CUSTOM)
        {
            buffer[EVISION_V2_PARAMETER_LED_MODE_COLOR] = config.ledmode;
        }
    }
    else if(part == EVISION_V2_KEYBOARD_PART_LOGO || part ==  ENDORFY_KEYBOARD_PART_EDGE)
    {
        // Use ledmode for logo on/off
        buffer[EVISION_V2_PARAMETER_LOGO_ON_OFF - EVISION_V2_PARAMETER_LOGO] = config.ledmode;
    }

    BeginConfigure();
    uint8_t current_profile;

    int ret = Read(EVISION_V2_CMD_READ_CONFIG, EVISION_V2_OFFSET_CURRENT_PROFILE, 1, &current_profile);
    if(ret < 0)
    {
        return;
    }
    if(current_profile > 2)
    {
        current_profile = 0;
        Write(EVISION_V2_CMD_WRITE_CONFIG, EVISION_V2_OFFSET_CURRENT_PROFILE, &current_profile, 1);
    }

    uint16_t offset = 0;
    uint8_t size    = 0;

    offset = current_profile * 0x40 + EVISION_V2_OFFSET_FIRST_PROFILE;

    switch(part)
    {
        case EVISION_V2_KEYBOARD_PART_KEYBOARD:
            size = sizeof(buffer);
            break;
        case EVISION_V2_KEYBOARD_PART_LOGO:
        case ENDORFY_KEYBOARD_PART_EDGE:
            offset += EVISION_V2_PARAMETER_LOGO;
            size = EVISION_V2_PARAMETER_LOGO_ON_OFF - EVISION_V2_PARAMETER_LOGO + 1;
            break;
        case EVISION_V2_KEYBOARD_PART_EDGE:
            offset += EVISION_V2_PARAMETER_EDGE;
            size = EVISION_V2_PARAMETER_END - EVISION_V2_PARAMETER_EDGE;
            break;
    }

    Write(EVISION_V2_CMD_WRITE_CONFIG, offset, buffer, size);
    if((part == EVISION_V2_KEYBOARD_PART_KEYBOARD) && (config.mode == EVISION_V2_MODE_CUSTOM))
    {
        Write(EVISION_V2_CMD_WRITE_CONFIG, current_profile * 0x40 + EVISION_V2_OFFSET_FIRST_PROFILE + EVISION_V2_PARAMETER_CURRENT_CUSTOM_MODE, &config.ledmode, sizeof(config.ledmode));
    }
    EndConfigure();
}

void EVisionV2KeyboardController::SetLedsDirect(const std::vector<RGBColor>& colours)
{
    const size_t colours_num = std::min(colours.size(), led_count);

    uint8_t* buffer = new uint8_t[3 * map_size];
    memset(buffer, 0, 3 * map_size);

    for(size_t i = 0; i < colours_num; i++)
    {
        size_t j = (size_t)keyvalue_map[i] * 3;
        buffer[j + 0] = RGBGetRValue(colours[i]);
        buffer[j + 1] = RGBGetGValue(colours[i]);
        buffer[j + 2] = RGBGetBValue(colours[i]);
    }

    Write(EVISION_V2_CMD_SEND_DYNAMIC_COLORS, 0, buffer, (uint16_t)(3 * map_size));

    delete[] buffer;
}

void EVisionV2KeyboardController::SetLedDirect(int led, RGBColor colour)
{
    uint8_t buffer[3];
    buffer[0] = RGBGetRValue(colour);
    buffer[1] = RGBGetGValue(colour);
    buffer[2] = RGBGetBValue(colour);

    Write(EVISION_V2_CMD_SEND_DYNAMIC_COLORS, keyvalue_map[led] * 3, buffer, sizeof(buffer));
}

void EVisionV2KeyboardController::RefreshLedDirect()
{
    // Write one zero byte in the first blank space
    Query(EVISION_V2_CMD_SEND_DYNAMIC_COLORS, BLANK_SPACE * 3, nullptr, 1, nullptr);
}

void EVisionV2KeyboardController::EndLedsDirect()
{
    Query(EVISION_V2_CMD_END_DYNAMIC_COLORS);
}

int EVisionV2KeyboardController::GetLedsCustom(uint8_t colorset, std::vector<RGBColor>& colours)
{
    if(colorset > 9)
    {
        return -256;
    }

    const size_t colours_num = std::min(colours.size(), led_count);

    uint8_t* buffer = new uint8_t[3 * map_size];
    memset(buffer, 0, 3 * map_size);

    int ret = Read(EVISION_V2_CMD_READ_CUSTOM_COLORS, 512 * colorset, (uint16_t)(3 * map_size), buffer);
    if(ret < 0)
    {
        return ret;
    }

    for(size_t i = 0; i < colours_num; i++)
    {
        size_t j = (size_t)keyvalue_map[i] * 3;
        colours[i] = ToRGBColor(buffer[j + 0], buffer[j + 1], buffer[j + 2]);
    }

    delete[] buffer;
    return 0;
}

void EVisionV2KeyboardController::SetLedsCustom(uint8_t colorset, const std::vector<RGBColor>& colours)
{
    if(colorset > 9)
    {
        return;
    }

    const size_t colours_num = std::min(colours.size(), led_count);

    uint8_t* buffer = new uint8_t[3 * map_size];
    memset(buffer, 0, 3 * map_size);

    for(size_t i = 0; i < colours_num; i++)
    {
        size_t j = (size_t)keyvalue_map[i] * 3;
        buffer[j + 0] = RGBGetRValue(colours[i]);
        buffer[j + 1] = RGBGetGValue(colours[i]);
        buffer[j + 2] = RGBGetBValue(colours[i]);
    }

    BeginConfigure();
    Write(EVISION_V2_CMD_WRITE_CUSTOM_COLORS, 512 * colorset, buffer, (uint16_t)(3 * map_size));
    EndConfigure();

    delete[] buffer;
}

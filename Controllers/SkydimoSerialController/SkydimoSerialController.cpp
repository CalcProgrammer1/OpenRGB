/*---------------------------------------------------------*\
| SkydimoSerialController.cpp                               |
|                                                           |
|   Driver for Skydimo serial devices                       |
|                                                           |
|   Bartholomew Ho <qiangqiang101@hotmail.com>  30 Jun 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <algorithm>
#include <cctype>
#include "LogManager.h"
#include "SkydimoSerialController.h"

enum SkydimoLayoutKind
{
    SKYDIMO_LAYOUT_NONE,
    SKYDIMO_LAYOUT_STRIP_1,
    SKYDIMO_LAYOUT_PERIMETER_3,
    SKYDIMO_LAYOUT_PERIMETER_4,
    SKYDIMO_LAYOUT_SIDES_2
};

struct SkydimoModelEntry
{
    const char*         model;
    SkydimoLayoutKind   layout;
    unsigned int        zone_leds[4];
    unsigned int        total_leds;
    unsigned int        max_leds;
};

static const SkydimoModelEntry skydimo_model_entries[] =
{
    { "SK0121", SKYDIMO_LAYOUT_PERIMETER_3, { 13,   25, 13,  0 },   51,   0 },
    { "SK0124", SKYDIMO_LAYOUT_PERIMETER_3, { 14,   26, 14,  0 },   54,   0 },
    { "SK0127", SKYDIMO_LAYOUT_PERIMETER_3, { 17,   31, 17,  0 },   65,   0 },
    { "SK0132", SKYDIMO_LAYOUT_PERIMETER_3, { 20,   37, 20,  0 },   77,   0 },
    { "SK0134", SKYDIMO_LAYOUT_PERIMETER_3, { 15,   41, 15,  0 },   71,   0 },
    { "SK0149", SKYDIMO_LAYOUT_PERIMETER_3, { 19,   69, 19,  0 },  107,   0 },
    { "SK0201", SKYDIMO_LAYOUT_SIDES_2,     { 20,   20,  0,  0 },   40,   0 },
    { "SK0202", SKYDIMO_LAYOUT_SIDES_2,     { 30,   30,  0,  0 },   60,   0 },
    { "SK0204", SKYDIMO_LAYOUT_SIDES_2,     { 25,   25,  0,  0 },   50,   0 },
    { "SK0402", SKYDIMO_LAYOUT_STRIP_1,     { 72,    0,  0,  0 },   72,   0 },
    { "SK0403", SKYDIMO_LAYOUT_STRIP_1,     { 96,    0,  0,  0 },   96,   0 },
    { "SK0404", SKYDIMO_LAYOUT_STRIP_1,     { 144,   0,  0,  0 },  144,   0 },
    { "SK0410", SKYDIMO_LAYOUT_NONE,        { 0,     0,  0,  0 },    0, 300 },
    { "SK0801", SKYDIMO_LAYOUT_STRIP_1,     { 2,     0,  0,  0 },    2,   0 },
    { "SK0802", SKYDIMO_LAYOUT_STRIP_1,     { 18,    0,  0,  0 },   18,   0 },
    { "SK0803", SKYDIMO_LAYOUT_STRIP_1,     { 10,    0,  0,  0 },   10,   0 },
    { "SK0901", SKYDIMO_LAYOUT_STRIP_1,     { 14,    0,  0,  0 },   14,   0 },
    { "SK0E01", SKYDIMO_LAYOUT_STRIP_1,     { 16,    0,  0,  0 },   16,   0 },
    { "SK0F01", SKYDIMO_LAYOUT_SIDES_2,     { 29,   29,  0,  0 },   58,   0 },
    { "SK0F02", SKYDIMO_LAYOUT_SIDES_2,     { 25,   25,  0,  0 },   50,   0 },
    { "SK0H01", SKYDIMO_LAYOUT_STRIP_1,     { 2,     0,  0,  0 },    2,   0 },
    { "SK0H02", SKYDIMO_LAYOUT_STRIP_1,     { 4,     0,  0,  0 },    4,   0 },
    { "SK0K01", SKYDIMO_LAYOUT_STRIP_1,     { 120,   0,  0,  0 },  120,   0 },
    { "SK0K02", SKYDIMO_LAYOUT_STRIP_1,     { 15,    0,  0,  0 },   15,   0 },
    { "SK0L21", SKYDIMO_LAYOUT_PERIMETER_4, { 13,   25, 13, 25 },   76,   0 },
    { "SK0L24", SKYDIMO_LAYOUT_PERIMETER_4, { 14,   26, 14, 26 },   80,   0 },
    { "SK0L27", SKYDIMO_LAYOUT_PERIMETER_4, { 17,   31, 17, 31 },   96,   0 },
    { "SK0L32", SKYDIMO_LAYOUT_PERIMETER_4, { 20,   37, 20, 37 },  114,   0 },
    { "SK0L34", SKYDIMO_LAYOUT_PERIMETER_4, { 15,   41, 15, 41 },  112,   0 },
    { "SK0M01", SKYDIMO_LAYOUT_STRIP_1,     { 24,    0,  0,  0 },   24,   0 },
    { "SK0N01", SKYDIMO_LAYOUT_STRIP_1,     { 256,   0,  0,  0 },  256,   0 },
    { "SK0N02", SKYDIMO_LAYOUT_STRIP_1,     { 1024,  0,  0,  0 }, 1024,   0 },
    { "SK0N03", SKYDIMO_LAYOUT_STRIP_1,     { 253,   0,  0,  0 },  253,   0 },
    { "SK0S01", SKYDIMO_LAYOUT_STRIP_1,     { 32,    0,  0,  0 },   32,   0 },
    { "SKA124", SKYDIMO_LAYOUT_PERIMETER_3, { 18,   34, 18,  0 },   70,   0 },
    { "SKA127", SKYDIMO_LAYOUT_PERIMETER_3, { 20,   41, 20,  0 },   81,   0 },
    { "SKA132", SKYDIMO_LAYOUT_PERIMETER_3, { 25,   45, 25,  0 },   95,   0 },
    { "SKA134", SKYDIMO_LAYOUT_PERIMETER_3, { 21,   51, 21,  0 },   93,   0 },
    { "SKB124", SKYDIMO_LAYOUT_PERIMETER_4, { 18,   34, 18, 34 },  104,   0 },
    { "SKB127", SKYDIMO_LAYOUT_PERIMETER_4, { 20,   41, 20, 41 },  122,   0 },
    { "SKB132", SKYDIMO_LAYOUT_PERIMETER_4, { 25,   44, 25, 44 },  138,   0 },
    { "SKB134", SKYDIMO_LAYOUT_PERIMETER_4, { 21,   50, 21, 50 },  142,   0 }
};

#define SKYDIMO_MODEL_ENTRY_COUNT (sizeof(skydimo_model_entries) / sizeof(skydimo_model_entries[0]))

static const unsigned char skydimo_serial_frame_header[] =
{
    0x41, 0x64, 0x61, 0x00
};

static std::string StringToUpper(const std::string& value)
{
    std::string result = value;

    for(unsigned int char_idx = 0; char_idx < result.size(); char_idx++)
    {
        result[char_idx] = (char)std::toupper((unsigned char)result[char_idx]);
    }

    return(result);
}

static const SkydimoModelEntry* FindModelEntry(const std::string& model)
{
    for(unsigned int entry_idx = 0; entry_idx < SKYDIMO_MODEL_ENTRY_COUNT; entry_idx++)
    {
        if(model == skydimo_model_entries[entry_idx].model)
        {
            return(&skydimo_model_entries[entry_idx]);
        }
    }

    return(nullptr);
}

static std::string BytesToHex(const std::vector<unsigned char>& bytes, unsigned int offset)
{
    static const char hex[] = "0123456789ABCDEF";
    std::string result;

    result.reserve((bytes.size() - offset) * 2);

    for(unsigned int byte_idx = offset; byte_idx < bytes.size(); byte_idx++)
    {
        result.push_back(hex[(bytes[byte_idx] >> 4) & 0x0F]);
        result.push_back(hex[bytes[byte_idx] & 0x0F]);
    }

    return(result);
}

SkydimoSerialController::SkydimoSerialController(const std::string& port_name_arg)
{
    port_name       = port_name_arg;
    model_name      = "Skydimo UNKNOWN";
    model_id        = "UNKNOWN";
    serial_id       = "000000";
    port_open       = serial_port_interface.serial_open(port_name.c_str(), SKYDIMO_SERIAL_BAUD_RATE);
    present         = false;
    resizable       = true;
    leds_count      = SKYDIMO_SERIAL_DEFAULT_LED_COUNT;
    leds_min        = SKYDIMO_SERIAL_DEFAULT_LED_COUNT;
    leds_max        = SKYDIMO_SERIAL_DEFAULT_MAX_LED_COUNT;
    matrix_width    = 0;
    matrix_height   = 0;

    if(!port_open)
    {
        LOG_DEBUG("[SkydimoSerialController] Failed to open serial port %s", port_name.c_str());
    }
}

SkydimoSerialController::~SkydimoSerialController()
{

}

bool SkydimoSerialController::IsPresent()
{
    if(present)
    {
        return(true);
    }

    present = QueryInfo();
    return(present);
}

bool SkydimoSerialController::QueryInfo()
{
    if(!port_open)
    {
        return(false);
    }

    serial_port_interface.serial_flush_rx();

    char query[] = SKYDIMO_SERIAL_QUERY;
    int bytes_written = serial_port_interface.serial_write(query, SKYDIMO_SERIAL_QUERY_LENGTH);

    if(bytes_written != SKYDIMO_SERIAL_QUERY_LENGTH)
    {
        LOG_DEBUG("[SkydimoSerialController] Failed to send identification query to %s (%d/%u bytes)",
                  port_name.c_str(), bytes_written, (unsigned int)SKYDIMO_SERIAL_QUERY_LENGTH);
        return(false);
    }

    std::vector<unsigned char> response;

    for(unsigned int attempt = 0; attempt < SKYDIMO_SERIAL_QUERY_ATTEMPTS; attempt++)
    {
        char buffer[SKYDIMO_SERIAL_READ_BUFFER_SIZE];
        int bytes_read = serial_port_interface.serial_read(buffer, SKYDIMO_SERIAL_READ_BUFFER_SIZE);

        if(bytes_read < 0)
        {
            LOG_DEBUG("[SkydimoSerialController] Failed to read identification response from %s", port_name.c_str());
            return(false);
        }

        if(bytes_read > 0)
        {
            response.insert(response.end(), buffer, buffer + bytes_read);
        }

        if(response.size() >= SKYDIMO_SERIAL_MAX_RESPONSE_SIZE)
        {
            break;
        }

        if(!response.empty())
        {
            unsigned char last = response[response.size() - 1];
            unsigned char prev = response.size() >= 2 ? response[response.size() - 2] : 0;

            if((last == '\r') || (last == '\n') || (prev == '\r'))
            {
                break;
            }
        }
    }

    while(!response.empty() && ((response.back() == '\r') || (response.back() == '\n')))
    {
        response.pop_back();
    }

    if(response.size() < SKYDIMO_SERIAL_MODEL_ID_LENGTH)
    {
        LOG_TRACE("[SkydimoSerialController] No Skydimo identification response from %s", port_name.c_str());
        return(false);
    }

    if((std::toupper(response[0]) != 'S') || (std::toupper(response[1]) != 'K'))
    {
        LOG_TRACE("[SkydimoSerialController] Ignoring non-Skydimo serial device at %s", port_name.c_str());
        return(false);
    }

    model_id       = StringToUpper(std::string(response.begin(), response.begin() + SKYDIMO_SERIAL_MODEL_ID_LENGTH));
    model_name     = "Skydimo " + model_id;
    serial_id      = "000000";

    unsigned int comma_offset = (unsigned int)response.size();

    for(unsigned int byte_idx = 0; byte_idx < response.size(); byte_idx++)
    {
        if(response[byte_idx] == ',')
        {
            comma_offset = byte_idx;
            break;
        }
    }

    if(comma_offset < response.size())
    {
        if(comma_offset < SKYDIMO_SERIAL_MODEL_ID_LENGTH)
        {
            LOG_DEBUG("[SkydimoSerialController] Invalid identification response from %s", port_name.c_str());
            return(false);
        }

        if((comma_offset + 1) < response.size())
        {
            serial_id = BytesToHex(response, comma_offset + 1);
        }
    }

    ConfigureOutput();
    return(true);
}

void SkydimoSerialController::ConfigureOutput()
{
    const SkydimoModelEntry* entry = FindModelEntry(model_id);

    resizable       = true;
    leds_count      = SKYDIMO_SERIAL_DEFAULT_LED_COUNT;
    leds_min        = SKYDIMO_SERIAL_DEFAULT_LED_COUNT;
    leds_max        = SKYDIMO_SERIAL_DEFAULT_MAX_LED_COUNT;
    matrix_width    = 0;
    matrix_height   = 0;
    matrix_map.clear();

    if((entry != nullptr) && (entry->max_leds != 0))
    {
        leds_max = entry->max_leds;
    }

    if((entry == nullptr) || (entry->layout == SKYDIMO_LAYOUT_NONE))
    {
        return;
    }

    resizable   = false;
    leds_count  = entry->total_leds;
    leds_min    = entry->total_leds;
    leds_max    = entry->total_leds;

    if(entry->layout == SKYDIMO_LAYOUT_STRIP_1)
    {
        return;
    }

    const unsigned int zone_1_leds = entry->zone_leds[0];
    const unsigned int zone_2_leds = entry->zone_leds[1];
    const unsigned int zone_3_leds = entry->zone_leds[2];
    const unsigned int zone_4_leds = entry->zone_leds[3];

    if(entry->layout == SKYDIMO_LAYOUT_PERIMETER_4)
    {
        matrix_height = std::max(zone_1_leds, zone_3_leds) + 2;
        matrix_width  = std::max(zone_2_leds, zone_4_leds) + 2;
    }
    else if(entry->layout == SKYDIMO_LAYOUT_PERIMETER_3)
    {
        matrix_height = std::max(zone_1_leds, zone_3_leds) + 1;
        matrix_width  = zone_2_leds + 2;
    }
    else
    {
        matrix_height = std::max(zone_1_leds, zone_2_leds) + 2;
        matrix_width  = std::max(3U, ((16 * matrix_height) + 4) / 9);
    }

    matrix_map.assign(matrix_width * matrix_height, SKYDIMO_SERIAL_MATRIX_NA);

    unsigned int led_index = 0;

    if(entry->layout == SKYDIMO_LAYOUT_SIDES_2)
    {
        unsigned int placed = 0;
        int y = (int)matrix_height - 2;

        while((placed < zone_1_leds) && (y >= 1))
        {
            SetMatrixCell((unsigned int)y, 0, led_index);
            placed++;
            y--;
        }
    }
    else
    {
        unsigned int placed = 0;
        int y = entry->layout == SKYDIMO_LAYOUT_PERIMETER_3 ? (int)matrix_height - 1
                                                            : (int)matrix_height - 2;

        while((placed < zone_1_leds) && (y >= 1))
        {
            SetMatrixCell((unsigned int)y, matrix_width - 1, led_index);
            placed++;
            y--;
        }
    }

    if((entry->layout == SKYDIMO_LAYOUT_PERIMETER_3) || (entry->layout == SKYDIMO_LAYOUT_PERIMETER_4))
    {
        unsigned int placed = 0;
        int x = (int)matrix_width - 2;

        while((placed < zone_2_leds) && (x >= 1))
        {
            SetMatrixCell(0, (unsigned int)x, led_index);
            placed++;
            x--;
        }
    }
    else if(entry->layout == SKYDIMO_LAYOUT_SIDES_2)
    {
        unsigned int placed = 0;
        unsigned int y = 1;

        while((placed < zone_2_leds) && (y <= (matrix_height - 2)))
        {
            SetMatrixCell(y, matrix_width - 1, led_index);
            placed++;
            y++;
        }
    }

    unsigned int end_y     = entry->layout == SKYDIMO_LAYOUT_PERIMETER_3 ? matrix_height - 1 : matrix_height - 2;
    unsigned int placed    = 0;
    unsigned int y         = 1;

    while((placed < zone_3_leds) && (y <= end_y))
    {
        SetMatrixCell(y, 0, led_index);
        placed++;
        y++;
    }

    if(entry->layout == SKYDIMO_LAYOUT_PERIMETER_4)
    {
        placed = 0;
        unsigned int x = 1;

        while((placed < zone_4_leds) && (x <= (matrix_width - 2)))
        {
            SetMatrixCell(matrix_height - 1, x, led_index);
            placed++;
            x++;
        }
    }

    if(led_index != entry->total_leds)
    {
        LOG_ERROR("[SkydimoSerialController] Invalid matrix map for %s (%u/%u LEDs placed)",
                  model_id.c_str(), led_index, entry->total_leds);
    }
}

void SkydimoSerialController::SetMatrixCell(unsigned int y, unsigned int x, unsigned int& led_index)
{
    if((y < matrix_height) && (x < matrix_width))
    {
        matrix_map[(y * matrix_width) + x] = led_index++;
    }
}

bool SkydimoSerialController::WriteFrame(const std::vector<RGBColor>& colors, unsigned int count)
{
    std::lock_guard<std::mutex> guard(device_mutex);

    if(!port_open)
    {
        return(false);
    }

    if(count > colors.size())
    {
        count = (unsigned int)colors.size();
    }

    std::vector<unsigned char> packet;
    packet.reserve(sizeof(skydimo_serial_frame_header) + SKYDIMO_SERIAL_FRAME_COUNT_SIZE + (count * SKYDIMO_SERIAL_BYTES_PER_LED));

    for(unsigned int header_idx = 0; header_idx < sizeof(skydimo_serial_frame_header); header_idx++)
    {
        packet.push_back(skydimo_serial_frame_header[header_idx]);
    }

    packet.push_back((unsigned char)((count >> 8) & 0xFF));
    packet.push_back((unsigned char)(count & 0xFF));

    for(unsigned int led_idx = 0; led_idx < count; led_idx++)
    {
        packet.push_back(RGBGetRValue(colors[led_idx]));
        packet.push_back(RGBGetGValue(colors[led_idx]));
        packet.push_back(RGBGetBValue(colors[led_idx]));
    }

    int packet_size         = (int)packet.size();
    int total_bytes_written = 0;

    while(total_bytes_written < packet_size)
    {
        int bytes_written = serial_port_interface.serial_write((char*)packet.data() + total_bytes_written,
                                                                packet_size - total_bytes_written);

        if(bytes_written <= 0)
        {
            LOG_ERROR("[SkydimoSerialController] Failed to write RGB frame to %s (%d/%d bytes)",
                      port_name.c_str(), total_bytes_written, packet_size);
            return(false);
        }

        total_bytes_written += bytes_written;
    }

    return(true);
}

void SkydimoSerialController::SetLEDs(const std::vector<RGBColor>& colors)
{
    WriteFrame(colors, leds_count);
}

void SkydimoSerialController::SetBlack()
{
    std::vector<RGBColor> black_colors;

    black_colors.resize(leds_count, ToRGBColor(0, 0, 0));
    WriteFrame(black_colors, leds_count);
}

void SkydimoSerialController::SetLEDCount(unsigned int led_count)
{
    if(!resizable)
    {
        return;
    }

    if((led_count < leds_min) || (led_count > leds_max))
    {
        LOG_WARNING("[SkydimoSerialController] Ignoring invalid LED count %u for %s (valid range %u-%u)",
                    led_count, model_id.c_str(), leds_min, leds_max);
        return;
    }

    leds_count = led_count;
}

std::string SkydimoSerialController::GetName() const
{
    return(model_name);
}

std::string SkydimoSerialController::GetSerial() const
{
    return(serial_id);
}

std::string SkydimoSerialController::GetLocation() const
{
    return("Serial: " + port_name);
}

bool SkydimoSerialController::IsResizable() const
{
    return(resizable);
}

unsigned int SkydimoSerialController::GetLEDCount() const
{
    return(leds_count);
}

unsigned int SkydimoSerialController::GetMinLEDCount() const
{
    return(leds_min);
}

unsigned int SkydimoSerialController::GetMaxLEDCount() const
{
    return(leds_max);
}

unsigned int SkydimoSerialController::GetMatrixWidth() const
{
    return(matrix_width);
}

unsigned int SkydimoSerialController::GetMatrixHeight() const
{
    return(matrix_height);
}

const std::vector<unsigned int>& SkydimoSerialController::GetMatrixMap() const
{
    return(matrix_map);
}

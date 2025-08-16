/*------------------------------------------*\
|  SinowealthKeyboardController.cpp          |
|                                            |
|  Definitions and types for Sinowealth      |
|  Keyboard, Hopefully generic, this was     |
|  made spefically for FL eSports F11 KB     |
|                                            |
|  Dmitri Kalinichenko (Dima-Kal) 23/06/2021 |
\*-----------------------------------------=*/

#include <cstring>
#include "SinowealthKeyboardController.h"
#include "StringUtils.h"

static unsigned char send_per_key_part_of_command_packet[]  = { 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                                                                0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00,
                                                                0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char mode_brightness_speed_packet[]         = { 0x06, 0x03, 0xB6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x5A, 0xA5, 0x03, 0x03, 0x00, 0x00, 0x00, 0x02, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00,
                                                                0x55, 0x55, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x20, 0x00, 0x44, 0x07, 0x30,
                                                                0x07, 0x23, 0x00, 0x23, 0x00, 0x23, 0x07, 0x33, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23,
                                                                0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23, 0x07, 0x23,
                                                                0x07, 0x23, 0x00, 0x10, 0x00, 0x10, 0x07, 0x44, 0x07, 0x44, 0x07, 0x44, 0x07, 0x44, 0x07, 0x44,
                                                                0x07, 0x44, 0x07, 0x44, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xA5, 0x03, 0x03 };

static unsigned char tkl_keys_per_key_index[]               = { 0x08, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                                                0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x1d, 0x1E, 0x1F,
                                                                0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D,
                                                                0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
                                                                0x40, 0x41, 0x42, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
                                                                0x50, 0x51, 0x52, 0x54, 0x5D, 0x5f,
                                                                0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x6A, 0x6B,
                                                                0x71, 0x72, 0x73, 0x76, 0x79, 0x7A, 0x7B, 0x7F, 0x80, 0x81 };

static unsigned int keys_tkl_keys_indices_static_command[]  = { 0x0022, 0x0024, 0x0026, 0x0027, 0x0029, 0x002B, 0x002D, 0x002E, 0x002F,
                                                                0x0030, 0x0031, 0x0032, 0x0037, 0x0039, 0x003B, 0x003C, 0x003E,
                                                                0x0040, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x004C, 0x004E,
                                                                0x0050, 0x0051, 0x0053, 0x0055, 0x0057, 0x0058, 0x0059, 0x005A, 0x005B, 0x005C,
                                                                0x0061, 0x0063, 0x0065, 0x0066, 0x0068, 0x006A, 0x006C, 0x006D, 0x006E, 0x006F,
                                                                0x0070, 0x0071, 0x0076, 0x0078, 0x007A, 0x007B, 0x007D, 0x007F,
                                                                0x0081, 0x0082, 0x0083, 0x0084, 0x0085, 0x0086, 0x008B, 0x008D, 0x008F,
                                                                0x0090, 0x0092, 0x0094, 0x0096, 0x0097, 0x0098, 0x0099, 0x009A, 0x009B,
                                                                0x00A0, 0x00A2, 0x00A4, 0x00A5, 0x00A7, 0x00A9, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x00AF,
                                                                0x00B0, 0x00B5, 0x00B7, 0x00B9, 0x00BA, 0x00BC, 0x00BE,
                                                                0x00C0, 0x00E1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00CA, 0x00CC, 0x00CE, 0x00CF,
                                                                0x00D1, 0x00D3, 0x00D5, 0x00D6, 0x00D7, 0x00D8, 0x00D9, 0x00DA, 0x00DF,
                                                                0x00E1, 0x00E3, 0x00E4, 0x00E6, 0x00E8, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF,
                                                                0x00F4, 0x00F6, 0x00F8, 0x00F9, 0x00FB, 0x00FD, 0x00FF,
                                                                0x0100, 0x0101, 0x0102, 0x0103, 0x0104, 0x0109, 0x010B, 0x010D, 0x010E,
                                                                0x0110, 0x0112, 0x0114, 0x0115, 0x0116, 0x0117, 0x0118, 0x0119, 0x011E,
                                                                0x0120, 0x0122, 0x0123, 0x0125, 0x0127, 0x0129, 0x012A, 0x012B, 0x012C, 0x012D, 0x012E,
                                                                0x0133, 0x0135, 0x0137, 0x0138, 0x013A, 0x013C, 0x013E, 0x013F,
                                                                0x0140, 0x0141, 0x0142, 0x0143, 0x0148, 0x014A, 0x014C, 0x014D, 0x014F,
                                                                0x0151, 0x0153, 0x0154, 0x0155, 0x0156, 0x0157, 0x0158, 0x015D, 0x015F,
                                                                0x0161, 0x0162, 0x0164, 0x0166, 0x0168, 0x0169, 0x016A, 0x016B, 0x016C, 0x016D,
                                                                0x0172, 0x0174, 0x0176, 0x0177, 0x0179, 0x017B, 0x017D, 0x017E, 0x017F,
                                                                0x0180, 0x0181, 0x0182, 0x0187, 0x0189, 0x018B, 0x018C, 0x018E,
                                                                0x0190, 0x0192, 0x0193, 0x0194, 0x0195, 0x0196, 0x0197, 0x019C, 0x019E,
                                                                0x01A0, 0x01A1, 0x01A3, 0x01A5, 0x01A7, 0x01A8, 0x01A9, 0x01AA, 0x01AB, 0x01AC,
                                                                0x01B1, 0x01B3, 0x01B5, 0x01B6, 0x01B8, 0x01BA, 0x01BC, 0x01BD, 0x01BE, 0x01BF,
                                                                0x01C0, 0x01C1, 0x01C6, 0x01C8, 0x01CA, 0x01Cb, 0x01CD, 0x01CF,
                                                                0x01D1, 0x01D2, 0x01D3, 0x01D4, 0x01D5, 0x01D6, 0x01DB, 0x01DD, 0x01DF,
                                                                0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E7, 0x01E8, 0x01E9, 0x01EA};


SinowealthKeyboardController::SinowealthKeyboardController(hid_device* dev_cmd_handle, hid_device* dev_data_handle, char* path, std::string dev_name)
{
    dev_cmd         = dev_cmd_handle;
    dev_data        = dev_data_handle;
    name            = dev_name;

    led_count       = sizeof(tkl_keys_per_key_index) / sizeof(*tkl_keys_per_key_index);

    current_mode    = MODE_STATIC;
    current_speed   = SPEED_NORMAL;

    location        = path;
}

SinowealthKeyboardController::~SinowealthKeyboardController()
{
    hid_close(dev_cmd);
    hid_close(dev_data);
}

std::string SinowealthKeyboardController::GetLocation()
{
    return("HID: " + location);
}

std::string SinowealthKeyboardController::GetName()
{
    return(name);
}

unsigned char SinowealthKeyboardController::GetCurrentMode()
{
    return current_mode;
}

unsigned int SinowealthKeyboardController::GetLEDCount()
{
    return(sizeof(tkl_keys_per_key_index) / sizeof(*tkl_keys_per_key_index));
}

std::string SinowealthKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_cmd, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void SinowealthKeyboardController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    const int buffer_size = 1032;

    unsigned char buf[buffer_size];
    unsigned int num_keys = sizeof(tkl_keys_per_key_index) / sizeof(*tkl_keys_per_key_index);

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x06;
    buf[0x01]   = 0x09;
    buf[0x02]   = 0xBC;
    buf[0x03]   = 0x00;
    buf[0x04]   = 0x40;

    for(unsigned int i = 0 ; i < (sizeof(send_per_key_part_of_command_packet) / sizeof(char)); i++)
    {
        buf[0x027C + i] = send_per_key_part_of_command_packet[i];
    }

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(unsigned int i = 0; i < num_keys; i++)
    {
        buf[tkl_keys_per_key_index[i]]               = RGBGetBValue(colors[i]);
        buf[tkl_keys_per_key_index[i] + 0x7E]        = RGBGetGValue(colors[i]);
        buf[tkl_keys_per_key_index[i] + 0x7E + 0x7E] = RGBGetRValue(colors[i]);
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev_data, buf, sizeof(buf));
}

void SinowealthKeyboardController::SetStaticColor(RGBColor* color_buf)
{
    const int buffer_size = 1032;

    unsigned char usb_buf[buffer_size];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    int offset = 0;

    usb_buf[offset] = 0x06;
    offset += 1;

    usb_buf[offset] = 0x08;
    offset += 1;

    usb_buf[offset] = 0xB8;
    offset += 2;

    usb_buf[offset] = 0x40;

    usb_buf[0x1D] = RGBGetRValue(color_buf[0]);
    usb_buf[0x1E] = RGBGetGValue(color_buf[0]);
    usb_buf[0x1F] = RGBGetBValue(color_buf[0]);

    unsigned int size_of_keys_array = sizeof (keys_tkl_keys_indices_static_command)/ sizeof(int);

    for(unsigned int i = 0x00; i < size_of_keys_array; i++)
    {
        unsigned int key_code = keys_tkl_keys_indices_static_command[i];
        usb_buf[key_code] = 0xFF;
    }

    hid_send_feature_report(dev_data, usb_buf, sizeof(usb_buf));
}

void SinowealthKeyboardController::SetMode(unsigned char mode, unsigned char brightness, unsigned char speed, unsigned char color_mode)
{
    const int buffer_size = 1032;

    int mode_byte_index = 0x15;
    const int speed_and_brightness_byte_index_start = 0x29; // Speed + brightnes level value, Seriously?

    unsigned int color_mode_value = color_mode == MODE_COLORS_RANDOM ? 0x07 : 0x00; // 0x07 - Value to set random color mode

    unsigned char usb_buf[buffer_size];
    memset(usb_buf, 0x00, sizeof(usb_buf));

    unsigned int mode_brightness_speed_packet_length = sizeof(mode_brightness_speed_packet)/sizeof(char);

    for(unsigned int i = 0x00; i < mode_brightness_speed_packet_length; i++)
    {
        usb_buf[i] = mode_brightness_speed_packet[i];
    }

    usb_buf[mode_byte_index] = mode;

    int speed_and_brightness_byte_index = speed_and_brightness_byte_index_start + ((mode - 2) * 2);

    switch(mode)
    {
        case MODE_OFF:
            break;
        case MODE_RAINBOW:
            break;
        case MODE_FLASH_AWAY:
            break;
        case MODE_RAINDROPS:
            break;
        case MODE_RAINBOW_WHEEL:
            break;
        case MODE_RIPPLES_SHINING:
            break;
        case MODE_STARS_TWINKLE:
            break;
        case MODE_SHADOW_DISAPPEAR:
            break;
        case MODE_RETRO_SNAKE:
            break;
        case MODE_NEON_STREAM:
            break;
        case MODE_REACTION:
            break;
        case MODE_SINE_WAVE:
            break;
        case MODE_RETINUE_SCANNING:
            break;
        case MODE_ROTATING_WINDMILL:
            break;
        case MODE_COLORFUL_WATERFALL:
            break;
        case MODE_BLOSSOMING:
            break;
        case MODE_ROTATING_STORM:
            break;
        case MODE_COLLISION:
            break;
        case MODE_PERFECT:
            break;
        case MODE_PER_KEY:
            usb_buf[mode_byte_index - 1] = 0x01;
            usb_buf[0x27] = 0x24;
            break;
    }

    int color_mode_byte_index = speed_and_brightness_byte_index - 1;

    usb_buf[speed_and_brightness_byte_index] = speed + brightness;
    usb_buf[color_mode_byte_index] = color_mode_value;

    int result = hid_send_feature_report(dev_data, usb_buf, sizeof(usb_buf));

    if(result != -1)
    {
        current_mode = mode;
    }
}

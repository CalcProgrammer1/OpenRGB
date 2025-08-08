/*---------------------------------------------------------*\
| EVGAKeyboardController.cpp                                |
|                                                           |
|   Driver for EVGA keyboard                                |
|                                                           |
|   Chris M (Dr_No)                             25 Nov 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "EVGAKeyboardController.h"
#include "StringUtils.h"

static uint8_t packet_map[EVGA_KEYBOARD_FULL_SIZE_KEYCOUNT + EVGA_KEYBOARD_Z20_EXTRA_KEYS] =
{
/*00        ESC  F1   F2   F3   F4   F5   F6   F7   F8   F9  */
             1,   2,   3,   4,   5,   6,   7,   8,   9,  10,

/*10        F10  F11  F12  PRT  SLK  PBK   `    1    2    3  */
            11,  12,  13,  14,  15,  16,  22,  23,  24,  25,

/*20         4    5    6    7    8    9    0    -    =   BSP */
            26,  27,  28,  29,  30,  31,  32,  33,  34,  35,

/*30        INS  HME  PUP  TAB   Q    W    E    R    T    Y  */
            36,  37,  38,  44,  45,  46,  47,  48,  49,  50,

/*40         U    I    O    P    [    ]    \   DEL  END  PDN */
            51,  52,  53,  54,  55,  56,  57,  58,  59,  60,

/*50        CAP   A    S    D    F    G    H    J    K    L  */
            66,  67,  68,  69,  70,  71,  72,  73,  74,  75,

/*60         ;    '   ENT  LSH   Z    X    C    V    B    N  */
            76,  77,  78,  83,  84,  85,  86,  87,  88,  89,

/*70         M    ,    .    /   RSH  UP  LCTL LWIN LALT SPC  */
            90,  91,  92,  93,  94,  96, 103, 104, 105, 106,

/*80       RALT RFNC MENU RCTL  LFT  DWN  RGT  NLK  NM/  NM* */
           107, 108, 109, 110, 111, 112, 113,  39,  40,  41,

/*90        NM-  NM+ NETR  NM1  NM2  NM3  NM4  NM5  NM6  NM7 */
            42,  64, 101,  98,  99, 100,  79,  80,  81,  61,

/*100       NM8  NM9  NM0  NM.  PRV  PLY  NXT  MTE  R1   R2  */
            62,  63, 114, 115,  18,  19,  20, 118, 176, 177,

/*110       R3   R4   R5   R6   R7   R8   R9   L1   L2   L3  */
           178, 179, 180, 181, 182, 183, 184, 160, 161, 162,

/*120       L4   L5   L6   L7   L8   L9   GM   M1   M2   M3  */
           163, 164, 165, 166, 167, 168,   0,  21,  43,  65,

/*130       M4   M5  */
            82, 102
};

EVGAKeyboardController::EVGAKeyboardController(hid_device* dev_handle, const char* path, uint16_t kb_pid, std::string dev_name)
{
    dev                     = dev_handle;
    location                = path;
    name                    = dev_name;
    pid                     = kb_pid;

    SetSleepTime();
}

EVGAKeyboardController::~EVGAKeyboardController()
{
    hid_close(dev);
}

std::string EVGAKeyboardController::GetName()
{
    return(name);
}

std::string EVGAKeyboardController::GetSerial()
{
    wchar_t serial_string[HID_MAX_STR];
    int ret = hid_get_serial_number_string(dev, serial_string, HID_MAX_STR);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

std::string EVGAKeyboardController::GetLocation()
{
    return("HID: " + location);
}

uint16_t EVGAKeyboardController::GetPid()
{
    return(pid);
}

void  EVGAKeyboardController::SetLedsDirect(std::vector<RGBColor> colors)
{
    uint8_t buffer[EVGA_KEYBOARD_CONTROLLER_ID_6_SIZE] = { 0x06, 0xEA, 0x02, 0x01 };

    /*-----------------------------------------------------------------*\
    | Set up Direct packet                                              |
    |   packet_map is the index of the Key from full_matrix_map and     |
    |   the value is the position in the direct packet buffer           |
    \*-----------------------------------------------------------------*/
    for(size_t i = 0; i < colors.size(); i++)
    {
        RGBColor key            = colors[i];
        uint16_t offset         = EVGA_KB_ZONE_BYTE + (packet_map[i] * 4);

        buffer[offset + 0]      = EVGA_KEYBOARD_CONTROLLER_BRIGHTNESS_MAX;
        buffer[offset + 1]      = RGBGetRValue(key);
        buffer[offset + 2]      = RGBGetGValue(key);
        buffer[offset + 3]      = RGBGetBValue(key);
    }

    buffer[EVGA_KB_CRC_BYTE]    = GetChecksum(&buffer[8], EVGA_KEYBOARD_CONTROLLER_ID_6_SIZE - EVGA_KB_ZONE_BYTE);
    hid_send_feature_report(dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_6_SIZE);
}

void EVGAKeyboardController::SaveMode()
{
    uint8_t buffer[EVGA_KEYBOARD_CONTROLLER_ID_4_SIZE] = { 0x04, 0xEA, 0x02, 0x12 };

    hid_send_feature_report(dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_4_SIZE);
}

void EVGAKeyboardController::SetMode(uint8_t mode, uint16_t speed, uint8_t brightness,
                                       uint8_t direction, std::vector<RGBColor> colors)
{
    SetHWModes();
    SendMode(mode, direction);
    SendColour(mode, speed, brightness, direction, colors);
}

void EVGAKeyboardController::GetStatus(mode *mode)
{
    /*-----------------------------------------------------------------*\
    | Gets the status of mode mode->value from the keyboard and then    |
    |   sets Colors, Brightness, Speed, Direction for the mode.         |
    \*-----------------------------------------------------------------*/
    uint8_t buffer[EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE] = { 0x07, 0xEA, 0x02, 0x0C, 0x01 };
    buffer[EVGA_KB_MODE_BYTE] += mode->value;

    hid_send_feature_report(dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE);
    int result = hid_get_feature_report (dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE);

    /*-----------------------------------------------------------------*\
    | If the read is successful fill in value from the packet           |
    \*-----------------------------------------------------------------*/
    if(result > 0)
    {
        switch(mode->value)
        {
            case EVGA_KEYBOARD_CONTROLLER_MODE_STATIC:
                mode->brightness = FindColours(&buffer[EVGA_KB_SPEED_LSB], mode->colors_max, mode->colors);
                break;

            case EVGA_KEYBOARD_CONTROLLER_MODE_BREATHING:
                mode->brightness = buffer[27];
                mode->speed      = buffer[EVGA_KB_SPEED_LSB] << 8 | buffer[EVGA_KB_SPEED_MSB];
                mode->colors.push_back(ToRGBColor(buffer[28], buffer[29], buffer[30]));
                mode->colors.push_back(ToRGBColor(buffer[33], buffer[34], buffer[35]));
                break;

            case EVGA_KEYBOARD_CONTROLLER_MODE_PULSE:
                mode->brightness = FindColours(&buffer[33], buffer[EVGA_KB_COLORS_SZ], mode->colors);
                mode->speed      = buffer[EVGA_KB_SPEED_LSB] << 8 | buffer[EVGA_KB_SPEED_MSB];
                break;

            case EVGA_KEYBOARD_CONTROLLER_MODE_SPIRAL:
            case EVGA_KEYBOARD_CONTROLLER_MODE_RAINBOW:
            case EVGA_KEYBOARD_CONTROLLER_MODE_TRIGGER:
                mode->direction  = FindDirection(mode->value, buffer[11] + buffer[12]);
                mode->brightness = FindColours(&buffer[27], buffer[EVGA_KB_COLORS_SZ], mode->colors);
                mode->speed      = buffer[EVGA_KB_SPEED_LSB] << 8 | buffer[EVGA_KB_SPEED_MSB];
                break;

            case EVGA_KEYBOARD_CONTROLLER_MODE_STAR:
                mode->brightness = buffer[EVGA_KB_COLORS_SZ];
                mode->speed      = buffer[EVGA_KB_SPEED_LSB];
                break;
        }
        LOG_DEBUG("[%s] Mode %d Setup with %d colours @ %04X speed and %02X brightness", name.c_str(), mode->value, mode->colors.size(), mode->speed, mode->brightness);
    }
    else
    {
        LOG_INFO("[%s] An error occured reading data for mode %d", name.c_str(), mode->value);
    }
}

void EVGAKeyboardController::SetHWModes()
{
    /*-----------------------------------------------------------------*\
    | Send Initialise Hardware Modes                                    |
    \*-----------------------------------------------------------------*/
    uint8_t buffer[EVGA_KEYBOARD_CONTROLLER_ID_6_SIZE] = { 0x06, 0xEA, 0x02 };

    hid_send_feature_report(dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_6_SIZE);
}

void EVGAKeyboardController::SendMode(uint8_t mode, uint8_t direction)
{
    /*-----------------------------------------------------------------*\
    | Send Mode                                                         |
    \*-----------------------------------------------------------------*/
    uint8_t buffer[EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE] = { 0x07, 0xEA, 0x02, 0x0C };

    buffer[EVGA_KB_ZONE_BYTE]   = mode;
    buffer[EVGA_KB_DIR_BYTE]    = direction;

    buffer[EVGA_KB_CRC_BYTE] = GetChecksum(&buffer[8], EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE - EVGA_KB_ZONE_BYTE);
    hid_send_feature_report(dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE);
}

void EVGAKeyboardController::SendColour(uint8_t mode, uint16_t speed, uint8_t brightness, uint8_t direction, std::vector<RGBColor> colors)
{
    uint8_t buffer[EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE] = { 0x07, 0xEA, 0x02, 0x0C };

    speed                      *= (mode == EVGA_KEYBOARD_CONTROLLER_MODE_TRIGGER) ? 10 : 100;
    buffer[EVGA_KB_MODE_BYTE]  += mode;
    buffer[EVGA_KB_ZONE_BYTE]   = EVGA_KEYBOARD_CONTROLLER_ZONE_ALL_KEYS; //zone

    /*-----------------------------------------------------------------*\
    | Static mode does not have speed but it will be overwritten        |
    \*-----------------------------------------------------------------*/
    buffer[EVGA_KB_SPEED_LSB]   = speed & 0xFF;
    buffer[EVGA_KB_SPEED_MSB]   = speed >> 8;
    /*-----------------------------------------------------------------*\
    | Static, Breathing and Star modes have fixed colour sizes          |
    |   buffer[26] will be overwritten for these modes                  |
    \*-----------------------------------------------------------------*/
    buffer[EVGA_KB_COLORS_SZ]   = (uint8_t)colors.size();

    switch(mode)
    {
        case EVGA_KEYBOARD_CONTROLLER_MODE_STATIC:
            FillColours(&buffer[24], brightness, colors);
            break;

        case EVGA_KEYBOARD_CONTROLLER_MODE_BREATHING:
            for(size_t i = 0; i < colors.size(); i++)
            {
                uint8_t offset      = (uint8_t)(26 + (i * 5));

                buffer[offset + 0]  = 0x0A;
                buffer[offset + 1]  = brightness;
                buffer[offset + 2]  = RGBGetRValue(colors[i]);
                buffer[offset + 3]  = RGBGetGValue(colors[i]);
                buffer[offset + 4]  = RGBGetBValue(colors[i]);
            }
            break;

        case EVGA_KEYBOARD_CONTROLLER_MODE_PULSE:
            /*-----------------------------------------------------------------*\
            | Buffer 27 thru 32 could be defining a "Transition Color"          |
            |   27      Identifier ??                                           |
            |   28      Time in ms ??                                           |
            |   29 - 32 Looks to be "Black"                                     |
            \*-----------------------------------------------------------------*/
            buffer[27]          = 0x0A;
            buffer[28]          = 0x0A;
            buffer[29]          = 0xFF;

            FillColours(&buffer[33], brightness, colors);
            break;

        case EVGA_KEYBOARD_CONTROLLER_MODE_SPIRAL:
            buffer[11]          = direction;

            FillColours(&buffer[27], brightness, colors);
            break;

        case EVGA_KEYBOARD_CONTROLLER_MODE_RAINBOW:
            buffer[12]          = direction;

            FillColours(&buffer[27], brightness, colors);
            break;

        case EVGA_KEYBOARD_CONTROLLER_MODE_TRIGGER:
            buffer[3]--;        //Why EVGA??
            buffer[12]          = direction;

            FillColours(&buffer[27], brightness, colors);
            break;

        case EVGA_KEYBOARD_CONTROLLER_MODE_STAR:
            buffer[3]++;        //Why EVGA??
            buffer[26]          = brightness;
            break;
    }

    buffer[EVGA_KB_CRC_BYTE]    = GetChecksum(&buffer[8], EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE - EVGA_KB_ZONE_BYTE);
    hid_send_feature_report(dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE);
}

void EVGAKeyboardController::FillColours(uint8_t * buffer, uint8_t brightness, std::vector<RGBColor> colors)
{
    for(size_t i = 0; i < colors.size(); i++)
    {
        uint8_t offset      = (uint8_t)(i * 4);

        buffer[offset + 0]  = brightness;
        buffer[offset + 1]  = RGBGetRValue(colors[i]);
        buffer[offset + 2]  = RGBGetGValue(colors[i]);
        buffer[offset + 3]  = RGBGetBValue(colors[i]);
    }
}

uint8_t EVGAKeyboardController::GetChecksum(uint8_t * data, size_t count)
{
    uint8_t checksum = 0;

    for(size_t i = 0; i < count; i++)
    {
        checksum -= data[i];
    }

    return(checksum);
}

uint8_t EVGAKeyboardController::FindDirection(uint8_t mode, uint8_t direction)
{
    /*-----------------------------------------------------------------*\
    | Converts EVGAs buffer direction value to OpenRGB's directions     |
    \*-----------------------------------------------------------------*/
    uint8_t temp = 0;

    for(size_t i = 0; i < sizeof(direction_map[mode]); i++)
    {
        if(direction_map[mode][i] == direction)
        {
            temp = direction_map[mode][i];
            break;
        }
    }

    return(temp);
}

uint8_t EVGAKeyboardController::FindColours(uint8_t * data, uint8_t count, std::vector<RGBColor> &colors)
{
    /*-----------------------------------------------------------------*\
    | Converts EVGAs buffer colours to OpenRGB's colours                |
    \*-----------------------------------------------------------------*/
    colors.clear();

    for(size_t i = 0; i < count; i++)
    {
        uint8_t offset = (uint8_t)(i * 4);

        colors.push_back(ToRGBColor(data[offset + 1],data[offset + 2],data[offset + 3]));
    }

    return(data[0]);
}

uint8_t EVGAKeyboardController::GetMode()
{
    static const uint16_t index = 1289;
    NFIPacket();
    /*-----------------------------------------------------------------*\
    | Requests the current set mode from the keyboard                   |
    |                                                                   |
    |   Request:  04 ea 02 07 01 00 00 6c 00 00 00 00 00 00 00 00 00    |
    |   Response: 04 ea 02 07 01 00 c0 6c 04 00 00 00 00 00 00 00 00    |
    |                     Key Count?? ⇗                                 |
    \*-----------------------------------------------------------------*/
    uint8_t buffer[EVGA_KEYBOARD_CONTROLLER_ID_3_SIZE] = { 0x08, 0xEA, 0x02, 0x01, 0xFE };

    hid_send_feature_report(dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_3_SIZE);
    int result = hid_get_feature_report (dev, buffer, EVGA_KEYBOARD_CONTROLLER_ID_3_SIZE);

    if(result > 0)
    {
        LOG_DEBUG("[%s] Returned mode %02X - %02X %02X %02X %02X %02X", name.c_str(), buffer[index], buffer[index-2], buffer[index-1], buffer[index], buffer[index+1], buffer[index+2]);
        return(buffer[index]);
    }
    else
    {
        LOG_INFO("[%s] An error occured reading current mode", name.c_str());
        return(0);
    }
}

void EVGAKeyboardController::NFIPacket()
{
    /*-----------------------------------------------------------------*\
    | Not sure what this packet is doing but it appears to be           |
    |   required to retrieve the current mode from the (first) profile  |
    \*-----------------------------------------------------------------*/
    uint8_t buffer1[EVGA_KEYBOARD_CONTROLLER_ID_4_SIZE] = { 0x04, 0xEA, 0x02, 0x33, 0x00, 0x00, 0x00, 0x01 };
    hid_send_feature_report(dev, buffer1, EVGA_KEYBOARD_CONTROLLER_ID_4_SIZE);

    uint8_t buffer2[EVGA_KEYBOARD_CONTROLLER_ID_4_SIZE] = { 0x04, 0xEA, 0x02, 0x06, 0x01 };
    hid_send_feature_report(dev, buffer2, EVGA_KEYBOARD_CONTROLLER_ID_4_SIZE);
}

void EVGAKeyboardController::SetSleepTime()
{
    /*-----------------------------------------------------------------*\
    | After a set timer the LED lighting on this keyboard will "sleep"  |
    \*-----------------------------------------------------------------*/
    const uint16_t minutes      = 0;    //Max value in Unleashed is 300min
    const uint8_t  multiply     = 0xEA;

    uint8_t buffer1[EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE] = { 0x07, 0xEA, 0x02, 0x1B, 0x00, 0x00, 0x00, 0xFE, 0x02 };
    hid_send_feature_report(dev, buffer1, EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE);

    uint8_t buffer2[EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE] = { 0x07, 0xEA, 0x02, 0x03 };

    uint32_t sleep              = minutes * multiply;
    buffer2[EVGA_KB_ZONE_BYTE]  = (sleep == 0)  ? 0 : EVGA_KEYBOARD_CONTROLLER_ZONE_ALL_KEYS; //zone
    buffer2[9]                  = sleep & 0xFF;
    buffer2[10]                 = (sleep >> 8)  & 0xFF;
    buffer2[11]                 = (sleep >> 16) & 0xFF;

    buffer2[EVGA_KB_CRC_BYTE]   = GetChecksum(&buffer2[8], EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE - EVGA_KB_ZONE_BYTE);
    hid_send_feature_report(dev, buffer2, EVGA_KEYBOARD_CONTROLLER_ID_7_SIZE);
}

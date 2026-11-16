/*---------------------------------------------------------*\
| CMKeyboardV3Controller.cpp                                |
|                                                           |
|   Driver for Cooler Master V3 keyboards                   |
|   Derived from CMKeyboardV2Controller.cpp                 |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|   Wikus van Rooyen                            14 Nov 2026 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <chrono>
#include <thread>
#include "CMKeyboardV3Controller.h"
#include "LogManager.h"

CMKeyboardV3Controller::CMKeyboardV3Controller(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name) : CMKeyboardAbstractController(dev_handle, dev_info, dev_name)
{
    m_profileId         = 0;
    m_bDongle           = (m_productId == COOLERMASTER_KEYBOARD_CK721_DONGLE_PID     ||
                           m_productId == COOLERMASTER_KEYBOARD_CK721LINE_DONGLE_PID ||
                           m_productId == COOLERMASTER_KEYBOARD_MK721_DONGLE_PID);

    if(m_bDongle)
    {
        m_writeSize     = CM_KEYBOARD_V3_DONGLE_WRITE_SIZE;
        m_payloadSize   = CM_KEYBOARD_V3_DONGLE_PAYLOAD_SIZE;
    }
    else
    {
        m_writeSize     = CM_KEYBOARD_V3_WIRED_WRITE_SIZE;
        m_payloadSize   = CM_KEYBOARD_V3_WIRED_PAYLOAD_SIZE;
    }

    _ReadActiveProfile();
}

CMKeyboardV3Controller::~CMKeyboardV3Controller()
{
}

/*---------------------------------------------------------*\
| Keyboard does not return layout, assume ANSI.             |
\*---------------------------------------------------------*/
KEYBOARD_LAYOUT CMKeyboardV3Controller::GetKeyboardLayout()
{
    return(KEYBOARD_LAYOUT_ANSI_QWERTY);
}

void CMKeyboardV3Controller::Initialize()
{

}

void CMKeyboardV3Controller::Shutdown()
{

}

void CMKeyboardV3Controller::SetLEDControl(bool /*bManual*/)
{
}

void CMKeyboardV3Controller::SetLeds(std::vector<led> leds, std::vector<RGBColor> colors)
{
    RGBColor rgbColorMap[CM_MAX_LEDS];
    memset(rgbColorMap, 0, sizeof(RGBColor)*CM_MAX_LEDS);

    for(size_t i = 0; i < leds.size(); i++)
    {
        rgbColorMap[leds[i].value] = colors[i];
    }

    /*---------------------------------------------------------*\
    | byte[0-1] = address                                       |
    | byte[2-3] = data length                                   |
    | byte[4+]  = R, G, B, 0xFF for each LED                    |
    \*---------------------------------------------------------*/
    const uint16_t nLength = CM_KEYBOARD_V3_LED_COUNT * 4;

    std::vector<uint8_t> data =
    {
        0x00, 0x00, (uint8_t)(nLength & 0xFF), (uint8_t)(nLength >> 8)
    };

    for(size_t i = 0; i < CM_KEYBOARD_V3_LED_COUNT; i++)
    {
        data.push_back(RGBGetRValue(rgbColorMap[i]));
        data.push_back(RGBGetGValue(rgbColorMap[i]));
        data.push_back(RGBGetBValue(rgbColorMap[i]));
        data.push_back(0xFF);
    }

    std::lock_guard<std::mutex> guard(m_mutex);

    _SendCommand(CM_KB_V3_CMD_WRITE_CUSTOM_RGB, data);
}

void CMKeyboardV3Controller::SetSingleLED(uint8_t /*in_led*/, RGBColor /*in_color*/)
{
}

void CMKeyboardV3Controller::_SetEffectMode(uint8_t effectId)
{
    _WriteRegister(m_profileId * CM_KEYBOARD_V3_PROFILE_SIZE + CM_KEYBOARD_V3_EFFECT_MODE_REG, {effectId});
}

/*---------------------------------------------------------*\
| Effects are stored per profile, read the active one.      |
| Reply:                                                    |
| byte[0]   = 0x02                                          |
| byte[1]   = 0x81                                          |
| byte[2-5] = register address and length                   |
| byte[6]   = active profile                                |
\*---------------------------------------------------------*/
void CMKeyboardV3Controller::_ReadActiveProfile()
{
    uint8_t packet[CM_KEYBOARD_V3_WIRED_WRITE_SIZE];

    while(hid_read_timeout(m_pDev, packet, m_writeSize - 1, 0) > 0)
    {
    }

    _SendCommand(CM_KB_V3_CMD_READ_REG, {CM_KEYBOARD_V3_PROFILE_REG & 0xFF, CM_KEYBOARD_V3_PROFILE_REG >> 8, 0x01, 0x00});

    if(hid_read_timeout(m_pDev, packet, m_writeSize - 1, CM_KEYBOARD_V3_READ_TIMEOUT) > 6 &&
       packet[0] == CM_KB_V3_CMD_READ_REG && packet[6] < CM_KEYBOARD_V3_PROFILE_COUNT)
    {
        m_profileId     = packet[6];

        LOG_INFO("[%s] Using onboard profile %d.", m_deviceName.c_str(), m_profileId + 1);
    }
    else
    {
        LOG_WARNING("[%s] Could not read the active profile, using profile %d.", m_deviceName.c_str(), m_profileId + 1);
    }
}

/*---------------------------------------------------------*\
| byte[0-1] = register address                              |
| byte[2-3] = data length                                   |
| byte[4+]  = data                                          |
\*---------------------------------------------------------*/
void CMKeyboardV3Controller::_WriteRegister(uint16_t address, std::vector<uint8_t> data)
{
    std::vector<uint8_t> buf =
    {
        (uint8_t)(address & 0xFF), (uint8_t)(address >> 8),
        (uint8_t)(data.size() & 0xFF), (uint8_t)(data.size() >> 8)
    };

    buf.insert(buf.end(), data.begin(), data.end());

    _SendCommand(CM_KB_V3_CMD_WRITE_REG, buf);
}

/*---------------------------------------------------------*\
| byte[0]  = report ID                                      |
| byte[1]  = command                                        |
| byte[2]  = 0x80 | packet count on the first packet,       |
|            packet index on the following packets          |
| byte[3+] = data                                           |
\*---------------------------------------------------------*/
void CMKeyboardV3Controller::_SendCommand(uint8_t command, std::vector<uint8_t> data)
{
    uint8_t packet[CM_KEYBOARD_V3_WIRED_WRITE_SIZE];
    size_t  nPackets = (data.size() + m_payloadSize - 1) / m_payloadSize;

    std::lock_guard<std::mutex> guard(m_mutexSendCommand);

    for(size_t i = 0; i < nPackets; i++)
    {
        memset(packet, 0, CM_KEYBOARD_V3_WIRED_WRITE_SIZE);

        packet[1] = command;
        packet[2] = (i == 0) ? (uint8_t)(0x80 | nPackets) : (uint8_t)i;

        for(size_t j = 0; j < m_payloadSize; j++)
        {
            size_t idx = i * m_payloadSize + j;

            if(idx < data.size())
            {
                packet[3 + j] = data[idx];
            }
        }

        if(hid_write(m_pDev, packet, m_writeSize) < 0)
        {
            LOG_ERROR("[%s] _SendCommand() failed.", m_deviceName.c_str());
            return;
        }

        /*---------------------------------------------------------*\
        | The dongle sometimes drops packets so add a delay.        |
        \*---------------------------------------------------------*/
        std::this_thread::sleep_for(std::chrono::milliseconds(m_bDongle ? CM_KEYBOARD_V3_DONGLE_DELAY : CM_KEYBOARD_TIMEOUT_SHORT));
    }
}

void CMKeyboardV3Controller::SetCustomMode()
{

}

void CMKeyboardV3Controller::SetMode(mode selectedMode)
{
    uint8_t cColor1_R   = 0;
    uint8_t cColor1_G   = 0;
    uint8_t cColor1_B   = 0;
    uint8_t cColor2_R   = 0;
    uint8_t cColor2_G   = 0;
    uint8_t cColor2_B   = 0;

    RGBColor color1     = 0;
    RGBColor color2     = 0;
    uint8_t cSpeed      = 0;

    if(selectedMode.speed >= 1 && selectedMode.speed <= 5)
    {
        cSpeed          = (uint8_t)selectedMode.speed - 1;
    }

    uint8_t cBright     = (uint8_t)selectedMode.brightness;
    uint8_t cDirection  = 0;
    uint8_t cBackground = 0;
    uint8_t cIndex      = 0x06;
    uint8_t effectId    = selectedMode.value;

    if(selectedMode.colors.size() >= 1)
    {
        color1          = selectedMode.colors[0];
        cColor1_R       = (uint8_t)RGBGetRValue(color1);
        cColor1_G       = (uint8_t)RGBGetGValue(color1);
        cColor1_B       = (uint8_t)RGBGetBValue(color1);
    }

    if(selectedMode.colors.size() >= 2)
    {
        color2          = selectedMode.colors[1];
        cColor2_R       = (uint8_t)RGBGetRValue(color2);
        cColor2_G       = (uint8_t)RGBGetGValue(color2);
        cColor2_B       = (uint8_t)RGBGetBValue(color2);
        cBackground     = 1;
    }

    switch(selectedMode.direction)
    {
        case MODE_DIRECTION_LEFT:
            cDirection      = 0x01;
            break;

        case MODE_DIRECTION_RIGHT:
            cDirection      = 0x00;
            break;

        case MODE_DIRECTION_UP:
            cDirection      = 0x02;
            break;

        case MODE_DIRECTION_DOWN:
            cDirection      = 0x03;
            break;

        default:
            break;
    }

    /*---------------------------------------------------------*\
    | Direction is inverted for these modes.                    |
    \*---------------------------------------------------------*/
    if(effectId == CM_KB_V3_CIRCLE_SPECTRUM || effectId == CM_KB_V3_REACTIVE_TORNADO)
    {
        cDirection      = (cDirection == 0) ? 1 : 0;
    }

    switch(effectId)
    {
        case CM_KB_V3_SNOW:
            cIndex          = 0x07;
            break;

        case CM_KB_V3_WATER_RIPPLE:
            cIndex          = 0x04;
            break;

        case CM_KB_V3_FIREBALL:
            cIndex          = 0x01;
            break;

        case CM_KB_V3_HEARTBEAT:
            cIndex          = 0x00;
            break;

        default:
            break;
    }

    std::lock_guard<std::mutex> guard(m_mutex);

    _ReadActiveProfile();

    _SetEffectMode(effectId);

    if(effectId != CM_KB_V3_OFF)
    {
        /*---------------------------------------------------------*\
        | byte[0]   = brightness                                    |
        | byte[1]   = background brightness                         |
        | byte[2]   = speed | direction << 4 | background << 6      |
        | byte[3-5] = color                                         |
        | byte[6-8] = background color                              |
        | byte[9]   = color index                                   |
        \*---------------------------------------------------------*/
        std::vector<uint8_t> data =
        {
            cBright, cBright, (uint8_t)(cSpeed | (cDirection << 4) | (cBackground << 6)),
            cColor1_R, cColor1_G, cColor1_B,
            cColor2_R, cColor2_G, cColor2_B,
            cIndex
        };

        _WriteRegister(m_profileId * CM_KEYBOARD_V3_PROFILE_SIZE + effectId * CM_KEYBOARD_V3_EFFECT_SIZE, data);
    }
}

struct stCMKeyboardV3_mode CMKeyboardV3_modes[] =
{
/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*\
|                                                    Speed        Brightness                   Colors                                                               |
|     NAME               VALUE                       MIN MAX  SET MAX    DIR                   NUM  COLOR_MODE                  FLAGS                               |
\*-----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    {"Custom",           CM_KB_V3_CUSTOM,            1,  5,   3,  0xFF,  0,                    0,   MODE_COLORS_PER_LED,        MODE_FLAG_HAS_PER_LED_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS},
    {"Static",           CM_KB_V3_STATIC,            1,  5,   3,  0xFF,  0,                    1,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS},
    {"Rainbow Wave",     CM_KB_V3_WAVE,              1,  5,   3,  0xFF,  MODE_DIRECTION_RIGHT, 0,   MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED |
                                                                                                                                MODE_FLAG_HAS_DIRECTION_LR |
                                                                                                                                MODE_FLAG_HAS_DIRECTION_UD},
    {"Crosshair",        CM_KB_V3_CROSS,             1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Reactive Fade",    CM_KB_V3_REACTIVE_FADE,     1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Stars",            CM_KB_V3_STARS,             1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Spectrum Cycle",   CM_KB_V3_CYCLE,             1,  5,   3,  0xFF,  0,                    0,   MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Breathing",        CM_KB_V3_BREATHE,           1,  5,   3,  0xFF,  0,                    1,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Ripple",           CM_KB_V3_RIPPLE,            1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Fireball",         CM_KB_V3_FIREBALL,          1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Water Ripple",     CM_KB_V3_WATER_RIPPLE,      1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Reactive Punch",   CM_KB_V3_REACTIVE_PUNCH,    1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Snowing",          CM_KB_V3_SNOW,              1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Heartbeat",        CM_KB_V3_HEARTBEAT,         1,  5,   3,  0xFF,  0,                    2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                                                MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED},
    {"Circle Spectrum",  CM_KB_V3_CIRCLE_SPECTRUM,   1,  5,   3,  0xFF,  MODE_DIRECTION_RIGHT, 0,   MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED |
                                                                                                                                MODE_FLAG_HAS_DIRECTION_LR},
    {"Reactive Tornado", CM_KB_V3_REACTIVE_TORNADO,  1,  5,   3,  0xFF,  MODE_DIRECTION_RIGHT, 0,   MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                                                MODE_FLAG_HAS_SPEED |
                                                                                                                                MODE_FLAG_HAS_DIRECTION_LR},
    {"Off",              CM_KB_V3_OFF,               0,  0,   0,  0xFF,  MODE_DIRECTION_LEFT,  0,   MODE_COLORS_NONE,           0},
    {0,                  CM_KB_V3_NONE,              0,  0,   0,  0,     0,                    0,   0,                          0},
};

void CMKeyboardV3Controller::InitializeModes(std::vector<mode> &modes)
{
    stCMKeyboardV3_mode * pCurrentMode = CMKeyboardV3_modes;

    while(pCurrentMode->value != CM_KB_V3_NONE)
    {
        mode m;

        m.name           = std::string(pCurrentMode->name);
        m.value          = pCurrentMode->value;
        m.flags          = pCurrentMode->flags;
        m.color_mode     = pCurrentMode->color_mode;
        m.speed_min      = pCurrentMode->speed_min;
        m.speed_max      = pCurrentMode->speed_max;
        m.speed          = pCurrentMode->speed;
        m.brightness_min = 0x00;
        m.brightness_max = 0xFF;
        m.brightness     = 0xFF;
        m.colors_min     = pCurrentMode->nColors;
        m.colors_max     = pCurrentMode->nColors;
        m.colors.resize(pCurrentMode->nColors);
        m.direction      = pCurrentMode->direction;

        /*---------------------------------------------------------*\
        | resize() defaults to black, start on white instead.       |
        \*---------------------------------------------------------*/
        if(pCurrentMode->nColors > 0)
        {
            m.colors[0]  = ToRGBColor(0xFF, 0xFF, 0xFF);
        }

        modes.push_back(m);

        pCurrentMode++;
    }
}

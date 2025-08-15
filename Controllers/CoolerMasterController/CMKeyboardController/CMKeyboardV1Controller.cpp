/*---------------------------------------------------------*\
| CMKeyboardV1Controller.cpp                                |
|                                                           |
|   Driver for Cooler Master MasterKeys (V1) keyboards      |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cmath>
#include "CMKeyboardV1Controller.h"
#include "LogManager.h"

CMKeyboardV1Controller::CMKeyboardV1Controller(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name) : CMKeyboardAbstractController(dev_handle, dev_info, dev_name)
{
    m_sFirmwareVersion = _GetFirmwareVersion();
}

CMKeyboardV1Controller::~CMKeyboardV1Controller()
{
}

void CMKeyboardV1Controller::Initialize()
{
    SetLEDControl(true);
}

void CMKeyboardV1Controller::SetActiveEffect(uint8_t effectId)
{
    SendCommand({0x51, 0x28, 0x00, 0x00, effectId});
}

uint8_t CMKeyboardV1Controller::GetActiveEffect()
{
    std::vector<uint8_t> data = SendCommand({0x52, 0x28});

    return data[4];
}

void CMKeyboardV1Controller::SetEffect(uint8_t effectId, uint8_t p1, uint8_t p2, uint8_t p3, RGBColor color1, RGBColor color2)
{
    std::vector<uint8_t> data;

    data.push_back(0x51);
    data.push_back(0x2C);
    data.push_back(0x00); // multilayer_mode - NOT SUPPORTED
    data.push_back(0x00);
    data.push_back(effectId);
    data.push_back(p1);
    data.push_back(p2);
    data.push_back(p3);
    data.push_back(0xFF);
    data.push_back(0xFF);
    data.push_back(RGBGetRValue(color1));
    data.push_back(RGBGetGValue(color1));
    data.push_back(RGBGetBValue(color1));
    data.push_back(RGBGetRValue(color2));
    data.push_back(RGBGetGValue(color2));
    data.push_back(RGBGetBValue(color2));

    /*-------------------------------------------*\
    | Likely a bit mask for each LEDs.            |
    | 3 bits per LED x 127 possible LEDs ~48 bytes|
    \*-------------------------------------------*/
    for(size_t i = 0; i < 48; i++)
    {
        data.push_back(0xFF);
    }

    SetCustomMode();
    SetActiveEffect(effectId);
    SendCommand(data);
}

void CMKeyboardV1Controller::SetCustomMode()
{
    SetControlMode(0x01);
}

void CMKeyboardV1Controller::SetMode(mode selectedMode)
{
    RGBColor color1     = 0;
    RGBColor color2     = 0;
    uint8_t cSpeed      = 0;
    uint8_t cDirection  = 0;
    uint8_t effectId    = selectedMode.value;
    bool bModeRandom    = false;

    if(selectedMode.colors.size() >= 1)
    {
        color1 = selectedMode.colors[0];
    }

    if(selectedMode.colors.size() >= 2)
    {
        color2 = selectedMode.colors[1];
    }

    if(selectedMode.color_mode == MODE_COLORS_RANDOM)
    {
        bModeRandom = true;
    }

    int selectedEffect = mapModeValueEffect[effectId];
    cSpeed = selectedMode.speed;

    switch(selectedMode.direction)
    {
        case MODE_DIRECTION_LEFT:
        case MODE_DIRECTION_HORIZONTAL:
        cDirection = 0x00;
        break;

        case MODE_DIRECTION_RIGHT:
        cDirection = 0x04;
        break;

        case MODE_DIRECTION_UP:
        case MODE_DIRECTION_VERTICAL:
        cDirection = 0x06;
        break;

        case MODE_DIRECTION_DOWN:
        cDirection = 0x02;
        break;

        default:
        break;
    }

    switch(selectedEffect)
    {
        case DIRECT:
        case STATIC:
        {
            SetEffect(effectId, 0, 0, 0, color1, color2);
        }
        break;

        case CROSS:
        case BREATHE:
        case REACTIVE_PUNCH:
        case CIRCLE_SPECTRUM:
        case SNAKE:
        {
            SetEffect(effectId, cSpeed, 0, 0xFF, color1, color2);
        }
        break;

        case WAVE:
        {
            SetEffect(effectId, cSpeed, cDirection, 0xFF, color1, color2);
        }
        break;

        case RIPPLE:
        {
            SetEffect(effectId, cSpeed, bModeRandom ? 0x80 : 0x00, 0xFF, color1, color2);
        }
        break;

        case RAINDROPS:
        {
            SetEffect(effectId, 0x6a, 0x00, cSpeed, color1, color2);
        }
        break;

        case STARS:
        {
            SetEffect(effectId, cSpeed, 0x00, 0x10, color1, color2);
        }
        break;

        default:
        break;
    }
}

void CMKeyboardV1Controller::InitializeModes(std::vector<mode> &modes)
{
    mode Direct;
    Direct.name                 = "Direct";
    Direct.value                = 0x02;
    Direct.flags                = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode           = MODE_COLORS_PER_LED;

    modes.push_back(Direct);

    mapModeValueEffect[0x02]    = DIRECT;

    mode Static;
    Static.name                 = "Static";
    Static.value                = 0x00;
    Static.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.colors.resize(1);
    modes.push_back(Static);
    mapModeValueEffect[0x00]    = STATIC;

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = 0x01;
    Breathing.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.speed_min         = 0x46;
    Breathing.speed_max         = 0x27;
    Breathing.speed             = 0x36;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.colors.resize(1);
    modes.push_back(Breathing);
    mapModeValueEffect[0x01]    = BREATHE;

    mode Cycle;
    Cycle.name                  = "Spectrum Cycle";
    Cycle.value                 = 0x02;
    Cycle.flags                 = MODE_FLAG_HAS_SPEED;
    Cycle.color_mode            = MODE_COLORS_NONE;
    Cycle.speed_min             = 0x96;
    Cycle.speed_max             = 0x68;
    Cycle.speed                 = 0x7F;
    modes.push_back(Cycle);
    mapModeValueEffect[0x02]    = CIRCLE_SPECTRUM;

    mode Reactive;
    Reactive.name               = "Reactive";
    Reactive.value              = 0x03;
    Reactive.flags              = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Reactive.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Reactive.speed_min          = 0x3C;
    Reactive.speed_max          = 0x2F;
    Reactive.speed              = 0x35;
    Reactive.colors_min         = 2;
    Reactive.colors_max         = 2;
    Reactive.colors.resize(2);
    modes.push_back(Reactive);
    mapModeValueEffect[0x03]    = REACTIVE_PUNCH;

    mode Wave;
    Wave.name                   = "Rainbow Wave";
    Wave.value                  = 0x04;
    Wave.flags                  = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_DIRECTION_LR | MODE_FLAG_HAS_DIRECTION_UD;
    Wave.color_mode             = MODE_COLORS_MODE_SPECIFIC;
    Wave.speed_min              = 0x48;
    Wave.speed_max              = 0x2A;
    Wave.speed                  = 0x29;
    Wave.direction              = MODE_DIRECTION_LEFT;
    Wave.colors_min             = 1;
    Wave.colors_max             = 1;
    Wave.colors.resize(1);
    modes.push_back(Wave);
    mapModeValueEffect[0x04]    = WAVE;

    mode Ripple;
    Ripple.name                 = "Ripple Effect";
    Ripple.value                = 0x05;
    Ripple.flags                = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR | MODE_FLAG_HAS_SPEED;
    Ripple.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Ripple.speed_min            = 0x96;
    Ripple.speed_max            = 0x62;
    Ripple.speed                = 0x7C;
    Ripple.colors_min           = 2;
    Ripple.colors_max           = 2;
    Ripple.colors.resize(2);
    modes.push_back(Ripple);
    mapModeValueEffect[0x05]    = RIPPLE;

    mode Cross;
    Cross.name                  = "Cross";
    Cross.value                 = 0x06;
    Cross.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Cross.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Cross.speed_min             = 0x2A;
    Cross.speed_max             = 0x48;
    Cross.speed                 = 0x39;
    Cross.colors_min            = 2;
    Cross.colors_max            = 2;
    Cross.colors.resize(2);
    modes.push_back(Cross);
    mapModeValueEffect[0x06]    = CROSS;

    mode Raindrops;
    Raindrops.name              = "Raindrops";
    Raindrops.value             = 0x07;
    Raindrops.flags             = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Raindrops.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Raindrops.speed_min         = 0x40;
    Raindrops.speed_max         = 0x08;
    Raindrops.speed             = 0x24;
    Raindrops.colors_min        = 2;
    Raindrops.colors_max        = 2;
    Raindrops.colors.resize(2);
    modes.push_back(Raindrops);
    mapModeValueEffect[0x07]    = RAINDROPS;

    mode Stars;
    Stars.name                  = "Starfield";
    Stars.value                 = 0x08;
    Stars.flags                 = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    Stars.color_mode            = MODE_COLORS_MODE_SPECIFIC;
    Stars.speed_min             = 0x46;
    Stars.speed_max             = 0x32;
    Stars.speed                 = 0x3C;
    Stars.colors_min            = 2;
    Stars.colors_max = 2;
    Stars.colors.resize(2);
    modes.push_back(Stars);
    mapModeValueEffect[0x08]    = STARS;

    mode Snake;
    Snake.name                  = "Snake";
    Snake.value                 = 0x09;
    Snake.flags                 = MODE_FLAG_HAS_SPEED;
    Snake.color_mode            = MODE_COLORS_NONE;
    Snake.speed_min             = 0x48;
    Snake.speed_max             = 0x2A;
    Snake.speed                 = 0x39;
    modes.push_back(Snake);
    mapModeValueEffect[0x09]    = SNAKE;
}

struct cm_keyboard_effect CMKeyboardV1Controller::GetEffect(uint8_t effectId)
{
    std::vector<uint8_t> data;
    data.push_back(0x52);
    data.push_back(0x2C);
    data.push_back(0x00);
    data.push_back(0x00);
    data.push_back(effectId);

    data = SendCommand(data);
    struct cm_keyboard_effect response;

    response.effectId   = effectId;
    response.p1         = data[5];
    response.p2         = data[6];
    response.p3         = data[7];
    response.color1     = ToRGBColor(data[10], data[11], data[12]);
    response.color2     = ToRGBColor(data[13], data[14], data[15]);

    return response;
}

std::vector<uint8_t> CMKeyboardV1Controller::GetEnabledEffects()
{
    std::vector<uint8_t> data;

    data = SendCommand({0x52, 0x29});

    std::vector<uint8_t> effects;

    for(size_t i = 4; data[i] != 0xFF; i++)
    {
        effects.push_back(data[i]);
    }

    return effects;
}


void CMKeyboardV1Controller::SetLEDControl(bool bManual)
{
    uint8_t modeId = 0; // firmware

    if(bManual)
    {
        modeId = 0x02;  // manual
    }

    SetControlMode(modeId);
};

void CMKeyboardV1Controller::SetLeds(std::vector<led> leds, std::vector<RGBColor> colors)
{
    SetLEDControl(true);

    RGBColor rgbColorMap[CM_MAX_LEDS];
    memset(rgbColorMap, 0, sizeof(RGBColor)*CM_MAX_LEDS);

    for(size_t i = 0; i < leds.size(); i++)
    {
        rgbColorMap[leds[i].value] = colors[i];
    }

    RGBColor * pRGBColor = rgbColorMap;

    std::lock_guard<std::mutex> guard(m_mutex);

    for(size_t i = 0; i < 8; i++)
    {
        std::vector<uint8_t> data;
        data.push_back(0xC0);
        data.push_back(0x02);
        data.push_back((uint8_t)(i * 2));
        data.push_back(0x00);

        for(size_t j = 0; j < 16; j++)
        {
            data.push_back(RGBGetRValue(*pRGBColor));
            data.push_back(RGBGetGValue(*pRGBColor));
            data.push_back(RGBGetBValue(*pRGBColor));

            ++pRGBColor;
        }

        SendCommand(data);
    }
}

void CMKeyboardV1Controller::SetSingleLED(uint8_t in_led, RGBColor in_color)
{
    std::vector<uint8_t> data;
    data.push_back(0xC0);
    data.push_back(0x01);
    data.push_back(0x01);
    data.push_back(0x00);
    data.push_back(in_led);
    data.push_back(RGBGetRValue(in_color));
    data.push_back(RGBGetGValue(in_color));
    data.push_back(RGBGetBValue(in_color));

    SendCommand(data);
}

/*-------------------------------------------------------------------*\
| Detect the Firmware Version                                         |
|                                                                     |
|  Firmware version string is in the format:                          |
|    <layout>.<minor>.<major>                                         |
|  Where <layout> is:                                                 |
|   UNK = 0, ANSI/US = 1, ISO/EU = 2, JP = 3                          |
|  Examples:                                                          |
|  1.2.1 = ANSI/US Keyboard (PRO S)                                   |
|  2.2.1 = ISO/EU Keyboard (PRO L)                                    |
\*-------------------------------------------------------------------*/
std::string CMKeyboardV1Controller::_GetFirmwareVersion()
{
    std::vector<uint8_t> read;

    SetControlMode(MODE_FIRMWARE);
    read = SendCommand({0x01, 0x02});

    char cVersionStr[CM_KEYBOARD_WRITE_SIZE];

    for(size_t i = 0; i < read.size(); i++)
    {
        cVersionStr[i] = read[i];
    }

    cVersionStr[CM_KEYBOARD_WRITE_SIZE - 1] = 0;

    std::string sFirmwareVersion;

    sFirmwareVersion = std::string(cVersionStr+4);

    LOG_VERBOSE("[%s] GetFirmwareVersion(): [%s]", m_deviceName.c_str(), sFirmwareVersion.c_str());

    return sFirmwareVersion;
}

void CMKeyboardV1Controller::Shutdown()
{

}

KEYBOARD_LAYOUT CMKeyboardV1Controller::GetKeyboardLayout()
{
    KEYBOARD_LAYOUT layout = KEYBOARD_LAYOUT_DEFAULT;

    if(m_sFirmwareVersion.empty())
    {
        LOG_WARNING("[%s] GetKeyboardLayout() empty firmware string detected. Unable to detect firmware layout. Assuming defaults.", m_deviceName.c_str());

        layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
        return layout;
    }

    switch(m_sFirmwareVersion.c_str()[0])
    {
        case '0':
        default:
        case '1':
        layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
        break;

        case '2':
        layout = KEYBOARD_LAYOUT_ISO_QWERTY;
        break;

        case '3':
        layout = KEYBOARD_LAYOUT_JIS;
        break;
    }

    return layout;
}

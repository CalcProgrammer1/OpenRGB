/*---------------------------------------------------------*\
| CMKeyboardV2Controller.cpp                                |
|                                                           |
|   Driver for Cooler Master V2 keyboards                   |
|                                                           |
|   Tam D (too.manyhobbies)                     30 Nov 2023 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <locale>
#include <codecvt>
#include <string>
#include "CMKeyboardV2Controller.h"
#include "LogManager.h"
#include "StringUtils.h"

CMKeyboardV2Controller::CMKeyboardV2Controller(hid_device* dev_handle, hid_device_info* dev_info, std::string dev_name) : CMKeyboardAbstractController(dev_handle, dev_info, dev_name)
{
    m_sFirmwareVersion  = _GetFirmwareVersion();
    m_bMoreFFs          = false;
}

CMKeyboardV2Controller::~CMKeyboardV2Controller()
{
}

/*-----------------------------------------------------------------*\
|  Firmware version string is in the format:                        |
|    V<layout>.<minor>.<major>                                      |
|  Where <layout> is:                                               |
|   UNK = 0, ANSI/US = 1, ISO/EU = 2, JP = 3                        |
\*-----------------------------------------------------------------*/
KEYBOARD_LAYOUT CMKeyboardV2Controller::GetKeyboardLayout()
{
    KEYBOARD_LAYOUT layout = KEYBOARD_LAYOUT_DEFAULT;

    if(m_sFirmwareVersion.empty())
    {
        LOG_WARNING("[%s] _GetKeyboardLayout() empty firmware string detected. Assuming ANSI layout as default..", m_deviceName.c_str());

        layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
    }
    else
    {
        switch(m_sFirmwareVersion.c_str()[1])
        {
            case '3':
            layout = KEYBOARD_LAYOUT_JIS;
            break;

            case '2':
            layout = KEYBOARD_LAYOUT_ISO_QWERTY;
            break;

            default:
            case '1':
            layout = KEYBOARD_LAYOUT_ANSI_QWERTY;
            break;
        }
    }

    /*------------------------------------------------------------*\
    | Double check keyboard type per #3592, EU keyboard reported   |
    | with version ID in serial number.                            |
    \*------------------------------------------------------------*/
    if(m_serialNumber.find("JP") != std::string::npos)
    {
        layout = KEYBOARD_LAYOUT_JIS;
    }

    if(m_serialNumber.find("EU") != std::string::npos)
    {
        layout = KEYBOARD_LAYOUT_ISO_QWERTY;
    }

    return(layout);
}

/*-----------------------------------------------------------------*\
| Gets the firmware version.                                        |
|   Strings are stored as UTF-16 so need to be converted to string  |
|   Firmware format is VX.YY.ZZ .                                   |
\*-----------------------------------------------------------------*/
std::string CMKeyboardV2Controller::_GetFirmwareVersion()
{
    std::vector<uint8_t> read;

    SetControlMode(MODE_FIRMWARE);
    read = SendCommand({0x12, 0x20});

    uint8_t cVersionStr[CM_KEYBOARD_WRITE_SIZE];

    size_t i = 0;
    for(uint8_t it : read)
    {
        cVersionStr[i++] = it;
    }

    std::u16string usFirmwareVersion(reinterpret_cast<const char16_t*>(cVersionStr+8));
    std::string sFirmwareVersion(StringUtils::u16string_to_string(usFirmwareVersion));

    LOG_VERBOSE("[%s] GetFirmwareVersion(): [%s]", m_deviceName.c_str(), sFirmwareVersion.c_str());

    return sFirmwareVersion;
}

void CMKeyboardV2Controller::Initialize()
{
    switch(m_productId)
    {
        case COOLERMASTER_KEYBOARD_SK620B_PID:
        case COOLERMASTER_KEYBOARD_SK620W_PID:
        MagicCommand(0x09);
        break;

        case COOLERMASTER_KEYBOARD_SK622B_PID:
        case COOLERMASTER_KEYBOARD_SK622W_PID:
        MagicStartupPacket();
        MagicCommand(0x09);
        break;

        case COOLERMASTER_KEYBOARD_SK630_PID:
        case COOLERMASTER_KEYBOARD_SK650_PID:
        SetActiveProfile(0x0C);
        m_bMoreFFs = true;
        break;

        case COOLERMASTER_KEYBOARD_SK652_PID:
        case COOLERMASTER_KEYBOARD_SK653_PID:
        MagicStartupPacket();
        MagicCommand(0x0C);
        m_bMoreFFs = true;
        break;

        case COOLERMASTER_KEYBOARD_CK530_PID:
        case COOLERMASTER_KEYBOARD_CK530_V2_PID:
        SetActiveProfile(0x05);
        MagicCommand(0x0A);
        break;

        case COOLERMASTER_KEYBOARD_CK550_V2_PID:
        SetActiveProfile(0x05);
        MagicCommand(0x09);
        break;

        case COOLERMASTER_KEYBOARD_MK730_PID:
        SetActiveProfile(0x05);
        MagicCommand(0x0A);
        m_bMoreFFs = true;
        break;

        case COOLERMASTER_KEYBOARD_CK552_V2_PID:
        SetControlMode(0x80);
        MagicCommand(0x01);
        break;

        default:
        SendCommand({0x56, 0x81, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0xBB, 0xBB, 0xBB});
        break;
    }
}

void CMKeyboardV2Controller::Shutdown()
{

}

/*---------------------------------------------------------*\
| Required for some keyboards. No idea what it does.        |
\*---------------------------------------------------------*/
void CMKeyboardV2Controller::MagicStartupPacket()
{
    std::lock_guard<std::mutex> guard(m_mutex);

    SendCommand({0x12});
    SendCommand({0x12, 0x20});
    SendCommand({0x12, 0x01});
    SendCommand({0x12, 0x22});
    SendCommand({0x42, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01});
    SendCommand({0x43, 0x00, 0x00, 0x00, 0x01});
};

void CMKeyboardV2Controller::SetLEDControl(bool bManual)
{
    uint8_t modeId = 0;

    if(bManual)
    {
        modeId = 0x05;
    }

    SetControlMode(modeId);
}

void CMKeyboardV2Controller::SetLeds(std::vector<led> leds, std::vector<RGBColor> colors)
{
    SetLEDControl(true);
    Initialize();

    RGBColor rgbColorMap[CM_MAX_LEDS];
    memset(rgbColorMap, 0, sizeof(RGBColor)*CM_MAX_LEDS);

    for(size_t i = 0; i < leds.size(); i++)
    {
        rgbColorMap[leds[i].value] = colors[i];
    }

    /*---------------------------------------------------------*\
    | Convert color array to linear map of RGB positions.       |
    | Map is sequential layout of RGB for each LED. i indicates |
    | the position in the map (i*3),                            |
    \*---------------------------------------------------------*/
    uint8_t  linearColorMap[CM_MAX_LEDS*3] = {0,};

    for(size_t i = 0; i < CM_MAX_LEDS; i++)
    {
        linearColorMap[i*3]   = RGBGetRValue(rgbColorMap[i]);
        linearColorMap[i*3+1] = RGBGetGValue(rgbColorMap[i]);
        linearColorMap[i*3+2] = RGBGetBValue(rgbColorMap[i]);
    }

    /*---------------------------------------------------------*\
    | For future reference, some keyboares may designate a      |
    | different number of max LEDs.                             |
    \*---------------------------------------------------------*/
    const uint8_t nLEDs = 0xC1;

    /*---------------------------------------------------------*\
    | Build initial packet.                                     |
    | This specifies some of the initial parameters such as the |
    | number of LEDs, and the first grouping of LEDs.           |
    | The packet structure seems to be fixed for these first 24 |
    | bytes.                                                    |
    \*---------------------------------------------------------*/
    std::vector<uint8_t> data =
    {
        0x56, 0x83, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
        0x80, 0x01, 0x00, nLEDs, 0x00, 0x00, 0x00, 0x00,
        0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00
    };

    size_t pktIdx;
    for(pktIdx = 0; pktIdx < 40; pktIdx++)
    {
        data.push_back(linearColorMap[pktIdx]);
    }

    std::lock_guard<std::mutex> guard(m_mutex);

    SendCommand(data);

    /*---------------------------------------------------------*\
    | Build subsequent packets.                                 |
    | Aproximately 9 packets of 60 bytes each. Total may change |
    | if we get a keyboard with an insane number of LEDs.       |
    \*---------------------------------------------------------*/
    for(size_t i = 1; i < 10; ++i)
    {
        data.clear();
        data.push_back(0x56);
        data.push_back(0x83);
        data.push_back((uint8_t)i);
        data.push_back(0x00);

        for(size_t j = 0; j < 60; j++)
        {
            data.push_back(linearColorMap[pktIdx++]);
        }

        SendCommand(data);
    }

    SendApplyPacket(0xFF);
}

void CMKeyboardV2Controller::SetSingleLED(uint8_t /*in_led*/, RGBColor /*in_color*/)
{
}

void CMKeyboardV2Controller::MagicCommand(uint8_t profileId)
{
    SendCommand(
        {
            0x56,      0x81, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
            profileId, 0x00, 0x00, 0x00, 0xBB, 0xBB, 0xBB, 0xBB}
        );
};

void CMKeyboardV2Controller::_SetEffectMode(uint8_t effectId)
{
    std::vector<uint8_t> data;

    switch(effectId)
    {
        case STATIC:
        case DIRECT:
        case CYCLE:
        case BREATHE:
        case CIRCLE_SPECTRUM:
        case REACTIVE_TORNADO:
            data = std::vector<uint8_t>
            {
                0x56, 0x81, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                0x01, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88,
            };
            break;

        case WAVE:
            data = std::vector<uint8_t>
            {
                0x56, 0x81, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                0x01, 0x00, 0x00, 0x00, 0x99, 0x99, 0x99, 0x99
            };
            break;

        case STARS:
        case RAINDROPS:
        case SNOW:
        case CROSS:
        case RIPPLE:
        case REACTIVE_PUNCH:
        case REACTIVE_FADE:
        case HEARTBEAT:
        case FIREBALL:
        case WATER_RIPPLE:
            data = std::vector<uint8_t>
            {
                0x56, 0x81, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
                0x02, 0x00, 0x00, 0x00, 0x88, 0x88, 0x88, 0x88,
            };
            break;

        case CUSTOMIZED:
            data = std::vector<uint8_t>{
                0x56, 0x81, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                0x09, 0x00, 0x00, 0x00, 0xBB, 0xBB, 0xBB, 0xBB,
            };
            break;

        default:
            break;
    }

    SendCommand(data);
}

void CMKeyboardV2Controller::SendApplyPacket(uint8_t mode)
{
    SendCommand({0x51, 0x28, 0x00, 0x00, mode});
}


/*---------------------------------------------------------*\
| Sets the speed for each mode. Seems like almost every mode|
| has a different set of speed settings.                    |
\*---------------------------------------------------------*/
void CMKeyboardV2Controller::_UpdateSpeed(mode selectedMode, uint8_t &cSpeed1, uint8_t &cSpeed2)
{
    std::vector<uint8_t>vSpeed1{0x00, 0x00, 0x00, 0x00, 0x00};
    std::vector<uint8_t>vSpeed2{0x00, 0x00, 0x00, 0x00, 0x00};

    switch(selectedMode.value)
    {
        case CROSS:
        case REACTIVE_FADE:
            vSpeed1 = std::vector<uint8_t>{0x17, 0x0E, 0x0B, 0x0A, 0x04};
            vSpeed2 = std::vector<uint8_t>{0x01, 0x01, 0x02, 0x05, 0x04};
            break;

        case WAVE:
            vSpeed1 = std::vector<uint8_t>{0x17, 0x0D, 0x07, 0x09, 0x08};
            vSpeed2 = std::vector<uint8_t>{0x01, 0x04, 0x06, 0x0C, 0x11};
            break;

        case REACTIVE_PUNCH:
            vSpeed1 = std::vector<uint8_t>{0x0E, 0x0A, 0x04, 0x07, 0x01};
            break;

        case BREATHE:
            vSpeed1 = std::vector<uint8_t>{0x08, 0x0A, 0x0C, 0x07, 0x09};
            vSpeed2 = std::vector<uint8_t>{0x01, 0x02, 0x04, 0x04, 0x09};
            break;

        case CIRCLE_SPECTRUM:
        case REACTIVE_TORNADO:
            switch (selectedMode.direction)
            {
                case MODE_DIRECTION_LEFT:
                vSpeed1 = std::vector<uint8_t>{0xFF, 0xFE, 0xFD, 0xFC, 0xFC};
                vSpeed2 = std::vector<uint8_t>{0x04, 0x08, 0x08, 0x0C, 0x00};
                break;

                default:
                case MODE_DIRECTION_RIGHT:
                vSpeed1 = std::vector<uint8_t>{0x00, 0x01, 0x02, 0x03, 0x04};
                vSpeed2 = std::vector<uint8_t>{0x0C, 0x08, 0x08, 0x04, 0x00};
                break;
            }
            break;

        case CYCLE:
            vSpeed1 = std::vector<uint8_t>{0x10, 0x0C, 0x08, 0x04, 0x00};
            break;

        case RIPPLE:
        case WATER_RIPPLE:
            vSpeed1 = std::vector<uint8_t>{0x36, 0x18, 0x0C, 0x06, 0x02};
            break;

        case FIREBALL:
        case HEARTBEAT:
            vSpeed1 = std::vector<uint8_t>{0x01, 0x02, 0x03, 0x05, 0x09};
            break;

        case RAINDROPS:
        case SNOW:
            vSpeed1 = std::vector<uint8_t>{0x0B, 0x08, 0x05, 0x02, 0x00};
            vSpeed2 = std::vector<uint8_t>{0x08, 0x18, 0x30, 0x38, 0x40};
            break;

        case STARS:
            vSpeed1 = std::vector<uint8_t>{0x17, 0x0E, 0x08, 0x0A, 0x0A};
            vSpeed2 = std::vector<uint8_t>{0x01, 0x01, 0x01, 0x02, 0x04};
            break;

        case OFF:
        default:
            break;
    }

    if(selectedMode.speed >= 1 && selectedMode.speed <= 5)
    {
        cSpeed1 = vSpeed1[selectedMode.speed-1];
        cSpeed2 = vSpeed2[selectedMode.speed-1];
    }
}

void CMKeyboardV2Controller::SetCustomMode()
{
    std::lock_guard<std::mutex> guard(m_mutex);

    SendCommand({0x41, 0x80});
    SendCommand({0x52});
}

void CMKeyboardV2Controller::SetMode(mode selectedMode)
{
    std::vector<uint8_t> data(64, 0);
    std::vector<uint8_t> data1(64, 0);
    std::vector<uint8_t> data2(64, 0);

    uint8_t cColor1_R   = 0;
    uint8_t cColor1_G   = 0;
    uint8_t cColor1_B   = 0;
    uint8_t cColor2_R   = 0;
    uint8_t cColor2_G   = 0;
    uint8_t cColor2_B   = 0;

    RGBColor color1     = 0;
    RGBColor color2     = 0;
    uint8_t cSpeed1     = 0;
    uint8_t cSpeed2     = 0;

    _UpdateSpeed(selectedMode, cSpeed1, cSpeed2);

    uint8_t cBright     = (uint8_t)selectedMode.brightness;
    uint8_t cDirection  = 0;
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
    }

    switch(selectedMode.direction)
    {
        case MODE_DIRECTION_LEFT:
        case MODE_DIRECTION_HORIZONTAL:
            cDirection      = 0x04;
            break;

        case MODE_DIRECTION_RIGHT:
            cDirection      = 0x00;
            break;

        case MODE_DIRECTION_UP:
        case MODE_DIRECTION_VERTICAL:
            cDirection      = 0x06;
            break;

        case MODE_DIRECTION_DOWN:
            cDirection      = 0x02;
            break;

        default:
            break;
    }

    SetCustomMode();

    std::lock_guard<std::mutex> guard(m_mutex);
    _SetEffectMode(effectId);

    switch(effectId)
    {
        case OFF:
            break;

        case DIRECT:
            break;

        case STATIC:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, 0x00, 0x00,
                };
                SendCommand(data);
            }
            break;

        case WAVE:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                    0x00, 0x32, 0x00, 0xC1, cSpeed1, 0x00, 0x00, 0x00,
                    0xFF, 0xFF, 0xFF, cBright, 0x00, cDirection, cSpeed2, 0x00,
                    0x00, 0x04, 0x08
                };
                SendCommand(data);
            }
            break;

        case STARS:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0D, 0x00, 0x0D, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x40, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, cSpeed2, 0x01,
                    0x01, 0x10, 0x08, 0x01, 0x10, 0x00, 0x00, 0x00,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data1 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF
                };

                data2 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF
                };

                data = m_bMoreFFs ? data2 : data1;

                SendCommand(data);
            }
            break;

        case RAINDROPS:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0D, 0x00, 0x0D, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x81, 0x40, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, 0x03, 0x00,
                    cSpeed2, 0x18, 0x04, 0x10, 0x01, 0x00, 0x00, 0x00,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data1 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF
                };

                data2 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF
                };

                data = m_bMoreFFs ? data2 : data1;

                SendCommand(data);
            }
            break;

        case SNOW:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0D, 0x00, 0x0D, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x81, 0x40, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, 0x03, 0x00,
                    cSpeed2, 0xFF, 0x10, 0x10, 0x01, 0x40, 0x00, 0x00,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);
            }
            break;

        case CYCLE:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                    0x00, 0x31, 0x00, 0xC1, cSpeed1, 0x00, 0x00, 0x00,
                    0x40, 0x00, 0xFF, cBright, 0x00, 0x00, 0x03, 0x00,
                };

                SendCommand(data);
            }
            break;

        case BREATHE:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                    0x00, 0x30, 0x00, 0xC1, cSpeed1, 0x00, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x01, 0x00, cSpeed2, 0x00,
                };
                SendCommand(data);
            }
            break;

        case CIRCLE_SPECTRUM:
            {
                /*---------------------------------------------------*\
                | Speed and directions are interlinked. Probably has  |
                | to do how speed is computed within the firmware.    |
                | The speed values for this mode take into account    |
                | the direction.                                      |
                \*---------------------------------------------------*/
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                    0x00, 0x34, 0x00, 0xC1, 0x04, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0xFF, cBright, 0x00, 0x00, cSpeed1, 0x00,
                    0x00, cSpeed2, 0x00, 0x04, 0xA0, 0x00, 0x30, 0x00,
                };
                SendCommand(data);
            }
            break;

        case CROSS:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x80, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, cSpeed2, 0x01,
                    0x03, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data1 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                };

                data2 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                };

                data = m_bMoreFFs ? data2 : data1;
                SendCommand(data);
            }
            break;

        case SINGLE:
            break;

        case RIPPLE:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x01, 0x82, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);
            }
            break;

        case REACTIVE_PUNCH:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0B, 0x00, 0x0B, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x28, 0x80, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, 0x01, 0x00,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };

                SendCommand(data);

                data1 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF
                };

                data2 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF
                };

                data = m_bMoreFFs ? data2 : data1;

                SendCommand(data);
            }
            break;

        case REACTIVE_FADE:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x80, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, 0x01, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data1 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                };

                data2 = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                };

                data = m_bMoreFFs ? data2 : data1;

                SendCommand(data);
            }
            break;

        case REACTIVE_TORNADO:
            {
                /*---------------------------------------------------*\
                | Speed and directions are interlinked. Probably has  |
                | to do how speed is computed within the firmware.    |
                | The speed values for this mode take into account    |
                | the direction.                                      |
                \*---------------------------------------------------*/
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
                    0x00, 0x83, 0x00, 0xC1, 0x04, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0xFF, cBright, 0x00, 0x00, cSpeed1, 0x00,
                    0x00, cSpeed2, 0x00, 0x00, 0x30, 0x00, 0x10, 0x00,
                };
                SendCommand(data);
            }
            break;

            case HEARTBEAT:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x20, 0xA0, 0x00, 0x80, 0x20, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, cSpeed1, 0x00,
                    0x02, 0xFF, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                };
                SendCommand(data);
            }
            break;

        case FIREBALL:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x20, 0xA0, 0x00, 0x80, 0x10, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, cSpeed1, 0x00,
                    0x01, 0x08, 0x09, 0x04, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                };
                SendCommand(data);
            }
            break;

        case WATER_RIPPLE:
            {
                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x00, 0x00, 0x0C, 0x00, 0x0C, 0x00,
                    0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00,
                    0x00, 0x01, 0x00, 0xC1, 0x00, 0x00, 0x00, 0x00,
                    cColor2_R, cColor2_G, cColor2_B, cBright, 0x00, 0x00, 0x00, 0x00,
                    0x01, 0x82, 0x00, 0x80, cSpeed1, 0x10, 0x00, 0x00,
                    cColor1_R, cColor1_G, cColor1_B, cBright, 0x00, 0x00, 0x06, 0x00,
                    0x00, 0x90, 0x14, 0x20, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
                };
                SendCommand(data);

                data = std::vector<uint8_t>
                {
                    0x56, 0x83, 0x01, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                };
                SendCommand(data);
            }
            break;

        case SNAKE:
        default:
            break;
    }

    switch(effectId)
    {
        case OFF:
            SendCommand({0x51, 0x28, 0x00, 0x00, 0x10});
            SendCommand({0x41, 0x80});
            break;

        default:
            SendCommand({0x41, 0x80});
            SendCommand({0x51, 0x28, 0x00, 0x00, 0xFF});
            break;
    }
}

struct stCMKeyboardV2_mode CMKeyboardV2_modes[] =
{
/*----------------------------------------------------------------------------------------------------------------------------------------*\
|                             Speed         Brightness                                                                                     |
|    NAME          VALUE      MIN  MAX SET   MAX   DIR  Number of Colors      COLOR_MODE                  FLAGS                            |
\*----------------------------------------------------------------------------------------------------------------------------------------*/
    {"Direct",      DIRECT,    1,  5,   3,  0xFF,  0,                  0,   MODE_COLORS_PER_LED,        MODE_FLAG_HAS_PER_LED_COLOR},
    {"Static",      STATIC,    1,  5,   3,  0xFF,  0,                  1,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS },
    {"Wave",        WAVE,      1,  5,   3,  0xFF,  MODE_DIRECTION_LEFT,1,   MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED |
                                                                                                         MODE_FLAG_HAS_DIRECTION_LR |
                                                                                                         MODE_FLAG_HAS_DIRECTION_UD},
    {"Crosshair",   CROSS,     1,  5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Reactive Fade",REACTIVE_FADE,1,5,  3,  0xFF,  0,                 2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Stars",       STARS,     1,  5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Raindrops",   RAINDROPS, 1,  5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Color Cycle", CYCLE,     1,  5,   3,  0xFF,  0,                  0,   MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Breathing",   BREATHE,   1,  5,   3,  0xFF,  0,                  1,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Ripple",      RIPPLE,    1,  5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Fireball",    FIREBALL,  1,  5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Water Ripple",WATER_RIPPLE,1,5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Reactive Punch",REACTIVE_PUNCH,1,5,3, 0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Snowing",     SNOW,      1,  5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Heartbeat",   HEARTBEAT, 1,  5,   3,  0xFF,  0,                  2,   MODE_COLORS_MODE_SPECIFIC,  MODE_FLAG_HAS_MODE_SPECIFIC_COLOR |
                                                                                                         MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED },
    {"Circle Spectrum",CIRCLE_SPECTRUM,1,5,3,0xFF, MODE_DIRECTION_LEFT,0,   MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED |
                                                                                                         MODE_FLAG_HAS_DIRECTION_LR},
    {"Reactive Tornado",REACTIVE_TORNADO,1,5,3,0xFF,MODE_DIRECTION_LEFT,0,  MODE_COLORS_NONE,           MODE_FLAG_HAS_BRIGHTNESS |
                                                                                                         MODE_FLAG_HAS_SPEED |
                                                                                                         MODE_FLAG_HAS_DIRECTION_LR},
    {"Off",         OFF,       0,  0,   0,  0xFF,  MODE_DIRECTION_LEFT, 0,  MODE_COLORS_NONE,           0},
    { 0,            NONE,      0,  0,   0,     0,  0,                   0,  0,                          0},
};

void CMKeyboardV2Controller::InitializeModes(std::vector<mode> &modes)
{
    stCMKeyboardV2_mode * pCurrentMode = CMKeyboardV2_modes;

    while(pCurrentMode->value != NONE)
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

        modes.push_back(m);

        pCurrentMode++;
    }
}

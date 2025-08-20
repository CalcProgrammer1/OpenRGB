/*---------------------------------------------------------*\
| GigabyteRGBFusion2USBController.cpp                       |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 USB motherboard  |
|                                                           |
|   jackun                                      08 Jan 2020 |
|   megadjc                                     31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "GigabyteRGBFusion2USBController.h"

/*-------------------------------------------------------------------------*\
| Low level RGB value conversion table                                      |
| This is stored as a uint32_t in the chip so is trasmitted LSB to MSB      |
| Therefore the numbers represent the index where the controller will find  |
| respective colour in a regular packet                                     |
\*-------------------------------------------------------------------------*/
static RGBCalibration GigabyteCalibrationsLookup
{
    { "BGR", {{{0x00, 0x01, 0x02, 0x00}}}},
    { "BRG", {{{0x01, 0x00, 0x02, 0x00}}}},
    { "GRB", {{{0x02, 0x00, 0x01, 0x00}}}},
    { "GBR", {{{0x00, 0x02, 0x01, 0x00}}}},
    { "RGB", {{{0x02, 0x01, 0x00, 0x00}}}},
    { "RBG", {{{0x01, 0x02, 0x00, 0x00}}}}
};

/*---------------------------------------------------------*\
| Converts LED counts to divisions in hardware              |
\*---------------------------------------------------------*/
static LEDCount LedCountToEnum(unsigned int c)
{
    if(c <= 32)
    {
        return(LEDS_32);
    }
    else if(c <= 64)
    {
        return(LEDS_64);
    }
    else if(c <= 256)
    {
        return(LEDS_256);
    }
    else if(c <= 512)
    {
        return(LEDS_512);
    }
    else
    {
        return(LEDS_1024);
    }
}

RGBFusion2USBController::RGBFusion2USBController(hid_device* handle, const char* path, std::string mb_name, uint16_t pid): dev(handle), product_id(pid)
{
    name = mb_name;
    location = path;
    
    if(!RefreshHardwareInfo())
    {
        return;
    }
    if(report.support_cmd_flag >= 0x02)
    {
        EnableLampArray(false);
    }
    ResetController();
    EnableBeat(false);
}

RGBFusion2USBController::~RGBFusion2USBController()
{
    hid_close(dev);
}

/*---------------------------------------------------------*\
| Read configuration data from hardware.                    |
| Returns false if read fails.                              |
\*---------------------------------------------------------*/
bool RGBFusion2USBController::RefreshHardwareInfo()
{
    unsigned char buffer[FUSION2_USB_BUFFER_SIZE] = {0};

    SendPacket(0x60, 0x00);
    buffer[0] = report_id;
    int res = hid_get_feature_report(dev, buffer, sizeof(buffer));

    if(res < static_cast<int>(sizeof(IT8297Report)))
    {
        report_loaded = false;
        return false;
    }

    std::memcpy(&report, buffer, sizeof(IT8297Report));
    report_loaded = true;

    description = std::string(report.str_product, 28);
    if(std::string::iterator nul = std::find(description.begin(), description.end(), '\0');
       nul != description.end())
    {
        description.erase(nul, description.end());
    }

    {
        char     text[32]{};

        uint32_t fw_ver = ((report.fw_ver & 0x000000FFu) << 24)
            | ((report.fw_ver & 0x0000FF00u) << 8)
            | ((report.fw_ver & 0x00FF0000u) >> 8)
            | ((report.fw_ver & 0xFF000000u) >> 24);

        uint8_t  b0 = static_cast<uint8_t>((fw_ver >> 24) & 0xFFu);
        uint8_t  b1 = static_cast<uint8_t>((fw_ver >> 16) & 0xFFu);
        uint8_t  b2 = static_cast<uint8_t>((fw_ver >> 8)  & 0xFFu);
        uint8_t  b3 = static_cast<uint8_t>( fw_ver        & 0xFFu);

        std::snprintf(text, sizeof(text), "%u.%u.%u.%u", b0, b1, b2, b3);
        version = text;
        std::snprintf(text, sizeof(text), "0x%08X", report.chip_id);
        chip_id = text;
    }

    D_LED1_count = LedCountToEnum(report.curr_led_count_low  & 0x0F);
    D_LED2_count = LedCountToEnum((report.curr_led_count_low  >> 4) & 0x0F);
    D_LED3_count = LedCountToEnum(report.curr_led_count_high & 0x0F);
    D_LED4_count = LedCountToEnum((report.curr_led_count_high >> 4) & 0x0F);

    cal_data.dled[0]   = report.cal_strip0;
    cal_data.dled[1]   = report.cal_strip1;
    cal_data.mainboard = report.rgb_cali;
    cal_data.spare[0]  = report.cal_spare0;
    cal_data.spare[1]  = report.cal_spare1;

    cali_loaded = false;
    if(product_id == 0x5711)
    {
        unsigned char buffer2[FUSION2_USB_BUFFER_SIZE] = {0};
        SendPacket(0x61, 0x00);
        buffer2[0] = report_id;
        int res2 = hid_get_feature_report(dev, buffer2, sizeof(buffer2));

        if(res2 >= static_cast<int>(sizeof(IT5711Calibration)))
        {
            std::memcpy(&cali, buffer2, sizeof(IT5711Calibration));
            cali_loaded = true;

            cal_data.dled[2]  = cali.cal_strip2;
            cal_data.dled[3]  = cali.cal_strip3;
            cal_data.spare[2] = cali.cal_spare2;
            cal_data.spare[3] = cali.cal_spare3;
        }
        else
        {
            cal_data.dled[2]  = 0;
            cal_data.dled[3]  = 0;
            cal_data.spare[2] = 0;
            cal_data.spare[3] = 0;
            cali_loaded       = false;
        }
    }
    else
    {
        cal_data.dled[2]  = 0;
        cal_data.dled[3]  = 0;
        cal_data.spare[2] = 0;
        cal_data.spare[3] = 0;
    }

    return report_loaded;
}

void RGBFusion2USBController::SetMode(int m)
{
    mode = m;
}

std::string RGBFusion2USBController::DecodeCalibrationBuffer(uint32_t value) const
{
    if(value==0) return "OFF";

    uint8_t bo_b =  value        & 0xFF;
    uint8_t bo_g = (value >> 8)  & 0xFF;
    uint8_t bo_r = (value >> 16) & 0xFF;

    bool in_range = (bo_r<3 && bo_g<3 && bo_b<3);
    bool distinct = (bo_r!=bo_g && bo_r!=bo_b && bo_g!=bo_b);

    if(in_range && distinct)
    {
        std::string out(3, '?');
        out[bo_r] = 'R';
        out[bo_g] = 'G';
        out[bo_b] = 'B';
        return out;
    }

    return "BAD";
}

uint32_t RGBFusion2USBController::EncodeCalibrationBuffer(const std::string& rgb_order)
{
    if(rgb_order.empty())
    {
        return 0u;
    }

    std::string key = rgb_order;
    std::transform(key.begin(), key.end(), key.begin(),
                   [](unsigned char c){ return char(std::toupper(c)); });

    if(key=="OFF" || key=="0")
    {
        return 0u;
    }

    RGBCalibration::const_iterator it = GigabyteCalibrationsLookup.find(key);
    if(it == GigabyteCalibrationsLookup.end())
    {
        return 0u;
    }

    const RGBA &rgb_cal = it->second;
    return  (uint32_t(rgb_cal.raw[0]))
           | (uint32_t(rgb_cal.raw[1]) << 8)
           | (uint32_t(rgb_cal.raw[2]) << 16)
           | (uint32_t(rgb_cal.raw[3]) << 24);
}


EncodedCalibration RGBFusion2USBController::GetCalibration(bool refresh_from_hw)
{
    if(refresh_from_hw || !report_loaded || (product_id == 0x5711 && !cali_loaded))
    {
        if(!RefreshHardwareInfo())
        {
            return EncodedCalibration{};
        }
    }

    EncodedCalibration out{};
    out.dled[0]   = DecodeCalibrationBuffer(cal_data.dled[0]);
    out.dled[1]   = DecodeCalibrationBuffer(cal_data.dled[1]);
    out.spare[0]  = DecodeCalibrationBuffer(cal_data.spare[0]);
    out.spare[1]  = DecodeCalibrationBuffer(cal_data.spare[1]);
    out.mainboard = DecodeCalibrationBuffer(cal_data.mainboard);

    if(product_id == 0x5711)
    {
        out.dled[2]  = DecodeCalibrationBuffer(cal_data.dled[2]);
        out.dled[3]  = DecodeCalibrationBuffer(cal_data.dled[3]);
        out.spare[2] = DecodeCalibrationBuffer(cal_data.spare[2]);
        out.spare[3] = DecodeCalibrationBuffer(cal_data.spare[3]);
    }
    else
    {
        out.dled[2]  = "OFF";
        out.dled[3]  = "OFF";
        out.spare[2] = "OFF";
        out.spare[3] = "OFF";
    }

    return out;
}

bool RGBFusion2USBController::SetCalibration(const EncodedCalibration& cal, bool refresh_from_hw)
{
    if(refresh_from_hw)
    {
        if(!RefreshHardwareInfo())
        {
            return false;
        }
    }

    if(EncodeCalibrationBuffer(cal.dled[0])       == cal_data.dled[0]
        && EncodeCalibrationBuffer(cal.dled[1])   == cal_data.dled[1]
        && EncodeCalibrationBuffer(cal.mainboard) == cal_data.mainboard
        && EncodeCalibrationBuffer(cal.spare[0])  == cal_data.spare[0]
        && EncodeCalibrationBuffer(cal.spare[1])  == cal_data.spare[1]
        && (product_id != 0x5711
        || (EncodeCalibrationBuffer(cal.dled[2])  == cal_data.dled[2]
        && EncodeCalibrationBuffer(cal.dled[3])   == cal_data.dled[3]
        && EncodeCalibrationBuffer(cal.spare[2])  == cal_data.spare[2]
        && EncodeCalibrationBuffer(cal.spare[3])  == cal_data.spare[3])))
    {
        return true;
    }

    CMD_0x33 desired{};
    std::memset(&desired, 0, sizeof(desired));
    desired.report_id  = report_id;
    desired.command_id = 0x33;

    desired.d_strip_c0  = EncodeCalibrationBuffer(cal.dled[0]);
    desired.d_strip_c1  = EncodeCalibrationBuffer(cal.dled[1]);
    desired.rgb_cali    = EncodeCalibrationBuffer(cal.mainboard);
    desired.c_spare0    = EncodeCalibrationBuffer(cal.spare[0]);
    desired.c_spare1    = EncodeCalibrationBuffer(cal.spare[1]);

    if(product_id == 0x5711)
    {
        desired.d_strip_c2 = EncodeCalibrationBuffer(cal.dled[2]);
        desired.d_strip_c3 = EncodeCalibrationBuffer(cal.dled[3]);
        desired.c_spare2   = EncodeCalibrationBuffer(cal.spare[2]);
        desired.c_spare3   = EncodeCalibrationBuffer(cal.spare[3]);
    }

    std::memset(desired.reserved, 0, sizeof(desired.reserved));
    int rc = SendPacket(reinterpret_cast<unsigned char*>(&desired));
    if(rc < 0)
    {
        return false;
    }

    ResetController();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    SaveCalState();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    cal_data.dled[0]   = desired.d_strip_c0;
    cal_data.dled[1]   = desired.d_strip_c1;
    cal_data.mainboard = desired.rgb_cali;
    cal_data.spare[0]  = desired.c_spare0;
    cal_data.spare[1]  = desired.c_spare1;
    
    if(product_id == 0x5711)
    {
        cal_data.dled[2]  = desired.d_strip_c2;
        cal_data.dled[3]  = desired.d_strip_c3;
        cal_data.spare[2] = desired.c_spare2;
        cal_data.spare[3] = desired.c_spare3;
    }
    else
    {
        cal_data.dled[2]  = 0u;
        cal_data.dled[3]  = 0u;
        cal_data.spare[2] = 0u;
        cal_data.spare[3] = 0u;
    }

    return true;
}

void RGBFusion2USBController::SetLedCount(unsigned int c0, unsigned int c1, unsigned int c2, unsigned int c3)
{
    new_d1 = LedCountToEnum(c0);
    new_d2 = LedCountToEnum(c1);
    new_d3 = LedCountToEnum(c2);
    new_d4 = LedCountToEnum(c3);

    if(new_d1 == D_LED1_count && new_d2 == D_LED2_count && new_d3 == D_LED3_count && new_d4 == D_LED4_count)
    {
        return;
    }

    D_LED1_count = new_d1;
    D_LED2_count = new_d2;
    D_LED3_count = new_d3;
    D_LED4_count = new_d4;

    unsigned char buffer[FUSION2_USB_BUFFER_SIZE] = { 0 };
    buffer[0] = report_id;
    buffer[1] = 0x34;
    buffer[2] = (new_d2 << 4) | new_d1;
    buffer[3] = (new_d4 << 4) | new_d3;
    SendPacket(buffer);
}

/*---------------------------------------------------------*\
| Switch ARGB header mode (single/addressable)              |
\*---------------------------------------------------------*/
bool RGBFusion2USBController::SetStripBuiltinEffectState(int hdr, bool enable)
{
    static bool first_call = true;
    int bitmask = 0;

    if(hdr == -1)
    {
        bitmask = 0x01 | 0x02 | 0x08 | 0x10;
    }
    else
    {
        switch(hdr)
        {
            case LED4:
            case HDR_D_LED2:
            case HDR_D_LED2_RGB:
                bitmask = 0x02;
                break;
            case HDR_D_LED3:
            case HDR_D_LED3_RGB:
                bitmask = 0x08;
                break;
            case HDR_D_LED4:
            case HDR_D_LED4_RGB:
                bitmask = 0x10;
                break;
            default:
                bitmask = 0x01;
                break;
        }
    }

    int new_effect_disabled = enable
        ? (effect_disabled & ~bitmask)
        : (effect_disabled | bitmask);

    if(!first_call && new_effect_disabled == effect_disabled)
    {
        return true;
    }

    first_call = false;
    effect_disabled = new_effect_disabled;
    int res = SendPacket(0x32, effect_disabled);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return res;
}

/*---------------------------------------------------------*\
| Persist LED config data                                   |
\*---------------------------------------------------------*/
bool RGBFusion2USBController::SaveLEDState(bool e)
{
    return SendPacket(0x47, e ? 1 : 0);
}

/*---------------------------------------------------------*\
| Persist calibration                                       |
\*---------------------------------------------------------*/
bool RGBFusion2USBController::SaveCalState()
{
    return SendPacket(0x5E, 0);
}

/*---------------------------------------------------------*\
| Set beat mode (hardware audio sync mode)                  |
\*---------------------------------------------------------*/
bool RGBFusion2USBController::EnableBeat(bool e)
{
    return SendPacket(0x31, e ? 1 : 0);
}

/*---------------------------------------------------------*\
| Set Lamp Array mode (MSDL)                                |
\*---------------------------------------------------------*/
bool RGBFusion2USBController::EnableLampArray(bool enable)
{
    return SendPacket(0x48, enable ? 1 : 0);
}
       
std::string RGBFusion2USBController::GetDeviceName()
{
    return(name);
}

std::string RGBFusion2USBController::GetDeviceDescription()
{
    return(description);
}

std::string RGBFusion2USBController::GetFWVersion()
{
    return(version);
}

std::string RGBFusion2USBController::GetDeviceLocation()
{
    return("HID: " + location);
}

std::string RGBFusion2USBController::GetSerial()
{
    return(chip_id);
}

/*---------------------------------------------------------*\
| PID (controller feature support)                          |
\*---------------------------------------------------------*/
uint16_t RGBFusion2USBController::GetProductID()
{
    return (product_id);
}

/*---------------------------------------------------------*\
| Low level controller number (multi-controller)            |
\*---------------------------------------------------------*/
uint8_t  RGBFusion2USBController::GetDeviceNum()
{
    return (device_num);
}

/*---------------------------------------------------------*\
| Set ARGB strips (addressable)                             |
\*---------------------------------------------------------*/
void RGBFusion2USBController::SetStripColors(unsigned int hdr, RGBColor* colors, unsigned int num_colors, int single_led)
{
    PktRGB pkt;
    pkt.Init(hdr, report_id);
    uint32_t byteorder;

    switch(pkt.s.header)
    {
        case HDR_D_LED2_RGB:
            byteorder = cal_data.dled[1];
            break;
        case HDR_D_LED3_RGB:
            byteorder = cal_data.dled[2];
            break;
        case HDR_D_LED4_RGB:
            byteorder = cal_data.dled[3];
            break;
        default:
            byteorder = cal_data.dled[0];
            break;
    }

    unsigned char bo_r  = byteorder >> 16;
    unsigned char bo_g  = byteorder >> 8;
    unsigned char bo_b  = byteorder & 0xFF;

    int res;
    int leds_left       = num_colors;
    int sent_data       = 0;
    int k               = 0;
    int leds_in_pkt     = sizeof(pkt.s.leds) / sizeof(*pkt.s.leds); /* 19 */

    if(single_led > -1)
    {
        leds_left       = 1;
        k               = single_led;
        sent_data       = k * 3;
        leds_in_pkt     = 1;
    }

    while(leds_left > 0)
    {
        leds_in_pkt     = (std::min)(leds_in_pkt, leds_left);
        leds_left      -= leds_in_pkt;

        pkt.s.bcount    = leds_in_pkt * 3;
        pkt.s.boffset   = sent_data;
        sent_data      += pkt.s.bcount;

        for(int i = 0; i < leds_in_pkt; i++)
        {
            RGBColor            color = colors[k];
            uint8_t            offset = (i * 3) + 5;

            pkt.buffer[offset + bo_r] = RGBGetRValue(color);
            pkt.buffer[offset + bo_g] = RGBGetGValue(color);
            pkt.buffer[offset + bo_b] = RGBGetBValue(color);
            k++;
        }

        res = SendPacket(pkt.buffer);

        if(res < 0)
        {
            return;
        }
    }
}

/*---------------------------------------------------------*\
| Set hardware effects (single)                             |
| Note: Effects paramters match that of gigabyte software.  |
| -(2)Gigabyte breathe ranges are 400-1000ms in 100ms steps |
|     and 1000-1600ms in 200ms steps                        |
| -(3)Gigabyte flash ranges are 600-2400ms in 200ms steps   |
| -(4)Gigabyte color cycle ranges are 300-2400ms for period0|
|     and 100-2200ms for period1 in 100ms steps.            |
|     the follow this trend between 300-1100/100-1000ms     |
|     then jump to 2400ms and 2200ms respective on speed 9. |
| -(6)Gigabyte Wave ranges are 30-300ms in steps following  |
|     the following formula. 2.5(s+1)^2 + 2.5(s+1) + 25.    |
| -(15)Gigabyte dflash ranges are 800-2600ms in 200ms steps |
| -(3)(15)flash and dflash parameters were combined.        |
\*---------------------------------------------------------*/
void RGBFusion2USBController::SetLEDEffect(int led, int mode, unsigned int speed, unsigned char brightness, bool random, uint32_t* color)
{
    PktEffect pkt;
    pkt.Init(led, report_id, product_id);
    if(led == -1)
    {
        effect_zone_mask            = pkt.e.zone0;
    }
    else if((effect_zone_mask & pkt.e.zone0) == 0)
    {
        effect_zone_mask           |= pkt.e.zone0;
    }
    pkt.e.max_brightness            = brightness;
    pkt.e.effect_type               = mode;
    pkt.e.effect_param0             = random ? 7 : 0;
    pkt.e.color0                    = RGBToBGRColor(*color);

    switch(mode)
    {
        case EFFECT_PULSE:
            pkt.e.period0           = (speed <= 6) ? (400 + speed * 100) : (1000 + (speed - 6) * 200);
            pkt.e.period1           = pkt.e.period0;
            pkt.e.period2           = 200;
            break;
        case EFFECT_DFLASH:
            pkt.e.effect_type       = 3;
            pkt.e.effect_param1     = 1;
            pkt.e.effect_param2     = 2;
        case EFFECT_BLINKING:
            pkt.e.period0           = 100;
            pkt.e.period1           = 100;
            pkt.e.period2           = (speed * 200) + 700;
            break;
        case EFFECT_COLORCYCLE:
            pkt.e.period0           = (speed * 100 + 300) + (speed > 8 ? 1300 * (speed - 8) : 0);
            pkt.e.period1           = pkt.e.period0 -200;
            pkt.e.effect_param0     = 7;
            break;
        case EFFECT_WAVE:
            pkt.e.period0           = (((speed + 1)^2) + (speed + 1) + 10) * 5 / 2;
            pkt.e.effect_param0     = 7;
            pkt.e.effect_param1     = 1;
            break;
        case EFFECT_RANDOM:
            pkt.e.period0           = 100;
            pkt.e.effect_param0     = 1;
            pkt.e.effect_param1     = 5;
            break;
        case EFFECT_WAVE1:
            pkt.e.period0           = 1200;
            pkt.e.period1           = 100;
            pkt.e.period2           = 360;
            pkt.e.period3           = 1200;
            break;
        case EFFECT_WAVE2:
        case EFFECT_WAVE4:
            pkt.e.period0           = 200;
            pkt.e.effect_param0     = 7;
            break;
        case EFFECT_WAVE3:
            pkt.e.period0           = 840;
            pkt.e.period1           = 20;
            pkt.e.period2           = 200;
            pkt.e.period3           = 840;
            break;
    }
    SendPacket(pkt.buffer);
}

/*---------------------------------------------------------*\
| Apply hardware effects (single)                           |
\*---------------------------------------------------------*/
bool RGBFusion2USBController::ApplyEffect(bool fast_apply)
{
    if(fast_apply)
    {
        if(product_id == 0x5711)
        {
            return SendPacket(0x28, 0xFF, 0x07);
        }
        else
        {
            return SendPacket(0x28, 0xFF, 0x00);
        }
    }

    PktEffectApply pkt = {};
    pkt.a.zone_sel0 = effect_zone_mask;

    effect_zone_mask = 0;
    return SendPacket(pkt.buffer);
}

bool RGBFusion2USBController::SendPacket(uint8_t a, uint8_t b, uint8_t c)
{
    unsigned char buffer[FUSION2_USB_BUFFER_SIZE] {};

    buffer[0] = report_id;
    buffer[1] = a;
    buffer[2] = b;
    buffer[3] = c;

    return(SendPacket(buffer) == FUSION2_USB_BUFFER_SIZE);
}

int RGBFusion2USBController::SendPacket(unsigned char* packet)
{
    return hid_send_feature_report(dev, packet, FUSION2_USB_BUFFER_SIZE);
}

/*---------------------------------------------------------*\
| Reset controller parameters                               |
\*---------------------------------------------------------*/
void RGBFusion2USBController::ResetController()
{
    for(uint8_t reg = 0x20; reg <= 0x27; ++reg)
    {
        SendPacket(reg, 0x00, 0x00);
    }

    if(product_id == 0x5711)
    {
        for(uint8_t reg = 0x90; reg <= 0x92; ++reg)
        {
            SendPacket(reg, 0x00, 0x00);
        }
    }
    ApplyEffect(true);
}

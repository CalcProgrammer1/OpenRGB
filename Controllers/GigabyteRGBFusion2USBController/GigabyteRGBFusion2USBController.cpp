/*---------------------------------------------------------*\
| GigabyteRGBFusion2USBController.cpp                       |
|                                                           |
|   Driver for Gigabyte Aorus RGB Fusion 2 USB motherboard  |
|                                                           |
|   jackun                                      08 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "GigabyteRGBFusion2USBController.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

/*-------------------------------------------------------------------------*\
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

static calibration GigabyteBoardCalibration
{
    { "D_LED1",     "GRB"   },
    { "D_LED2",     "GRB"   },
    { "Mainboard",  "BGR"   },
    { "Spare",      "BGR"   }
};

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

RGBFusion2USBController::RGBFusion2USBController(hid_device* handle, const char *path, std::string mb_name) : dev(handle)
{
    int res                     = 0;
    char text[64]               = { 0x00 };
    unsigned char buffer[64]    = { 0x00 };

    if(dev)
    {
        SetCalibration();

        name = mb_name;

        /*---------------------------------------------------------*\
        | HID report read needs 0x60 packet or it gives IO error    |
        \*---------------------------------------------------------*/
        SendPacket(0x60, 0x00);

        buffer[0]   = report_id;
        res         = hid_get_feature_report(dev, buffer, 64);

        if(res > 0)
        {
            report  = *reinterpret_cast<IT8297Report*>(buffer);

            description = std::string(report.str_product, 32);
            description.erase(std::find(description.begin(), description.end(), '\0'), description.end());

            snprintf(text, 11, "0x%08X", report.fw_ver);
            version = text;

            snprintf(text, 11, "0x%08X", report.chip_id);
            chip_id = text;

            D_LED1_count = LedCountToEnum(report.total_leds & 0x0F);
            D_LED2_count = LedCountToEnum(report.total_leds & 0xF0);
        }

        location = path;

        EnableBeat(false);
    }
}

RGBFusion2USBController::~RGBFusion2USBController()
{
    hid_close(dev);
}

void RGBFusion2USBController::SetMode(int m)
{
    mode = m;
}

RGBA RGBFusion2USBController::GetCalibration(std::string rgb_order)
{
    /*-------------------------------------------------*\
    | Check for RGB order string in calibration table   |
    | If not found return the "BGR" calibration         |
    \*-------------------------------------------------*/
    if(GigabyteCalibrationsLookup.count(rgb_order))
    {
        return GigabyteCalibrationsLookup.find(rgb_order)->second;
    }
    else
    {
        return GigabyteCalibrationsLookup.find("BGR")->second;
    }
}

void RGBFusion2USBController::SetCalibrationBuffer(std::string rgb_order, uint8_t* buffer, uint8_t offset)
{
    RGBA    rgb_cal;
    int     raw_size = sizeof(rgb_cal.raw) / sizeof(rgb_cal.raw[0]);

    rgb_cal = GetCalibration(rgb_order);

    for(int i = 0; i < raw_size; i++)
    {
        buffer[offset + i] = rgb_cal.raw[i];
    }
}

/*---------------------------------------------------------------------------------------------*\
| Sets RGB color mapping to LED pins.                                                           |
| "Custom" RGB packets don't seem to get remapped so use report.byteorderN and do it manually.  |
| Of course it all depends how we send data to the controller, but bios/rgb fusion 2 itself     |
| set it up like this.                                                                          |
\*---------------------------------------------------------------------------------------------*/
void RGBFusion2USBController::SetCalibration()
{
    const std::string detector_name     = "Gigabyte RGB Fusion 2 USB";
    const std::string json_cal          = "Calibration";
    SettingsManager* settings_manager   = ResourceManager::get()->GetSettingsManager();
    json device_settings                = settings_manager->GetSettings(detector_name);

    /*---------------------------------------------------------*\
    | Get Layouts from the settings manager                     |
    | If Calibration settings are not found then write them out |
    | Calibration will only be executed if it is explicitly     |
    | enabled by the user                                       |
    \*---------------------------------------------------------*/
    if(!device_settings.contains(json_cal))
    {
        device_settings[json_cal]["Enabled"]    = false;
        device_settings[json_cal]["Data"]       = GigabyteBoardCalibration;

        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }
    else if(device_settings[json_cal]["Enabled"])
    {
        GigabyteBoardCalibration["D_LED1"]      = device_settings[json_cal]["Data"]["D_LED1"];
        GigabyteBoardCalibration["D_LED2"]      = device_settings[json_cal]["Data"]["D_LED2"];
        GigabyteBoardCalibration["Mainboard"]   = device_settings[json_cal]["Data"]["Mainboard"];
        GigabyteBoardCalibration["Spare"]       = device_settings[json_cal]["Data"]["Spare"];

        uint8_t buffer[64]                      = { 0x00 };
        buffer[0]                               = report_id;
        buffer[1]                               = 0x33;

        SetCalibrationBuffer( GigabyteBoardCalibration.find("D_LED1")->second,      buffer, 2);
        SetCalibrationBuffer( GigabyteBoardCalibration.find("D_LED2")->second,      buffer, 6);
        SetCalibrationBuffer( GigabyteBoardCalibration.find("Mainboard")->second,   buffer, 10);
        SetCalibrationBuffer( GigabyteBoardCalibration.find("Spare")->second,       buffer, 14);

        SendPacket(buffer);
    }
}

void RGBFusion2USBController::SetLedCount(unsigned int led, unsigned int count)
{
    /*-----------------------------------------------------------------*\
    | Check which Digital LED we're setting then send the value of both |
    \*-----------------------------------------------------------------*/
    if(led == HDR_D_LED1)
    {
        D_LED1_count = LedCountToEnum(count);
    }
    else
    {
        D_LED2_count = LedCountToEnum(count);
    }

    SendPacket(0x34, D_LED1_count | (D_LED2_count << 4));
}

bool RGBFusion2USBController::DisableBuiltinEffect(int enable_bit, int mask)
{
    if(effect_disabled & enable_bit)
    {
        return(true);
    }

    effect_disabled &= ~mask;
    effect_disabled |= enable_bit;

    int res = SendPacket(0x32, effect_disabled);

    /*-----------------------------------------------------------------*\
    | Sometimes effect doesn't apply at first, delay a little and let   |
    | MCU react, if this packet is the cause                            |
    \*-----------------------------------------------------------------*/
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    return res;
}

bool RGBFusion2USBController::EnableBeat(bool e)
{
    return SendPacket(0x31, e ? 1 : 0);
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

void RGBFusion2USBController::SetStripColors
    (
    unsigned int    hdr,
    RGBColor *      colors,
    unsigned int    num_colors,
    int             single_led
    )
{
    PktRGB pkt;
    pkt.Init(hdr, report_id);

    /*-------------------------------------------------------------------------*\
    | FIXME assuming that LED strips ports are 0x58/0x59 for all boards         |
    \*-------------------------------------------------------------------------*/
    uint32_t byteorder;

    if(hdr == HDR_D_LED1_RGB)
    {
        byteorder       = report.byteorder0;
    }
    else
    {
        byteorder       = report.byteorder1;
    }

    unsigned char bo_r  = byteorder >> 16;
    unsigned char bo_g  = byteorder >> 8;
    unsigned char bo_b  = byteorder & 0xFF;

    int res;
    int leds_left       = num_colors;
    int sent_data       = 0;
    int k               = 0;
    int leds_in_pkt     = sizeof(pkt.s.leds) / sizeof(*pkt.s.leds); /* 19 */

    /*-------------------------------------------------------------------------*\
    | Other leds stay at whatever the builtin effect was doing at that moment   |
    | if breathing/pulse effect faded out then they stay dark                   |
    \*-------------------------------------------------------------------------*/
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
            RGBColor      color = colors[k];
            unsigned char red   = RGBGetRValue(color);
            unsigned char grn   = RGBGetGValue(color);
            unsigned char blu   = RGBGetBValue(color);

            pkt.buffer[5 + i * 3 + bo_r] = red;
            pkt.buffer[5 + i * 3 + bo_g] = grn;
            pkt.buffer[5 + i * 3 + bo_b] = blu;
            k++;
        }

        res = SendPacket(pkt.buffer);

        if(res < 0)
        {
            return;
        }
    }

    if(hdr == HDR_D_LED1_RGB)
    {
        DisableBuiltinEffect(0x01, 0x01);
    }
    else
    {
        DisableBuiltinEffect(0x02, 0x02);
    }
}

static const std::array< std::array<int, 3>, 5> speeds =
{
    {
        {1600,  1600,   200},
        {1200,  1200,   200},
        {800,   800,    200},
        {400,   400,    200},
        {200,   200,    200},
    },
};

void RGBFusion2USBController::SetLEDEffect(unsigned int led, int mode, unsigned int speed, unsigned char brightness, bool random, unsigned char r, unsigned char g, unsigned char b)
{
    PktEffect pkt;

    pkt.Init(led, report_id);
    pkt.e.effect_type   = mode;
    pkt.e.color0        = r << 16 | g << 8 | b;

    pkt.e.max_brightness = brightness;

    switch(mode)
    {
        case 0: // Direct
        case 1: // Static
            break;

        case 2: //Breathing
        case 3: //Blink
        case 4: // Color Cycle
            if(speed < speeds.size())
            {
                const std::array<int, 3>& s = speeds[speed];

                pkt.e.period0 = s[0];
                pkt.e.period1 = s[1];
                pkt.e.period2 = s[2];
            }

            if(random)
            {
                pkt.e.effect_param0 = 7; // cycle through up to 7 (max?) colors
            }
            break;

        // "Fake" effects
        case 10: // flashing, flashing color cycle
            pkt.e.period0       = 200;
            pkt.e.period1       = 200;
            pkt.e.period2       = 5000 - 1000 * speed; // time between flashing, doesn't seem to be affected by period0/period1
            pkt.e.effect_type   = 3;
            pkt.e.effect_param2 = 2; // flash twice

            if (random)
            {
                pkt.e.effect_param0 = 7;
            }
            break;
    }

    SendPacket(pkt.buffer);
}

bool RGBFusion2USBController::ApplyEffect()
{
    return SendPacket(0x28, 0xFF);
}

bool RGBFusion2USBController::SendPacket(uint8_t a, uint8_t b, uint8_t c)
{
    unsigned char buffer[64] {};

    buffer[0] = report_id;
    buffer[1] = a;
    buffer[2] = b;
    buffer[3] = c;

    return(SendPacket(buffer) == 64);
}

int RGBFusion2USBController::SendPacket(unsigned char* packet)
{
    return hid_send_feature_report(dev, packet, 64);
}

/*-----------------------------------------*\
|  GigabyteRGBFusion2USBController.cpp      |
|                                           |
|  Driver for Gigabyte Aorus RGB Fusion 2.0 |
|  USB lighting controller                  |
|                                           |
|  jackun 1/8/2020                          |
\*-----------------------------------------*/

#include "GigabyteRGBFusion2USBController.h"
#include <algorithm>
#include <array>
#include <thread>
#include <chrono>

static LEDCount LedCountToEnum(unsigned int c)
{
    if (c <= 32)
        return(LEDS_32);
    if (c <= 64)
        return(LEDS_64);
    if (c <= 256)
        return(LEDS_256);
    if (c <= 512)
        return(LEDS_512);

    return(LEDS_1024);
}

RGBFusion2USBController::RGBFusion2USBController(hid_device* handle, const char *path, std::string mb_name) : dev(handle)
{
    int res                     = 0;
    char text[64]               = { 0x00 };
    unsigned char buffer[64]    = { 0x00 };

    if( dev )
    {
        SetCalibration();

        name = mb_name;
        // hid report read needs 0x60 packet or it gives IO error
        SendPacket(0x60, 0x00);

        buffer[0] = report_id;
        res = hid_get_feature_report(dev, buffer, 64);
        if( res > 0 )
        {
            report = *reinterpret_cast<IT8297Report*>(buffer);

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
    if( dev ) {
        hid_close(dev);
    }
}

void RGBFusion2USBController::SetMode(int m)
{
    mode = m;
}

// Sets RGB color mapping to LED pins.
// "Custom" RGB packets don't seem to get remapped so use report.byteorderN and do it manually.
// Of course it all depends how we send data to the controller, but bios/rgb fusion 2 itself
// set it up like this.
void RGBFusion2USBController::SetCalibration()
{
    uint8_t buffer[64] {};
    buffer[0] = report_id;
    buffer[1] = 0x33;

    // D_LED1 WS2812 GRB, 0x00RRGGBB to 0x00GGRRBB
    buffer[2] = 0x02; // B
    buffer[3] = 0x00; // G
    buffer[4] = 0x01; // R
    buffer[5] = 0x00;

    // D_LED2 WS2812 GRB
    buffer[6] = 0x02;
    buffer[7] = 0x00;
    buffer[8] = 0x01;
    buffer[9] = 0x00;

    // LED C1/C2 12vGRB, seems pins already connect to LEDs correctly
    buffer[10] = 0x00;
    buffer[11] = 0x01;
    buffer[12] = 0x02;
    buffer[13] = 0x00;

    // Spare set seen in some Motherboard models
    buffer[14] = 0x00;
    buffer[15] = 0x01;
    buffer[16] = 0x02;
    buffer[17] = 0x00;

    SendPacket(buffer);
}

void RGBFusion2USBController::SetLedCount(unsigned int led, unsigned int count)
{
    //Check which Digital LED we're setting then send the value of both
    ( led == HDR_D_LED1 ) ? D_LED1_count = LedCountToEnum(count) : D_LED2_count = LedCountToEnum(count);
    SendPacket(0x34, D_LED1_count | (D_LED2_count << 4));
}

bool RGBFusion2USBController::DisableBuiltinEffect(int enable_bit, int mask)
{
    if(effect_disabled & enable_bit)
        return(true);

    effect_disabled &= ~mask;
    effect_disabled |= enable_bit;

    int res = SendPacket(0x32, effect_disabled);
    // Sometimes effect doesn't apply at first, delay a little and let MCU to react, if this packet is the cause
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

     // FIXME assuming that LED strips ports are 0x58/0x59 for all boards
    uint32_t byteorder = hdr == HDR_D_LED1_RGB ? report.byteorder0 : report.byteorder1;

    unsigned char bo_r = byteorder >> 16;
    unsigned char bo_g = byteorder >> 8;
    unsigned char bo_b = byteorder & 0xFF;

    int res;
    int leds_left = num_colors;
    int sent_data = 0;
    int k = 0;
    int leds_in_pkt = sizeof(pkt.s.leds) / sizeof(*pkt.s.leds); /* 19 */

    // other leds stay at whatever the builtin effect was doing at that moment
    // if breathing/pulse effect faded out then they stay dark
    if(single_led > -1)
    {
        leds_left = 1;
        k = single_led;
        sent_data = k * 3;
        leds_in_pkt = 1;
    }

    while(leds_left > 0)
    {
        leds_in_pkt = (std::min)(leds_in_pkt, leds_left);
        leds_left -= leds_in_pkt;

        pkt.s.bcount = leds_in_pkt * 3;
        pkt.s.boffset = sent_data;
        sent_data += pkt.s.bcount;

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
            return;
    }

    if (hdr == HDR_D_LED1_RGB)
        DisableBuiltinEffect(0x01, 0x01);
    else
        DisableBuiltinEffect(0x02, 0x02);
}

static const std::array< std::array<int, 3>, 5> speeds = {
    {
        {1600, 1600, 200},
        {1200, 1200, 200},
        {800, 800, 200},
        {400, 400, 200},
        {200, 200, 200},
    },
};

void RGBFusion2USBController::SetLEDEffect(unsigned int led, int mode, unsigned int speed, bool random, unsigned char r, unsigned char g, unsigned char b)
{
    PktEffect pkt;
    pkt.Init(led, report_id);
    pkt.e.effect_type = mode;
    pkt.e.color0 = r << 16 | g << 8 | b;

    if (speed < speeds.size()) {
        const auto& s = speeds[speed];
        pkt.e.period0 = s[0];
        pkt.e.period1 = s[1];
        pkt.e.period2 = s[2];
    }

    switch(mode)
    {
    case 0: break;
    case 1: break; // static
    case 2: // breathing
    case 3: // blink
        if (random)
            pkt.e.effect_param0 = 7; // cycle through up to 7 (max?) colors
        break;
    case 4: // color cycle
        pkt.e.effect_param0 = 7; // cycle through up to 7 (max?) colors
        break;

    // "fake" effects
    case 10: // flashing, flashing color cycle
        pkt.e.period0 = 200;
        pkt.e.period1 = 200;
        pkt.e.period2 = 5000 - 1000 * speed; // time between flashing, doesn't seem to be affected by period0/period1
        pkt.e.effect_type = 3;
        pkt.e.effect_param2 = 2; // flash twice
        if (random)
            pkt.e.effect_param0 = 7;
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
    return (SendPacket(buffer) == 64);
}

int RGBFusion2USBController::SendPacket(unsigned char* packet)
{
    return hid_send_feature_report(dev, packet, 64);
}

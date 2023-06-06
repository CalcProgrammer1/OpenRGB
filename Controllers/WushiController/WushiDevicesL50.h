
#pragma once

#include <string>
#include "RGBController.h"
#include "WushiDevices.h"

/*-----------------------------------------------------*\
| WUSHI product IDs                                  |
\*-----------------------------------------------------*/
#define WU_S80                         0x1234

enum WUSHI_L50_EFFECT
{
    WUSHI_L50_EFFECT_STATIC = 1,
    WUSHI_L50_EFFECT_BREATH = 3,
    WUSHI_L50_EFFECT_WAVE   = 4,
    WUSHI_L50_EFFECT_SMOOTH = 6,
    WUSHI_L50_EFFECT_RACE   = 8,
    WUSHI_L50_EFFECT_STACK  =10,
};

enum WUSHI_L50_BRIGHTNESS
{
    WUSHI_L50_BRIGHTNESS_LOW  = 1,
    WUSHI_L50_BRIGHTNESS_HIGH = 2,
};

enum WUSHI_L50_SPEED
{
    WUSHI_L50_SPEED_SLOWEST = 1,
    WUSHI_L50_SPEED_SLOW    = 2,
    WUSHI_L50_SPEED_FAST    = 3,
    WUSHI_L50_SPEED_FASTEST = 4,
};

enum WUSHI_L50_Direction
{
    WUSHI_L50_Direction_LEFT  = 1,
    WUSHI_L50_Direction_RIGHT = 2,
};
/// struct a USB packet for set the keyboard LEDs
class KeyboardState
{
public:
    uint8_t header[2] = {0xCC, 0x16};
    uint8_t effect = WUSHI_L50_EFFECT_STATIC;
    uint8_t speed = WUSHI_L50_SPEED_SLOWEST;
    uint8_t brightness = WUSHI_L50_BRIGHTNESS_LOW;
    uint8_t zone0_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone1_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone2_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone3_rgb[3] = {0xFF, 0xFF, 0xFF};

     //uint8_t zone_rgb[50][3];
    /*uint8_t zone0_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone1_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone2_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone3_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone4_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone5_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone6_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone7_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone8_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone9_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone10_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone11_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone12_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone13_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone14_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone15_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone16_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone17_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone18_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone19_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone20_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone21_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone22_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone23_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone24_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone25_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone26_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone27_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone28_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone29_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone30_rgb[3] = {0xFF, 0xFF, 0xFF};

    uint8_t zone31_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone32_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone33_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone34_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone35_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone36_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone37_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone38_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone39_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone40_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone41_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone42_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone43_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone44_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone45_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone46_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone47_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone48_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone49_rgb[3] = {0xFF, 0xFF, 0xFF};
    uint8_t zone50_rgb[3] = {0xFF, 0xFF, 0xFF};*/
    uint8_t padding = 0;
    uint8_t wave_ltr = 0;
    uint8_t wave_rtl = 0;
    uint8_t unused[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    void Reset()
    {
        header[0] = 0xCC, header[1] = 0x16;
        effect = WUSHI_L50_EFFECT_STATIC;
        speed = WUSHI_L50_SPEED_SLOWEST;
        brightness = WUSHI_L50_BRIGHTNESS_LOW;
        //zone0_rgb[0] = 0xFF, zone0_rgb[1] = 0xFF, zone0_rgb[2] = 0xFF;
       //zone1_rgb[0] = 0xFF, zone1_rgb[1] = 0xFF, zone1_rgb[2] = 0xFF;
        //zone2_rgb[0] = 0xFF, zone2_rgb[1] = 0xFF, zone2_rgb[2] = 0xFF;
        //zone3_rgb[0] = 0xFF, zone3_rgb[1] = 0xFF, zone3_rgb[2] = 0xFF;
        padding = 0;
        wave_ltr = 0;
        wave_rtl = 0;
        for(int i = 0; i < 13; ++i)
        {
            unused[i] = 0;
        }
    }

    void SetColors(std::vector<RGBColor> group_colors)
    {
        zone0_rgb[0] = RGBGetRValue(group_colors[0]);
        zone0_rgb[1] = RGBGetGValue(group_colors[0]);
        zone0_rgb[2] = RGBGetBValue(group_colors[0]);
        zone1_rgb[0] = RGBGetRValue(group_colors[1]);
        zone1_rgb[1] = RGBGetGValue(group_colors[1]);
        zone1_rgb[2] = RGBGetBValue(group_colors[1]);
        zone2_rgb[0] = RGBGetRValue(group_colors[2]);
        zone2_rgb[1] = RGBGetGValue(group_colors[2]);
        zone2_rgb[2] = RGBGetBValue(group_colors[2]);
        zone3_rgb[0] = RGBGetRValue(group_colors[3]);
        zone3_rgb[1] = RGBGetGValue(group_colors[3]);
        zone3_rgb[2] = RGBGetBValue(group_colors[3]);

        //zone_rgb[0][0]=0x12;zone_rgb[0][1]=0x34;zone_rgb[0][2]=0x56;
        //zone_rgb[1][0]=0x78;zone_rgb[1][1]=0x90;zone_rgb[1][2]=0x12;
        /*for(uint8_t zone=0;zone<50;zone++)
        {
           //for(uint8_t rgb_idx=0;rgb_idx<3;rgb_idx++)
           zone_rgb[zone][0] = RGBGetRValue(group_colors[zone]);
           zone_rgb[zone][1] = RGBGetGValue(group_colors[zone]);
           zone_rgb[zone][2] = RGBGetBValue(group_colors[zone]);
        }*/
        wave_rtl = 0;
    }
};

/*-------------------------*\
| L50 keyboard           |
\*-------------------------*/

static const Wushi_led WUSHI_L50_leds[]
{
        {0x00, "Zone 1"},
        {0x01, "Zone 2"},
        {0x02, "Zone 3"},
        {0x03, "Zone 4"},
/*    {0x00, "Pixel 1"},
    {0x01, "Pixel 2"},
    {0x02, "Pixel 3"},
    {0x03, "Pixel 4"},
        {0x04, "Pixel 5"},
        {0x05, "Pixel 6"},
        {0x06, "Pixel 7"},
        {0x07, "Pixel 8"},
        {0x08, "Pixel 9"},
        {0x09, "Pixel 10"},
        {10, "Pixel 11"},
        {11, "Pixel 12"},

        {12, "Pixel 13"},
        {13, "Pixel 14"},
        {14, "Pixel 15"},
        {15, "Pixel 16"},

        {16, "Pixel 17"},
        {17, "Pixel 18"},
        {18, "Pixel 19"},
        {19, "Pixel 20"},

        {20, "Pixel 21"},
        {21, "Pixel 22"},
        {22, "Pixel 23"},
        {23, "Pixel 24"},

        {24, "Pixel 25"},
        {25, "Pixel 26"},
        {26, "Pixel 27"},
        {27, "Pixel 28"},

        {28, "Pixel 29"},
        {29, "Pixel 30"},
        {30, "Pixel 31"},
        {31, "Pixel 32"},

        {32, "Pixel 33"},
        {33, "Pixel 34"},
        {34, "Pixel 35"},
        {35, "Pixel 36"},

        {36, "Pixel 37"},
        {37, "Pixel 38"},
        {38, "Pixel 39"},
        {39, "Pixel 40"},

        {40, "Pixel 41"},
        {41, "Pixel 42"},
        {42, "Pixel 43"},
        {43, "Pixel 44"},


        {44, "Pixel 45"},
        {45, "Pixel 46"},
        {46, "Pixel 47"},
        {47, "Pixel 48"},

        {48, "Pixel 49"},
        {49, "Pixel 50"},*/

};


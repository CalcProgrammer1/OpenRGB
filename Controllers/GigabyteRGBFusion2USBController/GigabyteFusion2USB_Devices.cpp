/*---------------------------------------------------------*\
| Gigabyte_Fusion2_USB_Devices.cpp                          |
|                                                           |
|   Gigabyte Fusion 2 USB Device layouts and                |
|     and mapping to the device IDs stored on chip          |
|                                                           |
|   megadjc                                     31 Jul 2025 |
|   chrism                                      29 Aug 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "GigabyteRGBFusion2USBController.h"

/*-------------------------------------------------------------------------*\
|  GB Fusion2 - Common Zone Definitions                                     |
\*-------------------------------------------------------------------------*/
static const gb_fusion2_zone common_d_led1_zone =
{
    HDR_D_LED1,
    0,
    1024,
    "Digital LED 1"
};

static const gb_fusion2_zone common_d_led2_zone =
{
    HDR_D_LED2,
    0,
    1024,
    "Digital LED 2"
};

static const gb_fusion2_zone common_d_led3_zone =
{
    HDR_D_LED3,
    0,
    1024,
    "Digital LED 3"
};

static const gb_fusion2_zone common_d_led4_zone =
{
    HDR_D_LED4,
    0,
    1024,
    "Digital LED 4"
};

static const gb_fusion2_zone common_led1_zone =
{
    LED1,
    1,
    1,
    "Name for Led 1"
};

static const gb_fusion2_zone common_led2_zone =
{
    LED2,
    1,
    1,
    "Name for Led 2"
};

static const gb_fusion2_zone common_led3_zone =
{
    LED3,
    1,
    1,
    "Name for Led 3"
};

static const gb_fusion2_zone common_led4_zone =
{
    LED4,
    1,
    1,
    "Name for Led 4"
};

static const gb_fusion2_zone common_led5_zone =
{
    LED5,
    1,
    1,
    "Name for Led 5"
};

static const gb_fusion2_zone common_led9_zone =
{
    LED9,
    1,
    1,
    "Name for Led 9"
};

static const gb_fusion2_zone common_led10_zone =
{
    LED10,
    1,
    1,
    "Name for Led 10"
};

static const gb_fusion2_zone common_led11_zone =
{
    LED11,
    1,
    1,
    "Name for Led 11"
};

static const gb_fusion2_zone common_accent_led1_zone =
{
    LED1,
    1,
    1,
    "Accent",
};

static const gb_fusion2_zone common_accent_led2_zone =
{
    LED2,
    1,
    1,
    "Accent",
};

static const gb_fusion2_zone common_accent_led3_zone =
{
    LED3,
    1,
    1,
    "Accent",
};

static const gb_fusion2_zone common_accent_led4_zone =
{
    LED4,
    1,
    1,
    "Accent",
};

static const gb_fusion2_zone common_accent_led5_zone =
{
    LED5,
    1,
    1,
    "Accent",
};

static const gb_fusion2_zone common_accent_led11_zone =
{
    LED11,
    1,
    1,
    "Accent",
};

static const gb_fusion2_zone common_aorus_logo_led1_zone =
{
    LED1,
    1,
    1,
    "Aorus Logo",
};

static const gb_fusion2_zone common_cover_left_led4_zone =
{
    LED4,
    1,
    1,
    "Cover Left",
};

static const gb_fusion2_zone common_cover_right_led1_zone =
{
    LED1,
    1,
    1,
    "Cover Right",
};

static const gb_fusion2_zone common_cpu_hdr_led3_zone =
{
    LED3,
    1,
    1,
    "CPU Header",
};

static const gb_fusion2_zone common_logo_led3_zone =
{
    LED3,
    1,
    1,
    "Logo"
};

static const gb_fusion2_zone common_logo_led10_zone =
{
    LED10,
    1,
    1,
    "Logo"
};

static const gb_fusion2_zone common_pcie_led4_zone =
{
    LED4,
    1,
    1,
    "PCIe"
};

static const gb_fusion2_zone common_12v_led2_zone =
{
    LED2,
    1,
    1,
    "12V RGB Strip"
};

static const gb_fusion2_zone common_12v_led4_zone =
{
    LED4,
    1,
    1,
    "12V RGB Strip"
};

static const gb_fusion2_zone common_12v_led5_zone =
{
    LED5,
    1,
    1,
    "12V RGB Strip"
};


/*-------------------------------------------------------------------------*\
|  GB Fusion2 Layouts                                                       |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  A620MH    048D:5702                                          |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout a620mh_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_12v_led2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device a620m_ds3h_5702_device =
{
    &a620mh_layout,
    0xCDE4CFFE,
    1,
    "A620M DS3H",
};

static const gb_fusion2_device a620m_gaming_x_5702_device =
{
    &a620mh_layout,
    0xCDE4CFFE,
    1,
    "A620M GAMING X",
};

static const gb_fusion2_device a620m_gaming_x_ax_5702_device =
{
    &a620mh_layout,
    0xCDE4CFFE,
    1,
    "A620M GAMING X AX",
};

static const gb_fusion2_device a620m_h_5702_device =
{
    &a620mh_layout,
    0xCDE4CFFE,
    1,
    "A620M H",
};

/*-------------------------------------------------------------*\
|  B650M-DS3H    048D:5702                                      |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650m_ds3h_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device a620m_ds3h_5711_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "A620M DS3H",
};


static const gb_fusion2_device a620m_gaming_x_5711_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "A620M GAMING X",
};

static const gb_fusion2_device a620m_gaming_x_ax_5711_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "A620M GAMING X AX",
};

static const gb_fusion2_device a620m_h_5711_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "A620M H",
};


static const gb_fusion2_device a620m_s2h_5711_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "A620M S2H",
};

static const gb_fusion2_device b650_eagle_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650 EAGLE",
};

static const gb_fusion2_device b650_eagle_ax_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650 EAGLE AX",
};

static const gb_fusion2_device b650m_d2hp_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M D2HP",
};

static const gb_fusion2_device b650m_d3hp_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M D3HP",
};

static const gb_fusion2_device b650m_d3hp_ax_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M D3HP AX",
};

static const gb_fusion2_device b650m_ds3h_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M DS3H",
};

static const gb_fusion2_device b650m_gaming_plus_wifi_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M GAMING PLUS WIFI",
};

static const gb_fusion2_device b650m_gaming_wifi_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M GAMING WIFI",
};

static const gb_fusion2_device b650m_gaming_wifi6_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M GAMING WIFI6",
};

static const gb_fusion2_device b650m_gaming_x_ax_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M GAMING X AX",
};

static const gb_fusion2_device b650m_k_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B650M K",
};

static const gb_fusion2_device b760_ds3h_gen5_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H GEN5",
};

static const gb_fusion2_device b760_ds3h_wifi6e_gen5_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H WIFI6E GEN5",
};

static const gb_fusion2_device b760_gaming_x_ddr4_gen5_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B760 GAMING X DDR4 GEN5",
};

static const gb_fusion2_device b760_gaming_x_gen5_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B760 GAMING X GEN5",
};

static const gb_fusion2_device b760_gaming_x_wifi6_gen5_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B760 GAMING X WIFI6 GEN5",
};

static const gb_fusion2_device b840m_d2h_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B840M D2H",
};

static const gb_fusion2_device b840m_ds3h_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B840M DS3H",
};

static const gb_fusion2_device b840m_eagle_wifi6_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B840M EAGLE WIFI6",
};

static const gb_fusion2_device b850i_aorus_pro_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B850I AORUS PRO",
};

static const gb_fusion2_device b850m_d3hp_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "B850M D3HP",
};

static const gb_fusion2_device x670_gaming_x_ax_v2_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "X670 GAMING X AX V2",
};

static const gb_fusion2_device x870i_aorus_pro_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "X870I AORUS PRO",
};

static const gb_fusion2_device x870i_aorus_pro_ice_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "X870I AORUS PRO ICE",
};

static const gb_fusion2_device z890m_gaming_x_device =
{
    &b650m_ds3h_layout,
    0xCDE4CFFE,
    1,
    "Z890M GAMING X",
};

/*-------------------------------------------------------------*\
|  B550I-AORUS-PRO-AX    048D:5702                              |
|                                                               |
|    Zone "Digital LED 1"           : Linear                    |
|    Zone "Top LED"                 : Single                    |
|    Zone "Upper Middle LED"        : Single                    |
|    Zone "Lower Middle LED"        : Single                    |
|    Zone "Bottom LED"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_zone top_led_zone =
{
    LED1,
    1,
    1,
    "Top LED"
};

static const gb_fusion2_zone upper_middle_led_zone =
{
    LED2,
    1,
    1,
    "Upper Middle LED"
};

static const gb_fusion2_zone lower_middle_led_zone =
{
    LED3,
    1,
    1,
    "Lower Middle LED"
};

static const gb_fusion2_zone bottom_led_zone =
{
    LED4,
    1,
    1,
    "Bottom LED"
};

static gb_fusion2_layout b550i_aorus_pro_ax_layout =
{
    &common_d_led1_zone,
    &top_led_zone,
    &upper_middle_led_zone,
    &lower_middle_led_zone,
    &bottom_led_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr
};

static const gb_fusion2_device b550i_aorus_pro_ax_device =
{
    &b550i_aorus_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "B550I AORUS PRO AX",
};

static const gb_fusion2_device b760i_aorus_pro_device =
{
    &b550i_aorus_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "B760I AORUS PRO",
};

static const gb_fusion2_device b760i_aorus_pro_ddr4_device =
{
    &b550i_aorus_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "B760I AORUS PRO DDR4",
};

static const gb_fusion2_device x570i_aorus_pro_wifi_device =
{
    &b550i_aorus_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "X570 I AORUS PRO WIFI",
};

static const gb_fusion2_device x570si_aorus_pro_ax_device =
{
    &b550i_aorus_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "X570SI AORUS PRO AX",
};

static const gb_fusion2_device z790i_aorus_ultra_device =
{
    &b550i_aorus_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "Z790I AORUS ULTRA",
};

/*-------------------------------------------------------------*\
|  B650-C-V2    048D:5702                                       |
|                                                               |
|    Zone "Digital LED 1"           : Linear                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650i_ax_layout =
{
    &common_d_led1_zone,
    &common_12v_led2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device a620i_ax_5702_device =
{
    &b650i_ax_layout,
    0xCDE4CFFE,
    1,
    "A620I AX", //The DMI name for both the 5702 & 5711 PID are the smae
};

static const gb_fusion2_device a620m_s2h_5702_device =
{
    &b650i_ax_layout,
    0xCDE4CFFE,
    1,
    "A620M S2H",
};

static const gb_fusion2_device b650i_ax_device =
{
    &b650i_ax_layout,
    0xCDE4CFFE,
    1,
    "B650I AX",
};

static const gb_fusion2_device b650m_c_v2_device =
{
    &b650i_ax_layout,
    0xCDE4CFFE,
    1,
    "B650M C V2",
};

static const gb_fusion2_device b650m_c_v3_device =
{
    &b650i_ax_layout,
    0xCDE4CFFE,
    1,
    "B650M C V3",
};

/*-------------------------------------------------------------*\
|  B650-D2H    048D:5702                                        |
|                                                               |
|    Zone "Digital LED 1"           : Linear                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650_d2h_layout =
{
    &common_d_led1_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device a620i_ax_5711_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "A620I AX", //The DMI name for both the 5702 & 5711 PID are the smae
};

static const gb_fusion2_device a620m_c_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "A620M C",
};

static const gb_fusion2_device a620m_d2h_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "A620M D2H",
};

static const gb_fusion2_device b650i_aorus_ultra_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "B650I AORUS ULTRA",
};

static const gb_fusion2_device b650m_d2h_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "B650M D2H",
};

static const gb_fusion2_device b650m_h_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "B650M H",
};


static const gb_fusion2_device b650m_s2h_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "B650M S2H",
};

static const gb_fusion2_device b650_ud_ac_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "B650 UD AC",
};

static const gb_fusion2_device b650_ud_ax_device =
{
    &b650_d2h_layout,
    0xCDE4CFFE,
    1,
    "B650 UD AX",
};

/*-------------------------------------------------------------*\
|  B650E-AORUS-MSTR    048D:5702                                |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Cover Left               : Single                    |
|    Zone "Cover Right              : Single                    |
|    Zone "CPU Header"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650e_aorus_mstr_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_cover_left_led4_zone,
    &common_cover_right_led1_zone,
    &common_cpu_hdr_led3_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device b650e_aorus_mstr_device =
{
    &b650e_aorus_mstr_layout,
    0xCDE4CFFE,
    1,
    "B650E AORUS MASTER",
};

static const gb_fusion2_device x670e_aorus_mstr_device =
{
    &b650e_aorus_mstr_layout,
    0xCDE4CFFE,
    1,
    "X670E AORUS MASTER",
};

static const gb_fusion2_device x670e_aorus_xtrm_device =
{
    &b650e_aorus_mstr_layout,
    0xCDE4CFFE,
    1,
    "X670E AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  B650E-AORUS-STEALTH    048D:5711                             |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650e_aorus_stealth_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led11_zone,
    &common_logo_led10_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650e_aorus_stealth_ice_device =
{
    &b650e_aorus_stealth_layout,
    0xCDE4CFFE,
    1,
    "B650E AORUS STEALTH ICE",
};

/*-------------------------------------------------------------*\
|  B650-ELITE    048D:5702                                      |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "CPU Header"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650_elite_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led4_zone,
    &common_cpu_hdr_led3_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650_aorus_elite_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "B650 AORUS ELITE",
};

static const gb_fusion2_device b650_aorus_elite_ax_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "B650 AORUS ELITE AX",
};

static const gb_fusion2_device b650m_aorus_elite_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "B650M AORUS ELITE",
};

static const gb_fusion2_device b650m_aorus_elite_ax_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "B650M AORUS ELITE AX",
};

static const gb_fusion2_device b650m_aorus_elite_ax_ice_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "B650M AORUS ELITE AX ICE",
};

static const gb_fusion2_device b650m_aorus_pro_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "B650M AORUS PRO",
};

static const gb_fusion2_device b650m_aorus_pro_ax_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "B650M AORUS PRO AX",
};

static const gb_fusion2_device x570s_aorus_elite_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "X570S AORUS ELITE",
};

static const gb_fusion2_device x570s_aorus_elite_ax_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "X570S AORUS ELITE AX",
};

static const gb_fusion2_device x570s_gaming_x_device =
{
    &b650_elite_layout,
    0xCDE4CFFE,
    1,
    "X570S GAMING X",
};

/*-------------------------------------------------------------*\
|  B650-ELITE-V2    048D:5702                                   |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650_elite_v2_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led4_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650_aorus_elite_ax_ice_device =
{
    &b650_elite_v2_layout,
    0xCDE4CFFE,
    1,
    "B650 AORUS ELITE AX ICE",
};

static const gb_fusion2_device b650_aorus_elite_ax_v2_device =
{
    &b650_elite_v2_layout,
    0xCDE4CFFE,
    1,
    "B650 AORUS ELITE AX V2",
};

static const gb_fusion2_device b650_aorus_elite_v2_device =
{
    &b650_elite_v2_layout,
    0xCDE4CFFE,
    1,
    "B650 AORUS ELITE V2",
};

static const gb_fusion2_device b650e_aorus_elite_x_ax_ice_device =
{
    &b650_elite_v2_layout,
    0xCDE4CFFE,
    1,
    "B650E AORUS ELITE X AX ICE",
};

/*-------------------------------------------------------------*\
|  B650-TACH    048D:5702                                       |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650_tach_layout =
{
    &common_d_led1_zone,
    &common_accent_led3_zone,
    &common_12v_led2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device b650e_aorus_tachyon_device =
{
    &b650_tach_layout,
    0xCDE4CFFE,
    1,
    "B650E AORUS TACHYON",
};

/*-------------------------------------------------------------*\
|  B650-USB4    048D:5702                                       |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b650_usb4_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led3_zone,
    &common_accent_led4_zone,
    &common_aorus_logo_led1_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650e_aorus_pro_x_usb4_device =
{
    &b650_usb4_layout,
    0xCDE4CFFE,
    1,
    "B650E AORUS PRO X USB4",
};

/*-------------------------------------------------------------*\
|  B760_AORUS_ELITE    048D:5702                                |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b760_aorus_elite_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device b760_aorus_elite_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 AORUS ELITE",
};

static const gb_fusion2_device b760_aorus_elite_ax_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 AORUS ELITE AX",
};

static const gb_fusion2_device b760_ds3h_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H",
};

static const gb_fusion2_device b760_ds3h_ac_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H AC",
};

static const gb_fusion2_device b760_ds3h_ac_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H AC DDR4",
};

static const gb_fusion2_device b760_ds3h_ax_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H AX",
};

static const gb_fusion2_device b760_ds3h_ax_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H AX DDR4",
};

static const gb_fusion2_device b760_ds3h_ax_v2_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H AX V2",
};

static const gb_fusion2_device b760_ds3h_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 DS3H DDR4",
};

static const gb_fusion2_device b760_gaming_x_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 GAMING X",
};

static const gb_fusion2_device b760_gaming_x_ax_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 GAMING X AX",
};

static const gb_fusion2_device b760_gaming_x_ax_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 GAMING X AX DDR4",
};

static const gb_fusion2_device b760_gaming_x_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 GAMING X DDR4",
};

static const gb_fusion2_device b760m_c_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760M C",
};

static const gb_fusion2_device b760m_c_v2_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760M C V2",
};

static const gb_fusion2_device b760m_gaming_x_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X",
};

static const gb_fusion2_device b760m_gaming_x_ax_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X AX",
};

static const gb_fusion2_device b760m_gaming_x_ax_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X AX DDR4",
};

static const gb_fusion2_device b760m_gaming_x_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X DDR4",
};

static const gb_fusion2_device z790_aero_g_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 AERO G",
};

static const gb_fusion2_device z790_gaming_plus_ax_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 GAMING PLUS AX",
};

static const gb_fusion2_device z790_gaming_x_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 GAMING X",
};

static const gb_fusion2_device z790_gaming_x_ax_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 GAMING X AX",
};

static const gb_fusion2_device z790_s_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 S DDR4",
};

static const gb_fusion2_device z790_s_wifi_ddr4_device =
{
    &b760_aorus_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 S WIFI DDR4",
};

/*-------------------------------------------------------------*\
|  B760M-D2H    048D:5702                                       |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b760m_d2h_layout =
{
    &common_d_led1_zone,
    &common_accent_led3_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device b760m_d2h_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M D2H",
};

static const gb_fusion2_device b760m_d2h_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M D2H DDR4",
};

static const gb_fusion2_device b760m_d3h_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M D3H",
};

static const gb_fusion2_device b760m_d3h_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M D3H DDR4",
};

static const gb_fusion2_device b760m_d3hp_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M D3HP",
};

static const gb_fusion2_device b760m_d3hp_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M D3HP DDR4",
};

static const gb_fusion2_device b760m_d3hp_wifi6_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M D3HP WIFI6",
};

static const gb_fusion2_device b760m_ds3h_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M DS3H",
};

static const gb_fusion2_device b760m_ds3h_ax_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M DS3H AX",
};

static const gb_fusion2_device b760m_ds3h_ax_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M DS3H AX DDR4",
};

static const gb_fusion2_device b760m_ds3h_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M DS3H DDR4",
};

static const gb_fusion2_device b760m_gaming_ac_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING AC DDR4",
};

static const gb_fusion2_device b760m_gaming_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING DDR4",
};

static const gb_fusion2_device b760m_gaming_plus_wifi_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING PLUS WIFI DDR4",
};

static const gb_fusion2_device b760m_power_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M POWER",
};

static const gb_fusion2_device b760m_power_ddr4_device =
{
    &b760m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B760M POWER DDR4",
};

/*-------------------------------------------------------------*\
|  B760_DS3H_DR_G5    048D:5711                                 |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b760_ds3h_dr_g5_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led1_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_ds3h_ddr4_gen_5_device =
{
    &b760_ds3h_dr_g5_layout,
    0xCDE4CFFE,
    1,
    "B760M DS3H DDR4 GEN 5",
};

static const gb_fusion2_device b760m_ds3h_gen_5_device =
{
    &b760_ds3h_dr_g5_layout,
    0xCDE4CFFE,
    1,
    "B760M DS3H GEN 5",
};

static const gb_fusion2_device b760m_ds3h_wifi6e_ddr4_gen_5_device =
{
    &b760_ds3h_dr_g5_layout,
    0xCDE4CFFE,
    1,
    "B760M DS3H WIFI6E DDR4 GEN 5",
};

static const gb_fusion2_device b760m_gaming_wifi6e_gen_5_device =
{
    &b760_ds3h_dr_g5_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING WIFI6E GEN 5",
};

static const gb_fusion2_device b760m_gaming_wifi6_plus_gen5_device =
{
    &b760_ds3h_dr_g5_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING WIFI6 PLUS GEN5",
};

static const gb_fusion2_device z790_aorus_mstr_device =
{
    &b760_ds3h_dr_g5_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  B760M-EXAX    048D:5702                                      |
|                                                               |
|    Zone "Digital LED 1"           : Linear                    |
|    Zone "Top Logo"                : Single                    |
|    Zone "Upper Middle Logo"       : Single                    |
|    Zone "Lower Middle Logo"       : Single                    |
|    Zone "Bottom Logo"             : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_zone top_logo_zone =
{
    LED4,
    1,
    1,
    "Top Logo",
};

static const gb_fusion2_zone upper_middle_logo_zone =
{
    LED3,
    1,
    1,
    "Upper Middle Logo",
};

static const gb_fusion2_zone lower_middle_logo_zone =
{
    LED2,
    1,
    1,
    "Lower Middle Logo",
};

static const gb_fusion2_zone bottom_logo_zone =
{
    LED1,
    1,
    1,
    "Bottom Logo",
};

static gb_fusion2_layout b760m_elite_x_ax_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &top_logo_zone,
    &upper_middle_logo_zone,
    &lower_middle_logo_zone,
    &bottom_logo_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device b760m_elite_x_ax_device =
{
    &b760m_elite_x_ax_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE X AX",
};

/*-------------------------------------------------------------*\
|  B760M-GAX    048D:5702                                       |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b760m_gax_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led5_zone,
    &common_12v_led2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_gaming_device =
{
    &b760m_gax_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING",
};

static const gb_fusion2_device b760m_gaming_ac_device =
{
    &b760m_gax_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING AC",
};

static const gb_fusion2_device b760m_gaming_wifi_device =
{
    &b760m_gax_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING WIFI",
};

static const gb_fusion2_device b760m_gaming_wifi_plus_device =
{
    &b760m_gax_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING WIFI PLUS",
};

static const gb_fusion2_device b760m_g_ax_device =
{
    &b760m_gax_layout,
    0xCDE4CFFE,
    1,
    "B760M G AX",
};

/*-------------------------------------------------------------*\
|  B860M-D2H    048D:5711                                       |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b860m_d2h_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b860m_d2h_device =
{
    &b860m_d2h_layout,
    0xCDE4CFFE,
    1,
    "B860M D2H",
};

/*-------------------------------------------------------------*\
|  B860-WIFI7    048D:5711                                      |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b860_wifi7_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_accent_led3_zone,
    &common_logo_led10_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b850m_aorus_pro_wifi7_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850M AORUS PRO WIFI7",
};

static const gb_fusion2_device b860_aorus_elite_wifi7_ice_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B860 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device b860m_aorus_elite_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B860M AORUS ELITE",
};

static const gb_fusion2_device b860m_aorus_elite_wifi6e_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B860M AORUS ELITE WIFI6E",
};

static const gb_fusion2_device b860m_aorus_elite_wifi6e_ice_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B860M AORUS ELITE WIFI6E ICE",
};

static const gb_fusion2_device b860m_aorus_pro_wifi7_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B860M AORUS PRO WIFI7",
};

static const gb_fusion2_device z890_aorus_elite_wifi7_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS ELITE WIFI7",
};

static const gb_fusion2_device z890_aorus_elite_wifi7_ice_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device z890_aorus_elite_x_ice_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS ELITE X ICE",
};

static const gb_fusion2_device z890_aorus_pro_ice_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS PRO ICE",
};

static const gb_fusion2_device z890_eagle_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890 EAGLE",
};

static const gb_fusion2_device z890_eagle_wifi7_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890 EAGLE WIFI7",
};

static const gb_fusion2_device z890_gaming_x_wifi7_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890 GAMING X WIFI7",
};

static const gb_fusion2_device z890m_aorus_elite_wifi7_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890M AORUS ELITE WIFI7",
};

static const gb_fusion2_device z890m_aorus_elite_wifi7_ice_device =
{
    &b860_wifi7_layout,
    0xCDE4CFFE,
    1,
    "Z890M AORUS ELITE WIFI7 ICE",
};

/*-------------------------------------------------------------*\
|  H810M    048D:8950                                           |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_zone gen2_argb1_zone =
{
    LED3,
    0,
    1024,
    "Gen2 ARGB 1"
};

static const gb_fusion2_zone gen2_argb2_zone =
{
    LED4,
    0,
    1024,
    "Gen2 ARGB 2"
};

static gb_fusion2_layout h810m_layout =
{
    &gen2_argb1_zone,
    &gen2_argb2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device h810m_gaming_wifi6_device =
{
    &h810m_layout,
    0xCDE4CFFE,
    1,
    "H810M GAMING WIFI6",
};

static const gb_fusion2_device h810m_h_device =
{
    &h810m_layout,
    0xCDE4CFFE,
    1,
    "H810M H",
};

static const gb_fusion2_device h810m_s2h_device =
{
    &h810m_layout,
    0xCDE4CFFE,
    1,
    "H810M S2H",
};

/*-------------------------------------------------------------*\
|  MSTR_ATX    048D:8297                                        |
|                                                               |
|    Zone "Digital LED 1"           : Linear                    |
|    Zone "Back IO & Digital LED 2" : Linear                    |
|    Zone "Chipset Logo"            : Single                    |
|    Zone "PCIe"                    : Single                    |
|    Zone "XMP Logo"                : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_zone back_io_dled_zone =
{
    HDR_D_LED2,
    8,
    1024,
    "Back IO & Digital LED 2"
};

static const gb_fusion2_zone xmp_logo_zone =
{
    LED2,
    1,
    1,
    "XMP Logo"
};

static gb_fusion2_layout mstr_atx_layout =
{
    &common_d_led1_zone,
    &back_io_dled_zone,
    &common_logo_led3_zone,
    &common_pcie_led4_zone,
    &xmp_logo_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr
};

static const gb_fusion2_device z390_aorus_master_cf_device =
{
    &mstr_atx_layout,
    0xCDE4CFFE,
    1,
    "Z390 AORUS MASTER-CF",
};

/*-------------------------------------------------------------*\
|  MSTR_ATX_3    048D:8297                                      |
|                                                               |
|    Zone "Digital LED 1"           : Linear                    |
|    Zone "Digital LED 2"           : Linear                    |
|    Zone "Chipset Logo"            : Single                    |
|    Zone "XMP Logo"                : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_zone aorus_logo_zone =
{
    LED7,
    1,
    1,
    "Aorus Logo",
};

static const gb_fusion2_zone ess_logo_zone =
{
    LED4,
    1,
    1,
    "ESS Logo",
};

static gb_fusion2_layout mstr_atx_3_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &aorus_logo_zone,
    &ess_logo_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device x570_aorus_master_device =
{
    &mstr_atx_3_layout,
    0xCDE4CFFE,
    1,
    "X570 AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  STD_ATX    048D:8297                                         |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Back I/O"                : Single                    |
|    Zone "PCIe"                    : Single                    |
|    Zone "CPU Header"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_zone back_io_zone =
{
    LED1,
    1,
    1,
    "Back I/O",
};

static const gb_fusion2_zone cpu_hdr_led2_zone =
{
    LED2,
    1,
    1,
    "CPU Header",
};

static gb_fusion2_layout std_atx_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &back_io_zone,
    &common_pcie_led4_zone,
    &cpu_hdr_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr
};

static const gb_fusion2_device b550_aorus_elite_device =
{
    &std_atx_layout,
    0xCDE4CFFE,
    1,
    "B550 AORUS ELITE",
};

static const gb_fusion2_device b550_aorus_pro_device =
{
    &std_atx_layout,
    0xCDE4CFFE,
    1,
    "B550 AORUS PRO",
};

static const gb_fusion2_device x570_aorus_elite_device =
{
    &std_atx_layout,
    0xCDE4CFFE,
    1,
    "X570 AORUS ELITE",
};

static const gb_fusion2_device x570_aorus_elite_wifi_device =
{
    &std_atx_layout,
    0xCDE4CFFE,
    1,
    "X570 AORUS ELITE WIFI",
};

static const gb_fusion2_device x570_aorus_pro_device =
{
    &std_atx_layout,
    0xCDE4CFFE,
    1,
    "X570 AORUS PRO",
};

static const gb_fusion2_device x570_aorus_pro_wifi_device =
{
    &std_atx_layout,
    0x0130005F,
    1,
    "X570 AORUS PRO WIFI",
};

static const gb_fusion2_device x570_aorus_ultra_device =
{
    &std_atx_layout,
    0xCDE4CFFE,
    1,
    "X570 AORUS ULTRA",
};

/*-------------------------------------------------------------------------*\
|  Generic Layout (Used when no match found)                                |
\*-------------------------------------------------------------------------*/
static const gb_fusion2_device generic_device =
{
    &std_atx_layout,
    0xCDE4CFFE,
    1,
    "GENERIC LAYOUT",
};

/*-------------------------------------------------------------*\
|  TRX50_AERO_D    048D:5702                                    |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout trx50_aero_d_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led3_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device trx50_aero_d_device =
{
    &trx50_aero_d_layout,
    0xCDE4CFFE,
    1,
    "TRX50 AERO D",
};

static const gb_fusion2_device z790_aorus_elite_x_device =
{
    &trx50_aero_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE X",
};

static const gb_fusion2_device z790_aorus_elite_x_ax_device =
{
    &trx50_aero_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE X AX",
};

static const gb_fusion2_device z790_aorus_elite_x_wifi7_device =
{
    &trx50_aero_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE X WIFI7",
};

/*-------------------------------------------------------------*\
|  X570-PRO-AX    048D:5702                                     |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Logo"                    : Single                    |
|    Zone "Accent"                  : Single                    |
|    Zone "CPU Header"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x570_pro_ax_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_aorus_logo_led1_zone,
    &common_accent_led4_zone,
    &common_cpu_hdr_led3_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device b550_aorus_elite_ax_v2_device =
{
    &x570_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "B550 AORUS ELITE AX V2",
};

static const gb_fusion2_device x570s_aorus_pro_ax_device =
{
    &x570_pro_ax_layout,
    0xCDE4CFFE,
    1,
    "X570S AORUS PRO AX",
};

/*-------------------------------------------------------------*\
|  X570S-AORUS_MSTR    048D:5702                                |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Logo"                    : Single                    |
|    Zone "CPU Header"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x570s_aorus_mstr_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_aorus_logo_led1_zone,
    &common_cpu_hdr_led3_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x570s_aorus_mstr_device =
{
    &x570s_aorus_mstr_layout,
    0xCDE4CFFE,
    1,
    "X570S AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  X670-ELITE    048D:8297                                      |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "CPU Header"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x670_elite_ax_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_cpu_hdr_led3_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device b650_aero_g_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "B650 AERO G",
};

static const gb_fusion2_device b650_aorus_pro_ax_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "B650 AORUS PRO AX",
};

static const gb_fusion2_device b650_gaming_x_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "B650 GAMING X",
};

static const gb_fusion2_device b650_gaming_x_ax_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "B650 GAMING X AX",
};

static const gb_fusion2_device b650_gaming_x_ax_v2_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "B650 GAMING X AX V2",
};

static const gb_fusion2_device b650_gaming_x_v2_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "B650 GAMING X V2",
};

static const gb_fusion2_device x570s_aero_g_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "X570S AERO G",
};

static const gb_fusion2_device x570s_ud_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "X570S UD",
};

static const gb_fusion2_device x670_aorus_elite_ax_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "X670 AORUS ELITE AX",
};

static const gb_fusion2_device x670_gaming_x_ax_device =
{
    &x670_elite_ax_layout,
    0xCDE4CFFE,
    1,
    "X670 GAMING X AX",
};

/*-------------------------------------------------------------*\
|  X670-A-PRO-X    048D:5702                                    |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Cover Left               : Single                    |
|    Zone "Cover Right              : Single                    |
|    Zone "CPU Header"              : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x670_aorus_pro_x_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_cover_left_led4_zone,
    &common_cover_right_led1_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x670_aorus_pro_x_device =
{
    &x670_aorus_pro_x_layout,
    0xCDE4CFFE,
    1,
    "X670E AORUS PRO X",
};

/*-------------------------------------------------------------*\
|  B860-GAMING-X    048D:5711                                   |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout b860_gaming_x_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_accent_led1_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device b860m_eagle_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M EAGLE",
};

static const gb_fusion2_device b860m_eagle_ds3h_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M EAGLE DS3H",
};

static const gb_fusion2_device b860m_eagle_ds3h_wifi6e_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M EAGLE DS3H WIFI6E",
};

static const gb_fusion2_device b860m_eagle_plus_wifi6e_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M EAGLE PLUS WIFI6E",
};

static const gb_fusion2_device b860m_eagle_wifi6_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M EAGLE WIFI6",
};

static const gb_fusion2_device b860m_eagle_wifi6_v2_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M EAGLE WIFI6 V2",
};

static const gb_fusion2_device b860m_gaming_x_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M GAMING X",
};

static const gb_fusion2_device b860m_gaming_x_wifi6e_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M GAMING X WIFI6E",
};

static const gb_fusion2_device b860m_power_device =
{
    &b860_gaming_x_layout,
    0xCDE4CFFE,
    1,
    "B860M POWER",
};

/*-------------------------------------------------------------*\
|  X870-EGL-WIFI6    048D:5711                                  |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x870_egl_wifi6_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_c_v3_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B760M C V3",
};

static const gb_fusion2_device b760m_gaming_x_ddr4_gen5_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X DDR4 GEN5",
};

static const gb_fusion2_device b760m_gaming_x_gen5_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X GEN5",
};

static const gb_fusion2_device b760m_gaming_x_wifi6e_ddr4_gen5_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X WIFI6E DDR4 GEN5",
};

static const gb_fusion2_device b760m_gaming_x_wifi6e_gen5_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B760M GAMING X WIFI6E GEN5",
};

static const gb_fusion2_device b850_ai_top_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850 AI TOP",
};

static const gb_fusion2_device b850_eagle_ice_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850 EAGLE ICE",
};

static const gb_fusion2_device b850_eagle_wifi6e_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850 EAGLE WIFI6E",
};

static const gb_fusion2_device b850_eagle_wifi7_ice_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850 EAGLE WIFI7 ICE",
};

static const gb_fusion2_device b850_gaming_wifi6_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850 GAMING WIFI6",
};

static const gb_fusion2_device b850m_ds3h_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850M DS3H",
};

static const gb_fusion2_device b850m_ds3h_ice_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850M DS3H ICE",
};

static const gb_fusion2_device b850m_eagle_wifi6e_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850M EAGLE WIFI6E",
};

static const gb_fusion2_device b850m_eagle_wifi6e_ice_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850M EAGLE WIFI6E ICE",
};

static const gb_fusion2_device b850m_force_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850M FORCE",
};

static const gb_fusion2_device b850m_force_wifi6e_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B850M FORCE WIFI6E",
};

static const gb_fusion2_device b860_force_wifi6e_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B860 EAGLE WIFI6E",
};

static const gb_fusion2_device b860m_gaming_wifi6_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "B860M GAMING WIFI6",
};

static const gb_fusion2_device trx50_ai_top_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "TRX50 AI TOP",
};

static const gb_fusion2_device w790_ai_top_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "W790 AI TOP",
};

static const gb_fusion2_device w880_ai_top_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "W880 AI TOP",
};

static const gb_fusion2_device z890_aero_d_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "Z890 AERO D",
};

static const gb_fusion2_device z890_aero_g_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "Z890 AERO G",
};

static const gb_fusion2_device z890_ai_top_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "Z890 AI TOP",
};

static const gb_fusion2_device z890_aorus_tachyon_ice_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS TACHYON ICE",
};

static const gb_fusion2_device z890_ud_wifi6e_device =
{
    &x870_egl_wifi6_layout,
    0xCDE4CFFE,
    1,
    "Z890 UD WIFI6E",
};

/*-------------------------------------------------------------*\
|  X870E-PRO    048D:5711                                       |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x870e_pro_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_accent_led11_zone,
    &common_logo_led10_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x870e_aorus_pro_device =
{
    &x870e_pro_layout,
    0xCDE4CFFE,
    1,
    "X870E AORUS PRO",
};

static const gb_fusion2_device x870e_aorus_pro_ice_device =
{
    &x870e_pro_layout,
    0xCDE4CFFE,
    1,
    "X870E AORUS PRO ICE",
};

static const gb_fusion2_device x870e_aorus_master_device =
{
    &x870e_pro_layout,
    0xCDE4CFFE,
    1,
    "X870E AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  X870-WIFI7    048D:5711                                      |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x870_wifi7_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_accent_led3_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device b760m_aorus_elite_ddr4_gen5_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE DDR4 GEN5",
};

static const gb_fusion2_device b760m_aorus_elite_gen5_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE GEN5",
};

static const gb_fusion2_device b760m_aorus_elite_wifi6_ddr4_gen5_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE WIFI6 DDR4 GEN5",
};

static const gb_fusion2_device b760m_aorus_elite_wifi6_gen5_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE WIFI6 GEN5",
};

static const gb_fusion2_device b840_aorus_elite_wifi6e_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B840M AORUS ELITE WIFI6E",
};

static const gb_fusion2_device b850_aorus_elite_wifi7_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850 AORUS ELITE WIFI7",
};

static const gb_fusion2_device b850_aorus_elite_wifi7_ice_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device b850_gaming_x_wifi6e_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850 GAMING X WIFI6E",
};

static const gb_fusion2_device b850m_aorus_elite_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850M AORUS ELITE",
};

static const gb_fusion2_device b850m_aorus_elite_wifi6e_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850M AORUS ELITE WIFI6E",
};

static const gb_fusion2_device b850m_aorus_elite_wifi6e_ice_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850M AORUS ELITE WIFI6E ICE",
};

static const gb_fusion2_device b850m_gaming_x_wifi6e_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "B850M GAMING X WIFI6E",
};

static const gb_fusion2_device x870_aorus_elite_wifi7_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "X870 AORUS ELITE WIFI7",
};

static const gb_fusion2_device x870_aorus_elite_wifi7_ice_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "X870 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device x870_eagle_wifi7_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "X870 EAGLE WIFI7",
};

static const gb_fusion2_device x870_gaming_wifi6_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "X870 GAMING WIFI6",
};

static const gb_fusion2_device x870_gaming_x_wifi_device =
{
    &x870_wifi7_layout,
    0xCDE4CFFE,
    1,
    "X870 GAMING X WIFI",
};

/*-------------------------------------------------------------*\
|  X870E-WIFI7    048D:5711                                     |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout x870e_wifi7_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_accent_led11_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device x870e_aorus_elite_wifi7_device =
{
    &x870e_wifi7_layout,
    0xCDE4CFFE,
    1,
    "X870E AORUS ELITE WIFI7",
};

static const gb_fusion2_device x870e_aorus_elite_wifi7_ice_device =
{
    &x870e_wifi7_layout,
    0xCDE4CFFE,
    1,
    "X870E AORUS ELITE WIFI7 ICE",
};

/*-------------------------------------------------------------*\
|  X870E-XTRM-AI    048D:5711                                   |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "Antenna Accent"          : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_zone antenna_zone =
{
    LED9,
    1,
    1,
    "Antenna Accent"
};

static gb_fusion2_layout x870e_xtrm_ai_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_accent_led11_zone,
    &common_logo_led10_zone,
    &antenna_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device x870e_aorus_xtrm_ai_device =
{
    &x870e_xtrm_ai_layout,
    0xCDE4CFFE,
    1,
    "X870E AORUS XTREME AI TOP",
};

/*-------------------------------------------------------------*\
|  Z690-ELITE    048D:5702                                      |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z690_elite_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led3_zone,
    &common_accent_led4_zone,
    &common_aorus_logo_led1_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device b760_aorus_master_ddr4_device =
{
    &z690_elite_layout,
    0xCDE4CFFE,
    1,
    "B760 AORUS MASTER DDR4",
};

static const gb_fusion2_device z690_aorus_elite_device =
{
    &z690_elite_layout,
    0xCDE4CFFE,
    1,
    "Z690 AORUS ELITE",
};

static const gb_fusion2_device z690_aorus_elite_ax_device =
{
    &z690_elite_layout,
    0xCDE4CFFE,
    1,
    "Z690 AORUS ELITE AX",
};

static const gb_fusion2_device z690_aorus_elite_ax_ddr4_device =
{
    &z690_elite_layout,
    0xCDE4CFFE,
    1,
    "Z690 AORUS ELITE AX DDR4",
};

static const gb_fusion2_device z690_aorus_elite_ddr4_device =
{
    &z690_elite_layout,
    0xCDE4CFFE,
    1,
    "Z690 AORUS ELITE DDR4",
};

/*-------------------------------------------------------------*\
|  Z790-D    048D:5702                                          |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z790_d_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device b860i_aorus_pro_ice_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "B860I AORUS PRO ICE",
};

static const gb_fusion2_device h610m_d3h_ddr4_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "H610M D3H DDR4",
};

static const gb_fusion2_device h610m_d3h_wifi_ddr4_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "H610M D3H WIFI DDR4",
};

static const gb_fusion2_device h610m_gaming_wifi_ddr4_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "H610M GAMING WIFI DDR4",
};

static const gb_fusion2_device z790_d_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 D",
};

static const gb_fusion2_device z790_d_ac_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 D AC",
};

static const gb_fusion2_device z790_d_ax_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 D AX",
};

static const gb_fusion2_device z790_d_wifi_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 D WIFI",
};

static const gb_fusion2_device z790_eagle_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 EAGLE",
};

static const gb_fusion2_device z790_eagle_ax_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 EAGLE AX",
};

static const gb_fusion2_device z790_ud_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 UD",
};

static const gb_fusion2_device z790_ud_ac_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 UD AC",
};

static const gb_fusion2_device z790_ud_ax_device =
{
    &z790_d_layout,
    0xCDE4CFFE,
    1,
    "Z790 UD AX",
};

/*-------------------------------------------------------------*\
|  Z790-ELITE    048D:5702                                      |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z790_elite_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_logo_led3_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr
};

static const gb_fusion2_device z790_aorus_elite_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE",
};

static const gb_fusion2_device z790_aorus_elite_ax_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE AX",
};

static const gb_fusion2_device z790_aorus_elite_ax_ddr4_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE AX DDR4",
};

static const gb_fusion2_device z790_aorus_elite_ax_ice_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE AX ICE",
};

static const gb_fusion2_device z790_aorus_elite_ax_w_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE AX-W",
};

static const gb_fusion2_device z790_aorus_elite_ddr4_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS ELITE DDR4",
};

static const gb_fusion2_device z790m_aorus_elite_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790M AORUS ELITE",
};

static const gb_fusion2_device z790m_aorus_elite_ax_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790M AORUS ELITE AX",
};

static const gb_fusion2_device z790m_aorus_elite_ax_ice_device =
{
    &z790_elite_layout,
    0xCDE4CFFE,
    1,
    "Z790M AORUS ELITE AX ICE",
};

/*-------------------------------------------------------------*\
|  Z790-MSTR-X    048D:5702                                     |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z790_mstr_x_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_aorus_logo_led1_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z790_aorus_master_x_device =
{
    &z790_mstr_x_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS MASTER X",
};

static const gb_fusion2_device z790_aorus_tachyon_x_device =
{
    &z790_mstr_x_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS TACHYON X",
};

/*-------------------------------------------------------------*\
|  Z790-PRO-X    048D:5702                                      |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Logo"                    : Single                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z790_pro_x_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_aorus_logo_led1_zone,
    &common_accent_led4_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z790_aorus_pro_x_device =
{
    &z790_pro_x_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS PRO X",
};

static const gb_fusion2_device z790_aorus_pro_x_wifi7_device =
{
    &z790_pro_x_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS PRO X WIFI7",
};

/*-------------------------------------------------------------*\
|  Z790-XTRM    048D:5702                                       |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z790_xtrm_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led3_zone,
    &common_12v_led2_zone,
    &common_12v_led5_zone,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_aorus_elite_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE",
};

static const gb_fusion2_device b760m_aorus_elite_ax_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE AX",
};

static const gb_fusion2_device b760m_aorus_elite_ax_ddr4_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE AX DDR4",
};

static const gb_fusion2_device b760m_aorus_elite_ddr4_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS ELITE DDR4",
};

static const gb_fusion2_device b760m_aorus_pro_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS PRO",
};

static const gb_fusion2_device b760m_aorus_pro_ax_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS PRO AX",
};

static const gb_fusion2_device b760m_aorus_pro_ax_ddr4_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS PRO AX DDR4",
};

static const gb_fusion2_device b760m_aorus_pro_ddr4_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "B760M AORUS PRO DDR4",
};

static const gb_fusion2_device z790_aorus_tachyon_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS TACHYON",
};

static const gb_fusion2_device z790_aorus_xtreme_device =
{
    &z790_xtrm_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Z790-XTRM-X    048D:5702                                     |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z790_xtrm_x_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_accent_led3_zone,
    &common_12v_led5_zone,
    &common_12v_led4_zone,
    &common_12v_led2_zone,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z790_aorus_xtreme_x_device =
{
    &z790_xtrm_x_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS XTREME X",
};

static const gb_fusion2_device z790_aorus_xtreme_x_ice_device =
{
    &z790_xtrm_x_layout,
    0xCDE4CFFE,
    1,
    "Z790 AORUS XTREME X ICE",
};

/*-------------------------------------------------------------*\
|  Z890-MSTR    048D:5711                                       |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Gen2 ARGB 4"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z890_mstr_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_d_led4_zone,
    &common_accent_led3_zone,
    &common_logo_led10_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device z890_aorus_mstr_device =
{
    &z890_mstr_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  Z890-MSTR-AI-TOP    048D:5711                                |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Gen2 ARGB 4"             : Linear                    |
|    Zone "Accent"                  : Single                    |
|    Zone "Logo"                    : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z890_mstr_ai_top_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_d_led4_zone,
    &common_accent_led11_zone,
    &common_logo_led10_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device z890_aorus_mstr_ai_top_device =
{
    &z890_mstr_ai_top_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS MASTER AI TOP",
};

/*-------------------------------------------------------------*\
|  Z890-ULTRA    048D:5711                                      |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Logo"                    : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z890_ultra_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_logo_led10_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z890i_aorus_ultra_device =
{
    &z890_ultra_layout,
    0xCDE4CFFE,
    1,
    "Z890I AORUS ULTRA",
};

/*-------------------------------------------------------------*\
|  Z890-XTRM-AI    048D:5711                                    |
|                                                               |
|    Zone "Gen2 ARGB 1"             : Linear                    |
|    Zone "Gen2 ARGB 2"             : Linear                    |
|    Zone "Gen2 ARGB 3"             : Linear                    |
|    Zone "Gen2 ARGB 4"             : Linear                    |
|    Zone "Logo"                    : Single                    |
|    Zone "Antenna Accent"          : Single                    |
|    Zone "12V RGB Strip"           : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout z890_xtrm_ai_layout =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_d_led3_zone,
    &common_d_led4_zone,
    &common_logo_led10_zone,
    &antenna_zone,
    &common_12v_led5_zone,
    nullptr,
};

static const gb_fusion2_device z890_aorus_xtrm_ai_top_device =
{
    &z890_xtrm_ai_layout,
    0xCDE4CFFE,
    1,
    "Z890 AORUS XTREME AI TOP",
};

/*-------------------------------------------------------------------------*\
|  DEVICE MASTER LIST                                                       |
\*-------------------------------------------------------------------------*/
const gb_fusion2_device* gb_fusion2_device_list_data[] =
{
/*-------------------------------------------------------------------------*\
|  Generic Layout (Used when no match found)                                |
\*-------------------------------------------------------------------------*/
    &generic_device,
/*-----------------------------------------------------------------*\
|  IT5702 Devices                                                   |
\*-----------------------------------------------------------------*/
    &a620i_ax_5702_device,
    &a620m_c_device,
    &a620m_ds3h_5702_device,
    &a620m_d2h_device,
    &a620m_gaming_x_5702_device,
    &a620m_gaming_x_ax_5702_device,
    &a620m_h_5702_device,
    &a620m_s2h_5702_device,
    &b550_aorus_elite_device,
    &b550_aorus_pro_device,
    &b550_aorus_elite_ax_v2_device,
    &b550i_aorus_pro_ax_device,
    &b650_aorus_elite_device,
    &b650_aorus_elite_ax_device,
    &b650_aorus_elite_ax_ice_device,
    &b650_aorus_elite_ax_v2_device,
    &b650_aorus_elite_v2_device,
    &b650_aorus_pro_ax_device,
    &b650_aero_g_device,
    &b650i_aorus_ultra_device,
    &b650i_ax_device,
    &b650_eagle_device,
    &b650_eagle_ax_device,
    &b650_gaming_x_device,
    &b650_gaming_x_ax_device,
    &b650_gaming_x_ax_v2_device,
    &b650_gaming_x_v2_device,
    &b650_ud_ac_device,
    &b650_ud_ax_device,
    &b650e_aorus_elite_x_ax_ice_device,
    &b650e_aorus_mstr_device,
    &b650e_aorus_pro_x_usb4_device,
    &b650e_aorus_tachyon_device,
    &b650m_aorus_elite_device,
    &b650m_aorus_elite_ax_device,
    &b650m_aorus_elite_ax_ice_device,
    &b650m_aorus_pro_device,
    &b650m_aorus_pro_ax_device,
    &b650m_c_v2_device,
    &b650m_c_v3_device,
    &b650m_d2h_device,
    &b650m_d2hp_device,
    &b650m_d3hp_device,
    &b650m_d3hp_ax_device,
    &b650m_ds3h_device,
    &b650m_gaming_plus_wifi_device,
    &b650m_gaming_wifi_device,
    &b650m_gaming_wifi6_device,
    &b650m_gaming_x_ax_device,
    &b650m_h_device,
    &b650m_k_device,
    &b650m_s2h_device,
    &b760_aorus_elite_device,
    &b760_aorus_elite_ax_device,
    &b760_aorus_master_ddr4_device,
    &b760_ds3h_device,
    &b760_ds3h_ac_device,
    &b760_ds3h_ac_ddr4_device,
    &b760_ds3h_ax_device,
    &b760_ds3h_ax_ddr4_device,
    &b760_ds3h_ax_v2_device,
    &b760_ds3h_ddr4_device,
    &b760i_aorus_pro_device,
    &b760i_aorus_pro_ddr4_device,
    &b760_gaming_x_device,
    &b760_gaming_x_ax_device,
    &b760_gaming_x_ax_ddr4_device,
    &b760_gaming_x_ddr4_device,
    &b760m_aorus_elite_device,
    &b760m_aorus_elite_ax_device,
    &b760m_aorus_elite_ax_ddr4_device,
    &b760m_aorus_elite_ddr4_device,
    &b760m_aorus_pro_device,
    &b760m_aorus_pro_ax_device,
    &b760m_aorus_pro_ax_ddr4_device,
    &b760m_aorus_pro_ddr4_device,
    &b760m_c_device,
    &b760m_c_v2_device,
    &b760m_d2h_device,
    &b760m_d2h_ddr4_device,
    &b760m_d3h_device,
    &b760m_d3h_ddr4_device,
    &b760m_d3hp_device,
    &b760m_d3hp_ddr4_device,
    &b760m_d3hp_wifi6_device,
    &b760m_ds3h_device,
    &b760m_ds3h_ax_device,
    &b760m_ds3h_ax_ddr4_device,
    &b760m_ds3h_ddr4_device,
    &b760m_ds3h_gen_5_device,
    &b760m_elite_x_ax_device,
    &b760m_g_ax_device,
    &b760m_gaming_device,
    &b760m_gaming_ac_device,
    &b760m_gaming_ac_ddr4_device,
    &b760m_gaming_ddr4_device,
    &b760m_gaming_plus_wifi_ddr4_device,
    &b760m_gaming_wifi_device,
    &b760m_gaming_wifi_plus_device,
    &b760m_gaming_x_device,
    &b760m_gaming_x_ax_device,
    &b760m_gaming_x_ax_ddr4_device,
    &b760m_gaming_x_ddr4_device,
    &b760m_power_device,
    &b760m_power_ddr4_device,
    &h610m_d3h_ddr4_device,
    &h610m_d3h_wifi_ddr4_device,
    &h610m_gaming_wifi_ddr4_device,
    &trx50_aero_d_device,
    &x570s_aero_g_device,
    &x570s_aorus_elite_device,
    &x570s_aorus_elite_ax_device,
    &x570s_aorus_mstr_device,
    &x570s_aorus_pro_ax_device,
    &x570s_gaming_x_device,
    &x570si_aorus_pro_ax_device,
    &x570s_ud_device,
    &x670_aorus_elite_ax_device,
    &x670_gaming_x_ax_device,
    &x670_gaming_x_ax_v2_device,
    &x670e_aorus_mstr_device,
    &x670_aorus_pro_x_device,
    &x670e_aorus_xtrm_device,
    &z690_aorus_elite_device,
    &z690_aorus_elite_ax_device,
    &z690_aorus_elite_ax_ddr4_device,
    &z690_aorus_elite_ddr4_device,
    &z790_aero_g_device,
    &z790_aorus_elite_device,
    &z790_aorus_elite_ax_device,
    &z790_aorus_elite_ax_ddr4_device,
    &z790_aorus_elite_ax_ice_device,
    &z790_aorus_elite_ax_w_device,
    &z790_aorus_elite_ddr4_device,
    &z790_aorus_elite_x_device,
    &z790_aorus_elite_x_ax_device,
    &z790_aorus_elite_x_wifi7_device,
    &z790_aorus_pro_x_device,
    &z790_aorus_pro_x_wifi7_device,
    &z790_aorus_mstr_device,
    &z790_aorus_master_x_device,
    &z790_aorus_tachyon_device,
    &z790_aorus_tachyon_x_device,
    &z790_aorus_xtreme_device,
    &z790_aorus_xtreme_x_device,
    &z790_aorus_xtreme_x_ice_device,
    &z790_d_device,
    &z790_d_ac_device,
    &z790_d_ax_device,
    &z790_d_wifi_device,
    &z790_eagle_device,
    &z790_eagle_ax_device,
    &z790_gaming_plus_ax_device,
    &z790_gaming_x_device,
    &z790_gaming_x_ax_device,
    &z790i_aorus_ultra_device,
    &z790m_aorus_elite_device,
    &z790m_aorus_elite_ax_device,
    &z790m_aorus_elite_ax_ice_device,
    &z790_s_ddr4_device,
    &z790_s_wifi_ddr4_device,
    &z790_ud_device,
    &z790_ud_ac_device,
    &z790_ud_ax_device,

/*-----------------------------------------------------------------*\
|  IT5711 Devices                                                   |
\*-----------------------------------------------------------------*/
    &a620i_ax_5711_device,
    &a620m_ds3h_5711_device,
    &a620m_gaming_x_5711_device,
    &a620m_gaming_x_ax_5711_device,
    &a620m_h_5711_device,
    &a620m_s2h_5711_device,
    &b650e_aorus_stealth_ice_device,
    &b760_ds3h_gen5_device,
    &b760_ds3h_wifi6e_gen5_device,
    &b760_gaming_x_ddr4_gen5_device,
    &b760_gaming_x_gen5_device,
    &b760_gaming_x_wifi6_gen5_device,
    &b760m_aorus_elite_ddr4_gen5_device,
    &b760m_aorus_elite_gen5_device,
    &b760m_aorus_elite_wifi6_ddr4_gen5_device,
    &b760m_aorus_elite_wifi6_gen5_device,
    &b760m_c_v3_device,
    &b760m_ds3h_ddr4_gen_5_device,
    &b760m_ds3h_wifi6e_ddr4_gen_5_device,
    &b760m_gaming_wifi6_plus_gen5_device,
    &b760m_gaming_wifi6e_gen_5_device,
    &b760m_gaming_x_ddr4_gen5_device,
    &b760m_gaming_x_gen5_device,
    &b760m_gaming_x_wifi6e_ddr4_gen5_device,
    &b760m_gaming_x_wifi6e_gen5_device,
    &b840_aorus_elite_wifi6e_device,
    &b840m_d2h_device,
    &b840m_ds3h_device,
    &b840m_eagle_wifi6_device,
    &b850_ai_top_device,
    &b850_aorus_elite_wifi7_device,
    &b850_aorus_elite_wifi7_ice_device,
    &b850_eagle_ice_device,
    &b850_eagle_wifi6e_device,
    &b850_eagle_wifi7_ice_device,
    &b850_gaming_wifi6_device,
    &b850_gaming_x_wifi6e_device,
    &b850i_aorus_pro_device,
    &b850m_aorus_elite_device,
    &b850m_aorus_elite_wifi6e_device,
    &b850m_aorus_elite_wifi6e_ice_device,
    &b850m_d3hp_device,
    &b850m_aorus_pro_wifi7_device,
    &b850m_ds3h_device,
    &b850m_ds3h_ice_device,
    &b850m_eagle_wifi6e_device,
    &b850m_eagle_wifi6e_ice_device,
    &b850m_force_device,
    &b850m_force_wifi6e_device,
    &b850m_gaming_x_wifi6e_device,
    &b860_aorus_elite_wifi7_ice_device,
    &b860i_aorus_pro_ice_device,
    &b860m_aorus_elite_device,
    &b860m_aorus_elite_wifi6e_device,
    &b860m_aorus_elite_wifi6e_ice_device,
    &b860m_aorus_pro_wifi7_device,
    &b860m_d2h_device,
    &b860m_eagle_device,
    &b860m_eagle_ds3h_device,
    &b860m_eagle_ds3h_wifi6e_device,
    &b860m_eagle_plus_wifi6e_device,
    &b860m_eagle_wifi6_device,
    &b860m_eagle_wifi6_v2_device,
    &b860m_gaming_x_device,
    &b860m_gaming_x_wifi6e_device,
    &b860m_gaming_wifi6_device,
    &b860m_power_device,
    &trx50_ai_top_device,
    &w790_ai_top_device,
    &w880_ai_top_device,
    &x870_aorus_elite_wifi7_device,
    &x870_aorus_elite_wifi7_ice_device,
    &x870_eagle_wifi7_device,
    &x870e_aorus_elite_wifi7_device,
    &x870e_aorus_elite_wifi7_ice_device,
    &x870e_aorus_master_device,
    &x870e_aorus_pro_device,
    &x870e_aorus_pro_ice_device,
    &x870e_aorus_xtrm_ai_device,
    &x870_gaming_wifi6_device,
    &x870_gaming_x_wifi_device,
    &x870i_aorus_pro_device,
    &x870i_aorus_pro_ice_device,
    &z890_aorus_elite_wifi7_device,
    &z890_aorus_elite_wifi7_ice_device,
    &z890_aorus_elite_x_ice_device,
    &z890_aorus_mstr_device,
    &z890_aorus_mstr_ai_top_device,
    &z890_aorus_pro_ice_device,
    &z890_aero_d_device,
    &z890_aero_g_device,
    &z890_eagle_device,
    &z890_eagle_wifi7_device,
    &z890_ai_top_device,
    &z890_aorus_tachyon_ice_device,
    &z890_aorus_xtrm_ai_top_device,
    &z890_ud_wifi6e_device,
    &z890_gaming_x_wifi7_device,
    &z890i_aorus_ultra_device,
    &z890m_aorus_elite_wifi7_device,
    &z890m_aorus_elite_wifi7_ice_device,
    &z890m_gaming_x_device,

/*-----------------------------------------------------------------*\
|  IT8297 Devices                                                   |
\*-----------------------------------------------------------------*/
    &x570_aorus_elite_device,
    &x570_aorus_elite_wifi_device,
    &x570_aorus_master_device,
    &x570_aorus_pro_device,
    &x570_aorus_pro_wifi_device,
    &x570_aorus_ultra_device,
    &x570i_aorus_pro_wifi_device,
    &z390_aorus_master_cf_device,

/*-----------------------------------------------------------------*\
|  IT8950 Devices                                                   |
\*-----------------------------------------------------------------*/
    &h810m_gaming_wifi6_device,
    &h810m_h_device,
    &h810m_s2h_device,
};

const unsigned int GB_FUSION2_DEVICE_COUNT = (sizeof(gb_fusion2_device_list_data) / sizeof(gb_fusion2_device_list_data[ 0 ]));
const gb_fusion2_device** gb_fusion2_device_list = gb_fusion2_device_list_data;


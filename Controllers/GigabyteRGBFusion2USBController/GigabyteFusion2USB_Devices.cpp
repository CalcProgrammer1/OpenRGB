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
static const gb_fusion2_zone common_d_led_zone =
{
    HDR_D_LED1,
    0,
    1024,
    "D_LED"
};

static const gb_fusion2_zone common_d_led1_zone =
{
    HDR_D_LED1,
    0,
    1024,
    "D_LED1"
};

static const gb_fusion2_zone common_dled1_dled2_zone =
{
    HDR_D_LED1,
    0,
    1024,
    "D_LED1/D_LED2"
};

static const gb_fusion2_zone common_argb_v2_1_zone =
{
    HDR_D_LED1,
    0,
    1024,
    "ARGB_V2_1"
};

static const gb_fusion2_zone alt_argb_v2_1_zone =
{
    LED3,
    0,
    1024,
    "ARGB_V2_1"
};

static const gb_fusion2_zone common_argb_v2_1_3_zone =
{
    HDR_D_LED1,
    0,
    1024,
    "ARGB_V2_1/ARGB_V2_3"
};

static const gb_fusion2_zone common_d_led2_zone =
{
    HDR_D_LED2,
    0,
    1024,
    "D_LED2"
};

static const gb_fusion2_zone common_d_led2_aux_zone =
{
    HDR_D_LED2,
    2,
    1024,
    "D_LED2 + Aux"
};

static const gb_fusion2_zone common_argb_v2_2_zone =
{
    HDR_D_LED2,
    0,
    1024,
    "ARGB_V2_2"
};

static const gb_fusion2_zone alt_argb_v2_2_zone =
{
    LED4,
    0,
    1024,
    "ARGB_V2_2"
};

static const gb_fusion2_zone common_argb_v2_3_1_zone =
{
    HDR_D_LED1,
    0,
    1024,
    "ARGB_V2_3"
};

static const gb_fusion2_zone common_argb_v2_3_zone =
{
    HDR_D_LED3,
    0,
    1024,
    "ARGB_V2_3"
};

static const gb_fusion2_zone common_argb_v2_4_zone =
{
    HDR_D_LED4,
    0,
    1024,
    "ARGB_V2_4"
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

static const gb_fusion2_zone common_led8_zone =
{
    LED8,
    1,
    1,
    "Name for Led 8"
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

static const gb_fusion2_zone common_amp_up_logo_4_zone =
{
    LED4,
    1,
    1,
    "AMP UP Logo"
};

static const gb_fusion2_zone common_aor_logo_3_zone =
{
    LED3,
    1,
    1,
    "Aorus Logo"
};

static const gb_fusion2_zone common_brs_3_zone =
{
    LED3,
    1,
    1,
    "Board Accent (Right Side)"
};

static const gb_fusion2_zone common_brs_4_zone =
{
    LED4,
    1,
    1,
    "Board Accent (Right Side)"
};

static const gb_fusion2_zone common_brs_8_zone =
{
    LED8,
    1,
    1,
    "Board Accent (Right Side)"
};

static const gb_fusion2_zone common_brst_1_zone =
{
    LED1,
    1,
    1,
    "Board Accent (Right Side, Top)"
};

static const gb_fusion2_zone common_brst_3_zone =
{
    LED3,
    1,
    1,
    "Board Accent (Right Side, Top)"
};

static const gb_fusion2_zone common_brstm_2_zone =
{
    LED2,
    1,
    1,
    "Board Accent (Right Side, Top Middle)"
};

static const gb_fusion2_zone common_brstm_4_zone =
{
    LED4,
    1,
    1,
    "Board Accent (Right Side, Top Middle)"
};

static const gb_fusion2_zone common_brsbm_3_zone =
{
    LED3,
    1,
    1,
    "Board Accent (Right Side, Bottom Middle)"
};

static const gb_fusion2_zone common_brsb_4_zone =
{
    LED4,
    1,
    1,
    "Board Accent (Right Side, Bottom)"
};

static const gb_fusion2_zone common_chip_acc_3_zone =
{
    LED3,
    1,
    1,
    "Chipset Accent"
};

static const gb_fusion2_zone common_chip_acc_4_zone =
{
    LED4,
    1,
    1,
    "Chipset Accent"
};

static const gb_fusion2_zone common_chip_acc_6_zone =
{
    LED6,
    1,
    1,
    "Chipset Accent"
};

static const gb_fusion2_zone common_chip_acc_7_zone =
{
    LED7,
    1,
    1,
    "Chipset Accent"
};

static const gb_fusion2_zone common_chip_acc_11_zone =
{
    LED11,
    1,
    1,
    "Chipset Accent"
};

static const gb_fusion2_zone common_led_c_2_zone =
{
    LED2,
    1,
    1,
    "LED_C"
};

static const gb_fusion2_zone common_led_c1_2_zone =
{
    LED2,
    1,
    1,
    "LED_C1"
};

static const gb_fusion2_zone common_led_c3_4_zone =
{
    LED4,
    1,
    1,
    "LED_C3"
};

static const gb_fusion2_zone common_led_c_5_zone =
{
    LED5,
    1,
    1,
    "LED_C"
};

static const gb_fusion2_zone common_led_c2_5_zone =
{
    LED5,
    1,
    1,
    "LED_C2"
};

static const gb_fusion2_zone common_led_c1_c2_5_zone =
{
    LED5,
    1,
    1,
    "LED_C1/LED_C2"
};

static const gb_fusion2_zone common_led_cpu_2_zone =
{
    LED2,
    1,
    1,
    "LED_CPU"
};

static const gb_fusion2_zone common_led_cpu_3_zone =
{
    LED3,
    1,
    1,
    "LED_CPU"
};

static const gb_fusion2_zone common_led_cpu_8_zone =
{
    LED8,
    1,
    1,
    "LED_CPU"
};

static const gb_fusion2_zone common_ess_logo_4_zone =
{
    LED4,
    1,
    1,
    "ESS Logo"
};

static const gb_fusion2_zone common_game_on_1_zone =
{
    LED1,
    1,
    1,
    "Game On LED"
};

static const gb_fusion2_zone common_io_cov_1_zone =
{
    LED1,
    1,
    1,
    "I/O Cover"
};

static const gb_fusion2_zone common_io_cov_7_zone =
{
    LED7,
    6,
    6,
    "I/O Cover"
};

static const gb_fusion2_zone common_io_cov_10_zone =
{
    LED10,
    1,
    1,
    "I/O Cover"
};

static const gb_fusion2_zone common_io_cov_btm_1_zone =
{
    LED1,
    1,
    1,
    "I/O Cover (Bottom)"
};

static const gb_fusion2_zone common_io_cov_btm_4_zone =
{
    LED4,
    1,
    1,
    "I/O Cover (Bottom)"
};

static const gb_fusion2_zone common_io_cov_btm_mid_2_zone =
{
    LED2,
    1,
    1,
    "I/O Cover (Bottom Middle)"
};

static const gb_fusion2_zone common_chip_io_cov_tm_3_zone =
{
    LED3,
    1,
    1,
    "I/O Cover (Top Middle)"
};

static const gb_fusion2_zone common_io_cov_top_1_zone =
{
    LED1,
    1,
    1,
    "I/O Cover (Top)"
};

static const gb_fusion2_zone common_io_cov_top_4_zone =
{
    LED4,
    1,
    1,
    "I/O Cover (Top)"
};

static const gb_fusion2_zone common_pcie_acc_1_zone =
{
    LED1,
    1,
    1,
    "PCI-E Accent"
};

static const gb_fusion2_zone common_pcie_acc_3_zone =
{
    LED3,
    1,
    1,
    "PCI-E Accent"
};

static const gb_fusion2_zone common_pcie_acc_4_zone =
{
    LED4,
    1,
    1,
    "PCI-E Accent"
};

static const gb_fusion2_zone common_ram_accent_2_zone =
{
    LED2,
    1,
    1,
    "RAM Accent"
};

static const gb_fusion2_zone common_ram_cov_4_zone =
{
    LED4,
    1,
    1,
    "RAM Cover"
};

static const gb_fusion2_zone common_ram_cov_7_zone =
{
    LED7,
    1,
    1,
    "RAM Cover"
};

static const gb_fusion2_zone common_ram_cov_8_zone =
{
    LED8,
    1,
    1,
    "RAM Cover"
};

static const gb_fusion2_zone common_ssd_cov_7_zone =
{
    LED7,
    1,
    1,
    "SSD Cover"
};

static const gb_fusion2_zone common_ssd_cov_8_zone =
{
    LED8,
    1,
    1,
    "SSD Cover"
};

static const gb_fusion2_zone common_wifi_ant_9_zone =
{
    LED9,
    1,
    1,
    "WIFI Antenna"
};

static const gb_fusion2_zone common_wifi_ant_11_zone =
{
    LED11,
    1,
    1,
    "WIFI Antenna"
};

static const gb_fusion2_zone common_xmp_logo_2_zone =
{
    LED2,
    1,
    1,
    "XMP Logo"
};

static const gb_fusion2_zone common_xtrm_logo_3_zone =
{
    LED3,
    1,
    1,
    "XTREME Logo"
};

/*-------------------------------------------------------------------------*\
|  GB Fusion2 Layouts                                                       |
\*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------*\
|  Generic    048D:8297/048D:5702                               |
|                                                               |
|    Zone "D_LED1"                  : Linear                    |
|    Zone "D_LED2"                  : Linear                    |
|    Zone "Name for Led 1"          : Single                    |
|    Zone "Name for LED 2"          : Single                    |
|    Zone "Name for LED 3"          : Single                    |
|    Zone "Name for LED 4"          : Single                    |
|    Zone "Name for LED 5"          : Single                    |
|    Zone "Name for LED 8"          : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led1_zone,
    &common_led2_zone,
    &common_led3_zone,
    &common_led4_zone,
    &common_led5_zone,
    &common_led8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device generic_it8297_device =
{
    &it8297_device,
    0x0000005F,
    1,
    "GENERIC IT8297/IT5702 LAYOUT",
};


/*-------------------------------------------------------------*\
|  Generic    048D:8950                                         |
|                                                               |
|    Zone "ARGB_V2_1"               : Linear                    |
|    Zone "ARGB_V2_2"               : Linear                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8950_device =
{
    &alt_argb_v2_1_zone,
    &alt_argb_v2_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device generic_it8950_device =
{
    &it8950_device,
    0x0000005F,
    0,
    "GENERIC IT82950 LAYOUT",
};

/*-------------------------------------------------------------*\
|  Generic    048D:5711                                         |
|                                                               |
|    Zone "ARGB_V2_1"               : Linear                    |
|    Zone "ARGB_V2_2"               : Linear                    |
|    Zone "ARGB_V2_3"               : Linear                    |
|    Zone "ARGB_V2_4"               : Linear                    |
|    Zone "Name for Led 1"          : Single                    |
|    Zone "Name for LED 2"          : Single                    |
|    Zone "Name for LED 3"          : Single                    |
|    Zone "Name for LED 4"          : Single                    |
|    Zone "Name for LED 5"          : Single                    |
|    Zone "Name for LED 9"          : Single                    |
|    Zone "Name for LED 10"         : Single                    |
|    Zone "Name for LED 11"         : Single                    |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_argb_v2_4_zone,
    &common_led1_zone,
    &common_led2_zone,
    &common_led3_zone,
    &common_led4_zone,
    &common_led5_zone,
    &common_led9_zone,
    &common_led10_zone,
    &common_led11_zone,
};

static const gb_fusion2_device generic_it5711_device =
{
    &it5711_device,
    0x000001DF,
    0,
    "GENERIC IT5711 LAYOUT",
};

/*-------------------------------------------------------------*\
|  Layout 1    048D:8950 (IT82950)                              |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8950_1_device =
{
    &alt_argb_v2_1_zone,
    &alt_argb_v2_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};


static const gb_fusion2_device h810m_gmg_wifi6_device =
{
    &it8950_1_device,
    0x1810004D,
    0,
    "H810M GAMING WIFI6",
};

static const gb_fusion2_device h810m_h_device =
{
    &it8950_1_device,
    0x1810004D,
    0,
    "H810M H",
};

static const gb_fusion2_device h810m_s2h_device =
{
    &it8950_1_device,
    0x1810004D,
    0,
    "H810M S2H",
};

/*-------------------------------------------------------------*\
|   Hybrid device with IT82950 and Super I/O                    |
\*-------------------------------------------------------------*/
static const gb_fusion2_device b860m_d_device =
{
    &it8950_1_device,
    0x08F1004D,
    0,
    "B860M D",
};

static const gb_fusion2_device b860m_d3hp_device =
{
    &it8950_1_device,
    0x08E1004D,
    0,
    "B860M D3HP",
};

static const gb_fusion2_device b860m_e_device =
{
    &it8950_1_device,
    0x08F1004D,
    0,
    "B860M E",
};

static const gb_fusion2_device b860m_h_device =
{
    &it8950_1_device,
    0x08F1004D,
    0,
    "B860M H",
};

static const gb_fusion2_device b860m_k_device =
{
    &it8950_1_device,
    0x08F1004D,
    0,
    "B860M K",
};

static const gb_fusion2_device z890m_gmg_x_device =
{
    &it8950_1_device,
    0x00F1004D,
    0,
    "Z890M GAMING X",
};

/*-------------------------------------------------------------*\
|  Layout 1    048D:8297                                        |
|                                                               |
|    Zone "D_LED1/D_LED2"                            : Linear   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_1_device =
{
    &common_dled1_dled2_zone,
    &common_led_cpu_2_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b450_gmg_x_device =
{
    &it8297_1_device,
    0x1110005F,
    0,
    "B450 GAMING X",
};

static const gb_fusion2_device b450m_ds3h_wifi_8297_device =
{
    &it8297_1_device,
    0x1010005F,
    0,
    "B450M DS3H WIFI",
};

/*-------------------------------------------------------------*\
|  Layout 2    048D:8297                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "Board Accent (Right Side)"                : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_2_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_brs_3_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z390_i_aor_pro_wifi_device =
{
    &it8297_2_device,
    0x0370005F,
    0,
    "Z390 I AORUS PRO WIFI",
};

/*-------------------------------------------------------------*\
|  Layout 3    048D:8297                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_3_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_cpu_3_zone,
    &common_led_c1_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x570_gaming_x_device =
{
    &it8297_3_device,
    0x0110005F,
    0,
    "X570 GAMING X",
};

/*-------------------------------------------------------------*\
|  Layout 4    048D:8297                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "I/O Cover"                                : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_4_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device trx40_aor_master_device =
{
    &it8297_4_device,
    0x0210005F,
    0,
    "TRX40 AORUS MASTER",
};

static const gb_fusion2_device trx40_aor_designare_device =
{
    &it8297_4_device,
    0x0210005F,
    0,
    "TRX40 DESIGNARE",
};

/*-------------------------------------------------------------*\
|  Layout 5    048D:8297                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "I/O Cover"                                : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_5_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_pcie_acc_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device trx40_aor_pro_wifi_device =
{
    &it8297_5_device,
    0x0120005F,
    0,
    "TRX40 AORUS PRO WIFI",
};

/*-------------------------------------------------------------*\
|  Layout 6    048D:8297                                        |
|                                                               |
|    Zone "D_LED1/D_LED2"                            : Linear   |
|    Zone "IO Cover"                                 : Linear   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_6_device =
{
    &common_dled1_dled2_zone,
    &common_io_cov_7_zone,
    &common_pcie_acc_4_zone,
    &common_led_c1_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x570_aor_mstr_device =
{
    &it8297_6_device,
    0x0120005F,
    0,
    "X570 AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  Layout 7    048D:8297                                        |
|                                                               |
|    Zone "D_LED1/D_LED2"                            : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "XMP Logo"                                 : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "AMP Up Logo"                              : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_7_device =
{
    &common_dled1_dled2_zone,
    &common_io_cov_1_zone,
    &common_xmp_logo_2_zone,
    &common_chip_acc_3_zone,
    &common_amp_up_logo_4_zone,
    &common_led_c1_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z390_aor_ultra_device =
{
    &it8297_7_device,
    0x0190005F,
    0,
    "Z390 AORUS ULTRA",
};

/*-------------------------------------------------------------*\
|  Layout 8    048D:8297                                        |
|                                                               |
|    Zone "D_LED1/D_LED2"                            : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "RAM Accent"                               : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_8_device =
{
    &common_dled1_dled2_zone,
    &common_io_cov_1_zone,
    &common_ram_accent_2_zone,
    &common_chip_acc_3_zone,
    &common_pcie_acc_4_zone,
    &common_led_c1_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z390_aor_pro_device =
{
    &it8297_8_device,
    0x0180005F,
    0,
    "Z390 AORUS PRO",
};

static const gb_fusion2_device z390_aor_pro_wifi_device =
{
    &it8297_8_device,
    0x0180005F,
    0,
    "Z390 AORUS PRO WIFI",
};

/*-------------------------------------------------------------*\
|  Layout 9    048D:8297                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_9_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_cpu_2_zone,
    &common_pcie_acc_4_zone,
    &common_led_c1_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x570_aor_elite_device =
{
    &it8297_9_device,
    0x0130005F,
    0,
    "X570 AORUS ELITE",
};

static const gb_fusion2_device x570_aor_elite_wifi_device =
{
    &it8297_9_device,
    0x0130005F,
    0,
    "X570 AORUS ELITE WIFI",
};

static const gb_fusion2_device x570_aor_pro_device =
{
    &it8297_9_device,
    0x0130005F,
    0,
    "X570 AORUS PRO",
};

static const gb_fusion2_device x570_aor_pro_wifi_device =
{
    &it8297_9_device,
    0x0130005F,
    0,
    "X570 AORUS PRO WIFI",
};

static const gb_fusion2_device x570_aor_ultra_device =
{
    &it8297_9_device,
    0x0140005F,
    0,
    "X570 AORUS ULTRA",
};

/*-------------------------------------------------------------*\
|  Layout 10   048D:8297                                        |
|                                                               |
|    Zone "D_LED1/D_LED2"                            : Linear   |
|    Zone "IO Cover"                                 : Linear   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_10_device =
{
    &common_dled1_dled2_zone,
    &common_io_cov_7_zone,
    &common_pcie_acc_3_zone,
    &common_chip_acc_4_zone,
    &common_led_c1_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x570_aor_xtrm_device =
{
    &it8297_10_device,
    0x016001DF,
    0,
    "X570 AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 11   048D:8297                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "Board Accent (Right Side, Top)"           : Single   |
|    Zone "Board Accent (Right Side, Top Middle)"    : Single   |
|    Zone "Board Accent (Right Side, Bottom Middle)" : Single   |
|    Zone "Board Accent (Right Side, Bottom)"        : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_11_device =
{
    &common_d_led_zone,
    &common_brst_1_zone,
    &common_brstm_2_zone,
    &common_brsbm_3_zone,
    &common_brsb_4_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x570_i_aor_pro_wifi_device =
{
    &it8297_11_device,
    0x0350007F,
    0,
    "X570 I AORUS PRO WIFI",
};

/*-------------------------------------------------------------*\
|  Layout 12   048D:8297                                        |
|                                                               |
|    Zone "D_LED1/D_LED2"                            : Linear   |
|    Zone "IO Cover"                                 : Linear   |
|    Zone "XMP Logo"                                 : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_12_device =
{
    &common_dled1_dled2_zone,
    &common_io_cov_7_zone,
    &common_chip_acc_3_zone,
    &common_pcie_acc_4_zone,
    &common_led_c1_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z390_aor_mstr_device =
{
    &it8297_12_device,
    0x01A0005F,
    0,
    "Z390 AORUS MASTER",
};

static const gb_fusion2_device z390_aor_mstr_g2_device =
{
    &it8297_12_device,
    0x01A0005F,
    0,
    "Z390 AORUS MASTER G2 EDITION",
};

/*-------------------------------------------------------------*\
|  Layout 13   048D:8297                                        |
|                                                               |
|    Zone "D_LED1/D_LED2"                            : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "XMP Logo"                                 : Single   |
|    Zone "XTREME Logo"                              : Single   |
|    Zone "ESS Logo"                                 : Single   |
|    Zone "LED_C1/LED_C2"                            : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "Board Accent (Right Side)"                : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_13_device =
{
    &common_dled1_dled2_zone,
    &common_xmp_logo_2_zone,
    &common_xtrm_logo_3_zone,
    &common_ess_logo_4_zone,
    &common_led_c1_c2_5_zone,
    &common_chip_acc_7_zone,
    &common_brs_8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z390_aor_xtrm_device =
{
    &it8297_13_device,
    0x01B0005F,
    0,
    "Z390 AORUS XTREME",
};

static const gb_fusion2_device z390_aor_xtrm_wtr_force_device =
{
    &it8297_13_device,
    0x01B0005F,
    0,
    "Z390 AORUS XTREME WATERFORCE",
};

static const gb_fusion2_device z390_aor_xtrm_wtr_force_5g_device =
{
    &it8297_13_device,
    0x01B0005F,
    0,
    "Z390 AORUS XTREME WATERFORCE 5G",
};

/*-------------------------------------------------------------*\
|  Layout 14   048D:8297                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2 + AUX"                             : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_14_device =
{
    &common_d_led1_zone,
    &common_d_led2_aux_zone,
    &common_led_c1_2_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device trx40_aor_xtrm_device =
{
    &it8297_14_device,
    0x023001DF,
    0,
    "TRX40 AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 15   048D:8297                                        |
|                                                               |
|    Zone "Aorus Logo"                               : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "Board Accent (Right Side)"                : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it8297_15_device =
{
    &common_aor_logo_3_zone,
    &common_chip_acc_4_zone,
    &common_brs_8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device trx40_aor_xtrm_2_device =
{
    &it8297_15_device,
    0x023001DF,
    1,
    "TRX40 AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 16   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_16_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device h610m_d3h_ddr4_device =
{
    &it5702_16_device,
    0x1810005F,
    0,
    "H610M D3H DDR4",
};

static const gb_fusion2_device h610m_d3h_wifi_ddr4_device =
{
    &it5702_16_device,
    0x1810005F,
    0,
    "H610M D3H WIFI DDR4",
};

static const gb_fusion2_device h610m_d3w_device =
{
    &it5702_16_device,
    0x1810005F,
    0,
    "H610M D3W",
};

static const gb_fusion2_device h610m_d3w_wifi6_device =
{
    &it5702_16_device,
    0x1810005F,
    0,
    "H610M D3W WIFI6",
};

static const gb_fusion2_device h610m_gmg_wifi_ddr4_device =
{
    &it5702_16_device,
    0x1810005F,
    0,
    "H610M GAMING WIFI DDR4",
};

/*-------------------------------------------------------------*\
|  Layout 17   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_17_device =
{
    &common_argb_v2_3_1_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650_ud_ac_device =
{
    &it5702_17_device,
    0x2120005F,
    0,
    "B650 UD AC",
};

static const gb_fusion2_device b650_ud_ax_device =
{
    &it5702_17_device,
    0x2120005F,
    0,
    "B650 UD AX",
};

/*-------------------------------------------------------------*\
|  Layout 18   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_18_device =
{
    &common_d_led_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650i_aor_ultra_device =
{
    &it5702_18_device,
    0x0B30005F,
    0,
    "B650I AORUS ULTRA",
};

/*-------------------------------------------------------------*\
|  Layout 19   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_19_device =
{
    &common_d_led_zone,
    &common_led_c_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device a620i_ax_device =
{
    &it5702_19_device,
    0x1330005F,
    0,
    "A620I AX",
};

static const gb_fusion2_device a620m_c_device =
{
    &it5702_19_device,
    0x1030005F,
    0,
    "A620M C",
};

static const gb_fusion2_device a620m_ds3h_device =
{
    &it5702_19_device,
    0x1030005F,
    0,
    "A620M DS3H",
};

static const gb_fusion2_device a620m_gmg_x_device =
{
    &it5702_19_device,
    0x1030005F,
    0,
    "A620M GAMING X",
};

static const gb_fusion2_device a620m_gmg_x_ax_device =
{
    &it5702_19_device,
    0x1030005F,
    0,
    "A620M GAMING X AX",
};

static const gb_fusion2_device a620m_h_device =
{
    &it5702_19_device,
    0x1030005F,
    0,
    "A620M H",
};

static const gb_fusion2_device a620m_s2h_device =
{
    &it5702_19_device,
    0x1030005F,
    0,
    "A620M S2H",
};

static const gb_fusion2_device b650i_ax_device =
{
    &it5702_19_device,
    0x0B20005F,
    0,
    "B650I AX",
};

static const gb_fusion2_device b650m_c_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M C",
};

static const gb_fusion2_device b650m_c_v2_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M C V2",
};

static const gb_fusion2_device b650m_c_v3_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M C V3",
};

static const gb_fusion2_device b650m_d2h_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M D2H",
};

static const gb_fusion2_device b650m_d2hp_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M D2HP",
};

static const gb_fusion2_device b650m_ds3h_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M DS3H",
};

static const gb_fusion2_device b650m_h_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M H",
};

static const gb_fusion2_device b650m_k_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M K",
};

static const gb_fusion2_device b650m_s2h_device =
{
    &it5702_19_device,
    0x0820005F,
    0,
    "B650M S2H",
};

static const gb_fusion2_device b760m_pwr_device =
{
    &it5702_19_device,
    0x0810005F,
    0,
    "B760M POWER",
};

static const gb_fusion2_device b760m_pwr_ddr4_device =
{
    &it5702_19_device,
    0x0810005F,
    0,
    "B760M POWER DDR4",
};

static const gb_fusion2_device a520i_ac_device =
{
    &it5702_19_device,
    0x0040005F,
    0,
    "A520I AC",
};

static const gb_fusion2_device a520m_ds3h_device =
{
    &it5702_19_device,
    0x0040005F,
    0,
    "A520M DS3H",
};

static const gb_fusion2_device a520m_ds3h_ac_device =
{
    &it5702_19_device,
    0x0040005F,
    0,
    "A520M DS3H AC",
};

static const gb_fusion2_device a520m_h_device =
{
    &it5702_19_device,
    0x0040005F,
    0,
    "A520M H",
};

static const gb_fusion2_device a520m_s2h_device =
{
    &it5702_19_device,
    0x0040005F,
    0,
    "A520M S2H",
};

static const gb_fusion2_device b550m_gmg_device =
{
    &it5702_19_device,
    0x0020005F,
    0,
    "B550M GAMING",
};

static const gb_fusion2_device b550m_h_device =
{
    &it5702_19_device,
    0x0020005F,
    0,
    "B550M H",
};

static const gb_fusion2_device b550m_s2h_device =
{
    &it5702_19_device,
    0x0020005F,
    0,
    "B550M S2H",
};

static const gb_fusion2_device z590i_vis_d_device =
{
    &it5702_19_device,
    0x0310005F,
    0,
    "Z590I VISION D",
};

/*-------------------------------------------------------------*\
|  Layout 20   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_20_device =
{
    &common_d_led1_zone,
    &common_led_c1_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z590_d_device =
{
    &it5702_20_device,
    0x0110005F,
    0,
    "Z590 D",
};

static const gb_fusion2_device z490m_device =
{
    &it5702_20_device,
    0x00A0005F,
    0,
    "Z490M",
};

static const gb_fusion2_device h490m_ds3h_device =
{
    &it5702_20_device,
    0x0870005F,
    0,
    "H470M DS3H",
};

static const gb_fusion2_device b460m_ds3h_v2_device =
{
    &it5702_20_device,
    0x0870005F,
    0,
    "B460M DS3H V2",
};

/*-------------------------------------------------------------*\
|  Layout 21   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_21_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_h_v2_device =
{
    &it5702_21_device,
    0x1830005F,
    0,
    "B760M H V2",
};

static const gb_fusion2_device z790_d_device =
{
    &it5702_21_device,
    0x0180005F,
    0,
    "Z790 D",
};

static const gb_fusion2_device z790_d_ac_device =
{
    &it5702_21_device,
    0x0180005F,
    0,
    "Z790 D AC",
};

static const gb_fusion2_device z790_d_ax_device =
{
    &it5702_21_device,
    0x0180005F,
    0,
    "Z790 D AX",
};

static const gb_fusion2_device z790_d_wifi_device =
{
    &it5702_21_device,
    0x0180005F,
    0,
    "Z790 D WIFI",
};

static const gb_fusion2_device z790_eagle_device =
{
    &it5702_21_device,
    0x0180005F,
    0,
    "Z790 EAGLE",
};

static const gb_fusion2_device z790_eagle_ax_device =
{
    &it5702_21_device,
    0x0180005F,
    0,
    "Z790 EAGLE AX",
};

/*-------------------------------------------------------------*\
|  Layout 22   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_1/ARGB_V2_3"                      : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_22_device =
{
    &common_argb_v2_1_3_zone,
    &common_argb_v2_2_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650_eagle_device =
{
    &it5702_22_device,
    0x2130005F,
    0,
    "B650 EAGLE",
};

static const gb_fusion2_device b650_eagle_ax_device =
{
    &it5702_22_device,
    0x2130005F,
    0,
    "B650 EAGLE AX",
};

static const gb_fusion2_device trx50_aero_d_device =
{
    &it5702_22_device,
    0x2A10005F,
    0,
    "TRX50 AERO D",
};

static const gb_fusion2_device x670_gmg_x_ax_v2_device =
{
    &it5702_22_device,
    0x1910005F,
    0,
    "X670 GAMING X AX V2",
};

static const gb_fusion2_device z790_aor_mstr_x_device =
{
    &it5702_22_device,
    0x123001DF,
    0,
    "Z790 AORUS MASTER X",
};

/*-------------------------------------------------------------*\
|  Layout 23   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "Board Accent(Right Side)"                 : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_23_device =
{
    &common_d_led_zone,
    &common_brs_3_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490i_aor_ultra_device =
{
    &it5702_23_device,
    0x0350007F,
    0,
    "Z490I AORUS ULTRA",
};

static const gb_fusion2_device h490i_aor_pro_ax_device =
{
    &it5702_23_device,
    0x0B50007F,
    0,
    "H470I AORUS PRO AX",
};

/*-------------------------------------------------------------*\
|  Layout 24   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_24_device =
{
    &common_d_led_zone,
    &common_led_cpu_3_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650e_tachyon_device =
{
    &it5702_24_device,
    0x0950005F,
    0,
    "B650E TACHYON",
};

/*-------------------------------------------------------------*\
|  Layout 25   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "Chipset Accent"                           : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_25_device =
{
    &common_d_led_zone,
    &common_led_c_2_zone,
    &common_chip_acc_4_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650m_gmg_x_ax_device =
{
    &it5702_25_device,
    0x0840015F,
    0,
    "B650M GAMING X AX",
};

/*-------------------------------------------------------------*\
|  Layout 26   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "PCI-E Accent"                             : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_26_device =
{
    &common_d_led_zone,
    &common_led_c_2_zone,
    &common_pcie_acc_4_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650m_d2h_ddr4_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M D2H DDR4",
};

static const gb_fusion2_device b650m_d3h_ddr4_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M D3H DDR4",
};

static const gb_fusion2_device b650m_ds3h_ax_ddr4_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M DS3H AX DDR4",
};

static const gb_fusion2_device b650m_ds3h_ddr4_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M DS3H DDR4",
};

static const gb_fusion2_device b650m_gmg_ac_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M GAMING AC",
};

static const gb_fusion2_device b650m_gmg_ac_ddr4_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M GAMING AC DDR4",
};

static const gb_fusion2_device b650m_gmg_ddr4_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M GAMING DDR4",
};

static const gb_fusion2_device b650m_pwr_ddr4_device =
{
    &it5702_26_device,
    0x0810005F,
    0,
    "B660M POWER DDR4",
};

static const gb_fusion2_device z690m_ds3h_ddr4_device =
{
    &it5702_26_device,
    0x00C0005F,
    0,
    "Z690M DS3H DDR4",
};

static const gb_fusion2_device b560_hd3_device =
{
    &it5702_26_device,
    0x1120005F,
    0,
    "B560 HD3",
};

static const gb_fusion2_device b560m_d2v_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M D2V",
};

static const gb_fusion2_device b560m_d3h_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M D3H",
};

static const gb_fusion2_device b560m_ds3h_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M DS3H",
};

static const gb_fusion2_device b560m_ds3h_ac_device =
{
    &it5702_26_device,
    0x1060005F,
    0,
    "B560M DS3H AC",
};

static const gb_fusion2_device b560m_ds3h_plus_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M DS3H PLUS",
};

static const gb_fusion2_device b560m_ds3h_v2_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M DS3H V2",
};

static const gb_fusion2_device b560m_gmg_hd_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M GAMING HD",
};

static const gb_fusion2_device b560m_h_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M H",
};

static const gb_fusion2_device b560m_pwr_device =
{
    &it5702_26_device,
    0x1020005F,
    0,
    "B560M POWER",
};

/*-------------------------------------------------------------*\
|  Layout 27   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "PCI-E Accent"                             : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_27_device =
{
    &common_d_led_zone,
    &common_led_c1_2_zone,
    &common_pcie_acc_4_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_d2h_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M D2H",
};

static const gb_fusion2_device b760m_d2h_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M D2H DDR4",
};

static const gb_fusion2_device b760m_d3h_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M D3H",
};

static const gb_fusion2_device b760m_d3h_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M D3H DDR4",
};

static const gb_fusion2_device b760m_d3hp_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M D3HP",
};

static const gb_fusion2_device b760m_d3hp_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M D3HP DDR4",
};

static const gb_fusion2_device b760m_d3hp_wifi6_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M D3HP WIFI6",
};

static const gb_fusion2_device b760m_ds3h_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M DS3H",
};

static const gb_fusion2_device b760m_ds3h_ax_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M DS3H AX",
};

static const gb_fusion2_device b760m_ds3h_ax_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M DS3H AX DDR4",
};

static const gb_fusion2_device b760m_ds3h_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M DS3H DDR4",
};

static const gb_fusion2_device b760m_gmg_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M GAMING",
};

static const gb_fusion2_device b760m_gmg_ac_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M GAMING AC",
};

static const gb_fusion2_device b760m_gmg_ac_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M GAMING AC DDR4",
};

static const gb_fusion2_device b760m_gmg_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M GAMING DDR4",
};

static const gb_fusion2_device b760m_gmg_plus_wifi_ddr4_device =
{
    &it5702_27_device,
    0x0810005F,
    0,
    "B760M GAMING PLUS WIFI DDR4",
};

/*-------------------------------------------------------------*\
|  Layout 28   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_28_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device h470_hd3_device =
{
    &it5702_28_device,
    0x0970005F,
    0,
    "H470 HD3",
};

static const gb_fusion2_device a620m_ds3h_2_device =
{
    &it5702_28_device,
    0x1040005F,
    0,
    "A620M DS3H",
};

static const gb_fusion2_device a620m_gmg_x_2_device =
{
    &it5702_28_device,
    0x1040005F,
    0,
    "A620M GAMING X",
};

static const gb_fusion2_device a620m_gmg_x_ax_2_device =
{
    &it5702_28_device,
    0x1040005F,
    0,
    "A620M GAMING X AX",
};

static const gb_fusion2_device a620m_h_2_device =
{
    &it5702_28_device,
    0x1040005F,
    0,
    "A620M H",
};

static const gb_fusion2_device a620m_s2h_2_device =
{
    &it5702_28_device,
    0x1040005F,
    0,
    "A620M S2H",
};

static const gb_fusion2_device b650m_d3hp_device =
{
    &it5702_28_device,
    0x0870005F,
    0,
    "B650M D3HP",
};

static const gb_fusion2_device b650m_d3hp_ax_device =
{
    &it5702_28_device,
    0x0870005F,
    0,
    "B650M D3HP AX",
};

static const gb_fusion2_device b650m_ds3h_2_device =
{
    &it5702_28_device,
    0x0870005F,
    0,
    "B650M DS3H",
};

static const gb_fusion2_device b650m_gmg_plus_wifi_device =
{
    &it5702_28_device,
    0x0870005F,
    0,
    "B650M GAMING PLUS WIFI",
};

static const gb_fusion2_device b650m_gmg_wifi_device =
{
    &it5702_28_device,
    0x0870005F,
    0,
    "B650M GAMING WIFI",
};

static const gb_fusion2_device b650m_gmg_wifi6e_device =
{
    &it5702_28_device,
    0x0870005F,
    0,
    "B650M GAMING WIFI6E",
};

static const gb_fusion2_device b650m_k_2_device =
{
    &it5702_28_device,
    0x0870005F,
    0,
    "B650M K",
};

/*-------------------------------------------------------------*\
|  Layout 29   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_1/ARGB_V2_3"                      : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_29_device =
{
    &common_argb_v2_1_3_zone,
    &common_argb_v2_2_zone,
    &common_chip_acc_4_zone,
    &common_led_c_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650_aor_elite_ax_ice_device =
{
    &it5702_29_device,
    0x2110015F,
    0,
    "B650 AORUS ELITE AX ICE",
};

static const gb_fusion2_device b650_aor_elite_ax_v2_device =
{
    &it5702_29_device,
    0x2110015F,
    0,
    "B650 AORUS ELITE AX V2",
};

static const gb_fusion2_device b650_aor_elite_v2_device =
{
    &it5702_29_device,
    0x2110015F,
    0,
    "B650 AORUS ELITE V2",
};

static const gb_fusion2_device b650_aor_elite_x_ax_ice_device =
{
    &it5702_29_device,
    0x2110015F,
    0,
    "B650E AORUS ELITE X AX ICE",
};

/*-------------------------------------------------------------*\
|  Layout 30   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_1/ARGB_V2_3"                      : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_30_device =
{
    &common_argb_v2_1_3_zone,
    &common_argb_v2_2_zone,
    &common_chip_acc_3_zone,
    &common_led_c_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z790_aor_elite_x_device =
{
    &it5702_30_device,
    0x1120015F,
    0,
    "Z790 AORUS ELITE X",
};

static const gb_fusion2_device z790_aor_elite_x_ax_device =
{
    &it5702_30_device,
    0x1120015F,
    0,
    "Z790 AORUS ELITE X AX",
};

static const gb_fusion2_device z790_aor_elite_x_wifi7_device =
{
    &it5702_30_device,
    0x1120015F,
    0,
    "Z790 AORUS ELITE X WIFI7",
};

static const gb_fusion2_device z790_aor_tachyon_x_device =
{
    &it5702_30_device,
    0x1120015F,
    0,
    "Z790 AORUS TACHYON X",
};

/*-------------------------------------------------------------*\
|  Layout 31   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_31_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_cpu_3_zone,
    &common_led_c_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b450m_ds3h_v3_device =
{
    &it5702_31_device,
    0x2010005F,
    0,
    "B450M DS3H V3",
};

static const gb_fusion2_device b450m_ds3h_wifi_device =
{
    &it5702_31_device,
    0x2010005F,
    0,
    "B450M DS3H WIFI",
};

/*-------------------------------------------------------------*\
|  Layout 32   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_32_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490_gmg_x_device =
{
    &it5702_32_device,
    0x0140005F,
    0,
    "Z490 GAMING X",
};

static const gb_fusion2_device z490_gmg_x_ax_device =
{
    &it5702_32_device,
    0x0140005F,
    0,
    "Z490 GAMING X AX",
};

static const gb_fusion2_device z490_ud_device =
{
    &it5702_32_device,
    0x0140005F,
    0,
    "Z490 UD",
};

static const gb_fusion2_device z490_ud_ac_device =
{
    &it5702_32_device,
    0x0140005F,
    0,
    "Z490 UD AC",
};

static const gb_fusion2_device b460m_ds3h_ac_device =
{
    &it5702_32_device,
    0x1080005F,
    0,
    "B460M DS3H AC",
};

static const gb_fusion2_device b660_ds3h_ac_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B660 DS3H AC",
};

static const gb_fusion2_device b660_ds3h_ac_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B660 DS3H AC DDR4",
};

static const gb_fusion2_device b660_ds3h_ax_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B660 DS3H AX DDR4",
};

static const gb_fusion2_device b660_ds3h_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B660 DS3H DDR4",
};

static const gb_fusion2_device b660_gmg_x_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B660 GAMING X",
};

static const gb_fusion2_device b660_gmg_x_ax_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B660 GAMING X AX DDR4",
};

static const gb_fusion2_device b660_gmg_x_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B660 GAMING X DDR4",
};

static const gb_fusion2_device b660m_aor_elite_ax_ddr4_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B660M AORUS ELITE AX DDR4",
};

static const gb_fusion2_device b660m_aor_elite_ddr4_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B660M AORUS ELITE DDR4",
};

static const gb_fusion2_device b660m_gmg_x_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B660M GAMING X",
};

static const gb_fusion2_device b660m_gmg_x_ax_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B660M GAMING X AX",
};

static const gb_fusion2_device b660m_gmg_x_ax_ddr4_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B660M GAMING X AX DDR4",
};

static const gb_fusion2_device b660m_gmg_x_ddr4_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B660M GAMING X DDR4",
};

static const gb_fusion2_device b760_aor_elite_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 AORUS ELITE",
};

static const gb_fusion2_device b760_aor_elite_ax_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 AORUS ELITE AX",
};

static const gb_fusion2_device b760_aor_elite_ax_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 AORUS ELITE AX DDR4",
};

static const gb_fusion2_device b760_aor_elite_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 AORUS ELITE DDR4",
};

static const gb_fusion2_device b760_ds3h_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 DS3H",
};

static const gb_fusion2_device b760_ds3h_ac_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 DS3H AC",
};

static const gb_fusion2_device b760_ds3h_ac_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 DS3H AC DDR4",
};

static const gb_fusion2_device b760_ds3h_ax_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 DS3H AX",
};

static const gb_fusion2_device b760_ds3h_ax_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 DS3H AX DDR4",
};

static const gb_fusion2_device b760_ds3h_ax_v2_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 DS3H AX V2",
};

static const gb_fusion2_device b760_ds3h_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 DS3H DDR4",
};

static const gb_fusion2_device b760_gmg_x_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 GAMING X",
};

static const gb_fusion2_device b760_gmg_x_ax_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 GAMING X AX",
};

static const gb_fusion2_device b760_gmg_x_ax_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 GAMING X AX DDR4",
};

static const gb_fusion2_device b760_gmg_x_ddr4_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "B760 GAMING X DDR4",
};

static const gb_fusion2_device b760m_c_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B760M C",
};

static const gb_fusion2_device b760m_c_v2_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B760M C V2",
};

static const gb_fusion2_device b760m_gmg_x_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B760M GAMING X",
};

static const gb_fusion2_device b760m_gmg_x_ax_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B760M GAMING X AX",
};

static const gb_fusion2_device b760m_gmg_x_ax_ddr4_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B760M GAMING X AX DDR4",
};

static const gb_fusion2_device b760m_gmg_x_ddr4_device =
{
    &it5702_32_device,
    0x0820005F,
    0,
    "B760M GAMING X DDR4",
};

static const gb_fusion2_device z690_aero_d_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 AERO D",
};

static const gb_fusion2_device z690_aero_g_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 AERO G",
};

static const gb_fusion2_device z690_aero_g_ddr4_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 AERO G DDR4",
};

static const gb_fusion2_device z690_aor_elite_ax_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 AORUS ELITE AX",
};

static const gb_fusion2_device z690_ud_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD",
};

static const gb_fusion2_device z690_ud_ac_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD AC",
};

static const gb_fusion2_device z690_ud_ax_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD AX",
};

static const gb_fusion2_device z690_ud_ax_ddr4_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD AX DDR4",
};

static const gb_fusion2_device z690_ud_ax_ddr4_v2_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD AX DDR4 V2",
};

static const gb_fusion2_device z690_ud_ax_v2_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD AX V2",
};

static const gb_fusion2_device z690_ud_ddr4_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD DDR4",
};

static const gb_fusion2_device z690_ud_ddr4_v2_device =
{
    &it5702_32_device,
    0x0110005F,
    0,
    "Z690 UD DDR4 V2",
};

static const gb_fusion2_device z690m_aor_elite_ax_ddr4_device =
{
    &it5702_32_device,
    0x0010005F,
    0,
    "Z690M AORUS ELITE AX DDR4",
};

static const gb_fusion2_device z690m_aor_elite_ddr4_device =
{
    &it5702_32_device,
    0x0010005F,
    0,
    "Z690M AORUS ELITE DDR4",
};

static const gb_fusion2_device z790_aero_g_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 AERO G",
};

static const gb_fusion2_device z790_d_ddr4_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 D DDR4",
};

static const gb_fusion2_device z790_gmg_plus_ax_device =
{
    &it5702_32_device,
    0x0920005F,
    0,
    "Z790 GAMING PLUS AX",
};

static const gb_fusion2_device z790_gmg_x_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 GAMING X",
};

static const gb_fusion2_device z790_gmg_x_ax_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 GAMING X AX",
};

static const gb_fusion2_device z790_s_ddr4_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 S DDR4",
};

static const gb_fusion2_device z790_s_wifi_ddr4_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 S WIFI DDR4",
};

static const gb_fusion2_device z790_ud_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 UD",
};

static const gb_fusion2_device z790_ud_ac_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 UD AC",
};

static const gb_fusion2_device z790_ud_ax_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z790 UD AX",
};

static const gb_fusion2_device z590_ud_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z590 UD",
};

static const gb_fusion2_device z590_ud_ac_device =
{
    &it5702_32_device,
    0x0120005F,
    0,
    "Z590 UD AC",
};

static const gb_fusion2_device z790_aor_mstr_device =
{
    &it5702_32_device,
    0x015001DF,
    0,
    "Z790 AORUS MASTER",
};

static const gb_fusion2_device z690_aor_xtrm_device =
{
    &it5702_32_device,
    0x028001DF,
    0,
    "Z690 AORUS XTREME",
};

static const gb_fusion2_device z690_aor_xtrm_waterforce_device =
{
    &it5702_32_device,
    0x029001DF,
    0,
    "Z690 AORUS XTREME WATERFORCE",
};

static const gb_fusion2_device z490_aor_xtrm_device =
{
    &it5702_32_device,
    0x029001DF,
    0,
    "Z490 AORUS XTREME",
};

static const gb_fusion2_device z490_aor_xtrm_waterforce_device =
{
    &it5702_32_device,
    0x029001DF,
    0,
    "Z490 AORUS XTREME WATERFORCE",
};

static const gb_fusion2_device z590_aor_xtrm_waterforce_device =
{
    &it5702_32_device,
    0x029001DF,
    0,
    "Z590 AORUS XTREME WATERFORCE",
};

static const gb_fusion2_device z790_aor_xtrm_device =
{
    &it5702_32_device,
    0x026001DF,
    0,
    "Z790 AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 33   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "Chipset Accent"                           : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_33_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c_2_zone,
    &common_chip_acc_4_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650m_gmg_x_ax_2_device =
{
    &it5702_33_device,
    0x0880015F,
    0,
    "B650M GAMING X AX",
};

/*-------------------------------------------------------------*\
|  Layout 34   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "PCI-E Accent"                             : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_34_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c_2_zone,
    &common_pcie_acc_4_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_g_ax_device =
{
    &it5702_34_device,
    0x0870005F,
    0,
    "B760M G AX",
};

static const gb_fusion2_device b760m_gmg_wifi_device =
{
    &it5702_34_device,
    0x0870005F,
    0,
    "B760M GAMING WIFI",
};

static const gb_fusion2_device b760m_gmg_wifi_plus_device =
{
    &it5702_34_device,
    0x0870005F,
    0,
    "B760M GAMING WIFI PLUS",
};

/*-------------------------------------------------------------*\
|  Layout 35   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_1/ARGB_V2_3"                      : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "IO Cover (Top)"                           : Single   |
|    Zone "IO Cover (Bottom)"                        : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_35_device =
{
    &common_argb_v2_3_1_zone,
    &common_argb_v2_2_zone,
    &common_io_cov_1_zone,
    &common_io_cov_btm_4_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x670e_aor_pro_x_device =
{
    &it5702_35_device,
    0x192001DF,
    0,
    "X670E AORUS PRO X",
};

static const gb_fusion2_device z790_aor_pro_x_device =
{
    &it5702_35_device,
    0x111001DF,
    0,
    "Z790 AORUS PRO X",
};

static const gb_fusion2_device z790_aor_pro_x_wifi7_device =
{
    &it5702_35_device,
    0x111001DF,
    0,
    "Z790 AORUS PRO X WIFI7",
};

/*-------------------------------------------------------------*\
|  Layout 36   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_1/ARGB_V2_3"                      : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_C2"                                   : Single   |
|    Zone "LED_C3"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_36_device =
{
    &common_argb_v2_3_1_zone,
    &common_argb_v2_2_zone,
    &common_led_c1_2_zone,
    &common_led_c3_4_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z790_aor_xtreme_x_ice_device =
{
    &it5702_36_device,
    0x1250005F,
    0,
    "Z790 AORUS XTREME X ICE",
};

static const gb_fusion2_device z790_aor_xtreme_x_device =
{
    &it5702_36_device,
    0x124001DF,
    0,
    "Z790 AORUS XTREME X",
};

/*-------------------------------------------------------------*\
|  Layout 37   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_37_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490_vision_g_device =
{
    &it5702_37_device,
    0x01B0005F,
    0,
    "Z490 VISION G",
};

static const gb_fusion2_device z690_aor_pro_device =
{
    &it5702_37_device,
    0x0130015F,
    0,
    "Z690 AORUS PRO",
};

static const gb_fusion2_device z690_aor_pro_ddr4_device =
{
    &it5702_37_device,
    0x0130015F,
    0,
    "Z690 AORUS PRO DDR4",
};

static const gb_fusion2_device z690_aor_ultra_device =
{
    &it5702_37_device,
    0x0130015F,
    0,
    "Z690 AORUS ULTRA",
};

static const gb_fusion2_device z590_aor_ultra_device =
{
    &it5702_37_device,
    0x0130015F,
    0,
    "Z590 AORUS ULTRA",
};

static const gb_fusion2_device z590_vision_d_device =
{
    &it5702_37_device,
    0x0130015F,
    0,
    "Z590 VISION D",
};

static const gb_fusion2_device z590_vision_g_device =
{
    &it5702_37_device,
    0x0130015F,
    0,
    "Z590 VISION G",
};

/*-------------------------------------------------------------*\
|  Layout 38   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_38_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_pcie_acc_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490m_gmg_x_device =
{
    &it5702_38_device,
    0x0040005F,
    0,
    "Z490M GAMING X",
};

static const gb_fusion2_device b460m_aor_elite_device =
{
    &it5702_38_device,
    0x1060005F,
    0,
    "B460M AORUS ELITE",
};

static const gb_fusion2_device b460m_aor_pro_device =
{
    &it5702_38_device,
    0x1060005F,
    0,
    "B460M AORUS PRO",
};

static const gb_fusion2_device b560m_aor_elite_device =
{
    &it5702_38_device,
    0x1040005F,
    0,
    "B560M AORUS ELITE",
};

static const gb_fusion2_device b560m_aor_pro_device =
{
    &it5702_38_device,
    0x1040005F,
    0,
    "B560M AORUS PRO",
};

static const gb_fusion2_device b560m_aor_pro_ax_device =
{
    &it5702_38_device,
    0x1040005F,
    0,
    "B560M AORUS PRO AX",
};

/*-------------------------------------------------------------*\
|  Layout 39   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_39_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_led_cpu_3_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b550m_ds3h_r2_device =
{
    &it5702_39_device,
    0x0030005F,
    0,
    "B550M DS3H R2",
};

static const gb_fusion2_device b650_aero_g_device =
{
    &it5702_39_device,
    0x0910005F,
    0,
    "B650 AERO G",
};

static const gb_fusion2_device b650_gmg_x_device =
{
    &it5702_39_device,
    0x0910005F,
    0,
    "B650 GAMING X",
};

static const gb_fusion2_device b650_gmg_x_ax_device =
{
    &it5702_39_device,
    0x0910005F,
    0,
    "B650 GAMING X AX",
};

static const gb_fusion2_device b650_gmg_x_ax_v2_device =
{
    &it5702_39_device,
    0x0910005F,
    0,
    "B650 GAMING X AX V2",
};

static const gb_fusion2_device x670_aor_elite_ax_device =
{
    &it5702_39_device,
    0x0110005F,
    0,
    "X670 AORUS ELITE AX",
};

static const gb_fusion2_device x670_gmg_x_ax_device =
{
    &it5702_39_device,
    0x0110005F,
    0,
    "X670 GAMING X AX",
};

static const gb_fusion2_device b550_eagle_device =
{
    &it5702_39_device,
    0x0140005F,
    0,
    "B550 EAGLE",
};

static const gb_fusion2_device b550_eagle_wifi6_device =
{
    &it5702_39_device,
    0x0140005F,
    0,
    "B550 EAGLE WIFI6",
};

static const gb_fusion2_device b550_gmg_x_device =
{
    &it5702_39_device,
    0x0140005F,
    0,
    "B550 GAMING X",
};

static const gb_fusion2_device b550_gmg_x_v2_device =
{
    &it5702_39_device,
    0x0140005F,
    0,
    "B550 GAMING X V2",
};

static const gb_fusion2_device b550_vision_d_device =
{
    &it5702_39_device,
    0x0140005F,
    0,
    "B550 VISION D",
};

static const gb_fusion2_device b550_vision_dp_device =
{
    &it5702_39_device,
    0x0140005F,
    0,
    "B550 VISION D-P",
};

static const gb_fusion2_device b550m_aor_elite_device =
{
    &it5702_39_device,
    0x0030005F,
    0,
    "B550M AORUS ELITE",
};

static const gb_fusion2_device b550m_aor_elite_ax_device =
{
    &it5702_39_device,
    0x0030005F,
    0,
    "B550M AORUS ELITE AX",
};

static const gb_fusion2_device b550m_ds3h_device =
{
    &it5702_39_device,
    0x0030005F,
    0,
    "B550M DS3H",
};

static const gb_fusion2_device b550m_ds3h_ac_device =
{
    &it5702_39_device,
    0x0030005F,
    0,
    "B550M DS3H AC",
};

static const gb_fusion2_device b550m_ds3h_ac_r2_device =
{
    &it5702_39_device,
    0x0030005F,
    0,
    "B550M DS3H AC R2",
};

static const gb_fusion2_device b550m_gmg_x_wifi6_device =
{
    &it5702_39_device,
    0x0030005F,
    0,
    "B550M GAMING X WIFI6",
};

static const gb_fusion2_device x570s_aero_g_device =
{
    &it5702_39_device,
    0x0110015F,
    0,
    "X570S AERO G",
};

static const gb_fusion2_device x570s_ud_device =
{
    &it5702_39_device,
    0x0110015F,
    0,
    "X570S UD",
};

static const gb_fusion2_device x670e_aor_xtrm_device =
{
    &it5702_39_device,
    0x023001DF,
    0,
    "X670E AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 40   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_40_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_chip_acc_3_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b660m_aor_pro_device =
{
    &it5702_40_device,
    0x0870015F,
    0,
    "B660M AORUS PRO",
};

static const gb_fusion2_device b660m_aor_pro_ax_device =
{
    &it5702_40_device,
    0x0870015F,
    0,
    "B660M AORUS PRO AX",
};

static const gb_fusion2_device b660m_aor_pro_ax_ddr4_device =
{
    &it5702_40_device,
    0x0870015F,
    0,
    "B660M AORUS PRO AX DDR4",
};

static const gb_fusion2_device b660m_aor_pro_ddr4_device =
{
    &it5702_40_device,
    0x0870015F,
    0,
    "B660M AORUS PRO DDR4",
};

static const gb_fusion2_device b760m_aor_elite_device =
{
    &it5702_40_device,
    0x0860015F,
    0,
    "B760M AORUS ELITE",
};

static const gb_fusion2_device b760m_aor_elite_ax_device =
{
    &it5702_40_device,
    0x0860015F,
    0,
    "B760M AORUS ELITE AX",
};

static const gb_fusion2_device b760m_aor_elite_ax_ddr4_device =
{
    &it5702_40_device,
    0x0860015F,
    0,
    "B760M AORUS ELITE AX DDR4",
};

static const gb_fusion2_device b760m_aor_elite_ddr4_device =
{
    &it5702_40_device,
    0x0860015F,
    0,
    "B760M AORUS ELITE DDR4",
};

static const gb_fusion2_device z690_aor_tachyon_device =
{
    &it5702_40_device,
    0x0120015F,
    0,
    "Z690 AORUS TACHYON",
};

static const gb_fusion2_device z790_aor_elite_device =
{
    &it5702_40_device,
    0x0130015F,
    0,
    "Z790 AORUS ELITE",
};

static const gb_fusion2_device z790_aor_elite_ax_device =
{
    &it5702_40_device,
    0x0130015F,
    0,
    "Z790 AORUS ELITE AX",
};

static const gb_fusion2_device z790_aor_elite_ax_ddr4_device =
{
    &it5702_40_device,
    0x0130015F,
    0,
    "Z790 AORUS ELITE AX DDR4",
};

static const gb_fusion2_device z790_aor_elite_ax_ice_device =
{
    &it5702_40_device,
    0x0130015F,
    0,
    "Z790 AORUS ELITE AX ICE",
};

static const gb_fusion2_device z790_aor_elite_ax_w_device =
{
    &it5702_40_device,
    0x0130015F,
    0,
    "Z790 AORUS ELITE AX-W",
};

static const gb_fusion2_device z790_aor_elite_ddr4_device =
{
    &it5702_40_device,
    0x0130015F,
    0,
    "Z790 AORUS ELITE DDR4",
};

static const gb_fusion2_device z790_aor_tachyon_device =
{
    &it5702_40_device,
    0x0130015F,
    0,
    "Z790 AORUS TACHYON",
};

static const gb_fusion2_device z790m_aor_elite_device =
{
    &it5702_40_device,
    0x0030015F,
    0,
    "Z790M AORUS ELITE",
};

static const gb_fusion2_device z790m_aor_elite_ax_device =
{
    &it5702_40_device,
    0x0030015F,
    0,
    "Z790M AORUS ELITE AX",
};

static const gb_fusion2_device z790m_aor_elite_ax_ice_device =
{
    &it5702_40_device,
    0x0030015F,
    0,
    "Z790M AORUS ELITE AX ICE",
};

static const gb_fusion2_device a520_aor_elite_device =
{
    &it5702_40_device,
    0x0150005F,
    0,
    "A520 AORUS ELITE",
};

static const gb_fusion2_device z590_aor_tachyon_device =
{
    &it5702_40_device,
    0x0180005F,
    0,
    "Z590 AORUS TACHYON",
};

/*-------------------------------------------------------------*\
|  Layout 41   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_41_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_pcie_acc_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z590m_device =
{
    &it5702_41_device,
    0x0020005F,
    0,
    "Z590M",
};

static const gb_fusion2_device z590m_gmg_x_device =
{
    &it5702_41_device,
    0x0020005F,
    0,
    "Z590M GAMING X",
};

/*-------------------------------------------------------------*\
|  Layout 42   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover (Top)"                           : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "IO Cover (Bottom)"                        : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_42_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_top_1_zone,
    &common_chip_acc_3_zone,
    &common_io_cov_btm_4_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650e_aor_pro_x_usb4_device =
{
    &it5702_42_device,
    0x214001DF,
    0,
    "B650E AORUS PRO X USB4",
};

/*-------------------------------------------------------------*\
|  Layout 43   048D:5702                                        |
|                                                               |
|    Zone "D_LED"                                    : Linear   |
|    Zone "Board Accent (Right Side, Top)"           : Single   |
|    Zone "Board Accent (Right Side, Top Middle)"    : Single   |
|    Zone "Board Accent (Right Side, Bottom Middle)" : Single   |
|    Zone "Board Accent (Right Side, Bottom)"        : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "IO Cover (Bottom)"                        : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_43_device =
{
    &common_d_led_zone,
    &common_brst_1_zone,
    &common_brstm_2_zone,
    &common_brsbm_3_zone,
    &common_brsb_4_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b660i_aor_pro_ddr4_device =
{
    &it5702_43_device,
    0x0B5001DF,
    0,
    "B660I AORUS PRO DDR4",
};

static const gb_fusion2_device b760i_aor_pro_device =
{
    &it5702_43_device,
    0x0B3001DF,
    0,
    "B760I AORUS PRO",
};

static const gb_fusion2_device b760i_aor_pro_ddr4_device =
{
    &it5702_43_device,
    0x0B3001DF,
    0,
    "B760I AORUS PRO DDR4",
};

static const gb_fusion2_device b690i_aor_ultra_device =
{
    &it5702_43_device,
    0x034001DF,
    0,
    "Z690I AORUS ULTRA",
};

static const gb_fusion2_device b690i_aor_ultra_ddr4_device =
{
    &it5702_43_device,
    0x034001DF,
    0,
    "Z690I AORUS ULTRA DDR4",
};

static const gb_fusion2_device b690i_aor_ultra_lite_device =
{
    &it5702_43_device,
    0x034001DF,
    0,
    "Z690I AORUS ULTRA LITE",
};

static const gb_fusion2_device b690i_aor_ultra_lite_ddr4_device =
{
    &it5702_43_device,
    0x034001DF,
    0,
    "Z690I AORUS ULTRA LITE DDR4",
};

static const gb_fusion2_device b690i_aor_ultra_plus_device =
{
    &it5702_43_device,
    0x034001DF,
    0,
    "Z690I AORUS ULTRA PLUS",
};

static const gb_fusion2_device b690i_aor_ultra_plus_ddr4_device =
{
    &it5702_43_device,
    0x034001DF,
    0,
    "Z690I AORUS ULTRA PLUS DDR4",
};

static const gb_fusion2_device b790i_aor_ultra_device =
{
    &it5702_43_device,
    0x034001DF,
    0,
    "Z790I AORUS ULTRA",
};

static const gb_fusion2_device b550i_aor_pro_ax_device =
{
    &it5702_43_device,
    0x0360007F,
    0,
    "B550I AORUS PRO AX",
};

static const gb_fusion2_device b560i_aor_pro_ax_device =
{
    &it5702_43_device,
    0x1310007F,
    0,
    "B560I AORUS PRO AX",
};

static const gb_fusion2_device x570si_aor_pro_ax_device =
{
    &it5702_43_device,
    0x034000FF,
    0,
    "X570SI AORUS PRO AX",
};

static const gb_fusion2_device z590i_aor_ultra_device =
{
    &it5702_43_device,
    0x0350007F,
    0,
    "Z590I AORUS ULTRA",
};

/*-------------------------------------------------------------*\
|  Layout 44   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_44_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_pcie_acc_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490_aor_elite_device =
{
    &it5702_44_device,
    0x0160005F,
    0,
    "Z490 AORUS ELITE",
};

static const gb_fusion2_device z490_aor_elite_ac_device =
{
    &it5702_44_device,
    0x0160005F,
    0,
    "Z490 AORUS ELITE AC",
};

static const gb_fusion2_device z490_aor_pro_ax_device =
{
    &it5702_44_device,
    0x0160005F,
    0,
    "Z490 AORUS PRO AX",
};

static const gb_fusion2_device h470_aor_pro_ax_device =
{
    &it5702_44_device,
    0x0960005F,
    0,
    "H470 AORUS PRO AX",
};

static const gb_fusion2_device b460_aor_pro_ac_device =
{
    &it5702_44_device,
    0x1170005F,
    0,
    "B460 AORUS PRO AC",
};

static const gb_fusion2_device z590_aor_pro_ax_device =
{
    &it5702_44_device,
    0x0160005F,
    0,
    "Z590 AORUS PRO AX",
};

/*-------------------------------------------------------------*\
|  Layout 45   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_45_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_chip_acc_3_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490_aor_mstr_device =
{
    &it5702_45_device,
    0x0170005F,
    0,
    "Z490 AORUS MASTER",
};

static const gb_fusion2_device z490_aor_mstr_waterforce_device =
{
    &it5702_45_device,
    0x0170005F,
    0,
    "Z490 AORUS MASTER WATERFORCE",
};

static const gb_fusion2_device z490_aor_ultra_device =
{
    &it5702_45_device,
    0x0170005F,
    0,
    "Z490 AORUS ULTRA",
};

static const gb_fusion2_device z490_aor_ultra_g2_device =
{
    &it5702_45_device,
    0x0170005F,
    0,
    "Z490 AORUS ULTRA G2",
};

static const gb_fusion2_device b760m_aor_pro_device =
{
    &it5702_45_device,
    0x0840015F,
    0,
    "B760M AORUS PRO",
};

static const gb_fusion2_device b760m_aor_pro_ax_device =
{
    &it5702_45_device,
    0x0840015F,
    0,
    "B760M AORUS PRO AX",
};

static const gb_fusion2_device b760m_aor_pro_ax_ddr4_device =
{
    &it5702_45_device,
    0x0840015F,
    0,
    "B760M AORUS PRO AX DDR4",
};

static const gb_fusion2_device b760m_aor_pro_ddr4_device =
{
    &it5702_45_device,
    0x0840015F,
    0,
    "B760M AORUS PRO DDR4",
};

static const gb_fusion2_device z590_aor_mstr_device =
{
    &it5702_45_device,
    0x0170005F,
    0,
    "Z590 AORUS MASTER",
};

static const gb_fusion2_device z590_aor_xtrm_device =
{
    &it5702_45_device,
    0x02B001DF,
    0,
    "Z590 AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 46   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_46_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_led_cpu_3_zone,
    &common_chip_acc_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650_aor_elite_device =
{
    &it5702_46_device,
    0x0960015F,
    0,
    "B650 AORUS ELITE",
};

static const gb_fusion2_device b650_aor_elite_ax_device =
{
    &it5702_46_device,
    0x0960015F,
    0,
    "B650 AORUS ELITE AX",
};

static const gb_fusion2_device b650m_aor_elite_device =
{
    &it5702_46_device,
    0x0860015F,
    0,
    "B650M AORUS ELITE",
};

static const gb_fusion2_device b650m_aor_elite_ax_device =
{
    &it5702_46_device,
    0x0860015F,
    0,
    "B650M AORUS ELITE AX",
};

static const gb_fusion2_device b650m_aor_elite_ax_ice_device =
{
    &it5702_46_device,
    0x0860015F,
    0,
    "B650M AORUS ELITE AX ICE",
};

static const gb_fusion2_device b650m_aor_pro_ax_device =
{
    &it5702_46_device,
    0x0860015F,
    0,
    "B650M AORUS PRO AX",
};

/*-------------------------------------------------------------*\
|  Layout 47   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "Board Accent (Right Side, Top)"           : Single   |
|    Zone "Board Accent (Right Side, Top Middle)"    : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_47_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_brst_3_zone,
    &common_brstm_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b660_aor_elite_ax_ddr4_device =
{
    &it5702_47_device,
    0x096001DF,
    0,
    "B660 AORUS ELITE AX DDR4",
};

static const gb_fusion2_device b660_aor_elite_ddr4_device =
{
    &it5702_47_device,
    0x096001DF,
    0,
    "B660 AORUS ELITE DDR4",
};

static const gb_fusion2_device z690_gmg_x_device =
{
    &it5702_47_device,
    0x01A001DF,
    0,
    "Z690 GAMING X",
};

static const gb_fusion2_device z690_gmg_x_ddr4_device =
{
    &it5702_47_device,
    0x01A001DF,
    0,
    "Z690 GAMING X DDR4",
};

static const gb_fusion2_device z690_gmg_x_ddr4_v2_device =
{
    &it5702_47_device,
    0x01A001DF,
    0,
    "Z690 GAMING X DDR4 V2",
};

static const gb_fusion2_device z590_gmg_x_device =
{
    &it5702_47_device,
    0x0140005F,
    0,
    "Z590 GAMING X",
};

/*-------------------------------------------------------------*\
|  Layout 48   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_48_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_led_cpu_3_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b550_aor_mstr_device =
{
    &it5702_48_device,
    0x0180005F,
    0,
    "B550 AORUS MASTER",
};

static const gb_fusion2_device x570s_aor_mstr_device =
{
    &it5702_48_device,
    0x0130015F,
    0,
    "X570S AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  Layout 49   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_49_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_led_c1_2_zone,
    &common_led_cpu_3_zone,
    &common_pcie_acc_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b550m_aor_pro_device =
{
    &it5702_49_device,
    0x0050005F,
    0,
    "B550M AORUS PRO",
};

static const gb_fusion2_device b550m_aor_pro_ax_device =
{
    &it5702_49_device,
    0x0050005F,
    0,
    "B550M AORUS PRO AX",
};

static const gb_fusion2_device b550m_aor_pro_p_device =
{
    &it5702_49_device,
    0x0050005F,
    0,
    "B550M AORUS PRO-P",
};

static const gb_fusion2_device b560_aor_pro_ax_device =
{
    &it5702_49_device,
    0x1150005F,
    0,
    "B560 AORUS PRO AX",
};

static const gb_fusion2_device z590_aor_elite_device =
{
    &it5702_49_device,
    0x01A0005F,
    0,
    "Z590 AORUS ELITE",
};

static const gb_fusion2_device z590_aor_elite_ax_device =
{
    &it5702_49_device,
    0x01A0005F,
    0,
    "Z590 AORUS ELITE AX",
};

/*-------------------------------------------------------------*\
|  Layout 50   048D:5702                                        |
|                                                               |
|    Zone "ARGB_V2_1/ARGB_V2_3"                      : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "IO Cover (Bottom)"                        : Single   |
|    Zone "IO Cover (Bottom Middle)"                 : Single   |
|    Zone "IO Cover (Top Middle)"                    : Single   |
|    Zone "IO Cover (Top)"                           : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_50_device =
{
    &common_argb_v2_1_3_zone,
    &common_argb_v2_2_zone,
    &common_io_cov_btm_1_zone,
    &common_io_cov_btm_mid_2_zone,
    &common_chip_io_cov_tm_3_zone,
    &common_io_cov_top_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_aor_elite_x_ax_device =
{
    &it5702_50_device,
    0x181001DF,
    0,
    "B760M AORUS ELITE X AX",
};

/*-------------------------------------------------------------*\
|  Layout 51   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_51_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_btm_1_zone,
    &common_io_cov_btm_mid_2_zone,
    &common_chip_io_cov_tm_3_zone,
    &common_io_cov_top_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490_vision_d_device =
{
    &it5702_51_device,
    0x0180005F,
    0,
    "Z490 VISION D",
};

/*-------------------------------------------------------------*\
|  Layout 52   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover (Top)"                           : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "IO Cover (Bottom)"                        : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_52_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_top_1_zone,
    &common_led_c1_2_zone,
    &common_led_cpu_3_zone,
    &common_io_cov_btm_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650_aor_pro_ax_device =
{
    &it5702_52_device,
    0x09A001DF,
    0,
    "B650 AORUS PRO AX",
};

static const gb_fusion2_device b650e_aor_mstr_device =
{
    &it5702_52_device,
    0x09A001DF,
    0,
    "B650E AORUS MASTER",
};

static const gb_fusion2_device x670e_aor_mstr_device =
{
    &it5702_52_device,
    0x024001DF,
    0,
    "X670E AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  Layout 53   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "Board Accent (Right Side, Top)"           : Single   |
|    Zone "Board Accent (Right Side, Top Middle)"    : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_53_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_brst_3_zone,
    &common_brstm_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b660_aor_mstr_device =
{
    &it5702_53_device,
    0x094001DF,
    0,
    "B660 AORUS MASTER",
};

static const gb_fusion2_device b660_aor_mstr_ddr4_device =
{
    &it5702_53_device,
    0x094001DF,
    0,
    "B660 AORUS MASTER DDR4",
};

static const gb_fusion2_device b760_aor_mstr_ddr4_device =
{
    &it5702_53_device,
    0x094001DF,
    0,
    "B760 AORUS MASTER DDR4",
};

static const gb_fusion2_device z690_aor_elite_device =
{
    &it5702_53_device,
    0x016001DF,
    0,
    "Z690 AORUS ELITE",
};

static const gb_fusion2_device z690_aor_elite_ax_2_device =
{
    &it5702_53_device,
    0x016001DF,
    0,
    "Z690 AORUS ELITE AX",
};

static const gb_fusion2_device z690_aor_elite_ax_ddr4_device =
{
    &it5702_53_device,
    0x016001DF,
    0,
    "Z690 AORUS ELITE AX DDR4",
};

static const gb_fusion2_device z690_aor_elite_ax_ddr4_v2_device =
{
    &it5702_53_device,
    0x016001DF,
    0,
    "Z690 AORUS ELITE AX DDR4 V2",
};

static const gb_fusion2_device z690_aor_elite_ddr4_device =
{
    &it5702_53_device,
    0x016001DF,
    0,
    "Z690 AORUS ELITE DDR4",
};

/*-------------------------------------------------------------*\
|  Layout 54   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "LED_CPU"                                  : Single   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_54_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_1_zone,
    &common_led_c1_2_zone,
    &common_led_cpu_3_zone,
    &common_pcie_acc_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b550_aor_elite_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS ELITE",
};

static const gb_fusion2_device b550_aor_elite_ax_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS ELITE AX",
};

static const gb_fusion2_device b550_aor_elite_ax_v2_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS ELITE AX V2",
};

static const gb_fusion2_device b550_aor_elite_ax_v3_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS ELITE AX V3",
};

static const gb_fusion2_device b550_aor_elite_v2_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS ELITE V2",
};

static const gb_fusion2_device b550_aor_pro_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS PRO",
};

static const gb_fusion2_device b550_aor_pro_ac_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS PRO AC",
};

static const gb_fusion2_device b550_aor_pro_ax_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS PRO AX",
};

static const gb_fusion2_device b550_aor_pro_v2_device =
{
    &it5702_54_device,
    0x0170005F,
    0,
    "B550 AORUS PRO V2",
};

static const gb_fusion2_device x570s_aor_pro_ax_device =
{
    &it5702_54_device,
    0x0150015F,
    0,
    "X570S AORUS PRO AX",
};

/*-------------------------------------------------------------*\
|  Layout 55   048D:5702                                        |
|                                                               |
|    Zone "IO Cover"                                 : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_55_device =
{
    &common_io_cov_1_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z790_aor_mstr_x_2_device =
{
    &it5702_55_device,
    0x123001DF,
    1,
    "Z790 AORUS MASTER X",
};

static const gb_fusion2_device z790_aor_mstr_2_device =
{
    &it5702_55_device,
    0x015001DF,
    1,
    "Z790 AORUS MASTER",
};

static const gb_fusion2_device x670_aor_xtrm_2_device =
{
    &it5702_55_device,
    0x023001DF,
    1,
    "X670E AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 56   048D:5702                                        |
|                                                               |
|    Zone "Chipset Accent"                           : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_56_device =
{
    &common_chip_acc_3_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z790_aor_xtrm_2_device =
{
    &it5702_56_device,
    0x026001DF,
    1,
    "Z790 AORUS XTREME",
};

static const gb_fusion2_device z790_aor_xtrm_x_2_device =
{
    &it5702_56_device,
    0x124001DF,
    1,
    "Z790 AORUS XTREME X",
};

/*-------------------------------------------------------------*\
|  Layout 57   048D:5702                                        |
|                                                               |
|    Zone "IO Cover"                                 : Single   |
|    Zone "LED_CPU"                                  : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_57_device =
{
    &common_chip_acc_3_zone,
    &common_led_cpu_8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z690_aor_xtrm_waterforce_2_device =
{
    &it5702_57_device,
    0x029001DF,
    1,
    "Z690 AORUS XTREME WATERFORCE",
};

/*-------------------------------------------------------------*\
|  Layout 58   048D:5702                                        |
|                                                               |
|    Zone "IO Cover"                                 : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_CPU"                                  : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_58_device =
{
    &common_chip_acc_3_zone,
    &common_chip_acc_3_zone,
    &common_led_cpu_8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z490_aor_xtrm_2_device =
{
    &it5702_58_device,
    0x029001DF,
    1,
    "Z490 AORUS XTREME",
};

static const gb_fusion2_device z490_aor_xtrm_waterforce_2_device =
{
    &it5702_58_device,
    0x029001DF,
    1,
    "Z490 AORUS XTREME WATERFORCE",
};

static const gb_fusion2_device z590_aor_xtrm_waterforce_2_device =
{
    &it5702_58_device,
    0x029001DF,
    1,
    "Z590 AORUS XTREME WATERFORCE",
};

/*-------------------------------------------------------------*\
|  Layout 59   048D:5702                                        |
|                                                               |
|    Zone "IO Cover"                                 : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "RAM Cover"                                : Single   |
|    Zone "LED_CPU"                                  : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_59_device =
{
    &common_chip_acc_3_zone,
    &common_chip_acc_3_zone,
    &common_ram_cov_4_zone,
    &common_led_cpu_8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z690_aor_xtrm_2_device =
{
    &it5702_59_device,
    0x028001DF,
    1,
    "Z690 AORUS XTREME",
};

/*-------------------------------------------------------------*\
|  Layout 60   048D:5702                                        |
|                                                               |
|    Zone "D_LED1"                                   : Linear   |
|    Zone "D_LED2"                                   : Linear   |
|    Zone "IO Cover (Top)"                           : Single   |
|    Zone "LED_C1"                                   : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "IO Cover (Bottom)"                        : Single   |
|    Zone "LED_C2"                                   : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5702_60_device =
{
    &common_d_led1_zone,
    &common_d_led2_zone,
    &common_io_cov_top_1_zone,
    &common_led_c1_2_zone,
    &common_chip_acc_3_zone,
    &common_io_cov_btm_4_zone,
    &common_led_c2_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z690_aor_mstr_device =
{
    &it5702_60_device,
    0x017001DF,
    0,
    "Z690 AORUS MASTER",
};

/*-------------------------------------------------------------*\
|  Layout 60   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_60_device =
{
    &common_argb_v2_1_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device a620i_ax_5711_device =
{
    &it5711_60_device,
    0x4310005F,
    0,
    "A620I AX",
};

/*-------------------------------------------------------------*\
|  Layout 61   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_61_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b860i_aor_pro_ice_5711_device =
{
    &it5711_61_device,
    0x0B10005F,
    0,
    "B860I AORUS PRO ICE",
};

/*-------------------------------------------------------------*\
|  Layout 62   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_62_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device a620m_ds3h_5711_device =
{
    &it5711_62_device,
    0x4020005F,
    0,
    "A620M DS3H",
};

static const gb_fusion2_device a620m_gmg_x_5711_device =
{
    &it5711_62_device,
    0x4020005F,
    0,
    "A620M GAMING X",
};

static const gb_fusion2_device a620m_h_5711_device =
{
    &it5711_62_device,
    0x4030005F,
    0,
    "A620M H",
};

static const gb_fusion2_device a620m_s2h_5711_device =
{
    &it5711_62_device,
    0x4030005F,
    0,
    "A620M S2H",
};

static const gb_fusion2_device b840_eagle_wifi6e_5711_device =
{
    &it5711_62_device,
    0x112001DF,
    0,
    "B840 EAGLE WIFI6E",
};

static const gb_fusion2_device b840m_d2h_5711_device =
{
    &it5711_62_device,
    0x1010005F,
    0,
    "B840M D2H",
};

static const gb_fusion2_device b840m_ds3h_5711_device =
{
    &it5711_62_device,
    0x1010005F,
    0,
    "B840M DS3H",
};

static const gb_fusion2_device b840m_ds3h_wifi6_5711_device =
{
    &it5711_62_device,
    0x1010005F,
    0,
    "B840M DS3H WIFI6",
};

static const gb_fusion2_device b840m_eagle_wifi6_5711_device =
{
    &it5711_62_device,
    0x1010005F,
    0,
    "B840M EAGLE WIFI6",
};

static const gb_fusion2_device b850i_aor_pro_5711_device =
{
    &it5711_62_device,
    0x0B20005F,
    0,
    "B850I AORUS PRO",
};

static const gb_fusion2_device b850m_d3hp_5711_device =
{
    &it5711_62_device,
    0x0820005F,
    0,
    "B850M D3HP",
};

static const gb_fusion2_device x870i_aor_pro_ice_5711_device =
{
    &it5711_62_device,
    0x0310005F,
    0,
    "X870I AORUS PRO ICE",
};

/*-------------------------------------------------------------*\
|  Layout 63   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "IO Cover"                                 : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_63_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_io_cov_10_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z890i_aor_ultra_5711_device =
{
    &it5711_63_device,
    0x032001DF,
    0,
    "Z890I AORUS ULTRA",
};

/*-------------------------------------------------------------*\
|  Layout 64   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_64_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650e_eagle_wifi6e_5711_device =
{
    &it5711_64_device,
    0x3130005F,
    0,
    "B650E EAGLE WIFI6E",
};

static const gb_fusion2_device b650em_c_5711_device =
{
    &it5711_64_device,
    0x3030005F,
    0,
    "B650EM C",
};

static const gb_fusion2_device b650em_ds3h_wifi6e_5711_device =
{
    &it5711_64_device,
    0x3030005F,
    0,
    "B650EM DS3H WIFI6E",
};

static const gb_fusion2_device b650em_force_wifi6e_5711_device =
{
    &it5711_64_device,
    0x3030005F,
    0,
    "B650EM FORCE WIFI6E",
};

static const gb_fusion2_device b760_ds3h_gen5_5711_device =
{
    &it5711_64_device,
    0x2120005F,
    0,
    "B760 DS3H GEN5",
};

static const gb_fusion2_device b760_ds3h_wifi6e_gen5_5711_device =
{
    &it5711_64_device,
    0x2120005F,
    0,
    "B760 DS3H WIFI6E GEN5",
};

static const gb_fusion2_device b760_gmg_x_ddr4_gen5_5711_device =
{
    &it5711_64_device,
    0x2120005F,
    0,
    "B760 GAMING X DDR4 GEN5",
};

static const gb_fusion2_device b760_gmg_x_gen5_5711_device =
{
    &it5711_64_device,
    0x2120005F,
    0,
    "B760 GAMING X GEN5",
};

static const gb_fusion2_device b760_gmg_x_wifi6e_gen5_5711_device =
{
    &it5711_64_device,
    0x2120005F,
    0,
    "B760 GAMING X WIFI6E GEN5",
};

static const gb_fusion2_device b760m_c_v3_5711_device =
{
    &it5711_64_device,
    0x2020005F,
    0,
    "B760M C V3",
};

static const gb_fusion2_device b760m_gmg_x_ddr4_gen5_5711_device =
{
    &it5711_64_device,
    0x2020005F,
    0,
    "B760M GAMING X DDR4 GEN5",
};

static const gb_fusion2_device b760m_gmg_x_gen5_5711_device =
{
    &it5711_64_device,
    0x2020005F,
    0,
    "B760M GAMING X GEN5",
};

static const gb_fusion2_device b760m_gmg_x_wifi6e_ddr4_gen5_5711_device =
{
    &it5711_64_device,
    0x2020005F,
    0,
    "B760M GAMING X WIFI6E DDR4 GEN5",
};

static const gb_fusion2_device b760m_gmg_x_wifi6e_gen5_5711_device =
{
    &it5711_64_device,
    0x2020005F,
    0,
    "B760M GAMING X WIFI6E GEN5",
};

static const gb_fusion2_device b840_gmg_x_wifi6e_5711_device =
{
    &it5711_64_device,
    0x112001DF,
    0,
    "B840 GAMING X WIFI6E",
};

static const gb_fusion2_device b840m_h_5711_device =
{
    &it5711_64_device,
    0x1030005F,
    0,
    "B840M H",
};

static const gb_fusion2_device b850_ai_top_5711_device =
{
    &it5711_64_device,
    0x0930005F,
    0,
    "B850 AI TOP",
};

static const gb_fusion2_device b850_aor_stealth_5711_device =
{
    &it5711_64_device,
    0x0930005F,
    0,
    "B850 AORUS STEALTH",
};

static const gb_fusion2_device b850_aor_stealth_ice_5711_device =
{
    &it5711_64_device,
    0x0930005F,
    0,
    "B850 AORUS STEALTH ICE",
};

static const gb_fusion2_device b850_eagle_ice_5711_device =
{
    &it5711_64_device,
    0x0930005F,
    0,
    "B850 EAGLE ICE",
};

static const gb_fusion2_device b850_eagle_wifi6e_5711_device =
{
    &it5711_64_device,
    0x0930005F,
    0,
    "B850 EAGLE WIFI6E",
};

static const gb_fusion2_device b850_eagle_wifi7_ice_5711_device =
{
    &it5711_64_device,
    0x0930005F,
    0,
    "B850 EAGLE WIFI7 ICE",
};

static const gb_fusion2_device b850_gmg_wifi6_5711_device =
{
    &it5711_64_device,
    0x0930005F,
    0,
    "B850 GAMING WIFI6",
};

static const gb_fusion2_device b850m_c_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M C",
};

static const gb_fusion2_device b850m_ds3h_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M DS3H",
};

static const gb_fusion2_device b850m_ds3h_ice_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M DS3H ICE",
};

static const gb_fusion2_device b850m_eagle_wifi6e_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M EAGLE WIFI6E",
};

static const gb_fusion2_device b850m_eagle_wifi6e_ice_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M EAGLE WIFI6E ICE",
};

static const gb_fusion2_device b850m_force_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M FORCE",
};

static const gb_fusion2_device b850m_force_v2_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M FORCE V2",
};

static const gb_fusion2_device b850m_force_wifi6e_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M FORCE WIFI6E",
};

static const gb_fusion2_device b850m_force_wifi6e_v2_5711_device =
{
    &it5711_64_device,
    0x0830005F,
    0,
    "B850M FORCE WIFI6E V2",
};

static const gb_fusion2_device b860_ds3h_5711_device =
{
    &it5711_64_device,
    0x0940005F,
    0,
    "B860 DS3H",
};

static const gb_fusion2_device b860_ds3h_wifi6e_5711_device =
{
    &it5711_64_device,
    0x0940005F,
    0,
    "B860 DS3H WIFI6E",
};

static const gb_fusion2_device b860m_gmg_x_5711_device =
{
    &it5711_64_device,
    0x0840005F,
    0,
    "B860M GAMING X",
};

static const gb_fusion2_device b860m_gmg_x_wifi6e_5711_device =
{
    &it5711_64_device,
    0x0840005F,
    0,
    "B860M GAMING X WIFI6E",
};

static const gb_fusion2_device x870_aor_tachyon_ice_5711_device =
{
    &it5711_64_device,
    0x0230005F,
    0,
    "X870 AORUS TACHYON ICE",
};

static const gb_fusion2_device x870_gaming_wifi6_5711_device =
{
    &it5711_64_device,
    0x0230005F,
    0,
    "X870 GAMING WIFI6",
};

static const gb_fusion2_device trx50_ai_top_5711_device =
{
    &it5711_64_device,
    0x3910005F,
    0,
    "TRX50 AI TOP",
};

static const gb_fusion2_device w790_ai_top_5711_device =
{
    &it5711_64_device,
    0x2210005F,
    0,
    "W790 AI TOP",
};

static const gb_fusion2_device w880_ai_top_5711_device =
{
    &it5711_64_device,
    0x1110005F,
    0,
    "W880 AI TOP",
};

static const gb_fusion2_device z890_aero_d_5711_device =
{
    &it5711_64_device,
    0x0110005F,
    0,
    "Z890 AERO D",
};

static const gb_fusion2_device z890_aero_g_5711_device =
{
    &it5711_64_device,
    0x0110005F,
    0,
    "Z890 AERO G",
};

static const gb_fusion2_device z890_ai_top_5711_device =
{
    &it5711_64_device,
    0x0110005F,
    0,
    "Z890 AI TOP",
};

static const gb_fusion2_device z890_aor_tachyon_ice_5711_device =
{
    &it5711_64_device,
    0x0210005F,
    0,
    "Z890 AORUS TACHYON ICE",
};

static const gb_fusion2_device z890_ud_5711_device =
{
    &it5711_64_device,
    0x0110005F,
    0,
    "Z890 UD",
};

static const gb_fusion2_device z890_ud_wifi6e_5711_device =
{
    &it5711_64_device,
    0x0110005F,
    0,
    "Z890 UD WIFI6E",
};

/*-------------------------------------------------------------*\
|  Layout 65   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_65_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_pcie_acc_1_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_ds3h_ddr4_gen5_5711_device =
{
    &it5711_65_device,
    0x2010005F,
    0,
    "B760M DS3H DDR4 GEN5",
};

static const gb_fusion2_device b760m_ds3h_gen5_5711_device =
{
    &it5711_65_device,
    0x2010005F,
    0,
    "B760M DS3H GEN5",
};

static const gb_fusion2_device b760m_ds3h_wifi6e_ddr4_gen5_5711_device =
{
    &it5711_65_device,
    0x2010005F,
    0,
    "B760M DS3H WIFI6E DDR4 GEN5",
};

static const gb_fusion2_device b760m_ds3h_wifi6e_gen5_5711_device =
{
    &it5711_65_device,
    0x2010005F,
    0,
    "B760M DS3H WIFI6E GEN5",
};

static const gb_fusion2_device b760m_gmg_ac_ddr4_gen5_5711_device =
{
    &it5711_65_device,
    0x2010005F,
    0,
    "B760M GAMING AC DDR4 GEN5",
};

static const gb_fusion2_device b760m_gmg_wifi6_plus_gen5_5711_device =
{
    &it5711_65_device,
    0x2010005F,
    0,
    "B760M GAMING WIFI6 PLUS GEN5",
};

static const gb_fusion2_device b760m_gmg_wifi6e_gen5_5711_device =
{
    &it5711_65_device,
    0x2010005F,
    0,
    "B760M GAMING WIFI6E GEN5",
};

/*-------------------------------------------------------------*\
|  Layout 66   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "PCI-E Accent"                             : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_66_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_pcie_acc_1_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b860m_d2h_5711_device =
{
    &it5711_66_device,
    0x0870005F,
    0,
    "B860M D2H",
};

static const gb_fusion2_device b860m_eagle_5711_device =
{
    &it5711_66_device,
    0x0870005F,
    0,
    "B860M EAGLE",
};

static const gb_fusion2_device b860m_eagle_v2_5711_device =
{
    &it5711_66_device,
    0x0870005F,
    0,
    "B860M EAGLE V2",
};

static const gb_fusion2_device b860m_eagle_wifi6_5711_device =
{
    &it5711_66_device,
    0x0870005F,
    0,
    "B860M EAGLE WIFI6",
};

static const gb_fusion2_device b860m_eagle_wifi6_v2_5711_device =
{
    &it5711_66_device,
    0x0870005F,
    0,
    "B860M EAGLE WIFI6 V2",
};

static const gb_fusion2_device b860m_gmg_wifi6_5711_device =
{
    &it5711_66_device,
    0x0870005F,
    0,
    "B860M GAMING WIFI6",
};

static const gb_fusion2_device b860m_power_5711_device =
{
    &it5711_66_device,
    0x0870005F,
    0,
    "B860M POWER",
};

/*-------------------------------------------------------------*\
|  Layout 67   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "PCI-E Accent"                             : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_67_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_pcie_acc_1_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b860m_c_5711_device =
{
    &it5711_67_device,
    0x0850005F,
    0,
    "B860M C",
};

static const gb_fusion2_device b860m_ds3h_5711_device =
{
    &it5711_67_device,
    0x0850005F,
    0,
    "B860M DS3H",
};

static const gb_fusion2_device b860m_ds3h_wifi6e_5711_device =
{
    &it5711_67_device,
    0x0850005F,
    0,
    "B860M DS3H WIFI6E",
};

static const gb_fusion2_device b860m_eagle_plus_wifi6e_5711_device =
{
    &it5711_67_device,
    0x0850005F,
    0,
    "B860M EAGLE PLUS WIFI6E",
};

/*-------------------------------------------------------------*\
|  Layout 68   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C"                                    : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_68_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_chip_acc_3_zone,
    &common_led_c_5_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b760m_aor_elite_ddr4_gen5_5711_device =
{
    &it5711_68_device,
    0x2030015F,
    0,
    "B760M AORUS ELITE DDR4 GEN5",
};

static const gb_fusion2_device b760m_aor_elite_gen5_5711_device =
{
    &it5711_68_device,
    0x2030015F,
    0,
    "B760M AORUS ELITE GEN5",
};

static const gb_fusion2_device b760m_aor_elite_wifi6e_ddr4_gen5_5711_device =
{
    &it5711_68_device,
    0x2030015F,
    0,
    "B760M AORUS ELITE WIFI6E DDR4 GEN5",
};

static const gb_fusion2_device b760m_aor_elite_wifi6e_gen5_5711_device =
{
    &it5711_68_device,
    0x2030015F,
    0,
    "B760M AORUS ELITE WIFI6E GEN5",
};

static const gb_fusion2_device b760m_gmg_x_wifi6e_ddr4_gen5_2_5711_device =
{
    &it5711_68_device,
    0x2030015F,
    0,
    "B760M GAMING X WIFI6E DDR4 GEN5",
};

static const gb_fusion2_device b840m_aor_elite_wifi6e_5711_device =
{
    &it5711_68_device,
    0x102001DF,
    0,
    "B840M AORUS ELITE WIFI6E",
};

static const gb_fusion2_device b850_aor_elite_wifi7_5711_device =
{
    &it5711_68_device,
    0x094001DF,
    0,
    "B850 AORUS ELITE WIFI7",
};

static const gb_fusion2_device b850_aor_elite_wifi7_ice_5711_device =
{
    &it5711_68_device,
    0x094001DF,
    0,
    "B850 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device b850m_aor_elite_5711_device =
{
    &it5711_68_device,
    0x084001DF,
    0,
    "B850M AORUS ELITE",
};

static const gb_fusion2_device b850m_aor_elite_wifi6e_5711_device =
{
    &it5711_68_device,
    0x084001DF,
    0,
    "B850M AORUS ELITE WIFI6E",
};

static const gb_fusion2_device b850m_aor_elite_wifi6e_ice_5711_device =
{
    &it5711_68_device,
    0x084001DF,
    0,
    "B850M AORUS ELITE WIFI6E ICE",
};

static const gb_fusion2_device b850m_aor_elite_wifi6e_ice_p_5711_device =
{
    &it5711_68_device,
    0x084001DF,
    0,
    "B850M AORUS ELITE WIFI7 ICE-P",
};

static const gb_fusion2_device b850m_gmg_x_wifi6e_5711_device =
{
    &it5711_68_device,
    0x084001DF,
    0,
    "B850M GAMING X WIFI6E",
};

static const gb_fusion2_device x870_aor_elite_wifi7_5711_device =
{
    &it5711_68_device,
    0x014001DF,
    0,
    "X870 AORUS ELITE WIFI7",
};

static const gb_fusion2_device x870_aor_elite_wifi7_ice_5711_device =
{
    &it5711_68_device,
    0x014001DF,
    0,
    "X870 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device x870_eagle_wifi7_5711_device =
{
    &it5711_68_device,
    0x014001DF,
    0,
    "X870 EAGLE WIFI7",
};

static const gb_fusion2_device x870_gmg_x_wifi7_5711_device =
{
    &it5711_68_device,
    0x014001DF,
    0,
    "X870 GAMING X WIFI7",
};

static const gb_fusion2_device x870m_aor_elite_wifi7_5711_device =
{
    &it5711_68_device,
    0x004001DF,
    0,
    "X870M AORUS ELITE WIFI7",
};

static const gb_fusion2_device x870m_aor_elite_wifi7_ice_5711_device =
{
    &it5711_68_device,
    0x004001DF,
    0,
    "X870M AORUS ELITE WIFI7 ICE",
};

/*-------------------------------------------------------------*\
|  Layout 69   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "Chipset Accent"                           : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_69_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_led_c_5_zone,
    &common_chip_acc_11_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x870_aor_elite_x3d_ice_5711_device =
{
    &it5711_69_device,
    0x015001DF,
    0,
    "X870 AORUS ELITE X3D ICE",
};

static const gb_fusion2_device x870_aor_stealth_5711_device =
{
    &it5711_69_device,
    0x015001DF,
    0,
    "X870 AORUS STEALTH",
};

static const gb_fusion2_device x870_aor_stealth_ice_5711_device =
{
    &it5711_69_device,
    0x015001DF,
    0,
    "X870 AORUS STEALTH ICE",
};

static const gb_fusion2_device x870e_aor_elite_wifi7_5711_device =
{
    &it5711_69_device,
    0x015001DF,
    0,
    "X870E AORUS ELITE WIFI7",
};

static const gb_fusion2_device x870e_aor_elite_wifi7_ice_5711_device =
{
    &it5711_69_device,
    0x015001DF,
    0,
    "X870E AORUS ELITE WIFI7 ICE",
};

/*-------------------------------------------------------------*\
|  Layout 70   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "Chipset Accent"                           : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_70_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_led_c_5_zone,
    &common_io_cov_10_zone,
    &common_chip_acc_11_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b650e_aor_stealth_ice_5711_device =
{
    &it5711_70_device,
    0x311001DF,
    0,
    "B650E AORUS STEALTH ICE",
};

static const gb_fusion2_device x870e_aero_x3d_wood_5711_device =
{
    &it5711_70_device,
    0x016001DF,
    0,
    "X870E AERO X3D WOOD",
};

static const gb_fusion2_device x870e_aor_elite_x3d_5711_device =
{
    &it5711_70_device,
    0x016001DF,
    0,
    "X870E AORUS ELITE X3D",
};

static const gb_fusion2_device x870e_aor_elite_x3d_ice_5711_device =
{
    &it5711_70_device,
    0x016001DF,
    0,
    "X870E AORUS ELITE X3D ICE",
};

static const gb_fusion2_device x870e_aor_pro_5711_device =
{
    &it5711_70_device,
    0x016001DF,
    0,
    "X870E AORUS PRO",
};

static const gb_fusion2_device x870e_aor_pro_ice_5711_device =
{
    &it5711_70_device,
    0x016001DF,
    0,
    "X870E AORUS PRO ICE",
};

static const gb_fusion2_device x870e_aor_pro_x3d_5711_device =
{
    &it5711_70_device,
    0x016001DF,
    0,
    "X870E AORUS PRO X3D",
};

static const gb_fusion2_device x870e_aor_pro_x3d_ice_5711_device =
{
    &it5711_70_device,
    0x016001DF,
    0,
    "X870E AORUS PRO X3D ICE",
};

/*-------------------------------------------------------------*\
|  Layout 71   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C"                                    : Single   |
|    Zone "IO Cover"                                 : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_71_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_chip_acc_3_zone,
    &common_led_c_5_zone,
    &common_io_cov_10_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device b850m_aor_pro_wifi7_5711_device =
{
    &it5711_71_device,
    0x085001DF,
    0,
    "B850M AORUS PRO WIFI7",
};

static const gb_fusion2_device b860_aor_elite_wifi7_ice_5711_device =
{
    &it5711_71_device,
    0x096001DF,
    0,
    "B860 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device b860_eagle_wifi6e_5711_device =
{
    &it5711_71_device,
    0x096001DF,
    0,
    "B860 EAGLE WIFI6E",
};

static const gb_fusion2_device b860_gmg_x_wifi6e_5711_device =
{
    &it5711_71_device,
    0x096001DF,
    0,
    "B860 GAMING X WIFI6E",
};

static const gb_fusion2_device b860m_aor_elite_5711_device =
{
    &it5711_71_device,
    0x086001DF,
    0,
    "B860M AORUS ELITE",
};

static const gb_fusion2_device b860m_aor_elite_wifi6e_5711_device =
{
    &it5711_71_device,
    0x086001DF,
    0,
    "B860M AORUS ELITE WIFI6E",
};

static const gb_fusion2_device b860m_aor_elite_wifi6e_ice_5711_device =
{
    &it5711_71_device,
    0x086001DF,
    0,
    "B860M AORUS ELITE WIFI6E ICE",
};

static const gb_fusion2_device b860m_aor_pro_wifi7_5711_device =
{
    &it5711_71_device,
    0x086001DF,
    0,
    "B860M AORUS PRO WIFI7",
};

static const gb_fusion2_device z890_aor_elite_wifi7_5711_device =
{
    &it5711_71_device,
    0x013001DF,
    0,
    "Z890 AORUS ELITE WIFI7",
};

static const gb_fusion2_device z890_aor_elite_wifi7_ice_5711_device =
{
    &it5711_71_device,
    0x013001DF,
    0,
    "Z890 AORUS ELITE WIFI7 ICE",
};

static const gb_fusion2_device z890_aor_elite_x_ice_5711_device =
{
    &it5711_71_device,
    0x013001DF,
    0,
    "Z890 AORUS ELITE X ICE",
};

static const gb_fusion2_device z890_aor_pro_ice_5711_device =
{
    &it5711_71_device,
    0x013001DF,
    0,
    "Z890 AORUS PRO ICE",
};

static const gb_fusion2_device z890m_aor_elite_wifi7_5711_device =
{
    &it5711_71_device,
    0x003001DF,
    0,
    "Z890M AORUS ELITE WIFI7",
};

static const gb_fusion2_device z890m_aor_elite_wifi7_ice_5711_device =
{
    &it5711_71_device,
    0x003001DF,
    0,
    "Z890M AORUS ELITE WIFI7 ICE",
};

/*-------------------------------------------------------------*\
|  Layout 72   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "ARGB_V2_4"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "Chipset Accent"                           : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_72_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_argb_v2_4_zone,
    &common_led_c_5_zone,
    &common_io_cov_10_zone,
    &common_chip_acc_11_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x870e_aor_mstr_5711_device =
{
    &it5711_72_device,
    0x017001DF,
    0,
    "X870E AORUS MASTER",
};

static const gb_fusion2_device x870e_aor_mstr_x3d_5711_device =
{
    &it5711_72_device,
    0x017001DF,
    0,
    "X870E AORUS MASTER X3D",
};

static const gb_fusion2_device x870e_aor_mstr_x3d_ice_5711_device =
{
    &it5711_72_device,
    0x017001DF,
    0,
    "X870E AORUS MASTER X3D ICE",
};

static const gb_fusion2_device z890_aor_mstr_ai_top_5711_device =
{
    &it5711_72_device,
    0x026001DF,
    0,
    "Z890 AORUS MASTER AI TOP",
};

/*-------------------------------------------------------------*\
|  Layout 73   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "ARGB_V2_4"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "WIFI Antenna"                             : Single   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "Chipset Accent"                           : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_73_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_argb_v2_4_zone,
    &common_led_c_5_zone,
    &common_wifi_ant_9_zone,
    &common_io_cov_10_zone,
    &common_chip_acc_11_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x870e_aor_xtrm_ai_top_5711_device =
{
    &it5711_73_device,
    0x028001DF,
    0,
    "X870E AORUS XTREME AI TOP",
};

/*-------------------------------------------------------------*\
|  Layout 74   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "Game On LED"                              : Single   |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "LED_C"                                    : Single   |
|    Zone "IO Cover"                                 : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_74_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_game_on_1_zone,
    &common_chip_acc_3_zone,
    &common_led_c_5_zone,
    &common_io_cov_10_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z890_eagle_5711_device =
{
    &it5711_74_device,
    0x014001DF,
    0,
    "Z890 EAGLE",
};

static const gb_fusion2_device z890_eagle_wifi7_5711_device =
{
    &it5711_74_device,
    0x014001DF,
    0,
    "Z890 EAGLE WIFI7",
};

static const gb_fusion2_device z890_gmg_x_wifi7_5711_device =
{
    &it5711_74_device,
    0x014001DF,
    0,
    "Z890 GAMING X WIFI7",
};

/*-------------------------------------------------------------*\
|  Layout 75   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "ARGB_V2_4"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "WIFI Antenna"                             : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_75_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_argb_v2_4_zone,
    &common_led_c_5_zone,
    &common_wifi_ant_11_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x870e_aor_xtrm_x3d_ai_top_5711_device =
{
    &it5711_75_device,
    0x029001DF,
    0,
    "X870E AORUS XTREME X3D AI TOP",
};

/*-------------------------------------------------------------*\
|  Layout 76   048D:5711                                        |
|                                                               |
|    Zone "ARGB_V2_1"                                : Linear   |
|    Zone "ARGB_V2_2"                                : Linear   |
|    Zone "ARGB_V2_3"                                : Linear   |
|    Zone "ARGB_V2_4"                                : Linear   |
|    Zone "LED_C"                                    : Single   |
|    Zone "IO Cover"                                 : Single   |
|    Zone "WIFI Antenna"                             : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_76_device =
{
    &common_argb_v2_1_zone,
    &common_argb_v2_2_zone,
    &common_argb_v2_3_zone,
    &common_argb_v2_4_zone,
    &common_led_c_5_zone,
    &common_io_cov_10_zone,
    &common_wifi_ant_11_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z890_aor_xtrm_ai_top_5711_device =
{
    &it5711_76_device,
    0x027001DF,
    0,
    "Z890 AORUS XTREME AI TOP",
};

/*-------------------------------------------------------------*\
|  Layout 77   048D:5711                                        |
|                                                               |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "RAM Cover"                                : Single   |
|    Zone "SSD Cover"                                : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_77_device =
{
    &common_chip_acc_6_zone,
    &common_ram_cov_7_zone,
    &common_ssd_cov_8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device x870e_aor_xtrm_ai_x3d_top_5711_device =
{
    &it5711_77_device,
    0x029001DF,
    1,
    "X870E AORUS XTREME X3D AI TOP",
};

/*-------------------------------------------------------------*\
|  Layout 78   048D:5711                                        |
|                                                               |
|    Zone "Chipset Accent"                           : Single   |
|    Zone "SSD Cover"                                : Single   |
|    Zone "RAM Cover"                                : Single   |
\*-------------------------------------------------------------*/
static gb_fusion2_layout it5711_78_device =
{
    &common_chip_acc_6_zone,
    &common_ssd_cov_7_zone,
    &common_ram_cov_8_zone,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
};

static const gb_fusion2_device z890_aor_xtrm_ai_top_2_5711_device =
{
    &it5711_78_device,
    0x027001DF,
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
    &generic_it8297_device,
    &generic_it8950_device,
    &generic_it5711_device,

/*-----------------------------------------------------------------*\
|  IT8297 Devices                                                   |
\*-----------------------------------------------------------------*/
    &b450_gmg_x_device,
    &b450m_ds3h_wifi_8297_device,
    &trx40_aor_designare_device,
    &trx40_aor_master_device,
    &trx40_aor_pro_wifi_device,
    &trx40_aor_xtrm_2_device,
    &trx40_aor_xtrm_device,
    &x570_aor_elite_device,
    &x570_aor_elite_wifi_device,
    &x570_aor_mstr_device,
    &x570_aor_pro_device,
    &x570_aor_ultra_device,
    &x570_aor_xtrm_device,
    &x570_i_aor_pro_wifi_device,
    &z390_aor_mstr_device,
    &z390_aor_mstr_g2_device,
    &z390_aor_pro_device,
    &z390_aor_pro_wifi_device,
    &z390_aor_ultra_device,
    &z390_aor_xtrm_device,
    &z390_aor_xtrm_wtr_force_5g_device,
    &z390_aor_xtrm_wtr_force_device,
    &z390_i_aor_pro_wifi_device,

/*-----------------------------------------------------------------*\
|  IT8950 Devices                                                   |
\*-----------------------------------------------------------------*/
    &h810m_gmg_wifi6_device,
    &h810m_h_device,
    &h810m_s2h_device,

/*-----------------------------------------------------------------*\
|  IT8950 + Super I/O Hybrid Devices                                |
\*-----------------------------------------------------------------*/
    &b860m_d_device,
    &b860m_d3hp_device,
    &b860m_e_device,
    &b860m_h_device,
    &b860m_k_device,
    &z890m_gmg_x_device,

/*-----------------------------------------------------------------*\
|  IT5702 Devices                                                   |
\*-----------------------------------------------------------------*/
    &a520_aor_elite_device,
    &a520i_ac_device,
    &a520m_ds3h_ac_device,
    &a520m_ds3h_device,
    &a520m_h_device,
    &a520m_s2h_device,
    &a620i_ax_device,
    &a620m_c_device,
    &a620m_ds3h_device,
    &a620m_ds3h_2_device,
    &a620m_gmg_x_ax_device,
    &a620m_gmg_x_ax_2_device,
    &a620m_gmg_x_device,
    &a620m_gmg_x_2_device,
    &a620m_h_device,
    &a620m_h_2_device,
    &a620m_s2h_device,
    &a620m_s2h_2_device,
    &b450m_ds3h_v3_device,
    &b450m_ds3h_wifi_device,
    &b460_aor_pro_ac_device,
    &b460m_aor_elite_device,
    &b460m_aor_pro_device,
    &b460m_ds3h_ac_device,
    &b460m_ds3h_v2_device,
    &b550_aor_elite_ax_device,
    &b550_aor_elite_ax_v2_device,
    &b550_aor_elite_ax_v3_device,
    &b550_aor_elite_device,
    &b550_aor_elite_v2_device,
    &b550_aor_mstr_device,
    &b550_aor_pro_ac_device,
    &b550_aor_pro_ax_device,
    &b550_aor_pro_device,
    &b550_aor_pro_v2_device,
    &b550_eagle_device,
    &b550_eagle_wifi6_device,
    &b550_gmg_x_device,
    &b550_gmg_x_v2_device,
    &b550_vision_d_device,
    &b550_vision_dp_device,
    &b550i_aor_pro_ax_device,
    &b550m_aor_elite_ax_device,
    &b550m_aor_elite_device,
    &b550m_aor_pro_ax_device,
    &b550m_aor_pro_device,
    &b550m_aor_pro_p_device,
    &b550m_ds3h_ac_device,
    &b550m_ds3h_ac_r2_device,
    &b550m_ds3h_device,
    &b550m_ds3h_r2_device,
    &b550m_gmg_device,
    &b550m_gmg_x_wifi6_device,
    &b550m_h_device,
    &b550m_s2h_device,
    &b560_hd3_device,
    &b560i_aor_pro_ax_device,
    &b560m_aor_elite_device,
    &b560m_aor_pro_ax_device,
    &b560m_aor_pro_device,
    &b560m_d2v_device,
    &b560m_d3h_device,
    &b560m_ds3h_ac_device,
    &b560m_ds3h_device,
    &b560m_ds3h_plus_device,
    &b560m_gmg_hd_device,
    &b560m_h_device,
    &b560m_pwr_device,
    &b650_aero_g_device,
    &b650_aor_elite_ax_device,
    &b650_aor_elite_ax_ice_device,
    &b650_aor_elite_ax_v2_device,
    &b650_aor_elite_device,
    &b650_aor_elite_v2_device,
    &b650_aor_elite_x_ax_ice_device,
    &b650_aor_pro_ax_device,
    &b650_eagle_ax_device,
    &b650_eagle_device,
    &b650_gmg_x_ax_device,
    &b650_gmg_x_ax_v2_device,
    &b650_gmg_x_device,
    &b650_ud_ac_device,
    &b650_ud_ax_device,
    &b650e_aor_mstr_device,
    &b650e_aor_pro_x_usb4_device,
    &b650e_tachyon_device,
    &b650i_aor_ultra_device,
    &b650i_ax_device,
    &b650m_aor_elite_ax_device,
    &b650m_aor_elite_ax_ice_device,
    &b650m_aor_elite_device,
    &b650m_aor_pro_ax_device,
    &b650m_c_device,
    &b650m_c_v2_device,
    &b650m_c_v3_device,
    &b650m_d2h_ddr4_device,
    &b650m_d2h_device,
    &b650m_d2hp_device,
    &b650m_d3h_ddr4_device,
    &b650m_d3hp_ax_device,
    &b650m_d3hp_device,
    &b650m_ds3h_ax_ddr4_device,
    &b650m_ds3h_ddr4_device,
    &b650m_ds3h_device,
    &b650m_ds3h_2_device,
    &b650m_gmg_ac_ddr4_device,
    &b650m_gmg_ac_device,
    &b650m_gmg_ddr4_device,
    &b650m_gmg_plus_wifi_device,
    &b650m_gmg_wifi_device,
    &b650m_gmg_wifi6e_device,
    &b650m_gmg_x_ax_device,
    &b650m_gmg_x_ax_2_device,
    &b650m_h_device,
    &b650m_k_device,
    &b650m_k_2_device,
    &b650m_pwr_ddr4_device,
    &b650m_s2h_device,
    &b660_aor_elite_ax_ddr4_device,
    &b660_aor_elite_ddr4_device,
    &b660_aor_mstr_ddr4_device,
    &b660_aor_mstr_device,
    &b660_ds3h_ac_ddr4_device,
    &b660_ds3h_ac_device,
    &b660_ds3h_ax_ddr4_device,
    &b660_ds3h_ddr4_device,
    &b660_gmg_x_ax_ddr4_device,
    &b660_gmg_x_ddr4_device,
    &b660_gmg_x_device,
    &b660i_aor_pro_ddr4_device,
    &b660m_aor_elite_ax_ddr4_device,
    &b660m_aor_elite_ddr4_device,
    &b660m_aor_pro_ax_ddr4_device,
    &b660m_aor_pro_ax_device,
    &b660m_aor_pro_ddr4_device,
    &b660m_aor_pro_device,
    &b660m_gmg_x_ax_ddr4_device,
    &b660m_gmg_x_ax_device,
    &b660m_gmg_x_ddr4_device,
    &b660m_gmg_x_device,
    &b690i_aor_ultra_ddr4_device,
    &b690i_aor_ultra_device,
    &b690i_aor_ultra_lite_ddr4_device,
    &b690i_aor_ultra_lite_device,
    &b690i_aor_ultra_plus_ddr4_device,
    &b690i_aor_ultra_plus_device,
    &b760_aor_elite_ax_ddr4_device,
    &b760_aor_elite_ax_device,
    &b760_aor_elite_ddr4_device,
    &b760_aor_elite_device,
    &b760_aor_mstr_ddr4_device,
    &b760_ds3h_ac_ddr4_device,
    &b760_ds3h_ac_device,
    &b760_ds3h_ax_ddr4_device,
    &b760_ds3h_ax_device,
    &b760_ds3h_ax_v2_device,
    &b760_ds3h_ddr4_device,
    &b760_ds3h_device,
    &b760_gmg_x_ax_ddr4_device,
    &b760_gmg_x_ax_device,
    &b760_gmg_x_ddr4_device,
    &b760_gmg_x_device,
    &b760i_aor_pro_ddr4_device,
    &b760i_aor_pro_device,
    &b760m_aor_elite_ax_ddr4_device,
    &b760m_aor_elite_ax_device,
    &b760m_aor_elite_ddr4_device,
    &b760m_aor_elite_device,
    &b760m_aor_elite_x_ax_device,
    &b760m_aor_pro_ax_ddr4_device,
    &b760m_aor_pro_ax_device,
    &b760m_aor_pro_ddr4_device,
    &b760m_aor_pro_device,
    &b760m_c_device,
    &b760m_c_v2_device,
    &b760m_d2h_ddr4_device,
    &b760m_d2h_device,
    &b760m_d3h_ddr4_device,
    &b760m_d3h_device,
    &b760m_d3hp_ddr4_device,
    &b760m_d3hp_device,
    &b760m_d3hp_wifi6_device,
    &b760m_ds3h_ax_ddr4_device,
    &b760m_ds3h_ax_device,
    &b760m_ds3h_ddr4_device,
    &b760m_ds3h_device,
    &b760m_gmg_ac_ddr4_device,
    &b760m_gmg_ac_device,
    &b760m_gmg_ddr4_device,
    &b760m_gmg_device,
    &b760m_gmg_plus_wifi_ddr4_device,
    &b760m_gmg_wifi_device,
    &b760m_gmg_wifi_plus_device,
    &b760m_gmg_x_ax_ddr4_device,
    &b760m_gmg_x_ax_device,
    &b760m_gmg_x_ddr4_device,
    &b760m_gmg_x_device,
    &b760m_h_v2_device,
    &b760m_pwr_ddr4_device,
    &b760m_pwr_device,
    &b790i_aor_ultra_device,
    &h470_aor_pro_ax_device,
    &h470_hd3_device,
    &h490i_aor_pro_ax_device,
    &h490m_ds3h_device,
    &h610m_d3h_ddr4_device,
    &h610m_d3h_wifi_ddr4_device,
    &h610m_d3w_device,
    &h610m_d3w_wifi6_device,
    &h610m_gmg_wifi_ddr4_device,
    &trx50_aero_d_device,
    &x570s_aero_g_device,
    &x570s_aor_mstr_device,
    &x570s_aor_pro_ax_device,
    &x570si_aor_pro_ax_device,
    &x670_aor_elite_ax_device,
    &x670_aor_xtrm_2_device,
    &x670_gmg_x_ax_device,
    &x670_gmg_x_ax_v2_device,
    &x670e_aor_mstr_device,
    &x670e_aor_pro_x_device,
    &x670e_aor_xtrm_device,
    &z490_aor_elite_ac_device,
    &z490_aor_elite_device,
    &z490_aor_mstr_device,
    &z490_aor_mstr_waterforce_device,
    &z490_aor_pro_ax_device,
    &z490_aor_ultra_device,
    &z490_aor_ultra_g2_device,
    &z490_aor_xtrm_2_device,
    &z490_aor_xtrm_device,
    &z490_aor_xtrm_waterforce_2_device,
    &z490_aor_xtrm_waterforce_device,
    &z490_gmg_x_ax_device,
    &z490_gmg_x_device,
    &z490_ud_ac_device,
    &z490_ud_device,
    &z490_vision_d_device,
    &z490_vision_g_device,
    &z490i_aor_ultra_device,
    &z490m_device,
    &z490m_gmg_x_device,
    &z590_aor_elite_ax_device,
    &z590_aor_elite_device,
    &z590_aor_mstr_device,
    &z590_aor_pro_ax_device,
    &z590_aor_tachyon_device,
    &z590_aor_ultra_device,
    &z590_aor_xtrm_device,
    &z590_aor_xtrm_waterforce_2_device,
    &z590_aor_xtrm_waterforce_device,
    &z590_d_device,
    &z590_gmg_x_device,
    &z590_ud_ac_device,
    &z590_ud_device,
    &z590_vision_d_device,
    &z590_vision_g_device,
    &z590i_aor_ultra_device,
    &z590i_vis_d_device,
    &z590m_device,
    &z590m_gmg_x_device,
    &z690_aero_d_device,
    &z690_aero_g_ddr4_device,
    &z690_aero_g_device,
    &z690_aor_elite_ax_ddr4_device,
    &z690_aor_elite_ax_ddr4_v2_device,
    &z690_aor_elite_ax_device,
    &z690_aor_elite_ax_2_device,
    &z690_aor_elite_ddr4_device,
    &z690_aor_elite_device,
    &z690_aor_mstr_device,
    &z690_aor_pro_ddr4_device,
    &z690_aor_pro_device,
    &z690_aor_tachyon_device,
    &z690_aor_ultra_device,
    &z690_aor_xtrm_2_device,
    &z690_aor_xtrm_device,
    &z690_aor_xtrm_waterforce_2_device,
    &z690_aor_xtrm_waterforce_device,
    &z690_gmg_x_ddr4_device,
    &z690_gmg_x_ddr4_v2_device,
    &z690_gmg_x_device,
    &z690_ud_ac_device,
    &z690_ud_ax_ddr4_device,
    &z690_ud_ax_ddr4_v2_device,
    &z690_ud_ax_device,
    &z690_ud_ax_v2_device,
    &z690_ud_ddr4_device,
    &z690_ud_ddr4_v2_device,
    &z690_ud_device,
    &z690m_aor_elite_ax_ddr4_device,
    &z690m_ds3h_ddr4_device,
    &z790_aero_g_device,
    &z790_aor_elite_ax_ddr4_device,
    &z790_aor_elite_ax_device,
    &z790_aor_elite_ax_ice_device,
    &z790_aor_elite_ax_w_device,
    &z790_aor_elite_ddr4_device,
    &z790_aor_elite_device,
    &z790_aor_elite_x_ax_device,
    &z790_aor_elite_x_device,
    &z790_aor_elite_x_wifi7_device,
    &z790_aor_mstr_2_device,
    &z790_aor_mstr_device,
    &z790_aor_mstr_x_2_device,
    &z790_aor_mstr_x_device,
    &z790_aor_pro_x_device,
    &z790_aor_pro_x_wifi7_device,
    &z790_aor_tachyon_device,
    &z790_aor_tachyon_x_device,
    &z790_aor_xtreme_x_device,
    &z790_aor_xtrm_2_device,
    &z790_aor_xtrm_device,
    &z790_aor_xtrm_x_2_device,
    &z790_d_ac_device,
    &z790_d_ax_device,
    &z790_d_ddr4_device,
    &z790_d_device,
    &z790_d_wifi_device,
    &z790_eagle_ax_device,
    &z790_eagle_device,
    &z790_gmg_plus_ax_device,
    &z790_gmg_x_ax_device,
    &z790_gmg_x_device,
    &z790_s_ddr4_device,
    &z790_s_wifi_ddr4_device,
    &z790_ud_ac_device,
    &z790_ud_ax_device,
    &z790_ud_device,
    &z790m_aor_elite_ax_device,
    &z790m_aor_elite_ax_ice_device,
    &z790m_aor_elite_device,


/*-----------------------------------------------------------------*\
|  IT5711 Devices                                                   |
\*-----------------------------------------------------------------*/
    &a620i_ax_5711_device,
    &a620m_ds3h_5711_device,
    &a620m_gmg_x_5711_device,
    &a620m_h_5711_device,
    &a620m_s2h_5711_device,
    &b650e_aor_stealth_ice_5711_device,
    &b650e_eagle_wifi6e_5711_device,
    &b650em_c_5711_device,
    &b650em_ds3h_wifi6e_5711_device,
    &b650em_force_wifi6e_5711_device,
    &b760_ds3h_gen5_5711_device,
    &b760_ds3h_wifi6e_gen5_5711_device,
    &b760_gmg_x_ddr4_gen5_5711_device,
    &b760_gmg_x_gen5_5711_device,
    &b760_gmg_x_wifi6e_gen5_5711_device,
    &b760m_aor_elite_ddr4_gen5_5711_device,
    &b760m_aor_elite_gen5_5711_device,
    &b760m_aor_elite_wifi6e_ddr4_gen5_5711_device,
    &b760m_aor_elite_wifi6e_gen5_5711_device,
    &b760m_c_v3_5711_device,
    &b760m_ds3h_ddr4_gen5_5711_device,
    &b760m_ds3h_gen5_5711_device,
    &b760m_ds3h_wifi6e_ddr4_gen5_5711_device,
    &b760m_ds3h_wifi6e_gen5_5711_device,
    &b760m_gmg_ac_ddr4_gen5_5711_device,
    &b760m_gmg_wifi6_plus_gen5_5711_device,
    &b760m_gmg_wifi6e_gen5_5711_device,
    &b760m_gmg_x_ddr4_gen5_5711_device,
    &b760m_gmg_x_gen5_5711_device,
    &b760m_gmg_x_wifi6e_ddr4_gen5_5711_device,
    &b760m_gmg_x_wifi6e_ddr4_gen5_2_5711_device,
    &b760m_gmg_x_wifi6e_gen5_5711_device,
    &b840_eagle_wifi6e_5711_device,
    &b840_gmg_x_wifi6e_5711_device,
    &b840m_aor_elite_wifi6e_5711_device,
    &b840m_d2h_5711_device,
    &b840m_ds3h_5711_device,
    &b840m_ds3h_wifi6_5711_device,
    &b840m_eagle_wifi6_5711_device,
    &b840m_h_5711_device,
    &b850_ai_top_5711_device,
    &b850_aor_elite_wifi7_5711_device,
    &b850_aor_elite_wifi7_ice_5711_device,
    &b850_aor_stealth_5711_device,
    &b850_aor_stealth_ice_5711_device,
    &b850_eagle_ice_5711_device,
    &b850_eagle_wifi6e_5711_device,
    &b850_eagle_wifi7_ice_5711_device,
    &b850_gmg_wifi6_5711_device,
    &b850i_aor_pro_5711_device,
    &b850m_aor_elite_5711_device,
    &b850m_aor_elite_wifi6e_5711_device,
    &b850m_aor_elite_wifi6e_ice_5711_device,
    &b850m_aor_elite_wifi6e_ice_p_5711_device,
    &b850m_aor_pro_wifi7_5711_device,
    &b850m_c_5711_device,
    &b850m_d3hp_5711_device,
    &b850m_ds3h_5711_device,
    &b850m_ds3h_ice_5711_device,
    &b850m_eagle_wifi6e_5711_device,
    &b850m_eagle_wifi6e_ice_5711_device,
    &b850m_force_5711_device,
    &b850m_force_v2_5711_device,
    &b850m_force_wifi6e_5711_device,
    &b850m_force_wifi6e_v2_5711_device,
    &b850m_gmg_x_wifi6e_5711_device,
    &b860_aor_elite_wifi7_ice_5711_device,
    &b860_ds3h_5711_device,
    &b860_ds3h_wifi6e_5711_device,
    &b860_eagle_wifi6e_5711_device,
    &b860_gmg_x_wifi6e_5711_device,
    &b860i_aor_pro_ice_5711_device,
    &b860m_aor_elite_5711_device,
    &b860m_aor_elite_wifi6e_5711_device,
    &b860m_aor_elite_wifi6e_ice_5711_device,
    &b860m_aor_pro_wifi7_5711_device,
    &b860m_c_5711_device,
    &b860m_d2h_5711_device,
    &b860m_ds3h_5711_device,
    &b860m_ds3h_wifi6e_5711_device,
    &b860m_eagle_plus_wifi6e_5711_device,
    &b860m_eagle_v2_5711_device,
    &b860m_eagle_wifi6_5711_device,
    &b860m_eagle_wifi6_v2_5711_device,
    &b860m_gmg_wifi6_5711_device,
    &b860m_gmg_x_5711_device,
    &b860m_gmg_x_wifi6e_5711_device,
    &b860m_power_5711_device,
    &trx50_ai_top_5711_device,
    &w790_ai_top_5711_device,
    &w880_ai_top_5711_device,
    &x870_aor_elite_wifi7_5711_device,
    &x870_aor_elite_wifi7_ice_5711_device,
    &x870_aor_elite_x3d_ice_5711_device,
    &x870_aor_stealth_5711_device,
    &x870_aor_stealth_ice_5711_device,
    &x870_aor_tachyon_ice_5711_device,
    &x870_eagle_wifi7_5711_device,
    &x870_gaming_wifi6_5711_device,
    &x870_gmg_x_wifi7_5711_device,
    &x870e_aero_x3d_wood_5711_device,
    &x870e_aor_elite_wifi7_5711_device,
    &x870e_aor_elite_wifi7_ice_5711_device,
    &x870e_aor_elite_x3d_5711_device,
    &x870e_aor_elite_x3d_ice_5711_device,
    &x870e_aor_mstr_5711_device,
    &x870e_aor_mstr_x3d_5711_device,
    &x870e_aor_mstr_x3d_ice_5711_device,
    &x870e_aor_pro_5711_device,
    &x870e_aor_pro_ice_5711_device,
    &x870e_aor_pro_x3d_5711_device,
    &x870e_aor_pro_x3d_ice_5711_device,
    &x870e_aor_xtrm_ai_top_5711_device,
    &x870e_aor_xtrm_ai_x3d_top_5711_device,
    &x870e_aor_xtrm_x3d_ai_top_5711_device,
    &x870i_aor_pro_ice_5711_device,
    &x870m_aor_elite_wifi7_5711_device,
    &x870m_aor_elite_wifi7_ice_5711_device,
    &z890_aero_d_5711_device,
    &z890_aero_g_5711_device,
    &z890_ai_top_5711_device,
    &z890_aor_elite_wifi7_5711_device,
    &z890_aor_elite_wifi7_ice_5711_device,
    &z890_aor_elite_x_ice_5711_device,
    &z890_aor_mstr_ai_top_5711_device,
    &z890_aor_pro_ice_5711_device,
    &z890_aor_tachyon_ice_5711_device,
    &z890_aor_xtrm_ai_top_5711_device,
    &z890_aor_xtrm_ai_top_2_5711_device,
    &z890_eagle_5711_device,
    &z890_eagle_wifi7_5711_device,
    &z890_gmg_x_wifi7_5711_device,
    &z890_ud_5711_device,
    &z890_ud_wifi6e_5711_device,
    &z890i_aor_ultra_5711_device,
    &z890m_aor_elite_wifi7_5711_device,
    &z890m_aor_elite_wifi7_ice_5711_device,
};
const unsigned int GB_FUSION2_DEVICE_COUNT = (sizeof(gb_fusion2_device_list_data) / sizeof(gb_fusion2_device_list_data[ 0 ]));
const gb_fusion2_device** gb_fusion2_device_list = gb_fusion2_device_list_data;


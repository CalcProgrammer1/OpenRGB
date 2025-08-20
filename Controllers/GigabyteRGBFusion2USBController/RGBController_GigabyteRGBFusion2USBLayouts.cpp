/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USBLayouts.cpp            |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   megadjc                                     31 Jul 2025 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "RGBController_GigabyteRGBFusion2USBLayouts.h"
#include "GigabyteRGBFusion2USBController.h"

/*-------------------------------------------------*\
| LedHeaders is a map of the led header addresses   |
\*-------------------------------------------------*/
const FwdLedHeaders LedLookup =
{
    {"LED1",                0},
    {"LED2",                1},
    {"LED3",                2},
    {"LED4",                3},
    {"LED5",                4},
    {"LED6",                5},
    {"LED7",                6},
    {"LED8",                7},
    {"LED9",                8},
    {"LED10",               9},
    {"LED11",              10},
    {"HDR_D_LED1",          5},
    {"HDR_D_LED2",          6},
    {"HDR_D_LED3",          7},
    {"HDR_D_LED4",          8},
    {"HDR_D_LED1_RGB",   0x58},
    {"HDR_D_LED2_RGB",   0x59},
    {"HDR_D_LED3_RGB",   0x62},
    {"HDR_D_LED4_RGB",   0x63},
};

/*-------------------------------------------------*\
| These are the default Custom layouts that will    |
| be written into config if they doesn't exist      |
\*-------------------------------------------------*/
const KnownLayout HardcodedCustom_Gen1 =
{
    {
        "Custom",
        {
            {
                "D_LED2 Top",
                {
                    { "Name for LED Strip 2", HDR_D_LED2, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "Name for LED Strip 1", HDR_D_LED1, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Name for Led 1",     LED1, 1 },
                    { "Name for Led 2",     LED2, 1 },
                    { "Name for Led 3",     LED3, 1 },
                    { "Name for Led 4",     LED4, 1 },
                    { "Name for Led 5",     LED5, 1 },
                    { "Name for Led 8",     LED8, 1 },
                }
            }
        }
    }
};

const KnownLayout HardcodedCustom_Gen2 =
{
    {
        "Custom",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB_V2_4",
                {
                    { "ARGB_V2_4", HDR_D_LED4, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "0x20",                   LED1, 1 },
                    { "0x21",                   LED2, 1 },
                    { "0x22",                   LED3, 1 },
                    { "0x23",                   LED4, 1 },
                    { "0x90",                   LED9, 1 },
                    { "0x91",                   LED10, 1 },
                    { "0x92",                   LED11, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
};


/*-------------------------------------------------*\
| KnownLayoutsLookup contains zone mapping data     |
| for each motherboard.                             |
\*-------------------------------------------------*/
const KnownLayout knownLayoutsLookup =
{
    {
        "STD_ATX",
        {
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Back I/O",   LED1, 1 },
                    { "PCIe",       LED4, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header", LED2, 1 },
                    { "LED_C1/C2",  LED5, 1 },
                }
            }
        }
    },
    {
        "A620M-H",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",           LED2, 1 },
                }
            }
        }
    },
    {
        "B550I-AORUS-PRO-AX",
        {
            {
                "D_LED",
                {
                    { "D_LED", HDR_D_LED1, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Top LED",            LED1, 1 },
                    { "Top Middle LED",     LED2, 1 },
                    { "Bottom Middle LED",  LED3, 1 },
                    { "Bottom LED",         LED4, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",  LED5, 1 },
                }
            }
        }
    },
    {
        "B650-C-V2",
        {
            {
                "D_LED",
                {
                    { "D_LED", HDR_D_LED1, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED2, 1 },
                }
            }
        }
    },
    {
        "B650-D2H",
        {
            {
                "D_LED",
                {
                    { "D_LED", HDR_D_LED1, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B650-Eagle-AX",
        {
            {
                "ARGB_V2_1/V2.3",
                {
                    { "ARGB_V2_1/V2.3", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B650-ELITE",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED4, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",     LED3, 1 },
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "B650-ELITE-V2",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED4, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",     LED5, 1 },
                }
            }
        }
    },
    {
        "B650-PRO",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",     LED3, 1 },
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "B650-UD",
        {
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED1, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B650E-AORUS-STEALTH",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED11, 1 },
                    { "Logo",           LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B650-TACH",
        {
            {
                "D_LED",
                {
                    { "D_LED", HDR_D_LED1, 0 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",      LED3, 1 },
                    { "LED_C",           LED2, 1 },
                }
            }
        }
    },
    {
        "B650-USB4",
        {
            {
                "ARGB_V2_1/V2.3",
                {
                    { "ARGB_V2_1/V2.3",  HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED3, 1 },
                    { "Cover Left",     LED4, 1 },
                    { "Cover Right",    LED1, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",     LED5, 1 },
                }
            }
        }
    },
    {
        "B650M-DS3H",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",           LED5, 1 },
                }
            }
        }
    },
    {
        "B760M-D2H",
        {
            {
                "D_LED",
                {
                    { "D_LED", HDR_D_LED1, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED4, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED2, 1 },
                }
            }
        }
    },
    {
        "B760M-EXAX",
        {
            {
                "ARGB_V2_1/V2.3",
                {
                    { "ARGB_V2_1/V2.3", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Top Logo",           LED4, 1 },
                    { "Top Middle Logo",    LED3, 1 },
                    { "Bottom Middle Logo", LED2, 1 },
                    { "Bottom Logo",        LED1, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B760M-GAX",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED2, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED2, 1 },
                }
            }
        }
    },
    {
        "B760M-DS3H-DR-G5",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED1, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B840M-WIFI6E",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED3, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B840M-DS3H",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B850-AI-TOP",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B850-EGL-WIFI6",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B850-GMX-WIFI6",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED3, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B860-DS3H",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED3, 1 },
                    { "Logo",         LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B860-EGL-WIFI6",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B860-EGL-P-WIFI6",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED1, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B860-WIFI7",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED3, 1 },
                    { "Logo",          LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "B860I-Pro",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "H810M",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", LED3, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", LED4, 0 },
                }
            },
        }
    },
    {
        "MSTR_ATX",
        {
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",     HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Back IO / VRM",  LED7, 0 },
                    { "Chipset Logo",   LED3, 1 },
                    { "PCIe",           LED4, 1 },
                    { "XMP Logo",       LED2, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1/C2",      LED5, 1 },
                }
            }
        }
    },
    {
        "MSTR_ATX_2",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Cover Left",     LED4, 1 },
                    { "Cover Right",    LED1, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",     LED3, 1 },
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "MSTR_ATX_3",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Aorus Logo", LED7, 1 },
                    { "ESS Logo",   LED4, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1/C2",  LED5, 1 },
                }
            }
        }
    },
    {
        "TRX50-AERO-D",
        {
            {
                "ARGB_V2_1/V2.3",
                {
                    { "ARGB_V2_1/V2.3", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED3, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "TRX50-A-TP",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "X570S-ELITE",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Top Right Accents",     LED4, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",     LED3, 1 },
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "X570S-A-MSTR",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",     LED1, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",     LED3, 1 },
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "X570S-PRO-AX",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",           LED1, 1 },
                    { "ESS Logo",       LED4, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",     LED3, 1 },
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "X670-ELITE",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "CPU Header",     LED3, 1 },
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "X670-A-PRO-X",
        {
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Cover Left",     LED4, 1 },
                    { "Cover Right",    LED1, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1",         LED2, 1 },
                    { "LED_C2",         LED5, 1 },
                }
            }
        }
    },
    {
        "X870-WIFI7",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",        LED3, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "X870E-WIFI7",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED11, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            },
        }
    },
    {
        "X870E-PRO",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED11, 1 },
                    { "Logo",           LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "X870E-MSTR",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB_V2_4",
                {
                    { "ARGB_V2_4", HDR_D_LED4, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED11, 1 },
                    { "Logo",           LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "X870E-XTRM-AI-TOP",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",                 LED11, 1 },
                    { "Logo",                   LED10, 1 },
                    { "Wifi Antenna Accent",     LED9, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "X870I-PRO",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z690-ELITE",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Top Right Accent",        LED3, 1 },
                    { "Middle Right Accent",     LED4, 1 },
                    { "Logo",                    LED1, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1",          LED2, 1 },
                    { "LED_C2",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z790-D",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "Z790-PRO-X",
        {
            {
                "ARGB_V2_1/ARGB V 2.3",
                {
                    { "ARGB_V2_1/ARGB V 2.3", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",            LED1, 1 },
                    { "Logo 2",            LED4, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z790-MSTR",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",            LED1, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z790-MSTR-X",
        {
            {
                "ARGB_V2_1/V2.3",
                {
                    { "ARGB_V2_1/V2.3", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",            LED1, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z790-ELITE",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",            LED3, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1",          LED2, 1 },
                    { "LED_C2",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z790-S-DDR4",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1",          LED2, 1 },
                    { "LED_C2",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z790-XTRM",
        {
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",          LED3, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1",          LED2, 1 },
                    { "LED_C2",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z790-XTRM-X",
        {
            {
                "ARGB_V2_1/V2.3",
                {
                    { "ARGB_V2_1/V2.3", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",          LED3, 1 },
                }
            },
            {
                "12V RGB Strips",
                {
                    { "LED_C1",          LED5, 1 },
                    { "LED_C2",          LED4, 1 },
                    { "LED_C3",          LED2, 1 },
                }
            }
        }
    },
    {
        "Z890-WIFI7",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",          LED3, 1 },
                    { "Logo",           LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z890-MSTR",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB_V2_4",
                {
                    { "ARGB_V2_4", HDR_D_LED4, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",          LED3, 1 },
                    { "Logo",           LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z890-MSTR-AI-TOP",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB_V2_4",
                {
                    { "ARGB_V2_4", HDR_D_LED4, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Accent",         LED11, 1 },
                    { "Logo",           LED10, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z890-XTRM-AI-TOP",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB_V2_4",
                {
                    { "ARGB_V2_4", HDR_D_LED4, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",                   LED10, 1 },
                    { "WIFI Antenna Accent",    LED11, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "Z890-A-ULTRA",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Logo",           LED10, 1 },
                }
            }
        }
    },
    {
        "IT5711-Generic",
        {
            {
                "ARGB_V2_1",
                {
                    { "ARGB_V2_1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB_V2_2",
                {
                    { "ARGB_V2_2", HDR_D_LED2, 0 },
                }
            },
            {
                "ARGB_V2_3",
                {
                    { "ARGB_V2_3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB_V2_4",
                {
                    { "ARGB_V2_4", HDR_D_LED4, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "0x20",          LED1, 1 },
                    { "Ox21",          LED2, 1 },
                    { "0x22",          LED3, 1 },
                    { "0x23",          LED4, 1 },
                    { "0x90",          LED9, 1 },
                    { "0x91",          LED10, 1 },
                    { "0x92",          LED11, 1 },
                }
            },
            {
                "12V RGB Strip",
                {
                    { "LED_C",          LED5, 1 },
                }
            }
        }
    },
    {
        "IT8297BX-GBX570",
        {
            {
                "D_LED2 Top",
                {
                    { "Name for LED Strip 2", HDR_D_LED2, 0 },
                }
            },
            {
                "D_LED1 Bottom",
                {
                    { "Name for LED Strip 1", HDR_D_LED1, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "Name for Led 1", LED1, 1 },
                    { "Name for Led 2", LED2, 1 },
                    { "Name for Led 3", LED3, 1 },
                    { "Name for Led 4", LED4, 1 },
                    { "Name for Led 5", LED5, 1 },
                    { "Name for Led 6", LED6, 1 },
                    { "Name for Led 7", LED7, 1 },
                    { "Name for Led 8", LED8, 1 },
                }
            }
        }
    }
};

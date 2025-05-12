/*---------------------------------------------------------*\
| RGBController_GigabyteRGBFusion2USB.cpp                   |
|                                                           |
|   RGBController for Gigabyte Aorus RGB Fusion 2 USB       |
|   motherboard                                             |
|                                                           |
|   jackun                                      08 Jan 2020 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <array>
#include <sstream>
#include "RGBController_GigabyteRGBFusion2USB.h"
#include "ResourceManager.h"
#include "SettingsManager.h"

/*-------------------------------------------------*\
| LedHeaders is a map of the led header addresses   |
\*-------------------------------------------------*/
static FwdLedHeaders LedLookup
{
    {"LED1",   0x20}, {"LED2",   0x21}, {"LED3", 0x22},     {"LED4", 0x23},
    {"LED5",   0x24}, {"LED6",   0x25}, {"LED7", 0x26},     {"LED8", 0x27},
    {"D_LED1", 0x58}, {"D_LED2", 0x59}, {"D_LED3", 0x62},   {"LED9", 0x90},
    {"LED10", 0x91},  {"LED11", 0x92},
};

/*-------------------------------------------------*\
| This is the default knownLayouts structure and    |
| will be written into config if it doesn't exist   |
\*-------------------------------------------------*/
static MBName MBName2LayoutLookup
{
    {"B550 AORUS ELITE",                    "STD_ATX" },
    {"B550 AORUS PRO",                      "STD_ATX" },
    {"B550I AORUS PRO AX",                      "ITX" },
    {"B650 EAGLE AX",                 "B650-Eagle-AX" },
    {"B650E AORUS STEALTH",     "B650E-AORUS-STEALTH" },
    {"B650E AORUS STEALTH ICE", "B650E-AORUS-STEALTH" },
    {"B650M DS3H",                       "B650M-DS3H" },
    {"B850 AORUS ELITE WIFI7",           "X870-WIFI7" },
    {"B850 AORUS ELITE WIFI7 ICE",       "X870-WIFI7" },
    {"X570 AORUS ELITE",                    "STD_ATX" },
    {"X570 AORUS ELITE WIFI",               "STD_ATX" },
    {"X570 AORUS MASTER",                "MSTR_ATX_3" },
    {"X570 AORUS PRO",                      "STD_ATX" },
    {"X570 AORUS PRO WIFI",                 "STD_ATX" },
    {"X570 AORUS ULTRA",                    "STD_ATX" },
    {"X570 I AORUS PRO WIFI",                   "ITX" },
    {"X670E AORUS MASTER",               "MSTR_ATX_2" },
    {"X870 AORUS ELITE WIFI7",           "X870-WIFI7" },
    {"X870 AORUS ELITE WIFI7 ICE",       "X870-WIFI7" },
    {"X870E AORUS ELITE WIFI7",         "X870E-WIFI7" },
    {"X870E AORUS ELITE WIFI7 ICE",     "X870E-WIFI7" },
    {"X870E AORUS PRO",                   "X870E-PRO" },
    {"X870E AORUS PRO ICE",               "X870E-PRO" },
    {"Z390 AORUS MASTER-CF",               "MSTR_ATX" },
    {"Z890 AORUS ELITE WIFI7",           "Z890-WIFI7" },
    {"Z890 AORUS ELITE WIFI7 ICE",       "Z890-WIFI7" }
};

/*-------------------------------------------------*\
| This is the default Custom layout that will be    |
| written into config if it doesn't exist           |
\*-------------------------------------------------*/
static const KnownLayout HardcodedCustom_Gen1
{
    {
        "Custom",
        {
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
            },
            {
                "D_LED1 Bottom",
                {
                    { "Name for LED Strip 1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2 Top",
                {
                    { "Name for LED Strip 2", HDR_D_LED2, 0 },
                }
            }
        }
    }
};

static const KnownLayout HardcodedCustom_Gen2
{
    {
        "Custom",
        {
            {
                "Motherboard",
                {
                    { "0x20",                   LED1, 1 }, //This one should apply everything globally (testing needed).
                    { "0x21",                   LED2, 1 },
                    { "0x22",                   LED3, 1 }, //Confirmed accent Lighting for X870 Aorus Elite Boards.
                    { "0x23",                   LED4, 1 },
                    { "0x24",                   LED5, 1 }, //should be LED_C on most if not all models.
                    { "0x90",                   LED9, 1 },
                    { "0x91",                   LED10, 1 },//Should be logo on pro models.
                    { "0x92",                   LED11, 1 },//Should be accent on pro models.
                }
            },
                {
                "ARGB V2.3",
                {
                    { "ARGB V2.3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
};


/*-------------------------------------------------*\
| KnownLayoutsLookup now needs to be variable to    |
| allow for a custom addition from config           |
\*-------------------------------------------------*/
static KnownLayout knownLayoutsLookup
{
    {
        "STD_ATX",
        {
            {
                "Motherboard",
                {
                    { "Back I/O",   HDR_BACK_IO, 1 },
                    { "CPU Header", HDR_CPU, 1 },
                    { "PCIe",       HDR_PCIE, 1 },
                    { "LED C1/C2",  HDR_LED_C1C2, 1 },
                }
            },
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
            }
        }
    },
    {
        "ITX",
        {
            {
                "Motherboard",
                {
                    { "LED Group0", HDR_BACK_IO, 1 },
                    { "LED Group1", HDR_CPU, 1 },
                    { "LED Group2", HDR_LED_2, 1 },
                    { "LED Group3", HDR_PCIE, 1 },
                    { "LED C1/C2",  HDR_LED_C1C2, 1 },
                }
            },
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            }
        }
    },
    {
        "B650-Eagle-AX",
        {
            {
                "Motherboard",
                {
                    { "LED_C",          LED5, 1 },
                }
            },
            {
                "ARGB V2.3",
                {
                    { "ARGB V2.3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "B650E-AORUS-STEALTH",
        {
            {
                "Motherboard",
                {
                    { "Logo",           LED10, 1 },
                    { "Accent",         LED11, 1 },
                    { "LED C",           LED5, 1 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "B650M-DS3H",
        {
            {
                "Motherboard",
                {
                    { "LED_C",           LED5, 1 },
                }
            },
            {
                "D_LED1",
                {
                    { "D_LED1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2",
                {
                    { "D_LED2", HDR_D_LED1, 0 },
                }
            }
        }
    },
    {
        "X870-WIFI7",
        {
            {
                "Motherboard",
                {
                    { "Accent",        LED3, 1 },
                    { "LED C",         LED5, 1 },
                }
            },
            {
                "ARGB V2.3",
                {
                    { "ARGB V2.3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "X870E-WIFI7",
        {
            {
                "Motherboard",
                {
                    { "Accent",         LED11, 1 },
                    { "LED C",           LED5, 1 },
                }
            },
            {
                "ARGB V2.3",
                {
                    { "ARGB V2.3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "X870E-PRO",
        {
            {
                "Motherboard",
                {
                    { "Logo",           LED10, 1 },
                    { "Accent",         LED11, 1 },
                    { "LED C",           LED5, 1 },
                }
            },
            {
                "ARGB V2.3",
                {
                    { "ARGB V2.3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "MSTR_ATX_2",
        {
            {
                "D_LED1 Bottom",
                {
                    { "D_LED1 Bottom", HDR_D_LED2, 0 },
                }
            },
            {
                "D_LED2 Top",
                {
                    { "D_LED2 Top",  HDR_D_LED1, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "LED C1",         LED2, 1 },
                    { "LED C2",         LED5, 1 },
                    { "CPU Header",     LED3, 1 },
                    { "Cover Left",     LED4, 1 },
                    { "Cover Right",    LED1, 1 },
                }
            }
        }
    },
    {
        "MSTR_ATX_3",
        {
            {
                "Digital Headers",
                {
                    { "D_LED1 / D_LED2", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB Strip",
                {
                    { "LED C1/C2",  LED5, 1 },
                }
            },
            {
                "Motherboard",
                {
                    { "Aorus Logo", LED7, 1 },
                    { "ESS Logo",   LED4, 1 },
                }
            }
        }
    },
    {
        "MSTR_ATX",
        {
            {
                "Digital Headers",
                {
                    { "D_LED1 / D_LED2", LED6, 0 },
                }
            },
            {
                "ARGB Strip",
                {
                    { "Back IO / VRM",  LED7, 0 },
                }
            },
            {
                "Motherboard",
                {
                    { "XMP Logo",       LED2, 1 },
                    { "Chipset Logo",   LED3, 1 },
                    { "PCIe",           LED4, 1 },
                    { "LED C1/C2",      LED5, 1 },
                }
            }
        }
    },
    {
        "Z890-WIFI7",
        {
            {
                "Motherboard",
                {
                    { "Logo",           LED10, 1 },
                    { "Accent",          LED3, 1 },
                    { "LED C",           LED5, 1 },
                }
            },
            {
                "ARGB V2.3",
                {
                    { "ARGB V2.3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "IT5711-Generic",
        {
            {
                "Motherboard",
                {
                    { "0x20",          LED1, 1 },
                    { "Ox21",          LED2, 1 },
                    { "0x22",          LED3, 1 },
                    { "0x23",          LED4, 1 },
                    { "0x24",          LED5, 1 },
                    { "0x90",          LED9, 1 },
                    { "0x91",          LED10, 1 },
                    { "0x92",          LED11, 1 },
                }
            },
            {
                "ARGB V2.3",
                {
                    { "ARGB V2.3", HDR_D_LED3, 0 },
                }
            },
            {
                "ARGB V2.1",
                {
                    { "ARGB V2.1", HDR_D_LED1, 0 },
                }
            },
            {
                "ARGB V2.2",
                {
                    { "ARGB V2.2", HDR_D_LED2, 0 },
                }
            }
        }
    },
    {
        "IT8297BX-GBX570",
        {
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
            },
            {
                "D_LED1 Bottom",
                {
                    { "Name for LED Strip 1", HDR_D_LED1, 0 },
                }
            },
            {
                "D_LED2 Top",
                {
                    { "Name for LED Strip 2", HDR_D_LED2, 0 },
                }
            }
        }
    }
};


/**------------------------------------------------------------------*\
    @name Gigabyte RGB Fusion 2 USB
    @category Motherboard
    @type USB
    @save :x:
    @direct :white_check_mark:
    @effects :white_check_mark:
    @detectors DetectGigabyteRGBFusion2USBControllers
    @comment The Fusion 2 USB controller applies to most AMD and
        Intel mainboards from the x570 and z390 chipsets onwards.
\*-------------------------------------------------------------------*/

RGBController_RGBFusion2USB::RGBController_RGBFusion2USB(RGBFusion2USBController* controller_ptr, std::string detector)
{
    controller              = controller_ptr;

    name                    = controller->GetDeviceName();
    detector_name           = detector;
    vendor                  = "Gigabyte";
    type                    = DEVICE_TYPE_MOTHERBOARD;
    description             = controller->GetDeviceDescription();
    version                 = controller->GetFWVersion();
    location                = controller->GetDeviceLocation();
    serial                  = controller->GetSerial();

    mode Direct;
    Direct.name             = "Direct";
    Direct.value            = 0xFFFF;
    Direct.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.brightness_min   = RGBFUSION2_BRIGHTNESS_MIN;
    Direct.brightness_max   = RGBFUSION2_BRIGHTNESS_MAX;
    Direct.brightness       = RGBFUSION2_BRIGHTNESS_MAX;
    Direct.color_mode       = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name                 = "Static";
    Static.value                = EFFECT_STATIC;
    Static.flags                = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Static.brightness_min       = RGBFUSION2_BRIGHTNESS_MIN;
    Static.brightness_max       = RGBFUSION2_BRIGHTNESS_MAX;
    Static.brightness           = RGBFUSION2_BRIGHTNESS_MAX;
    Static.colors_min           = 1;
    Static.colors_max           = 1;
    Static.color_mode           = MODE_COLORS_MODE_SPECIFIC;
    Static.colors.resize(1);
    modes.push_back(Static);

    mode Breathing;
    Breathing.name              = "Breathing";
    Breathing.value             = EFFECT_PULSE;
    Breathing.flags             = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Breathing.brightness_min    = RGBFUSION2_BRIGHTNESS_MIN;
    Breathing.brightness_max    = RGBFUSION2_BRIGHTNESS_MAX;
    Breathing.brightness        = RGBFUSION2_BRIGHTNESS_MAX;
    Breathing.speed_min         = 0;
    Breathing.speed_max         = 4;
    Breathing.colors_min        = 1;
    Breathing.colors_max        = 1;
    Breathing.color_mode        = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.speed             = 2;
    modes.push_back(Breathing);

    mode Blinking;
    Blinking.name               = "Blinking";
    Blinking.value              = EFFECT_BLINKING;
    Blinking.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Blinking.brightness_min     = RGBFUSION2_BRIGHTNESS_MIN;
    Blinking.brightness_max     = RGBFUSION2_BRIGHTNESS_MAX;
    Blinking.brightness         = RGBFUSION2_BRIGHTNESS_MAX;
    Blinking.speed_min          = 0;
    Blinking.speed_max          = 4;
    Blinking.colors_min         = 1;
    Blinking.colors_max         = 1;
    Blinking.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Blinking.colors.resize(1);
    Blinking.speed              = 2;
    modes.push_back(Blinking);

    mode ColorCycle;
    ColorCycle.name             = "Color Cycle";
    ColorCycle.value            = EFFECT_COLORCYCLE;
    ColorCycle.flags            = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED;
    ColorCycle.brightness_min   = RGBFUSION2_BRIGHTNESS_MIN;
    ColorCycle.brightness_max   = RGBFUSION2_BRIGHTNESS_MAX;
    ColorCycle.brightness       = RGBFUSION2_BRIGHTNESS_MAX;
    ColorCycle.speed_min        = 0;
    ColorCycle.speed_max        = 4;
    ColorCycle.color_mode       = MODE_COLORS_NONE;
    ColorCycle.speed            = 2;
    modes.push_back(ColorCycle);

    mode Flashing;
    Flashing.name               = "Flashing";
    Flashing.value              = 10;
    Flashing.flags              = MODE_FLAG_HAS_BRIGHTNESS | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_RANDOM_COLOR;
    Flashing.brightness_min     = RGBFUSION2_BRIGHTNESS_MIN;
    Flashing.brightness_max     = RGBFUSION2_BRIGHTNESS_MAX;
    Flashing.brightness         = RGBFUSION2_BRIGHTNESS_MAX;
    Flashing.speed_min          = 0;
    Flashing.speed_max          = 4;
    Flashing.colors_min         = 1;
    Flashing.colors_max         = 1;
    Flashing.color_mode         = MODE_COLORS_MODE_SPECIFIC;
    Flashing.colors.resize(1);
    Flashing.speed              = 2;
    modes.push_back(Flashing);

    Load_Device_Config();
    Init_Controller();
    SetupZones();
}

RGBController_RGBFusion2USB::~RGBController_RGBFusion2USB()
{
    delete controller;
}

void RGBController_RGBFusion2USB::Load_Device_Config()
{
    const std::string SectionLayout     = "MotherboardLayouts";
    const std::string SectionCustom     = "CustomLayout";
    SettingsManager* settings_manager   = ResourceManager::get()->GetSettingsManager();
    json device_settings                = settings_manager->GetSettings(detector_name);
    RvrseLedHeaders ReverseLedLookup    = reverse_map(LedLookup);

    /*-------------------------------------------------*\
    | Get Layouts from the settings manager             |
    | If MotherboardLayouts is not found then write it  |
    | to settings                                       |
    \*-------------------------------------------------*/
    if(!device_settings.contains(SectionLayout))
    {
        device_settings[SectionLayout] = MBName2LayoutLookup;
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
        MBName2Layout = MBName2LayoutLookup;
    }
    else
    {
        for(const nlohmann::detail::iteration_proxy_value<nlohmann::detail::iter_impl<nlohmann::json>>& it : device_settings[SectionLayout].items())
        {
            MBName2Layout.insert( std::pair<std::string, std::string>(it.key(), it.value() ));
        }
    }

    /*-------------------------------------------------*\
    | Get Custom Layout from the settings manager.      |
    | Selects appropriate layout based on controller.   |
    \*-------------------------------------------------*/
    uint16_t pid = controller->GetProductID();

    if(pid == 0x5711)
    {
        layout = HardcodedCustom_Gen2.find("Custom")->second;
    }
    else
    {
        layout = HardcodedCustom_Gen1.find("Custom")->second;
    }

    if (!device_settings.contains(SectionCustom))
    {
        /*---------------------------------------------*\
        | If the Custom layout is not found then write  |
        | it to settings                                |
        \*---------------------------------------------*/
        json json_HCL;

        for(ZoneLeds::iterator zl = layout.begin(); zl != layout.end(); zl++)
        {
            std::vector<LedPort> v_lp = zl->second;
            json json_zl;

            for(std::vector<LedPort>::iterator lp_it = v_lp.begin(); lp_it != v_lp.end(); lp_it++)
            {
                json json_lp;
                json_lp["name"]     = lp_it[0].name;
                json_lp["header"]   = ReverseLedLookup.find(lp_it[0].header)->second;
                json_zl.push_back(json_lp);
            }
            json_HCL.emplace(zl->first, json_zl);
        }

        device_settings[SectionCustom]["Enabled"] = false;
        device_settings[SectionCustom]["Data"] = json_HCL;
        settings_manager->SetSettings(detector_name, device_settings);
        settings_manager->SaveSettings();
    }
    else
    {
        custom_layout = device_settings[SectionCustom]["Enabled"];

        /*---------------------------------------------*\
        | If the Custom layout is found and enabled     |
        | then read it in from config                   |
        \*---------------------------------------------*/
        if(custom_layout)
        {
            json json_HCL = device_settings[SectionCustom]["Data"];
            layout.clear();

            for(const nlohmann::detail::iteration_proxy_value<nlohmann::detail::iter_impl<nlohmann::json>>& json_layout_it : json_HCL.items())
            {
                json json_zl = json_layout_it.value();
                std::vector<LedPort> v_lp;

                for(json& zl : json_zl)
                {
                    json json_vlp = zl;

                    LedPort lp;

                    /*---------------------------------*\
                    | Initialize the name, header, and  |
                    | count values.  Set the D_LED      |
                    | headers LED count to 0            |
                    \*---------------------------------*/
                    lp.name         = json_vlp["name"].get<std::string>();
                    lp.header       = LedLookup.find(json_vlp["header"].get<std::string>())->second;
                    if(lp.header == HDR_D_LED1 || lp.header == HDR_D_LED2 || lp.header == HDR_D_LED3)
                        lp.count = 0;
                    else
                        lp.count = 1;
                    v_lp.push_back(lp);
                }

                layout.insert(std::pair<std::string,std::vector<LedPort>>(json_layout_it.key(),v_lp));
            }
        }
    }
}

void RGBController_RGBFusion2USB::Init_Controller()
{
    /*---------------------------------------------------------*\
    | Look up channel map based on device name                  |
    \*---------------------------------------------------------*/
    if(!custom_layout)
    {
        /*-----------------------------------------------------*\
        | If the layout is custom then it's loaded and ready,   |
        | otherwise get known layouts                           |
        | This check is a quick way to get a boolean on find()  |
        \*-----------------------------------------------------*/
        if(MBName2Layout.count(controller->GetDeviceName()))
        {
            layout = knownLayoutsLookup.find(MBName2Layout.find(controller->GetDeviceName())->second )->second;
        }
        else
        {
            uint16_t pid = controller->GetProductID();
            std::string fallback_layout;

            switch(pid)
            {
                case 0x5711:
                    fallback_layout = "IT5711-Generic";
                    break;
                case 0x5702:
                    fallback_layout = "STD_ATX";
                    break;
                case 0x8297:
                    fallback_layout = "STD_ATX";
                    break;
                default:
                    fallback_layout = "IT5711-Generic";  // safest default
                    break;
            }
        }
    }

    /*---------------------------------------------------------*\
    | Initialize the number of zones from the layout            |
    \*---------------------------------------------------------*/
    zones.resize(layout.size());

    /*---------------------------------------------------------*\
    | Iterate through layout and process each zone              |
    \*---------------------------------------------------------*/
    int zone_idx = 0;
    for(ZoneLeds::iterator zl = layout.begin(); zl != layout.end(); zl++)
    {
        std::vector<LedPort> lp     = zl->second;
        int LED_count               = 0;
        bool single_zone            = true;

        for(std::size_t lp_idx = 0; lp_idx < lp.size(); lp_idx++)
        {
            /*-------------------------------------------------*\
            | Get LED count and check if it is a single LED zone|
            \*-------------------------------------------------*/
            int lp_count            = lp[lp_idx].count;
            single_zone             = single_zone && (lp_count == 1);
            LED_count              += lp_count;
        }

        zones[zone_idx].name        = zl->first;
        zones[zone_idx].leds_min    = (single_zone) ? LED_count : RGBFusion2_Digital_LEDS_Min;
        zones[zone_idx].leds_max    = (single_zone) ? LED_count : RGBFusion2_Digital_LEDS_Max;
        zones[zone_idx].leds_count  = (single_zone) ? LED_count : 0;
        zones[zone_idx].type        = (single_zone) ? ZONE_TYPE_SINGLE : ZONE_TYPE_LINEAR;
        zones[zone_idx].matrix_map  = NULL;
        zone_idx++;
    }
}

void RGBController_RGBFusion2USB::SetupZones()
{
    /*-------------------------------------------------*\
    | Clear any existing color/LED configuration        |
    \*-------------------------------------------------*/
    leds.clear();
    colors.clear();

    /*---------------------------------------------------------*\
    | Set up zones                                              |
    \*---------------------------------------------------------*/
    int zone_idx = 0;
    for(ZoneLeds::iterator zl = layout.begin(); zl != layout.end(); zl++)
    {
        bool single_zone = (zones[zone_idx].type == ZONE_TYPE_SINGLE);

        if(!single_zone)
        {
            controller->SetLedCount(zl->second.at(0).header, zones[zone_idx].leds_count);
            controller->DisableBuiltinEffect(0, 0x3);
        }

        for(unsigned int lp_idx = 0; lp_idx < zones[zone_idx].leds_count; lp_idx++)
        {
            led new_led;

            if(single_zone)
            {
                new_led.name  = zl->second.at(lp_idx).name;
                new_led.value = zl->second.at(lp_idx).header;
            }
            else
            {
                new_led.name  = zl->second.at(0).name;
                new_led.name.append(" LED " + std::to_string(lp_idx));
                new_led.value = zl->second.at(0).header;
            }

            leds.push_back(new_led);
        }

        zone_idx++;
    }

    SetupColors();
}

void RGBController_RGBFusion2USB::ResizeZone(int zone, int new_size)
{
    if((size_t) zone >= zones.size())
    {
        return;
    }

    if(((unsigned int)new_size >= zones[zone].leds_min) && ((unsigned int)new_size <= zones[zone].leds_max))
    {
        zones[zone].leds_count = new_size;

        SetupZones();
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateLEDs()
{
    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_RGBFusion2USB::UpdateZoneLEDs(int zone)
{
    /*---------------------------------------------------------*\
    | Get mode parameters                                       |
    \*---------------------------------------------------------*/
    int     mode_value  = (modes[active_mode].value);
    bool    random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM || mode_value == EFFECT_COLORCYCLE);

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zones[zone].type == ZONE_TYPE_SINGLE)
    {
        unsigned char red = 0;
        unsigned char grn = 0;
        unsigned char blu = 0;

        for(std::size_t led_idx = 0; led_idx < zones[zone].leds_count; led_idx++)
        {
            /*---------------------------------------------------------*\
            | Initialize mode value                                     |
            \*---------------------------------------------------------*/
            mode_value = modes[active_mode].value;

            /*---------------------------------------------------------*\
            | Motherboard LEDs always use effect mode, so use static for|
            | direct mode but get colors from zone                      |
            \*---------------------------------------------------------*/
            if(mode_value == 0xFFFF)
            {
                red = RGBGetRValue(zones[zone].colors[led_idx]);
                grn = RGBGetGValue(zones[zone].colors[led_idx]);
                blu = RGBGetBValue(zones[zone].colors[led_idx]);

                mode_value = EFFECT_STATIC;
            }
            /*---------------------------------------------------------*\
            | If the mode uses mode-specific color, get color from mode |
            \*---------------------------------------------------------*/
            else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
            {
                red = RGBGetRValue(modes[active_mode].colors[0]);
                grn = RGBGetGValue(modes[active_mode].colors[0]);
                blu = RGBGetBValue(modes[active_mode].colors[0]);
            }

            /*---------------------------------------------------------*\
            | Apply the mode and color to the zone                      |
            \*---------------------------------------------------------*/
            controller->SetLEDEffect(zones[zone].leds[led_idx].value, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, red, grn, blu);
        }

        controller->ApplyEffect();
    }
    /*---------------------------------------------------------*\
    | Set strip LEDs                                            |
    \*---------------------------------------------------------*/
    else
    {
        if(zones[zone].leds_count)
        {
            unsigned char hdr = zones[zone].leds->value;

            /*---------------------------------------------------------*\
            | Direct mode addresses a different register                |
            \*---------------------------------------------------------*/
            if(mode_value == 0xFFFF)
            {
                //Updated: Proper RGB register mapping for each header
                switch(hdr)
                {
                    case HDR_D_LED1:
                        hdr = HDR_D_LED1_RGB;
                        controller->DisableBuiltinEffect(1, 0x01);
                        break;
                    case HDR_D_LED2:
                        hdr = HDR_D_LED2_RGB;
                        controller->DisableBuiltinEffect(1, 0x02);
                        break;
                    case HDR_D_LED3:
                        hdr = HDR_D_LED3_RGB;
                        controller->DisableBuiltinEffect(1, 0x08);
                        break;
                    default:
                        break;
                }

                controller->SetStripColors(hdr, zones[zone].colors, zones[zone].leds_count);
            }
            /*---------------------------------------------------------*\
            | Effect mode                                               |
            \*---------------------------------------------------------*/
            else
            {
                unsigned char red = 0;
                unsigned char grn = 0;
                unsigned char blu = 0;

                /*---------------------------------------------------------*\
                | If mode has mode specific color, load color from mode     |
                \*---------------------------------------------------------*/
                if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
                {
                    red = RGBGetRValue(modes[active_mode].colors[0]);
                    grn = RGBGetGValue(modes[active_mode].colors[0]);
                    blu = RGBGetBValue(modes[active_mode].colors[0]);
                }

                /*---------------------------------------------------------*\
                | Apply built-in effects to LED strips                      |
                \*---------------------------------------------------------*/

                switch(hdr)
                {
                    case HDR_D_LED1:
                        controller->DisableBuiltinEffect(0, 0x01);
                        break;
                    case HDR_D_LED2:
                        controller->DisableBuiltinEffect(0, 0x02);
                        break;
                    case HDR_D_LED3:
                        controller->DisableBuiltinEffect(0, 0x08);
                        break;
                    default:
                    break;
                }

                controller->SetLEDEffect(hdr, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, red, grn, blu);
                controller->ApplyEffect();
            }
        }
    }
}

void RGBController_RGBFusion2USB::UpdateSingleLED(int led)
{
    /*---------------------------------------------------------*\
    | Get mode parameters                                       |
    \*---------------------------------------------------------*/
    bool            random      = (modes[active_mode].color_mode == MODE_COLORS_RANDOM);
    int             mode_value  = (modes[active_mode].value);
    unsigned int    zone_idx    = GetLED_Zone(led);

    /*---------------------------------------------------------*\
    | Set motherboard LEDs                                      |
    \*---------------------------------------------------------*/
    if(zones[zone_idx].type == ZONE_TYPE_SINGLE)
    {
        unsigned char red = 0;
        unsigned char grn = 0;
        unsigned char blu = 0;

        /*---------------------------------------------------------*\
        | Motherboard LEDs always use effect mode, so use static for|
        | direct mode but get colors from zone                      |
        \*---------------------------------------------------------*/
        if(mode_value == 0xFFFF)
        {
            red = RGBGetRValue(colors[led]);
            grn = RGBGetGValue(colors[led]);
            blu = RGBGetBValue(colors[led]);

            mode_value = EFFECT_STATIC;
        }
        /*---------------------------------------------------------*\
        | If the mode uses mode-specific color, get color from mode |
        \*---------------------------------------------------------*/
        else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
        {
            red = RGBGetRValue(modes[active_mode].colors[0]);
            grn = RGBGetGValue(modes[active_mode].colors[0]);
            blu = RGBGetBValue(modes[active_mode].colors[0]);
        }

        controller->SetLEDEffect(leds[led].value, mode_value, modes[active_mode].speed, modes[active_mode].brightness, random, red, grn, blu);
        controller->ApplyEffect();
    }
    /*---------------------------------------------------------*\
    | Set strip LEDs                                            |
    \*---------------------------------------------------------*/
    else
    {
        UpdateZoneLEDs(zone_idx);
    }
}

void RGBController_RGBFusion2USB::DeviceUpdateMode()
{
    DeviceUpdateLEDs();
}

int RGBController_RGBFusion2USB::GetLED_Zone(int led_idx)
{
    for(int zone_idx = 0; zone_idx < (int)zones.size(); zone_idx++)
    {
        int zone_start  = zones[zone_idx].start_idx;
        int zone_end    = zone_start + zones[zone_idx].leds_count - 1;

        if((zone_start <= led_idx) && (zone_end >= led_idx))
        {
            return(zone_idx);
        }
    }

    /*---------------------------------*\
    | If zone is not found, return -1   |
    \*---------------------------------*/
    return(-1);
}
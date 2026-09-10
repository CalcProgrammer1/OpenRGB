/*---------------------------------------------------------*\
| PalitGPUControllerDetect.cpp                              |
|                                                           |
|   Detector for Palit GPU                                  |
|                                                           |
|   Manatsawin Hanmongkolchai                   11 Apr 2023 |
|   TheRogueZeta                                05 Nov 2020 |
|   KundaPanda                                  04 Jan 2021 |
|   KendallMorgan                               17 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-or-later               |
\*---------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "DetectionManager.h"
#include "i2c_smbus.h"
#include "PalitGPUv1Controller.h"
#include "PalitGPUv2Controller.h"
#include "pci_ids.h"
#include "RGBController_PalitGPUv1.h"
#include "RGBController_PalitGPUv2.h"

bool TestForPalitGPUv1Controller(i2c_smbus_interface* bus, uint8_t i2c_addr)
{
    bool pass = false;

    switch(i2c_addr)
    {
        case 0x08:
            pass = bus->i2c_smbus_write_quick(i2c_addr, I2C_SMBUS_WRITE);
            break;
    }

    return(pass);
}

bool TestForPalitGPUv2Controller(i2c_smbus_interface* bus, uint8_t i2c_addr)
{
    bool pass = false;

    switch(i2c_addr)
    {
        case 0x49:
            s32 data        = bus->i2c_smbus_read_byte_data(i2c_addr, 0x0);
            s32 mode_data   = bus->i2c_smbus_read_byte_data(i2c_addr, 0xe0);
            pass            = (data == 0x0) && (mode_data < 0x5);
            break;
    }

    return(pass);
}

DetectedControllers DetectPalitGPUv1Controllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    DetectedControllers detected_controllers;

    if(TestForPalitGPUv1Controller(bus, i2c_addr))
    {
        PalitGPUv1Controller*     controller     = new PalitGPUv1Controller(bus, i2c_addr, name);
        RGBController_PalitGPUv1* rgb_controller = new RGBController_PalitGPUv1(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

DetectedControllers DetectPalitGPUv2Controllers(i2c_smbus_interface* bus, uint8_t i2c_addr, const std::string& name)
{
    DetectedControllers detected_controllers;

    if(TestForPalitGPUv2Controller(bus, i2c_addr))
    {
        PalitGPUv2Controller*     controller     = new PalitGPUv2Controller(bus, i2c_addr, name);
        RGBController_PalitGPUv2* rgb_controller = new RGBController_PalitGPUv2(controller);

        detected_controllers.push_back(rgb_controller);
    }

    return(detected_controllers);
}

/*---------------------------------------------------------*\
| Register detectors for Palit GPU controllers              |
| Uses the unified PalitGPUv1Controllers for all V1 protocol|
| devices ( Gainward and Palit branded)                     |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1060",                     DetectPalitGPUv1Controllers,    NVIDIA_VEN, NVIDIA_GTX1060_DEV,         NVIDIA_SUB_VEN,     NVIDIA_GTX1060_DEV,                         0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1070",                     DetectPalitGPUv1Controllers,    NVIDIA_VEN, NVIDIA_GTX1070_DEV,         NVIDIA_SUB_VEN,     NVIDIA_GTX1070_DEV,                         0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1070 Ti",                  DetectPalitGPUv1Controllers,    NVIDIA_VEN, NVIDIA_GTX1070TI_DEV,       NVIDIA_SUB_VEN,     NVIDIA_GTX1070TI_DEV,                       0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1080",                     DetectPalitGPUv1Controllers,    NVIDIA_VEN, NVIDIA_GTX1080_DEV,         NVIDIA_SUB_VEN,     NVIDIA_GTX1080_DEV,                         0x08);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce GTX 1080 Ti",                  DetectPalitGPUv1Controllers,    NVIDIA_VEN, NVIDIA_GTX1080TI_DEV,       NVIDIA_SUB_VEN,     NVIDIA_GTX1080TI_DEV,                       0x08);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce GTX 1080 Phoenix",          DetectPalitGPUv1Controllers,    NVIDIA_VEN, NVIDIA_GTX1080_DEV,         GAINWARD_SUB_VEN,   GAINWARD_GTX_1080_PHOENIX,                  0x08);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce GTX 1080 Ti Phoenix",       DetectPalitGPUv1Controllers,    NVIDIA_VEN, NVIDIA_GTX1080TI_DEV,       GAINWARD_SUB_VEN,   GAINWARD_GTX_1080TI_PHOENIX,                0x08);

/*---------------------------------------------------------*\
| Register detectors for Palit V2 GPU controllers           |
| (Gainward and PNY branded cards)                          |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce GTX 1660 SUPER Ghost",      DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_GTX1660S_DEV,        NVIDIA_SUB_VEN,     NVIDIA_GTX1660S_DEV,                        0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 2070 SUPER Phantom",    DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,     GAINWARD_SUB_VEN,   NVIDIA_RTX2070S_OC_DEV,                     0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 2080 Phoenix GS",       DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX2080_DEV,         GAINWARD_SUB_VEN,   NVIDIA_RTX2080_A_DEV,                       0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3060 Pegasus 12G",      DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3060_DEV,         NVIDIA_SUB_VEN,     NVIDIA_RTX3060_DEV,                         0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3070 Phantom",          DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         GAINWARD_SUB_VEN,   GAINWARD_RTX_3070_PHANTOM,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3070 Phoenix",          DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         GAINWARD_SUB_VEN,   NVIDIA_RTX3070_DEV,                         0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3070 Ti Phoenix",       DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       GAINWARD_SUB_VEN,   NVIDIA_RTX3070TI_DEV,                       0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3080 Phoenix",          DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         GAINWARD_SUB_VEN,   NVIDIA_RTX3080_DEV,                         0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3080 Ti Phoenix",       DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       GAINWARD_SUB_VEN,   NVIDIA_RTX3080TI_DEV,                       0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3090 Phoenix",          DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         GAINWARD_SUB_VEN,   NVIDIA_RTX3090_DEV,                         0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 3090 Ti Phantom",       DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3090TI_DEV,       GAINWARD_SUB_VEN,   GAINWARD_RTX_3090TI_PHANTOM,                0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 4080 Phantom GS",       DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4080_DEV,         GAINWARD_SUB_VEN,   GAINWARD_RTX_4080_PHANTOM_GS_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 4090 Phantom",          DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4090_DEV,         GAINWARD_SUB_VEN,   GAINWARD_RTX_4090_PHANTOM_SUB_DEV,          0x49);
REGISTER_I2C_PCI_DETECTOR("Gainward GeForce RTX 5080 Phoenix",          DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5080_DEV,         GAINWARD_SUB_VEN,   GAINWARD_RTX_5080_PHOENIX_SUB_DEV,          0x49);

/*---------------------------------------------------------*\
| PNY GPU controllers (also Palit-manufactured)             |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 2060 XLR8 OC EDITION",       DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX2060_TU104_DEV,   PNY_SUB_VEN,        PNY_RTX_2060_XLR8_OC_SUB_DEV,               0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3060 XLR8 Revel EPIC-X",     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3060_DEV,         PNY_SUB_VEN,        PNY_RTX_3060_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3070 XLR8 Revel EPIC-X",     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         PNY_SUB_VEN,        PNY_RTX_3070_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3070 XLR8 Revel EPIC-X LHR", DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     PNY_SUB_VEN,        PNY_RTX_3070_XLR8_REVEL_EPIC_X_LHR_SUB_DEV, 0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3080 XLR8 Revel EPIC-X",     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PNY_SUB_VEN,        PNY_RTX_3080_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3080 Ti XLR8 Revel EPIC-X",  DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       PNY_SUB_VEN,        PNY_RTX_3080TI_XLR8_REVEL_EPIC_X_SUB_DEV,   0x49);
REGISTER_I2C_PCI_DETECTOR("PNY GeForce RTX 3090 XLR8 Revel EPIC-X",     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PNY_SUB_VEN,        PNY_RTX_3090_XLR8_REVEL_EPIC_X_SUB_DEV,     0x49);

/*---------------------------------------------------------*\
| Palit GPU controllers detected via PNY detector           |
\*---------------------------------------------------------*/
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 2070 SUPER Jetstream",     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX2070S_OC_DEV,     NVIDIA_SUB_VEN,     NVIDIA_RTX2070S_OC_DEV,                     0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060",                     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3060_DEV,         PALIT_SUB_VEN,      PALIT_RTX3060_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 LHR",                 DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3060_LHR_DEV,     PALIT_SUB_VEN,      PALIT_RTX3060_LHR_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 Ti",                  DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX3060TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3060 Ti LHR",              DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3060TI_LHR_DEV,   PALIT_SUB_VEN,      NVIDIA_RTX3060TI_LHR_DEV,                   0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070",                     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070_DEV,         PALIT_SUB_VEN,      PALIT_RTX3070_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070 LHR",                 DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070_LHR_DEV,     PALIT_SUB_VEN,      PALIT_RTX3070_LHR_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3070 Ti",                  DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3070TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX3070TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080",                     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3080_DEV,         PALIT_SUB_VEN,      PALIT_RTX3080_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 LHR",                 DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3080_LHR_DEV,     PALIT_SUB_VEN,      PALIT_RTX3080_LHR_SUB_DEV,                  0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3080 Ti",                  DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3080TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX3080TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3090",                     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PALIT_SUB_VEN,      PALIT_RTX3090_SUB_DEV,                      0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3090 Gamerock",            DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3090_DEV,         PALIT_SUB_VEN,      PALIT_RTX3090_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 3090 Ti Gamerock",         DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX3090TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX3090TI_GAMEROCK_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4070 Ti",                  DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX4070TI_SUB_DEV,                    0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4070 Ti Gamerock",         DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4070TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX4070TI_GAMEROCK_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4070 Ti SUPER GamingPro",  DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4070TIS_DEV,      PALIT_SUB_VEN,      PALIT_RTX4080_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4080 GamingPro",           DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4080_DEV,         PALIT_SUB_VEN,      PALIT_RTX4080_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4080 SUPER GamingPro",     DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4080S_DEV,        PALIT_SUB_VEN,      PALIT_RTX4080_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 4090 Gamerock",            DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX4090_DEV,         PALIT_SUB_VEN,      PALIT_RTX4090_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5060 Ti White OC 16GB",    DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5060TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX5060TI_WHITE_OC_16G_SUB_DEV,       0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5070 GamingPro",           DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5070_DEV,         PALIT_SUB_VEN,      PALIT_RTX5070_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5070 Ti GameRock",         DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX5070TI_GAMEROCK_SUB_DEV,           0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5070 Ti GamingPro",        DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX5070TI_GAMINGPRO_SUB_DEV,          0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5070 Ti GamingPro-S",      DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5070TI_DEV,       PALIT_SUB_VEN,      PALIT_RTX5070TI_GAMINGPRO_S_SUB_DEV,        0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5080 GameRock",            DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5080_DEV,         PALIT_SUB_VEN,      PALIT_RTX5080_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5080 GamingPro",           DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5080_DEV,         PALIT_SUB_VEN,      PALIT_RTX5080_GAMINGPRO_SUB_DEV,            0x49);
REGISTER_I2C_PCI_DETECTOR("Palit GeForce RTX 5090 Gamerock",            DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX5090_DEV,         PALIT_SUB_VEN,      PALIT_RTX5090_GAMEROCK_SUB_DEV,             0x49);
REGISTER_I2C_PCI_DETECTOR("NVIDIA GeForce RTX 2060 SUPER",              DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX2060_TU106_DEV,   NVIDIA_SUB_VEN,     NVIDIA_RTX2060_TU106_DEV,                   0x49);
REGISTER_I2C_PCI_DETECTOR("NVIDIA GeForce RTX 2080 SUPER",              DetectPalitGPUv2Controllers,    NVIDIA_VEN, NVIDIA_RTX2080S_DEV,        NVIDIA_SUB_VEN,     NVIDIA_RTX2080S_DEV,                        0x49);

/*---------------------------------------------------------*\
| PCI Vendor IDs                                            |
\*---------------------------------------------------------*/
#define AMD_VEN                                         0x1022
#define AMD_GPU_VEN                                     0x1002
#define INTEL_VEN                                       0x8086
#define NVIDIA_VEN                                      0x10DE

/*-----------------------------------------------------*\
| AMD Device IDs                                        |
\*-----------------------------------------------------*/
#define AMD_FCH_SMBUS_DEV                               0x790B

/*-----------------------------------------------------*\
| AMD GPU Device IDs                                    |
|
| Note: AMD uses the same PCI IDs for different SKUs    |
| based on the same silicon, so we use internal code    |
| names here instead of referring to specific products. |
\*-----------------------------------------------------*/
#define AMD_POLARIS_DEV                                 0x67DF
#define AMD_POLARIS20XL_DEV                             0x6FDF
#define AMD_VEGA10_DEV                                  0x687F
#define AMD_NAVI10_DEV                                  0x731F
#define AMD_NAVI14_DEV                                  0x7340
#define AMD_NAVI21_DEV                                  0x73BF

/*-----------------------------------------------------*\
| Intel Device IDs                                      |
\*-----------------------------------------------------*/
#define INTEL_ICH10_SMBUS_DEV                           0x3A30

/*-----------------------------------------------------*\
| nVidia Device IDs                                     |
\*-----------------------------------------------------*/
#define NVIDIA_GTX1050TI_DEV                            0x1C82
#define NVIDIA_GTX1060_DEV                              0x1C03
#define NVIDIA_GTX1070_DEV                              0x1B81
#define NVIDIA_GTX1070TI_DEV                            0x1B82
#define NVIDIA_GTX1080_DEV                              0x1B80
#define NVIDIA_GTX1080TI_DEV                            0x1B06
#define NVIDIA_GTX1650_DEV                              0x1F82
#define NVIDIA_GTX1660_DEV                              0x2184
#define NVIDIA_GTX1660TI_DEV                            0x2182
#define NVIDIA_GTX1660S_DEV                             0x21C4
#define NVIDIA_RTX2060_TU104_DEV                        0x1E89
#define NVIDIA_RTX2060_TU106_DEV                        0x1F08
#define NVIDIA_RTX2060S_DEV                             0x1F47
#define NVIDIA_RTX2060S_OC_DEV                          0x1F06
#define NVIDIA_RTX2070_DEV                              0x1F02
#define NVIDIA_RTX2070_OC_DEV                           0x1F07
#define NVIDIA_RTX2070S_DEV                             0x1E84
#define NVIDIA_RTX2080_DEV                              0x1E82
#define NVIDIA_RTX2080_A_DEV                            0x1E87
#define NVIDIA_RTX2080S_DEV                             0x1E81
#define NVIDIA_RTX2080TI_DEV                            0x1E07
#define NVIDIA_RTX3060_DEV                              0x2503
#define NVIDIA_RTX3060TI_DEV                            0x2486
#define NVIDIA_RTX3070_DEV                              0x2484
#define NVIDIA_RTX3080_DEV                              0x2206
#define NVIDIA_RTX3090_DEV                              0x2204

/*---------------------------------------------------------*\
| PCI Sub-Vendor IDs                                        |
\*---------------------------------------------------------*/
#define ASUS_SUB_VEN                                    0x1043
#define EVGA_SUB_VEN                                    0x3842
#define GALAX_SUB_VEN                                   0x1B4C
#define GAINWARD_SUB_VEN                                0x10B0
#define GIGABYTE_SUB_VEN                                0x1458
#define MSI_SUB_VEN                                     0x1462
#define NVIDIA_SUB_VEN                                  0x10DE
#define PNY_SUB_VEN                                     0x196E
#define SAPPHIRE_SUB_VEN                                0x1DA2
#define SAPPHIRE_LEGACY_SUB_VEN                         0x174B
#define ZOTAC_SUB_VEN                                   0x19DA

/*-----------------------------------------------------*\
| ASUS Sub-Device IDs                                   |
\*-----------------------------------------------------*/
#define ASUS_GTX1060_STRIX                              0x85AC
#define ASUS_GTX1070_STRIX_GAMING                       0x8598
#define ASUS_GTX1070_STRIX_OC                           0x8599
#define ASUS_GTX1080_STRIX                              0x8592
#define ASUS_ROG_STRIX_GTX1080_A8G_GAMING               0x85AA
#define ASUS_ROG_STRIX_GTX1080_O8G_GAMING               0x85F9
#define ASUS_ROG_STRIX_GTX1080TI_GAMING                 0x85EA
#define ASUS_ROG_GTX1660TI_OC                           0x86A5
#define ASUS_ROG_STRIX_RTX2060_EVO_GAMING               0x86D3
#define ASUS_ROG_STRIX_RTX2060S_A8G_EVO_GAMING          0x8703
#define ASUS_ROG_STRIX_RTX2070_A8G_GAMING               0x8671
#define ASUS_ROG_STRIX_RTX2070_O8G_GAMING               0x8670
#define ASUS_ROG_STRIX_RTX2070S_A8G_GAMING              0x8728
#define ASUS_ROG_STRIX_RTX2070S_O8G_GAMING              0x8727
#define ASUS_ROG_STRIX_RTX2080_O8G_GAMING               0x865F
#define ASUS_ROG_STRIX_RTX2080TI_O11G_GAMING            0x866A
#define ASUS_VEGA64_STRIX                               0x04C4
#define ASUS_RX5700XT_STRIX_GAMING_OC                   0x04E2
#define ASUS_RX570_STRIX_O4G_GAMING_OC                  0x04C2
#define ASUS_RX580_STRIX_GAMING_OC                      0x0517

/*-----------------------------------------------------*\
| EVGA Sub-Device IDs                                   |
\*-----------------------------------------------------*/
#define EVGA_GTX1070_FTW_SUB_DEV                        0x6276
#define EVGA_GTX1070TI_FTW2_SUB_DEV                     0x6775
#define EVGA_GTX1080_FTW_SUB_DEV                        0x6286
#define EVGA_RTX2070_XC_GAMING_SUB_DEV                  0x2172
#define EVGA_RTX2070_XC_OC_SUB_DEV                      0x2173
#define EVGA_RTX2070S_XC_ULTRA_SUB_DEV                  0x3173
#define EVGA_RTX2070S_XC_ULTRA_PLUS_SUB_DEV             0x3175
#define EVGA_RTX2080_XC_BLACK_SUB_DEV                   0x2082
#define EVGA_RTX2080_XC_GAMING_SUB_DEV                  0x2182
#define EVGA_RTX2080_XC_ULTRA_GAMING_SUB_DEV            0x2183
#define EVGA_RTX2080TI_XC_ULTRA_GAMING_SUB_DEV          0x2383
#define EVGA_RTX2080TI_FTW3_ULTRA_SUB_DEV               0x2487

/*-----------------------------------------------------*\
| Gainward Sub-Device IDs                               |
\*-----------------------------------------------------*/
#define GAINWARD_GTX_1080_PHOENIX                       0x1B80

/*-----------------------------------------------------*\
| GALAX / KFA2 Sub-Device IDs                           |
\*-----------------------------------------------------*/
#define KFA2_RTX_2070_EX_SUB_DEV                        0x12FE
#define GALAX_RTX_2070S_EX_GAMER_BLACK_SUB_DEV          0x139F
#define KFA2_RTX_2080_EX_OC_SUB_DEV                     0x12B0

/*-----------------------------------------------------*\
| Gigabyte (Aorus) Sub-Device IDs                       |
\*-----------------------------------------------------*/
#define GIGABYTE_GTX1050TI_G1_GAMING_SUB_DEV            0x372A
#define GIGABYTE_GTX1060_XTREME_SUB_DEV                 0x3776
#define GIGABYTE_GTX1070_XTREME_SUB_DEV                 0x3778
#define GIGABYTE_GTX1080_G1_GAMING_SUB_DEV              0x3702
#define GIGABYTE_GTX1080TI_SUB_DEV                      0x3752
#define GIGABYTE_GTX1080TI_GAMING_OC_SUB_DEV            0x374C
#define GIGABYTE_GTX1080TI_GAMING_OC_BLACK_SUB_DEV      0x377A
#define GIGABYTE_GTX1080TI_XTREME_SUB_DEV               0x3751
#define GIGABYTE_GTX1080TI_XTREME_WATERFORCE_SUB_DEV    0x3762
#define GIGABYTE_GTX1650_GAMING_OC_SUB_DEV              0x3FE4
#define GIGABYTE_GTX1660S_GAMING_OC_SUB_DEV             0x4014
#define GIGABYTE_RTX2060_GAMING_OC_SUB_DEV              0x37CE
#define GIGABYTE_RTX2060_GAMING_OC_PRO_SUB_DEV          0x3FC2
#define GIGABYTE_RTX2060S_GAMING_SUB_DEV                0x404A
#define GIGABYTE_RTX2070_GAMING_OC_SUB_DEV              0x37AD
#define GIGABYTE_RTX2070_WINDFORCE_SUB_DEV              0x37C2
#define GIGABYTE_RTX2080_A_GAMING_OC_SUB_DEV            0x37A7
#define GIGABYTE_RTX2080_GAMING_OC_SUB_DEV              0x37D6
#define GIGABYTE_RTX2070S_GAMING_OC_SUB_DEV             0x3FEB
#define GIGABYTE_RTX2070S_GAMING_OC_3X_SUB_DEV          0x4008
#define GIGABYTE_RTX3060_GAMING_OC_12GB_SUB_DEV         0x4074
#define GIGABYTE_RTX3060TI_EAGLE_OC_SUB_DEV             0x405B
#define GIGABYTE_RTX3070_VISION_OC_SUB_DEV              0x404D
#define GIGABYTE_RTX3080_GAMING_OC_SUB_DEV              0x403F

/*-----------------------------------------------------*\
| MSI Sub-Device IDs                                    |
\*-----------------------------------------------------*/
#define MSI_GTX1070_GAMING_X_SUB_DEV                    0x3306
#define MSI_GTX1660_GAMING_X_6G_SUB_DEV                 0x3790
#define MSI_GTX1660TI_GAMING_X_6G_SUB_DEV               0x375A
#define MSI_GTX1660S_GAMING_6G_SUB_DEV                  0xC759
#define MSI_GTX1660S_GAMING_X_6G_SUB_DEV                0xC758
#define MSI_RTX2060_GAMING_Z_6G_SUB_DEV                 0x3752
#define MSI_RTX2060_GAMING_Z_6G_SUB_DEV_2               0x3754
#define MSI_RTX2060S_GAMING_X_SUB_DEV                   0xC752
#define MSI_RTX2060S_ARMOR_OC_SUB_DEV                   0xC754
#define MSI_RTX2070_GAMING_Z_SUB_DEV                    0x3732
#define MSI_RTX2070_GAMING_SUB_DEV                      0x3733
#define MSI_RTX2070_ARMOR_SUB_DEV                       0x3734
#define MSI_RTX2070S_GAMING_TRIO_SUB_DEV                0xC727
#define MSI_RTX2070S_GAMING_Z_TRIO_SUB_DEV              0x37B6
#define MSI_RTX2070S_GAMING_X_SUB_DEV                   0x373e
#define MSI_RTX2070S_GAMING_X_TRIO_SUB_DEV              0xC726
#define MSI_RTX2080_GAMING_TRIO_SUB_DEV                 0x372E
#define MSI_RTX2080_GAMING_X_TRIO_SUB_DEV               0x3726
#define MSI_RTX2080_SEA_HAWK_EK_X_SUB_DEV               0x3728
#define MSI_RTX2080S_GAMING_X_TRIO_SUB_DEV              0xC724
#define MSI_RTX2080TI_GAMING_X_TRIO_SUB_DEV             0x3715
#define MSI_RTX2080TI_SEA_HAWK_EK_X_SUB_DEV             0x3717
#define MSI_RTX3070_GAMING_X_TRIO_SUB_DEV               0x3903
#define MSI_RTX3080_GAMING_X_TRIO_SUB_DEV               0x3892
#define MSI_RTX3080_SUPRIM_X_SUB_DEV                    0x3897
#define MSI_RTX3090_GAMING_X_TRIO_SUB_DEV               0x3884
#define MSI_RTX3090_SUPRIM_X_SUB_DEV                    0x3882

/*-----------------------------------------------------*\
| Sapphire Sub-Device IDs                               |
\*-----------------------------------------------------*/
#define SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV1            0xE366
#define SAPPHIRE_POLARIS_NITRO_PLUS_SUB_DEV2            0xE399
#define SAPPHIRE_LEGACY_POLARIS_NITRO_PLUS_SUB_DEV      0xE347
#define SAPPHIRE_VEGA10_NITRO_PLUS_SUB_DEV              0xE37F
#define SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV1             0xE409
#define SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV2             0xE410
#define SAPPHIRE_NAVI10_NITRO_PLUS_SUB_DEV3             0x426E
#define SAPPHIRE_NAVI14_NITRO_PLUS_SUB_DEV              0xE423
#define SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV1             0x438E
#define SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV2             0xE438
#define SAPPHIRE_NAVI21_NITRO_PLUS_SUB_DEV3             0xE439

/*---------------------------------------------------------*\
| PCI ID Macros                                             |
\*---------------------------------------------------------*/
#define IF_DRAM_SMBUS(ven, dev)                             \
    if(((ven == AMD_VEN) && (dev == AMD_FCH_SMBUS_DEV)) ||  \
    ((ven == INTEL_VEN) && (dev == INTEL_ICH10_SMBUS_DEV)))

#define IF_MOBO_SMBUS(ven, dev)                             \
    if((ven == 0) ||                                        \
    ((ven == AMD_VEN) && (dev == AMD_FCH_SMBUS_DEV)) ||     \
    ((ven == INTEL_VEN) && (dev == INTEL_ICH10_SMBUS_DEV)))

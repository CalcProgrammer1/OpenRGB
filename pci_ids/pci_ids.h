/*---------------------------------------------------------*\
| PCI Vendor IDs                                            |
\*---------------------------------------------------------*/
#define AMD_VEN                                 0x1022
#define AMD_GPU_VEN                             0x1002
#define INTEL_VEN                               0x8086
#define NVIDIA_VEN                              0x10DE

/*-----------------------------------------------------*\
| AMD Device IDs                                        |
\*-----------------------------------------------------*/
#define AMD_FCH_SMBUS_DEV                       0x790B

/*-----------------------------------------------------*\
| AMD GPU Device IDs                                    |
\*-----------------------------------------------------*/
#define AMD_RX580_DEV                           0x67DF

/*-----------------------------------------------------*\
| Intel Device IDs                                      |
\*-----------------------------------------------------*/
#define INTEL_ICH10_SMBUS_DEV                   0x3A30

/*-----------------------------------------------------*\
| nVidia Device IDs                                     |
\*-----------------------------------------------------*/
#define NVIDIA_GTX1070_DEV                      0x1B81
#define NVIDIA_GTX1660_DEV                      0x2184
#define NVIDIA_RTX2060_DEV                      0x1F08
#define NVIDIA_RTX2060S_DEV                     0x1F06
#define NVIDIA_RTX2070_DEV                      0x1F02
#define NVIDIA_RTX2070_OC_DEV                   0x1F07
#define NVIDIA_RTX2070S_DEV                     0x1E84
#define NVIDIA_RTX2080_DEV                      0x1E87
#define NVIDIA_RTX2080S_DEV                     0x1E81
#define NVIDIA_RTX2080TI_DEV                    0x1E07
#define NVIDIA_RTX3080_DEV                      0x2206

/*---------------------------------------------------------*\
| PCI Sub-Vendor IDs                                        |
\*---------------------------------------------------------*/
#define ASUS_SUB_VEN                            0x1043
#define EVGA_SUB_VEN                            0x3842
#define GIGABYTE_SUB_VEN                        0x1458
#define MSI_SUB_VEN                             0x1462
#define SAPPHIRE_SUB_VEN                        0x1DA2

/*-----------------------------------------------------*\
| EVGA Sub-Device IDs                                   |
\*-----------------------------------------------------*/
#define EVGA_GTX1070_FTW_SUB_DEV                0x6276
#define EVGA_RTX2070_XC_OC_SUB_DEV              0x2173
#define EVGA_RTX2080_XC_GAMING_SUB_DEV          0x2182

/*-----------------------------------------------------*\
| MSI Sub-Device IDs                                    |
\*-----------------------------------------------------*/
#define MSI_GTX1070_GAMING_X_SUB_DEV            0x3306
#define MSI_GTX1660_GAMING_X_6G_SUB_DEV         0x3790
#define MSI_RTX2060S_GAMING_X_SUB_DEV           0xC752
#define MSI_RTX2070S_GAMING_X_SUB_DEV           0x373e
#define MSI_RTX2070S_GAMING_X_TRIO_SUB_DEV      0xC726
#define MSI_RTX2080_GAMING_X_TRIO_SUB_DEV       0x3726
#define MSI_RTX2080S_GAMING_X_TRIO_SUB_DEV      0xC724
#define MSI_RTX2080TI_GAMING_X_TRIO_SUB_DEV     0x3715
#define MSI_RTX2060_GAMING_Z_6G_SUB_DEV         0x3754
#define MSI_RTX2060_GAMING_Z_6G_SUB_DEV_2       0x3752
#define MSI_RTX2070_ARMOR_SUB_DEV               0x3734
#define MSI_RTX2060S_ARMOR_OC_SUB_DEV           0xC754
#define MSI_RTX2080_SEA_HAWK_EK_X_SUB_DEV       0x3728
#define MSI_RTX2080TI_SEA_HAWK_EK_X_SUB_DEV     0x3717
#define MSI_RTX3080_GAMING_X_TRIO_SUB_DEV       0x3892

/*-----------------------------------------------------*\
| Sapphire Sub-Device IDs                               |
\*-----------------------------------------------------*/
#define SAPPHIRE_RX580_NITRO_PLUS_SUB_DEV       0xE366

/*---------------------------------------------------------*\
| PCI ID Macros                                             |
\*---------------------------------------------------------*/
#define IF_DRAM_SMBUS(ven, dev)                             \
    if((ven == AMD_VEN) || (ven == INTEL_VEN))

#define IF_MOBO_SMBUS(ven, dev)                             \
    if((ven == 0) || (ven == AMD_VEN) || (ven == INTEL_VEN))

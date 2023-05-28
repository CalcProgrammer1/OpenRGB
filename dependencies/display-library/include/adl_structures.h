//
// Copyright (c) 2016 - 2022 Advanced Micro Devices, Inc. All rights reserved.
//
// MIT LICENSE:
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#ifndef ADL_STRUCTURES_H_
#define ADL_STRUCTURES_H_

#include "adl_defines.h"
#include <stdbool.h>
typedef struct AdapterInfo
{

    int iSize;
    int iAdapterIndex;
    char strUDID[ADL_MAX_PATH];
    int iBusNumber;
    int iDeviceNumber;
    int iFunctionNumber;
    int iVendorID;
    char strAdapterName[ADL_MAX_PATH];
    char strDisplayName[ADL_MAX_PATH];
    int iPresent;

#if defined (_WIN32) || defined (_WIN64)

    int iExist;
    char strDriverPath[ADL_MAX_PATH];
    char strDriverPathExt[ADL_MAX_PATH];
    char strPNPString[ADL_MAX_PATH];
    int iOSDisplayIndex;

#endif /* (_WIN32) || (_WIN64) */

#if defined (LINUX)

    int iXScreenNum;
    int iDrvIndex;
    char strXScreenConfigName[ADL_MAX_PATH];

#endif /* (LINUX) */
} AdapterInfo, *LPAdapterInfo;

#if defined (LINUX)
typedef struct XScreenInfo
{
    int iXScreenNum;
    char strXScreenConfigName[ADL_MAX_PATH];
} XScreenInfo, *LPXScreenInfo;
#endif /* (LINUX) */

typedef struct ADLAdapterCaps
{
    int iAdapterID;
    int iNumControllers;
    int iNumDisplays;
    int iNumOverlays;
    int iNumOfGLSyncConnectors;
    int iCapsMask;
    int iCapsValue;
}ADLAdapterCaps;

typedef struct ADLMemoryInfo2
{
    long long iMemorySize;
    char strMemoryType[ADL_MAX_PATH];
    long long iMemoryBandwidth;
    long long iHyperMemorySize;

    long long iInvisibleMemorySize;
    long long iVisibleMemorySize;
} ADLMemoryInfo2, *LPADLMemoryInfo2;

typedef struct ADLMemoryInfo3
{
    long long iMemorySize;
    char strMemoryType[ADL_MAX_PATH];
    long long iMemoryBandwidth;
    long long iHyperMemorySize;

    long long iInvisibleMemorySize;
    long long iVisibleMemorySize;
    long long iVramVendorRevId;
} ADLMemoryInfo3, *LPADLMemoryInfo3;

typedef struct ADLMemoryInfoX4
{
    long long iMemorySize;
    char strMemoryType[ADL_MAX_PATH];
    long long iMemoryBandwidth;
    long long iHyperMemorySize;

    long long iInvisibleMemorySize;
    long long iVisibleMemorySize;
    long long iVramVendorRevId;
    long long iMemoryBandwidthX2;
    long long iMemoryBitRateX2;

} ADLMemoryInfoX4, *LPADLMemoryInfoX4;

// ADLvRamVendor Enumeration
enum ADLvRamVendors
{
    ADLvRamVendor_Unsupported = 0x0,
    ADLvRamVendor_SAMSUNG,
    ADLvRamVendor_INFINEON,
    ADLvRamVendor_ELPIDA,
    ADLvRamVendor_ETRON,
    ADLvRamVendor_NANYA,
    ADLvRamVendor_HYNIX,
    ADLvRamVendor_MOSEL,
    ADLvRamVendor_WINBOND,
    ADLvRamVendor_ESMT,
    ADLvRamVendor_MICRON = 0xF,
    ADLvRamVendor_Undefined
};


typedef struct ADLGcnInfo
{
    int CuCount; //Number of compute units on the ASIC.
    int TexCount; //Number of texture mapping units.
    int RopCount; //Number of Render backend Units.
    int ASICFamilyId; //Such SI, VI. See /inc/asic_reg/atiid.h for family ids
    int ASICRevisionId; //Such as Ellesmere, Fiji.   For example - VI family revision ids are stored in /inc/asic_reg/vi_id.h
}ADLGcnInfo;

typedef struct ADLVirtualSegmentSettingsOutput
{
    int                      virtualSegmentSupported;   // 1 - subsequent values are valid
    int                      virtualSegmentDefault;     //virtual segment default, 1: enable, 0: disable
    int                      virtualSegmentCurrent;     //virtual segment current, 1: enable, 0: disable
    int                      iMinSizeInMB;              //minimum value
    int                      iMaxSizeInMB;              //maximum value
    int                      icurrentSizeInMB;          //last configured otherwise same as factory default
    int                      idefaultSizeInMB;          //factory default
    int                      iMask;                     //fileds for extension in the future
    int                      iValue;                    //fileds for extension in the future
} ADLVirtualSegmentSettingsOutput;

typedef struct ADLChipSetInfo
{
    int iBusType;
    int iBusSpeedType;
    int iMaxPCIELaneWidth;
    int iCurrentPCIELaneWidth;
    int iSupportedAGPSpeeds;
    int iCurrentAGPSpeed;
} ADLChipSetInfo, *LPADLChipSetInfo;

typedef struct ADLMemoryInfo
{
    long long iMemorySize;
    char strMemoryType[ADL_MAX_PATH];
    long long iMemoryBandwidth;
} ADLMemoryInfo, *LPADLMemoryInfo;

typedef struct ADLMemoryRequired
{
    long long iMemoryReq;
    int iType;
    int iDisplayFeatureValue;
} ADLMemoryRequired, *LPADLMemoryRequired;

typedef struct ADLMemoryDisplayFeatures
{
    int iDisplayIndex;
    int iDisplayFeatureValue;
} ADLMemoryDisplayFeatures, *LPADLMemoryDisplayFeatures;

typedef struct ADLDDCInfo
{
    int  ulSize;
    int  ulSupportsDDC;
    int  ulManufacturerID;
    int  ulProductID;
    char cDisplayName[ADL_MAX_DISPLAY_NAME];
    int  ulMaxHResolution;
    int  ulMaxVResolution;
    int  ulMaxRefresh;
    int  ulPTMCx;
    int  ulPTMCy;
    int  ulPTMRefreshRate;
    int  ulDDCInfoFlag;
} ADLDDCInfo, *LPADLDDCInfo;

typedef struct ADLDDCInfo2
{
    int  ulSize;
    int  ulSupportsDDC;
    int  ulManufacturerID;
    int  ulProductID;
    char cDisplayName[ADL_MAX_DISPLAY_NAME];
    int  ulMaxHResolution;
    int  ulMaxVResolution;
    int  ulMaxRefresh;
    int  ulPTMCx;
    int  ulPTMCy;
    int  ulPTMRefreshRate;
    int  ulDDCInfoFlag;
    int bPackedPixelSupported;
    int iPanelPixelFormat;
    int  ulSerialID;
    int ulMinLuminanceData;
    int ulAvgLuminanceData;
    int ulMaxLuminanceData;

    int iSupportedTransferFunction;

    int iSupportedColorSpace;

    int iNativeDisplayChromaticityRedX;
    int iNativeDisplayChromaticityRedY;
    int iNativeDisplayChromaticityGreenX;
    int iNativeDisplayChromaticityGreenY;
    int iNativeDisplayChromaticityBlueX;
    int iNativeDisplayChromaticityBlueY;
    int iNativeDisplayChromaticityWhitePointX;
    int iNativeDisplayChromaticityWhitePointY;
    int iDiffuseScreenReflectance;
    int iSpecularScreenReflectance;
    int iSupportedHDR;
    int iFreesyncFlags;

    int ulMinLuminanceNoDimmingData;

    int ulMaxBacklightMaxLuminanceData;
    int ulMinBacklightMaxLuminanceData;
    int ulMaxBacklightMinLuminanceData;
    int ulMinBacklightMinLuminanceData;

    // Reserved for future use
    int iReserved[4];
} ADLDDCInfo2, *LPADLDDCInfo2;

typedef struct ADLGamma
{
    float fRed;
    float fGreen;
    float fBlue;
} ADLGamma, *LPADLGamma;

typedef struct ADLCustomMode
{
    int iFlags;
    int iModeWidth;
    int iModeHeight;
    int iBaseModeWidth;
    int iBaseModeHeight;
    int iRefreshRate;
} ADLCustomMode, *LPADLCustomMode;

typedef struct ADLGetClocksOUT
{
    long ulHighCoreClock;
    long ulHighMemoryClock;
    long ulHighVddc;
    long ulCoreMin;
    long ulCoreMax;
    long ulMemoryMin;
    long ulMemoryMax;
    long ulActivityPercent;
    long ulCurrentCoreClock;
    long ulCurrentMemoryClock;
    long ulReserved;
} ADLGetClocksOUT;

typedef struct ADLDisplayConfig
{
    long ulSize;
    long ulConnectorType;
    long ulDeviceData;
    long ulOverridedDeviceData;
    long ulReserved;
} ADLDisplayConfig;

typedef struct ADLDisplayID
{
    int iDisplayLogicalIndex;

    int iDisplayPhysicalIndex;

    int iDisplayLogicalAdapterIndex;

    int iDisplayPhysicalAdapterIndex;
} ADLDisplayID, *LPADLDisplayID;

typedef struct ADLDisplayInfo
{
    ADLDisplayID displayID;

    int  iDisplayControllerIndex;

    char strDisplayName[ADL_MAX_PATH];

    char strDisplayManufacturerName[ADL_MAX_PATH];

    int  iDisplayType;

    int  iDisplayOutputType;

    int  iDisplayConnector;

    int  iDisplayInfoMask;

    int  iDisplayInfoValue;
} ADLDisplayInfo, *LPADLDisplayInfo;

typedef struct ADLDisplayDPMSTInfo
{
    ADLDisplayID displayID;

    int    iTotalAvailableBandwidthInMpbs;
    int    iAllocatedBandwidthInMbps;

    // info from DAL DpMstSinkInfo
    char    strGlobalUniqueIdentifier[ADL_MAX_PATH];

    int        radLinkCount;
    int        iPhysicalConnectorID;

    char    rad[ADL_MAX_RAD_LINK_COUNT];
} ADLDisplayDPMSTInfo, *LPADLDisplayDPMSTInfo;

typedef struct ADLDisplayMode
{
    int  iPelsHeight;
    int  iPelsWidth;
    int  iBitsPerPel;
    int  iDisplayFrequency;
} ADLDisplayMode;

typedef struct ADLDetailedTiming
{
    int   iSize;
    short sTimingFlags;
    short sHTotal;
    short sHDisplay;
    short sHSyncStart;
    short sHSyncWidth;
    short sVTotal;
    short sVDisplay;
    short sVSyncStart;
    short sVSyncWidth;
    short sPixelClock;
    short sHOverscanRight;
    short sHOverscanLeft;
    short sVOverscanBottom;
    short sVOverscanTop;
    short sOverscan8B;
    short sOverscanGR;
} ADLDetailedTiming;

typedef struct ADLDisplayModeInfo
{
    int  iTimingStandard;
    int  iPossibleStandard;
    int  iRefreshRate;
    int  iPelsWidth;
    int  iPelsHeight;
    ADLDetailedTiming  sDetailedTiming;
} ADLDisplayModeInfo;

typedef struct ADLDisplayProperty
{
    int iSize;
    int iPropertyType;
    int iExpansionMode;
    int iSupport;
    int iCurrent;
    int iDefault;
} ADLDisplayProperty;

typedef struct ADLClockInfo
{
    int iCoreClock;
    int iMemoryClock;
} ADLClockInfo, *LPADLClockInfo;

typedef struct ADLI2C
{
    int iSize;
    int iLine;
    int iAddress;
    int iOffset;
    int iAction;
    int iSpeed;
    int iDataSize;
    char *pcData;
} ADLI2C;

typedef struct ADLDisplayEDIDData
{
    int iSize;
    int iFlag;
    int iEDIDSize;
    int iBlockIndex;
    char cEDIDData[ADL_MAX_EDIDDATA_SIZE];
    int iReserved[4];
}ADLDisplayEDIDData;

typedef struct ADLControllerOverlayInput
{
    int  iSize;
    int  iOverlayAdjust;
    int  iValue;
    int  iReserved;
} ADLControllerOverlayInput;

typedef struct ADLAdjustmentinfo
{
    int iDefault;
    int iMin;
    int iMax;
    int iStep;
} ADLAdjustmentinfo;

typedef struct ADLControllerOverlayInfo
{
    int                    iSize;
    ADLAdjustmentinfo        sOverlayInfo;
    int                    iReserved[3];
} ADLControllerOverlayInfo;

typedef struct ADLGLSyncModuleID
{
    int        iModuleID;
    int        iGlSyncGPUPort;
    int        iFWBootSectorVersion;
    int        iFWUserSectorVersion;
} ADLGLSyncModuleID , *LPADLGLSyncModuleID;

typedef struct ADLGLSyncPortCaps
{
    int        iPortType;
    int        iNumOfLEDs;
}ADLGLSyncPortCaps, *LPADLGLSyncPortCaps;

typedef struct ADLGLSyncGenlockConfig
{
    int        iValidMask;
    int        iSyncDelay;
    int        iFramelockCntlVector;
    int        iSignalSource;
    int        iSampleRate;
    int        iSyncField;
    int        iTriggerEdge;
    int        iScanRateCoeff;
}ADLGLSyncGenlockConfig, *LPADLGLSyncGenlockConfig;

typedef struct ADLGlSyncPortInfo
{
    int        iPortType;
    int        iNumOfLEDs;
    int        iPortState;
    int        iFrequency;
    int        iSignalType;
    int        iSignalSource;
} ADLGlSyncPortInfo, *LPADLGlSyncPortInfo;

typedef struct ADLGlSyncPortControl
{
    int        iPortType;
    int        iControlVector;
    int        iSignalSource;
} ADLGlSyncPortControl;

typedef struct ADLGlSyncMode
{
    int        iControlVector;
    int        iStatusVector;
    int        iGLSyncConnectorIndex;
} ADLGlSyncMode, *LPADLGlSyncMode;

typedef struct ADLGlSyncMode2
{
    int        iControlVector;
    int        iStatusVector;
    int        iGLSyncConnectorIndex;
    int        iDisplayIndex;
} ADLGlSyncMode2, *LPADLGlSyncMode2;

typedef struct  ADLInfoPacket
{
    char hb0;
    char hb1;
    char hb2;
    char sb[28];
}ADLInfoPacket;

typedef struct ADLAVIInfoPacket  //Valid user defined data/
{
    char bPB3_ITC;
    char bPB5;
}ADLAVIInfoPacket;

// Overdrive clock setting structure definition.

typedef struct ADLODClockSetting
{
    int iDefaultClock;
    int iCurrentClock;
    int iMaxClock;
    int iMinClock;
    int iRequestedClock;
    int iStepClock;
} ADLODClockSetting;

typedef struct ADLAdapterODClockInfo
{
    int iSize;
    int iFlags;
    ADLODClockSetting sMemoryClock;
    ADLODClockSetting sEngineClock;
} ADLAdapterODClockInfo;

typedef struct ADLAdapterODClockConfig
{
    int iSize;
    int iFlags;
    int iMemoryClock;
    int iEngineClock;
} ADLAdapterODClockConfig;

typedef struct ADLPMActivity
{
    int iSize;
    int iEngineClock;
    int iMemoryClock;
    int iVddc;
    int iActivityPercent;
    int iCurrentPerformanceLevel;
    int iCurrentBusSpeed;
    int iCurrentBusLanes;
    int iMaximumBusLanes;
    int iReserved;
} ADLPMActivity;

typedef struct ADLThermalControllerInfo
{
    int iSize;
    int iThermalDomain;
    int iDomainIndex;
    int iFlags;
} ADLThermalControllerInfo;

typedef struct ADLTemperature
{
    int iSize;
    int iTemperature;
} ADLTemperature;

typedef struct ADLFanSpeedInfo
{
    int iSize;
    int iFlags;
    int iMinPercent;
    int iMaxPercent;
    int iMinRPM;
    int iMaxRPM;
} ADLFanSpeedInfo;

typedef struct ADLFanSpeedValue
{
    int iSize;
    int iSpeedType;
    int iFanSpeed;
    int iFlags;
} ADLFanSpeedValue;

typedef struct ADLODParameterRange
{
    int iMin;
    int iMax;
    int iStep;
} ADLODParameterRange;

typedef struct ADLODParameters
{
    int iSize;
    int iNumberOfPerformanceLevels;
    int iActivityReportingSupported;
    int iDiscretePerformanceLevels;
    int iReserved;
    ADLODParameterRange sEngineClock;
    ADLODParameterRange sMemoryClock;
    ADLODParameterRange sVddc;
} ADLODParameters;

typedef struct ADLODPerformanceLevel
{
    int iEngineClock;
    int iMemoryClock;
    int iVddc;
} ADLODPerformanceLevel;

typedef struct ADLODPerformanceLevels
{
    int iSize;
    int iReserved;
    ADLODPerformanceLevel aLevels [1];
} ADLODPerformanceLevels;

typedef struct ADLCrossfireComb
{
    int iNumLinkAdapter;
    int iAdaptLink[3];
} ADLCrossfireComb;

typedef struct ADLCrossfireInfo
{
    int iErrorCode;
    int iState;
    int iSupported;
} ADLCrossfireInfo;

typedef struct ADLBiosInfo
{
    char strPartNumber[ADL_MAX_PATH];
    char strVersion[ADL_MAX_PATH];
    char strDate[ADL_MAX_PATH];
} ADLBiosInfo, *LPADLBiosInfo;

typedef struct ADLAdapterLocation
{
    int iBus;
    int iDevice;
    int iFunction;
} ADLAdapterLocation,ADLBdf;

typedef struct ADLVersionsInfo
{
    char strDriverVer[ADL_MAX_PATH];
    char strCatalystVersion[ADL_MAX_PATH];
    char strCatalystWebLink[ADL_MAX_PATH];
} ADLVersionsInfo, *LPADLVersionsInfo;

typedef struct ADLVersionsInfoX2
{
    char strDriverVer[ADL_MAX_PATH];
    char strCatalystVersion[ADL_MAX_PATH];
    char strCrimsonVersion[ADL_MAX_PATH];
    char strCatalystWebLink[ADL_MAX_PATH];
} ADLVersionsInfoX2, *LPADLVersionsInfoX2;

typedef struct ADLMVPUCaps
{
    int iSize;
    int iAdapterCount;
    int iPossibleMVPUMasters;
    int iPossibleMVPUSlaves;
    char cAdapterPath[ADL_DL_MAX_MVPU_ADAPTERS][ADL_DL_MAX_REGISTRY_PATH];
} ADLMVPUCaps;

typedef struct ADLMVPUStatus
{
    int iSize;
    int iActiveAdapterCount;
    int iStatus;
    ADLAdapterLocation aAdapterLocation[ADL_DL_MAX_MVPU_ADAPTERS];
} ADLMVPUStatus;

// Displays Manager structures

typedef struct ADLActivatableSource
{
    int iAdapterIndex;
    int iNumActivatableSources;
    int iActivatableSourceMask;
    int iActivatableSourceValue;
} ADLActivatableSource, *LPADLActivatableSource;


typedef struct ADLMode
{
    int iAdapterIndex;
    ADLDisplayID displayID;
    int iXPos;
    int iYPos;
    int iXRes;
    int iYRes;
    int iColourDepth;
    float fRefreshRate;
    int iOrientation;
    int iModeFlag;
    int iModeMask;
    int iModeValue;
} ADLMode, *LPADLMode;

typedef struct ADLDisplayTarget
{
    ADLDisplayID displayID;

    int iDisplayMapIndex;

    int  iDisplayTargetMask;

    int  iDisplayTargetValue;
} ADLDisplayTarget, *LPADLDisplayTarget;

typedef struct tagADLBezelTransientMode
{
    int iAdapterIndex;

    int iSLSMapIndex;

    int iSLSModeIndex;

    ADLMode displayMode;

    int  iNumBezelOffset;

    int  iFirstBezelOffsetArrayIndex;

    int  iSLSBezelTransientModeMask;

    int  iSLSBezelTransientModeValue;
} ADLBezelTransientMode, *LPADLBezelTransientMode;

typedef struct ADLAdapterDisplayCap
{
    int iAdapterIndex;
    int  iAdapterDisplayCapMask;
    int  iAdapterDisplayCapValue;
} ADLAdapterDisplayCap, *LPADLAdapterDisplayCap;

typedef struct ADLDisplayMap
{
    int iDisplayMapIndex;

    ADLMode displayMode;

    int iNumDisplayTarget;

    int iFirstDisplayTargetArrayIndex;

    int  iDisplayMapMask;

    int  iDisplayMapValue;
} ADLDisplayMap, *LPADLDisplayMap;

typedef struct ADLPossibleMap
{
    int iIndex;
    int iAdapterIndex;
    int iNumDisplayMap;
    ADLDisplayMap* displayMap;
    int iNumDisplayTarget;
    ADLDisplayTarget* displayTarget;
} ADLPossibleMap, *LPADLPossibleMap;

typedef struct ADLPossibleMapping
{
    int iDisplayIndex;
    int iDisplayControllerIndex;
    int iDisplayMannerSupported;
} ADLPossibleMapping, *LPADLPossibleMapping;

typedef struct ADLPossibleMapResult
{
    int iIndex;
    // The bit mask identifies the number of bits   PossibleMapResult is currently using. It will be the sum all the bit definitions defined in ADL_DISPLAY_POSSIBLEMAPRESULT_VALID.
    int iPossibleMapResultMask;
    int iPossibleMapResultValue;
} ADLPossibleMapResult, *LPADLPossibleMapResult;

typedef struct ADLSLSGrid
{
    int iAdapterIndex;

    int  iSLSGridIndex;

    int  iSLSGridRow;

    int  iSLSGridColumn;

    int  iSLSGridMask;

    int  iSLSGridValue;
} ADLSLSGrid, *LPADLSLSGrid;

typedef struct    ADLSLSMap
{
    int iAdapterIndex;

    int iSLSMapIndex;

    ADLSLSGrid grid;

    int  iSurfaceMapIndex;

    int iOrientation;

    int  iNumSLSTarget;

    int  iFirstSLSTargetArrayIndex;

    int  iNumNativeMode;

    int  iFirstNativeModeArrayIndex;

    int  iNumBezelMode;

    int  iFirstBezelModeArrayIndex;

    int  iNumBezelOffset;

    int  iFirstBezelOffsetArrayIndex;

    int  iSLSMapMask;

    int  iSLSMapValue;
} ADLSLSMap, *LPADLSLSMap;

typedef struct ADLSLSOffset
{
    int iAdapterIndex;

    int iSLSMapIndex;

    ADLDisplayID displayID;

    int iBezelModeIndex;

    int iBezelOffsetX;

    int iBezelOffsetY;

    int iDisplayWidth;

    int iDisplayHeight;

    int iBezelOffsetMask;

    int  iBezelffsetValue;
} ADLSLSOffset, *LPADLSLSOffset;

typedef struct ADLSLSMode
{
    int iAdapterIndex;

    int iSLSMapIndex;

    int iSLSModeIndex;

    ADLMode displayMode;

    int iSLSNativeModeMask;

    int iSLSNativeModeValue;
} ADLSLSMode, *LPADLSLSMode;

typedef struct ADLPossibleSLSMap
{
    int iSLSMapIndex;

    int iNumSLSMap;

    ADLSLSMap* lpSLSMap;

    int iNumSLSTarget;

    ADLDisplayTarget* lpDisplayTarget;
} ADLPossibleSLSMap, *LPADLPossibleSLSMap;

typedef struct ADLSLSTarget
{
    int iAdapterIndex;

    int iSLSMapIndex;

    ADLDisplayTarget displayTarget;

    int iSLSGridPositionX;

    int iSLSGridPositionY;

    ADLMode viewSize;

    int iSLSTargetMask;

    int iSLSTargetValue;
} ADLSLSTarget, *LPADLSLSTarget;

typedef struct ADLBezelOffsetSteppingSize
{
    int iAdapterIndex;

    int iSLSMapIndex;

    int iBezelOffsetSteppingSizeX;

    int iBezelOffsetSteppingSizeY;

    int iBezelOffsetSteppingSizeMask;

    int iBezelOffsetSteppingSizeValue;
} ADLBezelOffsetSteppingSize, *LPADLBezelOffsetSteppingSize;

typedef struct ADLSLSOverlappedMode
{
    ADLMode SLSMode;
    int iNumSLSTarget;
    int iFirstTargetArrayIndex;
}ADLSLSTargetOverlap, *LPADLSLSTargetOverlap;

typedef struct ADLPXConfigCaps
{
    int iAdapterIndex;

    int  iPXConfigCapMask;

    int  iPXConfigCapValue;
} ADLPXConfigCaps, *LPADLPXConfigCaps;

typedef enum ADLPxType
{
    //Not AMD related PX/HG or not PX or HG at all
    ADL_PX_NONE = 0,
    //A+A PX
    ADL_SWITCHABLE_AMDAMD = 1,
    // A+A HG
    ADL_HG_AMDAMD = 2,
    //A+I PX
    ADL_SWITCHABLE_AMDOTHER = 3,
    //A+I HG
    ADL_HG_AMDOTHER = 4,
}ADLPxType;

typedef struct ADLApplicationData
{
    char strPathName[ADL_MAX_PATH];
    char strFileName[ADL_APP_PROFILE_FILENAME_LENGTH];
    char strTimeStamp[ADL_APP_PROFILE_TIMESTAMP_LENGTH];
    char strVersion[ADL_APP_PROFILE_VERSION_LENGTH];
}ADLApplicationData;

typedef struct ADLApplicationDataX2
{
    wchar_t strPathName[ADL_MAX_PATH];
    wchar_t strFileName[ADL_APP_PROFILE_FILENAME_LENGTH];
    wchar_t strTimeStamp[ADL_APP_PROFILE_TIMESTAMP_LENGTH];
    wchar_t strVersion[ADL_APP_PROFILE_VERSION_LENGTH];
}ADLApplicationDataX2;

typedef struct ADLApplicationDataX3
{
    wchar_t strPathName[ADL_MAX_PATH];
    wchar_t strFileName[ADL_APP_PROFILE_FILENAME_LENGTH];
    wchar_t strTimeStamp[ADL_APP_PROFILE_TIMESTAMP_LENGTH];
    wchar_t strVersion[ADL_APP_PROFILE_VERSION_LENGTH];
    //Application Process id
    unsigned int iProcessId;
}ADLApplicationDataX3;

typedef struct PropertyRecord
{
    char strName [ADL_APP_PROFILE_PROPERTY_LENGTH];
    ADLProfilePropertyType eType;
    int iDataSize;
    unsigned char uData[1];
}PropertyRecord;

typedef struct ADLApplicationProfile
{
    int iCount;
    PropertyRecord record[1];
}ADLApplicationProfile;

typedef struct ADLPowerControlInfo
{
    int iMinValue;
    int iMaxValue;
    int iStepValue;
} ADLPowerControlInfo;

typedef struct ADLControllerMode
{
    int iModifiers;

    int iViewPositionCx;

    int iViewPositionCy;

    int iViewPanLockLeft;

    int iViewPanLockRight;

    int iViewPanLockTop;

    int iViewPanLockBottom;

    int iViewResolutionCx;

    int iViewResolutionCy;
}ADLControllerMode;

typedef struct ADLDisplayIdentifier
{
    long ulDisplayIndex;

    long ulManufacturerId;

    long ulProductId;

    long ulSerialNo;
} ADLDisplayIdentifier;

typedef struct ADLOD6ParameterRange
{
    int     iMin;
    int     iMax;
    int     iStep;
} ADLOD6ParameterRange;

typedef struct ADLOD6Capabilities
{
    int     iCapabilities;
    int     iSupportedStates;
    int     iNumberOfPerformanceLevels;
    ADLOD6ParameterRange     sEngineClockRange;
    ADLOD6ParameterRange     sMemoryClockRange;

    int     iExtValue;
    int     iExtMask;
} ADLOD6Capabilities;

typedef struct ADLOD6PerformanceLevel
{
    int iEngineClock;
    int iMemoryClock;
} ADLOD6PerformanceLevel;

typedef struct ADLOD6StateInfo
{
    int     iNumberOfPerformanceLevels;

    int     iExtValue;
    int     iExtMask;

    ADLOD6PerformanceLevel aLevels [1];
} ADLOD6StateInfo;

typedef struct ADLOD6CurrentStatus
{
    int     iEngineClock;
    int     iMemoryClock;
    int     iActivityPercent;
    int     iCurrentPerformanceLevel;
    int     iCurrentBusSpeed;
    int     iCurrentBusLanes;
    int     iMaximumBusLanes;

    int     iExtValue;
    int     iExtMask;
} ADLOD6CurrentStatus;

typedef struct ADLOD6ThermalControllerCaps
{
    int     iCapabilities;
    int     iFanMinPercent;
    int     iFanMaxPercent;
    int     iFanMinRPM;
    int     iFanMaxRPM;

    int     iExtValue;
    int     iExtMask;
} ADLOD6ThermalControllerCaps;

typedef struct ADLOD6FanSpeedInfo
{
    int     iSpeedType;
    int     iFanSpeedPercent;
    int        iFanSpeedRPM;

    int     iExtValue;
    int     iExtMask;
} ADLOD6FanSpeedInfo;

typedef struct ADLOD6FanSpeedValue
{
    int     iSpeedType;
    int     iFanSpeed;

    int     iExtValue;
    int     iExtMask;
} ADLOD6FanSpeedValue;

typedef struct ADLOD6PowerControlInfo
{
    int     iMinValue;
    int     iMaxValue;
    int     iStepValue;

    int     iExtValue;
    int     iExtMask;
} ADLOD6PowerControlInfo;

typedef struct ADLOD6VoltageControlInfo
{
    int     iMinValue;
    int     iMaxValue;
    int     iStepValue;

    int     iExtValue;
    int     iExtMask;
} ADLOD6VoltageControlInfo;

typedef struct ADLECCData
{
    // Single error count - count of errors that can be corrected
    int iSec;
    // Double error detect - count of errors that cannot be corrected
    int iDed;
} ADLECCData;

typedef void *ADL_CONTEXT_HANDLE;

typedef void *ADL_FRAME_DURATION_HANDLE;

typedef struct ADLDisplayModeX2
{
    int  iWidth;
    int  iHeight;
    int  iScanType;
    int  iRefreshRate;
    int  iTimingStandard;
} ADLDisplayModeX2;

typedef enum ADLAppProcessState
{
    APP_PROC_INVALID = 0,          // Invalid Application
    APP_PROC_PREMPTION = 1,          // The Application is being set up for Process Creation
    APP_PROC_CREATION = 2,          // The Application's Main Process is created by the OS
    APP_PROC_READ = 3,          // The Application's Data is ready to be read
    APP_PROC_WAIT = 4,          // The Application is waiting for Timeout or Notification to Resume
    APP_PROC_RUNNING = 5,          // The Application is running
    APP_PROC_TERMINATE = 6           // The Application is about to terminate
}ADLAppProcessState;

typedef enum ADLAppInterceptionListType
{
    ADL_INVALID_FORMAT = 0,
    ADL_IMAGEFILEFORMAT = 1,
    ADL_ENVVAR = 2
}ADLAppInterceptionListType;

typedef struct ADLAppInterceptionInfo
{
    wchar_t                     AppName[ADL_MAX_PATH]; // the file name of the application or env var
    unsigned int                ProcessId;
    ADLAppInterceptionListType  AppFormat;
    ADLAppProcessState          AppState;
} ADLAppInterceptionInfo;

typedef enum ADL_AP_DATABASE // same as _SHARED_AP_DATABASE in "inc/shared/shared_escape.h"
{
    ADL_AP_DATABASE__SYSTEM,
    ADL_AP_DATABASE__USER,
    ADL_AP_DATABASE__OEM
} ADL_AP_DATABASE;

typedef struct ADLAppInterceptionInfoX2
{
    wchar_t                     AppName[ADL_MAX_PATH]; // the file name of the application or env var
    unsigned int                ProcessId;
    unsigned int                WaitForResumeNeeded;
    wchar_t                     CommandLine[ADL_MAX_PATH]; // The command line on app start/stop event
    ADLAppInterceptionListType  AppFormat;
    ADLAppProcessState          AppState;
} ADLAppInterceptionInfoX2;

typedef struct ADLAppInterceptionInfoX3
{
    wchar_t                     AppName[ADL_MAX_PATH]; // the file name of the application or env var
    unsigned int                ProcessId;
    unsigned int                WaitForResumeNeeded;
    unsigned int                RayTracingStatus; // returns the Ray Tracing status if it is enabled atleast once in session.
    wchar_t                     CommandLine[ADL_MAX_PATH]; // The command line on app start/stop event
    ADLAppInterceptionListType  AppFormat;
    ADLAppProcessState          AppState;
} ADLAppInterceptionInfoX3;

typedef struct ADLPropertyRecordCreate
{
    wchar_t * strPropertyName;
    ADLProfilePropertyType eType;
    // Value of the property
    wchar_t * strPropertyValue;
} ADLPropertyRecordCreate;

typedef struct ADLApplicationRecord
{
    wchar_t * strTitle;
    wchar_t * strPathName;
    wchar_t * strFileName;
    wchar_t * strVersion;
    wchar_t * strNotes;
    wchar_t * strArea;
    wchar_t * strProfileName;
    // Source where this application record come from
    ADL_AP_DATABASE recordSource;
} ADLApplicationRecord;

typedef struct ADLOD6CapabilitiesEx
{
    int iCapabilities;
    int iSupportedStates;
    ADLOD6ParameterRange sEngineClockPercent;
    ADLOD6ParameterRange sMemoryClockPercent;
    ADLOD6ParameterRange sPowerControlPercent;
    int iExtValue;
    int iExtMask;
} ADLOD6CapabilitiesEx;

typedef struct ADLOD6StateEx
{
    int iEngineClockPercent;
    int iMemoryClockPercent;
    int iPowerControlPercent;
    int iExtValue;
    int iExtMask;
} ADLOD6StateEx;

typedef struct ADLOD6MaxClockAdjust
{
    int iEngineClockMax;
    int iMemoryClockMax;
    int iExtValue;
    int iExtMask;
} ADLOD6MaxClockAdjust;

typedef struct ADLConnectorInfo
{
    int iConnectorIndex;
    int iConnectorId;
    int iSlotIndex;
    int iType;
    int iOffset;
    int iLength;
} ADLConnectorInfo;

typedef struct ADLBracketSlotInfo
{
    int iSlotIndex;
    int iLength;
    int iWidth;
} ADLBracketSlotInfo;

typedef struct ADLMSTRad
{
    int iLinkNumber;
    char rad[ADL_MAX_RAD_LINK_COUNT];
} ADLMSTRad;

typedef struct ADLDevicePort
{
    int iConnectorIndex;
    ADLMSTRad aMSTRad;
} ADLDevicePort;

typedef struct ADLSupportedConnections
{
    int iSupportedConnections;
    int iSupportedProperties[ADL_MAX_CONNECTION_TYPES];
} ADLSupportedConnections;

typedef struct ADLConnectionState
{
    int iEmulationStatus;
    int iEmulationMode;
    int iDisplayIndex;
} ADLConnectionState;

typedef struct ADLConnectionProperties
{
    //Bit vector. Represents actual properties. Supported properties for specific connection type. \ref define_connection_properties
    int iValidProperties;
    //Bitrate(in MHz). Could be used for MST branch, DP or DP active dongle. \ref define_linkrate_constants
    int iBitrate;
    //Number of lanes in DP connection. \ref define_lanecount_constants
    int iNumberOfLanes;
    //Color depth(in bits). \ref define_colordepth_constants
    int iColorDepth;
    //3D capabilities. It could be used for some dongles. For instance: alternate framepack. Value of this property is bit vector.
    int iStereo3DCaps;
    int iOutputBandwidth;
} ADLConnectionProperties;

typedef struct ADLConnectionData
{
    int iConnectionType;
    ADLConnectionProperties aConnectionProperties;
    int iNumberofPorts;
    int iActiveConnections;
    int iDataSize;
    char EdidData[ADL_MAX_DISPLAY_EDID_DATA_SIZE];
} ADLConnectionData;

typedef struct ADLAdapterCapsX2
{
    int iAdapterID;
    int iNumControllers;
    int iNumDisplays;
    int iNumOverlays;
    int iNumOfGLSyncConnectors;
    int iCapsMask;
    int iCapsValue;
    int iNumConnectors;
}ADLAdapterCapsX2;

typedef enum ADL_ERROR_RECORD_SEVERITY
{
    ADL_GLOBALLY_UNCORRECTED  = 1,
    ADL_LOCALLY_UNCORRECTED   = 2,
    ADL_DEFFERRED             = 3,
    ADL_CORRECTED             = 4
}ADL_ERROR_RECORD_SEVERITY;

typedef union _ADL_ECC_EDC_FLAG
{
    struct
    {
        unsigned int isEccAccessing        : 1;
        unsigned int reserved              : 31;
    }bits;
    unsigned int u32All;
}ADL_ECC_EDC_FLAG;

typedef struct ADLErrorRecord
{
    // Severity of error
    ADL_ERROR_RECORD_SEVERITY Severity;

    // Is the counter valid?
    int  countValid;

    // Counter value, if valid
    unsigned int count;

    // Is the location information valid?
    int locationValid;

    // Physical location of error
    unsigned int CU; // CU number on which error occurred, if known
    char StructureName[32]; // e.g. LDS, TCC, etc.

    // Time of error record creation (e.g. time of query, or time of poison interrupt)
    char tiestamp[32];

    unsigned int padding[3];
}ADLErrorRecord;

typedef enum ADL_EDC_BLOCK_ID
{
    ADL_EDC_BLOCK_ID_SQCIS = 1,
    ADL_EDC_BLOCK_ID_SQCDS = 2,
    ADL_EDC_BLOCK_ID_SGPR  = 3,
    ADL_EDC_BLOCK_ID_VGPR  = 4,
    ADL_EDC_BLOCK_ID_LDS   = 5,
    ADL_EDC_BLOCK_ID_GDS   = 6,
    ADL_EDC_BLOCK_ID_TCL1  = 7,
    ADL_EDC_BLOCK_ID_TCL2  = 8
}ADL_EDC_BLOCK_ID;

typedef enum ADL_ERROR_INJECTION_MODE
{
    ADL_ERROR_INJECTION_MODE_SINGLE      = 1,
    ADL_ERROR_INJECTION_MODE_MULTIPLE    = 2,
    ADL_ERROR_INJECTION_MODE_ADDRESS     = 3
}ADL_ERROR_INJECTION_MODE;

typedef union _ADL_ERROR_PATTERN
{
    struct
    {
        unsigned long  EccInjVector         :  16;
        unsigned long  EccInjEn             :  9;
        unsigned long  EccBeatEn            :  4;
        unsigned long  EccChEn              :  4;
        unsigned long  reserved             :  31;
    } bits;
    unsigned long long u64Value;
} ADL_ERROR_PATTERN;

typedef struct ADL_ERROR_INJECTION_DATA
{
    unsigned long long errorAddress;
    ADL_ERROR_PATTERN errorPattern;
}ADL_ERROR_INJECTION_DATA;

typedef struct ADLErrorInjection
{
    ADL_EDC_BLOCK_ID blockId;
    ADL_ERROR_INJECTION_MODE errorInjectionMode;
}ADLErrorInjection;

typedef struct ADLErrorInjectionX2
{
    ADL_EDC_BLOCK_ID blockId;
    ADL_ERROR_INJECTION_MODE errorInjectionMode;
    ADL_ERROR_INJECTION_DATA errorInjectionData;
}ADLErrorInjectionX2;

typedef struct ADLFreeSyncCap
{
    int iCaps;
    int iMinRefreshRateInMicroHz;
    int iMaxRefreshRateInMicroHz;
    unsigned char ucLabelIndex;
    char cReserved[3];
    int iReserved[4];
} ADLFreeSyncCap;

typedef struct ADLDceSettings
{
    DceSettingsType type;                       // Defines which structure is in the union below
    union
    {
        struct
        {
            bool qualityDetectionEnabled;
        } HdmiLq;
        struct
        {
            DpLinkRate linkRate;                // Read-only
            unsigned int numberOfActiveLanes;   // Read-only
            unsigned int numberofTotalLanes;    // Read-only
            int relativePreEmphasis;            // Allowable values are -2 to +2
            int relativeVoltageSwing;           // Allowable values are -2 to +2
            int persistFlag;
        } DpLink;
        struct
        {
            bool linkProtectionEnabled;         // Read-only
        } Protection;
    } Settings;
    int iReserved[15];
} ADLDceSettings;

typedef struct ADLGraphicCoreInfo
{
    int iGCGen;

    union
    {
        int iNumCUs;
        int iNumWGPs;
    };

    union
    {
        int iNumPEsPerCU;
        int iNumPEsPerWGP;
    };

    int iNumSIMDs;

    int iNumROPs;

    int iReserved[11];
}ADLGraphicCoreInfo;

typedef struct ADLODNParameterRange
{
    int     iMode;
    int     iMin;
    int     iMax;
    int     iStep;
    int     iDefault;
} ADLODNParameterRange;

typedef struct ADLODNCapabilities
{
    int     iMaximumNumberOfPerformanceLevels;
    ADLODNParameterRange     sEngineClockRange;
    ADLODNParameterRange     sMemoryClockRange;
    ADLODNParameterRange     svddcRange;
    ADLODNParameterRange     power;
    ADLODNParameterRange     powerTuneTemperature;
    ADLODNParameterRange     fanTemperature;
    ADLODNParameterRange     fanSpeed;
    ADLODNParameterRange     minimumPerformanceClock;
} ADLODNCapabilities;

typedef struct ADLODNCapabilitiesX2
{
    int     iMaximumNumberOfPerformanceLevels;
    int iFlags;
    ADLODNParameterRange     sEngineClockRange;
    ADLODNParameterRange     sMemoryClockRange;
    ADLODNParameterRange     svddcRange;
    ADLODNParameterRange     power;
    ADLODNParameterRange     powerTuneTemperature;
    ADLODNParameterRange     fanTemperature;
    ADLODNParameterRange     fanSpeed;
    ADLODNParameterRange     minimumPerformanceClock;
    ADLODNParameterRange throttleNotificaion;
    ADLODNParameterRange autoSystemClock;
} ADLODNCapabilitiesX2;

typedef struct ADLODNPerformanceLevel
{
    int iClock;
    int iVddc;
    int iEnabled;
} ADLODNPerformanceLevel;

typedef struct ADLODNPerformanceLevels
{
    int iSize;
    //Automatic/manual
    int iMode;
    int iNumberOfPerformanceLevels;
    ADLODNPerformanceLevel aLevels[1];
} ADLODNPerformanceLevels;

typedef struct ADLODNFanControl
{
    int iMode;
    int iFanControlMode;
    int iCurrentFanSpeedMode;
    int iCurrentFanSpeed;
    int iTargetFanSpeed;
    int iTargetTemperature;
    int iMinPerformanceClock;
    int iMinFanLimit;
} ADLODNFanControl;

typedef struct ADLODNPowerLimitSetting
{
    int iMode;
    int iTDPLimit;
    int iMaxOperatingTemperature;
} ADLODNPowerLimitSetting;

typedef struct ADLODNPerformanceStatus
{
    int iCoreClock;
    int iMemoryClock;
    int iDCEFClock;
    int iGFXClock;
    int iUVDClock;
    int iVCEClock;
    int iGPUActivityPercent;
    int iCurrentCorePerformanceLevel;
    int iCurrentMemoryPerformanceLevel;
    int iCurrentDCEFPerformanceLevel;
    int iCurrentGFXPerformanceLevel;
    int iUVDPerformanceLevel;
    int iVCEPerformanceLevel;
    int iCurrentBusSpeed;
    int iCurrentBusLanes;
    int iMaximumBusLanes;
    int iVDDC;
    int iVDDCI;
} ADLODNPerformanceStatus;

typedef struct ADLODNPerformanceLevelX2
{
    int iClock;
    int iVddc;
    int iEnabled;
    int iControl;
} ADLODNPerformanceLevelX2;

typedef struct ADLODNPerformanceLevelsX2
{
    int iSize;
    //Automatic/manual
    int iMode;
    int iNumberOfPerformanceLevels;
    ADLODNPerformanceLevelX2 aLevels[1];
} ADLODNPerformanceLevelsX2;

typedef enum ADLODNCurrentPowerType
{
    ODN_GPU_TOTAL_POWER = 0,
    ODN_GPU_PPT_POWER,
    ODN_GPU_SOCKET_POWER,
    ODN_GPU_CHIP_POWER
} ADLODNCurrentPowerType;

// in/out: CWDDEPM_CURRENTPOWERPARAMETERS
typedef struct ADLODNCurrentPowerParameters
{
    int   size;
    ADLODNCurrentPowerType   powerType;
    int  currentPower;
} ADLODNCurrentPowerParameters;

//ODN Ext range data structure
typedef struct ADLODNExtSingleInitSetting
{
    int mode;
    int minValue;
    int maxValue;
    int step;
    int defaultValue;
} ADLODNExtSingleInitSetting;

//OD8 Ext range data structure
typedef struct ADLOD8SingleInitSetting
{
    int featureID;
    int minValue;
    int maxValue;
    int defaultValue;
} ADLOD8SingleInitSetting;

typedef struct ADLOD8InitSetting
{
    int count;
    int overdrive8Capabilities;
    ADLOD8SingleInitSetting  od8SettingTable[OD8_COUNT];
} ADLOD8InitSetting;

typedef struct ADLOD8CurrentSetting
{
    int count;
    int Od8SettingTable[OD8_COUNT];
} ADLOD8CurrentSetting;


typedef struct ADLOD8SingleSetSetting
{
    int value;
    int requested;      // 0 - default , 1 - requested
    int reset;          // 0 - do not reset , 1 - reset setting back to default
} ADLOD8SingleSetSetting;

typedef struct ADLOD8SetSetting
{
    int count;
    ADLOD8SingleSetSetting  od8SettingTable[OD8_COUNT];
} ADLOD8SetSetting;

typedef struct ADLSingleSensorData
{
    int supported;
    int  value;
} ADLSingleSensorData;

typedef struct ADLPMLogDataOutput
{
    int size;
    ADLSingleSensorData sensors[ADL_PMLOG_MAX_SENSORS];
}ADLPMLogDataOutput;

typedef struct ADLPPLogSettings
{
    int BreakOnAssert;
    int BreakOnWarn;
    int LogEnabled;
    int LogFieldMask;
    int LogDestinations;
    int LogSeverityEnabled;
    int LogSourceMask;
    int PowerProfilingEnabled;
    int PowerProfilingTimeInterval;
}ADLPPLogSettings;

typedef struct ADLFPSSettingsOutput
{
    int ulSize;
    int bACFPSEnabled;
    int bDCFPSEnabled;
    int ulACFPSCurrent;
    int ulDCFPSCurrent;
    int ulACFPSMaximum;
    int ulACFPSMinimum;
    int ulDCFPSMaximum;
    int ulDCFPSMinimum;
} ADLFPSSettingsOutput;

typedef struct ADLFPSSettingsInput
{
    int ulSize;
    int bGlobalSettings;
    int ulACFPSCurrent;
    int ulDCFPSCurrent;
    int ulReserved[6];
} ADLFPSSettingsInput;

enum { ADL_PMLOG_MAX_SUPPORTED_SENSORS = 256 };

typedef struct ADLPMLogSupportInfo
{
    unsigned short usSensors[ADL_PMLOG_MAX_SUPPORTED_SENSORS];
    int ulReserved[16];
} ADLPMLogSupportInfo;

typedef struct ADLPMLogStartInput
{
    unsigned short usSensors[ADL_PMLOG_MAX_SUPPORTED_SENSORS];
    unsigned long ulSampleRate;
    int ulReserved[15];
} ADLPMLogStartInput;

typedef struct ADLPMLogData
{
    unsigned int ulVersion;
    unsigned int ulActiveSampleRate;
    unsigned long long ulLastUpdated;
    unsigned int ulValues[ADL_PMLOG_MAX_SUPPORTED_SENSORS][2];
    unsigned int ulReserved[256];
} ADLPMLogData;

typedef struct ADLPMLogStartOutput
{
    union
    {
        void* pLoggingAddress;
        unsigned long long ptr_LoggingAddress;
    };
    int ulReserved[14];
} ADLPMLogStartOutput;

typedef struct ADLPMLogSensorLimits
{
    int SensorLimits[ADL_PMLOG_MAX_SENSORS][2]; //index 0: min, 1: max
} ADLPMLogSensorLimits;


typedef struct ADLRASGetErrorCountsInput
{
    unsigned int                Reserved[16];
} ADLRASGetErrorCountsInput;


typedef struct ADLRASGetErrorCountsOutput
{
    unsigned int                CorrectedErrors;    // includes both DRAM and SRAM ECC
    unsigned int                UnCorrectedErrors;  // includes both DRAM and SRAM ECC
    unsigned int                Reserved[14];
} ADLRASGetErrorCountsOutput;


typedef struct ADLRASGetErrorCounts
{
    unsigned int                InputSize;
    ADLRASGetErrorCountsInput   Input;
    unsigned int                OutputSize;
    ADLRASGetErrorCountsOutput  Output;
} ADLRASGetErrorCounts;


typedef struct ADLRASResetErrorCountsInput
{
    unsigned int                Reserved[8];
} ADLRASResetErrorCountsInput;


typedef struct ADLRASResetErrorCountsOutput
{
    unsigned int                Reserved[8];
} ADLRASResetErrorCountsOutput;


typedef struct ADLRASResetErrorCounts
{
    unsigned int                    InputSize;
    ADLRASResetErrorCountsInput     Input;
    unsigned int                    OutputSize;
    ADLRASResetErrorCountsOutput    Output;
} ADLRASResetErrorCounts;


typedef struct ADLRASErrorInjectonInput
{
    unsigned long long Address;
    ADL_RAS_INJECTION_METHOD Value;
    ADL_RAS_BLOCK_ID BlockId;
    ADL_RAS_ERROR_TYPE InjectErrorType;
    ADL_MEM_SUB_BLOCK_ID SubBlockIndex;
    unsigned int padding[9];
} ADLRASErrorInjectonInput;


typedef struct ADLRASErrorInjectionOutput
{
    unsigned int ErrorInjectionStatus;
    unsigned int padding[15];
} ADLRASErrorInjectionOutput;


typedef struct ADLRASErrorInjection
{
    unsigned int                           InputSize;
    ADLRASErrorInjectonInput               Input;
    unsigned int                           OutputSize;
    ADLRASErrorInjectionOutput             Output;
} ADLRASErrorInjection;

typedef struct ADLSGApplicationInfo
{
    wchar_t strFileName[ADL_MAX_PATH];
    wchar_t strFilePath[ADL_MAX_PATH];
    wchar_t strVersion[ADL_MAX_PATH];
    long long int timeStamp;
    unsigned int iProfileExists;
    unsigned int iGPUAffinity;
    ADLBdf GPUBdf;
} ADLSGApplicationInfo;

enum { ADLPreFlipPostProcessingInfoInvalidLUTIndex = 0xFFFFFFFF };

enum ADLPreFlipPostProcessingLUTAlgorithm
{
    ADLPreFlipPostProcessingLUTAlgorithm_Default = 0,
    ADLPreFlipPostProcessingLUTAlgorithm_Full,
    ADLPreFlipPostProcessingLUTAlgorithm_Approximation
};

typedef struct ADLPreFlipPostProcessingInfo
{
    int ulSize;
    int bEnabled;
    int ulSelectedLUTIndex;
    int ulSelectedLUTAlgorithm;
    int ulReserved[12];
} ADLPreFlipPostProcessingInfo;

typedef struct ADL_ERROR_REASON
{
    int boost; //ON, when boost is Enabled
    int delag; //ON, when delag is Enabled
    int chill; //ON, when chill is Enabled
    int proVsr; //ON, when proVsr is Enabled
}ADL_ERROR_REASON;

typedef struct ADL_DELAG_NOTFICATION_REASON
{
    int HotkeyChanged; //Set when Hotkey value is changed
    int GlobalEnableChanged; //Set when Global enable value is changed
    int GlobalLimitFPSChanged; //Set when Global enable value is changed
}ADL_DELAG_NOTFICATION_REASON;

typedef struct ADL_DELAG_SETTINGS
{
    int Hotkey; // Hotkey value
    int GlobalEnable; //Global enable value
    int GlobalLimitFPS; //Global Limit FPS
    int GlobalLimitFPS_MinLimit; //Gloabl Limit FPS slider min limit value
    int GlobalLimitFPS_MaxLimit; //Gloabl Limit FPS slider max limit value
    int GlobalLimitFPS_Step; //Gloabl Limit FPS step  value
}ADL_DELAG_SETTINGS;

typedef struct ADL_BOOST_NOTFICATION_REASON
{
    int HotkeyChanged; //Set when Hotkey value is changed
    int GlobalEnableChanged; //Set when Global enable value is changed
    int GlobalMinResChanged; //Set when Global min resolution value is changed
}ADL_BOOST_NOTFICATION_REASON;

typedef struct ADL_BOOST_SETTINGS
{
    int Hotkey; // Hotkey value
    int GlobalEnable; //Global enable value
    int GlobalMinRes; //Gloabl Min Resolution value
    int GlobalMinRes_MinLimit; //Gloabl Min Resolution slider min limit value
    int GlobalMinRes_MaxLimit; //Gloabl Min Resolution slider max limit value
    int GlobalMinRes_Step; //Gloabl Min Resolution step  value
}ADL_BOOST_SETTINGS;


typedef struct ADL_PROVSR_NOTFICATION_REASON
{
    int HotkeyChanged; //Set when Hotkey value is changed
    int GlobalEnableChanged; //Set when Global enable value is changed
}ADL_PROVSR_NOTFICATION_REASON;

typedef struct ADL_PROVSR_SETTINGS
{
    int Hotkey; // Hotkey value
    int GlobalEnable; //Global enable value
}ADL_PROVSR_SETTINGS;

typedef struct ADL_IMAGE_BOOST_NOTFICATION_REASON
{
    int HotkeyChanged; //Set when Hotkey value is changed
    int GlobalEnableChanged; //Set when Global enable value is changed
}ADL_IMAGE_BOOST_NOTFICATION_REASON;

typedef struct ADL_IMAGE_BOOST_SETTINGS
{
    int Hotkey; // Hotkey value
    int GlobalEnable; //Global enable value
}ADL_IMAGE_BOOST_SETTINGS;


typedef struct ADL_RIS_NOTFICATION_REASON
{
    unsigned int GlobalEnableChanged; //Set when Global enable value is changed
    unsigned int GlobalSharpeningDegreeChanged; //Set when Global sharpening Degree value is changed
}ADL_RIS_NOTFICATION_REASON;

typedef struct ADL_RIS_SETTINGS
{
    int GlobalEnable; //Global enable value
    int GlobalSharpeningDegree; //Global sharpening value
    int GlobalSharpeningDegree_MinLimit; //Gloabl sharpening slider min limit value
    int GlobalSharpeningDegree_MaxLimit; //Gloabl sharpening slider max limit value
    int GlobalSharpeningDegree_Step; //Gloabl sharpening step  value
}ADL_RIS_SETTINGS;

typedef struct ADL_CHILL_NOTFICATION_REASON
{
    int HotkeyChanged; //Set when Hotkey value is changed
    int GlobalEnableChanged; //Set when Global enable value is changed
    int GlobalMinFPSChanged; //Set when Global min FPS value is changed
    int GlobalMaxFPSChanged; //Set when Global max FPS value is changed
}ADL_CHILL_NOTFICATION_REASON;

typedef struct ADL_CHILL_SETTINGS
{
    int Hotkey; // Hotkey value
    int GlobalEnable; //Global enable value
    int GlobalMinFPS; //Global Min FPS value
    int GlobalMaxFPS; //Global Max FPS value
    int GlobalFPS_MinLimit; //Gloabl FPS slider min limit value
    int GlobalFPS_MaxLimit; //Gloabl FPS slider max limit value
    int GlobalFPS_Step; //Gloabl FPS Slider step  value
}ADL_CHILL_SETTINGS;

typedef struct ADL_DRIVERUPSCALE_NOTFICATION_REASON
{
    int ModeOverrideEnabledChanged;     //Set when Global min resolution value is changed
    int GlobalEnabledChanged;           //Set when Global enable value is changed
}ADL_DRIVERUPSCALE_NOTFICATION_REASON;

typedef struct ADL_DRIVERUPSCALE_SETTINGS
{
    int ModeOverrideEnabled;
    int GlobalEnabled;
}ADL_DRIVERUPSCALE_SETTINGS;

typedef struct ADL_RADEON_LED_COLOR_CONFIG
{
    unsigned short R : 8; // Red Value
    unsigned short G : 8; // Green Value
    unsigned short B : 8; // Blue Value
}ADL_RADEON_LED_COLOR_CONFIG;

typedef struct ADL_RADEON_LED_PATTERN_CONFIG_GENERIC
{
    short brightness : 8; // Brightness of LED
    short speed : 8; // Speed of LED pattern
    bool directionCounterClockWise; //Direction of LED Pattern
    ADL_RADEON_LED_COLOR_CONFIG colorConfig; // RGB value of LED pattern
    char morseCodeText[ADL_RADEON_LED_MAX_MORSE_CODE]; // Morse Code user input for Morse Code LED pattern
    char morseCodeTextOutPut[ADL_RADEON_LED_MAX_MORSE_CODE]; // Driver set output representation of Morse Code
    int  morseCodeTextOutPutLen; // Length of Morse Code output
}ADL_RADEON_LED_PATTERN_CONFIG_GENERIC;

typedef struct ADL_RADEON_LED_CUSTOM_LED_CONFIG
{
    short brightness : 8; // Brightness of LED
    ADL_RADEON_LED_COLOR_CONFIG colorConfig[ADL_RADEON_LED_MAX_LED_ROW_ON_GRID][ADL_RADEON_LED_MAX_LED_COLUMN_ON_GRID]; // Full grid array representation of Radeon LED to be populated by user
}ADL_RADEON_LED_CUSTOM_GRID_LED_CONFIG;

typedef struct ADL_RADEON_LED_PATTERN_CONFIG
{
    ADL_RADEON_USB_LED_BAR_CONTROLS control; //Requested LED pattern

    union
    {
        ADL_RADEON_LED_PATTERN_CONFIG_GENERIC genericPararmeters; //Requested pattern configuration settings
        ADL_RADEON_LED_CUSTOM_GRID_LED_CONFIG customGridConfig; //Requested custom grid configuration settings
    };
}ADL_RADEON_LED_PATTERN_CONFIG;

typedef struct AdapterInfoX2
{

    int iSize;
    int iAdapterIndex;
    char strUDID[ADL_MAX_PATH];
    int iBusNumber;
    int iDeviceNumber;
    int iFunctionNumber;
    int iVendorID;
    char strAdapterName[ADL_MAX_PATH];
    char strDisplayName[ADL_MAX_PATH];
    int iPresent;
    int iExist;
    char strDriverPath[ADL_MAX_PATH];
    char strDriverPathExt[ADL_MAX_PATH];
    char strPNPString[ADL_MAX_PATH];
    int iOSDisplayIndex;
    int iInfoMask;
    int iInfoValue;
} AdapterInfoX2, *LPAdapterInfoX2;


typedef  struct ADLGamutReference
{
    int      iGamutRef;
}ADLGamutReference;


typedef struct ADLGamutInfo
{
    int    SupportedGamutSpace;

    int    SupportedWhitePoint;
} ADLGamutInfo;


typedef struct ADLPoint
{
    int          iX;
    int          iY;
} ADLPoint;

typedef struct ADLGamutCoordinates
{
    ADLPoint      Red;
    ADLPoint      Green;
    ADLPoint      Blue;
} ADLGamutCoordinates;


typedef  struct ADLGamutData
{
    int        iFeature;

    int         iPredefinedGamut;

    int         iPredefinedWhitePoint;

    ADLPoint             CustomWhitePoint;

    ADLGamutCoordinates  CustomGamut;
} ADLGamutData;

typedef struct ADLDetailedTimingX2
{
    int   iSize;
    int sTimingFlags;
    int sHTotal;
    int sHDisplay;
    int sHSyncStart;
    int sHSyncWidth;
    int sVTotal;
    int sVDisplay;
    int sVSyncStart;
    int sVSyncWidth;
    int sPixelClock;
    short sHOverscanRight;
    short sHOverscanLeft;
    short sVOverscanBottom;
    short sVOverscanTop;
    short sOverscan8B;
    short sOverscanGR;
} ADLDetailedTimingX2;

typedef struct ADLDisplayModeInfoX2
{
    int  iTimingStandard;
    int  iPossibleStandard;
    int  iRefreshRate;
    int  iPelsWidth;
    int  iPelsHeight;
    ADLDetailedTimingX2  sDetailedTiming;
} ADLDisplayModeInfoX2;

typedef struct ADLI2CLargePayload
{
    int iSize;
    int iLine;
    int iAddress;
    int iOffset;
    int iAction;
    int iSpeed;
    int iFlags;
    int iDataSize;
    char *pcData;
} ADLI2CLargePayload;

#define ADL_FEATURE_NAME_LENGTH         16

typedef struct ADLFeatureName
{
    char FeatureName[ADL_FEATURE_NAME_LENGTH];
}       ADLFeatureName, *LPADLFeatureName;

typedef struct ADLFeatureCaps
{
    ADLFeatureName      Name;
    //  char strFeatureName[ADL_FEATURE_NAME_LENGTH];

    int  iGroupID;

    int  iVisualID;

    int iPageID;

    int iFeatureMask;

    int  iFeatureProperties;

    int  iControlType;

    int  iControlStyle;

    int  iAdjustmentType;

    int  iAdjustmentStyle;

    int bDefault;

    int iMin;

    int iMax;

    int iStep;

    int iDefault;

    float fMin;

    float fMax;

    float fStep;

    float fDefault;

    int EnumMask;
} ADLFeatureCaps, *LPADLFeatureCaps;

typedef struct ADLFeatureValues
{
    ADLFeatureName      Name;
    //  char strFeatureName[ADL_FEATURE_NAME_LENGTH];

    int bCurrent;

    int iCurrent;

    float fCurrent;

    int EnumStates;
} ADLFeatureValues, *LPADLFeatureValues;


typedef struct ADLHDCPSettings
{
    int iHDCPProtectionVersion; // Version, starting from 1
    int iHDCPCaps; //Caps used to ensure at least one protection scheme is supported, 1 is HDCP1X and 2 is HDCP22
    int iAllowAll; //Allow all is true, disable all is false
    int iHDCPVale;
    int iHDCPMask;
} ADLHDCPSettings;


typedef struct ADLMantleAppInfo
{
    int   apiVersion;
    long   driverVersion;
    long   vendorId;
    long   deviceId;
    int     gpuType;
    char     gpuName[256];
    int     maxMemRefsPerSubmission;
    long long virtualMemPageSize;
    long long maxInlineMemoryUpdateSize;
    long     maxBoundDescriptorSets;
    long     maxThreadGroupSize;
    long  long timestampFrequency;
    long     multiColorTargetClears;
}ADLMantleAppInfo, *LPADLMantleAppInfo;

typedef struct ADLSDIData
{
    int iSDIState;
    int iSizeofSDISegment;
} ADLSDIData, *LPADLSDIData;


typedef struct ADL_FRTCPRO_Settings
{
    int DefaultState;              //The default status for FRTC pro
    int CurrentState;              //The current enable/disable status for FRTC pro
    unsigned int DefaultValue;     //The default FPS value for FRTC pro.
    unsigned int CurrentValue;      //The current FPS value for FRTC pro.
    unsigned int maxSupportedFps;      //The max value for FRTC pro.
    unsigned int minSupportedFps;      //The min value for FRTC pro.
}ADL_FRTCPRO_Settings, *LPADLFRTCProSettings;

typedef struct ADL_FRTCPRO_CHANGED_REASON
{
    int StateChanged;               // FRTCPro state changed
    int ValueChanged;               // FRTCPro value changed
}ADL_FRTCPRO_CHANGED_REASON;

typedef struct ADL_DL_DISPLAY_MODE
{
    int  iPelsHeight;                      // Vertical resolution (in pixels).
    int  iPelsWidth;                       // Horizontal resolution (in pixels).
    int  iBitsPerPel;                      // Color depth.
    int  iDisplayFrequency;                // Refresh rate.
} ADL_DL_DISPLAY_MODE;

typedef union _ADLDCESupport
{
    struct
    {
        unsigned int PrePhasis : 1;
        unsigned int voltageSwing : 1;
        unsigned int reserved : 30;
    }bits;
    unsigned int u32All;
}ADLDCESupport;

typedef struct ADLSmartShiftSettings
{
    int iMinRange;
    int iMaxRange;
    int iDefaultMode; //Refer to CWDDEPM_ODN_CONTROL_TYPE
    int iDefaultValue;
    int iCurrentMode;
    int iCurrentValue;
    int iFlags; //refer to define_smartshift_bits
}ADLSmartShiftSettings, *LPADLSmartShiftSettings;
#endif /* ADL_STRUCTURES_H_ */

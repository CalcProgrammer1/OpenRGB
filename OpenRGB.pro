#-----------------------------------------------------------------------------------------------#
# OpenRGB 0.x QMake Project                                                                     #
#                                                                                               #
#   Adam Honse (CalcProgrammer1)                        5/25/2020                               #
#-----------------------------------------------------------------------------------------------#

#-----------------------------------------------------------------------------------------------#
# Qt Configuration                                                                              #
#-----------------------------------------------------------------------------------------------#
QT +=                                                                                           \
    core                                                                                        \
    gui                                                                                         \

#-----------------------------------------------------------------------------------------------#
# Set compiler to use C++17 to make std::filesystem available                                   #
#-----------------------------------------------------------------------------------------------#
CONFIG += c++17

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 5): DEFINES += _QT6

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
VERSION     = 0.61
TARGET      = OpenRGB
TEMPLATE    = app

#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID   = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse HEAD)
GIT_COMMIT_DATE = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ show -s --format=%ci HEAD)
GIT_BRANCH      = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse --abbrev-ref HEAD)

DEFINES +=                                                                                      \
    VERSION_STRING=\\"\"\"$$VERSION\\"\"\"                                                      \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                                                  \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                                                 \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                                             \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"

#-----------------------------------------------------------------------------------------------#
# OpenRGB Common                                                                                #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    dependencies/ColorWheel                                                                     \
    dependencies/CRCpp/                                                                         \
    dependencies/hueplusplus-1.0.0/include                                                      \
    dependencies/hueplusplus-1.0.0/include/hueplusplus                                          \
    dependencies/json/                                                                          \
    dependencies/libe131/src/                                                                   \
    dependencies/libcmmk/include/                                                               \
    i2c_smbus/                                                                                  \
    i2c_tools/                                                                                  \
    net_port/                                                                                   \
    pci_ids/                                                                                    \
    serial_port/                                                                                \
    super_io/                                                                                   \
    Controllers/AMDWraithPrismController/                                                       \
    Controllers/ASRockPolychromeSMBusController/                                                \
    Controllers/ASRockPolychromeUSBController/                                                  \
    Controllers/AsusAuraCoreController/                                                         \
    Controllers/AsusAuraGPUController/                                                          \
    Controllers/AsusAuraSMBusController/                                                        \
    Controllers/AsusAuraUSBController/                                                          \
    Controllers/CoolerMasterController/                                                         \
    Controllers/CorsairCommanderCoreController/                                                 \
    Controllers/CorsairDominatorPlatinumController/                                             \
    Controllers/CorsairHydroController/                                                         \
    Controllers/CorsairHydroPlatinumController/                                                 \
    Controllers/CorsairPeripheralController/                                                    \
    Controllers/CorsairLightingNodeController/                                                  \
    Controllers/CorsairVengeanceController/                                                     \
    Controllers/CorsairVengeanceProController/                                                  \
    Controllers/CorsairWirelessController/                                                      \
    Controllers/CreativeController/                                                             \
    Controllers/CrucialController/                                                              \
    Controllers/DasKeyboardController/                                                          \
    Controllers/DebugController/                                                                \
    Controllers/DuckyKeyboardController/                                                        \
    Controllers/E131Controller/                                                                 \
    Controllers/EKController/                                                                   \
    Controllers/EspurnaController/                                                              \
    Controllers/EVGAGPUController/                                                              \
    Controllers/EVisionKeyboardController/                                                      \
    Controllers/FanBusController/                                                               \
    Controllers/GainwardGPUController/                                                          \
    Controllers/GalaxGPUController/                                                             \
    Controllers/GigabyteAorusCPUCoolerController/                                               \
    Controllers/GigabyteRGBFusion2DRAMController/                                               \
    Controllers/GigabyteRGBFusion2SMBusController/                                              \
    Controllers/GigabyteRGBFusion2USBController/                                                \
    Controllers/GigabyteRGBFusionController/                                                    \
    Controllers/GigabyteRGBFusionGPUController/                                                 \
    Controllers/HoltekController/                                                               \
    Controllers/HyperXDRAMController/                                                           \
    Controllers/HyperXKeyboardController/                                                       \
    Controllers/HyperXMouseController/                                                          \
    Controllers/HyperXMousematController/                                                       \
    Controllers/LEDStripController/                                                             \
    Controllers/LianLiController/                                                               \
    Controllers/LogitechController/                                                             \
    Controllers/MSI3ZoneController/                                                             \
    Controllers/MSIGPUController/                                                               \
    Controllers/MSIMysticLightController/                                                       \
    Controllers/MSIRGBController/                                                               \
    Controllers/NZXTHue2Controller/                                                             \
    Controllers/NZXTHuePlusController/                                                          \
    Controllers/NZXTKrakenController/                                                           \
    Controllers/OpenRazerController/                                                            \
    Controllers/PatriotViperController/                                                         \
    Controllers/PhilipsHueController/                                                           \
    Controllers/PhilipsWizController/                                                           \
    Controllers/QMKOpenRGBController/                                                           \
    Controllers/RazerController/                                                                \
    Controllers/RedragonController/                                                             \
    Controllers/RoccatController/                                                               \
    Controllers/SapphireGPUController/                                                          \
    Controllers/SinowealthController/                                                           \
    Controllers/SonyDS4Controller/                                                              \
    Controllers/SteelSeriesController/                                                          \
    Controllers/TecknetController/                                                              \
    Controllers/ThermaltakePoseidonZRGBController/                                              \
    Controllers/ThermaltakeRiingController/                                                     \
    Controllers/WootingKeyboardController/                                                      \
    Controllers/YeelightController/                                                             \
    Controllers/ZalmanZSyncController/                                                          \
    RGBController/                                                                              \
    qt/

HEADERS +=                                                                                      \
    dependencies/ColorWheel/ColorWheel.h                                                        \
    dependencies/json/json.hpp                                                                  \
    dependencies/libcmmk/include/libcmmk/libcmmk.h                                              \
    LogManager.h                                                                                \
    NetworkClient.h                                                                             \
    NetworkProtocol.h                                                                           \
    NetworkServer.h                                                                             \
    OpenRGBPluginInterface.h                                                                    \
    PluginManager.h                                                                             \
    ProfileManager.h                                                                            \
    ResourceManager.h                                                                           \
    SettingsManager.h                                                                           \
    Detector.h                                                                                  \
    DeviceDetector.h                                                                            \
    filesystem.h                                                                                \
    qt/DetectorTableModel.h                                                                     \
    qt/OpenRGBClientInfoPage.h                                                                  \
    qt/OpenRGBDeviceInfoPage.h                                                                  \
    qt/OpenRGBDevicePage.h                                                                      \
    qt/OpenRGBDialog.h                                                                          \
    i2c_smbus/i2c_smbus.h                                                                       \
    i2c_tools/i2c_tools.h                                                                       \
    net_port/net_port.h                                                                         \
    pci_ids/pci_ids.h                                                                           \
    qt/DeviceView.h                                                                             \
    qt/OpenRGBDialog2.h                                                                         \
    qt/OpenRGBPluginContainer.h                                                                 \
    qt/OpenRGBProfileSaveDialog.h                                                               \
    qt/OpenRGBServerInfoPage.h                                                                  \
    qt/OpenRGBSettingsPage.h                                                                    \
    qt/OpenRGBSoftwareInfoPage.h                                                                \
    qt/OpenRGBSupportedDevicesPage.h                                                            \
    qt/OpenRGBSystemInfoPage.h                                                                  \
    qt/OpenRGBZoneResizeDialog.h                                                                \
    serial_port/find_usb_serial_port.h                                                          \
    serial_port/serial_port.h                                                                   \
    super_io/super_io.h                                                                         \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.h                             \
    Controllers/AMDWraithPrismController/RGBController_AMDWraithPrism.h                         \
    Controllers/AnnePro2Controller/AnnePro2Controller.h                                         \
    Controllers/AnnePro2Controller/RGBController_AnnePro2.h                                     \
    Controllers/ASRockPolychromeSMBusController/ASRockPolychromeSMBusController.h               \
    Controllers/ASRockPolychromeSMBusController/RGBController_ASRockPolychromeSMBus.h           \
    Controllers/ASRockPolychromeUSBController/ASRockPolychromeUSBController.h                   \
    Controllers/ASRockPolychromeUSBController/RGBController_ASRockPolychromeUSB.h               \
    Controllers/AsusAuraCoreController/AsusAuraCoreController.h                                 \
    Controllers/AsusAuraCoreController/RGBController_AsusAuraCore.h                             \
    Controllers/AsusAuraGPUController/AsusAuraGPUController.h                                   \
    Controllers/AsusAuraGPUController/RGBController_AsusAuraGPU.h                               \
    Controllers/AsusAuraSMBusController/AsusAuraSMBusController.h                               \
    Controllers/AsusAuraSMBusController/RGBController_AsusAuraSMBus.h                           \
    Controllers/AsusAuraUSBController/AsusAuraUSBController.h                                   \
    Controllers/AsusAuraUSBController/AsusAuraAddressableController.h                           \
    Controllers/AsusAuraUSBController/AsusAuraHeadsetStandController.h                          \
    Controllers/AsusAuraUSBController/AsusAuraKeyboardController.h                              \
    Controllers/AsusAuraUSBController/AsusAuraMainboardController.h                             \
    Controllers/AsusAuraUSBController/AsusAuraMouseController.h                                 \
    Controllers/AsusAuraUSBController/AsusAuraTUFKeyboardController.h                           \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraHeadsetStand.h                      \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraKeyboard.h                          \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMouse.h                             \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraTUFKeyboard.h                       \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraUSB.h                               \
    Controllers/CoolerMasterController/CMARGBcontroller.h                                       \
    Controllers/CoolerMasterController/CMMP750Controller.h                                      \
    Controllers/CoolerMasterController/CMSmallARGBController.h                                  \
    Controllers/CoolerMasterController/CMR6000Controller.h                                      \
    Controllers/CoolerMasterController/CMMKController.h                                         \
    Controllers/CoolerMasterController/RGBController_CMARGBController.h                         \
    Controllers/CoolerMasterController/RGBController_CMMP750Controller.h                        \
    Controllers/CoolerMasterController/RGBController_CMSmallARGBController.h                    \
    Controllers/CoolerMasterController/RGBController_CMR6000Controller.h                        \
    Controllers/CoolerMasterController/RGBController_CMMKController.h                           \
    Controllers/CorsairCommanderCoreController/CorsairCommanderCoreController.h                 \
    Controllers/CorsairCommanderCoreController/RGBController_CorsairCommanderCore.h             \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumController.h         \
    Controllers/CorsairDominatorPlatinumController/RGBController_CorsairDominatorPlatinum.h     \
    Controllers/CorsairHydroController/CorsairHydroController.h                                 \
    Controllers/CorsairHydroController/RGBController_CorsairHydro.h                             \
    Controllers/CorsairHydroPlatinumController/CorsairHydroPlatinumController.h                 \
    Controllers/CorsairHydroPlatinumController/RGBController_CorsairHydroPlatinum.h             \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.h                   \
    Controllers/CorsairLightingNodeController/RGBController_CorsairLightingNode.h               \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.h                       \
    Controllers/CorsairPeripheralController/RGBController_CorsairPeripheral.h                   \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.h                         \
    Controllers/CorsairVengeanceController/RGBController_CorsairVengeance.h                     \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.h                   \
    Controllers/CorsairVengeanceProController/RGBController_CorsairVengeancePro.h               \
    Controllers/CorsairWirelessController/CorsairWirelessController.h                           \
    Controllers/CorsairWirelessController/RGBController_CorsairWireless.h                       \
    Controllers/CreativeController/CreativeSoundBlasterXG6Controller.h                          \
    Controllers/CreativeController/RGBController_CreativeSoundBlasterXG6.h                      \
    Controllers/CrucialController/CrucialController.h                                           \
    Controllers/CrucialController/RGBController_Crucial.h                                       \
    Controllers/DasKeyboardController/DasKeyboardController.h                                   \
    Controllers/DasKeyboardController/RGBController_DasKeyboard.h                               \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.h                               \
    Controllers/DuckyKeyboardController/RGBController_DuckyKeyboard.h                           \
    Controllers/DebugController/RGBController_Debug.h                                           \
    Controllers/E131Controller/RGBController_E131.h                                             \
    Controllers/EKController/EKController.h                                                     \
    Controllers/EKController/RGBController_EKController.h                                       \
    Controllers/EspurnaController/EspurnaController.h                                           \
    Controllers/EspurnaController/RGBController_Espurna.h                                       \
    Controllers/EVGAGPUController/EVGAGPUv1Controller.h                                         \
    Controllers/EVGAGPUController/EVGAGPUv2Controller.h                                         \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv1.h                                     \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv2.h                                     \
    Controllers/EVisionKeyboardController/EVisionKeyboardController.h                           \
    Controllers/EVisionKeyboardController/RGBController_EVisionKeyboard.h                       \
    Controllers/FanBusController/FanBusController.h                                             \
    Controllers/FanBusController/FanBusInterface.h                                              \
    Controllers/FanBusController/RGBController_FanBus.h                                         \
    Controllers/GainwardGPUController/GainwardGPUv1Controller.h                                 \
    Controllers/GainwardGPUController/GainwardGPUv2Controller.h                                 \
    Controllers/GainwardGPUController/RGBController_GainwardGPUv1.h                             \
    Controllers/GainwardGPUController/RGBController_GainwardGPUv2.h                             \
    Controllers/GalaxGPUController/GalaxGPUController.h                                         \
    Controllers/GalaxGPUController/RGBController_GalaxGPU.h                                     \
    Controllers/GigabyteAorusCPUCoolerController/ATC800Controller.h                             \
    Controllers/GigabyteAorusCPUCoolerController/RGBController_AorusATC800.h                    \
    Controllers/GigabyteRGBFusion2DRAMController/GigabyteRGBFusion2DRAMController.h             \
    Controllers/GigabyteRGBFusion2DRAMController/RGBController_GigabyteRGBFusion2DRAM.h         \
    Controllers/GigabyteRGBFusion2SMBusController/GigabyteRGBFusion2SMBusController.h           \
    Controllers/GigabyteRGBFusion2SMBusController/RGBController_GigabyteRGBFusion2SMBus.h       \
    Controllers/GigabyteRGBFusion2USBController/GigabyteRGBFusion2USBController.h               \
    Controllers/GigabyteRGBFusion2USBController/RGBController_GigabyteRGBFusion2USB.h           \
    Controllers/GigabyteRGBFusionController/GigabyteRGBFusionController.h                       \
    Controllers/GigabyteRGBFusionController/RGBController_GigabyteRGBFusion.h                   \
    Controllers/GigabyteRGBFusionGPUController/GigabyteRGBFusionGPUController.h                 \
    Controllers/GigabyteRGBFusionGPUController/RGBController_GigabyteRGBFusionGPU.h             \
    Controllers/HoltekController/HoltekA070Controller.h                                         \
    Controllers/HoltekController/HoltekA1FAController.h                                         \
    Controllers/HoltekController/RGBController_HoltekA070.h                                     \
    Controllers/HoltekController/RGBController_HoltekA1FA.h                                     \
    Controllers/HyperXDRAMController/HyperXDRAMController.h                                     \
    Controllers/HyperXDRAMController/RGBController_HyperXDRAM.h                                 \
    Controllers/HyperXKeyboardController/HyperXAlloyElite2Controller.h                          \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.h                         \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsCoreController.h                     \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.h                             \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyElite2.h                      \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins.h                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOriginsCore.h                 \
    Controllers/HyperXKeyboardController/RGBController_HyperXKeyboard.h                         \
    Controllers/HyperXMouseController/HyperXPulsefireFPSProController.h                         \
    Controllers/HyperXMouseController/HyperXPulsefireSurgeController.h                          \
    Controllers/HyperXMouseController/HyperXPulsefireDartController.h                           \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireFPSPro.h                     \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireSurge.h                      \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireDart.h                       \
    Controllers/HyperXMousematController/HyperXMousematController.h                             \
    Controllers/HyperXMousematController/RGBController_HyperXMousemat.h                         \
    Controllers/LEDStripController/LEDStripController.h                                         \
    Controllers/LEDStripController/RGBController_LEDStrip.h                                     \
    Controllers/LianLiController/LianLiUniHubController.h                                       \
    Controllers/LianLiController/RGBController_LianLiUniHub.h                                   \
    Controllers/LogitechController/LogitechProtocolCommon.h                                     \
    Controllers/LogitechController/LogitechG203LController.h                                    \
    Controllers/LogitechController/LogitechG213Controller.h                                     \
    Controllers/LogitechController/LogitechG560Controller.h                                     \
    Controllers/LogitechController/LogitechG810Controller.h                                     \
    Controllers/LogitechController/LogitechG910Controller.h                                     \
    Controllers/LogitechController/LogitechG815Controller.h                                     \
    Controllers/LogitechController/LogitechGLightsyncController.h                               \
    Controllers/LogitechController/LogitechLightspeedController.h                               \
    Controllers/LogitechController/LogitechX56Controller.h                                      \
    Controllers/LogitechController/RGBController_LogitechG203L.h                                \
    Controllers/LogitechController/RGBController_LogitechG213.h                                 \
    Controllers/LogitechController/RGBController_LogitechG560.h                                 \
    Controllers/LogitechController/RGBController_LogitechG810.h                                 \
    Controllers/LogitechController/RGBController_LogitechG910.h                                 \
    Controllers/LogitechController/RGBController_LogitechG815.h                                 \
    Controllers/LogitechController/RGBController_LogitechGLightsync.h                           \
    Controllers/LogitechController/RGBController_LogitechGLightsync1zone.h                      \
    Controllers/LogitechController/RGBController_LogitechGPowerPlay.h                           \
    Controllers/LogitechController/RGBController_LogitechLightspeed.h                           \
    Controllers/LogitechController/RGBController_LogitechX56.h                                  \
    Controllers/MSI3ZoneController/MSI3ZoneController.h                                         \
    Controllers/MSI3ZoneController/RGBController_MSI3Zone.h                                     \
    Controllers/MSIGPUController/MSIGPUController.h                                             \
    Controllers/MSIGPUController/RGBController_MSIGPU.h                                         \
    Controllers/MSIMysticLightController/MSIMysticLightCommon.h                                 \
    Controllers/MSIMysticLightController/MSIMysticLight162Controller.h                          \
    Controllers/MSIMysticLightController/MSIMysticLight185Controller.h                          \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight162.h                      \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight185.h                      \
    Controllers/MSIRGBController/MSIRGBController.h                                             \
    Controllers/MSIRGBController/RGBController_MSIRGB.h                                         \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.h                                         \
    Controllers/NZXTHue2Controller/RGBController_NZXTHue2.h                                     \
    Controllers/NZXTHuePlusController/NZXTHuePlusController.h                                   \
    Controllers/NZXTHuePlusController/RGBController_NZXTHuePlus.h                               \
    Controllers/NZXTKrakenController/NZXTKrakenController.h                                     \
    Controllers/NZXTKrakenController/RGBController_NZXTKraken.h                                 \
    Controllers/OpenRazerController/OpenRazerDevices.h                                          \
    Controllers/PatriotViperController/PatriotViperController.h                                 \
    Controllers/PatriotViperController/RGBController_PatriotViper.h                             \
    Controllers/PhilipsHueController/PhilipsHueController.h                                     \
    Controllers/PhilipsHueController/RGBController_PhilipsHue.h                                 \
    Controllers/PhilipsWizController/PhilipsWizController.h                                     \
    Controllers/PhilipsWizController/RGBController_PhilipsWiz.h                                 \
    Controllers/QMKOpenRGBController/QMKOpenRGBController.h                                     \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGB.h                                 \
    Controllers/RazerController/RazerController.h                                               \
    Controllers/RazerController/RazerKrakenController.h                                         \
    Controllers/RazerController/RazerDevices.h                                                  \
    Controllers/RazerController/RGBController_Razer.h                                           \
    Controllers/RazerController/RGBController_RazerAddressable.h                                \
    Controllers/RazerController/RGBController_RazerKraken.h                                     \
    Controllers/RedragonController/RedragonM711Controller.h                                     \
    Controllers/RedragonController/RGBController_RedragonM711.h                                 \
    Controllers/RoccatController/RGBController_RoccatKoneAimo.h                                 \
    Controllers/RoccatController/RoccatKoneAimoController.h                                     \
    Controllers/SapphireGPUController/SapphireNitroGlowV1Controller.h                           \
    Controllers/SapphireGPUController/SapphireNitroGlowV3Controller.h                           \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV1.h                       \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV3.h                       \
    Controllers/SinowealthController/SinowealthController.h                                     \
    Controllers/SinowealthController/RGBController_Sinowealth.h                                 \
    Controllers/SonyDS4Controller/SonyDS4Controller.h                                           \
    Controllers/SonyDS4Controller/RGBController_SonyDS4.h                                       \
    Controllers/SteelSeriesController/color32.h                                                 \
    Controllers/SteelSeriesController/SteelSeriesApexBaseController.h                           \
    Controllers/SteelSeriesController/SteelSeriesApexController.h                               \
    Controllers/SteelSeriesController/SteelSeriesApexMController.h                              \
    Controllers/SteelSeriesController/SteelSeriesGeneric.h                                      \
    Controllers/SteelSeriesController/SteelSeriesOldApexController.h                            \
    Controllers/SteelSeriesController/SteelSeriesRivalController.h                              \
    Controllers/SteelSeriesController/SteelSeriesSenseiController.h                             \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.h                            \
    Controllers/SteelSeriesController/SteelSeriesQCKMatController.h                             \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex.h                           \
    Controllers/SteelSeriesController/RGBController_SteelSeriesOldApex.h                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival.h                          \
    Controllers/SteelSeriesController/RGBController_SteelSeriesSiberia.h                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesQCKMat.h                         \
    Controllers/SteelSeriesController/RGBController_SteelSeriesSensei.h                         \
    Controllers/TecknetController/TecknetController.h                                           \
    Controllers/TecknetController/RGBController_Tecknet.h                                       \
    Controllers/ThermaltakePoseidonZRGBController/ThermaltakePoseidonZRGBController.h           \
    Controllers/ThermaltakePoseidonZRGBController/RGBController_ThermaltakePoseidonZRGB.h       \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.h                         \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingQuadController.h                     \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiing.h                     \
    Controllers/WootingKeyboardController/WootingKeyboardController.h                           \
    Controllers/WootingKeyboardController/RGBController_WootingKeyboard.h                       \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiingQuad.h                 \
    Controllers/YeelightController/YeelightController.h                                         \
    Controllers/YeelightController/RGBController_Yeelight.h                                     \
    Controllers/ZalmanZSyncController/ZalmanZSyncController.h                                   \
    Controllers/ZalmanZSyncController/RGBController_ZalmanZSync.h                               \
    RGBController/RGBController.h                                                               \
    RGBController/RGBController_Dummy.h                                                         \
    RGBController/RGBController_Network.h                                                       \

SOURCES +=                                                                                      \
    dependencies/dmiinfo.cpp                                                                    \
    dependencies/ColorWheel/ColorWheel.cpp                                                      \
    dependencies/hueplusplus-1.0.0/src/Action.cpp                                               \
    dependencies/hueplusplus-1.0.0/src/APICache.cpp                                             \
    dependencies/hueplusplus-1.0.0/src/BaseDevice.cpp                                           \
    dependencies/hueplusplus-1.0.0/src/BaseHttpHandler.cpp                                      \
    dependencies/hueplusplus-1.0.0/src/Bridge.cpp                                               \
    dependencies/hueplusplus-1.0.0/src/BridgeConfig.cpp                                         \
    dependencies/hueplusplus-1.0.0/src/CLIPSensors.cpp                                          \
    dependencies/hueplusplus-1.0.0/src/ColorUnits.cpp                                           \
    dependencies/hueplusplus-1.0.0/src/EntertainmentMode.cpp                                    \
    dependencies/hueplusplus-1.0.0/src/ExtendedColorHueStrategy.cpp                             \
    dependencies/hueplusplus-1.0.0/src/ExtendedColorTemperatureStrategy.cpp                     \
    dependencies/hueplusplus-1.0.0/src/Group.cpp                                                \
    dependencies/hueplusplus-1.0.0/src/HueCommandAPI.cpp                                        \
    dependencies/hueplusplus-1.0.0/src/HueDeviceTypes.cpp                                       \
    dependencies/hueplusplus-1.0.0/src/HueException.cpp                                         \
    dependencies/hueplusplus-1.0.0/src/Light.cpp                                                \
    dependencies/hueplusplus-1.0.0/src/ModelPictures.cpp                                        \
    dependencies/hueplusplus-1.0.0/src/NewDeviceList.cpp                                        \
    dependencies/hueplusplus-1.0.0/src/Scene.cpp                                                \
    dependencies/hueplusplus-1.0.0/src/Schedule.cpp                                             \
    dependencies/hueplusplus-1.0.0/src/Sensor.cpp                                               \
    dependencies/hueplusplus-1.0.0/src/SimpleBrightnessStrategy.cpp                             \
    dependencies/hueplusplus-1.0.0/src/SimpleColorHueStrategy.cpp                               \
    dependencies/hueplusplus-1.0.0/src/SimpleColorTemperatureStrategy.cpp                       \
    dependencies/hueplusplus-1.0.0/src/StateTransaction.cpp                                     \
    dependencies/hueplusplus-1.0.0/src/TimePattern.cpp                                          \
    dependencies/hueplusplus-1.0.0/src/UPnP.cpp                                                 \
    dependencies/hueplusplus-1.0.0/src/Utils.cpp                                                \
    dependencies/hueplusplus-1.0.0/src/ZLLSensors.cpp                                           \
    dependencies/libe131/src/e131.c                                                             \
    dependencies/libcmmk/src/libcmmk.c                                                          \
    main.cpp                                                                                    \
    cli.cpp                                                                                     \
    LogManager.cpp                                                                              \
    NetworkClient.cpp                                                                           \
    NetworkServer.cpp                                                                           \
    PluginManager.cpp                                                                           \
    ProfileManager.cpp                                                                          \
    ResourceManager.cpp                                                                         \
    SettingsManager.cpp                                                                         \
    qt/DetectorTableModel.cpp                                                                   \
    qt/OpenRGBClientInfoPage.cpp                                                                \
    qt/OpenRGBDeviceInfoPage.cpp                                                                \
    qt/OpenRGBDevicePage.cpp                                                                    \
    qt/OpenRGBDialog.cpp                                                                        \
    i2c_smbus/i2c_smbus.cpp                                                                     \
    i2c_tools/i2c_tools.cpp                                                                     \
    net_port/net_port.cpp                                                                       \
    qt/DeviceView.cpp                                                                           \
    qt/OpenRGBDialog2.cpp                                                                       \
    qt/OpenRGBPluginContainer.cpp                                                               \
    qt/OpenRGBProfileSaveDialog.cpp                                                             \
    qt/OpenRGBServerInfoPage.cpp                                                                \
    qt/OpenRGBSettingsPage.cpp                                                                  \
    qt/OpenRGBSoftwareInfoPage.cpp                                                              \
    qt/OpenRGBSupportedDevicesPage.cpp                                                          \
    qt/OpenRGBSystemInfoPage.cpp                                                                \
    qt/OpenRGBZoneResizeDialog.cpp                                                              \
    qt/hsv.cpp                                                                                  \
    serial_port/serial_port.cpp                                                                 \
    super_io/super_io.cpp                                                                       \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.cpp                           \
    Controllers/AMDWraithPrismController/AMDWraithPrismControllerDetect.cpp                     \
    Controllers/AMDWraithPrismController/RGBController_AMDWraithPrism.cpp                       \
    Controllers/AnnePro2Controller/AnnePro2Controller.cpp                                       \
    Controllers/AnnePro2Controller/AnnePro2ControllerDetect.cpp                                 \
    Controllers/AnnePro2Controller/RGBController_AnnePro2.cpp                                   \
    Controllers/ASRockPolychromeSMBusController/ASRockPolychromeSMBusController.cpp             \
    Controllers/ASRockPolychromeSMBusController/ASRockPolychromeSMBusControllerDetect.cpp       \
    Controllers/ASRockPolychromeSMBusController/RGBController_ASRockPolychromeSMBus.cpp         \
    Controllers/ASRockPolychromeUSBController/ASRockPolychromeUSBController.cpp                 \
    Controllers/ASRockPolychromeUSBController/ASRockPolychromeUSBControllerDetect.cpp           \
    Controllers/ASRockPolychromeUSBController/RGBController_ASRockPolychromeUSB.cpp             \
    Controllers/AsusAuraCoreController/AsusAuraCoreController.cpp                               \
    Controllers/AsusAuraCoreController/AsusAuraCoreControllerDetect.cpp                         \
    Controllers/AsusAuraCoreController/RGBController_AsusAuraCore.cpp                           \
    Controllers/AsusAuraGPUController/AsusAuraGPUController.cpp                                 \
    Controllers/AsusAuraGPUController/AsusAuraGPUControllerDetect.cpp                           \
    Controllers/AsusAuraGPUController/RGBController_AsusAuraGPU.cpp                             \
    Controllers/AsusAuraSMBusController/AsusAuraSMBusController.cpp                             \
    Controllers/AsusAuraSMBusController/AsusAuraSMBusControllerDetect.cpp                       \
    Controllers/AsusAuraSMBusController/RGBController_AsusAuraSMBus.cpp                         \
    Controllers/AsusAuraUSBController/AsusAuraUSBController.cpp                                 \
    Controllers/AsusAuraUSBController/AsusAuraAddressableController.cpp                         \
    Controllers/AsusAuraUSBController/AsusAuraHeadsetStandController.cpp                        \
    Controllers/AsusAuraUSBController/AsusAuraKeyboardController.cpp                            \
    Controllers/AsusAuraUSBController/AsusAuraMainboardController.cpp                           \
    Controllers/AsusAuraUSBController/AsusAuraMouseController.cpp                               \
    Controllers/AsusAuraUSBController/AsusAuraTUFKeyboardController.cpp                         \
    Controllers/AsusAuraUSBController/AsusAuraUSBControllerDetect.cpp                           \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraHeadsetStand.cpp                    \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraKeyboard.cpp                        \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMouse.cpp                           \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraTUFKeyboard.cpp                     \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraUSB.cpp                             \
    Controllers/CoolerMasterController/CMARGBcontroller.cpp                                     \
    Controllers/CoolerMasterController/CMMP750Controller.cpp                                    \
    Controllers/CoolerMasterController/CMSmallARGBController.cpp                                \
    Controllers/CoolerMasterController/CMR6000Controller.cpp                                    \
    Controllers/CoolerMasterController/CMMKController.cpp                                       \
    Controllers/CoolerMasterController/CoolerMasterControllerDetect.cpp                         \
    Controllers/CoolerMasterController/RGBController_CMARGBController.cpp                       \
    Controllers/CoolerMasterController/RGBController_CMMP750Controller.cpp                      \
    Controllers/CoolerMasterController/RGBController_CMSmallARGBController.cpp                  \
    Controllers/CoolerMasterController/RGBController_CMR6000Controller.cpp                      \
    Controllers/CoolerMasterController/RGBController_CMMKController.cpp                         \
    Controllers/CorsairCommanderCoreController/CorsairCommanderCoreController.cpp               \
    Controllers/CorsairCommanderCoreController/CorsairCommanderCoreControllerDetect.cpp         \
    Controllers/CorsairCommanderCoreController/RGBController_CorsairCommanderCore.cpp           \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumController.cpp       \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumControllerDetect.cpp \
    Controllers/CorsairDominatorPlatinumController/RGBController_CorsairDominatorPlatinum.cpp   \
    Controllers/CorsairHydroController/CorsairHydroController.cpp                               \
    Controllers/CorsairHydroController/CorsairHydroControllerDetect.cpp                         \
    Controllers/CorsairHydroController/RGBController_CorsairHydro.cpp                           \
    Controllers/CorsairHydroPlatinumController/CorsairHydroPlatinumController.cpp               \
    Controllers/CorsairHydroPlatinumController/CorsairHydroPlatinumControllerDetect.cpp         \
    Controllers/CorsairHydroPlatinumController/RGBController_CorsairHydroPlatinum.cpp           \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.cpp                 \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeControllerDetect.cpp           \
    Controllers/CorsairLightingNodeController/RGBController_CorsairLightingNode.cpp             \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.cpp                     \
    Controllers/CorsairPeripheralController/CorsairPeripheralControllerDetect.cpp               \
    Controllers/CorsairPeripheralController/RGBController_CorsairPeripheral.cpp                 \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.cpp                       \
    Controllers/CorsairVengeanceController/CorsairVengeanceControllerDetect.cpp                 \
    Controllers/CorsairVengeanceController/RGBController_CorsairVengeance.cpp                   \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.cpp                 \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProControllerDetect.cpp           \
    Controllers/CorsairVengeanceProController/RGBController_CorsairVengeancePro.cpp             \
    Controllers/CorsairWirelessController/CorsairWirelessController.cpp                         \
    Controllers/CorsairWirelessController/CorsairWirelessControllerDetect.cpp                   \
    Controllers/CorsairWirelessController/RGBController_CorsairWireless.cpp                     \
    Controllers/CreativeController/CreativeSoundBlasterXG6Controller.cpp                        \
    Controllers/CreativeController/CreativeControllerDetect.cpp                                 \
    Controllers/CreativeController/RGBController_CreativeSoundBlasterXG6.cpp                    \
    Controllers/CrucialController/CrucialController.cpp                                         \
    Controllers/CrucialController/CrucialControllerDetect.cpp                                   \
    Controllers/CrucialController/RGBController_Crucial.cpp                                     \
    Controllers/DebugController/DebugControllerDetect.cpp                                       \
    Controllers/DasKeyboardController/DasKeyboardController.cpp                                 \
    Controllers/DasKeyboardController/DasKeyboardControllerDetect.cpp                           \
    Controllers/DasKeyboardController/RGBController_DasKeyboard.cpp                             \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.cpp                             \
    Controllers/DuckyKeyboardController/DuckyKeyboardControllerDetect.cpp                       \
    Controllers/DuckyKeyboardController/RGBController_DuckyKeyboard.cpp                         \
    Controllers/DebugController/RGBController_Debug.cpp                                         \
    Controllers/E131Controller/E131ControllerDetect.cpp                                         \
    Controllers/E131Controller/RGBController_E131.cpp                                           \
    Controllers/EKController/EKControllerDetect.cpp                                             \
    Controllers/EKController/EKController.cpp                                                   \
    Controllers/EKController/RGBController_EKController.cpp                                     \
    Controllers/EspurnaController/EspurnaController.cpp                                         \
    Controllers/EspurnaController/EspurnaControllerDetect.cpp                                   \
    Controllers/EspurnaController/RGBController_Espurna.cpp                                     \
    Controllers/EVGAGPUController/EVGAGPUv1Controller.cpp                                       \
    Controllers/EVGAGPUController/EVGAGPUv2Controller.cpp                                       \
    Controllers/EVGAGPUController/EVGAGPUControllerDetect.cpp                                   \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv1.cpp                                   \
    Controllers/EVGAGPUController/RGBController_EVGAGPUv2.cpp                                   \
    Controllers/EVisionKeyboardController/EVisionKeyboardController.cpp                         \
    Controllers/EVisionKeyboardController/EVisionKeyboardControllerDetect.cpp                   \
    Controllers/EVisionKeyboardController/RGBController_EVisionKeyboard.cpp                     \
    Controllers/FanBusController/FanBusController.cpp                                           \
    Controllers/FanBusController/FanBusControllerDetect.cpp                                     \
    Controllers/FanBusController/FanBusInterface.cpp                                            \
    Controllers/FanBusController/RGBController_FanBus.cpp                                       \
    Controllers/GainwardGPUController/GainwardGPUControllerDetect.cpp                           \
    Controllers/GainwardGPUController/GainwardGPUv1Controller.cpp                               \
    Controllers/GainwardGPUController/GainwardGPUv2Controller.cpp                               \
    Controllers/GainwardGPUController/RGBController_GainwardGPUv1.cpp                           \
    Controllers/GainwardGPUController/RGBController_GainwardGPUv2.cpp                           \
    Controllers/GalaxGPUController/GalaxGPUController.cpp                                       \
    Controllers/GalaxGPUController/GalaxGPUControllerDetect.cpp                                 \
    Controllers/GalaxGPUController/RGBController_GalaxGPU.cpp                                   \
    Controllers/GigabyteAorusCPUCoolerController/ATC800Controller.cpp                           \
    Controllers/GigabyteAorusCPUCoolerController/GigabyteAorusCPUCoolerControllerDetect.cpp     \
    Controllers/GigabyteAorusCPUCoolerController/RGBController_AorusATC800.cpp                  \
    Controllers/GigabyteRGBFusion2DRAMController/GigabyteRGBFusion2DRAMController.cpp           \
    Controllers/GigabyteRGBFusion2DRAMController/GigabyteRGBFusion2DRAMControllerDetect.cpp     \
    Controllers/GigabyteRGBFusion2DRAMController/RGBController_GigabyteRGBFusion2DRAM.cpp       \
    Controllers/GigabyteRGBFusion2SMBusController/GigabyteRGBFusion2SMBusController.cpp         \
    Controllers/GigabyteRGBFusion2SMBusController/GigabyteRGBFusion2SMBusControllerDetect.cpp   \
    Controllers/GigabyteRGBFusion2SMBusController/RGBController_GigabyteRGBFusion2SMBus.cpp     \
    Controllers/GigabyteRGBFusion2USBController/GigabyteRGBFusion2USBController.cpp             \
    Controllers/GigabyteRGBFusion2USBController/GigabyteRGBFusion2USBControllerDetect.cpp       \
    Controllers/GigabyteRGBFusion2USBController/RGBController_GigabyteRGBFusion2USB.cpp         \
    Controllers/GigabyteRGBFusionController/GigabyteRGBFusionController.cpp                     \
    Controllers/GigabyteRGBFusionController/GigabyteRGBFusionControllerDetect.cpp               \
    Controllers/GigabyteRGBFusionController/RGBController_GigabyteRGBFusion.cpp                 \
    Controllers/GigabyteRGBFusionGPUController/GigabyteRGBFusionGPUController.cpp               \
    Controllers/GigabyteRGBFusionGPUController/GigabyteRGBFusionGPUControllerDetect.cpp         \
    Controllers/GigabyteRGBFusionGPUController/RGBController_GigabyteRGBFusionGPU.cpp           \
    Controllers/HoltekController/HoltekA070Controller.cpp                                       \
    Controllers/HoltekController/HoltekA1FAController.cpp                                       \
    Controllers/HoltekController/HoltekControllerDetect.cpp                                     \
    Controllers/HoltekController/RGBController_HoltekA070.cpp                                   \
    Controllers/HoltekController/RGBController_HoltekA1FA.cpp                                   \
    Controllers/HyperXDRAMController/HyperXDRAMController.cpp                                   \
    Controllers/HyperXDRAMController/HyperXDRAMControllerDetect.cpp                             \
    Controllers/HyperXDRAMController/RGBController_HyperXDRAM.cpp                               \
    Controllers/HyperXKeyboardController/HyperXAlloyElite2Controller.cpp                        \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.cpp                       \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsCoreController.cpp                   \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.cpp                           \
    Controllers/HyperXKeyboardController/HyperXKeyboardControllerDetect.cpp                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyElite2.cpp                    \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins.cpp                   \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOriginsCore.cpp               \
    Controllers/HyperXKeyboardController/RGBController_HyperXKeyboard.cpp                       \
    Controllers/HyperXMouseController/HyperXMouseControllerDetect.cpp                           \
    Controllers/HyperXMouseController/HyperXPulsefireFPSProController.cpp                       \
    Controllers/HyperXMouseController/HyperXPulsefireSurgeController.cpp                        \
    Controllers/HyperXMouseController/HyperXPulsefireDartController.cpp                         \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireFPSPro.cpp                   \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireSurge.cpp                    \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireDart.cpp                     \
    Controllers/HyperXMousematController/HyperXMousematController.cpp                           \
    Controllers/HyperXMousematController/HyperXMousematControllerDetect.cpp                     \
    Controllers/HyperXMousematController/RGBController_HyperXMousemat.cpp                       \
    Controllers/LEDStripController/LEDStripController.cpp                                       \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp                                 \
    Controllers/LEDStripController/RGBController_LEDStrip.cpp                                   \
    Controllers/LianLiController/LianLiControllerDetect.cpp                                     \
    Controllers/LianLiController/LianLiUniHubController.cpp                                     \
    Controllers/LianLiController/RGBController_LianLiUniHub.cpp                                 \
    Controllers/LogitechController/LogitechControllerDetect.cpp                                 \
    Controllers/LogitechController/LogitechProtocolCommon.cpp                                   \
    Controllers/LogitechController/LogitechG203LController.cpp                                  \
    Controllers/LogitechController/LogitechG213Controller.cpp                                   \
    Controllers/LogitechController/LogitechG560Controller.cpp                                   \
    Controllers/LogitechController/LogitechG810Controller.cpp                                   \
    Controllers/LogitechController/LogitechG910Controller.cpp                                   \
    Controllers/LogitechController/LogitechG815Controller.cpp                                   \
    Controllers/LogitechController/LogitechGLightsyncController.cpp                             \
    Controllers/LogitechController/LogitechLightspeedController.cpp                             \
    Controllers/LogitechController/LogitechX56Controller.cpp                                    \
    Controllers/LogitechController/RGBController_LogitechG203L.cpp                              \
    Controllers/LogitechController/RGBController_LogitechG213.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG560.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG810.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG910.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG815.cpp                               \
    Controllers/LogitechController/RGBController_LogitechGLightsync.cpp                         \
    Controllers/LogitechController/RGBController_LogitechGLightsync1zone.cpp                    \
    Controllers/LogitechController/RGBController_LogitechGPowerPlay.cpp                         \
    Controllers/LogitechController/RGBController_LogitechLightspeed.cpp                         \
    Controllers/LogitechController/RGBController_LogitechX56.cpp                                \
    Controllers/MSI3ZoneController/MSI3ZoneController.cpp                                       \
    Controllers/MSI3ZoneController/MSI3ZoneControllerDetect.cpp                                 \
    Controllers/MSI3ZoneController/RGBController_MSI3Zone.cpp                                   \
    Controllers/MSIGPUController/MSIGPUController.cpp                                           \
    Controllers/MSIGPUController/MSIGPUControllerDetect.cpp                                     \
    Controllers/MSIGPUController/RGBController_MSIGPU.cpp                                       \
    Controllers/MSIMysticLightController/MSIMysticLight162Controller.cpp                        \
    Controllers/MSIMysticLightController/MSIMysticLight185Controller.cpp                        \
    Controllers/MSIMysticLightController/MSIMysticLightControllerDetect.cpp                     \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight162.cpp                    \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight185.cpp                    \
    Controllers/MSIRGBController/MSIRGBController.cpp                                           \
    Controllers/MSIRGBController/MSIRGBControllerDetect.cpp                                     \
    Controllers/MSIRGBController/RGBController_MSIRGB.cpp                                       \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.cpp                                       \
    Controllers/NZXTHue2Controller/NZXTHue2ControllerDetect.cpp                                 \
    Controllers/NZXTHue2Controller/RGBController_NZXTHue2.cpp                                   \
    Controllers/NZXTHuePlusController/NZXTHuePlusController.cpp                                 \
    Controllers/NZXTHuePlusController/NZXTHuePlusControllerDetect.cpp                           \
    Controllers/NZXTHuePlusController/RGBController_NZXTHuePlus.cpp                             \
    Controllers/NZXTKrakenController/NZXTKrakenController.cpp                                   \
    Controllers/NZXTKrakenController/NZXTKrakenControllerDetect.cpp                             \
    Controllers/NZXTKrakenController/RGBController_NZXTKraken.cpp                               \
    Controllers/PatriotViperController/PatriotViperController.cpp                               \
    Controllers/PatriotViperController/PatriotViperControllerDetect.cpp                         \
    Controllers/PatriotViperController/RGBController_PatriotViper.cpp                           \
    Controllers/PhilipsHueController/PhilipsHueController.cpp                                   \
    Controllers/PhilipsHueController/PhilipsHueControllerDetect.cpp                             \
    Controllers/PhilipsHueController/PhilipsHueEntertainmentController.cpp                      \
    Controllers/PhilipsHueController/RGBController_PhilipsHue.cpp                               \
    Controllers/PhilipsHueController/RGBController_PhilipsHueEntertainment.cpp                  \
    Controllers/PhilipsWizController/PhilipsWizController.cpp                                   \
    Controllers/PhilipsWizController/PhilipsWizControllerDetect.cpp                             \
    Controllers/PhilipsWizController/RGBController_PhilipsWiz.cpp                               \
    Controllers/QMKOpenRGBController/QMKOpenRGBControllerDetect.cpp                             \
    Controllers/QMKOpenRGBController/QMKOpenRGBController.cpp                                   \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGB.cpp                               \
    Controllers/RazerController/RazerController.cpp                                             \
    Controllers/RazerController/RazerKrakenController.cpp                                       \
    Controllers/RazerController/RazerControllerDetect.cpp                                       \
    Controllers/RazerController/RGBController_Razer.cpp                                         \
    Controllers/RazerController/RGBController_RazerAddressable.cpp                              \
    Controllers/RazerController/RGBController_RazerKraken.cpp                                   \
    Controllers/RedragonController/RedragonM711Controller.cpp                                   \
    Controllers/RedragonController/RedragonControllerDetect.cpp                                 \
    Controllers/RedragonController/RGBController_RedragonM711.cpp                               \
    Controllers/RoccatController/RGBController_RoccatKoneAimo.cpp                               \
    Controllers/RoccatController/RoccatKoneAimoController.cpp                                   \
    Controllers/RoccatController/RoccatControllerDetect.cpp                                     \
    Controllers/SapphireGPUController/SapphireNitroGlowV1Controller.cpp                         \
    Controllers/SapphireGPUController/SapphireNitroGlowV3Controller.cpp                         \
    Controllers/SapphireGPUController/SapphireGPUControllerDetect.cpp                           \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV1.cpp                     \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV3.cpp                     \
    Controllers/SinowealthController/SinowealthController.cpp                                   \
    Controllers/SinowealthController/SinowealthControllerDetect.cpp                             \
    Controllers/SinowealthController/RGBController_Sinowealth.cpp                               \
    Controllers/SonyDS4Controller/SonyDS4Controller.cpp                                         \
    Controllers/SonyDS4Controller/SonyDS4ControllerDetect.cpp                                   \
    Controllers/SonyDS4Controller/RGBController_SonyDS4.cpp                                     \
    Controllers/SteelSeriesController/SteelSeriesApexController.cpp                             \
    Controllers/SteelSeriesController/SteelSeriesApexMController.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesOldApexController.cpp                          \
    Controllers/SteelSeriesController/SteelSeriesRivalController.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesSenseiController.cpp                           \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.cpp                          \
    Controllers/SteelSeriesController/SteelSeriesQCKMatController.cpp                           \
    Controllers/SteelSeriesController/SteelSeriesControllerDetect.cpp                           \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex.cpp                         \
    Controllers/SteelSeriesController/RGBController_SteelSeriesOldApex.cpp                      \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival.cpp                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesSensei.cpp                       \
    Controllers/SteelSeriesController/RGBController_SteelSeriesSiberia.cpp                      \
    Controllers/SteelSeriesController/RGBController_SteelSeriesQCKMat.cpp                       \
    Controllers/TecknetController/TecknetController.cpp                                         \
    Controllers/TecknetController/TecknetControllerDetect.cpp                                   \
    Controllers/TecknetController/RGBController_Tecknet.cpp                                     \
    Controllers/ThermaltakePoseidonZRGBController/ThermaltakePoseidonZRGBController.cpp         \
    Controllers/ThermaltakePoseidonZRGBController/ThermaltakePoseidonZRGBControllerDetect.cpp   \
    Controllers/ThermaltakePoseidonZRGBController/RGBController_ThermaltakePoseidonZRGB.cpp     \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.cpp                       \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingQuadController.cpp                   \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingControllerDetect.cpp                 \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiing.cpp                   \
    Controllers/WootingKeyboardController/WootingKeyboardController.cpp                         \
    Controllers/WootingKeyboardController/WootingKeyboardControllerDetect.cpp                   \
    Controllers/WootingKeyboardController/RGBController_WootingKeyboard.cpp                     \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiingQuad.cpp               \
    Controllers/YeelightController/YeelightController.cpp                                       \
    Controllers/YeelightController/YeelightControllerDetect.cpp                                 \
    Controllers/YeelightController/RGBController_Yeelight.cpp                                   \
    Controllers/ZalmanZSyncController/ZalmanZSyncController.cpp                                 \
    Controllers/ZalmanZSyncController/ZalmanZSyncControllerDetect.cpp                           \
    Controllers/ZalmanZSyncController/RGBController_ZalmanZSync.cpp                             \
    RGBController/RGBController.cpp                                                             \
    RGBController/RGBController_Dummy.cpp                                                       \
    RGBController/RGBController_Network.cpp                                                     \

RESOURCES +=                                                                                    \
    qt/resources.qrc

FORMS +=                                                                                        \
    qt/OpenRGBClientInfoPage.ui                                                                 \
    qt/OpenRGBDeviceInfoPage.ui                                                                 \
    qt/OpenRGBDevicePage.ui                                                                     \
    qt/OpenRGBDialog.ui                                                                         \
    qt/OpenRGBDialog2.ui                                                                        \
    qt/OpenRGBPluginContainer.ui                                                                \
    qt/OpenRGBProfileSaveDialog.ui                                                              \
    qt/OpenRGBServerInfoPage.ui                                                                 \
    qt/OpenRGBSettingsPage.ui                                                                   \
    qt/OpenRGBSoftwareInfoPage.ui                                                               \
    qt/OpenRGBSupportedDevicesPage.ui                                                           \
    qt/OpenRGBSystemInfoPage.ui                                                                 \
    qt/OpenRGBZoneResizeDialog.ui                                                               \

#-----------------------------------------------------------------------------------------------#
# Windows-specific Configuration                                                                #
#-----------------------------------------------------------------------------------------------#
win32:INCLUDEPATH +=                                                                            \
    dependencies/display-library/include                                                        \
    dependencies/hidapi                                                                         \
    dependencies/inpout32_1501/Win32/                                                           \
    dependencies/libusb-1.0.22/include                                                          \
    dependencies/mbedtls-2.24.0/include                                                         \
    dependencies/NVFC                                                                           \
    dependencies/openrazer-win32                                                                \
    wmi/                                                                                        \
    Controllers/AsusTUFLaptopController                                                         \

win32:SOURCES +=                                                                                \
#   dependencies/hidapi/hidapi.c                                                                \
    dependencies/hueplusplus-1.0.0/src/WinHttpHandler.cpp                                       \
    dependencies/mbedtls-2.24.0/library/aes.c                                                   \
    dependencies/mbedtls-2.24.0/library/aesni.c                                                 \
    dependencies/mbedtls-2.24.0/library/arc4.c                                                  \
    dependencies/mbedtls-2.24.0/library/aria.c                                                  \
    dependencies/mbedtls-2.24.0/library/asn1parse.c                                             \
    dependencies/mbedtls-2.24.0/library/asn1write.c                                             \
    dependencies/mbedtls-2.24.0/library/base64.c                                                \
    dependencies/mbedtls-2.24.0/library/bignum.c                                                \
    dependencies/mbedtls-2.24.0/library/blowfish.c                                              \
    dependencies/mbedtls-2.24.0/library/camellia.c                                              \
    dependencies/mbedtls-2.24.0/library/ccm.c                                                   \
    dependencies/mbedtls-2.24.0/library/certs.c                                                 \
    dependencies/mbedtls-2.24.0/library/chacha20.c                                              \
    dependencies/mbedtls-2.24.0/library/chachapoly.c                                            \
    dependencies/mbedtls-2.24.0/library/cipher.c                                                \
    dependencies/mbedtls-2.24.0/library/cipher_wrap.c                                           \
    dependencies/mbedtls-2.24.0/library/cmac.c                                                  \
    dependencies/mbedtls-2.24.0/library/ctr_drbg.c                                              \
    dependencies/mbedtls-2.24.0/library/debug.c                                                 \
    dependencies/mbedtls-2.24.0/library/des.c                                                   \
    dependencies/mbedtls-2.24.0/library/dhm.c                                                   \
    dependencies/mbedtls-2.24.0/library/ecdh.c                                                  \
    dependencies/mbedtls-2.24.0/library/ecdsa.c                                                 \
    dependencies/mbedtls-2.24.0/library/ecjpake.c                                               \
    dependencies/mbedtls-2.24.0/library/ecp.c                                                   \
    dependencies/mbedtls-2.24.0/library/ecp_curves.c                                            \
    dependencies/mbedtls-2.24.0/library/entropy.c                                               \
    dependencies/mbedtls-2.24.0/library/entropy_poll.c                                          \
    dependencies/mbedtls-2.24.0/library/error.c                                                 \
    dependencies/mbedtls-2.24.0/library/gcm.c                                                   \
    dependencies/mbedtls-2.24.0/library/havege.c                                                \
    dependencies/mbedtls-2.24.0/library/hkdf.c                                                  \
    dependencies/mbedtls-2.24.0/library/hmac_drbg.c                                             \
    dependencies/mbedtls-2.24.0/library/md.c                                                    \
    dependencies/mbedtls-2.24.0/library/md2.c                                                   \
    dependencies/mbedtls-2.24.0/library/md4.c                                                   \
    dependencies/mbedtls-2.24.0/library/md5.c                                                   \
    dependencies/mbedtls-2.24.0/library/memory_buffer_alloc.c                                   \
    dependencies/mbedtls-2.24.0/library/net_sockets.c                                           \
    dependencies/mbedtls-2.24.0/library/nist_kw.c                                               \
    dependencies/mbedtls-2.24.0/library/oid.c                                                   \
    dependencies/mbedtls-2.24.0/library/padlock.c                                               \
    dependencies/mbedtls-2.24.0/library/pem.c                                                   \
    dependencies/mbedtls-2.24.0/library/pk.c                                                    \
    dependencies/mbedtls-2.24.0/library/pk_wrap.c                                               \
    dependencies/mbedtls-2.24.0/library/pkcs5.c                                                 \
    dependencies/mbedtls-2.24.0/library/pkcs11.c                                                \
    dependencies/mbedtls-2.24.0/library/pkcs12.c                                                \
    dependencies/mbedtls-2.24.0/library/pkparse.c                                               \
    dependencies/mbedtls-2.24.0/library/pkwrite.c                                               \
    dependencies/mbedtls-2.24.0/library/platform.c                                              \
    dependencies/mbedtls-2.24.0/library/platform_util.c                                         \
    dependencies/mbedtls-2.24.0/library/poly1305.c                                              \
    dependencies/mbedtls-2.24.0/library/psa_crypto.c                                            \
    dependencies/mbedtls-2.24.0/library/psa_crypto_se.c                                         \
    dependencies/mbedtls-2.24.0/library/psa_crypto_slot_management.c                            \
    dependencies/mbedtls-2.24.0/library/psa_crypto_storage.c                                    \
    dependencies/mbedtls-2.24.0/library/psa_its_file.c                                          \
    dependencies/mbedtls-2.24.0/library/ripemd160.c                                             \
    dependencies/mbedtls-2.24.0/library/rsa.c                                                   \
    dependencies/mbedtls-2.24.0/library/rsa_internal.c                                          \
    dependencies/mbedtls-2.24.0/library/sha1.c                                                  \
    dependencies/mbedtls-2.24.0/library/sha256.c                                                \
    dependencies/mbedtls-2.24.0/library/sha512.c                                                \
    dependencies/mbedtls-2.24.0/library/ssl_cache.c                                             \
    dependencies/mbedtls-2.24.0/library/ssl_ciphersuites.c                                      \
    dependencies/mbedtls-2.24.0/library/ssl_cli.c                                               \
    dependencies/mbedtls-2.24.0/library/ssl_cookie.c                                            \
    dependencies/mbedtls-2.24.0/library/ssl_msg.c                                               \
    dependencies/mbedtls-2.24.0/library/ssl_srv.c                                               \
    dependencies/mbedtls-2.24.0/library/ssl_ticket.c                                            \
    dependencies/mbedtls-2.24.0/library/ssl_tls.c                                               \
    dependencies/mbedtls-2.24.0/library/threading.c                                             \
    dependencies/mbedtls-2.24.0/library/timing.c                                                \
    dependencies/mbedtls-2.24.0/library/version.c                                               \
    dependencies/mbedtls-2.24.0/library/version_features.c                                      \
    dependencies/mbedtls-2.24.0/library/x509.c                                                  \
    dependencies/mbedtls-2.24.0/library/x509_create.c                                           \
    dependencies/mbedtls-2.24.0/library/x509_crl.c                                              \
    dependencies/mbedtls-2.24.0/library/x509_crt.c                                              \
    dependencies/mbedtls-2.24.0/library/x509_csr.c                                              \
    dependencies/mbedtls-2.24.0/library/x509write_crt.c                                         \
    dependencies/mbedtls-2.24.0/library/x509write_csr.c                                         \
    dependencies/mbedtls-2.24.0/library/xtea.c                                                  \
    dependencies/NVFC/nvapi.cpp                                                                 \
    i2c_smbus/i2c_smbus_amdadl.cpp                                                              \
    i2c_smbus/i2c_smbus_i801.cpp                                                                \
    i2c_smbus/i2c_smbus_nct6775.cpp                                                             \
    i2c_smbus/i2c_smbus_nvapi.cpp                                                               \
    i2c_smbus/i2c_smbus_piix4.cpp                                                               \
    serial_port/find_usb_serial_port_win.cpp                                                    \
    wmi/wmi.cpp                                                                                 \
    wmi/acpiwmi.cpp                                                                             \
    Controllers/AsusTUFLaptopController/AsusTUFLaptopWMIDetect.cpp                              \
    Controllers/AsusTUFLaptopController/RGBController_AsusTUFLaptopWMI.cpp                      \
    Controllers/OpenRazerController/OpenRazerWindowsDetect.cpp                                  \
    Controllers/OpenRazerController/RGBController_OpenRazerWindows.cpp                          \

win32:HEADERS +=                                                                                \
    dependencies/display-library/include/adl_defines.h                                          \
    dependencies/display-library/include/adl_sdk.h                                              \
    dependencies/display-library/include/adl_structures.h                                       \
    dependencies/inpout32_1501/Win32/inpout32.h                                                 \
    dependencies/NVFC/nvapi.h                                                                   \
    i2c_smbus/i2c_smbus_i801.h                                                                  \
    i2c_smbus/i2c_smbus_nct6775.h                                                               \
    i2c_smbus/i2c_smbus_nvapi.h                                                                 \
    i2c_smbus/i2c_smbus_piix4.h                                                                 \
    wmi/wmi.h                                                                                   \
    wmi/acpiwmi.h                                                                               \
    Controllers/AsusTUFLaptopController/RGBController_AsusTUFLaptopWMI.h                        \
    Controllers/OpenRazerController/RGBController_OpenRazerWindows.h                            \

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -L"$$PWD/dependencies/inpout32_1501/x64/" -linpoutx64                                   \
        -L"$$PWD/dependencies/libusb-1.0.22/MS64/dll" -llibusb-1.0                              \
        -L"$$PWD/dependencies/hidapi-win/x64/" -lhidapi                                         \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -L"$$PWD/dependencies/inpout32_1501/Win32/" -linpout32                                  \
        -L"$$PWD/dependencies/libusb-1.0.22/MS32/dll" -llibusb-1.0                              \
        -L"$$PWD/dependencies/hidapi-win/x86/" -lhidapi                                         \
}

win32:DEFINES -=                                                                                \
    UNICODE

win32:DEFINES +=                                                                                \
    USE_HID_USAGE                                                                               \
    _MBCS                                                                                       \
    WIN32                                                                                       \
    _CRT_SECURE_NO_WARNINGS                                                                     \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                                             \
    WIN32_LEAN_AND_MEAN                                                                         \

win32:RC_ICONS +=                                                                               \
    qt/OpenRGB.ico

#-----------------------------------------------------------------------------------------------#
# Windows GitLab CI Configuration                                                               #
#-----------------------------------------------------------------------------------------------#
win32:CONFIG(debug, debug|release) {
    win32:DESTDIR = debug
}

win32:CONFIG(release, debug|release) {
    win32:DESTDIR = release
}

win32:OBJECTS_DIR = _intermediate_$$DESTDIR/.obj
win32:MOC_DIR     = _intermediate_$$DESTDIR/.moc
win32:RCC_DIR     = _intermediate_$$DESTDIR/.qrc
win32:UI_DIR      = _intermediate_$$DESTDIR/.ui

#-----------------------------------------------------------------------------------------------#
# Copy dependencies to output directory                                                         #
#-----------------------------------------------------------------------------------------------#

win32:contains(QMAKE_TARGET.arch, x86_64) {
    copydata.commands  = $(COPY_FILE) \"$$shell_path($$PWD/dependencies/openrazer-win32/OpenRazer64.dll      )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/inpout32_1501/x64/inpoutx64.dll      )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/libusb-1.0.22/MS64/dll/libusb-1.0.dll)\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/hidapi-win/x64/hidapi.dll            )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

win32:contains(QMAKE_TARGET.arch, x86) {
    copydata.commands  = $(COPY_FILE) \"$$shell_path($$PWD/dependencies/openrazer-win32/OpenRazer.dll        )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/inpout32_1501/Win32/inpout32.dll     )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/libusb-1.0.22/MS32/dll/libusb-1.0.dll)\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/hidapi-win/x86/hidapi.dll            )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)

    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

#-----------------------------------------------------------------------------------------------#
# Linux-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
unix:!macx {
    TARGET = $$lower($$TARGET)

    INCLUDEPATH +=                                                                              \
    Controllers/FaustusController                                                               \
    Controllers/LinuxLEDController                                                              \

    HEADERS +=                                                                                  \
    i2c_smbus/i2c_smbus_linux.h                                                                 \
    Controllers/FaustusController/RGBController_Faustus.h                                       \
    Controllers/LinuxLEDController/LinuxLEDController.h                                         \
    Controllers/LinuxLEDController/RGBController_LinuxLED.h                                     \
    Controllers/OpenRazerController/RGBController_OpenRazer.h                                   \

    LIBS +=                                                                                     \
    -lusb-1.0                                                                                   \
    -lstdc++fs                                                                                  \
    -lmbedx509                                                                                  \
    -lmbedcrypto                                                                                \
    -lmbedtls                                                                                   \

    #-------------------------------------------------------------------------------------------#
    # Determine which hidapi to use based on availability                                       #
    #   Prefer hidraw backend, then libusb                                                      #
    #-------------------------------------------------------------------------------------------#
    packagesExist(hidapi-hidraw) {
        LIBS += -lhidapi-hidraw

        #---------------------------------------------------------------------------------------#
        # hidapi-hidraw >= 0.10.1 supports USAGE/USAGE_PAGE                                     #
        # Define USE_HID_USAGE if hidapi-hidraw supports it                                     #
        #---------------------------------------------------------------------------------------#
        HIDAPI_HIDRAW_VERSION = $$system(pkgconf --modversion hidapi-hidraw)
        if(versionAtLeast(HIDAPI_HIDRAW_VERSION, "0.10.1")) {
            DEFINES += USE_HID_USAGE
        }
    } else {
        packagesExist(hidapi-libusb) {
            LIBS += -lhidapi-libusb
        } else {
            LIBS += -lhidapi
        }
    }

    SOURCES +=                                                                                  \
    dependencies/hueplusplus-1.0.0/src/LinHttpHandler.cpp                                       \
    i2c_smbus/i2c_smbus_linux.cpp                                                               \
    serial_port/find_usb_serial_port_linux.cpp                                                  \
    Controllers/FaustusController/RGBController_Faustus.cpp                                     \
    Controllers/LinuxLEDController/LinuxLEDController.cpp                                       \
    Controllers/LinuxLEDController/LinuxLEDControllerDetect.cpp                                 \
    Controllers/LinuxLEDController/RGBController_LinuxLED.cpp                                   \
    Controllers/OpenRazerController/OpenRazerDetect.cpp                                         \
    Controllers/OpenRazerController/RGBController_OpenRazer.cpp                                 \

    #-------------------------------------------------------------------------------------------#
    # Set up install paths                                                                      #
    # These install paths are used for AppImage and .deb packaging                              #
    #-------------------------------------------------------------------------------------------#
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    target.path=$$PREFIX/bin/
    desktop.path=$$PREFIX/share/applications/
    desktop.files+=qt/OpenRGB.desktop
    pixmap.path=$$PREFIX/share/pixmaps/
    pixmap.files+=qt/OpenRGB.png
    rules.path=/lib/udev/rules.d/
    rules.files+=60-openrgb.rules
    INSTALLS += target desktop pixmap rules
}

unix:!macx:CONFIG(asan) {
    message("ASan Mode")
    QMAKE_CFLAGS=-fsanitize=address
    QMAKE_CXXFLAGS=-fsanitize=address
    QMAKE_LFLAGS=-fsanitize=address
}

#-----------------------------------------------------------------------------------------------#
# MacOS-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

macx:ICON = qt/OpenRGB.icns

#-------------------------------------------------------------------------------------------#
# Common MacOS definitions                                                                  #
#-------------------------------------------------------------------------------------------#
macx {
    DEFINES +=                                                                                  \
    USE_HID_USAGE                                                                               \

    SOURCES +=                                                                                  \
    dependencies/hueplusplus-1.0.0/src/LinHttpHandler.cpp                                       \
    serial_port/find_usb_serial_port_linux.cpp                                                  \

    LIBS +=                                                                                     \
    -lusb-1.0                                                                                   \
    -lhidapi                                                                                    \
    -lmbedx509                                                                                  \
    -lmbedcrypto                                                                                \
    -lmbedtls                                                                                   \
}

#-------------------------------------------------------------------------------------------#
# Apple Silicon (arm64) Homebrew installs at /opt/homebrew                                  #
#-------------------------------------------------------------------------------------------#
macx:contains(QMAKE_HOST.arch, arm64) {
    INCLUDEPATH +=                                                                              \
    /opt/homebrew/include                                                                       \

    LIBS +=                                                                                     \
    -L/opt/homebrew/lib                                                                         \
}

#-------------------------------------------------------------------------------------------#
# Intel (x86_64) Homebrew installs at /usr/local/lib                                        #
#-------------------------------------------------------------------------------------------#
macx:contains(QMAKE_HOST.arch, x86_64) {
    INCLUDEPATH +=                                                                              \
    /usr/local/include                                                                          \
    /usr/local/homebrew/include                                                                 \

    LIBS +=                                                                                     \
    -L/usr/local/lib                                                                            \
    -L/usr/local/homebrew/lib                                                                   \
}

DISTFILES += \
    debian/openrgb-udev.postinst \
    debian/openrgb.postinst

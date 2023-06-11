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
CONFIG +=   c++17                                                                               \
            lrelease                                                                            \
            embed_translations                                                                  \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

greaterThan(QT_MAJOR_VERSION, 5): DEFINES += _QT6

#-----------------------------------------------------------------------------------------------#
# pkg-config Configuration                                                                      #
#-----------------------------------------------------------------------------------------------#
greaterThan(QT_MAJOR_VERSION, 4) {
    PKG_CONFIG = $$pkgConfigExecutable()
} else {
    PKG_CONFIG = "pkgconf"
}

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
MAJOR       = 0
MINOR       = 8
REVISION    = 1
VERSION     = $$MAJOR"."$$MINOR$$REVISION
TARGET      = OpenRGB
TEMPLATE    = app

#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID   = $$system(git log -n 1 --pretty=format:"%H")
GIT_COMMIT_DATE = $$system(git log -n 1 --pretty=format:"%ci")
GIT_BRANCH      = $$system(git branch --show-current)

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
    dependencies/Swatches/                                                                      \
    dependencies/CRCpp/                                                                         \
    dependencies/hueplusplus-1.0.0/include                                                      \
    dependencies/hueplusplus-1.0.0/include/hueplusplus                                          \
    dependencies/httplib                                                                        \
    dependencies/json/                                                                          \
    dependencies/libe131/src/                                                                   \
    dependencies/libcmmk/include/                                                               \
    dependencies/mdns                                                                           \
    hidapi_wrapper/                                                                             \
    i2c_smbus/                                                                                  \
    i2c_tools/                                                                                  \
    net_port/                                                                                   \
    pci_ids/                                                                                    \
    serial_port/                                                                                \
    super_io/                                                                                   \
    AutoStart/                                                                                  \
    Controllers/A4TechController/                                                               \
    Controllers/AlienwareController/                                                            \
    Controllers/AlienwareKeyboardController/                                                    \
    Controllers/AMDWraithPrismController/                                                       \
    Controllers/AOCKeyboardController/                                                          \
    Controllers/AOCMouseController/                                                             \
    Controllers/AOCMousematController/                                                          \
    Controllers/ASRockPolychromeUSBController/                                                  \
    Controllers/ASRockSMBusController/                                                          \
    Controllers/AsusAuraCoreController/                                                         \
    Controllers/AsusAuraGPUController/                                                          \
    Controllers/AsusAuraUSBController/                                                          \
    Controllers/CherryKeyboardController/                                                       \
    Controllers/CoolerMasterController/                                                         \
    Controllers/CorsairCommanderCoreController/                                                 \
    Controllers/CorsairDominatorPlatinumController/                                             \
    Controllers/CorsairHydroController/                                                         \
    Controllers/CorsairHydro2Controller/                                                        \
    Controllers/CorsairHydroPlatinumController/                                                 \
    Controllers/CorsairPeripheralController/                                                    \
    Controllers/CorsairPeripheralV2Controller/                                                  \
    Controllers/CorsairLightingNodeController/                                                  \
    Controllers/CorsairVengeanceController/                                                     \
    Controllers/CorsairVengeanceProController/                                                  \
    Controllers/CorsairWirelessController/                                                      \
    Controllers/CreativeController/                                                             \
    Controllers/CrucialController/                                                              \
    Controllers/CryorigH7QuadLumiController/                                                    \
    Controllers/DasKeyboardController/                                                          \
    Controllers/DebugController/                                                                \
    Controllers/DMXController/                                                                  \
    Controllers/DuckyKeyboardController/                                                        \
    Controllers/DygmaRaiseController/                                                           \
    Controllers/E131Controller/                                                                 \
    Controllers/EKController/                                                                   \
    Controllers/ENESMBusController/                                                             \
    Controllers/ENESMBusController/ENESMBusInterface                                            \
    Controllers/EpomakerController/                                                             \
    Controllers/EspurnaController/                                                              \
    Controllers/EVGAGP102GPUController/                                                         \
    Controllers/EVGAPascalGPUController/                                                        \
    Controllers/EVGATuringGPUController/                                                        \
    Controllers/EVGAAmpereGPUController/                                                        \
    Controllers/EVisionKeyboardController/                                                      \
    Controllers/FanBusController/                                                               \
    Controllers/GainwardGPUController/                                                          \
    Controllers/GaiZongGaiKeyboardController/                                                   \
    Controllers/GalaxGPUController/                                                             \
    Controllers/GigabyteAorusCPUCoolerController/                                               \
    Controllers/GigabyteAorusMouseController/                                                   \
    Controllers/GigabyteRGBFusion2DRAMController/                                               \
    Controllers/GigabyteRGBFusion2SMBusController/                                              \
    Controllers/GigabyteRGBFusion2USBController/                                                \
    Controllers/GigabyteRGBFusionController/                                                    \
    Controllers/GigabyteRGBFusionGPUController/                                                 \
    Controllers/GigabyteRGBFusion2GPUController/                                                \
    Controllers/GigabyteSuperIORGBController/                                                   \
    Controllers/HoltekController/                                                               \
    Controllers/HPOmen30LController/                                                            \
    Controllers/HyperXDRAMController/                                                           \
    Controllers/HyperXKeyboardController/                                                       \
    Controllers/HyperXMicrophoneController/                                                     \
    Controllers/HyperXMouseController/                                                          \
    Controllers/HyperXMousematController/                                                       \
    Controllers/IntelArcA770LEController/                                                       \
    Controllers/IonicoController/                                                               \
    Controllers/LEDStripController/                                                             \
    Controllers/LegoDimensionsToypadBaseController/                                             \
    Controllers/LenovoControllers/                                                              \
    Controllers/LenovoMotherboardController/                                                    \
    Controllers/LianLiController/                                                               \
    Controllers/LIFXController/                                                                 \
    Controllers/LogitechController/                                                             \
    Controllers/MSI3ZoneController/                                                             \
    Controllers/MSIGPUController/                                                               \
    Controllers/MSIMysticLightController/                                                       \
    Controllers/MSIOptixController/                                                             \
    Controllers/MSIRGBController/                                                               \
    Controllers/MSIVigorController/                                                             \
    Controllers/NanoleafController/                                                             \
    Controllers/NZXTHue1Controller/                                                             \
    Controllers/NZXTHue2Controller/                                                             \
    Controllers/NZXTHuePlusController/                                                          \
    Controllers/NZXTKrakenController/                                                           \
    Controllers/OpenRazerController/                                                            \
    Controllers/PatriotViperController/                                                         \
    Controllers/PatriotViperSteelController/                                                    \
    Controllers/PhilipsHueController/                                                           \
    Controllers/PhilipsWizController/                                                           \
    Controllers/PNYGPUController/                                                               \
    Controllers/QMKOpenRGBController/                                                           \
    Controllers/RazerController/                                                                \
    Controllers/RedragonController/                                                             \
    Controllers/RedSquareKeyroxController/                                                      \
    Controllers/RoccatController/                                                               \
    Controllers/SapphireGPUController/                                                          \
    Controllers/SinowealthController/                                                           \
    Controllers/SonyGamepadController/                                                          \
    Controllers/SRGBmodsController/                                                             \
    Controllers/SteelSeriesController/                                                          \
    Controllers/TecknetController/                                                              \
    Controllers/ThermaltakePoseidonZRGBController/                                              \
    Controllers/ThermaltakeRiingController/                                                     \
    Controllers/ViewSonicController/                                                            \
    Controllers/WootingKeyboardController/                                                      \
    Controllers/WushiController/                                                                \
    Controllers/YeelightController/                                                             \
    Controllers/ZalmanZSyncController/                                                          \
    Controllers/ZotacTuringGPUController/                                                       \
    KeyboardLayoutManager/                                                                      \
    RGBController/                                                                              \
    qt/

contains(QMAKE_PLATFORM, freebsd) {
    INCLUDEPATH -=                                                                              \
        Controllers/GigabyteRGBFusion2GPUController/
}

HEADERS +=                                                                                      \
    Colors.h                                                                                    \
    dependencies/ColorWheel/ColorWheel.h                                                        \
    dependencies/Swatches/swatches.h                                                            \
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
    qt/OpenRGBConsolePage.h                                                                     \
    qt/OpenRGBDeviceInfoPage.h                                                                  \
    qt/OpenRGBDevicePage.h                                                                      \
    qt/OpenRGBDialog.h                                                                          \
    hidapi_wrapper/hidapi_wrapper.h                                                             \
    i2c_smbus/i2c_smbus.h                                                                       \
    i2c_tools/i2c_tools.h                                                                       \
    net_port/net_port.h                                                                         \
    pci_ids/pci_ids.h                                                                           \
    qt/DeviceView.h                                                                             \
    qt/OpenRGBDialog2.h                                                                         \
    qt/OpenRGBElgatoKeyLightSettingsPage/OpenRGBElgatoKeyLightSettingsEntry.h                   \
    qt/OpenRGBElgatoKeyLightSettingsPage/OpenRGBElgatoKeyLightSettingsPage.h                    \
    qt/OpenRGBHardwareIDsDialog.h                                                               \
    qt/OpenRGBKasaSmartSettingsPage/OpenRGBKasaSmartSettingsEntry.h                             \
    qt/OpenRGBKasaSmartSettingsPage/OpenRGBKasaSmartSettingsPage.h                              \
    qt/OpenRGBPluginContainer.h                                                                 \
    qt/OpenRGBPluginsPage/OpenRGBPluginsEntry.h                                                 \
    qt/OpenRGBPluginsPage/OpenRGBPluginsList.h                                                  \
    qt/OpenRGBPluginsPage/OpenRGBPluginsPage.h                                                  \
    qt/OpenRGBProfileSaveDialog.h                                                               \
    qt/OpenRGBServerInfoPage.h                                                                  \
    qt/OpenRGBSettingsPage.h                                                                    \
    qt/OpenRGBSoftwareInfoPage.h                                                                \
    qt/OpenRGBSupportedDevicesPage.h                                                            \
    qt/OpenRGBSystemInfoPage.h                                                                  \
    qt/OpenRGBThemeManager.h                                                                    \
    qt/OpenRGBZoneResizeDialog.h                                                                \
    qt/OpenRGBDMXSettingsPage/OpenRGBDMXSettingsEntry.h                                         \
    qt/OpenRGBDMXSettingsPage/OpenRGBDMXSettingsPage.h                                          \
    qt/OpenRGBE131SettingsPage/OpenRGBE131SettingsEntry.h                                       \
    qt/OpenRGBE131SettingsPage/OpenRGBE131SettingsPage.h                                        \
    qt/OpenRGBLIFXSettingsPage/OpenRGBLIFXSettingsEntry.h                                       \
    qt/OpenRGBLIFXSettingsPage/OpenRGBLIFXSettingsPage.h                                        \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafSettingsEntry.h                               \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafSettingsPage.h                                \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafScanningThread.h                              \
    qt/OpenRGBPhilipsHueSettingsPage/OpenRGBPhilipsHueSettingsEntry.h                           \
    qt/OpenRGBPhilipsHueSettingsPage/OpenRGBPhilipsHueSettingsPage.h                            \
    qt/OpenRGBPhilipsWizSettingsPage/OpenRGBPhilipsWizSettingsEntry.h                           \
    qt/OpenRGBPhilipsWizSettingsPage/OpenRGBPhilipsWizSettingsPage.h                            \
    qt/OpenRGBQMKORGBSettingsPage/OpenRGBQMKORGBSettingsEntry.h                                 \
    qt/OpenRGBQMKORGBSettingsPage/OpenRGBQMKORGBSettingsPage.h                                  \
    qt/OpenRGBSerialSettingsPage/OpenRGBSerialSettingsEntry.h                                   \
    qt/OpenRGBSerialSettingsPage/OpenRGBSerialSettingsPage.h                                    \
    qt/OpenRGBYeelightSettingsPage/OpenRGBYeelightSettingsEntry.h                               \
    qt/OpenRGBYeelightSettingsPage/OpenRGBYeelightSettingsPage.h                                \
    qt/OpenRGBZonesBulkResizer.h                                                                \
    qt/QTooltipedSlider.h                                                                       \
    qt/TabLabel.h                                                                               \
    serial_port/find_usb_serial_port.h                                                          \
    serial_port/serial_port.h                                                                   \
    StringUtils.h                                                                               \
    super_io/super_io.h                                                                         \
    AutoStart/AutoStart.h                                                                       \
    Controllers/A4TechController/BloodyMouseController.h                                        \
    Controllers/A4TechController/RGBController_BloodyMouse.h                                    \
    Controllers/AlienwareController/AlienwareController.h                                       \
    Controllers/AlienwareController/RGBController_Alienware.h                                   \
    Controllers/AlienwareKeyboardController/AlienwareAW510KController.h                         \
    Controllers/AlienwareKeyboardController/RGBController_AlienwareAW510K.h                     \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.h                             \
    Controllers/AMDWraithPrismController/RGBController_AMDWraithPrism.h                         \
    Controllers/AnnePro2Controller/AnnePro2Controller.h                                         \
    Controllers/AnnePro2Controller/RGBController_AnnePro2.h                                     \
    Controllers/AOCKeyboardController/AOCKeyboardController.h                                   \
    Controllers/AOCKeyboardController/RGBController_AOCKeyboard.h                               \
    Controllers/AOCMouseController/AOCMouseController.h                                         \
    Controllers/AOCMouseController/RGBController_AOCMouse.h                                     \
    Controllers/AOCMousematController/AOCMousematController.h                                   \
    Controllers/AOCMousematController/RGBController_AOCMousemat.h                               \
    Controllers/ASRockPolychromeUSBController/ASRockPolychromeUSBController.h                   \
    Controllers/ASRockPolychromeUSBController/RGBController_ASRockPolychromeUSB.h               \
    Controllers/ASRockSMBusController/ASRockASRRGBSMBusController.h                             \
    Controllers/ASRockSMBusController/RGBController_ASRockASRRGBSMBus.h                         \
    Controllers/ASRockSMBusController/ASRockPolychromeV1SMBusController.h                       \
    Controllers/ASRockSMBusController/RGBController_ASRockPolychromeV1SMBus.h                   \
    Controllers/ASRockSMBusController/ASRockPolychromeV2SMBusController.h                       \
    Controllers/ASRockSMBusController/RGBController_ASRockPolychromeV2SMBus.h                   \
    Controllers/AsusAuraCoreController/AsusAuraCoreController.h                                 \
    Controllers/AsusAuraCoreController/AsusAuraCoreLaptopController.h                           \
    Controllers/AsusAuraCoreController/RGBController_AsusAuraCore.h                             \
    Controllers/AsusAuraCoreController/RGBController_AsusAuraCoreLaptop.h                       \
    Controllers/AsusAuraGPUController/AsusAuraGPUController.h                                   \
    Controllers/AsusAuraGPUController/RGBController_AsusAuraGPU.h                               \
    Controllers/AsusAuraUSBController/AsusAuraUSBController.h                                   \
    Controllers/AsusAuraUSBController/AsusAuraAddressableController.h                           \
    Controllers/AsusAuraUSBController/AsusAuraHeadsetStandController.h                          \
    Controllers/AsusAuraUSBController/AsusAuraKeyboardController.h                              \
    Controllers/AsusAuraUSBController/AsusAuraMainboardController.h                             \
    Controllers/AsusAuraUSBController/AsusAuraMonitorController.h                               \
    Controllers/AsusAuraUSBController/AsusAuraMouseController.h                                 \
    Controllers/AsusAuraUSBController/AsusAuraMousematController.h                              \
    Controllers/AsusAuraUSBController/AsusAuraMouseDevices.h                                    \
    Controllers/AsusAuraUSBController/AsusAuraRyuoAIOController.h                               \
    Controllers/AsusAuraUSBController/AsusAuraStrixEvolveController.h                           \
    Controllers/AsusAuraUSBController/AsusAuraTUFKeyboardController.h                           \
    Controllers/AsusAuraUSBController/AsusAuraTUFKeyboardLayouts.h                              \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraHeadsetStand.h                      \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraKeyboard.h                          \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMonitor.h                           \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMouse.h                             \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMousemat.h                          \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraRyuoAIO.h                           \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraStrixEvolve.h                       \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraTUFKeyboard.h                       \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraUSB.h                               \
    Controllers/AsusAuraUSBController/RGBController_ROGStrixLC_Controller.h                     \
    Controllers/AsusAuraUSBController/ROGStrixLC_Controller.h                                   \
    Controllers/AsusLegacyUSBController/AsusCerberusKeyboardController.h                        \
    Controllers/AsusLegacyUSBController/AsusStrixClawController.h                               \
    Controllers/AsusLegacyUSBController/RGBController_AsusCerberusKeyboard.h                    \
    Controllers/AsusLegacyUSBController/RGBController_AsusStrixClaw.h                           \
    Controllers/BlinkyTapeController/BlinkyTapeController.h                                     \
    Controllers/BlinkyTapeController/RGBController_BlinkyTape.h                                 \
    Controllers/CherryKeyboardController/CherryKeyboardController.h                             \
    Controllers/CherryKeyboardController/RGBController_CherryKeyboard.h                         \
    Controllers/ColorfulGPUController/ColorfulGPUController.h                                   \
    Controllers/ColorfulGPUController/RGBController_ColorfulGPU.h                               \
    Controllers/ColorfulTuringGPUController/ColorfulTuringGPUController.h                       \
    Controllers/ColorfulTuringGPUController/RGBController_ColorfulTuringGPU.h                   \
    Controllers/CoolerMasterController/CMARGBcontroller.h                                       \
    Controllers/CoolerMasterController/CMARGBGen2A1controller.h                                 \
    Controllers/CoolerMasterController/CMMKController.h                                         \
    Controllers/CoolerMasterController/CMMMController.h                                         \
    Controllers/CoolerMasterController/CMMM711Controller.h                                      \
    Controllers/CoolerMasterController/CMMP750Controller.h                                      \
    Controllers/CoolerMasterController/CMR6000Controller.h                                      \
    Controllers/CoolerMasterController/CMRGBController.h                                        \
    Controllers/CoolerMasterController/CMSmallARGBController.h                                  \
    Controllers/CoolerMasterController/RGBController_CMARGBController.h                         \
    Controllers/CoolerMasterController/RGBController_CMARGBGen2A1Controller.h                   \
    Controllers/CoolerMasterController/RGBController_CMMKController.h                           \
    Controllers/CoolerMasterController/RGBController_CMMMController.h                           \
    Controllers/CoolerMasterController/RGBController_CMMM711Controller.h                        \
    Controllers/CoolerMasterController/RGBController_CMMP750Controller.h                        \
    Controllers/CoolerMasterController/RGBController_CMR6000Controller.h                        \
    Controllers/CoolerMasterController/RGBController_CMRGBController.h                          \
    Controllers/CoolerMasterController/RGBController_CMSmallARGBController.h                    \
    Controllers/CorsairCommanderCoreController/CorsairCommanderCoreController.h                 \
    Controllers/CorsairCommanderCoreController/RGBController_CorsairCommanderCore.h             \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumController.h         \
    Controllers/CorsairDominatorPlatinumController/RGBController_CorsairDominatorPlatinum.h     \
    Controllers/CorsairHydroController/CorsairHydroController.h                                 \
    Controllers/CorsairHydroController/RGBController_CorsairHydro.h                             \
    Controllers/CorsairHydro2Controller/CorsairHydro2Controller.h                               \
    Controllers/CorsairHydro2Controller/RGBController_CorsairHydro2.h                           \
    Controllers/CorsairHydroPlatinumController/CorsairHydroPlatinumController.h                 \
    Controllers/CorsairHydroPlatinumController/RGBController_CorsairHydroPlatinum.h             \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.h                   \
    Controllers/CorsairLightingNodeController/RGBController_CorsairLightingNode.h               \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.h                       \
    Controllers/CorsairPeripheralController/CorsairK100Controller.h                             \
    Controllers/CorsairPeripheralController/CorsairK55RGBPROXTController.h                      \
    Controllers/CorsairPeripheralController/CorsairK65MiniController.h                          \
    Controllers/CorsairPeripheralController/CorsairK95PlatinumXTController.h                    \
    Controllers/CorsairPeripheralController/RGBController_CorsairPeripheral.h                   \
    Controllers/CorsairPeripheralController/RGBController_CorsairK100.h                         \
    Controllers/CorsairPeripheralController/RGBController_CorsairK55RGBPROXT.h                  \
    Controllers/CorsairPeripheralController/RGBController_CorsairK65Mini.h                      \
    Controllers/CorsairPeripheralController/RGBController_CorsairK95PlatinumXT.h                \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2Controller.h                   \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2Devices.h                      \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2HardwareController.h           \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2SoftwareController.h           \
    Controllers/CorsairPeripheralV2Controller/RGBController_CorsairV2Hardware.h                 \
    Controllers/CorsairPeripheralV2Controller/RGBController_CorsairV2Software.h                 \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.h                         \
    Controllers/CorsairVengeanceController/RGBController_CorsairVengeance.h                     \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.h                   \
    Controllers/CorsairVengeanceProController/RGBController_CorsairVengeancePro.h               \
    Controllers/CorsairWirelessController/CorsairWirelessController.h                           \
    Controllers/CorsairWirelessController/RGBController_CorsairWireless.h                       \
    Controllers/CougarController/CougarKeyboardController.h                                     \
    Controllers/CougarController/CougarRevengerSTController.h                                   \
    Controllers/CougarController/RGBController_CougarKeyboard.h                                 \
    Controllers/CougarController/RGBController_CougarRevengerST.h                               \
    Controllers/CreativeController/CreativeSoundBlasterXG6Controller.h                          \
    Controllers/CreativeController/RGBController_CreativeSoundBlasterXG6.h                      \
    Controllers/CrucialController/CrucialController.h                                           \
    Controllers/CrucialController/RGBController_Crucial.h                                       \
    Controllers/CryorigH7QuadLumiController/CryorigH7QuadLumiController.h                       \
    Controllers/CryorigH7QuadLumiController/RGBController_CryorigH7QuadLumi.h                   \
    Controllers/DarkProject/DarkProjectKeyboardController.h                                     \
    Controllers/DarkProject/RGBController_DarkProjectKeyboard.h                                 \
    Controllers/DasKeyboardController/DasKeyboardController.h                                   \
    Controllers/DasKeyboardController/RGBController_DasKeyboard.h                               \
    Controllers/DMXController/RGBController_DMX.h                                               \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.h                               \
    Controllers/DuckyKeyboardController/RGBController_DuckyKeyboard.h                           \
    Controllers/DygmaRaiseController/DygmaRaiseController.h                                     \
    Controllers/DygmaRaiseController/RGBController_DygmaRaise.h                                 \
    Controllers/DebugController/RGBController_Debug.h                                           \
    Controllers/E131Controller/RGBController_E131.h                                             \
    Controllers/EKController/EKController.h                                                     \
    Controllers/EKController/RGBController_EKController.h                                       \
    Controllers/ElgatoKeyLightController/ElgatoKeyLightController.h                             \
    Controllers/ElgatoKeyLightController/RGBController_ElgatoKeyLight.h                         \
    Controllers/ENESMBusController/ENESMBusController.h                                         \
    Controllers/ENESMBusController/RGBController_ENESMBus.h                                     \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface.h                        \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_i2c_smbus.h              \
    Controllers/EpomakerController/EpomakerController.h                                         \
    Controllers/EpomakerController/RGBController_EpomakerController.h                           \
    Controllers/EspurnaController/EspurnaController.h                                           \
    Controllers/EspurnaController/RGBController_Espurna.h                                       \
    Controllers/EVGAGP102GPUController/EVGAGP102Controller.h                                    \
    Controllers/EVGAGP102GPUController/RGBController_EVGAGP102.h                                \
    Controllers/EVGAPascalGPUController/EVGAGPUv1Controller.h                                   \
    Controllers/EVGAPascalGPUController/RGBController_EVGAGPUv1.h                               \
    Controllers/EVGATuringGPUController/EVGAGPUv2Controller.h                                   \
    Controllers/EVGATuringGPUController/RGBController_EVGAGPUv2.h                               \
    Controllers/EVGAAmpereGPUController/EVGAGPUv3Controller.h                                   \
    Controllers/EVGAAmpereGPUController/RGBController_EVGAGPUv3.h                               \
    Controllers/EVGAUSBController/EVGAKeyboardController.h                                      \
    Controllers/EVGAUSBController/EVGAMouseController.h                                         \
    Controllers/EVGAUSBController/RGBController_EVGAKeyboard.h                                  \
    Controllers/EVGAUSBController/RGBController_EVGAMouse.h                                     \
    Controllers/EVisionKeyboardController/EVisionKeyboardController.h                           \
    Controllers/EVisionKeyboardController/RGBController_EVisionKeyboard.h                       \
    Controllers/FanBusController/FanBusController.h                                             \
    Controllers/FanBusController/FanBusInterface.h                                              \
    Controllers/FanBusController/RGBController_FanBus.h                                         \
    Controllers/GainwardGPUController/GainwardGPUv1Controller.h                                 \
    Controllers/GainwardGPUController/GainwardGPUv2Controller.h                                 \
    Controllers/GainwardGPUController/RGBController_GainwardGPUv1.h                             \
    Controllers/GainwardGPUController/RGBController_GainwardGPUv2.h                             \
    Controllers/GaiZongGaiKeyboardController/GaiZhongGaiKeyboardController.h                    \
    Controllers/GaiZongGaiKeyboardController/RGBController_GaiZhongGaiKeyboard.h                \
    Controllers/GalaxGPUController/GalaxGPUController.h                                         \
    Controllers/GalaxGPUController/RGBController_GalaxGPU.h                                     \
    Controllers/GigabyteAorusCPUCoolerController/ATC800Controller.h                             \
    Controllers/GigabyteAorusCPUCoolerController/RGBController_AorusATC800.h                    \
    Controllers/GigabyteAorusMouseController/GigabyteAorusMouseController.h                     \
    Controllers/GigabyteAorusMouseController/RGBController_GigabyteAorusMouse.h                 \
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
    Controllers/GigabyteRGBFusion2GPUController/GigabyteRGBFusion2GPUController.h               \
    Controllers/GigabyteRGBFusion2GPUController/RGBController_GigabyteRGBFusion2GPU.h           \
    Controllers/GigabyteSuperIORGBController/GigabyteSuperIORGBController.h                     \
    Controllers/HoltekController/HoltekA070Controller.h                                         \
    Controllers/HoltekController/HoltekA1FAController.h                                         \
    Controllers/HoltekController/RGBController_HoltekA070.h                                     \
    Controllers/HoltekController/RGBController_HoltekA1FA.h                                     \
    Controllers/HPOmen30LController/HPOmen30LController.h                                       \
    Controllers/HPOmen30LController/RGBController_HPOmen30L.h                                   \
    Controllers/HyperXDRAMController/HyperXDRAMController.h                                     \
    Controllers/HyperXDRAMController/RGBController_HyperXDRAM.h                                 \
    Controllers/HyperXKeyboardController/HyperXAlloyEliteController.h                           \
    Controllers/HyperXKeyboardController/HyperXAlloyElite2Controller.h                          \
    Controllers/HyperXKeyboardController/HyperXAlloyFPSController.h                             \
    Controllers/HyperXKeyboardController/HyperXAlloyOrigins60and65Controller.h                  \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.h                         \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsCoreController.h                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyElite.h                       \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyElite2.h                      \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyFPS.h                         \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins.h                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins60and65.h              \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOriginsCore.h                 \
    Controllers/HyperXMicrophoneController/HyperXMicrophoneController.h                         \
    Controllers/HyperXMicrophoneController/RGBController_HyperXMicrophone.h                     \
    Controllers/HyperXMouseController/HyperXPulsefireFPSProController.h                         \
    Controllers/HyperXMouseController/HyperXPulsefireHasteController.h                          \
    Controllers/HyperXMouseController/HyperXPulsefireSurgeController.h                          \
    Controllers/HyperXMouseController/HyperXPulsefireDartController.h                           \
    Controllers/HyperXMouseController/HyperXPulsefireRaidController.h                           \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireFPSPro.h                     \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireHaste.h                      \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireSurge.h                      \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireDart.h                       \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireRaid.h                       \
    Controllers/HyperXMousematController/HyperXMousematController.h                             \
    Controllers/HyperXMousematController/RGBController_HyperXMousemat.h                         \
    Controllers/IntelArcA770LEController/IntelArcA770LEController.h                             \
    Controllers/IntelArcA770LEController/RGBController_IntelArcA770LE.h                         \
    Controllers/IonicoController/IonicoController.h                                             \
    Controllers/IonicoController/RGBController_Ionico.h                                         \
    Controllers/KasaSmartController/KasaSmartController.h                                       \
    Controllers/KasaSmartController/RGBController_KasaSmart.h                                   \
    Controllers/KeychronKeyboardController/KeychronKeyboardController.h                         \
    Controllers/KeychronKeyboardController/RGBController_KeychronKeyboard.h                     \
    Controllers/LEDStripController/LEDStripController.h                                         \
    Controllers/LEDStripController/RGBController_LEDStrip.h                                     \
    Controllers/LegoDimensionsToypadBaseController/LegoDimensionsToypadBaseController.h         \
    Controllers/LegoDimensionsToypadBaseController/RGBController_LegoDimensionsToypadBase.h     \
    Controllers/LenovoControllers/LenovoDevices.h                                               \
    Controllers/LenovoControllers/LenovoDevices4Zone.h                                          \
    Controllers/LenovoControllers/LenovoUSBController.h                                         \
    Controllers/LenovoControllers/Lenovo4ZoneUSBController.h                                    \
    Controllers/LenovoControllers/RGBController_LenovoUSB.h                                     \
    Controllers/LenovoControllers/RGBController_Lenovo4ZoneUSB.h                                \
    Controllers/LenovoMotherboardController/LenovoMotherboardController.h                       \
    Controllers/LenovoMotherboardController/RGBController_LenovoMotherboard.h                   \
    Controllers/LexipMouseController/LexipMouseController.h                                     \
    Controllers/LexipMouseController/RGBController_LexipMouse.h                                 \
    Controllers/LIFXController/LIFXController.h                                                 \
    Controllers/LIFXController/RGBController_LIFX.h                                             \
    Controllers/LianLiController/LianLiUniHubController.h                                       \
    Controllers/LianLiController/LianLiUniHub_AL10Controller.h                                  \
    Controllers/LianLiController/RGBController_LianLiUniHub.h                                   \
    Controllers/LianLiController/RGBController_StrimerLConnect.h                                \
    Controllers/LianLiController/StrimerLConnectController.h                                    \
    Controllers/LianLiController/LianLiUniHubALController.h                                     \
    Controllers/LianLiController/RGBController_LianLiUniHubAL.h                                 \
    Controllers/LianLiController/RGBController_LianLiUniHub_AL10.h                              \
    Controllers/LianLiController/LianLiUniHubSLV2Controller.h                                   \
    Controllers/LianLiController/RGBController_LianLiUniHubSLV2.h                               \
    Controllers/LogitechController/LogitechProtocolCommon.h                                     \
    Controllers/LogitechController/LogitechG203LController.h                                    \
    Controllers/LogitechController/LogitechG213Controller.h                                     \
    Controllers/LogitechController/LogitechG560Controller.h                                     \
    Controllers/LogitechController/LogitechG933Controller.h                                     \
    Controllers/LogitechController/LogitechG810Controller.h                                     \
    Controllers/LogitechController/LogitechGProKeyboardController.h                             \
    Controllers/LogitechController/LogitechG910Controller.h                                     \
    Controllers/LogitechController/LogitechG815Controller.h                                     \
    Controllers/LogitechController/LogitechG915Controller.h                                     \
    Controllers/LogitechController/LogitechGLightsyncController.h                               \
    Controllers/LogitechController/LogitechLightspeedController.h                               \
    Controllers/LogitechController/LogitechX56Controller.h                                      \
    Controllers/LogitechController/RGBController_LogitechG203L.h                                \
    Controllers/LogitechController/RGBController_LogitechG213.h                                 \
    Controllers/LogitechController/RGBController_LogitechG560.h                                 \
    Controllers/LogitechController/RGBController_LogitechG933.h                                 \
    Controllers/LogitechController/RGBController_LogitechG810.h                                 \
    Controllers/LogitechController/RGBController_LogitechGProKeyboard.h                         \
    Controllers/LogitechController/RGBController_LogitechG910.h                                 \
    Controllers/LogitechController/RGBController_LogitechG815.h                                 \
    Controllers/LogitechController/RGBController_LogitechG915.h                                 \
    Controllers/LogitechController/RGBController_LogitechGLightsync.h                           \
    Controllers/LogitechController/RGBController_LogitechGLightsync1zone.h                      \
    Controllers/LogitechController/RGBController_LogitechGPowerPlay.h                           \
    Controllers/LogitechController/RGBController_LogitechLightspeed.h                           \
    Controllers/LogitechController/RGBController_LogitechX56.h                                  \
    Controllers/MountainKeyboardController/MountainKeyboardController.h                         \
    Controllers/MountainKeyboardController/RGBController_MountainKeyboard.h                     \
    Controllers/MSI3ZoneController/MSI3ZoneController.h                                         \
    Controllers/MSI3ZoneController/RGBController_MSI3Zone.h                                     \
    Controllers/MSIGPUController/MSIGPUController.h                                             \
    Controllers/MSIGPUController/MSIGPUv2Controller.h                                           \
    Controllers/MSIGPUController/RGBController_MSIGPU.h                                         \
    Controllers/MSIGPUController/RGBController_MSIGPUv2.h                                       \
    Controllers/MSIMysticLightController/MSIMysticLightCommon.h                                 \
    Controllers/MSIMysticLightController/MSIMysticLight64Controller.h                           \
    Controllers/MSIMysticLightController/MSIMysticLight112Controller.h                          \
    Controllers/MSIMysticLightController/MSIMysticLight162Controller.h                          \
    Controllers/MSIMysticLightController/MSIMysticLight185Controller.h                          \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight64.h                       \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight112.h                      \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight162.h                      \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight185.h                      \
    Controllers/MSIOptixController/MSIOptixController.h                                         \
    Controllers/MSIOptixController/RGBController_MSIOptix.h                                     \
    Controllers/MSIRGBController/MSIRGBController.h                                             \
    Controllers/MSIVigorController/RGBController_MSIVigorGK30.h                                 \
    Controllers/MSIVigorController/MSIVigorGK30Controller.h                                     \
    Controllers/NanoleafController/NanoleafController.h                                         \
    Controllers/MSIRGBController/RGBController_MSIRGB.h                                         \
    Controllers/N5312AController/N5312AController.h                                             \
    Controllers/N5312AController/RGBController_N5312A.h                                         \
    Controllers/NvidiaESAController/NvidiaESAController.h                                       \
    Controllers/NanoleafController/RGBController_Nanoleaf.h                                     \
    Controllers/NvidiaESAController/RGBController_NvidiaESA.h                                   \
    Controllers/NZXTHue1Controller/NZXTHue1Controller.h                                         \
    Controllers/NZXTHue1Controller/RGBController_NZXTHue1.h                                     \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.h                                         \
    Controllers/NZXTHue2Controller/RGBController_NZXTHue2.h                                     \
    Controllers/NZXTHuePlusController/NZXTHuePlusController.h                                   \
    Controllers/NZXTHuePlusController/RGBController_NZXTHuePlus.h                               \
    Controllers/NZXTKrakenController/NZXTKrakenController.h                                     \
    Controllers/NZXTKrakenController/RGBController_NZXTKraken.h                                 \
    Controllers/OpenRazerController/OpenRazerDevices.h                                          \
    Controllers/PalitGPUController/PalitGPUController.h                                         \
    Controllers/PalitGPUController/RGBController_PalitGPU.h                                     \
    Controllers/PatriotViperController/PatriotViperController.h                                 \
    Controllers/PatriotViperController/RGBController_PatriotViper.h                             \
    Controllers/PatriotViperSteelController/PatriotViperSteelController.h                       \
    Controllers/PatriotViperSteelController/RGBController_PatriotViperSteel.h                   \
    Controllers/PhilipsHueController/PhilipsHueController.h                                     \
    Controllers/PhilipsHueController/RGBController_PhilipsHue.h                                 \
    Controllers/PhilipsWizController/PhilipsWizController.h                                     \
    Controllers/PhilipsWizController/RGBController_PhilipsWiz.h                                 \
    Controllers/PNYGPUController/PNYGPUController.h                                             \
    Controllers/PNYGPUController/RGBController_PNYGPU.h                                         \
    Controllers/QMKOpenRGBController/QMKOpenRGBController.h                                     \
    Controllers/QMKOpenRGBController/QMKOpenRGBRev9Controller.h                                 \
    Controllers/QMKOpenRGBController/QMKOpenRGBRevBController.h                                 \
    Controllers/QMKOpenRGBController/QMKOpenRGBRevDController.h                                 \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRev9.h                             \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRevB.h                             \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRevD.h                             \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRevE.h                             \
    Controllers/RazerController/RazerController.h                                               \
    Controllers/RazerController/RazerKrakenController.h                                         \
    Controllers/RazerController/RazerDevices.h                                                  \
    Controllers/RazerController/RGBController_Razer.h                                           \
    Controllers/RazerController/RGBController_RazerAddressable.h                                \
    Controllers/RazerController/RGBController_RazerKraken.h                                     \
    Controllers/RedragonController/RGBController_RedragonMouse.h                                \
    Controllers/RedragonController/RedragonMouseController.h                                    \
    Controllers/RedSquareKeyroxController/RedSquareKeyroxController.h                           \
    Controllers/RedSquareKeyroxController/RedSquareKeyroxTKLClassicController.h                 \
    Controllers/RedSquareKeyroxController/RGBController_RedSquareKeyrox.h                       \
    Controllers/RedSquareKeyroxController/RGBController_RedSquareKeyroxTKLClassic.h             \
    Controllers/RoccatController/RGBController_RoccatBurst.h                                    \
    Controllers/RoccatController/RGBController_RoccatElo.h                                      \
    Controllers/RoccatController/RGBController_RoccatHordeAimo.h                                \
    Controllers/RoccatController/RGBController_RoccatKoneAimo.h                                 \
    Controllers/RoccatController/RGBController_RoccatKova.h                                     \
    Controllers/RoccatController/RGBController_RoccatVulcanAimo.h                               \
    Controllers/RoccatController/RoccatBurstController.h                                        \
    Controllers/RoccatController/RoccatEloController.h                                          \
    Controllers/RoccatController/RoccatHordeAimoController.h                                    \
    Controllers/RoccatController/RoccatKoneAimoController.h                                     \
    Controllers/RoccatController/RoccatKovaController.h                                         \
    Controllers/RoccatController/RoccatVulcanAimoController.h                                   \
    Controllers/RoccatController/RoccatVulcanAimoLayouts.h                                      \
    Controllers/SapphireGPUController/SapphireNitroGlowV1Controller.h                           \
    Controllers/SapphireGPUController/SapphireNitroGlowV3Controller.h                           \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV1.h                       \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV3.h                       \
    Controllers/SinowealthController/SinowealthController.h                                     \
    Controllers/SinowealthController/SinowealthController1007.h                                 \
    Controllers/SinowealthController/SinowealthGMOWController.h                                 \
    Controllers/SinowealthController/SinowealthKeyboardController.h                             \
    Controllers/SinowealthController/SinowealthKeyboard16Controller.h                           \
    Controllers/SinowealthController/SinowealthKeyboard90Controller.h                           \
    Controllers/SinowealthController/RGBController_Sinowealth.h                                 \
    Controllers/SinowealthController/RGBController_Sinowealth1007.h                             \
    Controllers/SinowealthController/RGBController_SinowealthGMOW.h                             \
    Controllers/SinowealthController/RGBController_SinowealthKeyboard.h                         \
    Controllers/SinowealthController/RGBController_SinowealthKeyboard16.h                       \
    Controllers/SinowealthController/RGBController_SinowealthKeyboard90.h                       \
    Controllers/SonyGamepadController/SonyDualSenseController.h                                 \
    Controllers/SonyGamepadController/RGBController_SonyDualSense.h                             \
    Controllers/SonyGamepadController/SonyDS4Controller.h                                       \
    Controllers/SonyGamepadController/RGBController_SonyDS4.h                                   \
    Controllers/SRGBmodsController/SRGBmodsPicoController.h                                     \
    Controllers/SRGBmodsController/RGBController_SRGBmodsPico.h                                 \
    Controllers/SteelSeriesController/color32.h                                                 \
    Controllers/SteelSeriesController/SteelSeriesAerox3Controller.h                             \
    Controllers/SteelSeriesController/SteelSeriesAerox9Controller.h                             \
    Controllers/SteelSeriesController/SteelSeriesApexBaseController.h                           \
    Controllers/SteelSeriesController/SteelSeriesApexController.h                               \
    Controllers/SteelSeriesController/SteelSeriesApex3Controller.h                              \
    Controllers/SteelSeriesController/SteelSeriesApex8ZoneController.h                          \
    Controllers/SteelSeriesController/SteelSeriesApexMController.h                              \
    Controllers/SteelSeriesController/SteelSeriesApexRegions.h                                  \
    Controllers/SteelSeriesController/SteelSeriesApexTZoneController.h                          \
    Controllers/SteelSeriesController/SteelSeriesArctis5Controller.h                            \
    Controllers/SteelSeriesController/SteelSeriesGeneric.h                                      \
    Controllers/SteelSeriesController/SteelSeriesMouseController.h                              \
    Controllers/SteelSeriesController/SteelSeriesOldApexController.h                            \
    Controllers/SteelSeriesController/SteelSeriesRivalController.h                              \
    Controllers/SteelSeriesController/SteelSeriesRival3Controller.h                             \
    Controllers/SteelSeriesController/SteelSeriesSenseiController.h                             \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.h                            \
    Controllers/SteelSeriesController/SteelSeriesQCKMatController.h                             \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex.h                           \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex3.h                          \
    Controllers/SteelSeriesController/RGBController_SteelSeriesArctis5.h                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesOldApex.h                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival.h                          \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival3.h                         \
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
    Controllers/ThingMController/BlinkController.h                                              \
    Controllers/ThingMController/RGBController_BlinkController.h                                \
    Controllers/TrustController/RGBController_TrustGXT114.h                                     \
    Controllers/TrustController/RGBController_TrustGXT180.h                                     \
    Controllers/TrustController/TrustGXT114Controller.h                                         \
    Controllers/TrustController/TrustGXT180Controller.h                                         \
    Controllers/ViewSonicController/VS_XG270QG_Controller.h                                     \
    Controllers/ViewSonicController/RGBController_XG270QG.h                                     \
    Controllers/WootingKeyboardController/WootingKeyboardController.h                           \
    Controllers/WootingKeyboardController/WootingOneKeyboardController.h                        \
    Controllers/WootingKeyboardController/WootingTwoKeyboardController.h                        \
    Controllers/WootingKeyboardController/RGBController_WootingKeyboard.h                       \
    Controllers/WushiController/WushiL50USBController.h                                         \
    Controllers/WushiController/RGBController_WushiL50USB.h                                     \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiingQuad.h                 \
    Controllers/YeelightController/YeelightController.h                                         \
    Controllers/YeelightController/RGBController_Yeelight.h                                     \
    Controllers/ZalmanZSyncController/ZalmanZSyncController.h                                   \
    Controllers/ZalmanZSyncController/RGBController_ZalmanZSync.h                               \
    Controllers/ZETEdgeAirProController/ZETEdgeAirProController.h                               \
    Controllers/ZETEdgeAirProController/RGBController_ZETEdgeAirPro.h                           \
    Controllers/ZETKeyboardController/ZETBladeOpticalController.h                               \
    Controllers/ZETKeyboardController/RGBController_ZETBladeOptical.h                           \
    Controllers/ZotacTuringGPUController/ZotacTuringGPUController.h                             \
    Controllers/ZotacTuringGPUController/RGBController_ZotacTuringGPU.h                         \
    KeyboardLayoutManager/KeyboardLayoutManager.h                                               \
    RGBController/RGBController.h                                                               \
    RGBController/RGBController_Dummy.h                                                         \
    RGBController/RGBControllerKeyNames.h                                                       \
    RGBController/RGBController_Network.h                                                       \

contains(QMAKE_PLATFORM, freebsd) {
    HEADERS -=                                                                                  \
        Controllers/GigabyteRGBFusion2GPUController/GigabyteRGBFusion2GPUController.h           \
        Controllers/GigabyteRGBFusion2GPUController/RGBController_GigabyteRGBFusion2GPU.h       \
        Controllers/HoltekController/HoltekA070Controller.h                                     \
        Controllers/HoltekController/HoltekA1FAController.h                                     \
        Controllers/HoltekController/RGBController_HoltekA070.h                                 \
        Controllers/HoltekController/RGBController_HoltekA1FA.h
}

SOURCES +=                                                                                      \
    dependencies/Swatches/swatches.cpp                                                          \
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
    qt/OpenRGBConsolePage.cpp                                                                   \
    qt/OpenRGBDeviceInfoPage.cpp                                                                \
    qt/OpenRGBDevicePage.cpp                                                                    \
    qt/OpenRGBDialog.cpp                                                                        \
    i2c_smbus/i2c_smbus.cpp                                                                     \
    i2c_tools/i2c_tools.cpp                                                                     \
    net_port/net_port.cpp                                                                       \
    qt/DeviceView.cpp                                                                           \
    qt/OpenRGBDialog2.cpp                                                                       \
    qt/OpenRGBElgatoKeyLightSettingsPage/OpenRGBElgatoKeyLightSettingsEntry.cpp                 \
    qt/OpenRGBElgatoKeyLightSettingsPage/OpenRGBElgatoKeyLightSettingsPage.cpp                  \
    qt/OpenRGBHardwareIDsDialog.cpp                                                             \
    qt/OpenRGBKasaSmartSettingsPage/OpenRGBKasaSmartSettingsEntry.cpp                           \
    qt/OpenRGBKasaSmartSettingsPage/OpenRGBKasaSmartSettingsPage.cpp                            \
    qt/OpenRGBPluginContainer.cpp                                                               \
    qt/OpenRGBPluginsPage/OpenRGBPluginsEntry.cpp                                               \
    qt/OpenRGBPluginsPage/OpenRGBPluginsList.cpp                                                \
    qt/OpenRGBPluginsPage/OpenRGBPluginsPage.cpp                                                \
    qt/OpenRGBProfileSaveDialog.cpp                                                             \
    qt/OpenRGBServerInfoPage.cpp                                                                \
    qt/OpenRGBSettingsPage.cpp                                                                  \
    qt/OpenRGBSoftwareInfoPage.cpp                                                              \
    qt/OpenRGBSupportedDevicesPage.cpp                                                          \
    qt/OpenRGBSystemInfoPage.cpp                                                                \
    qt/OpenRGBThemeManager.cpp                                                                  \
    qt/OpenRGBZoneResizeDialog.cpp                                                              \
    qt/OpenRGBZonesBulkResizer.cpp                                                              \
    qt/QTooltipedSlider.cpp                                                                     \
    qt/TabLabel.cpp                                                                             \
    qt/hsv.cpp                                                                                  \
    qt/OpenRGBDMXSettingsPage/OpenRGBDMXSettingsEntry.cpp                                       \
    qt/OpenRGBDMXSettingsPage/OpenRGBDMXSettingsPage.cpp                                        \
    qt/OpenRGBE131SettingsPage/OpenRGBE131SettingsEntry.cpp                                     \
    qt/OpenRGBE131SettingsPage/OpenRGBE131SettingsPage.cpp                                      \
    qt/OpenRGBLIFXSettingsPage/OpenRGBLIFXSettingsEntry.cpp                                     \
    qt/OpenRGBLIFXSettingsPage/OpenRGBLIFXSettingsPage.cpp                                      \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafSettingsEntry.cpp                             \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafSettingsPage.cpp                              \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafScanningThread.cpp                            \
    qt/OpenRGBPhilipsHueSettingsPage/OpenRGBPhilipsHueSettingsEntry.cpp                         \
    qt/OpenRGBPhilipsHueSettingsPage/OpenRGBPhilipsHueSettingsPage.cpp                          \
    qt/OpenRGBPhilipsWizSettingsPage/OpenRGBPhilipsWizSettingsEntry.cpp                         \
    qt/OpenRGBPhilipsWizSettingsPage/OpenRGBPhilipsWizSettingsPage.cpp                          \
    qt/OpenRGBQMKORGBSettingsPage/OpenRGBQMKORGBSettingsEntry.cpp                               \
    qt/OpenRGBQMKORGBSettingsPage/OpenRGBQMKORGBSettingsPage.cpp                                \
    qt/OpenRGBSerialSettingsPage/OpenRGBSerialSettingsEntry.cpp                                 \
    qt/OpenRGBSerialSettingsPage/OpenRGBSerialSettingsPage.cpp                                  \
    qt/OpenRGBYeelightSettingsPage/OpenRGBYeelightSettingsEntry.cpp                             \
    qt/OpenRGBYeelightSettingsPage/OpenRGBYeelightSettingsPage.cpp                              \
    serial_port/serial_port.cpp                                                                 \
    StringUtils.cpp                                                                             \
    super_io/super_io.cpp                                                                       \
    AutoStart/AutoStart.cpp                                                                     \
    Controllers/A4TechController/A4Tech_Detector.cpp                                            \
    Controllers/A4TechController/BloodyMouseController.cpp                                      \
    Controllers/A4TechController/RGBController_BloodyMouse.cpp                                  \
    Controllers/AlienwareController/AlienwareController.cpp                                     \
    Controllers/AlienwareController/AlienwareControllerDetect.cpp                               \
    Controllers/AlienwareController/RGBController_Alienware.cpp                                 \
    Controllers/AlienwareKeyboardController/AlienwareAW510KController.cpp                       \
    Controllers/AlienwareKeyboardController/AlienwareKeyboardControllerDetect.cpp               \
    Controllers/AlienwareKeyboardController/RGBController_AlienwareAW510K.cpp                   \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.cpp                           \
    Controllers/AMDWraithPrismController/AMDWraithPrismControllerDetect.cpp                     \
    Controllers/AMDWraithPrismController/RGBController_AMDWraithPrism.cpp                       \
    Controllers/AnnePro2Controller/AnnePro2Controller.cpp                                       \
    Controllers/AnnePro2Controller/AnnePro2ControllerDetect.cpp                                 \
    Controllers/AnnePro2Controller/RGBController_AnnePro2.cpp                                   \
    Controllers/AOCKeyboardController/AOCKeyboardController.cpp                                 \
    Controllers/AOCKeyboardController/AOCKeyboardControllerDetect.cpp                           \
    Controllers/AOCKeyboardController/RGBController_AOCKeyboard.cpp                             \
    Controllers/AOCMouseController/AOCMouseController.cpp                                       \
    Controllers/AOCMouseController/AOCMouseControllerDetect.cpp                                 \
    Controllers/AOCMouseController/RGBController_AOCMouse.cpp                                   \
    Controllers/AOCMousematController/AOCMousematController.cpp                                 \
    Controllers/AOCMousematController/AOCMousematControllerDetect.cpp                           \
    Controllers/AOCMousematController/RGBController_AOCMousemat.cpp                             \
    Controllers/ASRockPolychromeUSBController/ASRockPolychromeUSBController.cpp                 \
    Controllers/ASRockPolychromeUSBController/ASRockPolychromeUSBControllerDetect.cpp           \
    Controllers/ASRockPolychromeUSBController/RGBController_ASRockPolychromeUSB.cpp             \
    Controllers/ASRockSMBusController/ASRockASRRGBSMBusController.cpp                           \
    Controllers/ASRockSMBusController/ASRockPolychromeV1SMBusController.cpp                     \
    Controllers/ASRockSMBusController/ASRockPolychromeV2SMBusController.cpp                     \
    Controllers/ASRockSMBusController/ASRockSMBusControllerDetect.cpp                           \
    Controllers/ASRockSMBusController/RGBController_ASRockASRRGBSMBus.cpp                       \
    Controllers/ASRockSMBusController/RGBController_ASRockPolychromeV1SMBus.cpp                 \
    Controllers/ASRockSMBusController/RGBController_ASRockPolychromeV2SMBus.cpp                 \
    Controllers/AsusAuraCoreController/AsusAuraCoreController.cpp                               \
    Controllers/AsusAuraCoreController/AsusAuraCoreControllerDetect.cpp                         \
    Controllers/AsusAuraCoreController/AsusAuraCoreLaptopController.cpp                         \
    Controllers/AsusAuraCoreController/RGBController_AsusAuraCore.cpp                           \
    Controllers/AsusAuraCoreController/RGBController_AsusAuraCoreLaptop.cpp                     \
    Controllers/AsusAuraGPUController/AsusAuraGPUController.cpp                                 \
    Controllers/AsusAuraGPUController/AsusAuraGPUControllerDetect.cpp                           \
    Controllers/AsusAuraGPUController/RGBController_AsusAuraGPU.cpp                             \
    Controllers/AsusAuraUSBController/AsusAuraUSBController.cpp                                 \
    Controllers/AsusAuraUSBController/AsusAuraAddressableController.cpp                         \
    Controllers/AsusAuraUSBController/AsusAuraHeadsetStandController.cpp                        \
    Controllers/AsusAuraUSBController/AsusAuraKeyboardController.cpp                            \
    Controllers/AsusAuraUSBController/AsusAuraMainboardController.cpp                           \
    Controllers/AsusAuraUSBController/AsusAuraMonitorController.cpp                             \
    Controllers/AsusAuraUSBController/AsusAuraMouseController.cpp                               \
    Controllers/AsusAuraUSBController/AsusAuraMousematController.cpp                            \
    Controllers/AsusAuraUSBController/AsusAuraRyuoAIOController.cpp                             \
    Controllers/AsusAuraUSBController/AsusAuraStrixEvolveController.cpp                         \
    Controllers/AsusAuraUSBController/AsusAuraTUFKeyboardController.cpp                         \
    Controllers/AsusAuraUSBController/AsusAuraUSBControllerDetect.cpp                           \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraHeadsetStand.cpp                    \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraKeyboard.cpp                        \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMonitor.cpp                         \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMouse.cpp                           \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraMousemat.cpp                        \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraRyuoAIO.cpp                         \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraStrixEvolve.cpp                     \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraTUFKeyboard.cpp                     \
    Controllers/AsusAuraUSBController/RGBController_AsusAuraUSB.cpp                             \
    Controllers/AsusAuraUSBController/RGBController_ROGStrixLC_Controller.cpp                   \
    Controllers/AsusAuraUSBController/ROGStrixLC_Controller.cpp                                 \
    Controllers/AsusLegacyUSBController/AsusCerberusKeyboardController.cpp                      \
    Controllers/AsusLegacyUSBController/AsusLegacyUSBControllerDetect.cpp                       \
    Controllers/AsusLegacyUSBController/AsusStrixClawController.cpp                             \
    Controllers/AsusLegacyUSBController/RGBController_AsusCerberusKeyboard.cpp                  \
    Controllers/AsusLegacyUSBController/RGBController_AsusStrixClaw.cpp                         \
    Controllers/BlinkyTapeController/BlinkyTapeController.cpp                                   \
    Controllers/BlinkyTapeController/BlinkyTapeControllerDetect.cpp                             \
    Controllers/BlinkyTapeController/RGBController_BlinkyTape.cpp                               \
    Controllers/CherryKeyboardController/CherryKeyboardController.cpp                           \
    Controllers/CherryKeyboardController/CherryKeyboardControllerDetect.cpp                     \
    Controllers/CherryKeyboardController/RGBController_CherryKeyboard.cpp                       \
    Controllers/ColorfulGPUController/ColorfulGPUController.cpp                                 \
    Controllers/ColorfulGPUController/ColorfulGPUControllerDetect.cpp                           \
    Controllers/ColorfulGPUController/RGBController_ColorfulGPU.cpp                             \
    Controllers/ColorfulTuringGPUController/ColorfulTuringGPUController.cpp                     \
    Controllers/ColorfulTuringGPUController/ColorfulTuringGPUControllerDetect.cpp               \
    Controllers/ColorfulTuringGPUController/RGBController_ColorfulTuringGPU.cpp                 \
    Controllers/CoolerMasterController/CMARGBcontroller.cpp                                     \
    Controllers/CoolerMasterController/CMARGBGen2A1controller.cpp                               \
    Controllers/CoolerMasterController/CMMKController.cpp                                       \
    Controllers/CoolerMasterController/CMMMController.cpp                                       \
    Controllers/CoolerMasterController/CMMM711Controller.cpp                                    \
    Controllers/CoolerMasterController/CMMP750Controller.cpp                                    \
    Controllers/CoolerMasterController/CMR6000Controller.cpp                                    \
    Controllers/CoolerMasterController/CMRGBController.cpp                                      \
    Controllers/CoolerMasterController/CMSmallARGBController.cpp                                \
    Controllers/CoolerMasterController/CoolerMasterControllerDetect.cpp                         \
    Controllers/CoolerMasterController/RGBController_CMARGBController.cpp                       \
    Controllers/CoolerMasterController/RGBController_CMARGBGen2A1Controller.cpp                 \
    Controllers/CoolerMasterController/RGBController_CMMKController.cpp                         \
    Controllers/CoolerMasterController/RGBController_CMMMController.cpp                         \
    Controllers/CoolerMasterController/RGBController_CMMM711Controller.cpp                      \
    Controllers/CoolerMasterController/RGBController_CMMP750Controller.cpp                      \
    Controllers/CoolerMasterController/RGBController_CMR6000Controller.cpp                      \
    Controllers/CoolerMasterController/RGBController_CMRGBController.cpp                        \
    Controllers/CoolerMasterController/RGBController_CMSmallARGBController.cpp                  \
    Controllers/CorsairCommanderCoreController/CorsairCommanderCoreController.cpp               \
    Controllers/CorsairCommanderCoreController/CorsairCommanderCoreControllerDetect.cpp         \
    Controllers/CorsairCommanderCoreController/RGBController_CorsairCommanderCore.cpp           \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumController.cpp       \
    Controllers/CorsairDominatorPlatinumController/CorsairDominatorPlatinumControllerDetect.cpp \
    Controllers/CorsairDominatorPlatinumController/RGBController_CorsairDominatorPlatinum.cpp   \
    Controllers/CorsairHydroController/CorsairHydroController.cpp                               \
    Controllers/CorsairHydroController/CorsairHydroControllerDetect.cpp                         \
    Controllers/CorsairHydroController/RGBController_CorsairHydro.cpp                           \
    Controllers/CorsairHydro2Controller/CorsairHydro2Controller.cpp                             \
    Controllers/CorsairHydro2Controller/CorsairHydro2ControllerDetect.cpp                       \
    Controllers/CorsairHydro2Controller/RGBController_CorsairHydro2.cpp                         \
    Controllers/CorsairHydroPlatinumController/CorsairHydroPlatinumController.cpp               \
    Controllers/CorsairHydroPlatinumController/CorsairHydroPlatinumControllerDetect.cpp         \
    Controllers/CorsairHydroPlatinumController/RGBController_CorsairHydroPlatinum.cpp           \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.cpp                 \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeControllerDetect.cpp           \
    Controllers/CorsairLightingNodeController/RGBController_CorsairLightingNode.cpp             \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.cpp                     \
    Controllers/CorsairPeripheralController/CorsairPeripheralControllerDetect.cpp               \
    Controllers/CorsairPeripheralController/CorsairK100Controller.cpp                           \
    Controllers/CorsairPeripheralController/CorsairK55RGBPROXTController.cpp                    \
    Controllers/CorsairPeripheralController/CorsairK65MiniController.cpp                        \
    Controllers/CorsairPeripheralController/CorsairK95PlatinumXTController.cpp                  \
    Controllers/CorsairPeripheralController/RGBController_CorsairPeripheral.cpp                 \
    Controllers/CorsairPeripheralController/RGBController_CorsairK100.cpp                       \
    Controllers/CorsairPeripheralController/RGBController_CorsairK55RGBPROXT.cpp                \
    Controllers/CorsairPeripheralController/RGBController_CorsairK65Mini.cpp                    \
    Controllers/CorsairPeripheralController/RGBController_CorsairK95PlatinumXT.cpp              \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2Devices.cpp                    \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2Controller.cpp                 \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2ControllerDetect.cpp           \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2HardwareController.cpp         \
    Controllers/CorsairPeripheralV2Controller/CorsairPeripheralV2SoftwareController.cpp         \
    Controllers/CorsairPeripheralV2Controller/RGBController_CorsairV2Hardware.cpp               \
    Controllers/CorsairPeripheralV2Controller/RGBController_CorsairV2Software.cpp               \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.cpp                       \
    Controllers/CorsairVengeanceController/CorsairVengeanceControllerDetect.cpp                 \
    Controllers/CorsairVengeanceController/RGBController_CorsairVengeance.cpp                   \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.cpp                 \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProControllerDetect.cpp           \
    Controllers/CorsairVengeanceProController/RGBController_CorsairVengeancePro.cpp             \
    Controllers/CorsairWirelessController/CorsairWirelessController.cpp                         \
    Controllers/CorsairWirelessController/CorsairWirelessControllerDetect.cpp                   \
    Controllers/CorsairWirelessController/RGBController_CorsairWireless.cpp                     \
    Controllers/CougarController/CougarKeyboardController.cpp                                   \
    Controllers/CougarController/CougarRevengerSTController.cpp                                 \
    Controllers/CougarController/CougarControllerDetect.cpp                                     \
    Controllers/CougarController/RGBController_CougarKeyboard.cpp                               \
    Controllers/CougarController/RGBController_CougarRevengerST.cpp                             \
    Controllers/CreativeController/CreativeSoundBlasterXG6Controller.cpp                        \
    Controllers/CreativeController/CreativeControllerDetect.cpp                                 \
    Controllers/CreativeController/RGBController_CreativeSoundBlasterXG6.cpp                    \
    Controllers/CrucialController/CrucialController.cpp                                         \
    Controllers/CrucialController/CrucialControllerDetect.cpp                                   \
    Controllers/CrucialController/RGBController_Crucial.cpp                                     \
    Controllers/CryorigH7QuadLumiController/CryorigH7QuadLumiController.cpp                     \
    Controllers/CryorigH7QuadLumiController/CryorigH7QuadLumiControllerDetect.cpp               \
    Controllers/CryorigH7QuadLumiController/RGBController_CryorigH7QuadLumi.cpp                 \
    Controllers/DebugController/DebugControllerDetect.cpp                                       \
    Controllers/DarkProject/DarkProjectControllerDetect.cpp                                     \
    Controllers/DarkProject/DarkProjectKeyboardController.cpp                                   \
    Controllers/DarkProject/RGBController_DarkProjectKeyboard.cpp                               \
    Controllers/DasKeyboardController/DasKeyboardController.cpp                                 \
    Controllers/DasKeyboardController/DasKeyboardControllerDetect.cpp                           \
    Controllers/DasKeyboardController/RGBController_DasKeyboard.cpp                             \
    Controllers/DMXController/DMXControllerDetect.cpp                                           \
    Controllers/DMXController/RGBController_DMX.cpp                                             \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.cpp                             \
    Controllers/DuckyKeyboardController/DuckyKeyboardControllerDetect.cpp                       \
    Controllers/DuckyKeyboardController/RGBController_DuckyKeyboard.cpp                         \
    Controllers/DebugController/RGBController_Debug.cpp                                         \
    Controllers/DygmaRaiseController/DygmaRaiseController.cpp                                   \
    Controllers/DygmaRaiseController/DygmaRaiseControllerDetect.cpp                             \
    Controllers/DygmaRaiseController/RGBController_DygmaRaise.cpp                               \
    Controllers/E131Controller/E131ControllerDetect.cpp                                         \
    Controllers/E131Controller/RGBController_E131.cpp                                           \
    Controllers/EKController/EKControllerDetect.cpp                                             \
    Controllers/EKController/EKController.cpp                                                   \
    Controllers/EKController/RGBController_EKController.cpp                                     \
    Controllers/ElgatoKeyLightController/ElgatoKeyLightController.cpp                           \
    Controllers/ElgatoKeyLightController/ElgatoKeyLightControllerDetect.cpp                     \
    Controllers/ElgatoKeyLightController/RGBController_ElgatoKeyLight.cpp                       \
    Controllers/ENESMBusController/ENESMBusController.cpp                                       \
    Controllers/ENESMBusController/ENESMBusControllerDetect.cpp                                 \
    Controllers/ENESMBusController/RGBController_ENESMBus.cpp                                   \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_i2c_smbus.cpp            \
    Controllers/EpomakerController/EpomakerController.cpp                                       \
    Controllers/EpomakerController/EpomakerControllerDetect.cpp                                 \
    Controllers/EpomakerController/RGBController_EpomakerController.cpp                         \
    Controllers/EspurnaController/EspurnaController.cpp                                         \
    Controllers/EspurnaController/EspurnaControllerDetect.cpp                                   \
    Controllers/EspurnaController/RGBController_Espurna.cpp                                     \
    Controllers/EVGAGP102GPUController/EVGAGP102Controller.cpp                                  \
    Controllers/EVGAGP102GPUController/EVGAGP102GPUControllerDetect.cpp                         \
    Controllers/EVGAGP102GPUController/RGBController_EVGAGP102.cpp                              \
    Controllers/EVGAPascalGPUController/EVGAGPUv1Controller.cpp                                 \
    Controllers/EVGAPascalGPUController/EVGAPascalGPUControllerDetect.cpp                       \
    Controllers/EVGAPascalGPUController/RGBController_EVGAGPUv1.cpp                             \
    Controllers/EVGATuringGPUController/EVGAGPUv2Controller.cpp                                 \
    Controllers/EVGATuringGPUController/EVGATuringGPUControllerDetect.cpp                       \
    Controllers/EVGATuringGPUController/RGBController_EVGAGPUv2.cpp                             \
    Controllers/EVGAAmpereGPUController/EVGAGPUv3Controller.cpp                                 \
    Controllers/EVGAAmpereGPUController/EVGAAmpereGPUControllerDetect.cpp                       \
    Controllers/EVGAAmpereGPUController/RGBController_EVGAGPUv3.cpp                             \
    Controllers/EVGAUSBController/EVGAKeyboardController.cpp                                    \
    Controllers/EVGAUSBController/EVGAMouseController.cpp                                       \
    Controllers/EVGAUSBController/EVGAUSBControllerDetect.cpp                                   \
    Controllers/EVGAUSBController/RGBController_EVGAKeyboard.cpp                                \
    Controllers/EVGAUSBController/RGBController_EVGAMouse.cpp                                   \
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
    Controllers/GaiZongGaiKeyboardController/GaiZhongGaiKeyboardController.cpp                  \
    Controllers/GaiZongGaiKeyboardController/GaiZhongGaiKeyboardControllerDetect.cpp            \
    Controllers/GaiZongGaiKeyboardController/RGBController_GaiZhongGaiKeyboard.cpp              \
    Controllers/GalaxGPUController/GalaxGPUController.cpp                                       \
    Controllers/GalaxGPUController/GalaxGPUControllerDetect.cpp                                 \
    Controllers/GalaxGPUController/RGBController_GalaxGPU.cpp                                   \
    Controllers/GigabyteAorusCPUCoolerController/ATC800Controller.cpp                           \
    Controllers/GigabyteAorusCPUCoolerController/GigabyteAorusCPUCoolerControllerDetect.cpp     \
    Controllers/GigabyteAorusCPUCoolerController/RGBController_AorusATC800.cpp                  \
    Controllers/GigabyteAorusMouseController/GigabyteAorusMouseController.cpp                   \
    Controllers/GigabyteAorusMouseController/GigabyteAorusMouseControllerDetect.cpp             \
    Controllers/GigabyteAorusMouseController/RGBController_GigabyteAorusMouse.cpp               \
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
    Controllers/GigabyteRGBFusion2GPUController/GigabyteRGBFusion2GPUController.cpp             \
    Controllers/GigabyteRGBFusion2GPUController/GigabyteRGBFusion2GPUControllerDetect.cpp       \
    Controllers/GigabyteRGBFusion2GPUController/RGBController_GigabyteRGBFusion2GPU.cpp         \
    Controllers/GigabyteSuperIORGBController/GigabyteSuperIORGBController.cpp                   \
    Controllers/GigabyteSuperIORGBController/GigabyteSuperIORGBControllerDetect.cpp             \
    Controllers/GigabyteSuperIORGBController/RGBController_GigabyteSuperIORGB.cpp               \
    Controllers/HoltekController/HoltekA070Controller.cpp                                       \
    Controllers/HoltekController/HoltekA1FAController.cpp                                       \
    Controllers/HoltekController/HoltekControllerDetect.cpp                                     \
    Controllers/HoltekController/RGBController_HoltekA070.cpp                                   \
    Controllers/HoltekController/RGBController_HoltekA1FA.cpp                                   \
    Controllers/HPOmen30LController/HPOmen30LController.cpp                                     \
    Controllers/HPOmen30LController/HPOmen30LControllerDetect.cpp                               \
    Controllers/HPOmen30LController/RGBController_HPOmen30L.cpp                                 \
    Controllers/HyperXDRAMController/HyperXDRAMController.cpp                                   \
    Controllers/HyperXDRAMController/HyperXDRAMControllerDetect.cpp                             \
    Controllers/HyperXDRAMController/RGBController_HyperXDRAM.cpp                               \
    Controllers/HyperXKeyboardController/HyperXAlloyEliteController.cpp                         \
    Controllers/HyperXKeyboardController/HyperXAlloyElite2Controller.cpp                        \
    Controllers/HyperXKeyboardController/HyperXAlloyFPSController.cpp                           \
    Controllers/HyperXKeyboardController/HyperXAlloyOrigins60and65Controller.cpp                \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.cpp                       \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsCoreController.cpp                   \
    Controllers/HyperXKeyboardController/HyperXKeyboardControllerDetect.cpp                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyElite.cpp                     \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyElite2.cpp                    \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyFPS.cpp                       \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins.cpp                   \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOrigins60and65.cpp            \
    Controllers/HyperXKeyboardController/RGBController_HyperXAlloyOriginsCore.cpp               \
    Controllers/HyperXMicrophoneController/HyperXMicrophoneController.cpp                       \
    Controllers/HyperXMicrophoneController/HyperXMicrophoneControllerDetect.cpp                 \
    Controllers/HyperXMicrophoneController/RGBController_HyperXMicrophone.cpp                   \
    Controllers/HyperXMouseController/HyperXMouseControllerDetect.cpp                           \
    Controllers/HyperXMouseController/HyperXPulsefireFPSProController.cpp                       \
    Controllers/HyperXMouseController/HyperXPulsefireHasteController.cpp                        \
    Controllers/HyperXMouseController/HyperXPulsefireSurgeController.cpp                        \
    Controllers/HyperXMouseController/HyperXPulsefireDartController.cpp                         \
    Controllers/HyperXMouseController/HyperXPulsefireRaidController.cpp                         \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireFPSPro.cpp                   \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireHaste.cpp                    \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireSurge.cpp                    \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireDart.cpp                     \
    Controllers/HyperXMouseController/RGBController_HyperXPulsefireRaid.cpp                     \
    Controllers/HyperXMousematController/HyperXMousematController.cpp                           \
    Controllers/HyperXMousematController/HyperXMousematControllerDetect.cpp                     \
    Controllers/HyperXMousematController/RGBController_HyperXMousemat.cpp                       \
    Controllers/IntelArcA770LEController/IntelArcA770LEController.cpp                           \
    Controllers/IntelArcA770LEController/IntelArcA770LEControllerDetect.cpp                     \
    Controllers/IonicoController/IonicoController.cpp                                           \
    Controllers/IonicoController/IonicoControllerDetect.cpp                                     \
    Controllers/IonicoController/RGBController_Ionico.cpp                                       \
    Controllers/IntelArcA770LEController/RGBController_IntelArcA770LE.cpp                       \
    Controllers/KasaSmartController/KasaSmartController.cpp                                     \
    Controllers/KasaSmartController/KasaSmartControllerDetect.cpp                               \
    Controllers/KasaSmartController/RGBController_KasaSmart.cpp                                 \
    Controllers/KeychronKeyboardController/KeychronKeyboardController.cpp                       \
    Controllers/KeychronKeyboardController/KeychronKeyboardControllerDetect.cpp                 \
    Controllers/KeychronKeyboardController/RGBController_KeychronKeyboard.cpp                   \
    Controllers/LEDStripController/LEDStripController.cpp                                       \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp                                 \
    Controllers/LEDStripController/RGBController_LEDStrip.cpp                                   \
    Controllers/LegoDimensionsToypadBaseController/LegoDimensionsToypadBaseController.cpp       \
    Controllers/LegoDimensionsToypadBaseController/LegoDimensionsToypadBaseControllerDetect.cpp \
    Controllers/LegoDimensionsToypadBaseController/RGBController_LegoDimensionsToypadBase.cpp   \
    Controllers/LenovoControllers/LenovoUSBController.cpp                                       \
    Controllers/LenovoControllers/Lenovo4ZoneUSBController.cpp                                  \
    Controllers/LenovoControllers/LenovoUSBDetect.cpp                                           \
    Controllers/LenovoControllers/Lenovo4ZoneUSBDetect.cpp                                      \
    Controllers/LenovoControllers/RGBController_LenovoUSB.cpp                                   \
    Controllers/LenovoControllers/RGBController_Lenovo4ZoneUSB.cpp                              \
    Controllers/LenovoMotherboardController/LenovoMotherboardController.cpp                     \
    Controllers/LenovoMotherboardController/LenovoMotherboardControllerDetect.cpp               \
    Controllers/LenovoMotherboardController/RGBController_LenovoMotherboard.cpp                 \
    Controllers/LexipMouseController/LexipMouseController.cpp                                   \
    Controllers/LexipMouseController/LexipMouseControllerDetect.cpp                             \
    Controllers/LexipMouseController/RGBController_LexipMouse.cpp                               \
    Controllers/LIFXController/LIFXController.cpp                                               \
    Controllers/LIFXController/LIFXControllerDetect.cpp                                         \
    Controllers/LIFXController/RGBController_LIFX.cpp                                           \
    Controllers/LianLiController/LianLiControllerDetect.cpp                                     \
    Controllers/LianLiController/LianLiUniHub_AL10Controller.cpp                                \
    Controllers/LianLiController/LianLiUniHubController.cpp                                     \
    Controllers/LianLiController/RGBController_LianLiUniHub.cpp                                 \
    Controllers/LianLiController/RGBController_LianLiUniHub_AL10.cpp                            \
    Controllers/LianLiController/RGBController_StrimerLConnect.cpp                              \
    Controllers/LianLiController/StrimerLConnectController.cpp                                  \
    Controllers/LianLiController/LianLiUniHubALController.cpp                                   \
    Controllers/LianLiController/RGBController_LianLiUniHubAL.cpp                               \
    Controllers/LianLiController/LianLiUniHubSLV2Controller.cpp                                 \
    Controllers/LianLiController/RGBController_LianLiUniHubSLV2.cpp                             \
    Controllers/LogitechController/LogitechControllerDetect.cpp                                 \
    Controllers/LogitechController/LogitechProtocolCommon.cpp                                   \
    Controllers/LogitechController/LogitechG203LController.cpp                                  \
    Controllers/LogitechController/LogitechG213Controller.cpp                                   \
    Controllers/LogitechController/LogitechG560Controller.cpp                                   \
    Controllers/LogitechController/LogitechG933Controller.cpp                                   \
    Controllers/LogitechController/LogitechG810Controller.cpp                                   \
    Controllers/LogitechController/LogitechGProKeyboardController.cpp                           \
    Controllers/LogitechController/LogitechG910Controller.cpp                                   \
    Controllers/LogitechController/LogitechG815Controller.cpp                                   \
    Controllers/LogitechController/LogitechG915Controller.cpp                                   \
    Controllers/LogitechController/LogitechGLightsyncController.cpp                             \
    Controllers/LogitechController/LogitechLightspeedController.cpp                             \
    Controllers/LogitechController/LogitechX56Controller.cpp                                    \
    Controllers/LogitechController/RGBController_LogitechG203L.cpp                              \
    Controllers/LogitechController/RGBController_LogitechG213.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG560.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG933.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG810.cpp                               \
    Controllers/LogitechController/RGBController_LogitechGProKeyboard.cpp                       \
    Controllers/LogitechController/RGBController_LogitechG910.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG815.cpp                               \
    Controllers/LogitechController/RGBController_LogitechG915.cpp                               \
    Controllers/LogitechController/RGBController_LogitechGLightsync.cpp                         \
    Controllers/LogitechController/RGBController_LogitechGLightsync1zone.cpp                    \
    Controllers/LogitechController/RGBController_LogitechGPowerPlay.cpp                         \
    Controllers/LogitechController/RGBController_LogitechLightspeed.cpp                         \
    Controllers/LogitechController/RGBController_LogitechX56.cpp                                \
    Controllers/MountainKeyboardController/MountainKeyboardController.cpp                       \
    Controllers/MountainKeyboardController/MountainKeyboardControllerDetect.cpp                 \
    Controllers/MountainKeyboardController/RGBController_MountainKeyboard.cpp                   \
    Controllers/MSI3ZoneController/MSI3ZoneController.cpp                                       \
    Controllers/MSI3ZoneController/MSI3ZoneControllerDetect.cpp                                 \
    Controllers/MSI3ZoneController/RGBController_MSI3Zone.cpp                                   \
    Controllers/MSIGPUController/MSIGPUController.cpp                                           \
    Controllers/MSIGPUController/MSIGPUControllerDetect.cpp                                     \
    Controllers/MSIGPUController/MSIGPUv2Controller.cpp                                         \
    Controllers/MSIGPUController/MSIGPUv2ControllerDetect.cpp                                   \
    Controllers/MSIGPUController/RGBController_MSIGPU.cpp                                       \
    Controllers/MSIGPUController/RGBController_MSIGPUv2.cpp                                     \
    Controllers/MSIMysticLightController/MSIMysticLight64Controller.cpp                         \
    Controllers/MSIMysticLightController/MSIMysticLight162Controller.cpp                        \
    Controllers/MSIMysticLightController/MSIMysticLight112Controller.cpp                        \
    Controllers/MSIMysticLightController/MSIMysticLight185Controller.cpp                        \
    Controllers/MSIMysticLightController/MSIMysticLightControllerDetect.cpp                     \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight64.cpp                     \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight112.cpp                    \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight162.cpp                    \
    Controllers/MSIMysticLightController/RGBController_MSIMysticLight185.cpp                    \
    Controllers/MSIOptixController/MSIOptixController.cpp                                       \
    Controllers/MSIOptixController/MSIOptixControllerDetect.cpp                                 \
    Controllers/MSIOptixController/RGBController_MSIOptix.cpp                                   \
    Controllers/MSIRGBController/MSIRGBController.cpp                                           \
    Controllers/MSIRGBController/MSIRGBControllerDetect.cpp                                     \
    Controllers/MSIRGBController/RGBController_MSIRGB.cpp                                       \
    Controllers/MSIVigorController/RGBController_MSIVigorGK30.cpp                               \
    Controllers/MSIVigorController/MSIVigorControllerDetect.cpp                                 \
    Controllers/MSIVigorController/MSIVigorGK30Controller.cpp                                   \
    Controllers/NanoleafController/NanoleafController.cpp                                       \
    Controllers/NanoleafController/NanoleafControllerDetect.cpp                                 \
    Controllers/NanoleafController/RGBController_Nanoleaf.cpp                                   \
    Controllers/N5312AController/N5312AController.cpp                                           \
    Controllers/N5312AController/N5312AControllerDetect.cpp                                     \
    Controllers/N5312AController/RGBController_N5312A.cpp                                       \
    Controllers/NvidiaESAController/NvidiaESAController.cpp                                     \
    Controllers/NvidiaESAController/NvidiaESAControllerDetect.cpp                               \
    Controllers/NvidiaESAController/RGBController_NvidiaESA.cpp                                 \
    Controllers/NZXTHue1Controller/NZXTHue1Controller.cpp                                       \
    Controllers/NZXTHue1Controller/NZXTHue1ControllerDetect.cpp                                 \
    Controllers/NZXTHue1Controller/RGBController_NZXTHue1.cpp                                   \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.cpp                                       \
    Controllers/NZXTHue2Controller/NZXTHue2ControllerDetect.cpp                                 \
    Controllers/NZXTHue2Controller/RGBController_NZXTHue2.cpp                                   \
    Controllers/NZXTHuePlusController/NZXTHuePlusController.cpp                                 \
    Controllers/NZXTHuePlusController/NZXTHuePlusControllerDetect.cpp                           \
    Controllers/NZXTHuePlusController/RGBController_NZXTHuePlus.cpp                             \
    Controllers/NZXTKrakenController/NZXTKrakenController.cpp                                   \
    Controllers/NZXTKrakenController/NZXTKrakenControllerDetect.cpp                             \
    Controllers/NZXTKrakenController/RGBController_NZXTKraken.cpp                               \
    Controllers/PalitGPUController/PalitGPUController.cpp                                       \
    Controllers/PalitGPUController/PalitGPUControllerDetect.cpp                                 \
    Controllers/PalitGPUController/RGBController_PalitGPU.cpp                                   \
    Controllers/PatriotViperController/PatriotViperController.cpp                               \
    Controllers/PatriotViperController/PatriotViperControllerDetect.cpp                         \
    Controllers/PatriotViperController/RGBController_PatriotViper.cpp                           \
    Controllers/PatriotViperSteelController/PatriotViperSteelController.cpp                     \
    Controllers/PatriotViperSteelController/PatriotViperSteelControllerDetect.cpp               \
    Controllers/PatriotViperSteelController/RGBController_PatriotViperSteel.cpp                 \
    Controllers/PhilipsHueController/PhilipsHueController.cpp                                   \
    Controllers/PhilipsHueController/PhilipsHueControllerDetect.cpp                             \
    Controllers/PhilipsHueController/PhilipsHueEntertainmentController.cpp                      \
    Controllers/PhilipsHueController/RGBController_PhilipsHue.cpp                               \
    Controllers/PhilipsHueController/RGBController_PhilipsHueEntertainment.cpp                  \
    Controllers/PhilipsWizController/PhilipsWizController.cpp                                   \
    Controllers/PhilipsWizController/PhilipsWizControllerDetect.cpp                             \
    Controllers/PhilipsWizController/RGBController_PhilipsWiz.cpp                               \
    Controllers/PNYGPUController/PNYGPUController.cpp                                           \
    Controllers/PNYGPUController/PNYGPUControllerDetect.cpp                                     \
    Controllers/PNYGPUController/RGBController_PNYGPU.cpp                                       \
    Controllers/QMKOpenRGBController/QMKOpenRGBControllerDetect.cpp                             \
    Controllers/QMKOpenRGBController/QMKOpenRGBRev9Controller.cpp                               \
    Controllers/QMKOpenRGBController/QMKOpenRGBRevBController.cpp                               \
    Controllers/QMKOpenRGBController/QMKOpenRGBRevDController.cpp                               \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRev9.cpp                           \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRevB.cpp                           \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRevD.cpp                           \
    Controllers/QMKOpenRGBController/RGBController_QMKOpenRGBRevE.cpp                           \
    Controllers/RazerController/RazerDevices.cpp                                                \
    Controllers/RazerController/RazerController.cpp                                             \
    Controllers/RazerController/RazerKrakenController.cpp                                       \
    Controllers/RazerController/RazerControllerDetect.cpp                                       \
    Controllers/RazerController/RGBController_Razer.cpp                                         \
    Controllers/RazerController/RGBController_RazerAddressable.cpp                              \
    Controllers/RazerController/RGBController_RazerKraken.cpp                                   \
    Controllers/RedragonController/RGBController_RedragonMouse.cpp                              \
    Controllers/RedragonController/RedragonControllerDetect.cpp                                 \
    Controllers/RedragonController/RedragonMouseController.cpp                                  \
    Controllers/RedSquareKeyroxController/RedSquareKeyroxController.cpp                         \
    Controllers/RedSquareKeyroxController/RedSquareKeyroxControllerDetect.cpp                   \
    Controllers/RedSquareKeyroxController/RedSquareKeyroxTKLClassicController.cpp               \
    Controllers/RedSquareKeyroxController/RGBController_RedSquareKeyrox.cpp                     \
    Controllers/RedSquareKeyroxController/RGBController_RedSquareKeyroxTKLClassic.cpp           \
    Controllers/RoccatController/RGBController_RoccatBurst.cpp                                  \
    Controllers/RoccatController/RGBController_RoccatElo.cpp                                    \
    Controllers/RoccatController/RGBController_RoccatHordeAimo.cpp                              \
    Controllers/RoccatController/RGBController_RoccatKoneAimo.cpp                               \
    Controllers/RoccatController/RGBController_RoccatKova.cpp                                   \
    Controllers/RoccatController/RGBController_RoccatVulcanAimo.cpp                             \
    Controllers/RoccatController/RoccatBurstController.cpp                                      \
    Controllers/RoccatController/RoccatEloController.cpp                                        \
    Controllers/RoccatController/RoccatHordeAimoController.cpp                                  \
    Controllers/RoccatController/RoccatKoneAimoController.cpp                                   \
    Controllers/RoccatController/RoccatKovaController.cpp                                       \
    Controllers/RoccatController/RoccatVulcanAimoController.cpp                                 \
    Controllers/RoccatController/RoccatControllerDetect.cpp                                     \
    Controllers/SapphireGPUController/SapphireNitroGlowV1Controller.cpp                         \
    Controllers/SapphireGPUController/SapphireNitroGlowV3Controller.cpp                         \
    Controllers/SapphireGPUController/SapphireGPUControllerDetect.cpp                           \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV1.cpp                     \
    Controllers/SapphireGPUController/RGBController_SapphireNitroGlowV3.cpp                     \
    Controllers/SinowealthController/SinowealthController.cpp                                   \
    Controllers/SinowealthController/SinowealthController1007.cpp                               \
    Controllers/SinowealthController/SinowealthGMOWController.cpp                               \
    Controllers/SinowealthController/SinowealthKeyboardController.cpp                           \
    Controllers/SinowealthController/SinowealthKeyboard16Controller.cpp                         \
    Controllers/SinowealthController/SinowealthKeyboard90Controller.cpp                         \
    Controllers/SinowealthController/SinowealthControllerDetect.cpp                             \
    Controllers/SinowealthController/RGBController_Sinowealth.cpp                               \
    Controllers/SinowealthController/RGBController_Sinowealth1007.cpp                           \
    Controllers/SinowealthController/RGBController_SinowealthGMOW.cpp                           \
    Controllers/SinowealthController/RGBController_SinowealthKeyboard.cpp                       \
    Controllers/SinowealthController/RGBController_SinowealthKeyboard16.cpp                     \
    Controllers/SinowealthController/RGBController_SinowealthKeyboard90.cpp                     \
    Controllers/SonyGamepadController/SonyDualSenseController.cpp                               \
    Controllers/SonyGamepadController/RGBController_SonyDualSense.cpp                           \
    Controllers/SonyGamepadController/SonyDS4Controller.cpp                                     \
    Controllers/SonyGamepadController/RGBController_SonyDS4.cpp                                 \
    Controllers/SonyGamepadController/SonyGamepadControllerDetect.cpp                           \
    Controllers/SRGBmodsController/SRGBmodsPicoController.cpp                                   \
    Controllers/SRGBmodsController/SRGBmodsControllerDetect.cpp                                 \
    Controllers/SRGBmodsController/RGBController_SRGBmodsPico.cpp                               \
    Controllers/SteelSeriesController/SteelSeriesAerox3Controller.cpp                           \
    Controllers/SteelSeriesController/SteelSeriesAerox9Controller.cpp                           \
    Controllers/SteelSeriesController/SteelSeriesApexController.cpp                             \
    Controllers/SteelSeriesController/SteelSeriesApex3Controller.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesApex8ZoneController.cpp                        \
    Controllers/SteelSeriesController/SteelSeriesApexMController.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesApexTZoneController.cpp                        \
    Controllers/SteelSeriesController/SteelSeriesArctis5Controller.cpp                          \
    Controllers/SteelSeriesController/SteelSeriesControllerDetect.cpp                           \
    Controllers/SteelSeriesController/SteelSeriesMouseController.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesOldApexController.cpp                          \
    Controllers/SteelSeriesController/SteelSeriesRivalController.cpp                            \
    Controllers/SteelSeriesController/SteelSeriesRival3Controller.cpp                           \
    Controllers/SteelSeriesController/SteelSeriesSenseiController.cpp                           \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.cpp                          \
    Controllers/SteelSeriesController/SteelSeriesQCKMatController.cpp                           \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex.cpp                         \
    Controllers/SteelSeriesController/RGBController_SteelSeriesApex3.cpp                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesArctis5.cpp                      \
    Controllers/SteelSeriesController/RGBController_SteelSeriesOldApex.cpp                      \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival.cpp                        \
    Controllers/SteelSeriesController/RGBController_SteelSeriesRival3.cpp                       \
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
    Controllers/ThingMController/ThingMControllerDetect.cpp                                     \
    Controllers/ThingMController/BlinkController.cpp                                            \
    Controllers/ThingMController/RGBController_BlinkController.cpp                              \
    Controllers/TrustController/TrustGXT114Controller.cpp                                       \
    Controllers/TrustController/TrustGXT180Controller.cpp                                       \
    Controllers/TrustController/TrustControllerDetect.cpp                                       \
    Controllers/TrustController/RGBController_TrustGXT114.cpp                                   \
    Controllers/TrustController/RGBController_TrustGXT180.cpp                                   \
    Controllers/ViewSonicController/ViewSonicControllerDetect.cpp                               \
    Controllers/ViewSonicController/RGBController_XG270QG.cpp                                   \
    Controllers/ViewSonicController/VS_XG270QG_Controller.cpp                                   \
    Controllers/WootingKeyboardController/WootingKeyboardController.cpp                         \
    Controllers/WootingKeyboardController/WootingKeyboardControllerDetect.cpp                   \
    Controllers/WootingKeyboardController/WootingOneKeyboardController.cpp                      \
    Controllers/WootingKeyboardController/WootingTwoKeyboardController.cpp                      \
    Controllers/WootingKeyboardController/RGBController_WootingKeyboard.cpp                     \
    Controllers/WushiController/WushiL50USBController.cpp                                       \
    Controllers/WushiController/WushiL50USBDetect.cpp                                           \
    Controllers/WushiController/RGBController_WushiL50USB.cpp                                   \
    Controllers/ThermaltakeRiingController/RGBController_ThermaltakeRiingQuad.cpp               \
    Controllers/YeelightController/YeelightController.cpp                                       \
    Controllers/YeelightController/YeelightControllerDetect.cpp                                 \
    Controllers/YeelightController/RGBController_Yeelight.cpp                                   \
    Controllers/ZalmanZSyncController/ZalmanZSyncController.cpp                                 \
    Controllers/ZalmanZSyncController/ZalmanZSyncControllerDetect.cpp                           \
    Controllers/ZalmanZSyncController/RGBController_ZalmanZSync.cpp                             \
    Controllers/ZETEdgeAirProController/ZETEdgeAirProController.cpp                             \
    Controllers/ZETEdgeAirProController/ZETEdgeAirProControllerDetect.cpp                       \
    Controllers/ZETEdgeAirProController/RGBController_ZETEdgeAirPro.cpp                         \
    Controllers/ZETKeyboardController/ZETBladeOpticalController.cpp                             \
    Controllers/ZETKeyboardController/ZETKeyboardControllerDetect.cpp                           \
    Controllers/ZETKeyboardController/RGBController_ZETBladeOptical.cpp                         \
    Controllers/ZotacTuringGPUController/ZotacTuringGPUController.cpp                           \
    Controllers/ZotacTuringGPUController/ZotacTuringGPUControllerDetect.cpp                     \
    Controllers/ZotacTuringGPUController/RGBController_ZotacTuringGPU.cpp                       \
    KeyboardLayoutManager/KeyboardLayoutManager.cpp                                             \
    RGBController/RGBController.cpp                                                             \
    RGBController/RGBController_Dummy.cpp                                                       \
    RGBController/RGBControllerKeyNames.cpp                                                     \
    RGBController/RGBController_Network.cpp                                                     \

contains(QMAKE_PLATFORM, freebsd) {
    SOURCES -=                                                                                  \
        Controllers/GigabyteRGBFusion2GPUController/GigabyteRGBFusion2GPUController.cpp         \
        Controllers/GigabyteRGBFusion2GPUController/GigabyteRGBFusion2GPUControllerDetect.cpp   \
        Controllers/GigabyteRGBFusion2GPUController/RGBController_GigabyteRGBFusion2GPU.cpp     \
        Controllers/HoltekController/HoltekA070Controller.cpp                                   \
        Controllers/HoltekController/HoltekA1FAController.cpp                                   \
        Controllers/HoltekController/HoltekControllerDetect.cpp                                 \
        Controllers/HoltekController/RGBController_HoltekA070.cpp                               \
        Controllers/HoltekController/RGBController_HoltekA1FA.cpp
}

RESOURCES +=                                                                                    \
    qt/resources.qrc                                                                            \

TRANSLATIONS +=                                                                                 \
    qt/i18n/OpenRGB_de.ts                                                                       \
    qt/i18n/OpenRGB_en.ts                                                                       \
    qt/i18n/OpenRGB_en_AU.ts                                                                    \
    qt/i18n/OpenRGB_en_GB.ts                                                                    \
    qt/i18n/OpenRGB_es.ts                                                                       \
    qt/i18n/OpenRGB_fr.ts                                                                       \
    qt/i18n/OpenRGB_ms_MY.ts                                                                    \
    qt/i18n/OpenRGB_pl.ts                                                                       \
    qt/i18n/OpenRGB_pt_BR.ts                                                                    \
    qt/i18n/OpenRGB_ru.ts                                                                       \
    qt/i18n/OpenRGB_zh.ts                                                                       \
    qt/i18n/OpenRGB_zh_TW.ts                                                                    \

FORMS +=                                                                                        \
    qt/OpenRGBClientInfoPage.ui                                                                 \
    qt/OpenRGBConsolePage.ui                                                                    \
    qt/OpenRGBDeviceInfoPage.ui                                                                 \
    qt/OpenRGBDevicePage.ui                                                                     \
    qt/OpenRGBDialog.ui                                                                         \
    qt/OpenRGBDialog2.ui                                                                        \
    qt/OpenRGBHardwareIDsDialog.ui                                                              \
    qt/OpenRGBPluginContainer.ui                                                                \
    qt/OpenRGBProfileSaveDialog.ui                                                              \
    qt/OpenRGBServerInfoPage.ui                                                                 \
    qt/OpenRGBSettingsPage.ui                                                                   \
    qt/OpenRGBSoftwareInfoPage.ui                                                               \
    qt/OpenRGBSupportedDevicesPage.ui                                                           \
    qt/OpenRGBSystemInfoPage.ui                                                                 \
    qt/OpenRGBZoneResizeDialog.ui                                                               \
    qt/OpenRGBDMXSettingsPage/OpenRGBDMXSettingsEntry.ui                                        \
    qt/OpenRGBDMXSettingsPage/OpenRGBDMXSettingsPage.ui                                         \
    qt/OpenRGBE131SettingsPage/OpenRGBE131SettingsEntry.ui                                      \
    qt/OpenRGBE131SettingsPage/OpenRGBE131SettingsPage.ui                                       \
    qt/OpenRGBElgatoKeyLightSettingsPage/OpenRGBElgatoKeyLightSettingsEntry.ui                  \
    qt/OpenRGBElgatoKeyLightSettingsPage/OpenRGBElgatoKeyLightSettingsPage.ui                   \
    qt/OpenRGBKasaSmartSettingsPage/OpenRGBKasaSmartSettingsEntry.ui                            \
    qt/OpenRGBKasaSmartSettingsPage/OpenRGBKasaSmartSettingsPage.ui                             \
    qt/OpenRGBLIFXSettingsPage/OpenRGBLIFXSettingsEntry.ui                                      \
    qt/OpenRGBLIFXSettingsPage/OpenRGBLIFXSettingsPage.ui                                       \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafSettingsPage.ui                               \
    qt/OpenRGBNanoleafSettingsPage/OpenRGBNanoleafSettingsEntry.ui                              \
    qt/OpenRGBPhilipsHueSettingsPage/OpenRGBPhilipsHueSettingsEntry.ui                          \
    qt/OpenRGBPhilipsHueSettingsPage/OpenRGBPhilipsHueSettingsPage.ui                           \
    qt/OpenRGBPhilipsWizSettingsPage/OpenRGBPhilipsWizSettingsEntry.ui                          \
    qt/OpenRGBPhilipsWizSettingsPage/OpenRGBPhilipsWizSettingsPage.ui                           \
    qt/OpenRGBPluginsPage/OpenRGBPluginsEntry.ui                                                \
    qt/OpenRGBPluginsPage/OpenRGBPluginsPage.ui                                                 \
    qt/OpenRGBQMKORGBSettingsPage/OpenRGBQMKORGBSettingsEntry.ui                                \
    qt/OpenRGBQMKORGBSettingsPage/OpenRGBQMKORGBSettingsPage.ui                                 \
    qt/OpenRGBSerialSettingsPage/OpenRGBSerialSettingsEntry.ui                                  \
    qt/OpenRGBSerialSettingsPage/OpenRGBSerialSettingsPage.ui                                   \
    qt/OpenRGBYeelightSettingsPage/OpenRGBYeelightSettingsEntry.ui                              \
    qt/OpenRGBYeelightSettingsPage/OpenRGBYeelightSettingsPage.ui                               \
    qt/OpenRGBZonesBulkResizer.ui                                                               \
    qt/TabLabel.ui                                                                              \

#-----------------------------------------------------------------------------------------------#
# Windows-specific Configuration                                                                #
#-----------------------------------------------------------------------------------------------#
win32:INCLUDEPATH +=                                                                            \
    dependencies/display-library/include                                                        \
    dependencies/hidapi                                                                         \
    dependencies/winring0/include                                                               \
    dependencies/libusb-1.0.22/include                                                          \
    dependencies/mbedtls-2.24.0/include                                                         \
    dependencies/NVFC                                                                           \
    dependencies/openrazer-win32                                                                \
    wmi/                                                                                        \
    Controllers/AsusTUFLaptopController                                                         \
    Controllers/NVIDIAIlluminationController                                                    \

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
    AutoStart/AutoStart-Windows.cpp                                                             \
    Controllers/AsusTUFLaptopController/AsusTUFLaptopController.cpp                             \
    Controllers/AsusTUFLaptopController/AsusTUFLaptopWMIDetect.cpp                              \
    Controllers/AsusTUFLaptopController/RGBController_AsusTUFLaptopWMI.cpp                      \
    Controllers/ENESMBusController/XPGSpectrixS40GDetect_Windows.cpp                            \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_SpectrixS40G_Windows.cpp \
    Controllers/NVIDIAIlluminationController/nvapi_accessor.cpp                                 \
    Controllers/NVIDIAIlluminationController/NVIDIAIlluminationV1Controller.cpp                 \
    Controllers/NVIDIAIlluminationController/NVIDIAIlluminationControllerDetect.cpp             \
    Controllers/NVIDIAIlluminationController/RGBController_NVIDIAIllumination.cpp               \
    Controllers/OpenRazerController/OpenRazerWindowsDetect.cpp                                  \
    Controllers/OpenRazerController/RGBController_OpenRazerWindows.cpp                          \

win32:HEADERS +=                                                                                \
    dependencies/display-library/include/adl_defines.h                                          \
    dependencies/display-library/include/adl_sdk.h                                              \
    dependencies/display-library/include/adl_structures.h                                       \
    dependencies/winring0/include/OlsApi.h                                                      \
    dependencies/NVFC/nvapi.h                                                                   \
    i2c_smbus/i2c_smbus_i801.h                                                                  \
    i2c_smbus/i2c_smbus_nct6775.h                                                               \
    i2c_smbus/i2c_smbus_nvapi.h                                                                 \
    i2c_smbus/i2c_smbus_piix4.h                                                                 \
    wmi/wmi.h                                                                                   \
    wmi/acpiwmi.h                                                                               \
    AutoStart/AutoStart-Windows.h                                                               \
    Controllers/AsusTUFLaptopController/AsusTUFLaptopController.h                               \
    Controllers/AsusTUFLaptopController/RGBController_AsusTUFLaptopWMI.h                        \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_SpectrixS40G_Windows.h   \
    Controllers/NVIDIAIlluminationController/nvapi_accessor.h                                   \
    Controllers/NVIDIAIlluminationController/NVIDIAIlluminationV1Controller.h                   \
    Controllers/NVIDIAIlluminationController/RGBController_NVIDIAIllumination.h                 \
    Controllers/OpenRazerController/RGBController_OpenRazerWindows.h                            \

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -liphlpapi                                                                              \
        -L"$$PWD/dependencies/winring0/x64/" -lWinRing0x64                                      \
        -L"$$PWD/dependencies/libusb-1.0.22/MS64/dll" -llibusb-1.0                              \
        -L"$$PWD/dependencies/hidapi-win/x64/" -lhidapi                                         \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -liphlpapi                                                                              \
        -L"$$PWD/dependencies/winring0/Win32/" -lWinRing0                                       \
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
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/x64/WinRing0x64.dll         )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/x64/WinRing0x64.sys         )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/libusb-1.0.22/MS64/dll/libusb-1.0.dll)\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/hidapi-win/x64/hidapi.dll            )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

win32:contains(QMAKE_TARGET.arch, x86) {
    copydata.commands  = $(COPY_FILE) \"$$shell_path($$PWD/dependencies/openrazer-win32/OpenRazer.dll        )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/Win32/WinRing0.dll          )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/Win32/WinRing0.sys          )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/x64/WinRing0x64.sys         )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
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
contains(QMAKE_PLATFORM, linux) {
    TARGET = $$lower($$TARGET)

    INCLUDEPATH +=                                                                              \
    Controllers/FaustusController                                                               \
    Controllers/LinuxLEDController                                                              \

    HEADERS +=                                                                                  \
    i2c_smbus/i2c_smbus_linux.h                                                                 \
    AutoStart/AutoStart-Linux.h                                                                 \
    Controllers/AsusTUFLaptopLinuxController/AsusTUFLaptopLinuxController.h                     \
    Controllers/AsusTUFLaptopLinuxController/RGBController_AsusTUFLaptopLinux.h                 \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_SpectrixS40G.h           \
    Controllers/FaustusController/RGBController_Faustus.h                                       \
    Controllers/LinuxLEDController/LinuxLEDController.h                                         \
    Controllers/LinuxLEDController/RGBController_LinuxLED.h                                     \
    Controllers/OpenRazerController/RGBController_OpenRazer.h                                   \

    LIBS +=                                                                                     \
    -lusb-1.0                                                                                   \
    -lmbedx509                                                                                  \
    -lmbedtls                                                                                   \
    -lmbedcrypto                                                                                \
    -ldl                                                                                        \

    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    if (!versionAtLeast(COMPILER_VERSION, "9")) {
         LIBS += -lstdc++fs
    }

    QMAKE_CXXFLAGS += -Wno-implicit-fallthrough

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
        HIDAPI_HIDRAW_VERSION = $$system($$PKG_CONFIG --modversion hidapi-hidraw)
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
    AutoStart/AutoStart-Linux.cpp                                                               \
    Controllers/AsusTUFLaptopLinuxController/AsusTUFLaptopLinuxController.cpp                   \
    Controllers/AsusTUFLaptopLinuxController/AsusTUFLaptopLinuxDetect.cpp                       \
    Controllers/AsusTUFLaptopLinuxController/RGBController_AsusTUFLaptopLinux.cpp               \
    Controllers/ENESMBusController/XPGSpectrixS40GDetect.cpp                                    \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_SpectrixS40G.cpp         \
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

    #-------------------------------------------------------------------------------------------#
    # Custom target for dynamically created udev_rules                                          #
    #   Ordinarily you would add the 'udev_rules' target to both QMAKE_EXTRA_TARGETS to add a   #
    #   rule in the Makefile and PRE_TARGETDEPS to ensure it is a dependency of the TARGET      #
    #                                                                                           #
    #   ie. QMAKE_EXTRA_TARGETS += udev_rules                                                   #
    #       PRE_TARGETDEPS      += udev_rules                                                   #
    #-------------------------------------------------------------------------------------------#
    CONFIG(release, debug|release) {
        udev_rules.CONFIG       = no_check_exist
        udev_rules.target       = 60-openrgb.rules
        udev_rules.path         = $$PREFIX/lib/udev/rules.d/

        exists($$udev_rules.target) {
            message($$udev_rules.target " - UDEV rules file exists. Removing from build")
            udev_rules.files    = $$udev_rules.target
        } else {
            message($$udev_rules.target " - UDEV rules file missing. Adding script to build")
            #-------------------------------------------------------------------------------------------#
            # This is a compiler config flag to save the preproccessed .ii & .s                         #
            #   files so as to automatically process the UDEV rules and the Supported Devices           #
            #-------------------------------------------------------------------------------------------#
            QMAKE_CXXFLAGS+=-save-temps
            QMAKE_CXXFLAGS-=-pipe
            udev_rules.extra    = $$PWD/scripts/build-udev-rules.sh $$PWD $$GIT_COMMIT_ID
            udev_rules.files    = $$OUT_PWD/60-openrgb.rules
        }
    }

    #-------------------------------------------------------------------------------------------#
    # Add static files to installation                                                                      #
    #-------------------------------------------------------------------------------------------#
    target.path=$$PREFIX/bin/
    desktop.path=$$PREFIX/share/applications/
    desktop.files+=qt/OpenRGB.desktop
    icon.path=$$PREFIX/share/icons/hicolor/128x128/apps/
    icon.files+=qt/OpenRGB.png
    metainfo.path=$$PREFIX/share/metainfo/
    metainfo.files+=qt/org.openrgb.OpenRGB.metainfo.xml
    INSTALLS += target desktop icon metainfo udev_rules
}

#-----------------------------------------------------------------------------------------------#
# FreeBSD-specific Configuration                                                                #
#-----------------------------------------------------------------------------------------------#
contains(QMAKE_PLATFORM, freebsd) {
    TARGET = $$lower($$TARGET)

    INCLUDEPATH +=                                                                              \
    Controllers/FaustusController                                                               \
    Controllers/LinuxLEDController                                                              \

    HEADERS +=                                                                                  \
    AutoStart/AutoStart-FreeBSD.h                                                               \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_SpectrixS40G.h           \
    Controllers/FaustusController/RGBController_Faustus.h                                       \
    Controllers/LinuxLEDController/LinuxLEDController.h                                         \
    Controllers/LinuxLEDController/RGBController_LinuxLED.h                                     \
    Controllers/OpenRazerController/RGBController_OpenRazer.h                                   \

    LIBS +=                                                                                     \
    -lusb                                                                                       \
    -lmbedx509                                                                                  \
    -lmbedtls                                                                                   \
    -lmbedcrypto                                                                                \

    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    if (!versionAtLeast(COMPILER_VERSION, "9")) {
         LIBS += -lstdc++fs
    }

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
        HIDAPI_HIDRAW_VERSION = $$system($$PKG_CONFIG --modversion hidapi-hidraw)
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
    serial_port/find_usb_serial_port_linux.cpp                                                  \
    AutoStart/AutoStart-FreeBSD.cpp                                                             \
    Controllers/ENESMBusController/XPGSpectrixS40GDetect.cpp                                    \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_SpectrixS40G.cpp         \
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
    icon.path=$$PREFIX/share/icons/hicolor/128x128/apps/
    icon.files+=qt/OpenRGB.png
    metainfo.path=$$PREFIX/share/metainfo/
    metainfo.files+=qt/org.openrgb.OpenRGB.metainfo.xml
    rules.path=$$PREFIX/lib/udev/rules.d/
    rules.files+=60-openrgb.rules
    INSTALLS += target desktop icon metainfo rules
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

    QMAKE_CXXFLAGS +=                                                                           \
    -Wno-narrowing                                                                              \

    HEADERS +=                                                                                  \
    AutoStart/AutoStart-MacOS.h                                                                 \
    qt/macutils.h                                                                               \

    SOURCES +=                                                                                  \
    dependencies/hueplusplus-1.0.0/src/LinHttpHandler.cpp                                       \
    serial_port/find_usb_serial_port_linux.cpp                                                  \
    AutoStart/AutoStart-MacOS.cpp                                                               \
    qt/macutils.mm                                                                              \

    # Use mbedtls v2 instead of latest
    MBEDTLS_PREFIX = $$system(brew --prefix mbedtls@2)

    INCLUDEPATH +=                                                                              \
    $$MBEDTLS_PREFIX/include                                                                    \

    LIBS +=                                                                                     \
    -lusb-1.0                                                                                   \
    -lhidapi                                                                                    \
    -lmbedx509                                                                                  \
    -lmbedcrypto                                                                                \
    -lmbedtls                                                                                   \
    -L$$MBEDTLS_PREFIX/lib                                                                      \
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
    dependencies/macUSPCIO                                                                      \
    /usr/local/include                                                                          \
    /usr/local/homebrew/include                                                                 \

    SOURCES +=                                                                                  \
    i2c_smbus/i2c_smbus_i801.cpp                                                                \

    HEADERS +=                                                                                  \
    dependencies/macUSPCIO/macUSPCIOAccess.h                                                    \
    i2c_smbus/i2c_smbus_i801.h                                                                  \

    LIBS +=                                                                                     \
    -L/usr/local/lib                                                                            \
    -L/usr/local/homebrew/lib                                                                   \

    DEFINES +=                                                                                  \
    _MACOSX_X86_X64                                                                             \
}

DISTFILES += \
    debian/openrgb-udev.postinst \
    debian/openrgb.postinst

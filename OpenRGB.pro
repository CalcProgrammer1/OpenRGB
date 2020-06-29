#-----------------------------------------------------------------------#
# OpenRGB 0.x QMake Project                                             #
#                                                                       #
#   Adam Honse (CalcProgrammer1)                        5/25/2020       #
#-----------------------------------------------------------------------#

#-----------------------------------------------------------------------#
# Qt Configuration                                                      #
#-----------------------------------------------------------------------#
QT +=                                                                   \
    core                                                                \
    gui                                                                 \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-----------------------------------------------------------------------#
# Application Configuration                                             #
#-----------------------------------------------------------------------#
VERSION     = 0.21
TARGET      = OpenRGB
TEMPLATE    = app

#-----------------------------------------------------------------------#
# Automatically generated build information                             #
#-----------------------------------------------------------------------#
win32:BUILDDATE = $$system(date /t)
unix:BUILDDATE  = $$system(date -R)
GIT_COMMIT_ID   = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse HEAD)
GIT_COMMIT_DATE = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ show -s --format=%ci HEAD)
GIT_BRANCH      = $$system(git --git-dir $$_PRO_FILE_PWD_/.git --work-tree $$_PRO_FILE_PWD_ rev-parse --abbrev-ref HEAD)

DEFINES +=                                                              \
    VERSION_STRING=\\"\"\"$$VERSION\\"\"\"                              \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                          \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                         \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                     \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"

#-----------------------------------------------------------------------#
# OpenRGB Common                                                        #
#-----------------------------------------------------------------------#
INCLUDEPATH +=                                                          \
    dependencies/ColorWheel                                             \
    dependencies/libe131/src/                                           \
    i2c_smbus/                                                          \
    i2c_tools/                                                          \
    net_port/                                                           \
    serial_port/                                                        \
    super_io/                                                           \
    Controllers/AMDWraithPrismController/                               \
    Controllers/AuraUSBController/                                      \
    Controllers/AuraCoreController/                                     \
    Controllers/AuraGPUController/                                      \
    Controllers/AuraSMBusController/                                    \
    Controllers/CoolerMasterController/                                 \
    Controllers/CorsairPeripheralController/                            \
    Controllers/CorsairLightingNodeController/                          \
    Controllers/CorsairVengeanceController/                             \
    Controllers/CorsairVengeanceProController/                          \
    Controllers/CrucialController/                                      \
    Controllers/DuckyKeyboardController/                                \
    Controllers/GloriousModelOController/                               \
    Controllers/HuePlusController/                                      \
    Controllers/HyperXDRAMController/                                   \
    Controllers/HyperXKeyboardController/                               \
    Controllers/LEDStripController/                                     \
    Controllers/LogitechController/                                     \
    Controllers/MSI3ZoneController/                                     \
    Controllers/MSIGPUController/                                       \
    Controllers/MSIMysticLightController/                               \
    Controllers/MSIRGBController/                                       \
    Controllers/NZXTHue2Controller/                                     \
    Controllers/NZXTKrakenController/                                   \
    Controllers/PatriotViperController/                                 \
    Controllers/PolychromeController/                                   \
    Controllers/PoseidonZRGBController/                                 \
    Controllers/RedragonController/                                     \
    Controllers/RGBFusionController/                                    \
    Controllers/RGBFusion2DRAMController/                               \
    Controllers/RGBFusion2SMBusController/                              \
    Controllers/RGBFusion2USBController/                                \
    Controllers/RGBFusionGPUController/                                 \
    Controllers/SteelSeriesController/                                  \
    Controllers/ThermaltakeRiingController/                             \
    RGBController/                                                      \
    qt/

HEADERS +=                                                              \
    dependencies/ColorWheel/ColorWheel.h                                \
    NetworkClient.h                                                     \
    NetworkProtocol.h                                                   \
    NetworkServer.h                                                     \
    ProfileManager.h                                                    \
    qt/OpenRGBClientInfoPage.h                                          \
    qt/OpenRGBDeviceInfoPage.h                                          \
    qt/OpenRGBDevicePage.h                                              \
    qt/OpenRGBDialog.h                                                  \
    i2c_smbus/i2c_smbus.h                                               \
    i2c_tools/i2c_tools.h                                               \
    net_port/net_port.h                                                 \
    qt/OpenRGBDialog2.h                                                 \
    qt/OpenRGBProfileSaveDialog.h                                       \
    qt/OpenRGBServerInfoPage.h                                          \
    qt/OpenRGBSoftwareInfoPage.h                                        \
    qt/OpenRGBSystemInfoPage.h                                          \
    qt/OpenRGBZoneResizeDialog.h                                        \
    serial_port/find_usb_serial_port.h                                  \
    serial_port/serial_port.h                                           \
    super_io/super_io.h                                                 \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.h     \
    Controllers/AuraUSBController/AuraUSBController.h                   \
    Controllers/AuraUSBController/AuraAddressableController.h           \
    Controllers/AuraUSBController/AuraMainboardController.h             \
    Controllers/AuraCoreController/AuraCoreController.h                 \
    Controllers/AuraGPUController/AuraGPUController.h                   \
    Controllers/AuraSMBusController/AuraSMBusController.h               \
    Controllers/CoolerMasterController/CMMP750Controller.h              \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.h \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.h \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.h \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.h \
    Controllers/CrucialController/CrucialController.h                   \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.h       \
    Controllers/GloriousModelOController/GloriousModelOController.h     \
    Controllers/HuePlusController/HuePlusController.h                   \
    Controllers/HyperXDRAMController/HyperXDRAMController.h             \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.h \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.h     \
    Controllers/LEDStripController/LEDStripController.h                 \
    Controllers/LogitechController/LogitechG203Controller.h             \
    Controllers/LogitechController/LogitechG403Controller.h             \
    Controllers/LogitechController/LogitechG810Controller.h             \
    Controllers/MSI3ZoneController/MSI3ZoneController.h                 \
    Controllers/MSIGPUController/MSIGPUController.h                     \
    Controllers/MSIMysticLightController/MSIMysticLightController.h     \
    Controllers/MSIRGBController/MSIRGBController.h                     \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.h                 \
    Controllers/NZXTKrakenController/NZXTKrakenController.h             \
    Controllers/PatriotViperController/PatriotViperController.h         \
    Controllers/PolychromeController/PolychromeController.h             \
    Controllers/PoseidonZRGBController/PoseidonZRGBController.h         \
    Controllers/RGBFusionController/RGBFusionController.h               \
    Controllers/RGBFusion2DRAMController/RGBFusion2DRAMController.h     \
    Controllers/RGBFusion2SMBusController/RGBFusion2SMBusController.h   \
    Controllers/RGBFusion2USBController/RGBFusion2USBController.h       \
    Controllers/RGBFusionGPUController/RGBFusionGPUController.h         \
    Controllers/RedragonController/RedragonK556Controller.h             \
    Controllers/RedragonController/RedragonM711Controller.h             \
    Controllers/SteelSeriesController/SteelSeriesRivalController.h      \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.h    \
    Controllers/SteelSeriesController/SteelSeriesApexController.h       \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.h \
    RGBController/RGBController.h                                       \
    RGBController/RGBController_AMDWraithPrism.h                        \
    RGBController/RGBController_AuraUSB.h                               \
    RGBController/RGBController_AuraCore.h                              \
    RGBController/RGBController_AuraGPU.h                               \
    RGBController/RGBController_AuraSMBus.h                             \
    RGBController/RGBController_CMMP750Controller.h                     \
    RGBController/RGBController_CorsairLightingNode.h                   \
    RGBController/RGBController_CorsairPeripheral.h                     \
    RGBController/RGBController_CorsairVengeance.h                      \
    RGBController/RGBController_CorsairVengeancePro.h                   \
    RGBController/RGBController_Crucial.h                               \
    RGBController/RGBController_DuckyKeyboard.h                         \
    RGBController/RGBController_Dummy.h                                 \
    RGBController/RGBController_E131.h                                  \
    RGBController/RGBController_GloriousModelO.h                        \
    RGBController/RGBController_HuePlus.h                               \
    RGBController/RGBController_HyperXAlloyOrigins.h                    \
    RGBController/RGBController_HyperXDRAM.h                            \
    RGBController/RGBController_HyperXKeyboard.h                        \
    RGBController/RGBController_LEDStrip.h                              \
    RGBController/RGBController_LogitechG203.h                          \
    RGBController/RGBController_LogitechG403.h                          \
    RGBController/RGBController_LogitechG810.h                          \
    RGBController/RGBController_MSI3Zone.h                              \
    RGBController/RGBController_MSIGPU.h                                \
    RGBController/RGBController_MSIGPU.h                                \
    RGBController/RGBController_MSIMysticLight.h                        \
    RGBController/RGBController_MSIRGB.h                                \
    RGBController/RGBController_Network.h                               \
    RGBController/RGBController_NZXTHue2.h                              \
    RGBController/RGBController_NZXTKraken.h                            \
    RGBController/RGBController_PatriotViper.h                          \
    RGBController/RGBController_Polychrome.h                            \
    RGBController/RGBController_PoseidonZRGB.h                          \
    RGBController/RGBController_RedragonK556.h                          \
    RGBController/RGBController_RedragonM711.h                          \
    RGBController/RGBController_RGBFusion.h                             \
    RGBController/RGBController_RGBFusion2DRAM.h                        \
    RGBController/RGBController_RGBFusion2SMBus.h                       \
    RGBController/RGBController_RGBFusion2USB.h                         \
    RGBController/RGBController_RGBFusionGPU.h                          \
    RGBController/RGBController_SteelSeriesRival.h                      \
    RGBController/RGBController_SteelSeriesSiberia.h                    \
    RGBController/RGBController_SteelSeriesApex.h                       \
    RGBController/RGBController_ThermaltakeRiing.h                      \

SOURCES +=                                                              \
    dependencies/dmiinfo.cpp                                            \
    dependencies/ColorWheel/ColorWheel.cpp                              \
    dependencies/libe131/src/e131.c                                     \
    main.cpp                                                            \
    cli.cpp                                                             \
    OpenRGB.cpp                                                         \
    NetworkClient.cpp                                                   \
    NetworkServer.cpp                                                   \
    ProfileManager.cpp                                                  \
    qt/OpenRGBClientInfoPage.cpp                                        \
    qt/OpenRGBDeviceInfoPage.cpp                                        \
    qt/OpenRGBDevicePage.cpp                                            \
    qt/OpenRGBDialog.cpp                                                \
    i2c_smbus/i2c_smbus.cpp                                             \
    i2c_tools/i2c_tools.cpp                                             \
    net_port/net_port.cpp                                               \
    qt/OpenRGBDialog2.cpp                                               \
    qt/OpenRGBProfileSaveDialog.cpp                                     \
    qt/OpenRGBServerInfoPage.cpp                                        \
    qt/OpenRGBSoftwareInfoPage.cpp                                      \
    qt/OpenRGBSystemInfoPage.cpp                                        \
    qt/OpenRGBZoneResizeDialog.cpp                                      \
    qt/hsv.cpp                                                          \
    serial_port/serial_port.cpp                                         \
    super_io/super_io.cpp                                               \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.cpp   \
    Controllers/AMDWraithPrismController/AMDWraithPrismControllerDetect.cpp \
    Controllers/AuraUSBController/AuraUSBController.cpp                 \
    Controllers/AuraUSBController/AuraAddressableController.cpp         \
    Controllers/AuraUSBController/AuraMainboardController.cpp           \
    Controllers/AuraUSBController/AuraUSBControllerDetect.cpp           \
    Controllers/AuraCoreController/AuraCoreController.cpp               \
    Controllers/AuraCoreController/AuraCoreControllerDetect.cpp         \
    Controllers/AuraGPUController/AuraGPUController.cpp                 \
    Controllers/AuraGPUController/AuraGPUControllerDetect.cpp           \
    Controllers/AuraSMBusController/AuraSMBusController.cpp             \
    Controllers/AuraSMBusController/AuraSMBusControllerDetect.cpp       \
    Controllers/CoolerMasterController/CMMP750Controller.cpp            \
    Controllers/CoolerMasterController/CoolerMasterControllerDetect.cpp \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.cpp \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeControllerDetect.cpp \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.cpp \
    Controllers/CorsairPeripheralController/CorsairPeripheralControllerDetect.cpp \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.cpp \
    Controllers/CorsairVengeanceController/CorsairVengeanceControllerDetect.cpp \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.cpp \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProControllerDetect.cpp \
    Controllers/CrucialController/CrucialController.cpp                 \
    Controllers/CrucialController/CrucialControllerDetect.cpp           \
    Controllers/DuckyKeyboardController/DuckyKeyboardController.cpp     \
    Controllers/DuckyKeyboardController/DuckyKeyboardControllerDetect.cpp \
    Controllers/GloriousModelOController/GloriousModelOController.cpp   \
    Controllers/GloriousModelOController/GloriousModelOControllerDetect.cpp \
    Controllers/HuePlusController/HuePlusController.cpp                 \
    Controllers/HuePlusController/HuePlusControllerDetect.cpp           \
    Controllers/HyperXDRAMController/HyperXDRAMController.cpp           \
    Controllers/HyperXDRAMController/HyperXDRAMControllerDetect.cpp     \
    Controllers/HyperXKeyboardController/HyperXAlloyOriginsController.cpp \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.cpp   \
    Controllers/HyperXKeyboardController/HyperXKeyboardControllerDetect.cpp \
    Controllers/LEDStripController/LEDStripController.cpp               \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp         \
    Controllers/LogitechController/LogitechControllerDetect.cpp         \
    Controllers/LogitechController/LogitechG203Controller.cpp           \
    Controllers/LogitechController/LogitechG403Controller.cpp           \
    Controllers/LogitechController/LogitechG810Controller.cpp           \
    Controllers/MSI3ZoneController/MSI3ZoneController.cpp               \
    Controllers/MSI3ZoneController/MSI3ZoneControllerDetect.cpp         \
    Controllers/MSIGPUController/MSIGPUController.cpp                   \
    Controllers/MSIGPUController/MSIGPUControllerDetect.cpp             \
    Controllers/MSIMysticLightController/MSIMysticLightController.cpp   \
    Controllers/MSIMysticLightController/MSIMysticLightControllerDetect.cpp \
    Controllers/MSIRGBController/MSIRGBController.cpp                   \
    Controllers/MSIRGBController/MSIRGBControllerDetect.cpp             \
    Controllers/NZXTHue2Controller/NZXTHue2Controller.cpp               \
    Controllers/NZXTHue2Controller/NZXTHue2ControllerDetect.cpp         \
    Controllers/NZXTKrakenController/NZXTKrakenController.cpp           \
    Controllers/NZXTKrakenController/NZXTKrakenControllerDetect.cpp     \
    Controllers/PatriotViperController/PatriotViperController.cpp       \
    Controllers/PatriotViperController/PatriotViperControllerDetect.cpp \
    Controllers/PolychromeController/PolychromeController.cpp           \
    Controllers/PolychromeController/PolychromeControllerDetect.cpp     \
    Controllers/PoseidonZRGBController/PoseidonZRGBController.cpp       \
    Controllers/PoseidonZRGBController/PoseidonZRGBControllerDetect.cpp \
    Controllers/RGBFusionController/RGBFusionController.cpp             \
    Controllers/RGBFusionController/RGBFusionControllerDetect.cpp       \
    Controllers/RGBFusion2DRAMController/RGBFusion2DRAMController.cpp   \
    Controllers/RGBFusion2DRAMController/RGBFusion2DRAMControllerDetect.cpp \
    Controllers/RGBFusion2SMBusController/RGBFusion2SMBusController.cpp \
    Controllers/RGBFusion2SMBusController/RGBFusion2SMBusControllerDetect.cpp \
    Controllers/RGBFusion2USBController/RGBFusion2USBController.cpp     \
    Controllers/RGBFusion2USBController/RGBFusion2USBControllerDetect.cpp \
    Controllers/RGBFusionGPUController/RGBFusionGPUController.cpp       \
    Controllers/RGBFusionGPUController/RGBFusionGPUControllerDetect.cpp \
    Controllers/RedragonController/RedragonK556Controller.cpp           \
    Controllers/RedragonController/RedragonM711Controller.cpp           \
    Controllers/RedragonController/RedragonControllerDetect.cpp         \
    Controllers/SteelSeriesController/SteelSeriesRivalController.cpp    \
    Controllers/SteelSeriesController/SteelSeriesSiberiaController.cpp  \
    Controllers/SteelSeriesController/SteelSeriesApexController.cpp     \
    Controllers/SteelSeriesController/SteelSeriesControllerDetect.cpp   \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.cpp \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingControllerDetect.cpp \
    RGBController/RGBController.cpp                                     \
    RGBController/E131ControllerDetect.cpp                              \
    RGBController/RGBController_AMDWraithPrism.cpp                      \
    RGBController/RGBController_AuraUSB.cpp                             \
    RGBController/RGBController_AuraCore.cpp                            \
    RGBController/RGBController_AuraGPU.cpp                             \
    RGBController/RGBController_AuraSMBus.cpp                           \
    RGBController/RGBController_CMMP750Controller.cpp                   \
    RGBController/RGBController_CorsairLightingNode.cpp                 \
    RGBController/RGBController_CorsairPeripheral.cpp                   \
    RGBController/RGBController_CorsairVengeance.cpp                    \
    RGBController/RGBController_CorsairVengeancePro.cpp                 \
    RGBController/RGBController_Crucial.cpp                             \
    RGBController/RGBController_DuckyKeyboard.cpp                       \
    RGBController/RGBController_Dummy.cpp                               \
    RGBController/RGBController_GloriousModelO.cpp                      \
    RGBController/RGBController_HuePlus.cpp                             \
    RGBController/RGBController_HyperXAlloyOrigins.cpp                  \
    RGBController/RGBController_HyperXDRAM.cpp                          \
    RGBController/RGBController_HyperXKeyboard.cpp                      \
    RGBController/RGBController_E131.cpp                                \
    RGBController/RGBController_LEDStrip.cpp                            \
    RGBController/RGBController_LogitechG203.cpp                        \
    RGBController/RGBController_LogitechG403.cpp                        \
    RGBController/RGBController_LogitechG810.cpp                        \
    RGBController/RGBController_MSI3Zone.cpp                            \
    RGBController/RGBController_MSIGPU.cpp                              \
    RGBController/RGBController_MSIMysticLight.cpp                      \
    RGBController/RGBController_MSIRGB.cpp                              \
    RGBController/RGBController_Network.cpp                             \
    RGBController/RGBController_NZXTHue2.cpp                            \
    RGBController/RGBController_NZXTKraken.cpp                          \
    RGBController/RGBController_PatriotViper.cpp                        \
    RGBController/RGBController_Polychrome.cpp                          \
    RGBController/RGBController_PoseidonZRGB.cpp                        \
    RGBController/RGBController_RedragonK556.cpp                        \
    RGBController/RGBController_RedragonM711.cpp                        \
    RGBController/RGBController_RGBFusion.cpp                           \
    RGBController/RGBController_RGBFusion2DRAM.cpp                      \
    RGBController/RGBController_RGBFusion2SMBus.cpp                     \
    RGBController/RGBController_RGBFusion2USB.cpp                       \
    RGBController/RGBController_RGBFusionGPU.cpp                        \
    RGBController/RGBController_SteelSeriesRival.cpp                    \
    RGBController/RGBController_SteelSeriesSiberia.cpp                  \
    RGBController/RGBController_SteelSeriesApex.cpp                     \
    RGBController/RGBController_ThermaltakeRiing.cpp                    \

RESOURCES +=                                                            \
    qt/resources.qrc

FORMS +=                                                                \
    qt/OpenRGBClientInfoPage.ui                                         \
    qt/OpenRGBDeviceInfoPage.ui                                         \
    qt/OpenRGBDevicePage.ui                                             \
    qt/OpenRGBDialog.ui                                                 \
    qt/OpenRGBDialog2.ui                                                \
    qt/OpenRGBProfileSaveDialog.ui                                      \
    qt/OpenRGBServerInfoPage.ui                                         \
    qt/OpenRGBSoftwareInfoPage.ui                                       \
    qt/OpenRGBSystemInfoPage.ui                                         \
    qt/OpenRGBZoneResizeDialog.ui                                       \

#-----------------------------------------------------------------------#
# Windows-specific Configuration                                        #
#-----------------------------------------------------------------------#
win32:INCLUDEPATH +=                                                    \
    dependencies/display-library/include                                \
    dependencies/hidapi                                                 \
    dependencies/inpout32_1501/Win32/                                   \
    dependencies/libusb-1.0.22/include                                  \
    dependencies/NVFC                                                   \
    dependencies/openrazer-win32                                        \
    wmi/                                                                \

win32:SOURCES +=                                                        \
#   dependencies/hidapi/hidapi.c                                        \
    dependencies/NVFC/nvapi.cpp                                         \
    i2c_smbus/i2c_smbus_amdadl.cpp                                      \
    i2c_smbus/i2c_smbus_i801.cpp                                        \
    i2c_smbus/i2c_smbus_nct6775.cpp                                     \
    i2c_smbus/i2c_smbus_nvapi.cpp                                       \
    i2c_smbus/i2c_smbus_piix4.cpp                                       \
    serial_port/find_usb_serial_port_win.cpp                            \
    wmi/wmi.cpp                                                         \
    RGBController/OpenRazerWindowsDetect.cpp                            \
    RGBController/RGBController_OpenRazerWindows.cpp                    \

win32:HEADERS +=                                                        \
    dependencies/display-library/include/adl_defines.h                  \
    dependencies/display-library/include/adl_sdk.h                      \
    dependencies/display-library/include/adl_structures.h               \
    dependencies/inpout32_1501/Win32/inpout32.h                         \
    dependencies/NVFC/nvapi.h                                           \
    i2c_smbus/i2c_smbus_i801.h                                          \
    i2c_smbus/i2c_smbus_nct6775.h                                       \
    i2c_smbus/i2c_smbus_nvapi.h                                         \
    i2c_smbus/i2c_smbus_piix4.h                                         \
    wmi/wmi.h                                                           \
    RGBController/RGBController_OpenRazerWindows.h                      \

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                             \
        -lws2_32                                                        \
        -L"$$PWD/dependencies/inpout32_1501/x64/" -linpoutx64           \
        -L"$$PWD/dependencies/libusb-1.0.22/MS64/dll" -llibusb-1.0      \
        -L"$$PWD/dependencies/hidapi-win/x64/" -lhidapi                 \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                             \
        -lws2_32                                                        \
        -L"$$PWD/dependencies/inpout32_1501/Win32/" -linpout32          \
        -L"$$PWD/dependencies/libusb-1.0.22/MS32/dll" -llibusb-1.0      \
        -L"$$PWD/dependencies/hidapi-win/x86/" -lhidapi                 \
}

win32:DEFINES -=                                                        \
    UNICODE

win32:DEFINES +=                                                        \
    USE_HID_USAGE                                                       \
    _MBCS                                                               \
    WIN32                                                               \
    _CRT_SECURE_NO_WARNINGS                                             \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                     \
    WIN32_LEAN_AND_MEAN                                                 \

win32:RC_ICONS +=                                                       \
    qt/OpenRGB.ico

#-------------------------------------------------------------------#
# Windows GitLab CI Configuration                                   #
#-------------------------------------------------------------------#
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

#-------------------------------------------------------------------#
# Copy dependencies to output directory                             #
#-------------------------------------------------------------------#

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

#-----------------------------------------------------------------------#
# Linux-specific Configuration                                          #
#-----------------------------------------------------------------------#
unix:!macx {
    INCLUDEPATH +=                                                      \

    HEADERS +=                                                          \
    i2c_smbus/i2c_smbus_linux.h                                         \
    RGBController/RGBController_Faustus.h                               \

    LIBS +=                                                             \
    -lusb-1.0                                                           \
    -lstdc++fs                                                          \

    #-------------------------------------------------------------------#
    # Determine which hidapi to use based on availability               #
    #   Prefer hidraw backend, then libusb                              #
    #-------------------------------------------------------------------#
    packagesExist(hidapi-hidraw) {
        LIBS += -lhidapi-hidraw
    } else {
        packagesExist(hidapi-libusb) {
            LIBS += -lhidapi-libusb
        } else {
            LIBS += -lhidapi
        }
    }

    SOURCES +=                                                          \
    i2c_smbus/i2c_smbus_linux.cpp                                       \
    serial_port/find_usb_serial_port_linux.cpp                          \
    RGBController/OpenRazerDetect.cpp                                   \
    RGBController/RGBController_Faustus.cpp                             \
    RGBController/RGBController_OpenRazer.cpp                           \

    #-------------------------------------------------------------------#
    # Set up install paths                                              #
    # These install paths are used for AppImage and .deb packaging      #
    #-------------------------------------------------------------------#
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    target.path=$$PREFIX/bin/
    desktop.path=$$PREFIX/share/applications/
    desktop.files+=qt/OpenRGB.desktop
    pixmap.path=$$PREFIX/share/pixmaps/
    pixmap.files+=qt/OpenRGB.png
    INSTALLS += target desktop pixmap
}

#-----------------------------------------------------------------------#
# MacOS-specific Configuration                                          #
#-----------------------------------------------------------------------#
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

macx:ICON = qt/OpenRGB.icns

unix:macx {
    INCLUDEPATH +=                                                      \
    /usr/local/include                                                  \

    LIBS +=                                                             \
    -L/usr/local/lib -lusb-1.0 -lhidapi                                 \
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenRGB
TEMPLATE = app

VERSION         = 0.0
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

RC_ICONS = qt/OpenRGB.ico

INCLUDEPATH +=                                                          \
    dependencies/hidapi                                                 \
    dependencies/libe131/src/                                           \
    i2c_smbus/                                                          \
    i2c_tools/                                                          \
    net_port/                                                           \
    serial_port/                                                        \
    super_io/                                                           \
    Controllers/AMDWraithPrismController/                               \
    Controllers/AuraGPUController/                                      \
    Controllers/AuraSMBusController/                                    \
    Controllers/CorsairPeripheralController/                            \
    Controllers/CorsairLightingNodeController/                          \
    Controllers/CorsairVengeanceController/                             \
    Controllers/CorsairVengeanceProController/                          \
    Controllers/CrucialController/                                      \
    Controllers/Hue2Controller/                                         \
    Controllers/HuePlusController/                                      \
    Controllers/HyperXDRAMController/                                   \
    Controllers/HyperXKeyboardController/                               \
    Controllers/LEDStripController/                                     \
    Controllers/MSI3ZoneController/                                     \
    Controllers/MSIRGBController/                                       \
    Controllers/PatriotViperController/                                 \
    Controllers/PolychromeController/                                   \
    Controllers/PoseidonZRGBController/                                 \
    Controllers/RedragonController/                                     \
    Controllers/RGBFusionController/                                    \
    Controllers/RGBFusion2USBController/                                \
    Controllers/RGBFusionGPUController/                                 \
    Controllers/ThermaltakeRiingController/                             \
    RGBController/                                                      \
    qt/

SOURCES +=                                                              \
    dependencies/hidapi/hidapi.c                                        \
    dependencies/libe131/src/e131.c                                     \
    main.cpp                                                            \
    cli.cpp                                                             \
    OpenRGB.cpp                                                         \
    ProfileManager.cpp                                                  \
    qt/OpenRGBDeviceInfoPage.cpp                                        \
    qt/OpenRGBDevicePage.cpp                                            \
    qt/OpenRGBDialog.cpp                                                \
    i2c_smbus/i2c_smbus.cpp                                             \
    i2c_tools/i2c_tools.cpp                                             \
    net_port/net_port.cpp                                               \
    qt/OpenRGBDialog2.cpp                                               \
    qt/OpenRGBProfileSaveDialog.cpp                                     \
    qt/OpenRGBSoftwareInfoPage.cpp                                      \
    qt/OpenRGBSystemInfoPage.cpp                                        \
    qt/OpenRGBZoneResizeDialog.cpp                                      \
    qt/hsv.cpp                                                          \
    serial_port/serial_port.cpp                                         \
    super_io/super_io.cpp                                               \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.cpp   \
    Controllers/AMDWraithPrismController/AMDWraithPrismControllerDetect.cpp \
    Controllers/AuraGPUController/AuraGPUController.cpp                 \
    Controllers/AuraGPUController/AuraGPUControllerDetect.cpp           \
    Controllers/AuraSMBusController/AuraSMBusController.cpp             \
    Controllers/AuraSMBusController/AuraSMBusControllerDetect.cpp       \
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
    Controllers/Hue2Controller/Hue2Controller.cpp                       \
    Controllers/Hue2Controller/Hue2ControllerDetect.cpp                 \
    Controllers/HuePlusController/HuePlusController.cpp                 \
    Controllers/HuePlusController/HuePlusControllerDetect.cpp           \
    Controllers/HyperXDRAMController/HyperXDRAMController.cpp           \
    Controllers/HyperXDRAMController/HyperXDRAMControllerDetect.cpp     \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.cpp   \
    Controllers/HyperXKeyboardController/HyperXKeyboardControllerDetect.cpp \
    Controllers/LEDStripController/LEDStripController.cpp               \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp         \
    Controllers/MSI3ZoneController/MSI3ZoneController.cpp               \
    Controllers/MSI3ZoneController/MSI3ZoneControllerDetect.cpp         \
    Controllers/MSIRGBController/MSIRGBController.cpp                   \
    Controllers/MSIRGBController/MSIRGBControllerDetect.cpp             \
    Controllers/PatriotViperController/PatriotViperController.cpp       \
    Controllers/PatriotViperController/PatriotViperControllerDetect.cpp \
    Controllers/PolychromeController/PolychromeController.cpp           \
    Controllers/PolychromeController/PolychromeControllerDetect.cpp     \
    Controllers/PoseidonZRGBController/PoseidonZRGBController.cpp       \
    Controllers/PoseidonZRGBController/PoseidonZRGBControllerDetect.cpp \
    Controllers/RGBFusionController/RGBFusionController.cpp             \
    Controllers/RGBFusionController/RGBFusionControllerDetect.cpp       \
    Controllers/RGBFusion2USBController/RGBFusion2USBController.cpp     \
    Controllers/RGBFusion2USBController/RGBFusion2USBControllerDetect.cpp \
    Controllers/RGBFusionGPUController/RGBFusionGPUController.cpp       \
    Controllers/RGBFusionGPUController/RGBFusionGPUControllerDetect.cpp \
    Controllers/RedragonController/RedragonK556Controller.cpp           \
    Controllers/RedragonController/RedragonM711Controller.cpp           \
    Controllers/RedragonController/RedragonControllerDetect.cpp         \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.cpp \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingControllerDetect.cpp \
    RGBController/RGBController.cpp                                     \
    RGBController/E131ControllerDetect.cpp                              \
    RGBController/RGBController_AMDWraithPrism.cpp                      \
    RGBController/RGBController_AuraGPU.cpp                             \
    RGBController/RGBController_AuraSMBus.cpp                           \
    RGBController/RGBController_CorsairLightingNode.cpp                 \
    RGBController/RGBController_CorsairPeripheral.cpp                   \
    RGBController/RGBController_CorsairVengeance.cpp                    \
    RGBController/RGBController_CorsairVengeancePro.cpp                 \
    RGBController/RGBController_Crucial.cpp                             \
    RGBController/RGBController_Dummy.cpp                               \
    RGBController/RGBController_Hue2.cpp                                \
    RGBController/RGBController_HuePlus.cpp                             \
    RGBController/RGBController_HyperXDRAM.cpp                          \
    RGBController/RGBController_HyperXKeyboard.cpp                      \
    RGBController/RGBController_E131.cpp                                \
    RGBController/RGBController_LEDStrip.cpp                            \
    RGBController/RGBController_MSI3Zone.cpp                            \
    RGBController/RGBController_MSIRGB.cpp                              \
    RGBController/RGBController_PatriotViper.cpp                        \
    RGBController/RGBController_Polychrome.cpp                          \
    RGBController/RGBController_PoseidonZRGB.cpp                        \
    RGBController/RGBController_RedragonK556.cpp                        \
    RGBController/RGBController_RedragonM711.cpp                        \
    RGBController/RGBController_RGBFusion.cpp                           \
    RGBController/RGBController_RGBFusion2USB.cpp                       \
    RGBController/RGBController_RGBFusionGPU.cpp                        \
    RGBController/RGBController_ThermaltakeRiing.cpp                    \

HEADERS +=                                                              \
    ProfileManager.h                                                    \
    qt/OpenRGBDeviceInfoPage.h                                          \
    qt/OpenRGBDevicePage.h                                              \
    qt/OpenRGBDialog.h                                                  \
    i2c_smbus/i2c_smbus.h                                               \
    i2c_tools/i2c_tools.h                                               \
    net_port/net_port.h                                                 \
    qt/OpenRGBDialog2.h                                                 \
    qt/OpenRGBProfileSaveDialog.h                                       \
    qt/OpenRGBSoftwareInfoPage.h                                        \
    qt/OpenRGBSystemInfoPage.h                                          \
    qt/OpenRGBZoneResizeDialog.h                                        \
    serial_port/find_usb_serial_port.h                                  \
    serial_port/serial_port.h                                           \
    super_io/super_io.h                                                 \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.h     \
    Controllers/AuraGPUController/AuraGPUController.h                   \
    Controllers/AuraSMBusController/AuraSMBusController.h               \
    Controllers/CorsairLightingNodeController/CorsairLightingNodeController.h \
    Controllers/CorsairPeripheralController/CorsairPeripheralController.h \
    Controllers/CorsairVengeanceController/CorsairVengeanceController.h \
    Controllers/CorsairVengeanceProController/CorsairVengeanceProController.h \
    Controllers/CrucialController/CrucialController.h                   \
    Controllers/Hue2Controller/Hue2Controller.h                         \
    Controllers/HuePlusController/HuePlusController.h                   \
    Controllers/HyperXDRAMController/HyperXDRAMController.h             \
    Controllers/HyperXKeyboardController/HyperXKeyboardController.h     \
    Controllers/LEDStripController/LEDStripController.h                 \
    Controllers/MSI3ZoneController/MSI3ZoneController.h                 \
    Controllers/MSIRGBController/MSIRGBController.h                     \
    Controllers/PatriotViperController/PatriotViperController.h         \
    Controllers/PolychromeController/PolychromeController.h             \
    Controllers/PoseidonZRGBController/PoseidonZRGBController.h         \
    Controllers/RGBFusionController/RGBFusionController.h               \
    Controllers/RGBFusion2USBController/RGBFusion2USBController.h       \
    Controllers/RGBFusionGPUController/RGBFusionGPUController.h         \
    Controllers/RedragonController/RedragonK556Controller.h             \
    Controllers/RedragonController/RedragonM711Controller.h             \
    Controllers/ThermaltakeRiingController/ThermaltakeRiingController.h \
    RGBController/RGBController.h                                       \
    RGBController/RGBController_AMDWraithPrism.h                        \
    RGBController/RGBController_AuraGPU.h                               \
    RGBController/RGBController_AuraSMBus.h                             \
    RGBController/RGBController_CorsairLightingNode.h                   \
    RGBController/RGBController_CorsairPeripheral.h                     \
    RGBController/RGBController_CorsairVengeance.h                      \
    RGBController/RGBController_CorsairVengeancePro.h                   \
    RGBController/RGBController_Crucial.h                               \
    RGBController/RGBController_Dummy.h                                 \
    RGBController/RGBController_E131.h                                  \
    RGBController/RGBController_Hue2.h                                  \
    RGBController/RGBController_HuePlus.h                               \
    RGBController/RGBController_HyperXDRAM.h                            \
    RGBController/RGBController_HyperXKeyboard.h                        \
    RGBController/RGBController_LEDStrip.h                              \
    RGBController/RGBController_MSI3Zone.h                              \
    RGBController/RGBController_MSIRGB.h                                \
    RGBController/RGBController_PatriotViper.h                          \
    RGBController/RGBController_Polychrome.h                            \
    RGBController/RGBController_PoseidonZRGB.h                          \
    RGBController/RGBController_RedragonK556.h                          \
    RGBController/RGBController_RedragonM711.h                          \
    RGBController/RGBController_RGBFusion.h                             \
    RGBController/RGBController_RGBFusion2USB.h                         \
    RGBController/RGBController_RGBFusionGPU.h                          \
    RGBController/RGBController_ThermaltakeRiing.h                      \

RESOURCES += \
    qt/resources.qrc

FORMS += \
    qt/OpenRGBDeviceInfoPage.ui \
    qt/OpenRGBDevicePage.ui \
    qt/OpenRGBDialog.ui \
    qt/OpenRGBDialog2.ui \
    qt/OpenRGBProfileSaveDialog.ui \
    qt/OpenRGBSoftwareInfoPage.ui \
    qt/OpenRGBSystemInfoPage.ui \
    qt/OpenRGBZoneResizeDialog.ui

#-----------------------------------------------
# Windows specific project configuration
#-----------------------------------------------
win32:INCLUDEPATH +=                                                    \
    dependencies/inpout32_1501/Win32/                                   \
    dependencies/libusb-1.0.22/include                                  \
    dependencies/NVFC                                                   \
    dependencies/openrazer-win32                                        \
    wmi/                                                                \

win32:SOURCES +=                                                        \
    dependencies/NVFC/nvapi.cpp                                         \
    i2c_smbus/i2c_smbus_i801.cpp                                        \
    i2c_smbus/i2c_smbus_nct6775.cpp                                     \
    i2c_smbus/i2c_smbus_nvapi.cpp                                       \
    i2c_smbus/i2c_smbus_piix4.cpp                                       \
    serial_port/find_usb_serial_port_win.cpp                            \
    wmi/wmi.cpp                                                         \
    RGBController/OpenRazerWindowsDetect.cpp                            \
    RGBController/RGBController_OpenRazerWindows.cpp                    \

win32:HEADERS +=                                                        \
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
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                             \
        -lws2_32                                                        \
        -L"$$PWD/dependencies/inpout32_1501/Win32/" -linpout32          \
        -L"$$PWD/dependencies/libusb-1.0.22/MS32/dll" -llibusb-1.0      \
}

win32:DEFINES -=                                                        \
    UNICODE

win32:DEFINES +=                                                        \
    _MBCS                                                               \
    WIN32                                                               \
    _CRT_SECURE_NO_WARNINGS                                             \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                     \
    WIN32_LEAN_AND_MEAN

# Copy OpenRazer.dll to output directory
win32:contains(QMAKE_TARGET.arch, x86_64) {
    copydata.commands = $(COPY_FILE) \"$$shell_path($$PWD\\dependencies\\openrazer-win32\\OpenRazer64.dll)\" \"$$shell_path($$OUT_PWD)\"
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

win32:contains(QMAKE_TARGET.arch, x86) {
    copydata.commands = $(COPY_FILE) \"$$shell_path($$PWD\\dependencies\\openrazer-win32\\OpenRazer.dll)\" \"$$shell_path($$OUT_PWD)\"
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

#-----------------------------------------------
# Linux specific project configuration
#-----------------------------------------------
unix:INCLUDEPATH +=                                                     \

unix:HEADERS +=                                                         \
    i2c_smbus/i2c_smbus_linux.h                                         \
    RGBController/RGBController_Faustus.h                               \

unix:LIBS +=                                                            \
    -lusb-1.0                                                           \
    -lstdc++fs                                                          \

unix:SOURCES +=                                                         \
    i2c_smbus/i2c_smbus_linux.cpp                                       \
    serial_port/find_usb_serial_port_linux.cpp                          \
    RGBController/OpenRazerDetect.cpp                                   \
    RGBController/RGBController_Faustus.cpp                             \
    RGBController/RGBController_OpenRazer.cpp                           \

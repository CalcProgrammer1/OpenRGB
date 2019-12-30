QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenRGB
TEMPLATE = app

INCLUDEPATH +=                                                          \
    dependencies/libe131/src/                                           \
    i2c_smbus/                                                          \
    i2c_tools/                                                          \
    net_port/                                                           \
    serial_port/                                                        \
    Controllers/AMDWraithPrismController/                               \
    Controllers/AuraController/                                         \
    Controllers/CorsairController/                                      \
    Controllers/CorsairProController/                                   \
    Controllers/Hue2Controller/                                         \
    Controllers/HuePlusController/                                      \
    Controllers/HyperXController/                                       \
    Controllers/LEDStripController/                                     \
    Controllers/PatriotViperController/                                 \
    Controllers/PolychromeController/                                   \
    Controllers/RGBFusionController/                                    \
    RGBController/                                                      \
    qt/

SOURCES +=                                                              \
    dependencies/libe131/src/e131.c                                     \
    main.cpp                                                            \
    OpenRGB.cpp                                                         \
    qt/OpenRGBDeviceInfoPage.cpp                                        \
    qt/OpenRGBDevicePage.cpp                                            \
    qt/OpenRGBDialog.cpp                                                \
    i2c_smbus/i2c_smbus.cpp                                             \
    i2c_tools/i2c_tools.cpp                                             \
    net_port/net_port.cpp                                               \
    qt/OpenRGBDialog2.cpp                                               \
    qt/OpenRGBSystemInfoPage.cpp                                        \
    qt/hsv.cpp                                                          \
    serial_port/serial_port.cpp                                         \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.cpp   \
    Controllers/AMDWraithPrismController/AMDWraithPrismControllerDetect.cpp \
    Controllers/AuraController/AuraController.cpp                       \
    Controllers/AuraController/AuraControllerDetect.cpp                 \
    Controllers/CorsairController/CorsairController.cpp                 \
    Controllers/CorsairController/CorsairControllerDetect.cpp           \
    Controllers/CorsairProController/CorsairProController.cpp           \
    Controllers/CorsairProController/CorsairProControllerDetect.cpp     \
    Controllers/Hue2Controller/Hue2Controller.cpp                       \
    Controllers/Hue2Controller/Hue2ControllerDetect.cpp                 \
    Controllers/HuePlusController/HuePlusController.cpp                 \
    Controllers/HuePlusController/HuePlusControllerDetect.cpp           \
    Controllers/HyperXController/HyperXController.cpp                   \
    Controllers/HyperXController/HyperXControllerDetect.cpp             \
    Controllers/LEDStripController/LEDStripController.cpp               \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp         \
    Controllers/PatriotViperController/PatriotViperController.cpp       \
    Controllers/PatriotViperController/PatriotViperControllerDetect.cpp \
    Controllers/PolychromeController/PolychromeController.cpp           \
    Controllers/PolychromeController/PolychromeControllerDetect.cpp     \
    Controllers/RGBFusionController/RGBFusionController.cpp             \
    Controllers/RGBFusionController/RGBFusionControllerDetect.cpp       \
    RGBController/E131ControllerDetect.cpp                              \
    RGBController/RGBController_AMDWraithPrism.cpp                      \
    RGBController/RGBController_Aura.cpp                                \
    RGBController/RGBController_Corsair.cpp                             \
    RGBController/RGBController_CorsairPro.cpp                          \
    RGBController/RGBController_Hue2.cpp                                \
    RGBController/RGBController_HuePlus.cpp                             \
    RGBController/RGBController_HyperX.cpp                              \
    RGBController/RGBController_E131.cpp                                \
    RGBController/RGBController_LEDStrip.cpp                            \
    RGBController/RGBController_PatriotViper.cpp                        \
    RGBController/RGBController_Polychrome.cpp                          \
    RGBController/RGBController_RGBFusion.cpp

HEADERS +=                                                              \
    Controllers/RGBFusionController/RGBFusionController.h               \
    qt/OpenRGBDeviceInfoPage.h                                          \
    qt/OpenRGBDevicePage.h                                              \
    qt/OpenRGBDialog.h                                                  \
    i2c_smbus/i2c_smbus.h                                               \
    i2c_tools/i2c_tools.h                                               \
    net_port/net_port.h                                                 \
    qt/OpenRGBDialog2.h                                                 \
    qt/OpenRGBSystemInfoPage.h                                          \
    serial_port/find_usb_serial_port.h                                  \
    serial_port/serial_port.h                                           \
    Controllers/AMDWraithPrismController/AMDWraithPrismController.h     \
    Controllers/AuraController/AuraController.h                         \
    Controllers/CorsairController/CorsairController.h                   \
    Controllers/CorsairProController/CorsairProController.h             \
    Controllers/Hue2Controller/Hue2Controller.h                         \
    Controllers/HuePlusController/HuePlusController.h                   \
    Controllers/HyperXController/HyperXController.h                     \
    Controllers/LEDStripController/LEDStripController.h                 \
    Controllers/PatriotViperController/PatriotViperController.h         \
    Controllers/PolychromeController/PolychromeController.h             \
    Controllers/RGBFusionController/RGBFusionController.h               \
    RGBController/RGBController.h                                       \
    RGBController/RGBController_AMDWraithPrism.h                        \
    RGBController/RGBController_Aura.h                                  \
    RGBController/RGBController_Corsair.h                               \
    RGBController/RGBController_CorsairPro.h                            \
    RGBController/RGBController_E131.h                                  \
    RGBController/RGBController_Hue2.h                                  \
    RGBController/RGBController_HuePlus.h                               \
    RGBController/RGBController_HyperX.h                                \
    RGBController/RGBController_PatriotViper.h                          \
    RGBController/RGBController_Polychrome.h                            \
    RGBController/RGBController_RGBFusion.h

RESOURCES += \
    qt/resources.qrc

FORMS += \
    qt/OpenRGBDeviceInfoPage.ui \
    qt/OpenRGBDevicePage.ui \
    qt/OpenRGBDialog.ui \
    qt/OpenRGBDialog2.ui \
    qt/OpenRGBSystemInfoPage.ui

#-----------------------------------------------
# Windows specific project configuration
#-----------------------------------------------
win32:INCLUDEPATH +=                                                    \
    dependencies/inpout32_1501/Win32/                                   \
    dependencies/razer-chroma-2.9.0/inc                                 \
    dependencies/libusb-1.0.22/include                                  \
    wmi/                                                                \

win32:SOURCES +=                                                        \
    i2c_smbus/i2c_smbus_i801.cpp                                        \
    i2c_smbus/i2c_smbus_nct6775.cpp                                     \
    i2c_smbus/i2c_smbus_piix4.cpp                                       \
    serial_port/find_usb_serial_port_win.cpp                            \
    wmi/wmi.cpp                                                         \
    RGBController/AorusGPUDetect.cpp                                    \
    RGBController/RGBController_AorusGPU.cpp                            \
    RGBController/RazerChromaSDKDetect.cpp                              \
    RGBController/RGBController_RazerChromaSDK.cpp                      \

win32:HEADERS +=                                                        \
    dependencies/inpout32_1501/Win32/inpout32.h                         \
    i2c_smbus/i2c_smbus_i801.h                                          \
    i2c_smbus/i2c_smbus_nct6775.h                                       \
    i2c_smbus/i2c_smbus_piix4.h                                         \
    wmi/wmi.h                                                           \
    RGBController/RGBController_AorusGPU.h                              \
    RGBController/RGBController_RazerChromaSDK.h                        \

win32:LIBS +=                                                           \
    -lws2_32                                                            \
    -L"$$PWD/dependencies/inpout32_1501/Win32/" -linpout32              \
    -L"$$PWD/dependencies/libusb-1.0.22/MS32/dll" -llibusb-1.0

win32:DEFINES -=                                                        \
    UNICODE

win32:DEFINES +=                                                        \
    _MBCS                                                               \
    WIN32                                                               \
    _CRT_SECURE_NO_WARNINGS                                             \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                     \
    WIN32_LEAN_AND_MEAN

#-----------------------------------------------
# Linux specific project configuration
#-----------------------------------------------
unix:INCLUDEPATH +=                                                     \

unix:HEADERS +=                                                         \
    i2c_smbus/i2c_smbus_linux.h                                         \

unix:LIBS +=                                                            \
    -lusb-1.0

unix:SOURCES +=                                                         \
    i2c_smbus/i2c_smbus_linux.cpp                                       \
    serial_port/find_usb_serial_port_linux.cpp                          \
    RGBController/OpenRazerDetect.cpp                                   \
    RGBController/RGBController_OpenRazer.cpp                           \

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenRGB
TEMPLATE = app

INCLUDEPATH +=                                                          \
    i2c_smbus/                                                          \
    net_port/                                                           \
    serial_port/                                                        \
    Controllers/AuraController/                                         \
    Controllers/CorsairController/                                      \
    Controllers/CorsairProController/                                   \
    Controllers/HuePlusController/                                      \
    Controllers/HyperXController/                                       \
    Controllers/LEDStripController/                                     \
    Controllers/RGBFusionController/                                    \
    RGBController/                                                      \
    qt/

SOURCES +=                                                              \
    main.cpp                                                            \
    OpenAuraSDK.cpp                                                     \
    qt/OpenRGBDialog.cpp                                                \
    i2c_smbus/i2c_smbus.cpp                                             \
    net_port/net_port.cpp                                               \
    serial_port/serial_port.cpp                                         \
    Controllers/AuraController/AuraController.cpp                       \
    Controllers/AuraController/AuraControllerDetect.cpp                 \
    Controllers/CorsairController/CorsairController.cpp                 \
    Controllers/CorsairController/CorsairControllerDetect.cpp           \
    Controllers/CorsairProController/CorsairProController.cpp           \
    Controllers/CorsairProController/CorsairProControllerDetect.cpp     \
    Controllers/HuePlusController/HuePlusController.cpp                 \
    Controllers/HuePlusController/HuePlusControllerDetect.cpp           \
    Controllers/HyperXController/HyperXController.cpp                   \
    Controllers/HyperXController/HyperXControllerDetect.cpp             \
    Controllers/LEDStripController/LEDStripController.cpp               \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp         \
    Controllers/RGBFusionController/RGBFusionController.cpp             \
    Controllers/RGBFusionController/RGBFusionControllerDetect.cpp       \
    RGBController/RGBController_Aura.cpp                                \
    RGBController/RGBController_Corsair.cpp                             \
    RGBController/RGBController_CorsairPro.cpp                          \
    RGBController/RGBController_HuePlus.cpp                             \
    RGBController/RGBController_HyperX.cpp                              \
    RGBController/RGBController_LEDStrip.cpp                            \
    RGBController/RGBController_RGBFusion.cpp

HEADERS +=                                                              \
    qt/OpenRGBDialog.h                                                  \
    i2c_smbus/i2c_smbus.h                                               \
    net_port/net_port.h                                                 \
    serial_port/serial_port.h                                           \
    Controllers/AuraController/AuraController.h                         \
    Controllers/CorsairController/CorsairController.h                   \
    Controllers/CorsairProController/CorsairProController.h             \
    Controllers/HuePlusController/HuePlusController.h                   \
    Controllers/HyperXController/HyperXController.h                     \
    Controllers/LEDStripController/LEDStripController.h                 \
    RGBController/RGBController.h                                       \
    RGBController/RGBController_Aura.h                                  \
    RGBController/RGBController_Corsair.h                               \
    RGBController/RGBController_CorsairPro.h                            \
    RGBController/RGBController_HuePlus.h                               \
    RGBController/RGBController_HyperX.h                                \
    RGBController/RGBController_RGBFusion.h

RESOURCES += \
    qt/resources.qrc

FORMS += \
    qt/OpenRGBDialog.ui

#-----------------------------------------------
# Windows specific project configuration
#-----------------------------------------------
win32:INCLUDEPATH +=                                                    \
    dependencies/inpout32_1501/Win32/                                   \
    dependencies/razer-chroma-2.9.0/inc                                 \
    wmi/                                                                \

win32:SOURCES +=                                                        \
    i2c_smbus/i2c_smbus_i801.cpp                                        \
    i2c_smbus/i2c_smbus_nct6775.cpp                                     \
    i2c_smbus/i2c_smbus_piix4.cpp                                       \
    wmi/wmi.cpp                                                         \
    RGBController/RazerChromaSDKDetect.cpp                              \
    RGBController/RGBController_RazerChromaSDK.cpp                      \

win32:HEADERS +=                                                        \
    dependencies/inpout32_1501/Win32/inpout32.h                         \
    i2c_smbus/i2c_smbus_i801.h                                          \
    i2c_smbus/i2c_smbus_nct6775.h                                       \
    i2c_smbus/i2c_smbus_piix4.h                                         \
    wmi/wmi.h                                                           \
    RGBController/RGBController_RazerChromaSDK.h                        \

win32:LIBS +=                                                           \
    -lws2_32                                                            \
    -L"$$PWD/dependencies/inpout32_1501/Win32/" -linpout32

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
    dependencies/libe131/src/                                           \

unix:HEADERS +=                                                         \
    i2c_smbus/i2c_smbus_linux.h                                         \
    RGBController/RGBController_E131.h                                  \

unix:SOURCES +=                                                         \
    dependencies/libe131/src/e131.c                                     \
    i2c_smbus/i2c_smbus_linux.cpp                                       \
    RGBController/OpenRazerDetect.cpp                                   \
    RGBController/E131ControllerDetect.cpp                              \
    RGBController/RGBController_E131.cpp                                \
    RGBController/RGBController_OpenRazer.cpp                           \

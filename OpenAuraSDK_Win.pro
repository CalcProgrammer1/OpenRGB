QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenAuraSDK
TEMPLATE = app

LIBS +=                                                                 \
    -lws2_32                                                            \
    -L"$$PWD/dependencies/inpout32_1501/Win32/" -linpout32

DEFINES -=                                                              \
    UNICODE

DEFINES +=                                                              \
    _MBCS                                                               \
    WIN32                                                               \
    _CRT_SECURE_NO_WARNINGS                                             \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                     \
    WIN32_LEAN_AND_MEAN

INCLUDEPATH +=                                                          \
    dependencies/inpout32_1501/Win32/                                   \
    dependencies/razer-chroma-2.9.0/inc                                 \
    i2c_smbus/                                                          \
    net_port/                                                           \
    serial_port/                                                        \
    wmi/                                                                \
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
    qt/OpenAuraSDKQtDialog.cpp                                          \
    i2c_smbus/i2c_smbus.cpp                                             \
    i2c_smbus/i2c_smbus_i801.cpp                                        \
    i2c_smbus/i2c_smbus_nct6775.cpp                                     \
    i2c_smbus/i2c_smbus_piix4.cpp                                       \
    net_port/net_port.cpp                                               \
    serial_port/serial_port.cpp                                         \
    wmi/wmi.cpp                                                         \
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
    RGBController/RazerChromaSDKDetect.cpp                              \
    RGBController/RGBController_Aura.cpp                                \
    RGBController/RGBController_Corsair.cpp                             \
    RGBController/RGBController_CorsairPro.cpp                          \
    RGBController/RGBController_HuePlus.cpp                             \
    RGBController/RGBController_HyperX.cpp                              \
    RGBController/RGBController_LEDStrip.cpp                            \
    RGBController/RGBController_RazerChromaSDK.cpp                      \
    RGBController/RGBController_RGBFusion.cpp

HEADERS +=                                                              \
    dependencies/inpout32_1501/Win32/inpout32.h                         \
    qt/OpenAuraSDKQtDialog.h                                            \
    i2c_smbus/i2c_smbus.h                                               \
    i2c_smbus/i2c_smbus_i801.h                                          \
    i2c_smbus/i2c_smbus_nct6775.h                                       \
    i2c_smbus/i2c_smbus_piix4.h                                         \
    net_port/net_port.h                                                 \
    serial_port/serial_port.h                                           \
    wmi/wmi.h                                                           \
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
    RGBController/RGBController_RazerChromaSDK.h                        \
    RGBController/RGBController_RGBFusion.h

RESOURCES += \
    qt/resources.qrc

FORMS += \
    qt/openaurasdk.ui

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenAuraSDK.bin
TEMPLATE = app

INCLUDEPATH +=                                                          \
    dependencies/libe131/src/                                           \
    i2c_smbus/                                                          \
    net_port/                                                           \
    serial_port/                                                        \
    Controllers/AuraController/                                         \
    Controllers/CorsairController/                                      \
    Controllers/CorsairProController/                                   \
    Controllers/HuePlusController/                                      \
    Controllers/HyperXController/                                       \
    Controllers/LEDStripController/                                     \
    RGBController/                                                      \
    qt/

SOURCES +=                                                              \
    dependencies/libe131/src/e131.c                                     \
    main.cpp                                                            \
    OpenAuraSDK.cpp                                                     \
    qt/OpenAuraSDKQtDialog.cpp                                          \
    i2c_smbus/i2c_smbus.cpp                                             \
    i2c_smbus/i2c_smbus_linux.cpp                                       \
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
    RGBController/OpenRazerDetect.cpp                                   \
    RGBController/E131ControllerDetect.cpp                              \
    RGBController/RGBController_Aura.cpp                                \
    RGBController/RGBController_Corsair.cpp                             \
    RGBController/RGBController_CorsairPro.cpp                          \
    RGBController/RGBController_E131.cpp                                \
    RGBController/RGBController_HuePlus.cpp                             \
    RGBController/RGBController_HyperX.cpp                              \
    RGBController/RGBController_LEDStrip.cpp                            \
    RGBController/RGBController_OpenRazer.cpp

HEADERS += \
    qt/OpenAuraSDKQtDialog.h                                            \
    i2c_smbus/i2c_smbus.h                                               \
    i2c_smbus/i2c_smbus_linux.h                                         \
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
    RGBController/RGBController_E131.h                                  \
    RGBController/RGBController_HuePlus.h                               \
    RGBController/RGBController_HyperX.h                                \
    RGBController/RGBController_OpenRazer.h

RESOURCES += \
    qt/resources.qrc

FORMS += \
    qt/openaurasdk.ui

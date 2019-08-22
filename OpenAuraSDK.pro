QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenAuraSDK.bin
TEMPLATE = app

INCLUDEPATH +=                                                          \
    i2c_smbus/                                                          \
    serial_port/                                                        \
    Controllers/AuraController/                                         \
    Controllers/CorsairController/                                      \
    Controllers/CorsairProController/                                   \
    Controllers/HyperXController/                                       \
    Controllers/LEDStripController/                                     \
    RGBController/                                                      \
    qt/

SOURCES +=                                                              \
    main.cpp                                                            \
    OpenAuraSDK.cpp                                                     \
    qt/OpenAuraSDKQtDialog.cpp                                          \
    i2c_smbus/i2c_smbus.cpp                                             \
    i2c_smbus/i2c_smbus_linux.cpp                                       \
    serial_port/serial_port.cpp                                         \
    Controllers/AuraController/AuraController.cpp                       \
    Controllers/AuraController/AuraControllerDetect.cpp                 \
    Controllers/CorsairController/CorsairController.cpp                 \
    Controllers/CorsairController/CorsairControllerDetect.cpp           \
    Controllers/CorsairProController/CorsairProController.cpp           \
    Controllers/CorsairProController/CorsairProControllerDetect.cpp     \
    Controllers/HyperXController/HyperXController.cpp                   \
    Controllers/HyperXController/HyperXControllerDetect.cpp             \
    Controllers/LEDStripController/LEDStripController.cpp               \
    Controllers/LEDStripController/LEDStripControllerDetect.cpp         \
    RGBController/OpenRazerDetect.cpp                                   \
    RGBController/RGBController_Aura.cpp                                \
    RGBController/RGBController_Corsair.cpp                             \
    RGBController/RGBController_CorsairPro.cpp                          \
    RGBController/RGBController_HyperX.cpp                              \
    RGBController/RGBController_LEDStrip.cpp                            \
    RGBController/RGBController_OpenRazer.cpp

HEADERS += \
    qt/OpenAuraSDKQtDialog.h                                            \
    i2c_smbus/i2c_smbus.h                                               \
    i2c_smbus/i2c_smbus_linux.h                                         \
    serial_port/serial_port.h                                           \
    Controllers/AuraController/AuraController.h                         \
    Controllers/CorsairController/CorsairController.h                   \
    Controllers/CorsairProController/CorsairProController.h             \
    Controllers/HyperXController/HyperXController.h                     \
    Controllers/LEDStripController/LEDStripController.h                 \
    RGBController/RGBController.h                                       \
    RGBController/RGBController_Aura.h                                  \
    RGBController/RGBController_Corsair.h                               \
    RGBController/RGBController_CorsairPro.h                            \
    RGBController/RGBController_HyperX.h                                \
    RGBController/RGBController_OpenRazer.h

FORMS += \
    qt/openaurasdk.ui

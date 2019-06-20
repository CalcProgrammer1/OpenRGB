QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenAuraSDK.bin
TEMPLATE = app

SOURCES += \
    OpenAuraSDK/i2c_smbus.cpp \
    OpenAuraSDK/AuraController.cpp \
    OpenAuraSDK/OpenAuraSDK.cpp \
    OpenAuraSDK/i2c_smbus_linux.cpp \
    OpenAuraSDK/OpenAuraSDKQtDialog.cpp \
    OpenAuraSDK/CorsairController.cpp \
    OpenAuraSDK/RGBController_Aura.cpp \
    OpenAuraSDK/RGBController_Corsair.cpp \
    OpenAuraSDK/RGBController_OpenRazer.cpp \
    OpenAuraSDK/main.cpp

HEADERS += \
    OpenAuraSDK/i2c_smbus.h \
    OpenAuraSDK/i2c_smbus_linux.h \
    OpenAuraSDK/AuraController.h \
    OpenAuraSDK/OpenAuraSDKQtDialog.h \
    OpenAuraSDK/CorsairController.h \
    OpenAuraSDK/RGBController.h \
    OpenAuraSDK/RGBController_Aura.h \
    OpenAuraSDK/RGBController_Corsair.h \
    OpenAuraSDK/RGBController_OpenRazer.h

FORMS += \
    OpenAuraSDK/openaurasdk.ui

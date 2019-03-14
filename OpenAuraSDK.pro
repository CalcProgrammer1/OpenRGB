QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenAuraSDK.bin
TEMPLATE = app

SOURCES += \
    OpenAuraSDK/i2c_smbus.cpp \
    OpenAuraSDK/AuraController.cpp \
    OpenAuraSDK/OpenAuraSDK.cpp \
    OpenAuraSDK/i2c_smbus_linux.cpp \
    OpenAuraSDK/OpenAuraSDKQtDialog.cpp

HEADERS += \
    OpenAuraSDK/i2c_smbus.h \
    OpenAuraSDK/i2c_smbus_linux.h \
    OpenAuraSDK/AuraController.h \
    OpenAuraSDK/OpenAuraSDKQtDialog.h

FORMS += \
    OpenAuraSDK/openaurasdk.ui

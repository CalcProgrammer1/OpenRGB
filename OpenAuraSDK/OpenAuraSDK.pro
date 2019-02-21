QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenAuraSDK
TEMPLATE = app

SOURCES += \
    i2c_smbus.cpp \
    AuraController.cpp \
    OpenAuraSDK.cpp \
    i2c_smbus_linux.cpp \
    OpenAuraSDKQtDialog.cpp

HEADERS += \
    i2c_smbus.h \
    i2c_smbus_linux.h \
    AuraController.h \
    OpenAuraSDKQtDialog.h

FORMS += \
    openaurasdk.ui

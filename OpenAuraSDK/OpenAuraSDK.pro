TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    i2c_smbus.cpp \
    AuraController.cpp \
    OpenAuraSDK.cpp \
    i2c_smbus_linux.cpp

HEADERS += \
    i2c_smbus.h \
    i2c_smbus_linux.h \
    AuraController.h \
    OpenAuraSDK.h

FORMS +=

#-----------------------------------------------------------------------------------------------#
# OpenRGB 0.x QMake Project                                                                     #
#                                                                                               #
#   Adam Honse (CalcProgrammer1)                        5/25/2020                               #
#-----------------------------------------------------------------------------------------------#

#-----------------------------------------------------------------------------------------------#
# Qt Configuration                                                                              #
#-----------------------------------------------------------------------------------------------#
QT +=                                                                                           \
    core                                                                                        \
    gui                                                                                         \

#-----------------------------------------------------------------------------------------------#
# Set compiler to use C++17 to make std::filesystem available                                   #
#-----------------------------------------------------------------------------------------------#
CONFIG +=   c++17                                                                               \
            lrelease                                                                            \
            embed_translations                                                                  \
            silent                                                                              \

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#-----------------------------------------------------------------------------------------------#
# Application Configuration                                                                     #
#-----------------------------------------------------------------------------------------------#
MAJOR       = 0
MINOR       = 9
SUFFIX      = git

SHORTHASH   = $$system("git rev-parse --short=7 HEAD")
LASTTAG     = "release_"$$MAJOR"."$$MINOR
COMMAND     = "git rev-list --count "$$LASTTAG"..HEAD"
COMMITS     = $$system($$COMMAND)

VERSION_NUM = $$MAJOR"."$$MINOR"."$$COMMITS
VERSION_STR = $$MAJOR"."$$MINOR

VERSION_DEB = $$VERSION_NUM
VERSION_WIX = $$VERSION_NUM
VERSION_AUR = $$VERSION_NUM
VERSION_RPM = $$VERSION_NUM

equals(SUFFIX, "git") {
VERSION_STR = $$VERSION_STR"+ ("$$SUFFIX$$COMMITS")"
VERSION_DEB = $$VERSION_DEB"~git"$$SHORTHASH
VERSION_AUR = $$VERSION_AUR".g"$$SHORTHASH
VERSION_RPM = $$VERSION_RPM"^git"$$SHORTHASH
} else {
    !isEmpty(SUFFIX) {
VERSION_STR = $$VERSION_STR"+ ("$$SUFFIX")"
VERSION_DEB = $$VERSION_DEB"~"$$SUFFIX
VERSION_AUR = $$VERSION_AUR"."$$SUFFIX
VERSION_RPM = $$VERSION_RPM"^"$$SUFFIX
    }
}

TARGET      = OpenRGB
TEMPLATE    = app

message("VERSION_NUM: "$$VERSION_NUM)
message("VERSION_STR: "$$VERSION_STR)
message("VERSION_SFX: "$$SUFFIX)
message("VERSION_DEB: "$$VERSION_DEB)
message("VERSION_WIX: "$$VERSION_WIX)
message("VERSION_AUR: "$$VERSION_AUR)
message("VERSION_RPM: "$$VERSION_RPM)
message("QT_VERSION:  "$$QT_VERSION)
#-----------------------------------------------------------------------------------------------#
# Automatically generated build information                                                     #
#-----------------------------------------------------------------------------------------------#
win32:BUILDDATE         = $$system(date /t)
linux:BUILDDATE         = $$system(date -R -d "@${SOURCE_DATE_EPOCH:-$(date +%s)}")
freebsd:BUILDDATE       = $$system(date -j -R -r "${SOURCE_DATE_EPOCH:-$(date +%s)}")
macx:BUILDDATE          = $$system(date -j -R -r "${SOURCE_DATE_EPOCH:-$(date +%s)}")
GIT_COMMIT_ID           = $$system(git log -n 1 --pretty=format:"%H")
GIT_COMMIT_DATE         = $$system(git log -n 1 --pretty=format:"%ci")

unix {
    GIT_BRANCH          = $$system(sh scripts/git-get-branch.sh)
}
else {
    GIT_BRANCH          = $$system(powershell -ExecutionPolicy Bypass -File scripts/git-get-branch.ps1)
}

message("GIT_BRANCH: "$$GIT_BRANCH)
DEFINES +=                                                                                      \
    VERSION_STRING=\\"\"\"$$VERSION_STR\\"\"\"                                                  \
    BUILDDATE_STRING=\\"\"\"$$BUILDDATE\\"\"\"                                                  \
    GIT_COMMIT_ID=\\"\"\"$$GIT_COMMIT_ID\\"\"\"                                                 \
    GIT_COMMIT_DATE=\\"\"\"$$GIT_COMMIT_DATE\\"\"\"                                             \
    GIT_BRANCH=\\"\"\"$$GIT_BRANCH\\"\"\"

#-----------------------------------------------------------------------------------------------#
# OpenRGB dynamically added sources                                                             #
#-----------------------------------------------------------------------------------------------#
FORMS += $$files("qt/*.ui", true)

for(iter, FORMS) {
    GUI_INCLUDES += $$dirname(iter)
}
GUI_INCLUDES        = $$unique(GUI_INCLUDES)

GUI_H               = $$files("qt/*.h", true)
GUI_CPP             = $$files("qt/*.cpp", true)

CONTROLLER_H        = $$files("Controllers/*.h", true)
CONTROLLER_CPP      = $$files("Controllers/*.cpp", true)

for(iter, $$list($$CONTROLLER_H)) {
    CONTROLLER_INCLUDES += $$dirname(iter)
}
CONTROLLER_INCLUDES = $$unique(CONTROLLER_INCLUDES)

#-----------------------------------------------------------------------------------------------#
# Remove OS-specific files from the overall controller headers and sources lists                #
# The suffixes _Windows, _Linux, _FreeBSD, and _MacOS are usable to denote that a file only     #
# applies to one or more OSes.  The suffixes may be combined such as <file>_Windows_Linux.cpp.  #
#-----------------------------------------------------------------------------------------------#
CONTROLLER_H_WINDOWS    = $$files("Controllers/*_Windows*.h",   true)
CONTROLLER_CPP_WINDOWS  = $$files("Controllers/*_Windows*.cpp", true)
CONTROLLER_H_LINUX      = $$files("Controllers/*_Linux*.h",     true)
CONTROLLER_CPP_LINUX    = $$files("Controllers/*_Linux*.cpp",   true)
CONTROLLER_H_FREEBSD    = $$files("Controllers/*_FreeBSD*.h",   true)
CONTROLLER_CPP_FREEBSD  = $$files("Controllers/*_FreeBSD*.cpp", true)
CONTROLLER_H_MACOS      = $$files("Controllers/*_MacOS*.h",     true)
CONTROLLER_CPP_MACOS    = $$files("Controllers/*_MacOS*.cpp",   true)

CONTROLLER_H           -= $$CONTROLLER_H_WINDOWS
CONTROLLER_H           -= $$CONTROLLER_H_LINUX
CONTROLLER_H           -= $$CONTROLLER_H_FREEBSD
CONTROLLER_H           -= $$CONTROLLER_H_MACOS

CONTROLLER_CPP         -= $$CONTROLLER_CPP_WINDOWS
CONTROLLER_CPP         -= $$CONTROLLER_CPP_LINUX
CONTROLLER_CPP         -= $$CONTROLLER_CPP_FREEBSD
CONTROLLER_CPP         -= $$CONTROLLER_CPP_MACOS

#-----------------------------------------------------------------------------------------------#
# OpenRGB Common                                                                                #
#-----------------------------------------------------------------------------------------------#
INCLUDEPATH +=                                                                                  \
    $$CONTROLLER_INCLUDES                                                                       \
    $$GUI_INCLUDES                                                                              \
    dependencies/ColorWheel                                                                     \
    dependencies/CRCpp/                                                                         \
    dependencies/hueplusplus-1.2.0/include                                                      \
    dependencies/hueplusplus-1.2.0/include/hueplusplus                                          \
    dependencies/httplib                                                                        \
    dependencies/json/                                                                          \
    dependencies/mdns                                                                           \
    dmiinfo/                                                                                    \
    hidapi_wrapper/                                                                             \
    i2c_smbus/                                                                                  \
    i2c_tools/                                                                                  \
    interop/                                                                                    \
    net_port/                                                                                   \
    pci_ids/                                                                                    \
    scsiapi/                                                                                    \
    serial_port/                                                                                \
    super_io/                                                                                   \
    AutoStart/                                                                                  \
    KeyboardLayoutManager/                                                                      \
    RGBController/                                                                              \
    qt/                                                                                         \
    SPDAccessor/                                                                                \
    SuspendResume/

HEADERS +=                                                                                      \
    $$GUI_H                                                                                     \
    $$CONTROLLER_H                                                                              \
    Colors.h                                                                                    \
    dependencies/ColorWheel/ColorWheel.h                                                        \
    dependencies/json/nlohmann/json.hpp                                                         \
    LogManager.h                                                                                \
    NetworkClient.h                                                                             \
    NetworkProtocol.h                                                                           \
    NetworkServer.h                                                                             \
    OpenRGBPluginInterface.h                                                                    \
    PluginManager.h                                                                             \
    ProfileManager.h                                                                            \
    ResourceManager.h                                                                           \
    ResourceManagerInterface.h                                                                  \
    SettingsManager.h                                                                           \
    Detector.h                                                                                  \
    DeviceDetector.h                                                                            \
    dmiinfo/dmiinfo.h                                                                           \
    filesystem.h                                                                                \
    hidapi_wrapper/hidapi_wrapper.h                                                             \
    i2c_smbus/i2c_smbus.h                                                                       \
    i2c_tools/i2c_tools.h                                                                       \
    interop/DeviceGuard.h                                                                       \
    interop/DeviceGuardLock.h                                                                   \
    interop/DeviceGuardManager.h                                                                \
    net_port/net_port.h                                                                         \
    pci_ids/pci_ids.h                                                                           \
    scsiapi/scsiapi.h                                                                           \
    serial_port/find_usb_serial_port.h                                                          \
    serial_port/serial_port.h                                                                   \
    StringUtils.h                                                                               \
    super_io/super_io.h                                                                         \
    SuspendResume/SuspendResume.h                                                               \
    AutoStart/AutoStart.h                                                                       \
    KeyboardLayoutManager/KeyboardLayoutManager.h                                               \
    RGBController/RGBController.h                                                               \
    RGBController/RGBController_Dummy.h                                                         \
    RGBController/RGBControllerKeyNames.h                                                       \
    RGBController/RGBController_Network.h                                                       \
    startup/startup.h                                                                           \

SOURCES +=                                                                                      \
    $$GUI_CPP                                                                                   \
    $$CONTROLLER_CPP                                                                            \
    dependencies/ColorWheel/ColorWheel.cpp                                                      \
    dependencies/hueplusplus-1.2.0/src/Action.cpp                                               \
    dependencies/hueplusplus-1.2.0/src/APICache.cpp                                             \
    dependencies/hueplusplus-1.2.0/src/BaseDevice.cpp                                           \
    dependencies/hueplusplus-1.2.0/src/BaseHttpHandler.cpp                                      \
    dependencies/hueplusplus-1.2.0/src/Bridge.cpp                                               \
    dependencies/hueplusplus-1.2.0/src/BridgeConfig.cpp                                         \
    dependencies/hueplusplus-1.2.0/src/CLIPSensors.cpp                                          \
    dependencies/hueplusplus-1.2.0/src/ColorUnits.cpp                                           \
    dependencies/hueplusplus-1.2.0/src/EntertainmentMode.cpp                                    \
    dependencies/hueplusplus-1.2.0/src/ExtendedColorHueStrategy.cpp                             \
    dependencies/hueplusplus-1.2.0/src/ExtendedColorTemperatureStrategy.cpp                     \
    dependencies/hueplusplus-1.2.0/src/Group.cpp                                                \
    dependencies/hueplusplus-1.2.0/src/HueCommandAPI.cpp                                        \
    dependencies/hueplusplus-1.2.0/src/HueDeviceTypes.cpp                                       \
    dependencies/hueplusplus-1.2.0/src/HueException.cpp                                         \
    dependencies/hueplusplus-1.2.0/src/Light.cpp                                                \
    dependencies/hueplusplus-1.2.0/src/ModelPictures.cpp                                        \
    dependencies/hueplusplus-1.2.0/src/NewDeviceList.cpp                                        \
    dependencies/hueplusplus-1.2.0/src/Scene.cpp                                                \
    dependencies/hueplusplus-1.2.0/src/Schedule.cpp                                             \
    dependencies/hueplusplus-1.2.0/src/Sensor.cpp                                               \
    dependencies/hueplusplus-1.2.0/src/SimpleBrightnessStrategy.cpp                             \
    dependencies/hueplusplus-1.2.0/src/SimpleColorHueStrategy.cpp                               \
    dependencies/hueplusplus-1.2.0/src/SimpleColorTemperatureStrategy.cpp                       \
    dependencies/hueplusplus-1.2.0/src/StateTransaction.cpp                                     \
    dependencies/hueplusplus-1.2.0/src/TimePattern.cpp                                          \
    dependencies/hueplusplus-1.2.0/src/UPnP.cpp                                                 \
    dependencies/hueplusplus-1.2.0/src/Utils.cpp                                                \
    dependencies/hueplusplus-1.2.0/src/ZLLSensors.cpp                                           \
    startup/startup.cpp                                                                         \
    cli.cpp                                                                                     \
    dmiinfo/dmiinfo.cpp                                                                         \
    LogManager.cpp                                                                              \
    NetworkClient.cpp                                                                           \
    NetworkProtocol.cpp                                                                         \
    NetworkServer.cpp                                                                           \
    PluginManager.cpp                                                                           \
    ProfileManager.cpp                                                                          \
    ResourceManager.cpp                                                                         \
    SPDAccessor/DDR4DirectAccessor.cpp                                                          \
    SPDAccessor/DDR5DirectAccessor.cpp                                                          \
    SPDAccessor/SPDAccessor.cpp                                                                 \
    SPDAccessor/SPDDetector.cpp                                                                 \
    SPDAccessor/SPDWrapper.cpp                                                                  \
    SettingsManager.cpp                                                                         \
    i2c_smbus/i2c_smbus.cpp                                                                     \
    i2c_tools/i2c_tools.cpp                                                                     \
    interop/DeviceGuard.cpp                                                                     \
    interop/DeviceGuardLock.cpp                                                                 \
    interop/DeviceGuardManager.cpp                                                              \
    net_port/net_port.cpp                                                                       \
    serial_port/serial_port.cpp                                                                 \
    StringUtils.cpp                                                                             \
    super_io/super_io.cpp                                                                       \
    AutoStart/AutoStart.cpp                                                                     \
    KeyboardLayoutManager/KeyboardLayoutManager.cpp                                             \
    RGBController/RGBController.cpp                                                             \
    RGBController/RGBController_Dummy.cpp                                                       \
    RGBController/RGBControllerKeyNames.cpp                                                     \
    RGBController/RGBController_Network.cpp                                                     \

RESOURCES +=                                                                                    \
    qt/resources.qrc                                                                            \

#-----------------------------------------------------------------------------------------------#
# General configuration to decide if in-tree dependencies are used or not
#-----------------------------------------------------------------------------------------------#

!system_libe131:SOURCES += dependencies/libe131/src/e131.c
!system_libe131:INCLUDEPATH += dependencies/libe131/src/

#-----------------------------------------------------------------------------------------------#
# General configuration out-of-tree dependencies if in-tree are not used for systems
# who use pkg-config i.e. Unix-like. Also includes macOS as Homebrew uses pkg-config too.
#-----------------------------------------------------------------------------------------------#

unix {
    system_libe131 {
        CONFIG += link_pkgconfig
        PKGCONFIG += libe131
    }
}

#-----------------------------------------------------------------------------------------------#
# Translations                                                                                  #
#   NB: Translation files should not be added dynamically due to the process                    #
#       to add new translations relies on entries here in OpenRGB.pro                           #
#-----------------------------------------------------------------------------------------------#
TRANSLATIONS +=                                                                                 \
    qt/i18n/OpenRGB_be_BY.ts                                                                    \
    qt/i18n/OpenRGB_de_DE.ts                                                                    \
    qt/i18n/OpenRGB_el_GR.ts                                                                    \
    qt/i18n/OpenRGB_en_US.ts                                                                    \
    qt/i18n/OpenRGB_en_AU.ts                                                                    \
    qt/i18n/OpenRGB_en_GB.ts                                                                    \
    qt/i18n/OpenRGB_es_ES.ts                                                                    \
    qt/i18n/OpenRGB_fr_FR.ts                                                                    \
    qt/i18n/OpenRGB_hr_HR.ts                                                                    \
    qt/i18n/OpenRGB_it_IT.ts                                                                    \
    qt/i18n/OpenRGB_ja_JP.ts                                                                    \
    qt/i18n/OpenRGB_ko_KR.ts                                                                    \
    qt/i18n/OpenRGB_ms_MY.ts                                                                    \
    qt/i18n/OpenRGB_nb_NO.ts                                                                    \
    qt/i18n/OpenRGB_pl_PL.ts                                                                    \
    qt/i18n/OpenRGB_pt_BR.ts                                                                    \
    qt/i18n/OpenRGB_ru_RU.ts                                                                    \
    qt/i18n/OpenRGB_uk_UA.ts                                                                    \
    qt/i18n/OpenRGB_zh_CN.ts                                                                    \
    qt/i18n/OpenRGB_zh_TW.ts                                                                    \

#-----------------------------------------------------------------------------------------------#
# Windows-specific Configuration                                                                #
#-----------------------------------------------------------------------------------------------#
win32:QMAKE_CXXFLAGS += /utf-8
win32:INCLUDEPATH +=                                                                            \
    dependencies/display-library/include                                                        \
    dependencies/hidapi-win/include                                                             \
    dependencies/winring0/include                                                               \
    dependencies/libusb-1.0.27/include                                                          \
    dependencies/mbedtls-3.2.1/include                                                          \
    dependencies/NVFC                                                                           \
    wmi/                                                                                        \

win32:SOURCES += $$CONTROLLER_CPP_WINDOWS

win32:SOURCES +=                                                                                \
    dependencies/hueplusplus-1.2.0/src/WinHttpHandler.cpp                                       \
    dependencies/NVFC/nvapi.cpp                                                                 \
    i2c_smbus/i2c_smbus_amdadl.cpp                                                              \
    i2c_smbus/i2c_smbus_i801.cpp                                                                \
    i2c_smbus/i2c_smbus_nct6775.cpp                                                             \
    i2c_smbus/i2c_smbus_nvapi.cpp                                                               \
    i2c_smbus/i2c_smbus_piix4.cpp                                                               \
    scsiapi/scsiapi_windows.c                                                                   \
    serial_port/find_usb_serial_port_win.cpp                                                    \
    SuspendResume/SuspendResume_Windows.cpp                                                     \
    wmi/wmi.cpp                                                                                 \
    AutoStart/AutoStart-Windows.cpp                                                             \
    startup/main_Windows.cpp                                                                    \

win32:HEADERS += $$CONTROLLER_H_WINDOWS

win32:HEADERS +=                                                                                \
    dependencies/display-library/include/adl_defines.h                                          \
    dependencies/display-library/include/adl_sdk.h                                              \
    dependencies/display-library/include/adl_structures.h                                       \
    dependencies/winring0/include/OlsApi.h                                                      \
    dependencies/NVFC/nvapi.h                                                                   \
    i2c_smbus/i2c_smbus_i801.h                                                                  \
    i2c_smbus/i2c_smbus_nct6775.h                                                               \
    i2c_smbus/i2c_smbus_nvapi.h                                                                 \
    i2c_smbus/i2c_smbus_piix4.h                                                                 \
    wmi/wmi.h                                                                                   \
    AutoStart/AutoStart-Windows.h                                                               \
    SuspendResume/SuspendResume_Windows.h                                                       \

win32:contains(QMAKE_TARGET.arch, x86_64) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -liphlpapi                                                                              \
        -L"$$PWD/dependencies/winring0/x64/" -lWinRing0x64                                      \
        -L"$$PWD/dependencies/libusb-1.0.27/VS2019/MS64/dll" -llibusb-1.0                       \
        -L"$$PWD/dependencies/hidapi-win/x64/" -lhidapi                                         \
        -L"$$PWD/dependencies/mbedtls-3.2.1/lib/x64/" -lmbedcrypto -lmbedtls -lmbedx509         \
}

win32:contains(QMAKE_TARGET.arch, x86) {
    LIBS +=                                                                                     \
        -lws2_32                                                                                \
        -liphlpapi                                                                              \
        -L"$$PWD/dependencies/winring0/Win32/" -lWinRing0                                       \
        -L"$$PWD/dependencies/libusb-1.0.27/VS2019/MS32/dll" -llibusb-1.0                       \
        -L"$$PWD/dependencies/hidapi-win/x86/" -lhidapi                                         \
        -L"$$PWD/dependencies/mbedtls-3.2.1/lib/x86/" -lmbedcrypto -lmbedtls -lmbedx509         \
}

win32:DEFINES -=                                                                                \
    UNICODE

win32:DEFINES +=                                                                                \
    USE_HID_USAGE                                                                               \
    _MBCS                                                                                       \
    WIN32                                                                                       \
    _CRT_SECURE_NO_WARNINGS                                                                     \
    _WINSOCK_DEPRECATED_NO_WARNINGS                                                             \
    WIN32_LEAN_AND_MEAN                                                                         \

win32:RC_ICONS +=                                                                               \
    qt/OpenRGB.ico

#-----------------------------------------------------------------------------------------------#
# Windows GitLab CI Configuration                                                               #
#-----------------------------------------------------------------------------------------------#
win32:CONFIG(debug, debug|release) {
    win32:DESTDIR = debug
}

win32:CONFIG(release, debug|release) {
    win32:DESTDIR = release
}

win32:OBJECTS_DIR = _intermediate_$$DESTDIR/.obj
win32:MOC_DIR     = _intermediate_$$DESTDIR/.moc
win32:RCC_DIR     = _intermediate_$$DESTDIR/.qrc
win32:UI_DIR      = _intermediate_$$DESTDIR/.ui

#-----------------------------------------------------------------------------------------------#
# Copy dependencies to output directory                                                         #
#-----------------------------------------------------------------------------------------------#

win32:contains(QMAKE_TARGET.arch, x86_64) {
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/x64/WinRing0x64.dll                )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/x64/WinRing0x64.sys                )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/libusb-1.0.27/VS2019/MS64/dll/libusb-1.0.dll)\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/hidapi-win/x64/hidapi.dll                   )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

win32:contains(QMAKE_TARGET.arch, x86) {
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/Win32/WinRing0.dll                 )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/Win32/WinRing0.sys                 )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/winring0/x64/WinRing0x64.sys                )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/libusb-1.0.27/VS2019/MS32/dll/libusb-1.0.dll)\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)
    copydata.commands += $(COPY_FILE) \"$$shell_path($$PWD/dependencies/hidapi-win/x86/hidapi.dll                   )\" \"$$shell_path($$DESTDIR)\" $$escape_expand(\n\t)

    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}

#-----------------------------------------------------------------------------------------------#
# Linux-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
contains(QMAKE_PLATFORM, linux) {
    CONFIG += link_pkgconfig

    PKGCONFIG +=                                                                                \
    libusb-1.0

    TARGET = $$lower($$TARGET)

    HEADERS += $$CONTROLLER_H_LINUX

    HEADERS +=                                                                                  \
    dependencies/NVFC/nvapi.h                                                                   \
    i2c_smbus/i2c_smbus_linux.h                                                                 \
    AutoStart/AutoStart-Linux.h                                                                 \
    SPDAccessor/EE1004Accessor_Linux.h                                                          \
    SPDAccessor/SPD5118Accessor_Linux.h                                                         \
    SuspendResume/SuspendResume_Linux_FreeBSD.h                                                 \

    INCLUDEPATH +=                                                                              \
    dependencies/NVFC                                                                           \
    /usr/include/mbedtls/                                                                       \

    LIBS +=                                                                                     \
    -L/usr/lib/mbedtls/                                                                         \
    -lmbedx509                                                                                  \
    -lmbedtls                                                                                   \
    -lmbedcrypto                                                                                \
    -ldl                                                                                        \

    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    if (!versionAtLeast(COMPILER_VERSION, "9")) {
         LIBS += -lstdc++fs
    }

    QT += dbus

    QMAKE_CXXFLAGS += -Wno-implicit-fallthrough -Wno-psabi

    #-------------------------------------------------------------------------------------------#
    # Determine which hidapi to use based on availability                                       #
    #   Prefer hidraw backend, then libusb                                                      #
    #-------------------------------------------------------------------------------------------#
    packagesExist(hidapi-hidraw) {
        PKGCONFIG += hidapi-hidraw

        #---------------------------------------------------------------------------------------#
        # hidapi-hidraw >= 0.10.1 supports USAGE/USAGE_PAGE                                     #
        # Define USE_HID_USAGE if hidapi-hidraw supports it                                     #
        #---------------------------------------------------------------------------------------#
        HIDAPI_HIDRAW_VERSION = $$system($$PKG_CONFIG --modversion hidapi-hidraw)
        if(versionAtLeast(HIDAPI_HIDRAW_VERSION, "0.10.1")) {
            DEFINES += USE_HID_USAGE
        }
    } else {
        packagesExist(hidapi-libusb) {
            PKGCONFIG += hidapi-libusb
        } else {
            PKGCONFIG += hidapi
        }
    }

    SOURCES += $$CONTROLLER_CPP_LINUX

    SOURCES +=                                                                                  \
    dependencies/hueplusplus-1.2.0/src/LinHttpHandler.cpp                                       \
    dependencies/NVFC/nvapi.cpp                                                                 \
    i2c_smbus/i2c_smbus_linux.cpp                                                               \
    scsiapi/scsiapi_linux.c                                                                     \
    serial_port/find_usb_serial_port_linux.cpp                                                  \
    AutoStart/AutoStart-Linux.cpp                                                               \
    SPDAccessor/EE1004Accessor_Linux.cpp                                                        \
    SPDAccessor/SPD5118Accessor_Linux.cpp                                                       \
    SuspendResume/SuspendResume_Linux_FreeBSD.cpp                                               \
    startup/main_Linux_MacOS.cpp                                                                \

    #-------------------------------------------------------------------------------------------#
    # Set up install paths                                                                      #
    # These install paths are used for AppImage and .deb packaging                              #
    #-------------------------------------------------------------------------------------------#
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    !defined(OPENRGB_SYSTEM_PLUGIN_DIRECTORY, var):OPENRGB_SYSTEM_PLUGIN_DIRECTORY =            \
        "$$PREFIX/lib/openrgb/plugins"                                                          \

    DEFINES +=                                                                                  \
        OPENRGB_SYSTEM_PLUGIN_DIRECTORY=\\"\"\"$$OPENRGB_SYSTEM_PLUGIN_DIRECTORY\\"\"\"         \

    #-------------------------------------------------------------------------------------------#
    # Custom target for dynamically created udev_rules                                          #
    #   Ordinarily you would add the 'udev_rules' target to both QMAKE_EXTRA_TARGETS to add a   #
    #   rule in the Makefile and PRE_TARGETDEPS to ensure it is a dependency of the TARGET      #
    #                                                                                           #
    #   ie. QMAKE_EXTRA_TARGETS += udev_rules                                                   #
    #       PRE_TARGETDEPS      += udev_rules                                                   #
    #-------------------------------------------------------------------------------------------#
    CONFIG(release, debug|release) {
        udev_rules.CONFIG       = no_check_exist
        udev_rules.target       = 60-openrgb.rules
        udev_rules.path         = $$PREFIX/lib/udev/rules.d/

        exists($$udev_rules.target) {
            message($$udev_rules.target " - UDEV rules file exists. Removing from build")
            udev_rules.files    = $$udev_rules.target
        } else {
            message($$udev_rules.target " - UDEV rules file missing. Adding script to build")
            #-----------------------------------------------------------------------------------#
            # This is a compiler config flag to save the preproccessed .ii & .s                 #
            #   files so as to automatically process the UDEV rules and the Supported Devices   #
            #-----------------------------------------------------------------------------------#
            QMAKE_CXXFLAGS+=-save-temps
            QMAKE_CXXFLAGS-=-pipe
            udev_rules.extra    = $$PWD/scripts/build-udev-rules.sh $$PWD $$GIT_COMMIT_ID
            udev_rules.files    = $$OUT_PWD/60-openrgb.rules
        }
    }

    #-------------------------------------------------------------------------------------------#
    # Add static files to installation                                                          #
    #-------------------------------------------------------------------------------------------#
    target.path=$$PREFIX/bin/
    desktop.path=$$PREFIX/share/applications/
    desktop.files+=qt/org.openrgb.OpenRGB.desktop
    icon.path=$$PREFIX/share/icons/hicolor/128x128/apps/
    icon.files+=qt/org.openrgb.OpenRGB.png
    metainfo.path=$$PREFIX/share/metainfo/
    metainfo.files+=qt/org.openrgb.OpenRGB.metainfo.xml
    systemd_service.path=/etc/systemd/system
    systemd_service.files+=qt/openrgb.service
    INSTALLS += target desktop icon metainfo udev_rules systemd_service
}

#-----------------------------------------------------------------------------------------------#
# FreeBSD-specific Configuration                                                                #
#-----------------------------------------------------------------------------------------------#
contains(QMAKE_PLATFORM, freebsd) {
    CONFIG += link_pkgconfig

    PKGCONFIG +=                                                                                \
    libusb-1.0

    TARGET = $$lower($$TARGET)

    HEADERS += $$CONTROLLER_H_FREEBSD

    HEADERS +=                                                                                  \
    AutoStart/AutoStart-FreeBSD.h                                                               \
    SuspendResume/SuspendResume_Linux_FreeBSD.h                                                 \

    HEADERS -=                                                                                  \
    Controllers/SeagateController/RGBController_Seagate.h                                       \
    Controllers/SeagateController/SeagateController.h                                           \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_ROGArion.h               \
    $$CONTROLLER_H_WINDOWS                                                                      \

    LIBS +=                                                                                     \
    -lmbedx509                                                                                  \
    -lmbedtls                                                                                   \
    -lmbedcrypto                                                                                \

    COMPILER_VERSION = $$system($$QMAKE_CXX " -dumpversion")
    if (!versionAtLeast(COMPILER_VERSION, "9")) {
         LIBS += -lstdc++fs
    }

    QT += dbus

    #-------------------------------------------------------------------------------------------#
    # Determine which hidapi to use based on availability                                       #
    #   Prefer hidraw backend, then libusb                                                      #
    #-------------------------------------------------------------------------------------------#
    packagesExist(hidapi-hidraw) {
        PKGCONFIG += hidapi-hidraw

        #---------------------------------------------------------------------------------------#
        # hidapi-hidraw >= 0.10.1 supports USAGE/USAGE_PAGE                                     #
        # Define USE_HID_USAGE if hidapi-hidraw supports it                                     #
        #---------------------------------------------------------------------------------------#
        packagesExist(hidapi-hidraw>=0.10.1) {
            DEFINES += USE_HID_USAGE
        }
    } else {
        packagesExist(hidapi-libusb) {
            PKGCONFIG += hidapi-libusb
        } else {
            PKGCONFIG += hidapi
        }
    }

    SOURCES += $$CONTROLLER_CPP_FREEBSD

    SOURCES +=                                                                                  \
    dependencies/hueplusplus-1.2.0/src/LinHttpHandler.cpp                                       \
    serial_port/find_usb_serial_port_linux.cpp                                                  \
    AutoStart/AutoStart-FreeBSD.cpp                                                             \
    SuspendResume/SuspendResume_Linux_FreeBSD.cpp                                               \

    SOURCES -=                                                                                  \
    Controllers/SeagateController/RGBController_Seagate.cpp                                     \
    Controllers/SeagateController/SeagateController.cpp                                         \
    Controllers/SeagateController/SeagateControllerDetect.cpp                                   \
    Controllers/ENESMBusController/ROGArionDetect.cpp                                           \
    Controllers/ENESMBusController/ENESMBusInterface/ENESMBusInterface_ROGArion.cpp             \

    #-------------------------------------------------------------------------------------------#
    # Set up install paths                                                                      #
    # These install paths are used for AppImage and .deb packaging                              #
    #-------------------------------------------------------------------------------------------#
    isEmpty(PREFIX) {
        PREFIX = /usr
    }

    target.path=$$PREFIX/bin/
    desktop.path=$$PREFIX/share/applications/
    desktop.files+=qt/org.openrgb.OpenRGB.desktop
    icon.path=$$PREFIX/share/icons/hicolor/128x128/apps/
    icon.files+=qt/org.openrgb.OpenRGB.png
    metainfo.path=$$PREFIX/share/metainfo/
    metainfo.files+=qt/org.openrgb.OpenRGB.metainfo.xml
    rules.path=$$PREFIX/lib/udev/rules.d/
    rules.files+=60-openrgb.rules
    INSTALLS += target desktop icon metainfo rules
}

unix:!macx:CONFIG(asan) {
    message("ASan Mode")
    QMAKE_CFLAGS=-fsanitize=address
    QMAKE_CXXFLAGS=-fsanitize=address
    QMAKE_LFLAGS=-fsanitize=address
}

#-----------------------------------------------------------------------------------------------#
# MacOS-specific Configuration                                                                  #
#-----------------------------------------------------------------------------------------------#
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

#-----------------------------------------------------------------------------------------------#
# Common MacOS definitions                                                                      #
#-----------------------------------------------------------------------------------------------#
macx {
    CONFIG += link_pkgconfig

    PKGCONFIG +=                                                                                \
    libusb-1.0                                                                                  \
    hidapi

    DEFINES +=                                                                                  \
    USE_HID_USAGE                                                                               \

    QMAKE_CXXFLAGS +=                                                                           \
    -Wno-narrowing                                                                              \

    HEADERS +=                                                                                  \
    AutoStart/AutoStart-MacOS.h                                                                 \
    qt/macutils.h                                                                               \
    SuspendResume/SuspendResume_MacOS.h                                                         \

    HEADERS += $$CONTROLLER_H_MACOS

    SOURCES +=                                                                                  \
    dependencies/hueplusplus-1.2.0/src/LinHttpHandler.cpp                                       \
    serial_port/find_usb_serial_port_macos.cpp                                                  \
    AutoStart/AutoStart-MacOS.cpp                                                               \
    qt/macutils.mm                                                                              \
    SuspendResume/SuspendResume_MacOS.cpp                                                       \
    startup/main_Linux_MacOS.cpp                                                                \

    SOURCES += $$CONTROLLER_CPP_MACOS

    # Use mbedtls
    MBEDTLS_PREFIX = $$system(brew --prefix mbedtls)

    INCLUDEPATH +=                                                                              \
    $$MBEDTLS_PREFIX/include                                                                    \

    LIBS +=                                                                                     \
    -lmbedx509                                                                                  \
    -lmbedcrypto                                                                                \
    -lmbedtls                                                                                   \
    -L$$MBEDTLS_PREFIX/lib

    ICON = qt/OpenRGB.icns

    info_plist.input = mac/Info.plist.in
    info_plist.output = $$OUT_PWD/Info.plist
    QMAKE_SUBSTITUTES += info_plist
    QMAKE_INFO_PLIST = $$OUT_PWD/Info.plist
}

#-----------------------------------------------------------------------------------------------#
# Apple Silicon (arm64) Homebrew installs at /opt/homebrew                                      #
#-----------------------------------------------------------------------------------------------#
macx:contains(QMAKE_HOST.arch, arm64) {
    INCLUDEPATH +=                                                                              \
    /opt/homebrew/include                                                                       \

    SOURCES +=                                                                                  \
    scsiapi/scsiapi_macos.c                                                                     \

    LIBS +=                                                                                     \
    -L/opt/homebrew/lib                                                                         \
}

#-----------------------------------------------------------------------------------------------#
# Intel (x86_64) Homebrew installs at /usr/local/lib                                            #
#-----------------------------------------------------------------------------------------------#
macx:contains(QMAKE_HOST.arch, x86_64) {
    INCLUDEPATH +=                                                                              \
    dependencies/macUSPCIO                                                                      \
    /usr/local/include                                                                          \
    /usr/local/homebrew/include                                                                 \

    SOURCES +=                                                                                  \
    i2c_smbus/i2c_smbus_i801.cpp                                                                \
    i2c_smbus/i2c_smbus_nct6775.cpp                                                             \
    i2c_smbus/i2c_smbus_piix4.cpp                                                               \
    scsiapi/scsiapi_macos.c                                                                     \

    HEADERS +=                                                                                  \
    dependencies/macUSPCIO/macUSPCIOAccess.h                                                    \
    i2c_smbus/i2c_smbus_i801.h                                                                  \
    i2c_smbus/i2c_smbus_nct6775.h                                                               \
    i2c_smbus/i2c_smbus_piix4.h                                                                 \

    LIBS +=                                                                                     \
    -L/usr/local/lib                                                                            \
    -L/usr/local/homebrew/lib                                                                   \

    DEFINES +=                                                                                  \
    _MACOSX_X86_X64                                                                             \
}

DISTFILES += \
    debian/openrgb-udev.postinst \
    debian/openrgb.postinst

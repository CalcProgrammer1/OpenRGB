#!/usr/bin/env bash

#-----------------------------------------------------------------------#
# OpenRGB AppImage Build Script                                         #
#-----------------------------------------------------------------------#

set -x
set -e

#-----------------------------------------------------------------------#
# Build in a temporary directory to keep the system clean               #
# Use RAM disk if possible (if available and not building on a CI       #
# system like Travis)                                                   #
#-----------------------------------------------------------------------#
TEMP_BASE=/tmp
BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" appimage-build-XXXXXX)

#-----------------------------------------------------------------------#
# Setup environment                                                     #
#-----------------------------------------------------------------------#
export APPIMAGE_EXTRACT_AND_RUN=1

if [ "$1" = "qt6" ]; then
    export QT_SELECT=qt6
else
    export QT_SELECT=qt5
fi

if [ "$QT_SELECT" = "qt6" ]; then
    export QMAKE=qmake6
else
    export QMAKE=qmake
fi

#-----------------------------------------------------------------------#
# This checks the Architecture of the system to work out if we're       #
#     building on i386 or x86_64 and saves for later use                #
#-----------------------------------------------------------------------#

if [ ${DEB_HOST_ARCH:0:1} == ${DEB_HOST_GNU_CPU:0:1} ]; then
    ARCH="$DEB_HOST_ARCH"
else
    ARCH="$DEB_HOST_GNU_CPU"
fi

if [ "$ARCH" == "arm64" ]; then
    ARCH_LINUXDEPLOY="aarch64"
else
    ARCH_LINUXDEPLOY="$ARCH"
fi

echo Inputs: "$DEB_HOST_ARCH" "$DEB_HOST_GNU_CPU"
echo Calculated: "$ARCH"

#-----------------------------------------------------------------------#
# Small fixes for CamelCase vs lowercase                                #
#-----------------------------------------------------------------------#
TARGET=OpenRGB
patch -p1 -i scripts/AppImage.patch

#-----------------------------------------------------------------------#
# Make sure to clean up build dir, even if errors occur                 #
#-----------------------------------------------------------------------#
cleanup () {
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
    fi
}
trap cleanup EXIT

#-----------------------------------------------------------------------#
# Store repo root as variable                                           #
#-----------------------------------------------------------------------#
REPO_ROOT=$(readlink -f $(dirname $(dirname $0)))
OLD_CWD=$(readlink -f .)

#-----------------------------------------------------------------------#
# Switch to build dir                                                   #
#-----------------------------------------------------------------------#
pushd "$BUILD_DIR"

#-----------------------------------------------------------------------#
# Configure build files with qmake                                      #
# we need to explicitly set the install prefix, as qmake's default is   #
# /usr/local for some reason...                                         #
#-----------------------------------------------------------------------#
$QMAKE "$REPO_ROOT"

#-----------------------------------------------------------------------#
# Build project and install files into AppDir                           #
#-----------------------------------------------------------------------#
make -j$(nproc) TARGET="$TARGET"
make install INSTALL_ROOT=AppDir

#-----------------------------------------------------------------------#
# Make sure Qt plugin finds QML sources so it can deploy the imported   #
# files.  Also deploy Wayland support and extra modules used by plugins #
#-----------------------------------------------------------------------#
export QML_SOURCES_PATHS="$REPO_ROOT"/src

#-----------------------------------------------------------------------#
# We need to bundle the libhidapi-libusb library for wrapped HID        #
# devices, find its path using ldconfig                                 #
#-----------------------------------------------------------------------#
LIBHIDAPI_LIBUSB_PATH=$(find /usr/lib -name "libhidapi-hotplug-libusb.so")
if [ ! -f $LIBHIDAPI_LIBUSB_PATH ]; then
    LIBHIDAPI_LIBUSB_PATH=$(find /usr/lib -name "libhidapi-libusb.so")
fi

#-----------------------------------------------------------------------#
# Create custom AppRun script to set environment variables for Qt       #
# plugins to find bundled libraries (handles both Qt5 and Qt6)          #
#-----------------------------------------------------------------------#
cat > AppRun << 'APPRUN_EOF'
#!/bin/bash

# Get the absolute path to the AppDir
HERE="$(cd "$(dirname "$(readlink -f "${0}")")" && pwd)"

# Set LD_LIBRARY_PATH so plugins can find bundled Qt libraries
export LD_LIBRARY_PATH="${HERE}/usr/lib"

# Execute the main binary
exec "${HERE}/usr/bin/OpenRGB" "$@"
APPRUN_EOF

chmod +x AppRun

if [ "$QT_SELECT" = "qt6" ]; then
    QT6_OPENGL_PATH=$(find /usr/lib -name "libQt6OpenGL.so.6")
    QT6_CORE5COMPAT_PATH=$(find /usr/lib -name "libQt6Core5Compat.so.6")
    linuxdeploy-"$ARCH_LINUXDEPLOY".AppImage --appdir AppDir --executable "$TARGET" --custom-apprun AppRun --icon-file "$REPO_ROOT"/qt/org.openrgb.OpenRGB.png --desktop-file "$REPO_ROOT"/qt/org.openrgb.OpenRGB.desktop --library "$LIBHIDAPI_LIBUSB_PATH" --library "$QT6_OPENGL_PATH" --library "$QT6_CORE5COMPAT_PATH"
else
    linuxdeploy-"$ARCH_LINUXDEPLOY".AppImage --appdir AppDir --executable "$TARGET" --custom-apprun AppRun --icon-file "$REPO_ROOT"/qt/org.openrgb.OpenRGB.png --desktop-file "$REPO_ROOT"/qt/org.openrgb.OpenRGB.desktop --library "$LIBHIDAPI_LIBUSB_PATH"
fi

linuxdeploy-plugin-qt-"$ARCH_LINUXDEPLOY".AppImage --appdir AppDir
linuxdeploy-"$ARCH_LINUXDEPLOY".AppImage --appdir AppDir --output appimage

#-----------------------------------------------------------------------#
# Move built AppImage & udev_rules back into original CWD               #
#-----------------------------------------------------------------------#
mv -v "$BUILD_DIR"/60-openrgb.rules "$OLD_CWD"
mv -v "$TARGET"*.AppImage "$OLD_CWD/OpenRGB-$ARCH.AppImage"

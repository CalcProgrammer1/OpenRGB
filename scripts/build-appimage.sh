#!/bin/bash

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
if [ "$CI" == "" ] && [ -d /dev/shm ]; then
    TEMP_BASE=/dev/shm
else
    TEMP_BASE=/tmp
fi

BUILD_DIR=$(mktemp -d -p "$TEMP_BASE" appimage-build-XXXXXX)

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
qmake "$REPO_ROOT"

#-----------------------------------------------------------------------#
# Build project and install files into AppDir                           #
#-----------------------------------------------------------------------#
make -j$(nproc)
make install INSTALL_ROOT=AppDir

#-----------------------------------------------------------------------#
# Now, build AppImage using linuxdeploy and linuxdeploy-plugin-qt       #
# Download linuxdeploy and its Qt plugin                                #
#-----------------------------------------------------------------------#
wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

#-----------------------------------------------------------------------#
# Make them executable                                                  #
#-----------------------------------------------------------------------#
chmod +x linuxdeploy*.AppImage

#-----------------------------------------------------------------------#
# Make sure Qt plugin finds QML sources so it can deploy the imported   #
# files                                                                 #
export QML_SOURCES_PATHS="$REPO_ROOT"/src

./linuxdeploy-x86_64.AppImage --appimage-extract-and-run --appdir AppDir -e OpenRGB -i "$REPO_ROOT"/qt/OpenRGB.png -d "$REPO_ROOT"/qt/OpenRGB.desktop 
./linuxdeploy-plugin-qt-x86_64.AppImage --appimage-extract-and-run --appdir AppDir
./linuxdeploy-x86_64.AppImage --appimage-extract-and-run --appdir AppDir --output appimage

#-----------------------------------------------------------------------#
# Move built AppImage back into original CWD                            #
#-----------------------------------------------------------------------#
mv OpenRGB*.AppImage "$OLD_CWD"


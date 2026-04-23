#!/usr/bin/env bash

#-----------------------------------------------------------------------#
# OpenRGB MacOS Build Script                                            #
#-----------------------------------------------------------------------#

#-----------------------------------------------------------------------#
# Setup environment                                                     #
#-----------------------------------------------------------------------#
export APPIMAGE_EXTRACT_AND_RUN=1

if   [ "$1" = "qt6" ] && [ "$2" = "arm" ]; then
    export ARCH=""
    export BREW_PATH=/opt/homebrew
    export QT_PATH=bin
elif [ "$1" = "qt6" ] && [ "$2" = "intel" ]; then
    export ARCH="arch -x86_64"
    export BREW_PATH=/usr/local
    export QT_PATH=bin
elif [ "$1" = "qt5" ] && [ "$2" = "arm" ]; then
    export ARCH=""
    export BREW_PATH=/opt/homebrew
    export QT_PATH=opt/qt@5/bin
elif [ "$1" = "qt5" ] && [ "$2" = "intel" ]; then
    export ARCH="arch -x86_64"
    export BREW_PATH=/usr/local
    export QT_PATH=opt/qt@5/bin
else
    echo "Invalid arguments, specify qt5 or qt6 and arm or intel"
    echo "Example: ./build-macos.sh qt6 arm"
fi

eval $($BREW_PATH/bin/brew shellenv)
$ARCH $BREW_PATH/$QT_PATH/qmake OpenRGB.pro
$ARCH make -j$(sysctl -n hw.ncpu)

if [ "$1" = "qt5" ]; then
    mkdir -p OpenRGB.app/Contents/Frameworks
    cp $BREW_PATH/lib/libsharpyuv.0.dylib OpenRGB.app/Contents/Frameworks
    install_name_tool -change @rpath/libsharpyuv.0.dylib @executable_path/../Frameworks/libsharpyuv.0.dylib OpenRGB.app/Contents/MacOS/OpenRGB
fi

$ARCH $BREW_PATH/$QT_PATH/macdeployqt OpenRGB.app -codesign=OpenRGB

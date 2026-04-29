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

#-----------------------------------------------------------------------#
# Build OpenRGB                                                         #
#-----------------------------------------------------------------------#
eval $($BREW_PATH/bin/brew shellenv)
$ARCH $BREW_PATH/$QT_PATH/qmake OpenRGB.pro
$ARCH make -j$(sysctl -n hw.ncpu)

#-----------------------------------------------------------------------#
# Pre-deploy quirks for Qt5 builds                                      #
#   * Manually deploy libsharpyuv.0.dylib as macdeployqt fails to do it #
#     automatically                                                     #
#   * Manually deploy QtOpenGL framework used by the Effects plugin     #
#-----------------------------------------------------------------------#
if [ "$1" = "qt5" ]; then
    mkdir -p OpenRGB.app/Contents/Frameworks

    cp $BREW_PATH/lib/libsharpyuv.0.dylib OpenRGB.app/Contents/Frameworks
    install_name_tool -change @rpath/libsharpyuv.0.dylib @executable_path/../Frameworks/libsharpyuv.0.dylib OpenRGB.app/Contents/MacOS/OpenRGB

    cp -R $BREW_PATH/opt/qt@5/lib/QtOpenGL.framework OpenRGB.app/Contents/Frameworks
    install_name_tool -change $BREW_PATH/opt/qt@5/lib/QtOpenGL.framework/Versions/5/QtOpenGL @executable_path/../Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL OpenRGB.app/Contents/MacOS/OpenRGB
fi

#-----------------------------------------------------------------------#
# Pre-deploy quirks for Qt6 builds                                      #
#   * Manually deploy QtOpenGL framework used by the Effects plugin     #
#   * Manually deploy QtCore5Compat framework used by the Effects       #
#     plugin                                                            #
#-----------------------------------------------------------------------#
if [ "$1" = "qt6" ]; then
    mkdir -p OpenRGB.app/Contents/Frameworks

    cp -R $BREW_PATH/opt/qtbase/lib/QtOpenGL.framework OpenRGB.app/Contents/Frameworks
    install_name_tool -change $BREW_PATH/opt/qtbase/lib/QtOpenGL.framework/Versions/A/QtOpenGL @executable_path/../Frameworks/QtOpenGL.framework/Versions/A/QtOpenGL OpenRGB.app/Contents/MacOS/OpenRGB

    cp -R $BREW_PATH/opt/qt5compat/lib/QtCore5Compat.framework OpenRGB.app/Contents/Frameworks
    install_name_tool -change $BREW_PATH/opt/qt5compat/lib/QtCore5Compat.framework/Versions/A/QtCore5Compat @executable_path/../Frameworks/QtCore5Compat.framework/Versions/A/QtCore5Compat OpenRGB.app/Contents/MacOS/OpenRGB
fi

#-----------------------------------------------------------------------#
# Run macdeploy to automatically deploy frameworks and sign the code    #
#-----------------------------------------------------------------------#
$ARCH $BREW_PATH/$QT_PATH/macdeployqt OpenRGB.app -codesign=OpenRGB

#-----------------------------------------------------------------------#
# Post-deploy quirks for Qt6 builds                                     #
#   * Run macdeploy again so that it deploys frameworks for default Qt  #
#     plugins, as it deploys the plugins without their dependent        #
#     frameworks the first time                                         #
#-----------------------------------------------------------------------#
if [ "$1" = "qt6" ]; then
    $ARCH $BREW_PATH/$QT_PATH/macdeployqt OpenRGB.app -codesign=OpenRGB
fi

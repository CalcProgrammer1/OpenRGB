# Compiling

This document details the process to compile OpenRGB from source on supported operating systems.

## Windows

  *  You will need the **Microsoft Visual 2019 C++ runtime** installed.  You can get it [here](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads)
  *  To build the application yourself on Windows:
      1. Download and install [PowerShell](https://learn.microsoft.com/en-us/powershell/scripting/install/installing-powershell-on-windows). Note that it's not the same as Windows Powershell.
      2. Download the latest [Visual Studio Community Edition](https://visualstudio.microsoft.com/thank-you-for-downloading-visual-studio-for-cplusplus/?sku=Community) and [Qt Creator](https://www.qt.io/download-thank-you).
      3. When [installing the QT toolset](https://gitlab.com/OpenRGBDevelopers/OpenRGB-Wiki/-/blob/stable/Downloading%20Qt%20and%20building%20OpenRGB.md) select the latest revision of Qt 5.15.x as OpenRGB is not yet compatible with QT6.
      4. Optionally [install Git](https://git-scm.com/download) if you intend to [contribute your changes](https://gitlab.com/CalcProgrammer1/OpenRGB/-/blob/master/CONTRIBUTING.md) to the mainline codebase.
      5. Open the OpenRGB.pro project in Qt Creator.
      6. Use the MSVC compiler kit, either 32- or 64-bit, to build the application.
      7. Run the project from Qt Creator.  If you want to use your custom build standalone, download the latest matching Release package and replace the OpenRGB.exe in it with your new build.

## Linux

  1. Install build dependencies
      - Debian/Ubuntu: `sudo apt install git build-essential qtcreator qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools libusb-1.0-0-dev libhidapi-dev pkgconf libmbedtls-dev qttools5-dev-tools`
      - Fedora: `sudo dnf install automake gcc-c++ git hidapi-devel libusbx-devel mbedtls-devel pkgconf qt5-qtbase-devel qt5-linguist`
  2. `git clone https://gitlab.com/CalcProgrammer1/OpenRGB`
  3. `cd OpenRGB`
  4. `mkdir build`
  5. `cd build`
  4. `qmake ../OpenRGB.pro`
  5. `make -j$(nproc)`
  6. You can then run the application from the compile directory with `./openrgb` or install with `make install`
  7. You will also need to [install the latest udev rules](UdevRules.md).

#### Packaging

You can also build OpenRGB generic AppImage packages and distribution-specific packages for Debian-based and Fedora-based distros.  Install the build dependencies from the section above for your distribution before proceeding.

  * AppImage:

  * Debian/Ubuntu:
    * Make sure OpenRGB is cloned in ~/OpenRGB before proceeding.  Output .deb is in ~/.
    1. `sudo apt install debhelper`
    2. `cd ~/OpenRGB`
    3. `scripts/build-package-files.sh debian/changelog`
    4. `dpkg-buildpackage -us -B`

  * Fedora:
    * Make sure OpenRGB is cloned in ~/OpenRGB before proceeding.  Output .rpm is in ~/rpmbuild/RPMS/.
    1.  `sudo dnf install rpmdevtools dnf-plugins-core`
    2.  `cd ~/`
    3.  `rpmdev-setuptree`
    4.  `tar -cf rpmbuild/SOURCES/OpenRGB.tar.gz OpenRGB/`
    4.  `cd OpenRGB`
    5.  `./scripts/build-package-files.sh fedora/OpenRGB.spec`
    6.  `cd ~/`
    7.  `cp OpenRGB/fedora/OpenRGB.spec rpmbuild/SPECS/`
    8.  `sudo dnf builddep rpmbuild/SPECS/OpenRGB.spec -y`
    9.  `cd rpmbuild/SOURCES`
    10. `tar -xf OpenRGB.tar.gz`
    11. `cd ~/`
    12. `rpmbuild -ba rpmbuild/SPECS/OpenRGB.spec`

## MacOS

  1. Install build dependencies with Homebrew
      - Install Homebrew by following the instructions at https://brew.sh/
       - `brew install git qt5 hidapi libusb mbedtls@2`
       - `brew link qt5`
  2. [Create a local certificate](https://support.apple.com/guide/keychain-access/create-self-signed-certificates-kyca8916/mac) called OpenRGB with code signing capability
  3. git clone https://gitlab.com/CalcProgrammer1/OpenRGB
  4. cd OpenRGB
  5. qmake OpenRGB.pro
  6. make -j8
  7. macdeployqt OpenRGB.app -codesign=OpenRGB
  8. Copy the OpenRGB.app application package to Applications

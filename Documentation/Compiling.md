# Compiling

This document details the process to compile OpenRGB from source on supported operating systems.

## Windows

  *  You will need the **Microsoft Visual 2019 C++ runtime** installed.  You can get it [here](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist)
  *  To build the application yourself on Windows:
      1. [Install Git](https://git-scm.com/download)
      2. Clone the [OpenRGB-Qt-Packages](https://gitlab.com/OpenRGBDevelopers/OpenRGB-Qt-Packages) git repo and run `install.bat` (or optionally `install-chocolatey.bat` if you use the Chocolatey package manager).
      3. In the OpenRGB source directory, run the `scripts\build-windows.bat` file with arguments `<Qt Version> <MSVC Version> <Bits>`.
        * Qt versions provided by `OpenRGB-Qt-Packages` include `5.15.0` (using MSVC `2019`) and `6.8.3` (using MSVC `2022`).
        * For example, for a Qt5 64-bit build, `.\scripts\build-windows.bat 5.15.0 2019 64`
      4. You can also use Qt Creator to build and debug the project, you will need to install it from the Qt Online Installer or by downloading and extracting a binary release and manually configuring it.

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

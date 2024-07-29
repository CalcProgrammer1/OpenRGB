# Udev Rules

On Linux, OpenRGB provides a udev rules file to configure access permissions to supported devices.

If you install OpenRGB through a distribution-specific package, whether provided by your distribution's official repositories, from packages downloaded from OpenRGB's website or GitLab CI, or from building packages yourself, the udev rules should be installed as part of that package.  You should not need to manually install them.

If you are using OpenRGB compiled from source (not as part of a package), using OpenRGB as an AppImage, or using OpenRGB from Flatpak, you will need to install the udev rules manually.

## Installation

  * If you have installed OpenRGB from a package then latest udev rules are installed locally at `/usr/lib/udev/rules.d/60-openrgb.rules`
  * Udev rules are built from the source at compile time. When building locally they are installed with the `make install` step to `/usr/lib/udev/rules.d/60-openrgb.rules`
  * If you need to install the udev rules file manually you can also download the [latest compiled udev rules](https://gitlab.com/CalcProgrammer1/OpenRGB/-/jobs/artifacts/master/raw/60-openrgb.rules?job=Linux+64+AppImage&inline=false) from Gitlab.
    - Copy this 60-openrgb.rules file to `/usr/lib/udev/rules.d/`
    - Then reload rules with `sudo udevadm control --reload-rules && sudo udevadm trigger`
  * There is also a [udev rules installation script available at openrgb.org](https://openrgb.org/udev.html).

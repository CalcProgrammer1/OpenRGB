
<!--
Please put "Initial commit for" name of the product, including manufacturer above this line
  and a description of the additions / removals / changes done below the line.
-->




<!-- For admin purposes: Please leave this section as is -->

# Checklist for Accepting a Merge Request for a New Device
- [ ] The source branch of the merge request is not protected (`master` is protected by default when creating a fork, so it is recommended to not use it as your source).
- [ ] The `New Device` issue raised for this device is linked to this MR with a keyword `Closes / Resolves / Implements`
- [ ] There is a device protocol page in the [Developer Wiki](https://gitlab.com/OpenRGBDevelopers/OpenRGB-Wiki) or there is enough information / captures in the `New Device` issue to provide ongoing support.
- [ ] The code to be merged follows the style guide and change requirements as [documented in the contributing guide](https://gitlab.com/CalcProgrammer1/OpenRGB/-/blob/master/CONTRIBUTING.md).
<!--
The metadata for this controller is required to produce both the udev permissions and
 the supported device page for this device. A stub can be found in the RGBController_Dummy.cpp
-->
- [ ] Meta data for the device is included in `RGBController_*` file
- [ ] This device is detected and is working on Windows 10 and / or 11
- [ ] This device is detected and is working on Linux (Please specify distribution and releases tested)
- [ ] Logging for Info, Warnings and Errors has been added for troubleshooting purposes
<!-- For admin purposes: Please leave this section as is -->

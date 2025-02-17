<!-- 
When naming the support request please title the request as 
`[New Device] <Name of new device>` 
Please open one issue per device you would like to add
-->

### Name of device:
<!--
Please put the name of the product, including manufacturer, beneath this line
-->


### Link to manufacturer's product page:
<!--
Please add a link to the manufacturer's product page beneath this line
-->


### Please select what type of device/interface the device uses:
<!-- Please select from one of the following
 This determines how the device connects to the PC -->

~"DeviceType::IDK" <!-- I Don't know -->
~"DeviceType::USB" <!-- The device connects to an internal header or external usb port -->
~"DeviceType::GPU::AMD" <!-- The device is an AMD GPU -->
~"DeviceType::GPU::Nvidia" <!-- The device is a Nvidia GPU -->
~"DeviceType::SMBus" <!-- The device is connected to the SMBus eg. RAM -->
~"DeviceType::WMI"  <!-- The device is controlled through Windows Management Instrumentation -->

<!-- Please delete any lines that are not relevant -->

### ID information:
<!--
For PCI (GPU) devices we will need the Vendor ID, Device ID, Sub-Vendor ID and Sub-Device IDs
To get the Device ID formation for a GPU on Windows run the following command in Powershell:

wmic path Win32_VideoController get name,PNPDeviceID

Linux this can be found using the terminal:

lspci -d 1002: -nnvm | head -6 | tail -n 4 && lspci -d 10DE: -nnvm | head -6 | tail -n 4
-->

<!--
For USB devices we will need the USB VID and PID
Windows Powershell:

gwmi Win32_USBControllerDevice |%{[wmi]($_.Dependent)} | Sort Manufacturer,Description,DeviceID | Ft -GroupBy Manufacturer Description,Service,DeviceID

Linux Terminal:
lsusb
-->


### Please attach screenshots of the device's official control application here:
<!--
Screenshots of the official control software should show lists of supported modes, color selection, and zone/LED selection capabilities of the device's official software.
-->


### Please attach device captures here:
<!-- If you have code examples from other projects please link them here or alternatively
for information on how to capture device packets please refer to the wiki article
https://gitlab.com/Dr_No/OpenRGB/-/wikis/OpenRGB-doesn%27t-have-my-device
-->









<!-- For admin purposes: Please leave this section as is -->
/label ~"Issue Type - New Device"
/label ~"NewDevice::Step0 - Unconfirmed"

# Checklist for Step2
- [ ] Name of device
- [ ] A link to the vendors product page has been included
- [ ] The transport bus has been identified and the appropriate label added to the issue.
- [ ] The device ID's have been included for [USB](https://gitlab.com/Dr_No/OpenRGB/-/wikis/USB-Vendor-Identification-and-Product-Identification) or PCI
- [ ] Screenshots of the OEM Application are included
- [ ] There is either, appropriate code examples linked or suitable device captures attached
<!-- For admin purposes: Please leave this section as is -->

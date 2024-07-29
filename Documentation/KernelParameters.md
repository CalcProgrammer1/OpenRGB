# Kernel Parameters

  * To resolve an ACPI conflict add the `acpi_enforce_resources=lax` kernel parameter.
  * If you want to check if the kernel was loaded with this option you can execute this command from the terminal once you've rebooted: `cat /proc/cmdline`.

### Arch Linux
  
  * Please see [the Arch wiki](https://wiki.archlinux.org/title/kernel_parameters) for details on how to update your bootloader.

### Debian/Ubuntu

  * Please see [the Ubuntu Documentation](https://wiki.ubuntu.com/Kernel/KernelBootParameters) for Kernel Parameters for more information on updating your boot parameters.

### Fedora

  * On Fedora, install `grubby` and then following command: `grubby --update-kernel=ALL --args="acpi_enforce_resources=lax"`.
  * For more information please refer to the Fedora docs for [grubby](https://docs.fedoraproject.org/en-US/fedora/latest/system-administrators-guide/kernel-module-driver-configuration/Working_with_the_GRUB_2_Boot_Loader/#sec-Making_Persistent_Changes_to_a_GRUB_2_Menu_Using_the_grubby_Tool).

#! /bin/bash

# Download udev rules file
wget https://openrgb.org/releases/release_0.9/60-openrgb.rules

# Check if sudo is available
if command -v sudo &>/dev/null; then
    PRIVILEGED_CMD="sudo"
else
    echo "sudo not found. Falling back to run0"
    PRIVILEGED_CMD="run0 -i"
fi

# If we don't have write permissions then we're running on an immutable distro
if PRIVILEGED_CMD test -w /usr/lib/udev/rules.d;
then
	# Move udev rules file to udev rules directory
	PRIVILEGED_CMD mv 60-openrgb.rules /usr/lib/udev/rules.d
else
	PRIVILEGED_CMD 60-openrgb.rules /etc/udev/rules.d
fi

# Reload the rules
PRIVILEGED_CMD udevadm control --reload-rules
PRIVILEGED_CMD udevadm trigger

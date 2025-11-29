#! /bin/bash

# Download udev rules file
wget https://openrgb.org/releases/release_0.9/60-openrgb.rules

# If we don't have write permissions then we're running on an immutable distro
if sudo test -w /usr/lib/udev/rules.d;
then
	# Move udev rules file to udev rules directory
	sudo mv 60-openrgb.rules /usr/lib/udev/rules.d
else
	sudo mv 60-openrgb.rules /etc/udev/rules.d
fi

# Reload the rules
sudo udevadm control --reload-rules
sudo udevadm trigger

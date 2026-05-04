#!/bin/bash

# Check if sudo is available
if command -v sudo &>/dev/null; then
    PRIVILEGED_CMD="sudo"
else
    echo "sudo not found. Falling back to run0"
    PRIVILEGED_CMD="run0 -i"
fi

# Run all privileged operations in one batch
$PRIVILEGED_CMD bash <<'EOF'
set -e

# Determine target directory
if test -w /usr/lib/udev/rules.d; then
    TARGET_DIR="/usr/lib/udev/rules.d"
else
    TARGET_DIR="/etc/udev/rules.d"
fi

# Download udev rules file
wget -O "$TARGET_DIR/60-openrgb.rules" \
    https://openrgb.org/releases/release_0.9/60-openrgb.rules

# Reload the rules
udevadm control --reload-rules
udevadm trigger

echo "Done. Rules installed to $TARGET_DIR"
EOF

#!/usr/bin/env python3
"""
TUXEDO Keyboard LED Interface Helper for OpenRGB Profiles

============================================================================
IMPORTANT: This is NOT part of OpenRGB itself!
============================================================================

This script is a helper utility specifically for TUXEDO/Clevo laptops
with per-key RGB keyboards using the tuxedo_keyboard kernel driver.

Purpose:
--------
Convert OpenRGB .orp profile files to the Linux kernel LED interface format
used by the tuxedo_keyboard driver. This allows applying OpenRGB profiles
through the kernel's /sys/class/leds interface instead of USB HID.

Why This Exists:
---------------
On TUXEDO laptops, the Embedded Controller (EC) can turn off the keyboard
backlight after idle periods to save power. When it wakes up, it uses firmware
default colors, not OpenRGB's USB HID settings.

The kernel LED interface has better integration with the EC and may provide
more persistent color storage. This script bridges the gap between OpenRGB
profiles and the kernel interface.

Hardware Compatibility:
----------------------
- TUXEDO laptops with Clevo per-key RGB keyboards (ITE 8291 controller)
- Linux with tuxedo_keyboard driver (kernel module)
- NOT compatible with other RGB hardware

Usage:
------
    python3 tuxedo_keyboard_interface_helper.py

The script will:
1. Read ~/.config/OpenRGB/<profile>.orp (default: ddd.orp)
2. Convert colors using the hardware LED mapping from OpenRGB's ClevoKeyboard controller
3. Apply colors to /sys/class/leds/rgb:kbd_backlight_*/multi_intensity

File Location:
--------------
Place this script in your OpenRGB repository root or in a tools/ directory.
It is NOT meant to be integrated into OpenRGB's build system.

Notes:
------
- This uses the SAME LED mapping as OpenRGB's RGBController_ClevoKeyboard
- Hardware LED indices (0-125) map directly to kernel zone numbers
- The kernel interface is: /sys/class/leds/rgb:kbd_backlight_<zone>/multi_intensity
- Each zone accepts "R G B" format (e.g., "255 128 0")

For more information on the EC timeout issue, see:
https://gitlab.com/tuxedocomputers/development/packages/tuxedo-keyboard

Author: Helper script for TUXEDO hardware community
License: GPL-2.0-or-later (same as OpenRGB)
============================================================================
"""

import sys
import os

# Hardware LED mapping from OpenRGB's RGBController_ClevoKeyboard
# Source: Controllers/ClevoKeyboardController/RGBController_ClevoKeyboard.cpp
# This mapping translates key names to hardware LED indices (0-125)
# The kernel LED zones use the SAME numbering as hardware LEDs
HW_LED_MAP = {
    # F-row
    "Key: Escape": 105,
    "Key: F1": 106, "Key: F2": 107, "Key: F3": 108, "Key: F4": 109,
    "Key: F5": 110, "Key: F6": 111, "Key: F7": 112, "Key: F8": 113,
    "Key: F9": 114, "Key: F10": 115, "Key: F11": 116, "Key: F12": 117,
    "Key: Print Screen": 118,
    # Number row
    "Key: `": 84,
    "Key: 1": 85, "Key: 2": 86, "Key: 3": 87, "Key: 4": 88,
    "Key: 5": 89, "Key: 6": 90, "Key: 7": 91, "Key: 8": 92,
    "Key: 9": 93, "Key: 0": 94,
    "Key: -": 95, "Key: =": 96, "Key: Backspace": 98,
    "Key: Insert": 119, "Key: Home": 121, "Key: Page Up": 123,
    "Key: Delete": 120, "Key: End": 122, "Key: Page Down": 124,
    # QWERTY row
    "Key: Tab": 63,
    "Key: Q": 65, "Key: W": 66, "Key: E": 67, "Key: R": 68,
    "Key: T": 69, "Key: Y": 70, "Key: U": 71, "Key: I": 72,
    "Key: O": 73, "Key: P": 74,
    "Key: [": 75, "Key: ]": 76,
    # ASDF row
    "Key: Caps Lock": 42,
    "Key: A": 44, "Key: S": 45, "Key: D": 46, "Key: F": 47,
    "Key: G": 48, "Key: H": 49, "Key: J": 50, "Key: K": 51,
    "Key: L": 52, "Key: ;": 53, "Key: '": 54, "Key: #": 55,
    "Key: Enter": 77,
    # ZXCV row
    "Key: Left Shift": 22,
    "Key: \\ (ISO)": 23,
    "Key: Z": 24, "Key: X": 25, "Key: C": 26, "Key: V": 27,
    "Key: B": 28, "Key: N": 29, "Key: M": 30,
    "Key: ,": 31, "Key: .": 32, "Key: /": 33,
    "Key: Right Shift": 35,
    # Modifiers
    "Key: Left Control": 0, "Key: Left Fn": 2, "Key: Left Windows": 3,
    "Key: Left Alt": 4, "Key: Space": 7, "Key: Right Alt": 10,
    "Key: Right Control": 12,
    # Arrows
    "Key: Up Arrow": 14, "Key: Left Arrow": 13,
    "Key: Down Arrow": 18, "Key: Right Arrow": 15,
    # Numpad
    "Key: Num Lock": 99,
    "Key: Number Pad /": 100, "Key: Number Pad *": 101, "Key: Number Pad -": 102,
    "Key: Number Pad +": 81,
    "Key: Number Pad 7": 78, "Key: Number Pad 8": 79, "Key: Number Pad 9": 80,
    "Key: Number Pad 6": 59,
    "Key: Number Pad 4": 57, "Key: Number Pad 5": 58,
    "Key: Number Pad 1": 36, "Key: Number Pad 2": 37, "Key: Number Pad 3": 38,
    "Key: Number Pad Enter": 39,
    "Key: Number Pad 0": 16, "Key: Number Pad .": 17
}

# OpenRGB LED list order from RGBController_ClevoKeyboard
# This is the order LEDs appear in OpenRGB's GUI and .orp files
OPENRGB_LED_NAMES = [
    "Key: Escape", "Key: F1", "Key: F2", "Key: F3", "Key: F4", "Key: F5", "Key: F6",
    "Key: F7", "Key: F8", "Key: F9", "Key: F10", "Key: F11", "Key: F12", "Key: Print Screen",
    "Key: `", "Key: 1", "Key: 2", "Key: 3", "Key: 4", "Key: 5", "Key: 6", "Key: 7", "Key: 8",
    "Key: 9", "Key: 0", "Key: -", "Key: =", "Key: Backspace",
    "Key: Insert", "Key: Num Lock", "Key: Number Pad /", "Key: Number Pad *", "Key: Number Pad -",
    "Key: Home", "Key: Page Up",
    "Key: Tab", "Key: Q", "Key: W", "Key: E", "Key: R", "Key: T", "Key: Y", "Key: U",
    "Key: I", "Key: O", "Key: P", "Key: [", "Key: ]", "Key: Delete",
    "Key: Number Pad 7", "Key: Number Pad 8", "Key: Number Pad 9", "Key: Number Pad +",
    "Key: End", "Key: Page Down",
    "Key: Caps Lock", "Key: A", "Key: S", "Key: D", "Key: F", "Key: G", "Key: H", "Key: J",
    "Key: K", "Key: L", "Key: ;", "Key: '", "Key: #", "Key: Enter",
    "Key: Number Pad 4", "Key: Number Pad 5", "Key: Number Pad 6",
    "Key: Left Shift", "Key: \\ (ISO)", "Key: Z", "Key: X", "Key: C", "Key: V", "Key: B",
    "Key: N", "Key: M", "Key: ,", "Key: .", "Key: /", "Key: Right Shift",
    "Key: Number Pad 1", "Key: Number Pad 2", "Key: Number Pad 3", "Key: Number Pad Enter", "Key: Up Arrow",
    "Key: Left Control", "Key: Left Fn", "Key: Left Windows", "Key: Left Alt", "Key: Space",
    "Key: Right Alt", "Key: Right Control", "Key: Left Arrow", "Key: Down Arrow",
    "Key: Number Pad 0", "Key: Number Pad .", "Key: Right Arrow"
]

def parse_orp_file(filepath):
    """
    Parse OpenRGB .orp profile file and extract colors

    The .orp file format:
    - Color data starts at offset 0x0e44
    - Format per LED: [0x00, R, G, B] (4 bytes)
    - LEDs are in OpenRGB's display order (OPENRGB_LED_NAMES)

    Returns: list of (R, G, B) tuples for each LED
    """
    with open(filepath, "rb") as f:
        data = f.read()

    # Color data offset determined from binary analysis
    color_offset = 0x0e44

    colors = []
    for i in range(color_offset, min(len(data), color_offset + 126 * 4), 4):
        if i + 3 >= len(data):
            break

        marker = data[i]
        r = data[i + 1]
        g = data[i + 2]
        b = data[i + 3]

        if marker != 0x00:
            break

        colors.append((r, g, b))

    return colors

def convert_openrgb_to_kernel(orp_file, profile_name="ddd"):
    """
    Convert OpenRGB profile to kernel LED interface format

    Args:
        orp_file: Path to OpenRGB .orp profile file
        profile_name: Name of the profile (used for finding the file)

    Returns:
        dict mapping kernel_zone -> (R, G, B)
    """
    if not os.path.exists(orp_file):
        print(f"❌ Profile not found: {orp_file}")
        print(f"   Looking for: {profile_name}.orp in ~/.config/OpenRGB/")
        return None

    print(f"📂 Reading profile: {orp_file}")

    # Parse OpenRGB profile
    openrgb_colors = parse_orp_file(orp_file)
    print(f"✅ Extracted {len(openrgb_colors)} colors from profile")
    print()

    # Build mapping: OpenRGB LED index -> Kernel zone
    openrgb_to_kernel = {}

    for openrgb_idx, key_name in enumerate(OPENRGB_LED_NAMES):
        if openrgb_idx >= len(openrgb_colors):
            break

        if key_name in HW_LED_MAP:
            kernel_zone = HW_LED_MAP[key_name]
            openrgb_to_kernel[openrgb_idx] = kernel_zone

    print(f"✅ Mapped {len(openrgb_to_kernel)} LEDs to kernel zones")
    print()

    # Convert colors: kernel_zone -> (R, G, B)
    kernel_colors = {}

    for openrgb_idx, kernel_zone in openrgb_to_kernel.items():
        if openrgb_idx < len(openrgb_colors):
            kernel_colors[kernel_zone] = openrgb_colors[openrgb_idx]

    return kernel_colors

def apply_to_kernel_interface(kernel_colors, brightness=50):
    """
    Apply colors to the kernel LED interface

    Args:
        kernel_colors: dict mapping kernel_zone -> (R, G, B)
        brightness: Brightness value (0-50, default 50)

    Returns:
        Number of zones successfully applied
    """
    leds_path = "/sys/class/leds"
    applied = 0
    errors = 0

    print(f"💡 Applying {len(kernel_colors)} colors with brightness {brightness}...")
    print()

    for kernel_zone, (r, g, b) in sorted(kernel_colors.items()):
        if kernel_zone == 0:
            zone = "rgb:kbd_backlight"
        else:
            zone = f"rgb:kbd_backlight_{kernel_zone}"

        intensity_file = os.path.join(leds_path, zone, "multi_intensity")
        brightness_file = os.path.join(leds_path, zone, "brightness")

        try:
            with open(intensity_file, 'w') as f:
                f.write(f"{r} {g} {b}")
            with open(brightness_file, 'w') as f:
                f.write(str(brightness))
            applied += 1
        except FileNotFoundError:
            errors += 1
        except Exception as e:
            print(f"  ⚠️  Zone {kernel_zone}: {e}")
            errors += 1

    if errors > 0:
        print(f"⚠️  {errors} zones had errors (might not exist on this hardware)")

    return applied

def main():
    print("=" * 70)
    print(" TUXEDO Keyboard LED Interface Helper for OpenRGB Profiles")
    print("=" * 70)
    print()
    print("⚠️  This is a helper utility for TUXEDO/Clevo laptops only!")
    print("    It converts OpenRGB profiles to the kernel LED interface.")
    print()

    # Default profile location
    config_dir = os.path.expanduser("~/.config/OpenRGB")
    default_profile = "ddd"

    # Allow command-line profile name override
    if len(sys.argv) > 1:
        default_profile = sys.argv[1]

    orp_file = os.path.join(config_dir, f"{default_profile}.orp")

    # Convert profile
    kernel_colors = convert_openrgb_to_kernel(orp_file, default_profile)

    if kernel_colors is None:
        sys.exit(1)

    if len(kernel_colors) == 0:
        print("❌ No colors to apply!")
        sys.exit(1)

    # Show sample conversions for verification
    print("🔍 Sample conversions (WASD + hjkl):")
    test_keys = {
        "Key: W": "W", "Key: A": "A", "Key: S": "S", "Key: D": "D",
        "Key: H": "H", "Key: J": "J", "Key: K": "K", "Key: L": "L"
    }

    for key_name, short_name in test_keys.items():
        openrgb_idx = OPENRGB_LED_NAMES.index(key_name)
        if key_name in HW_LED_MAP and openrgb_idx < len(orp_file):
            kernel_zone = HW_LED_MAP[key_name]
            if kernel_zone in kernel_colors:
                r, g, b = kernel_colors[kernel_zone]
                is_lit = (r > 0 or g > 0 or b > 0)
                if is_lit:
                    print(f"  {short_name}: OpenRGB {openrgb_idx} → Kernel zone {kernel_zone} → #{r:02x}{g:02x}{b:02x}")

    print()
    print("=" * 70)

    # Apply to kernel interface
    applied = apply_to_kernel_interface(kernel_colors)

    print()
    print(f"✅ Successfully applied {applied} LED colors!")
    print()
    print("📋 Test Summary:")
    print("   Your OpenRGB profile is now active via the kernel LED interface.")
    print("   Check if the keyboard shows your expected colors.")
    print()
    print("⏳ EC Timeout Test:")
    print("   1. Don't touch the keyboard for 5-10 minutes")
    print("   2. Wait for the EC to turn off the backlight")
    print("   3. Press any key to wake it")
    print("   4. Verify: Do your profile colors return?")
    print("   - If YES → Success! Kernel interface persists through EC wake")
    print("   - If NO → Need periodic reapplication (systemd timer)")
    print()
    print("=" * 70)

if __name__ == "__main__":
    main()

rpi_ws281x
==========

Userspace Raspberry Pi library for controlling WS281X LEDs.
This includes WS2812 and SK6812RGB RGB LEDs
Preliminary support is now included for SK6812RGBW LEDs (yes, RGB + W)
The LEDs can be controlled by either the PWM (2 independent channels)
or PCM controller (1 channel) or the SPI interface (1 channel).

### Bindings:

Language-specific bindings for rpi_ws281x are available in:

* Python - https://github.com/rpi-ws281x/rpi-ws281x-python
* Rust - https://github.com/rpi-ws281x/rpi-ws281x-rust
* Powershell - https://github.com/rpi-ws281x/rpi-ws281x-powershell
* Java - https://github.com/rpi-ws281x/rpi-ws281x-java
* CSharp - https://github.com/rpi-ws281x/rpi-ws281x-csharp
* Go - https://github.com/rpi-ws281x/rpi-ws281x-go

### Background:

The BCM2835 in the Raspberry Pi has both a PWM and a PCM module that
are well suited to driving individually controllable WS281X LEDs.
Using the DMA, PWM or PCM FIFO, and serial mode in the PWM, it's
possible to control almost any number of WS281X LEDs in a chain connected
to the appropriate output pin.
For SPI the Raspbian spidev driver is used (`/dev/spidev0.0`).
This library and test program set the clock rate to 3X the desired output
frequency and creates a bit pattern in RAM from an array of colors where
each bit is represented by 3 bits as follows.

    Bit 1 - 1 1 0
    Bit 0 - 1 0 0


### GPIO Usage:

The GPIOs that can be used are limited by the hardware of the Pi and will
vary based on the method used to drive them (PWM, PCM or SPI).
Beware that the GPIO numbers are not the same as the physical pin numbers
on the header.

PWM:
```
        PWM0, which can be set to use GPIOs 12, 18, 40, and 52.
        Only 12 (pin 32) and 18 (pin 12) are available on the B+/2B/3B

        PWM1 which can be set to use GPIOs 13, 19, 41, 45 and 53.
        Only 13 is available on the B+/2B/PiZero/3B, on pin 33
```

PCM:
```
        PCM_DOUT, which can be set to use GPIOs 21 and 31.
        Only 21 is available on the B+/2B/PiZero/3B, on pin 40.
```

SPI:
```
        SPI0-MOSI is available on GPIOs 10 and 38.
        Only GPIO 10 is available on all models.
        See also note for RPi 3 below.
```


### Power and voltage requirements

WS281X LEDs are generally driven at 5V. Depending on your actual
LED model and data line length you might be able to successfully drive
the data input with 3.3V. However in the general case you probably
want to use a level shifter to convert from the Raspberry Pi GPIO/PWM to 5V.

It is also possible to run the LEDs from a 3.3V - 3.6V power source, and
connect the GPIO directly at a cost of brightness, but this isn't
recommended.

The test program is designed to drive a 8x8 grid of LEDs e.g.from
Adafruit (http://www.adafruit.com/products/1487) or Pimoroni
(https://shop.pimoroni.com/products/unicorn-hat).
Please see the Adafruit and Pimoroni websites for more information.

Know what you're doing with the hardware and electricity.  I take no
reponsibility for damage, harm, or mistakes.

### Build:

#### Build with SCons:

- Install Scons (on raspbian, `apt-get install scons`).
- Make sure to adjust the parameters in main.c to suit your hardware.
  - Signal rate (400kHz to 800kHz).  Default 800kHz.
  - ledstring.invert=1 if using a inverting level shifter.
  - Width and height of LED matrix (height=1 for LED string).
- Type `scons` from inside the source directory.

#### Build and install with CMake:

- Install CMake
- Configure your build:

  For example:
  ```
  mkdir build
  cd build
  cmake -D BUILD_SHARED=OFF -D BUILD_TEST=ON ..
  ```
  See also for available options in `CMakeLists.txt`.
- Type `cmake --build .` to build
- To install built binaries and headers into your system type:
  ```
  sudo make install
  ```

### Running:

- Type `sudo ./test` (default uses PWM channel 0).
- That's it.  You should see a moving rainbow scroll across the
  display.
- More options are available, `./test -h` should show them:
```
./test version 1.1.0
Usage: ./test
-h (--help)    - this information
-s (--strip)   - strip type - rgb, grb, gbr, rgbw
-x (--width)   - matrix width (default 8)
-y (--height)  - matrix height (default 8)
-d (--dma)     - dma channel to use (default 10)
-g (--gpio)    - GPIO to use
                 If omitted, default is 18 (PWM0)
-i (--invert)  - invert pin output (pulse LOW)
-c (--clear)   - clear matrix on exit.
-v (--version) - version information
```

### Important warning about DMA channels

You must make sure that the DMA channel you choose to use for the LEDs is not [already in use](https://www.raspberrypi.org/forums/viewtopic.php?p=609380#p609380) by the operating system.

For example, **using DMA channel 5 [will cause](https://github.com/jgarff/rpi_ws281x/issues/224) filesystem corruption** on the Raspberry Pi 3 Model B.

The default DMA channel (10) should be safe for the Raspberry Pi 3 Model B, but this may change in future software releases.

### Limitations:

#### PWM

Since this library and the onboard Raspberry Pi audio
both use the PWM, they cannot be used together.  You will need to
blacklist the Broadcom audio kernel module by creating a file
`/etc/modprobe.d/snd-blacklist.conf` with

    blacklist snd_bcm2835

If the audio device is still loading after blacklisting, you may also
need to comment it out in the /etc/modules file.

On headless systems you may also need to force audio through hdmi
Edit config.txt and add:

    hdmi_force_hotplug=1
    hdmi_force_edid_audio=1

A reboot is required for this change to take effect

Some distributions use audio by default, even if nothing is being played.
If audio is needed, you can use a USB audio device instead.

#### PCM

When using PCM you cannot use digital audio devices which use I2S since I2S
uses the PCM hardware, but you can use analog audio.

#### SPI

When using SPI the led string is the only device which can be connected to
the SPI bus. Both digital (I2S/PCM) and analog (PWM) audio can be used.

Many distributions have a maximum SPI transfer of 4096 bytes. This can be
changed in `/boot/cmdline.txt` by appending
```
    spidev.bufsiz=32768
```

On an RPi 3 you have to change the GPU core frequency to 250 MHz, otherwise
the SPI clock has the wrong frequency.

Do this by adding the following line to /boot/config.txt and reboot:

```
    core_freq=250
```

On an RPi 4 you must set a fixed frequency to avoid the idle CPU scaling changing the SPI frequency and breaking the ws281x timings:

Do this by adding the following lines to /boot/config.txt and reboot:

```
    core_freq=500
    core_freq_min=500
```

SPI requires you to be in the `gpio` group if you wish to control your LEDs
without root.

### Comparison PWM/PCM/SPI

Both PWM and PCM use DMA transfer to output the control signal for the LEDs.
The max size of a DMA transfer is 65536 bytes. Since each LED needs 12 bytes
(4 colors, 8 symbols per color, 3 bits per symbol) this means you can
control approximately 5400 LEDs for a single strand in PCM and 2700 LEDs per string
for PWM (Only PWM can control 2 independent strings simultaneously)
SPI uses the SPI device driver in the kernel. For transfers larger than
96 bytes the kernel driver also uses DMA.
Of course there are practical limits on power and signal quality. These will
be more constraining in practice than the theoretical limits above.

When controlling a LED string of 240 LEDs the CPU load on the original Pi 2 (BCM2836) are:
  PWM  5%
  PCM  5%
  SPI  1%

### Usage:

The API is very simple.  Make sure to create and initialize the `ws2811_t`
structure as seen in [`main.c`](main.c).  From there it can be initialized
by calling `ws2811_init()`.  LEDs are changed by modifying the color in
the `.led[index]` array and calling `ws2811_render()`.
The rest is handled by the library, which either creates the DMA memory and
starts the DMA for PWM and PCM or prepares the SPI transfer buffer and sends
it out on the MISO pin.

Make sure to hook a signal handler for SIGKILL to do cleanup.  From the
handler make sure to call `ws2811_fini()`.  It'll make sure that the DMA
is finished before program execution stops and cleans up after itself.

/*
 * ws2811.c
 *
 * Copyright (c) 2014 Jeremy Garff <jer @ jers.net>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     1.  Redistributions of source code must retain the above copyright notice, this list of
 *         conditions and the following disclaimer.
 *     2.  Redistributions in binary form must reproduce the above copyright notice, this list
 *         of conditions and the following disclaimer in the documentation and/or other materials
 *         provided with the distribution.
 *     3.  Neither the name of the owner nor the names of its contributors may be used to endorse
 *         or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <signal.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <time.h>
#include <math.h>
#include "mailbox.h"
#include "clk.h"
#include "gpio.h"
#include "dma.h"
#include "pwm.h"
#include "pcm.h"
#include "rpihw.h"

#include "ws2811.h"


#define BUS_TO_PHYS(x)                           ((x)&~0xC0000000)

#define OSC_FREQ                                 19200000   // crystal frequency
#define OSC_FREQ_PI4                             54000000   // Pi 4 crystal frequency

/* 4 colors (R, G, B + W), 8 bits per byte, 3 symbols per bit + 55uS low for reset signal */
#define LED_COLOURS                              4
#define LED_RESET_uS                             55
#define LED_BIT_COUNT(leds, freq)                ((leds * LED_COLOURS * 8 * 3) + ((LED_RESET_uS * \
                                                  (freq * 3)) / 1000000))

/* Minimum time to wait for reset to occur in microseconds. */
#define LED_RESET_WAIT_TIME                      300

// Pad out to the nearest uint32 + 32-bits for idle low/high times the number of channels
#define PWM_BYTE_COUNT(leds, freq)               (((((LED_BIT_COUNT(leds, freq) >> 3) & ~0x7) + 4) + 4) * \
                                                  RPI_PWM_CHANNELS)
#define PCM_BYTE_COUNT(leds, freq)               ((((LED_BIT_COUNT(leds, freq) >> 3) & ~0x7) + 4) + 4)

// Driver mode definitions
#define NONE	0
#define PWM	1
#define PCM	2
#define SPI	3

// We use the mailbox interface to request memory from the VideoCore.
// This lets us request one physically contiguous chunk, find its
// physical address, and map it 'uncached' so that writes from this
// code are immediately visible to the DMA controller.  This struct
// holds data relevant to the mailbox interface.
typedef struct videocore_mbox {
    int handle;             /* From mbox_open() */
    unsigned mem_ref;       /* From mem_alloc() */
    unsigned bus_addr;      /* From mem_lock() */
    unsigned size;          /* Size of allocation */
    uint8_t *virt_addr;     /* From mapmem() */
} videocore_mbox_t;

typedef struct ws2811_device
{
    int driver_mode;
    volatile uint8_t *pxl_raw;
    volatile dma_t *dma;
    volatile pwm_t *pwm;
    volatile pcm_t *pcm;
    int spi_fd;
    volatile dma_cb_t *dma_cb;
    uint32_t dma_cb_addr;
    volatile gpio_t *gpio;
    volatile cm_clk_t *cm_clk;
    videocore_mbox_t mbox;
    int max_count;
} ws2811_device_t;

/**
 * Provides monotonic timestamp in microseconds.
 *
 * @returns  Current timestamp in microseconds or 0 on error.
 */
static uint64_t get_microsecond_timestamp()
{
    struct timespec t;

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &t) != 0) {
        return 0;
    }

    return (uint64_t) t.tv_sec * 1000000 + t.tv_nsec / 1000;
}

/**
 * Iterate through the channels and find the largest led count.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  Maximum number of LEDs in all channels.
 */
static int max_channel_led_count(ws2811_t *ws2811)
{
    int chan, max = 0;

    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)
    {
        if (ws2811->channel[chan].count > max)
        {
            max = ws2811->channel[chan].count;
        }
    }

    return max;
}

/**
 * Map all devices into userspace memory.
 * Not called for SPI
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  0 on success, -1 otherwise.
 */
static int map_registers(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;
    const rpi_hw_t *rpi_hw = ws2811->rpi_hw;
    uint32_t base = ws2811->rpi_hw->periph_base;
    uint32_t dma_addr;
    uint32_t offset = 0;

    dma_addr = dmanum_to_offset(ws2811->dmanum);
    if (!dma_addr)
    {
        return -1;
    }
    dma_addr += rpi_hw->periph_base;

    device->dma = mapmem(dma_addr, sizeof(dma_t), DEV_MEM);
    if (!device->dma)
    {
        return -1;
    }

    switch (device->driver_mode) {
    case PWM:
        device->pwm = mapmem(PWM_OFFSET + base, sizeof(pwm_t), DEV_MEM);
        if (!device->pwm)
        {
            return -1;
        }
        break;

    case PCM:
        device->pcm = mapmem(PCM_OFFSET + base, sizeof(pcm_t), DEV_MEM);
        if (!device->pcm)
        {
            return -1;
        }
        break;
    }

    /*
     * The below call can potentially work with /dev/gpiomem instead.
     * However, it used /dev/mem before, so I'm leaving it as such.
     */

    device->gpio = mapmem(GPIO_OFFSET + base, sizeof(gpio_t), DEV_MEM);
    if (!device->gpio)
    {
        return -1;
    }

    switch (device->driver_mode) {
    case PWM:
        offset = CM_PWM_OFFSET;
        break;
    case PCM:
        offset = CM_PCM_OFFSET;
        break;
    }
    device->cm_clk = mapmem(offset + base, sizeof(cm_clk_t), DEV_MEM);
    if (!device->cm_clk)
    {
        return -1;
    }

    return 0;
}

/**
 * Unmap all devices from virtual memory.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
static void unmap_registers(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;

    if (device->dma)
    {
        unmapmem((void *)device->dma, sizeof(dma_t));
    }

    if (device->pwm)
    {
        unmapmem((void *)device->pwm, sizeof(pwm_t));
    }

    if (device->pcm)
    {
        unmapmem((void *)device->pcm, sizeof(pcm_t));
    }

    if (device->cm_clk)
    {
        unmapmem((void *)device->cm_clk, sizeof(cm_clk_t));
    }

    if (device->gpio)
    {
        unmapmem((void *)device->gpio, sizeof(gpio_t));
    }
}

/**
 * Given a userspace address pointer, return the matching bus address used by DMA.
 *     Note: The bus address is not the same as the CPU physical address.
 *
 * @param    addr   Userspace virtual address pointer.
 *
 * @returns  Bus address for use by DMA.
 */
static uint32_t addr_to_bus(ws2811_device_t *device, const volatile void *virt)
{
    videocore_mbox_t *mbox = &device->mbox;

    uint32_t offset = (uint8_t *)virt - mbox->virt_addr;

    return mbox->bus_addr + offset;
}

/**
 * Stop the PWM controller.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
static void stop_pwm(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;
    volatile pwm_t *pwm = device->pwm;
    volatile cm_clk_t *cm_clk = device->cm_clk;

    // Turn off the PWM in case already running
    pwm->ctl = 0;
    usleep(10);

    // Kill the clock if it was already running
    cm_clk->ctl = CM_CLK_CTL_PASSWD | CM_CLK_CTL_KILL;
    usleep(10);
    while (cm_clk->ctl & CM_CLK_CTL_BUSY)
        ;
}

/**
 * Stop the PCM controller.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
static void stop_pcm(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;
    volatile pcm_t *pcm = device->pcm;
    volatile cm_clk_t *cm_clk = device->cm_clk;

    // Turn off the PCM in case already running
    pcm->cs = 0;
    usleep(10);

    // Kill the clock if it was already running
    cm_clk->ctl = CM_CLK_CTL_PASSWD | CM_CLK_CTL_KILL;
    usleep(10);
    while (cm_clk->ctl & CM_CLK_CTL_BUSY)
        ;
}

/**
 * Setup the PWM controller in serial mode on both channels using DMA to feed the PWM FIFO.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
static int setup_pwm(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;
    volatile dma_t *dma = device->dma;
    volatile dma_cb_t *dma_cb = device->dma_cb;
    volatile pwm_t *pwm = device->pwm;
    volatile cm_clk_t *cm_clk = device->cm_clk;
    int maxcount = device->max_count;
    uint32_t freq = ws2811->freq;
    int32_t byte_count;

    const rpi_hw_t *rpi_hw = ws2811->rpi_hw;
    const uint32_t rpi_type = rpi_hw->type;
    uint32_t osc_freq = OSC_FREQ;

    if(rpi_type == RPI_HWVER_TYPE_PI4){
        osc_freq = OSC_FREQ_PI4;
    }

    stop_pwm(ws2811);

    // Setup the Clock - Use OSC @ 19.2Mhz w/ 3 clocks/tick
    cm_clk->div = CM_CLK_DIV_PASSWD | CM_CLK_DIV_DIVI(osc_freq / (3 * freq));
    cm_clk->ctl = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC;
    cm_clk->ctl = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC | CM_CLK_CTL_ENAB;
    usleep(10);
    while (!(cm_clk->ctl & CM_CLK_CTL_BUSY))
        ;

    // Setup the PWM, use delays as the block is rumored to lock up without them.  Make
    // sure to use a high enough priority to avoid any FIFO underruns, especially if
    // the CPU is busy doing lots of memory accesses, or another DMA controller is
    // busy.  The FIFO will clock out data at a much slower rate (2.6Mhz max), so
    // the odds of a DMA priority boost are extremely low.

    pwm->rng1 = 32;  // 32-bits per word to serialize
    usleep(10);
    pwm->ctl = RPI_PWM_CTL_CLRF1;
    usleep(10);
    pwm->dmac = RPI_PWM_DMAC_ENAB | RPI_PWM_DMAC_PANIC(7) | RPI_PWM_DMAC_DREQ(3);
    usleep(10);
    pwm->ctl = RPI_PWM_CTL_USEF1 | RPI_PWM_CTL_MODE1 |
               RPI_PWM_CTL_USEF2 | RPI_PWM_CTL_MODE2;
    if (ws2811->channel[0].invert)
    {
        pwm->ctl |= RPI_PWM_CTL_POLA1;
    }
    if (ws2811->channel[1].invert)
    {
        pwm->ctl |= RPI_PWM_CTL_POLA2;
    }
    usleep(10);
    pwm->ctl |= RPI_PWM_CTL_PWEN1 | RPI_PWM_CTL_PWEN2;

    // Initialize the DMA control block
    byte_count = PWM_BYTE_COUNT(maxcount, freq);
    dma_cb->ti = RPI_DMA_TI_NO_WIDE_BURSTS |  // 32-bit transfers
                 RPI_DMA_TI_WAIT_RESP |       // wait for write complete
                 RPI_DMA_TI_DEST_DREQ |       // user peripheral flow control
                 RPI_DMA_TI_PERMAP(5) |       // PWM peripheral
                 RPI_DMA_TI_SRC_INC;          // Increment src addr

    dma_cb->source_ad = addr_to_bus(device, device->pxl_raw);

    dma_cb->dest_ad = (uintptr_t)&((pwm_t *)PWM_PERIPH_PHYS)->fif1;
    dma_cb->txfr_len = byte_count;
    dma_cb->stride = 0;
    dma_cb->nextconbk = 0;

    dma->cs = 0;
    dma->txfr_len = 0;

    return 0;
}

/**
 * Setup the PCM controller with one 32-bit channel in a 32-bit frame using DMA to feed the PCM FIFO.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
static int setup_pcm(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;
    volatile dma_t *dma = device->dma;
    volatile dma_cb_t *dma_cb = device->dma_cb;
    volatile pcm_t *pcm = device->pcm;
    volatile cm_clk_t *cm_clk = device->cm_clk;
    //int maxcount = max_channel_led_count(ws2811);
    int maxcount = device->max_count;
    uint32_t freq = ws2811->freq;
    int32_t byte_count;

    const rpi_hw_t *rpi_hw = ws2811->rpi_hw;
    const uint32_t rpi_type = rpi_hw->type;
    uint32_t osc_freq = OSC_FREQ;

    if(rpi_type == RPI_HWVER_TYPE_PI4){
        osc_freq = OSC_FREQ_PI4;
    }

    stop_pcm(ws2811);

    // Setup the PCM Clock - Use OSC @ 19.2Mhz w/ 3 clocks/tick
    cm_clk->div = CM_CLK_DIV_PASSWD | CM_CLK_DIV_DIVI(osc_freq / (3 * freq));
    cm_clk->ctl = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC;
    cm_clk->ctl = CM_CLK_CTL_PASSWD | CM_CLK_CTL_SRC_OSC | CM_CLK_CTL_ENAB;
    usleep(10);
    while (!(cm_clk->ctl & CM_CLK_CTL_BUSY))
        ;

    // Setup the PCM, use delays as the block is rumored to lock up without them.  Make
    // sure to use a high enough priority to avoid any FIFO underruns, especially if
    // the CPU is busy doing lots of memory accesses, or another DMA controller is
    // busy.  The FIFO will clock out data at a much slower rate (2.6Mhz max), so
    // the odds of a DMA priority boost are extremely low.

    pcm->cs = RPI_PCM_CS_EN;            // Enable PCM hardware
    pcm->mode = (RPI_PCM_MODE_FLEN(31) | RPI_PCM_MODE_FSLEN(1));
                // Framelength 32, clock enabled, frame sync pulse
    pcm->txc = RPI_PCM_TXC_CH1WEX | RPI_PCM_TXC_CH1EN | RPI_PCM_TXC_CH1POS(0) | RPI_PCM_TXC_CH1WID(8);
               // Single 32-bit channel
    pcm->cs |= RPI_PCM_CS_TXCLR;        // Reset transmit fifo
    usleep(10);
    pcm->cs |= RPI_PCM_CS_DMAEN;         // Enable DMA DREQ
    pcm->dreq = (RPI_PCM_DREQ_TX(0x3F) | RPI_PCM_DREQ_TX_PANIC(0x10)); // Set FIFO tresholds

    // Initialize the DMA control block
    byte_count = PCM_BYTE_COUNT(maxcount, freq);
    dma_cb->ti = RPI_DMA_TI_NO_WIDE_BURSTS |  // 32-bit transfers
                 RPI_DMA_TI_WAIT_RESP |       // wait for write complete
                 RPI_DMA_TI_DEST_DREQ |       // user peripheral flow control
                 RPI_DMA_TI_PERMAP(2) |       // PCM TX peripheral
                 RPI_DMA_TI_SRC_INC;          // Increment src addr

    dma_cb->source_ad = addr_to_bus(device, device->pxl_raw);
    dma_cb->dest_ad = (uintptr_t)&((pcm_t *)PCM_PERIPH_PHYS)->fifo;
    dma_cb->txfr_len = byte_count;
    dma_cb->stride = 0;
    dma_cb->nextconbk = 0;

    dma->cs = 0;
    dma->txfr_len = 0;

    return 0;
}

/**
 * Start the DMA feeding the PWM FIFO.  This will stream the entire DMA buffer out of both
 * PWM channels.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
static void dma_start(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;
    volatile dma_t *dma = device->dma;
    volatile pcm_t *pcm = device->pcm;
    uint32_t dma_cb_addr = device->dma_cb_addr;

    dma->cs = RPI_DMA_CS_RESET;
    usleep(10);

    dma->cs = RPI_DMA_CS_INT | RPI_DMA_CS_END;
    usleep(10);

    dma->conblk_ad = dma_cb_addr;
    dma->debug = 7; // clear debug error flags
    dma->cs = RPI_DMA_CS_WAIT_OUTSTANDING_WRITES |
              RPI_DMA_CS_PANIC_PRIORITY(15) |
              RPI_DMA_CS_PRIORITY(15) |
              RPI_DMA_CS_ACTIVE;

    if (device->driver_mode == PCM)
    {
        pcm->cs |= RPI_PCM_CS_TXON;  // Start transmission
    }
}

/**
 * Initialize the application selected GPIO pins for PWM/PCM operation.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  0 on success, -1 on unsupported pin
 */
static int gpio_init(ws2811_t *ws2811)
{
    volatile gpio_t *gpio = ws2811->device->gpio;
    int chan;
    int altnum;

    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)
    {
        int pinnum = ws2811->channel[chan].gpionum;

        if (pinnum)
        {
            switch (ws2811->device->driver_mode)
            {
            case PWM:
                altnum = pwm_pin_alt(chan, pinnum);
                break;
            case PCM:
                altnum = pcm_pin_alt(PCMFUN_DOUT, pinnum);
                break;
            default:
                altnum = -1;
            }

            if (altnum < 0)
            {
                return -1;
            }

            gpio_function_set(gpio, pinnum, altnum);
        }
    }

    return 0;
}

/**
 * Initialize the PWM DMA buffer with all zeros, inverted operation will be
 * handled by hardware.  The DMA buffer length is assumed to be a word
 * multiple.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
void pwm_raw_init(ws2811_t *ws2811)
{
    volatile uint32_t *pxl_raw = (uint32_t *)ws2811->device->pxl_raw;
    int maxcount = ws2811->device->max_count;
    int wordcount = (PWM_BYTE_COUNT(maxcount, ws2811->freq) / sizeof(uint32_t)) /
                    RPI_PWM_CHANNELS;
    int chan;

    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)
    {
        int i, wordpos = chan;

        for (i = 0; i < wordcount; i++)
        {
            pxl_raw[wordpos] = 0x0;
            wordpos += 2;
        }
    }
}

/**
 * Initialize the PCM DMA buffer with all zeros.
 * The DMA buffer length is assumed to be a word multiple.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
void pcm_raw_init(ws2811_t *ws2811)
{
    volatile uint32_t *pxl_raw = (uint32_t *)ws2811->device->pxl_raw;
    int maxcount = ws2811->device->max_count;
    int wordcount = PCM_BYTE_COUNT(maxcount, ws2811->freq) / sizeof(uint32_t);
    int i;

    for (i = 0; i < wordcount; i++)
    {
        pxl_raw[i] = 0x0;
    }
}

/**
 * Cleanup previously allocated device memory and buffers.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
void ws2811_cleanup(ws2811_t *ws2811)
{
    ws2811_device_t *device = ws2811->device;
    int chan;

    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)
    {
        if (ws2811->channel[chan].leds)
        {
            free(ws2811->channel[chan].leds);
        }
        ws2811->channel[chan].leds = NULL;
        if (ws2811->channel[chan].gamma)
        {
            free(ws2811->channel[chan].gamma);
        }
        ws2811->channel[chan].gamma = NULL;
    }

    if (device->mbox.handle != -1)
    {
        videocore_mbox_t *mbox = &device->mbox;

        unmapmem(mbox->virt_addr, mbox->size);
        mem_unlock(mbox->handle, mbox->mem_ref);
        mem_free(mbox->handle, mbox->mem_ref);
        mbox_close(mbox->handle);

        mbox->handle = -1;
    }

    if (device && (device->spi_fd > 0))
    {
        close(device->spi_fd);
    }

    if (device) {
        free(device);
    }
    ws2811->device = NULL;
}

static int set_driver_mode(ws2811_t *ws2811, int gpionum)
{
    int gpionum2;

    if (gpionum == 18 || gpionum == 12) {
        ws2811->device->driver_mode = PWM;
        // Check gpio for PWM1 (2nd channel) is OK if used
        gpionum2 = ws2811->channel[1].gpionum;
        if (gpionum2 == 0 || gpionum2 == 13 || gpionum2 == 19) {
            return 0;
        }
    }
    else if (gpionum == 21 || gpionum == 31) {
        ws2811->device->driver_mode = PCM;
    }
    else if (gpionum == 10) {
        ws2811->device->driver_mode = SPI;
    }
    else {
        fprintf(stderr, "gpionum %d not allowed\n", gpionum);
        return -1;
    }
    // For PCM and SPI zero the 2nd channel
    memset(&ws2811->channel[1], 0, sizeof(ws2811_channel_t));

    return 0;
}

static int check_hwver_and_gpionum(ws2811_t *ws2811)
{
    const rpi_hw_t *rpi_hw;
    int hwver, gpionum;
    int gpionums_B1[] = { 10, 18, 21 };
    int gpionums_B2[] = { 10, 18, 31 };
    int gpionums_40p[] = { 10, 12, 18, 21};
    int i;

    rpi_hw = ws2811->rpi_hw;
    hwver = rpi_hw->hwver & 0x0000ffff;
    gpionum = ws2811->channel[0].gpionum;
    if (hwver < 0x0004)  // Model B Rev 1
    {
        for ( i = 0; i < (int)(sizeof(gpionums_B1) / sizeof(gpionums_B1[0])); i++)
        {
            if (gpionums_B1[i] == gpionum) {
                // Set driver mode (PWM, PCM, or SPI)
                return set_driver_mode(ws2811, gpionum);
            }
        }
    }
    else if (hwver >= 0x0004 && hwver <= 0x000f)  // Models B Rev2, A
    {
        for ( i = 0; i < (int)(sizeof(gpionums_B2) / sizeof(gpionums_B2[0])); i++)
        {
            if (gpionums_B2[i] == gpionum) {
                // Set driver mode (PWM, PCM, or SPI)
                return set_driver_mode(ws2811, gpionum);
            }
        }
    }
    else if (hwver >= 0x010) // Models B+, A+, 2B, 3B, Zero Zero-W
    {
        if ((ws2811->channel[0].count == 0) && (ws2811->channel[1].count > 0))
        {
            // Special case: nothing in channel 0, channel 1 only PWM1 allowed
            // PWM1 only available on 40 pin GPIO interface
            gpionum = ws2811->channel[1].gpionum;
            if ((gpionum == 13) || (gpionum == 19))
            {
                ws2811->device->driver_mode = PWM;
                return 0;
            }
            else {
                return -1;
            }
        }
        for ( i = 0; i < (int)(sizeof(gpionums_40p) / sizeof(gpionums_40p[0])); i++)
        {
            if (gpionums_40p[i] == gpionum) {
                // Set driver mode (PWM, PCM, or SPI)
                return set_driver_mode(ws2811, gpionum);
            }
        }
    }
    fprintf(stderr, "Gpio %d is illegal for LED channel 0\n", gpionum);
    return -1;
}

static ws2811_return_t spi_init(ws2811_t *ws2811)
{
    int spi_fd;
    static uint8_t mode;
    static uint8_t bits = 8;
    uint32_t speed = ws2811->freq * 3;
    ws2811_device_t *device = ws2811->device;
    uint32_t base = ws2811->rpi_hw->periph_base;
    int pinnum = ws2811->channel[0].gpionum;

    spi_fd = open("/dev/spidev0.0", O_RDWR);
    if (spi_fd < 0) {
        fprintf(stderr, "Cannot open /dev/spidev0.0. spi_bcm2835 module not loaded?\n");
        return WS2811_ERROR_SPI_SETUP;
    }
    device->spi_fd = spi_fd;

    // SPI mode
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0)
    {
        return WS2811_ERROR_SPI_SETUP;
    }
    if (ioctl(spi_fd, SPI_IOC_RD_MODE, &mode) < 0)
    {
        return WS2811_ERROR_SPI_SETUP;
    }

    // Bits per word
    if (ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0)
    {
        return WS2811_ERROR_SPI_SETUP;
    }
    if (ioctl(spi_fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0)
    {
        return WS2811_ERROR_SPI_SETUP;
    }

    // Max speed Hz
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
    {
        return WS2811_ERROR_SPI_SETUP;
    }
    if (ioctl(spi_fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0)
    {
        return WS2811_ERROR_SPI_SETUP;
    }

    // Initialize device structure elements to not used
    // except driver_mode, spi_fd and max_count (already defined when spi_init called)
    device->pxl_raw = NULL;
    device->dma = NULL;
    device->pwm = NULL;
    device->pcm = NULL;
    device->dma_cb = NULL;
    device->dma_cb_addr = 0;
    device->cm_clk = NULL;
    device->mbox.handle = -1;

    // Set SPI-MOSI pin
    device->gpio = mapmem(GPIO_OFFSET + base, sizeof(gpio_t), DEV_GPIOMEM);
    if (!device->gpio)
    {
        return WS2811_ERROR_SPI_SETUP;
    }
    gpio_function_set(device->gpio, pinnum, 0);	// SPI-MOSI ALT0

    // Allocate LED buffer
    ws2811_channel_t *channel = &ws2811->channel[0];
    channel->leds = malloc(sizeof(ws2811_led_t) * channel->count);
    if (!channel->leds)
    {
        ws2811_cleanup(ws2811);
        return WS2811_ERROR_OUT_OF_MEMORY;
    }
    memset(channel->leds, 0, sizeof(ws2811_led_t) * channel->count);
    if (!channel->strip_type)
    {
      channel->strip_type=WS2811_STRIP_RGB;
    }

    // Set default uncorrected gamma table
    if (!channel->gamma)
    {
      channel->gamma = malloc(sizeof(uint8_t) * 256);
      int x;
      for(x = 0; x < 256; x++){
        channel->gamma[x] = x;
      }
    }

    channel->wshift = (channel->strip_type >> 24) & 0xff;
    channel->rshift = (channel->strip_type >> 16) & 0xff;
    channel->gshift = (channel->strip_type >> 8)  & 0xff;
    channel->bshift = (channel->strip_type >> 0)  & 0xff;

    // Allocate SPI transmit buffer (same size as PCM)
    device->pxl_raw = malloc(PCM_BYTE_COUNT(device->max_count, ws2811->freq));
    if (device->pxl_raw == NULL)
    {
        ws2811_cleanup(ws2811);
        return WS2811_ERROR_OUT_OF_MEMORY;
    }
    pcm_raw_init(ws2811);

    return WS2811_SUCCESS;
}

static ws2811_return_t spi_transfer(ws2811_t *ws2811)
{
    int ret;
    struct spi_ioc_transfer tr;

    memset(&tr, 0, sizeof(struct spi_ioc_transfer));
    tr.tx_buf = (unsigned long)ws2811->device->pxl_raw;
    tr.rx_buf = 0;
    tr.len = PCM_BYTE_COUNT(ws2811->device->max_count, ws2811->freq);

    ret = ioctl(ws2811->device->spi_fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
    {
        fprintf(stderr, "Can't send spi message");
        return WS2811_ERROR_SPI_TRANSFER;
    }

    return WS2811_SUCCESS;
}


/*
 *
 * Application API Functions
 *
 */


/**
 * Allocate and initialize memory, buffers, pages, PWM, DMA, and GPIO.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  0 on success, -1 otherwise.
 */
ws2811_return_t ws2811_init(ws2811_t *ws2811)
{
    ws2811_device_t *device;
    const rpi_hw_t *rpi_hw;
    int chan;

    ws2811->rpi_hw = rpi_hw_detect();
    if (!ws2811->rpi_hw)
    {
        return WS2811_ERROR_HW_NOT_SUPPORTED;
    }
    rpi_hw = ws2811->rpi_hw;

    ws2811->device = malloc(sizeof(*ws2811->device));
    if (!ws2811->device)
    {
        return WS2811_ERROR_OUT_OF_MEMORY;
    }
    memset(ws2811->device, 0, sizeof(*ws2811->device));
    device = ws2811->device;

    if (check_hwver_and_gpionum(ws2811) < 0)
    {
        return WS2811_ERROR_ILLEGAL_GPIO;
    }

    device->max_count = max_channel_led_count(ws2811);

    if (device->driver_mode == SPI) {
        return spi_init(ws2811);
    }

    // Determine how much physical memory we need for DMA
    switch (device->driver_mode) {
    case PWM:
        device->mbox.size = PWM_BYTE_COUNT(device->max_count, ws2811->freq) +
                            sizeof(dma_cb_t);
        break;

    case PCM:
        device->mbox.size = PCM_BYTE_COUNT(device->max_count, ws2811->freq) +
                            sizeof(dma_cb_t);
        break;
    }
    // Round up to page size multiple
    device->mbox.size = (device->mbox.size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

    device->mbox.handle = mbox_open();
    if (device->mbox.handle == -1)
    {
        return WS2811_ERROR_MAILBOX_DEVICE;
    }

    device->mbox.mem_ref = mem_alloc(device->mbox.handle, device->mbox.size, PAGE_SIZE,
                                     rpi_hw->videocore_base == 0x40000000 ? 0xC : 0x4);
    if (device->mbox.mem_ref == 0)
    {
        return WS2811_ERROR_OUT_OF_MEMORY;
    }

    device->mbox.bus_addr = mem_lock(device->mbox.handle, device->mbox.mem_ref);
    if (device->mbox.bus_addr == (uint32_t) ~0UL)
    {
       mem_free(device->mbox.handle, device->mbox.size);
       return WS2811_ERROR_MEM_LOCK;
    }

    device->mbox.virt_addr = mapmem(BUS_TO_PHYS(device->mbox.bus_addr), device->mbox.size, DEV_MEM);
    if (!device->mbox.virt_addr)
    {
        mem_unlock(device->mbox.handle, device->mbox.mem_ref);
        mem_free(device->mbox.handle, device->mbox.size);

        ws2811_cleanup(ws2811);
        return WS2811_ERROR_MMAP;
    }

    // Initialize all pointers to NULL.  Any non-NULL pointers will be freed on cleanup.
    device->pxl_raw = NULL;
    device->dma_cb = NULL;
    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)
    {
        ws2811->channel[chan].leds = NULL;
    }

    // Allocate the LED buffers
    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)
    {
        ws2811_channel_t *channel = &ws2811->channel[chan];

        channel->leds = malloc(sizeof(ws2811_led_t) * channel->count);
        if (!channel->leds)
        {
            ws2811_cleanup(ws2811);
            return WS2811_ERROR_OUT_OF_MEMORY;
        }

        memset(channel->leds, 0, sizeof(ws2811_led_t) * channel->count);

        if (!channel->strip_type)
        {
          channel->strip_type=WS2811_STRIP_RGB;
        }

        // Set default uncorrected gamma table
        if (!channel->gamma)
        {
          channel->gamma = malloc(sizeof(uint8_t) * 256);
          int x;
          for(x = 0; x < 256; x++){
            channel->gamma[x] = x;
          }
        }

        channel->wshift = (channel->strip_type >> 24) & 0xff;
        channel->rshift = (channel->strip_type >> 16) & 0xff;
        channel->gshift = (channel->strip_type >> 8)  & 0xff;
        channel->bshift = (channel->strip_type >> 0)  & 0xff;

    }

    device->dma_cb = (dma_cb_t *)device->mbox.virt_addr;
    device->pxl_raw = (uint8_t *)device->mbox.virt_addr + sizeof(dma_cb_t);

    switch (device->driver_mode) {
    case PWM:
       pwm_raw_init(ws2811);
       break;

    case PCM:
       pcm_raw_init(ws2811);
       break;
    }

    memset((dma_cb_t *)device->dma_cb, 0, sizeof(dma_cb_t));

    // Cache the DMA control block bus address
    device->dma_cb_addr = addr_to_bus(device, device->dma_cb);

    // Map the physical registers into userspace
    if (map_registers(ws2811))
    {
        ws2811_cleanup(ws2811);
        return WS2811_ERROR_MAP_REGISTERS;
    }

    // Initialize the GPIO pins
    if (gpio_init(ws2811))
    {
        unmap_registers(ws2811);
        ws2811_cleanup(ws2811);
        return WS2811_ERROR_GPIO_INIT;
    }

    switch (device->driver_mode) {
    case PWM:
        // Setup the PWM, clocks, and DMA
        if (setup_pwm(ws2811))
        {
            unmap_registers(ws2811);
            ws2811_cleanup(ws2811);
            return WS2811_ERROR_PWM_SETUP;
        }
        break;
    case PCM:
    // Setup the PCM, clock, and DMA
        if (setup_pcm(ws2811))
        {
            unmap_registers(ws2811);
            ws2811_cleanup(ws2811);
            return WS2811_ERROR_PCM_SETUP;
        }
        break;
    }

    return WS2811_SUCCESS;
}

/**
 * Shut down DMA, PWM, and cleanup memory.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
void ws2811_fini(ws2811_t *ws2811)
{
    volatile pcm_t *pcm = ws2811->device->pcm;

    ws2811_wait(ws2811);
    switch (ws2811->device->driver_mode) {
    case PWM:
        stop_pwm(ws2811);
        break;
    case PCM:
        while (!(pcm->cs & RPI_PCM_CS_TXE)) ;    // Wait till TX FIFO is empty
        stop_pcm(ws2811);
        break;
    }

    unmap_registers(ws2811);

    ws2811_cleanup(ws2811);
}

/**
 * Wait for any executing DMA operation to complete before returning.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  0 on success, -1 on DMA competion error
 */
ws2811_return_t ws2811_wait(ws2811_t *ws2811)
{
    volatile dma_t *dma = ws2811->device->dma;

    if (ws2811->device->driver_mode == SPI)  // Nothing to do for SPI
    {
        return WS2811_SUCCESS;
    }

    while ((dma->cs & RPI_DMA_CS_ACTIVE) &&
           !(dma->cs & RPI_DMA_CS_ERROR))
    {
        usleep(10);
    }

    if (dma->cs & RPI_DMA_CS_ERROR)
    {
        fprintf(stderr, "DMA Error: %08x\n", dma->debug);
        return WS2811_ERROR_DMA;
    }

    return WS2811_SUCCESS;
}

/**
 * Render the DMA buffer from the user supplied LED arrays and start the DMA
 * controller.  This will update all LEDs on both PWM channels.
 *
 * @param    ws2811  ws2811 instance pointer.
 *
 * @returns  None
 */
ws2811_return_t  ws2811_render(ws2811_t *ws2811)
{
	static uint8_t convert_table[3][256] =
	{
		{
			0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92,
			0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92,
			0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x93, 0x93, 0x93,
			0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93,
			0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93, 0x93,
			0x93, 0x93, 0x93, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A,
			0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A,
			0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9A, 0x9B, 0x9B, 0x9B, 0x9B,
			0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B,
			0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B, 0x9B,
			0x9B, 0x9B, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2,
			0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2,
			0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD2, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3,
			0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3,
			0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3, 0xD3,
			0xD3, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA,
			0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA,
			0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDA, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB,
			0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB,
			0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB
		},
		{
			0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x4D,
			0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69,
			0x69, 0x69, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x49, 0x49, 0x49,
			0x49, 0x49, 0x49, 0x49, 0x49, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D,
			0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D,
			0x6D, 0x6D, 0x6D, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x4D, 0x4D,
			0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69,
			0x69, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x49, 0x49, 0x49, 0x49,
			0x49, 0x49, 0x49, 0x49, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x69,
			0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D,
			0x6D, 0x6D, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x4D, 0x4D, 0x4D,
			0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69,
			0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x49, 0x49, 0x49, 0x49, 0x49,
			0x49, 0x49, 0x49, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x69, 0x69,
			0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D,
			0x6D, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49, 0x4D, 0x4D, 0x4D, 0x4D,
			0x4D, 0x4D, 0x4D, 0x4D, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x69, 0x6D,
			0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x49, 0x49, 0x49, 0x49, 0x49, 0x49,
			0x49, 0x49, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x4D, 0x69, 0x69, 0x69,
			0x69, 0x69, 0x69, 0x69, 0x69, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D, 0x6D
		},
		{
			0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24,
			0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6,
			0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34,
			0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6,
			0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4,
			0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26,
			0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4,
			0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36,
			0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24,
			0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6,
			0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34,
			0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6,
			0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4,
			0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26,
			0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4,
			0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36,
			0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24,
			0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6,
			0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34,
			0x36, 0xA4, 0xA6, 0xB4, 0xB6, 0x24, 0x26, 0x34, 0x36, 0xA4, 0xA6, 0xB4, 0xB6
		}
	};

    volatile uint8_t *pxl_raw = ws2811->device->pxl_raw;
    int driver_mode = ws2811->device->driver_mode;
    int i, l, chan;
    unsigned j;
    ws2811_return_t ret = WS2811_SUCCESS;
    uint32_t protocol_time = 0;
    static uint64_t previous_timestamp = 0;

    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)         // Channel
    {
        ws2811_channel_t *channel = &ws2811->channel[chan];

        int wordpos = chan; // PWM & PCM
        int bytepos = 0;    // SPI
        const int scale = (channel->brightness & 0xff) + 1;
        uint8_t array_size = 3; // Assume 3 color LEDs, RGB

        // If our shift mask includes the highest nibble, then we have 4 LEDs, RBGW.
        if (channel->strip_type & SK6812_SHIFT_WMASK)
        {
            array_size = 4;
        }

        // 1.25µs per bit
        const uint32_t channel_protocol_time = channel->count * array_size * 8 * 1.25;

        // Only using the channel which takes the longest as both run in parallel
        if (channel_protocol_time > protocol_time)
        {
            protocol_time = channel_protocol_time;
        }

        for (i = 0; i < channel->count; i++)                // Led
        {
            uint8_t color[] =
            {
                channel->gamma[(((channel->leds[i] >> channel->rshift) & 0xff) * scale) >> 8], // red
                channel->gamma[(((channel->leds[i] >> channel->gshift) & 0xff) * scale) >> 8], // green
                channel->gamma[(((channel->leds[i] >> channel->bshift) & 0xff) * scale) >> 8], // blue
                channel->gamma[(((channel->leds[i] >> channel->wshift) & 0xff) * scale) >> 8], // white
            };

            for (j = 0; j < array_size; j++)               // Color
            {
				for(l = 0; l < 3; ++l)
				{
					uint8_t pos = driver_mode == SPI ? bytepos : 3 - bytepos;
					uint8_t val = convert_table[l][color[j]];
					if ((driver_mode != PWM) && channel->invert) val = ~val;

					pxl_raw[wordpos * 4 + pos] = val;
					if(++bytepos == 4)
					{
						bytepos = 0;
						wordpos += driver_mode == PWM ? 2 : 1;
					}
				}
            }
        }
    }

    // Wait for any previous DMA operation to complete.
    if ((ret = ws2811_wait(ws2811)) != WS2811_SUCCESS)
    {
        return ret;
    }

    if (ws2811->render_wait_time != 0) {
        const uint64_t current_timestamp = get_microsecond_timestamp();
        uint64_t time_diff = current_timestamp - previous_timestamp;

        if (ws2811->render_wait_time > time_diff) {
            usleep(ws2811->render_wait_time - time_diff);
        }
    }

    if (driver_mode != SPI)
    {
        dma_start(ws2811);
    }
    else
    {
        ret = spi_transfer(ws2811);
    }

    // LED_RESET_WAIT_TIME is added to allow enough time for the reset to occur.
    previous_timestamp = get_microsecond_timestamp();
    ws2811->render_wait_time = protocol_time + LED_RESET_WAIT_TIME;

    return ret;
}

const char * ws2811_get_return_t_str(const ws2811_return_t state)
{
    const int index = -state;
    static const char * const ret_state_str[] = { WS2811_RETURN_STATES(WS2811_RETURN_STATES_STRING) };

    if (index < (int)(sizeof(ret_state_str) / sizeof(ret_state_str[0])))
    {
        return ret_state_str[index];
    }

    return "";
}


void ws2811_set_custom_gamma_factor(ws2811_t *ws2811, double gamma_factor)
{
    int chan, counter;
    for (chan = 0; chan < RPI_PWM_CHANNELS; chan++)
    {
        ws2811_channel_t *channel = &ws2811->channel[chan];

        if (channel->gamma)
        {
          for(counter = 0; counter < 256; counter++)
          {

             channel->gamma[counter] = (gamma_factor > 0)? (int)(pow((float)counter / (float)255.00, gamma_factor) * 255.00 + 0.5) : counter;

          }
        }

    }
}

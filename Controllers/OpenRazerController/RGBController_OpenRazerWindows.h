/*-----------------------------------------*\
|  RGBController_OpenRazerWindows.h         |
|                                           |
|  Generic RGB Interface for OpenRazer      |
|  kernel drivers for Chroma peripherals    |
|                                           |
|  Adam Honse (CalcProgrammer1) 6/15/2019   |
\*-----------------------------------------*/

#include "RGBController.h"
#include "OpenRazerDevices.h"

#include <fstream>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/hid.h>

typedef struct
{
    struct device_attribute* device_type;
    struct device_attribute* device_serial;
    struct device_attribute* firmware_version;

    struct device_attribute* matrix_custom_frame;
    struct device_attribute* matrix_brightness;

    struct device_attribute* matrix_effect_custom;
    struct device_attribute* matrix_effect_none;
    struct device_attribute* matrix_effect_static;
    struct device_attribute* matrix_effect_breath;
    struct device_attribute* matrix_effect_spectrum;
    struct device_attribute* matrix_effect_reactive;
    struct device_attribute* matrix_effect_wave;

    struct device_attribute* logo_led_brightness;
    struct device_attribute* logo_matrix_effect_none;
    struct device_attribute* logo_matrix_effect_static;
    struct device_attribute* logo_matrix_effect_breath;
    struct device_attribute* logo_matrix_effect_spectrum;
    struct device_attribute* logo_matrix_effect_reactive;

    struct device_attribute* scroll_led_brightness;
    struct device_attribute* scroll_matrix_effect_none;
    struct device_attribute* scroll_matrix_effect_static;
    struct device_attribute* scroll_matrix_effect_breath;
    struct device_attribute* scroll_matrix_effect_spectrum;
    struct device_attribute* scroll_matrix_effect_reactive;

    struct device_attribute* left_led_brightness;
    struct device_attribute* left_matrix_effect_none;
    struct device_attribute* left_matrix_effect_static;
    struct device_attribute* left_matrix_effect_breath;
    struct device_attribute* left_matrix_effect_spectrum;
    struct device_attribute* left_matrix_effect_reactive;
    struct device_attribute* left_matrix_effect_wave;

    struct device_attribute* right_led_brightness;
    struct device_attribute* right_matrix_effect_none;
    struct device_attribute* right_matrix_effect_static;
    struct device_attribute* right_matrix_effect_breath;
    struct device_attribute* right_matrix_effect_spectrum;
    struct device_attribute* right_matrix_effect_reactive;
    struct device_attribute* right_matrix_effect_wave;

    struct device_attribute* logo_led_effect;
    struct device_attribute* logo_led_rgb;
    struct device_attribute* logo_led_state;

    struct device_attribute* scroll_led_effect;
    struct device_attribute* scroll_led_rgb;
    struct device_attribute* scroll_led_state;
} device_fn_type;

class RGBController_OpenRazer : public RGBController
{
public:
    enum
    {
        OPEN_RAZER_MODE_CUSTOM,
        OPEN_RAZER_MODE_OFF,
        OPEN_RAZER_MODE_STATIC,
        OPEN_RAZER_MODE_BREATHING,
        OPEN_RAZER_MODE_SPECTRUM_CYCLE,
        OPEN_RAZER_MODE_WAVE,
        OPEN_RAZER_MODE_REACTIVE,
        OPEN_RAZER_MODE_FLASHING,
        OPEN_RAZER_NUM_MODES
    };

    enum
    {
        OPEN_RAZER_TYPE_MATRIX_FRAME,
        OPEN_RAZER_TYPE_MATRIX_NOFRAME,
        OPEN_RAZER_TYPE_MATRIX_STATIC,
        OPEN_RAZER_TYPE_NOMATRIX,
        OPEN_RAZER_NUM_TYPES
    };

public:
    RGBController_OpenRazer(device * open_razer_device, device_fn_type* open_razer_functions);
    ~RGBController_OpenRazer();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();

    int device_index;

private:
    void SetupMatrixDevice(device_fn_type* open_razer_functions, unsigned int rows, unsigned int cols);
    void SetupNonMatrixDevice();

    unsigned int matrix_type;
    unsigned int matrix_rows;
    unsigned int matrix_cols;

    device* open_razer_device;
    device_fn_type* open_razer_functions;

    //OpenRazer Sysfs Entries for Matrix Devices
    std::ofstream matrix_custom_frame;
    std::ofstream matrix_effect_custom;
    std::ofstream matrix_effect_breath;
    std::ofstream matrix_effect_none;
    std::ofstream matrix_effect_reactive;
    std::ofstream matrix_effect_spectrum;
    std::ofstream matrix_effect_static;
    std::ofstream matrix_effect_wave;

    //OpenRazer Sysfs Entries for Non-Matrix Devices
    std::ofstream logo_led_effect;
    std::ofstream logo_led_rgb;
    std::ofstream scroll_led_effect;
    std::ofstream scroll_led_rgb;
};

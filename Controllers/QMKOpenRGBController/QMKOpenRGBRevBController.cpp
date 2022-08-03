/*-------------------------------------------------------------------*\
|  QMKOpenRGBRevBController.cpp                                       |
|                                                                     |
|  Driver for QMK keyboards using OpenRGB Protocol (Revision B)       |
|                                                                     |
|  Kasper       10th Octobber 2020                                    |
|  Jath03       28th May 2021                                         |
\*-------------------------------------------------------------------*/

#include "RGBControllerKeyNames.h"
#include "QMKOpenRGBRevBController.h"

using namespace std::chrono_literals;

static std::map<uint8_t, std::string> QMKKeycodeToKeynameMap
{
    { 0, KEY_EN_UNUSED          }, { 1, KEY_EN_RIGHT_FUNCTION   }, { 2, KEY_EN_UNUSED           }, { 3, KEY_EN_UNUSED           },
    { 4, KEY_EN_A               }, { 5, KEY_EN_B                }, { 6, KEY_EN_C                }, { 7, KEY_EN_D                },
    { 8, KEY_EN_E               }, { 9, KEY_EN_F                }, { 10, KEY_EN_G               }, { 11, KEY_EN_H               },
    { 12, KEY_EN_I              }, { 13, KEY_EN_J               }, { 14, KEY_EN_K               }, { 15, KEY_EN_L               },
    { 16, KEY_EN_M              }, { 17, KEY_EN_N               }, { 18, KEY_EN_O               }, { 19, KEY_EN_P               },
    { 20, KEY_EN_Q              }, { 21, KEY_EN_R               }, { 22, KEY_EN_S               }, { 23, KEY_EN_T               },
    { 24, KEY_EN_U              }, { 25, KEY_EN_V               }, { 26, KEY_EN_W               }, { 27, KEY_EN_X               },
    { 28, KEY_EN_Y              }, { 29, KEY_EN_Z               }, { 30, KEY_EN_1               }, { 31, KEY_EN_2               },
    { 32, KEY_EN_3              }, { 33, KEY_EN_4               }, { 34, KEY_EN_5               }, { 35, KEY_EN_6               },
    { 36, KEY_EN_7              }, { 37, KEY_EN_8               }, { 38, KEY_EN_9               }, { 39, KEY_EN_0               },
    { 40, KEY_EN_ANSI_ENTER     }, { 41, KEY_EN_ESCAPE          }, { 42, KEY_EN_BACKSPACE       }, { 43, KEY_EN_TAB             },
    { 44, KEY_EN_SPACE          }, { 45, KEY_EN_MINUS           }, { 46, KEY_EN_EQUALS          }, { 47, KEY_EN_LEFT_BRACKET    },
    { 48, KEY_EN_RIGHT_BRACKET  }, { 49, KEY_EN_ANSI_BACK_SLASH }, { 50, KEY_EN_POUND           }, { 51, KEY_EN_SEMICOLON       },
    { 52, KEY_EN_QUOTE          }, { 53, KEY_EN_BACK_TICK       }, { 54, KEY_EN_COMMA           }, { 55, KEY_EN_PERIOD          },
    { 56, KEY_EN_FORWARD_SLASH  }, { 57, KEY_EN_CAPS_LOCK       }, { 58, KEY_EN_F1              }, { 59, KEY_EN_F2              },
    { 60, KEY_EN_F3             }, { 61, KEY_EN_F4              }, { 62, KEY_EN_F5              }, { 63, KEY_EN_F6              },
    { 64, KEY_EN_F7             }, { 65, KEY_EN_F8              }, { 66, KEY_EN_F9              }, { 67, KEY_EN_F10             },
    { 68, KEY_EN_F11            }, { 69, KEY_EN_F12             }, { 70, KEY_EN_PRINT_SCREEN    }, { 71, KEY_EN_SCROLL_LOCK     },
    { 72, KEY_EN_PAUSE_BREAK    }, { 73, KEY_EN_INSERT          }, { 74, KEY_EN_HOME            }, { 75, KEY_EN_PAGE_UP         },
    { 76, KEY_EN_DELETE         }, { 77, KEY_EN_END             }, { 78, KEY_EN_PAGE_DOWN       }, { 79, KEY_EN_RIGHT_ARROW     },
    { 80, KEY_EN_LEFT_ARROW     }, { 81, KEY_EN_DOWN_ARROW      }, { 82, KEY_EN_UP_ARROW        }, { 83, KEY_EN_NUMPAD_LOCK     },
    { 84, KEY_EN_NUMPAD_DIVIDE  }, { 85, KEY_EN_NUMPAD_TIMES    }, { 86, KEY_EN_NUMPAD_MINUS    }, { 87, KEY_EN_NUMPAD_PLUS     },
    { 88, KEY_EN_NUMPAD_ENTER   }, { 89, KEY_EN_NUMPAD_1        }, { 90, KEY_EN_NUMPAD_2        }, { 91, KEY_EN_NUMPAD_3        },
    { 92, KEY_EN_NUMPAD_4       }, { 93, KEY_EN_NUMPAD_5        }, { 94, KEY_EN_NUMPAD_6        }, { 95, KEY_EN_NUMPAD_7        },
    { 96, KEY_EN_NUMPAD_8       }, { 97, KEY_EN_NUMPAD_9        }, { 98, KEY_EN_NUMPAD_0        }, { 99, KEY_EN_NUMPAD_PERIOD   },
    { 100, KEY_EN_ISO_BACK_SLASH}, { 101, KEY_EN_MENU           }, { 102, KEY_EN_UNUSED         }, { 103, KEY_EN_UNUSED         },
    { 104, KEY_EN_UNUSED        }, { 105, KEY_EN_UNUSED         }, { 106, KEY_EN_UNUSED         }, { 107, KEY_EN_UNUSED         },
    { 108, KEY_EN_UNUSED        }, { 109, KEY_EN_UNUSED         }, { 110, KEY_EN_UNUSED         }, { 111, KEY_EN_UNUSED         },
    { 112, KEY_EN_UNUSED        }, { 113, KEY_EN_UNUSED         }, { 114, KEY_EN_UNUSED         }, { 115, KEY_EN_UNUSED         },
    { 116, KEY_EN_UNUSED        }, { 117, KEY_EN_UNUSED         }, { 118, KEY_EN_UNUSED         }, { 119, KEY_EN_UNUSED         },
    { 120, KEY_EN_UNUSED        }, { 121, KEY_EN_UNUSED         }, { 122, KEY_EN_UNUSED         }, { 123, KEY_EN_UNUSED         },
    { 124, KEY_EN_UNUSED        }, { 125, KEY_EN_UNUSED         }, { 126, KEY_EN_UNUSED         }, { 127, KEY_EN_UNUSED         },
    { 128, KEY_EN_UNUSED        }, { 129, KEY_EN_UNUSED         }, { 130, KEY_EN_UNUSED         }, { 131, KEY_EN_UNUSED         },
    { 132, KEY_EN_UNUSED        }, { 133, KEY_EN_UNUSED         }, { 134, KEY_EN_UNUSED         }, { 135, KEY_EN_UNUSED         },
    { 136, KEY_EN_UNUSED        }, { 137, KEY_EN_UNUSED         }, { 138, KEY_EN_UNUSED         }, { 139, KEY_EN_UNUSED         },
    { 140, KEY_EN_UNUSED        }, { 141, KEY_EN_UNUSED         }, { 142, KEY_EN_UNUSED         }, { 143, KEY_EN_UNUSED         },
    { 144, KEY_EN_UNUSED        }, { 145, KEY_EN_UNUSED         }, { 146, KEY_EN_UNUSED         }, { 147, KEY_EN_UNUSED         },
    { 148, KEY_EN_UNUSED        }, { 149, KEY_EN_UNUSED         }, { 150, KEY_EN_UNUSED         }, { 151, KEY_EN_UNUSED         },
    { 152, KEY_EN_UNUSED        }, { 153, KEY_EN_UNUSED         }, { 154, KEY_EN_UNUSED         }, { 155, KEY_EN_UNUSED         },
    { 156, KEY_EN_UNUSED        }, { 157, KEY_EN_UNUSED         }, { 158, KEY_EN_UNUSED         }, { 159, KEY_EN_UNUSED         },
    { 160, KEY_EN_UNUSED        }, { 161, KEY_EN_UNUSED         }, { 162, KEY_EN_UNUSED         }, { 163, KEY_EN_UNUSED         },
    { 164, KEY_EN_UNUSED        },
    /*Space Cadet Left Shift*/     { 216, KEY_EN_LEFT_SHIFT     }, /*Space Cadet Right Shift*/     { 217, KEY_EN_RIGHT_SHIFT    },
    { 224, KEY_EN_LEFT_CONTROL  }, { 225, KEY_EN_LEFT_SHIFT     }, { 226, KEY_EN_LEFT_ALT       }, { 227, KEY_EN_LEFT_WINDOWS   },
    { 228, KEY_EN_RIGHT_CONTROL }, { 229, KEY_EN_RIGHT_SHIFT    }, { 230, KEY_EN_RIGHT_ALT      }, { 231, KEY_EN_RIGHT_WINDOWS  },
};

QMKOpenRGBRevBController::QMKOpenRGBRevBController(hid_device *dev_handle, const char *path)
{
    /*-------------------------------------------------*\
    | Get QMKOpenRGB settings                           |
    \*-------------------------------------------------*/
    json qmk_settings = ResourceManager::get()->GetSettingsManager()->GetSettings("QMKOpenRGBDevices");
    if(qmk_settings.contains("leds_per_update"))
    {
        if(qmk_settings["leds_per_update"] > 20)
        {
            qmk_settings["leds_per_update"] = 20;
        }
        else if(qmk_settings["leds_per_update"] < 1)
        {
            qmk_settings["leds_per_update"] = 1;
        }
        SettingsManager* settings_manager   = ResourceManager::get()->GetSettingsManager();
        settings_manager->SetSettings("QMKOpenRGBDevices", qmk_settings);
        settings_manager->SaveSettings();
        leds_per_update = qmk_settings["leds_per_update"];
    }
    else
    {
        leds_per_update = 20;
    }

    if(qmk_settings.contains("delay"))
    {
        delay = (unsigned int)qmk_settings["delay"] * 1ms;
    }
    else
    {
        delay = 0ms;
    }

    dev         = dev_handle;
    location    = path;

    GetDeviceInfo();
    GetModeInfo();
}

QMKOpenRGBRevBController::~QMKOpenRGBRevBController()
{
    hid_close(dev);
}

std::string QMKOpenRGBRevBController::GetLocation()
{
    return location;
}

std::string QMKOpenRGBRevBController::GetDeviceName()
{
    return device_name;
}

std::string QMKOpenRGBRevBController::GetDeviceVendor()
{
    return device_vendor;
}

unsigned int QMKOpenRGBRevBController::GetTotalNumberOfLEDs()
{
    return total_number_of_leds;
}

unsigned int QMKOpenRGBRevBController::GetTotalNumberOfLEDsWithEmptySpace()
{
    return total_number_of_leds_with_empty_space;
}

unsigned int QMKOpenRGBRevBController::GetMode()
{
    return mode;
}

unsigned int QMKOpenRGBRevBController::GetModeSpeed()
{
    return mode_speed;
}

unsigned int QMKOpenRGBRevBController::GetModeColor()
{
    return mode_color;
}

std::vector<point_t> QMKOpenRGBRevBController::GetLEDPoints()
{
    return led_points;
}

std::vector<unsigned int> QMKOpenRGBRevBController::GetLEDFlags()
{
    return led_flags;
}

std::vector<std::string> QMKOpenRGBRevBController::GetLEDNames()
{
    return led_names;
}

std::vector<RGBColor>  QMKOpenRGBRevBController::GetLEDColors()
{
    return led_colors;
}

unsigned int QMKOpenRGBRevBController::GetProtocolVersion()
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_PROTOCOL_VERSION;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    return usb_buf[1];
}

std::string QMKOpenRGBRevBController::GetQMKVersion()
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_QMK_VERSION;

    hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
    hid_read(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);

    std::string qmk_version;
    int i = 1;
    while (usb_buf[i] != 0)
    {
        qmk_version.push_back(usb_buf[i]);
        i++;
    }

    return qmk_version;
}

void QMKOpenRGBRevBController::GetDeviceInfo()
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_DEVICE_INFO;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    total_number_of_leds = usb_buf[QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_BYTE];
    total_number_of_leds_with_empty_space = usb_buf[QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_WITH_EMPTY_SPACE_BYTE];

    int i = QMK_OPENRGB_TOTAL_NUMBER_OF_LEDS_WITH_EMPTY_SPACE_BYTE + 1;
    while (usb_buf[i] != 0)
    {
        device_name.push_back(usb_buf[i]);
        i++;
    }

    i++;
    while (usb_buf[i] != 0)
    {
        device_vendor.push_back(usb_buf[i]);
        i++;
    }
}

void QMKOpenRGBRevBController::GetModeInfo()
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_MODE_INFO;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, 65);
        bytes_read = hid_read_timeout(dev, usb_buf, 65, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    mode = usb_buf[QMK_OPENRGB_MODE_BYTE];
    mode_speed = usb_buf[QMK_OPENRGB_SPEED_BYTE];

    /*-----------------------------------------------------*\
    | QMK hue range is between 0-255 so hue needs to be     |
    | converted                                             |
    \*-----------------------------------------------------*/
    unsigned int oldRange       = 255;
    unsigned int newRange       = 359;
    unsigned int convertedHue   = (usb_buf[QMK_OPENRGB_HUE_BYTE] * newRange / oldRange);

    hsv_t hsv;
    hsv.hue                     = convertedHue;
    hsv.saturation              = usb_buf[QMK_OPENRGB_SATURATION_BYTE];
    hsv.value                   = usb_buf[QMK_OPENRGB_VALUE_BYTE];

    mode_color                  = hsv2rgb(&hsv);
}

void QMKOpenRGBRevBController::GetLEDInfo(unsigned int leds_count)
{
    unsigned int leds_sent           = 0;
    unsigned int leds_per_update_info     = 8;

    while (leds_sent < leds_count)
    {
        if ((leds_count - leds_sent) < leds_per_update_info)
        {
            leds_per_update_info = leds_count - leds_sent;
        }

        unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_OPENRGB_GET_LED_INFO;
        usb_buf[0x02] = leds_sent;
        usb_buf[0x03] = leds_per_update_info;

        int bytes_read = 0;
        do
        {
            hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
            bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
        } while(bytes_read <= 0);

        for (unsigned int led_idx = 0; led_idx < leds_per_update_info; led_idx++)
        {
            if(usb_buf[(led_idx * 7) + QMK_OPENRGB_FLAG_BYTE] != QMK_OPENRGB_FAILURE)
            {
                led_points.push_back(point_t{usb_buf[(led_idx * 7) + QMK_OPENRGB_POINT_X_BYTE], usb_buf[(led_idx * 7) + QMK_OPENRGB_POINT_Y_BYTE]});
                led_flags.push_back(usb_buf[(led_idx * 7) + QMK_OPENRGB_FLAG_BYTE]);
                led_colors.push_back(ToRGBColor(usb_buf[(led_idx * 7) + QMK_OPENRGB_R_COLOR_BYTE], usb_buf[(led_idx * 7) + QMK_OPENRGB_G_COLOR_BYTE], usb_buf[(led_idx * 7) + QMK_OPENRGB_B_COLOR_BYTE]));
            }

            if(usb_buf[(led_idx * 7) + QMK_OPENRGB_KEYCODE_BYTE] != 0)
            {
                if (QMKKeycodeToKeynameMap.count(usb_buf[(led_idx * 7) + QMK_OPENRGB_KEYCODE_BYTE]) > 0)
                {
                    led_names.push_back(QMKKeycodeToKeynameMap[usb_buf[(led_idx * 7) + QMK_OPENRGB_KEYCODE_BYTE]]);
                }
                else
                {
                    led_names.push_back(KEY_EN_UNUSED);
                }
            }
        }

        leds_sent += leds_per_update_info;
    }
}

std::vector<unsigned int> QMKOpenRGBRevBController::GetEnabledModes()
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_GET_ENABLED_MODES;

    int bytes_read = 0;
    do
    {
        hid_write(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE);
        bytes_read = hid_read_timeout(dev, usb_buf, QMK_OPENRGB_PACKET_SIZE, QMK_OPENRGB_HID_READ_TIMEOUT);
    } while(bytes_read <= 0);

    std::vector<unsigned int> enabled_modes;
    int i = 1;
    while (usb_buf[i] != 0)
    {
        enabled_modes.push_back(usb_buf[i]);
        i++;
    }
    return enabled_modes;
}

void QMKOpenRGBRevBController::SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed, bool save)
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/
    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_SET_MODE;
    usb_buf[0x02] = hsv_color.hue * 255 / 359;
    usb_buf[0x03] = hsv_color.saturation;
    usb_buf[0x04] = hsv_color.value;
    usb_buf[0x05] = mode;
    usb_buf[0x06] = speed;
    usb_buf[0x07] = save;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read_timeout(dev, usb_buf, 65, QMK_OPENRGB_HID_READ_TIMEOUT);
}

void QMKOpenRGBRevBController::DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue)
{
    unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

    /*-----------------------------------------------------*\
    | Set up config table request packet                    |
    \*-----------------------------------------------------*/

    usb_buf[0x00] = 0x00;
    usb_buf[0x01] = QMK_OPENRGB_DIRECT_MODE_SET_SINGLE_LED;
    usb_buf[0x02] = led;
    usb_buf[0x03] = red;
    usb_buf[0x04] = green;
    usb_buf[0x05] = blue;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 65);
    hid_read_timeout(dev, usb_buf, 65, QMK_OPENRGB_HID_READ_TIMEOUT);
}

void QMKOpenRGBRevBController::DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int leds_count)
{
    unsigned int leds_sent           = 0;
    unsigned int tmp_leds_per_update = leds_per_update;

    while (leds_sent < leds_count)
    {
        if ((leds_count - leds_sent) < tmp_leds_per_update)
        {
            tmp_leds_per_update = leds_count - leds_sent;
        }

        unsigned char usb_buf[QMK_OPENRGB_PACKET_SIZE];

        /*-----------------------------------------------------*\
        | Zero out buffer                                       |
        \*-----------------------------------------------------*/
        memset(usb_buf, 0x00, QMK_OPENRGB_PACKET_SIZE);

        /*-----------------------------------------------------*\
        | Set up config table request packet                    |
        \*-----------------------------------------------------*/
        usb_buf[0x00] = 0x00;
        usb_buf[0x01] = QMK_OPENRGB_DIRECT_MODE_SET_LEDS;
        usb_buf[0x02] = leds_sent;
        usb_buf[0x03] = tmp_leds_per_update;

        for (unsigned int led_idx = 0; led_idx < tmp_leds_per_update; led_idx++)
        {
            usb_buf[(led_idx * 3) + 4] = RGBGetRValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 5] = RGBGetGValue(colors[led_idx + leds_sent]);
            usb_buf[(led_idx * 3) + 6] = RGBGetBValue(colors[led_idx + leds_sent]);
        }

        hid_write(dev, usb_buf, 65);

        if(delay > 0ms)
        {
            std::this_thread::sleep_for(delay);
        }

        leds_sent += tmp_leds_per_update;
    }
}

#include <vector>
#include <cstring>
#include <string>
#include <tuple>
#include <iostream>
#include "OpenRGB.h"
#include "RGBController.h"
#include "i2c_smbus.h"

static std::vector<RGBController*> rgb_controllers;

struct DeviceOptions
{
    int device;
    std::vector<std::tuple<unsigned char, unsigned char, unsigned char>> colors;
    std::string mode;
    bool hasOption;
};

struct Options
{
    std::vector<DeviceOptions> devices;

    // if hasDevice is false, devices above is empty and allDeviceOptions
    // shall be applied to all available devices
    bool hasDevice;
    DeviceOptions allDeviceOptions;
};

void PrintHelp()
{
    std::string help_text;
    help_text += "OpenRGB ";
    help_text += VERSION_STRING;
    help_text += ", for controlling RGB lighting.\n";
    help_text += "Usage: OpenRGB (--device [--mode] [--color])...\n";
    help_text += "\nOptions:\n";
    help_text += "--gui\t\t\t\t\t Show GUI, also appears when not passing any parameters\n";
    help_text += "-l,  --list-devices\t\t\t Lists every compatible device with their number\n";
    help_text += "-d,  --device [0-9]\t\t\t Selects device to apply colors and/or effect to, or applies to all devices if omitted\n";
    help_text += "\t\t\t\t\t Can be specified multiple times with different modes and colors\n";
    help_text += "-c,  --color \"FFFFFF,00AAFF...\"\t\t Sets colors on each device directly if no effect is specified, and sets the effect color if an effect is specified\n";
    help_text += "\t\t\t\t\t If there are more LEDs than colors given, the last color will be applied to the remaining LEDs\n";
    help_text += "-m,  --mode [breathing | static | ...]   Sets the mode to be applied, check --list-devices to see which modes are supported on your device\n";
    help_text += "-v,  --version\t\t\t\t Display version and software build information\n";

    std::cout << help_text << std::endl;
}

void PrintVersion()
{
    std::string version_text;
    version_text += "OpenRGB ";
    version_text += VERSION_STRING;
    version_text += ", for controlling RGB lighting.\n";
    version_text += "  Version:\t\t ";
    version_text += VERSION_STRING;
    version_text += "\n  Build Date\t\t ";
    version_text += BUILDDATE_STRING;
    version_text += "\n  Git Commit ID\t\t ";
    version_text += GIT_COMMIT_ID;
    version_text += "\n  Git Commit Date\t ";
    version_text += GIT_COMMIT_DATE;
    version_text += "\n  Git Branch\t\t ";
    version_text += GIT_BRANCH;
    version_text += "\n";

    std::cout << version_text << std::endl;
}

bool ParseColors(std::string colors_string, DeviceOptions *options)
{
    while (colors_string.length() >= 6)
    {
        int rgb_end = colors_string.find_first_of(',');
        std::string color = colors_string.substr(0, rgb_end);
        if (color.length() != 6)
            break;

        try
        {
            unsigned char r = std::stoi(color.substr(0, 2), nullptr, 16);
            unsigned char g = std::stoi(color.substr(2, 2), nullptr, 16);
            unsigned char b = std::stoi(color.substr(4, 2), nullptr, 16);
            options->colors.push_back(std::make_tuple(r, g, b));
        }
        catch (...)
        {
            break;
        }

        // If there are no more colors
        if (rgb_end == std::string::npos)
            break;
        // Remove the current color and the next color's leading comma
        colors_string = colors_string.substr(color.length() + 1);
    }

    return options->colors.size() > 0;
}

int ParseMode(DeviceOptions& options)
{
    auto availableModes = rgb_controllers[options.device]->modes;
    for (int i = 0; i < availableModes.size(); i++)
    {
        if (availableModes[i].name == options.mode)
        {
            return i;
        }
    }

    std::cout << "Error: Mode '" + options.mode + "' not available for device '" +
        rgb_controllers[options.device]->name + "'" << std::endl;
    return false;
}

DeviceOptions* GetDeviceOptionsForDevID(Options *opts, int device)
{
    if (device == -1)
    {
        return &opts->allDeviceOptions;
    }

    for (int i = 0; i < opts->devices.size(); i++)
    {
        if (opts->devices[i].device == device)
        {
            return &opts->devices[i];
        }
    }

    // should never happen
    std::cout << "Internal error: Tried setting an option on a device that wasn't specified" << std::endl;
    abort();
}

std::string QuoteIfNecessary(std::string str)
{
    if (str.find(' ') == std::string::npos)
    {
        return str;
    }
    else
    {
        return "'" + str + "'";
    }
}

bool ProcessOptions(int argc, char *argv[], Options *res)
{
    int arg_index = 1;
    int currentDev = -1;

    while (arg_index < argc)
    {
        std::string option = argv[arg_index];

        // Handle options that take no arguments
        if (option == "--list-devices" || option == "-l")
        {
            for (int i = 0; i < rgb_controllers.size(); i++)
            {
                RGBController *c = rgb_controllers[i];
                std::cout << i << ": " << c->name << std::endl;

                std::cout << "  Type: " << device_type_to_str(c->type) << std::endl;

                if (!c->serial.empty())
                {
                    std::cout << "  Serial:      " << c->serial << std::endl;
                }

                if (!c->description.empty())
                {
                    std::cout << "  Description: " << c->description << std::endl;
                }

                if (!c->modes.empty())
                {
                    std::cout << "  Modes:";
                    int curMode = c->GetMode();
                    for (int i = 0; i < c->modes.size(); i++)
                    {
                        std::string modeStr = QuoteIfNecessary(c->modes[i].name);
                        if (curMode == i) {
                            modeStr = "[" + modeStr + "]";
                        }
                        std::cout << " " << modeStr;
                    }
                    std::cout << std::endl;
                }

                if (!c->zones.empty())
                {
                    std::cout << "  Zones:";
                    for (int i = 0; i < c->zones.size(); i++)
                    {
                        std::cout << " " << QuoteIfNecessary(c->zones[i].name);
                    }
                    std::cout << std::endl;
                }

                if (!c->leds.empty())
                {
                    std::cout << "  LEDs:";
                    for (int i = 0; i < c->leds.size(); i++)
                    {
                        std::cout << " " << QuoteIfNecessary(c->leds[i].name);
                    }
                    std::cout << std::endl;
                }

                std::cout << std::endl;
            }
            exit(0);
        }
        else if (arg_index + 1 < argc) // Handle options that take an argument
        {
            std::string argument = argv[arg_index + 1];

            if (option == "--device" || option == "-d")
            {
                try
                {
                    currentDev = std::stoi(argument);

                    if (rgb_controllers.size() <= currentDev || currentDev < 0)
                    {
                        throw;
                    }

                    DeviceOptions newDev;
                    newDev.device = currentDev;

                    if (!res->hasDevice)
                    {
                        res->hasDevice = true;
                    }

                    res->devices.push_back(newDev);
                }
                catch (...)
                {
                    std::cout << "Error: Invalid device ID: " + argument << std::endl;
                    return false;
                }
            }
            else if (option == "--color" || option == "-c")
            {
                DeviceOptions* currentDevOpts = GetDeviceOptionsForDevID(res, currentDev);
                if (ParseColors(argument, currentDevOpts))
                {
                    currentDevOpts->hasOption = true;
                }
                else
                {
                    std::cout << "Error: Invalid argument to " + option + ": " + argument << std::endl;
                    return false;
                }
            }
            else if (option == "--mode" || option == "-m")
            {
                DeviceOptions* currentDevOpts = GetDeviceOptionsForDevID(res, currentDev);
                currentDevOpts->mode = argument;
                currentDevOpts->hasOption = true;
            }
            else
            {
                std::cout << "Error: Invalid option: " + option << std::endl;
                return false;
            }

            arg_index++;
        }
        else
        {
            return false;
        }
        arg_index++;
    }

    if (res->hasDevice)
    {
        for (int i = 0; i < res->devices.size(); i++)
        {
            if (!res->devices[i].hasOption)
            {
                std::cout << "Error: Device " + std::to_string(i) + " specified, but neither mode nor color given" << std::endl;
                return false;
            }
        }
    }
    else
    {
        return res->allDeviceOptions.hasOption;
    }

    return true;
}

void ApplyOptions(DeviceOptions& options)
{
    RGBController *device = rgb_controllers[options.device];

    // Set mode first, in case it's 'direct' (which affects SetLED below)
    int mode = ParseMode(options);
    device->SetMode(mode);

    if (options.colors.size() != 0)
    {
        int last_set_color;
        for (int i = 0; i < device->leds.size(); i++)
        {
            if (i < options.colors.size())
            {
                last_set_color = i;
            }

            device->SetLED(i, ToRGBColor(std::get<0>(options.colors[last_set_color]),
                                         std::get<1>(options.colors[last_set_color]),
                                         std::get<2>(options.colors[last_set_color])));
        }
    }
}

int cli_main(int argc, char *argv[], std::vector<RGBController *> rgb_controllers_in)
{
    rgb_controllers = rgb_controllers_in;
    
    if (argc == 2 && (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-h")))
    {
        PrintHelp();
        return 0;
    }

    if (argc == 2 && (!strcmp(argv[1], "--version") || !strcmp(argv[1], "-v")))
    {
        PrintVersion();
        return 0;
    }

    Options options;
    if (!ProcessOptions(argc, argv, &options))
    {
        PrintHelp();
        return -1;
    }

    if (options.hasDevice)
    {
        for (int i = 0; i < options.devices.size(); i++)
        {
            ApplyOptions(options.devices[i]);
        }
    }
    else
    {
        for (int i = 0; i < rgb_controllers.size(); i++)
        {
            options.allDeviceOptions.device = i;
            ApplyOptions(options.allDeviceOptions);
        }
    }

    return 0;
}

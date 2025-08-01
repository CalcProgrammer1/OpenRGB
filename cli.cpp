/*---------------------------------------------------------*\
| cli.cpp                                                   |
|                                                           |
|   OpenRGB command line interface                          |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <vector>
#include <cstring>
#include <string>
#include <tuple>
#include <iostream>
#include "AutoStart.h"
#include "filesystem.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "RGBController.h"
#include "i2c_smbus.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "LogManager.h"
#include "Colors.h"

/*-------------------------------------------------------------*\
| Quirk for MSVC; which doesn't support this case-insensitive   |
| function                                                      |
\*-------------------------------------------------------------*/
#ifdef _WIN32
#include <shellapi.h>
    #define strcasecmp _strcmpi
#endif

using namespace std::chrono_literals;

static std::string                 profile_save_filename = "";
const unsigned int                 brightness_percentage = 100;
const unsigned int                 speed_percentage      = 100;

static int preserve_argc = 0;
static char** preserve_argv = nullptr;

enum
{
    RET_FLAG_PRINT_HELP         = 1,
    RET_FLAG_START_GUI          = 2,
    RET_FLAG_I2C_TOOLS          = 4,
    RET_FLAG_START_MINIMIZED    = 8,
    RET_FLAG_NO_DETECT          = 16,
    RET_FLAG_CLI_POST_DETECTION = 32,
    RET_FLAG_START_SERVER       = 64,
    RET_FLAG_NO_AUTO_CONNECT    = 128,
};

struct DeviceOptions
{
    int             device;
    int             zone            = -1;
    std::vector<std::tuple<unsigned char, unsigned char, unsigned char>> colors;
    std::string     mode;
    unsigned int    speed           = 100;
    unsigned int    brightness      = 100;
    unsigned int    size;
    bool            random_colors   = false;
    bool            hasSize         = false;
    bool            hasOption       = false;
};

struct ServerOptions
{
    bool start = false;
    unsigned short  port = OPENRGB_SDK_PORT;
};

struct Options
{
    std::vector<DeviceOptions>  devices;

    /*---------------------------------------------------------*\
    | If hasDevice is false, devices above is empty and         |
    | allDeviceOptions shall be applied to all available devices|
    | except in the case that a profile was loaded.             |
    \*---------------------------------------------------------*/
    bool                        hasDevice        = false;
    bool                        profile_loaded  = false;
    DeviceOptions               allDeviceOptions;
    ServerOptions               servOpts;
};

/*---------------------------------------------------------------------------------------------------------*\
| Support a common subset of human colors; for easier typing: https://www.w3.org/TR/css-color-3/#svg-color  |
\*---------------------------------------------------------------------------------------------------------*/
struct HumanColors { uint32_t rgb; const char* keyword; } static const human_colors[] =
{
    { COLOR_BLACK, "black" },
    { COLOR_NAVY, "navy" },
    { COLOR_DARKBLUE, "darkblue" },
    { COLOR_MEDIUMBLUE, "mediumblue" },
    { COLOR_BLUE, "blue" },
    { COLOR_DARKGREEN, "darkgreen" },
    { COLOR_GREEN, "green" },
    { COLOR_TEAL, "teal" },
    { COLOR_DARKCYAN, "darkcyan" },
    { COLOR_DEEPSKYBLUE, "deepskyblue" },
    { COLOR_DARKTURQUOISE, "darkturquoise" },
    { COLOR_MEDIUMSPRINGGREEN, "mediumspringgreen" },
    { COLOR_LIME, "lime" },
    { COLOR_SPRINGGREEN, "springgreen" },
    { COLOR_AQUA, "aqua" },
    { COLOR_CYAN, "cyan" },
    { COLOR_MIDNIGHTBLUE, "midnightblue" },
    { COLOR_DODGERBLUE, "dodgerblue" },
    { COLOR_LIGHTSEAGREEN, "lightseagreen" },
    { COLOR_FORESTGREEN, "forestgreen" },
    { COLOR_SEAGREEN, "seagreen" },
    { COLOR_DARKSLATEGRAY, "darkslategray" },
    { COLOR_DARKSLATEGREY, "darkslategrey" },
    { COLOR_LIMEGREEN, "limegreen" },
    { COLOR_MEDIUMSEAGREEN, "mediumseagreen" },
    { COLOR_TURQUOISE, "turquoise" },
    { COLOR_ROYALBLUE, "royalblue" },
    { COLOR_STEELBLUE, "steelblue" },
    { COLOR_DARKSLATEBLUE, "darkslateblue" },
    { COLOR_MEDIUMTURQUOISE, "mediumturquoise" },
    { COLOR_INDIGO, "indigo" },
    { COLOR_DARKOLIVEGREEN, "darkolivegreen" },
    { COLOR_CADETBLUE, "cadetblue" },
    { COLOR_CORNFLOWERBLUE, "cornflowerblue" },
    { COLOR_MEDIUMAQUAMARINE, "mediumaquamarine" },
    { COLOR_DIMGRAY, "dimgray" },
    { COLOR_DIMGREY, "dimgrey" },
    { COLOR_SLATEBLUE, "slateblue" },
    { COLOR_OLIVEDRAB, "olivedrab" },
    { COLOR_SLATEGRAY, "slategray" },
    { COLOR_SLATEGREY, "slategrey" },
    { COLOR_LIGHTSLATEGRAY, "lightslategray" },
    { COLOR_LIGHTSLATEGREY, "lightslategrey" },
    { COLOR_MEDIUMSLATEBLUE, "mediumslateblue" },
    { COLOR_LAWNGREEN, "lawngreen" },
    { COLOR_CHARTREUSE, "chartreuse" },
    { COLOR_AQUAMARINE, "aquamarine" },
    { COLOR_MAROON, "maroon" },
    { COLOR_PURPLE, "purple" },
    { COLOR_ELECTRIC_ULTRAMARINE, "electricultramarine" },
    { COLOR_OLIVE, "olive" },
    { COLOR_GRAY, "gray" },
    { COLOR_GREY, "grey" },
    { COLOR_SKYBLUE, "skyblue" },
    { COLOR_LIGHTSKYBLUE, "lightskyblue" },
    { COLOR_BLUEVIOLET, "blueviolet" },
    { COLOR_DARKRED, "darkred" },
    { COLOR_DARKMAGENTA, "darkmagenta" },
    { COLOR_SADDLEBROWN, "saddlebrown" },
    { COLOR_DARKSEAGREEN, "darkseagreen" },
    { COLOR_LIGHTGREEN, "lightgreen" },
    { COLOR_MEDIUMPURPLE, "mediumpurple" },
    { COLOR_DARKVIOLET, "darkviolet" },
    { COLOR_PALEGREEN, "palegreen" },
    { COLOR_DARKORCHID, "darkorchid" },
    { COLOR_YELLOWGREEN, "yellowgreen" },
    { COLOR_SIENNA, "sienna" },
    { COLOR_BROWN, "brown" },
    { COLOR_DARKGRAY, "darkgray" },
    { COLOR_DARKGREY, "darkgrey" },
    { COLOR_LIGHTBLUE, "lightblue" },
    { COLOR_GREENYELLOW, "greenyellow" },
    { COLOR_PALETURQUOISE, "paleturquoise" },
    { COLOR_LIGHTSTEELBLUE, "lightsteelblue" },
    { COLOR_POWDERBLUE, "powderblue" },
    { COLOR_FIREBRICK, "firebrick" },
    { COLOR_DARKGOLDENROD, "darkgoldenrod" },
    { COLOR_MEDIUMORCHID, "mediumorchid" },
    { COLOR_ROSYBROWN, "rosybrown" },
    { COLOR_DARKKHAKI, "darkkhaki" },
    { COLOR_SILVER, "silver" },
    { COLOR_MEDIUMVIOLETRED, "mediumvioletred" },
    { COLOR_INDIANRED, "indianred" },
    { COLOR_PERU, "peru" },
    { COLOR_CHOCOLATE, "chocolate" },
    { COLOR_TAN, "tan" },
    { COLOR_LIGHTGRAY, "lightgray" },
    { COLOR_LIGHTGREY, "lightgrey" },
    { COLOR_THISTLE, "thistle" },
    { COLOR_ORCHID, "orchid" },
    { COLOR_GOLDENROD, "goldenrod" },
    { COLOR_PALEVIOLETRED, "palevioletred" },
    { COLOR_CRIMSON, "crimson" },
    { COLOR_GAINSBORO, "gainsboro" },
    { COLOR_PLUM, "plum" },
    { COLOR_BURLYWOOD, "burlywood" },
    { COLOR_LIGHTCYAN, "lightcyan" },
    { COLOR_LAVENDER, "lavender" },
    { COLOR_DARKSALMON, "darksalmon" },
    { COLOR_VIOLET, "violet" },
    { COLOR_PALEGOLDENROD, "palegoldenrod" },
    { COLOR_LIGHTCORAL, "lightcoral" },
    { COLOR_KHAKI, "khaki" },
    { COLOR_ALICEBLUE, "aliceblue" },
    { COLOR_HONEYDEW, "honeydew" },
    { COLOR_AZURE, "azure" },
    { COLOR_SANDYBROWN, "sandybrown" },
    { COLOR_WHEAT, "wheat" },
    { COLOR_BEIGE, "beige" },
    { COLOR_WHITESMOKE, "whitesmoke" },
    { COLOR_MINTCREAM, "mintcream" },
    { COLOR_GHOSTWHITE, "ghostwhite" },
    { COLOR_SALMON, "salmon" },
    { COLOR_ANTIQUEWHITE, "antiquewhite" },
    { COLOR_LINEN, "linen" },
    { COLOR_LIGHTGOLDENRODYELLOW, "lightgoldenrodyellow" },
    { COLOR_OLDLACE, "oldlace" },
    { COLOR_RED, "red" },
    { COLOR_FUCHSIA, "fuchsia" },
    { COLOR_MAGENTA, "magenta" },
    { COLOR_DEEPPINK, "deeppink" },
    { COLOR_ORANGERED, "orangered" },
    { COLOR_TOMATO, "tomato" },
    { COLOR_HOTPINK, "hotpink" },
    { COLOR_CORAL, "coral" },
    { COLOR_DARKORANGE, "darkorange" },
    { COLOR_LIGHTSALMON, "lightsalmon" },
    { COLOR_ORANGE, "orange" },
    { COLOR_LIGHTPINK, "lightpink" },
    { COLOR_PINK, "pink" },
    { COLOR_GOLD, "gold" },
    { COLOR_PEACHPUFF, "peachpuff" },
    { COLOR_NAVAJOWHITE, "navajowhite" },
    { COLOR_MOCCASIN, "moccasin" },
    { COLOR_BISQUE, "bisque" },
    { COLOR_MISTYROSE, "mistyrose" },
    { COLOR_BLANCHEDALMOND, "blanchedalmond" },
    { COLOR_PAPAYAWHIP, "papayawhip" },
    { COLOR_LAVENDERBLUSH, "lavenderblush" },
    { COLOR_SEASHELL, "seashell" },
    { COLOR_CORNSILK, "cornsilk" },
    { COLOR_LEMONCHIFFON, "lemonchiffon" },
    { COLOR_FLORALWHITE, "floralwhite" },
    { COLOR_SNOW, "snow" },
    { COLOR_YELLOW, "yellow" },
    { COLOR_LIGHTYELLOW, "lightyellow" },
    { COLOR_IVORY, "ivory" },
    { COLOR_WHITE, "white" },
    { 0, NULL }
};

bool ParseColors(std::string colors_string, DeviceOptions *options)
{
    while (colors_string.length() > 0)
    {
        size_t    rgb_end = colors_string.find_first_of(',');
        std::string color = colors_string.substr(0, rgb_end);
        int32_t rgb = 0;

        bool parsed = false;

        if (color.length() <= 0)
            break;

        /*-----------------------------------------------------------------*\
        | This will set correct colour mode for modes with a                |
        |   MODE_COLORS_RANDOM else generate a random colour from the       |
        |   human_colors list above                                         |
        \*-----------------------------------------------------------------*/
        if (color == "random")
        {
            options->random_colors = true;
            srand((unsigned int)time(NULL));
            int index = rand() % (sizeof(human_colors) / sizeof(human_colors[0])) + 1; //Anything other than black
            rgb = human_colors[index].rgb;
            parsed = true;
        }
        else
        {
            /* swy: (A) try interpreting it as text; as human keywords, otherwise strtoul() will pick up 'darkgreen' as 0xDA */
            for (const struct HumanColors *hc = human_colors; hc->keyword != NULL; hc++)
            {
                if (strcasecmp(hc->keyword, color.c_str()) != 0)
                    continue;

                rgb = hc->rgb; parsed = true;

                break;
            }
        }

        /* swy: (B) no luck, try interpreting it as an hexadecimal number instead */
        if (!parsed)
        {
            if (color.length() == 6)
            {
                const char *colorptr = color.c_str(); char *endptr = NULL;

                rgb = strtoul(colorptr, &endptr, 16);

                /* swy: check that strtoul() has advanced the read pointer until the end (NULL terminator);
                        that means it has read the whole thing */
                if (colorptr != endptr && endptr && *endptr == '\0')
                    parsed = true;
            }
        }

        /* swy: we got it, save the 32-bit integer as a tuple of three RGB bytes */
        if (parsed)
        {
            options->colors.push_back(std::make_tuple(
                (rgb >> (8 * 2)) & 0xFF, /* RR.... */
                (rgb >> (8 * 1)) & 0xFF, /* ..GG.. */
                (rgb >> (8 * 0)) & 0xFF  /* ....BB */
            ));
        }
        else
        {
            std::cout << "Error: Unknown color: '" + color + "', skipping." << std::endl;
        }

        // If there are no more colors
        if (rgb_end == std::string::npos)
            break;

        // Remove the current color and the next color's leading comma
        colors_string = colors_string.substr(color.length() + 1);
    }

    return options->colors.size() > 0;
}

unsigned int ParseMode(DeviceOptions& options, std::vector<RGBController *> &rgb_controllers)
{
    // no need to check if --mode wasn't passed
    if (options.mode.size() == 0)
    {
        return rgb_controllers[options.device]->active_mode;
    }

    /*---------------------------------------------------------*\
    | Search through all of the device modes and see if there is|
    | a match.  If no match is found, print an error message.   |
    \*---------------------------------------------------------*/
    for(unsigned int mode_idx = 0; mode_idx < rgb_controllers[options.device]->modes.size(); mode_idx++)
    {
        if (strcasecmp(rgb_controllers[options.device]->modes[mode_idx].name.c_str(), options.mode.c_str()) == 0)
        {
            return mode_idx;
        }
    }

    std::cout << "Error: Mode '" + options.mode + "' not available for device '" + rgb_controllers[options.device]->GetName() + "'" << std::endl;
    return false;
}

DeviceOptions* GetDeviceOptionsForDevID(Options *opts, int device)
{
    if (device == -1)
    {
        return &opts->allDeviceOptions;
    }

    for (unsigned int i = 0; i < opts->devices.size(); i++)
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

/*---------------------------------------------------------------------------------------------------------*\
| Option processing functions                                                                               |
\*---------------------------------------------------------------------------------------------------------*/

void OptionHelp()
{
    std::string help_text;
    help_text += "OpenRGB ";
    help_text += VERSION_STRING;
    help_text += ", for controlling RGB lighting.\n";
    help_text += "Usage: OpenRGB (--device [--mode] [--color])...\n";
    help_text += "\n";
    help_text += "Options:\n";
    help_text += "--gui                                    Shows the GUI. GUI also appears when not passing any parameters\n";
    help_text += "--startminimized                         Starts the GUI minimized to tray. Implies --gui, even if not specified\n";
    help_text += "--client [IP]:[Port]                     Starts an SDK client on the given IP:Port (assumes port 6742 if not specified)\n";
    help_text += "--server                                 Starts the SDK's server\n";
    help_text += "--server-host                            Sets the SDK's server host. Default: 0.0.0.0 (all network interfaces)\n";
    help_text += "--server-port                            Sets the SDK's server port. Default: 6742 (1024-65535)\n";
    help_text += "-l,  --list-devices                      Lists every compatible device with their number\n";
    help_text += "-d,  --device [0-9 | \"name\"]             Selects device to apply colors and/or effect to, or applies to all devices if omitted\n";
    help_text += "                                           Basic string search is implemented 3 characters or more\n";
    help_text += "                                           Can be specified multiple times with different modes and colors\n";
    help_text += "-z,  --zone [0-9]                        Selects zone to apply colors and/or sizes to, or applies to all zones in device if omitted\n";
    help_text += "                                           Must be specified after specifying a device\n";
    help_text += "-c,  --color [random | FFFFF,00AAFF ...] Sets colors on each device directly if no effect is specified, and sets the effect color if an effect is specified\n";
    help_text += "                                           If there are more LEDs than colors given, the last color will be applied to the remaining LEDs\n";
    help_text += "-m,  --mode [breathing | static | ...]   Sets the mode to be applied, check --list-devices to see which modes are supported on your device\n";
    help_text += "-b,  --brightness [0-100]                Sets the brightness as a percentage if the mode supports brightness\n";
    help_text += "-s, --speed [0-100]                      Sets the speed as a percentage if the mode supports speed\n";
    help_text += "-sz,  --size [0-N]                       Sets the new size of the specified device zone.\n";
    help_text += "                                           Must be specified after specifying a zone.\n";
    help_text += "                                           If the specified size is out of range, or the zone does not offer resizing capability, the size will not be changed\n";
    help_text += "-V,  --version                           Display version and software build information\n";
    help_text += "-p,  --profile filename[.orp]            Load the profile from filename/filename.orp\n";
    help_text += "-sp, --save-profile filename.orp         Save the given settings to profile filename.orp\n";
    help_text += "--i2c-tools                              Shows the I2C/SMBus Tools page in the GUI. Implies --gui, even if not specified.\n";
    help_text += "                                           USE I2C TOOLS AT YOUR OWN RISK! Don't use this option if you don't know what you're doing!\n";
    help_text += "                                           There is a risk of bricking your motherboard, RGB controller, and RAM if you send invalid SMBus/I2C transactions.\n";
    help_text += "--localconfig                            Use the current working directory instead of the global configuration directory.\n";
    help_text += "--config path                            Use a custom path instead of the global configuration directory.\n";
    help_text += "--nodetect                               Do not try to detect hardware at startup.\n";
    help_text += "--noautoconnect                          Do not try to autoconnect to a local server at startup.\n";
    help_text += "--loglevel [0-6 | error | warning ...]   Set the log level (0: fatal to 6: trace).\n";
    help_text += "--print-source                           Print the source code file and line number for each log entry.\n";
    help_text += "-v,  --verbose                           Print log messages to stdout.\n";
    help_text += "-vv, --very-verbose                      Print debug messages and log messages to stdout.\n";
    help_text += "--autostart-check                        Check if OpenRGB starting at login is enabled.\n";
    help_text += "--autostart-disable                      Disable OpenRGB starting at login.\n";
    help_text += "--autostart-enable arguments             Enable OpenRGB to start at login. Requires arguments to give to OpenRGB at login.\n";

    std::cout << help_text << std::endl;
}

void OptionVersion()
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

void OptionListDevices(std::vector<RGBController *>& rgb_controllers)
{
    ResourceManager::get()->WaitForDeviceDetection();

    for(std::size_t controller_idx = 0; controller_idx < rgb_controllers.size(); controller_idx++)
    {
        RGBController *controller = rgb_controllers[controller_idx];

        /*---------------------------------------------------------*\
        | Print device name                                         |
        \*---------------------------------------------------------*/
        std::cout << controller_idx << ": " << controller->GetName() << std::endl;

        /*---------------------------------------------------------*\
        | Print device type                                         |
        \*---------------------------------------------------------*/
            std::cout << "  Type:           " << device_type_to_str(controller->type) << std::endl;

        /*---------------------------------------------------------*\
        | Print device description                                  |
        \*---------------------------------------------------------*/
        if(!controller->GetDescription().empty())
        {
            std::cout << "  Description:    " << controller->GetDescription() << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device version                                      |
        \*---------------------------------------------------------*/
        if(!controller->GetVersion().empty())
        {
            std::cout << "  Version:        " << controller->GetLocation() << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device location                                     |
        \*---------------------------------------------------------*/
        if(!controller->GetLocation().empty())
        {
            std::cout << "  Location:       " << controller->GetLocation() << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device serial                                       |
        \*---------------------------------------------------------*/
        if(!controller->GetSerial().empty())
        {
            std::cout << "  Serial:         " << controller->GetSerial() << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device modes                                        |
        \*---------------------------------------------------------*/
        if(!controller->modes.empty())
        {
            std::cout << "  Modes:";

            int current_mode = controller->GetMode();
            for(std::size_t mode_idx = 0; mode_idx < controller->modes.size(); mode_idx++)
            {
                std::string modeStr = QuoteIfNecessary(controller->modes[mode_idx].name);

                if(current_mode == (int)mode_idx)
                {
                    modeStr = "[" + modeStr + "]";
                }
                std::cout << " " << modeStr;
            }
            std::cout << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device zones                                        |
        \*---------------------------------------------------------*/
        if(!controller->zones.empty())
        {
            std::cout << "  Zones:";

            for(std::size_t zone_idx = 0; zone_idx < controller->zones.size(); zone_idx++)
            {
                std::cout << " " << QuoteIfNecessary(controller->zones[zone_idx].name);
            }
            std::cout << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device LEDs                                         |
        \*---------------------------------------------------------*/
        if(!controller->leds.empty())
        {
            std::cout << "  LEDs:";

            for(std::size_t led_idx = 0; led_idx < controller->leds.size(); led_idx++)
            {
                std::cout << " " << QuoteIfNecessary(controller->leds[led_idx].name);
            }
            std::cout << std::endl;
        }

        std::cout << std::endl;
    }
}

bool OptionDevice(std::vector<DeviceOptions>* current_devices, std::string argument, Options* options, std::vector<RGBController *>& rgb_controllers)
{
    bool found = false;
    ResourceManager::get()->WaitForDeviceDetection();

    try
    {
        int current_device = std::stoi(argument);

        LOG_TRACE("[CLI] using device number %d for argument %s", current_device, argument.c_str());

        if((current_device >= static_cast<int>(rgb_controllers.size())) || (current_device < 0))
        {
            throw nullptr;
        }

        DeviceOptions newDev;
        newDev.device = current_device;

        if(!options->hasDevice)
        {
            options->hasDevice = true;
        }

        current_devices->push_back(newDev);

        found = true;
    }
    catch(...)
    {
        if(argument.length() > 1)
        {
            std::string argument_lower = argument;
            std::transform(argument_lower.begin(), argument_lower.end(), argument_lower.begin(), ::tolower);

            LOG_TRACE("[CLI] Searching for %s", argument_lower.c_str());

            for(unsigned int i = 0; i < rgb_controllers.size(); i++)
            {
                /*---------------------------------------------------------*\
                | If the argument is not a number then check all the        |
                |   controllers names for a match                           |
                \*---------------------------------------------------------*/
                std::string name            = rgb_controllers[i]->GetName();
                std::transform(name.begin(), name.end(), name.begin(), ::tolower);
                LOG_TRACE("[CLI] Comparing to %s", name.c_str());

                if(name.find(argument_lower) != std::string::npos)
                {
                    found                   = true;

                    DeviceOptions newDev;
                    newDev.device           = i;

                    if(!options->hasDevice)
                    {
                        options->hasDevice  = true;
                    }

                    current_devices->push_back(newDev);
                }
            }
        }
        else
        {
            std::cout << "Error: Empty device ID" << std::endl;
            return false;
        }
    }

    if(!found)
    {
        std::cout << "Error: Cannot find device \"" << argument << "\"" << std::endl;
    }

    return found;
}

bool OptionZone(std::vector<DeviceOptions>* current_devices, std::string argument, Options* /*options*/, std::vector<RGBController *>& rgb_controllers)
{
    bool found = false;
    ResourceManager::get()->WaitForDeviceDetection();

    try
    {
        int current_zone = std::stoi(argument);

        for(size_t i = 0; i < current_devices->size(); i++)
        {
            int current_device = current_devices->at(i).device;

            if(current_zone >= static_cast<int>(rgb_controllers[current_device]->zones.size()) || (current_zone < 0))
            {
                throw nullptr;
            }

            current_devices->at(i).zone = current_zone;
            found = true;
        }
    }
    catch(...)
    {
        std::cout << "Error: Invalid zone ID: " + argument << std::endl;
        return false;
    }

    return found;
}

bool CheckColor(std::string argument, DeviceOptions* currentDevOpts)
{
    if(ParseColors(argument, currentDevOpts))
    {
        currentDevOpts->hasOption = true;
        return true;
    }
    else
    {
        std::cout << "Error: Invalid color value: " + argument << std::endl;
        return false;
    }
}

bool OptionColor(std::vector<DeviceOptions>* current_devices, std::string argument, Options* options)
{
    /*---------------------------------------------------------*\
    | If a device is not selected  i.e. size() == 0             |
    |   then add color to allDeviceOptions                      |
    \*---------------------------------------------------------*/
    bool found                      = false;
    DeviceOptions* currentDevOpts   = &options->allDeviceOptions;

    if(current_devices->size() == 0)
    {
        found = CheckColor(argument, currentDevOpts);
    }
    else
    {
        for(size_t i = 0; i < current_devices->size(); i++)
        {
            currentDevOpts = &current_devices->at(i);

            found = CheckColor(argument, currentDevOpts);
        }
    }

    return found;
}

bool OptionMode(std::vector<DeviceOptions>* current_devices, std::string argument, Options* options)
{
    if(argument.size() == 0)
    {
        std::cout << "Error: --mode passed with no argument" << std::endl;
        return false;
    }

    /*---------------------------------------------------------*\
    | If a device is not selected  i.e. size() == 0             |
    |   then add mode to allDeviceOptions                       |
    \*---------------------------------------------------------*/
    bool found                      = false;
    DeviceOptions* currentDevOpts   = &options->allDeviceOptions;

    if(current_devices->size() == 0)
    {
        currentDevOpts->mode = argument;
        currentDevOpts->hasOption = true;
        found = true;
    }
    else
    {
        for(size_t i = 0; i < current_devices->size(); i++)
        {
            currentDevOpts = &current_devices->at(i);

            currentDevOpts->mode = argument;
            currentDevOpts->hasOption = true;
            found = true;
        }
    }

    if(!found)
    {
        std::cout << "Error: No devices for mode \"" << argument << "\"" << std::endl;
    }
    return found;
}

bool OptionSpeed(std::vector<DeviceOptions>* current_devices, std::string argument, Options* options)
{
    if(argument.size() == 0)
    {
        std::cout << "Error: --speed passed with no argument" << std::endl;
        return false;
    }

    /*---------------------------------------------------------*\
    | If a device is not selected  i.e. size() == 0             |
    |   then add speed to allDeviceOptions                 |
    \*---------------------------------------------------------*/
    bool found                      = false;
    DeviceOptions* currentDevOpts   = &options->allDeviceOptions;

    if(current_devices->size() == 0)
    {
        currentDevOpts->speed  = std::min(std::max(std::stoi(argument), 0),(int)speed_percentage);
        currentDevOpts->hasOption   = true;
        found = true;
    }
    else
    {
        for(size_t i = 0; i < current_devices->size(); i++)
        {
            DeviceOptions* currentDevOpts   = &current_devices->at(i);

            currentDevOpts->speed      = std::min(std::max(std::stoi(argument), 0),(int)speed_percentage);
            currentDevOpts->hasOption       = true;
            found = true;
        }
    }

    if(!found)
    {
        std::cout << "Error: No devices for speed \"" << argument << "\"" << std::endl;
    }
    return found;
}

bool OptionBrightness(std::vector<DeviceOptions>* current_devices, std::string argument, Options* options)
{
    if(argument.size() == 0)
    {
        std::cout << "Error: --brightness passed with no argument" << std::endl;
        return false;
    }

    /*---------------------------------------------------------*\
    | If a device is not selected  i.e. size() == 0             |
    |   then add brightness to allDeviceOptions                 |
    \*---------------------------------------------------------*/
    bool found                      = false;
    DeviceOptions* currentDevOpts   = &options->allDeviceOptions;

    if(current_devices->size() == 0)
    {
        currentDevOpts->brightness  = std::min(std::max(std::stoi(argument), 0),(int)brightness_percentage);
        currentDevOpts->hasOption   = true;
        found = true;
    }
    else
    {
        for(size_t i = 0; i < current_devices->size(); i++)
        {
            DeviceOptions* currentDevOpts   = &current_devices->at(i);

            currentDevOpts->brightness      = std::min(std::max(std::stoi(argument), 0),(int)brightness_percentage);
            currentDevOpts->hasOption       = true;
            found = true;
        }
    }

    if(!found)
    {
        std::cout << "Error: No devices for brightness \"" << argument << "\"" << std::endl;
    }
    return found;
}

bool OptionSize(std::vector<DeviceOptions>* current_devices, std::string argument, Options* /*options*/, std::vector<RGBController *>& rgb_controllers)
{
    const unsigned int new_size = std::stoi(argument);

    ResourceManager::get()->WaitForDeviceDetection();

    for(size_t i = 0; i < current_devices->size(); i++)
    {
        int current_device      = current_devices->at(i).device;
        int current_zone        = current_devices->at(i).zone;

        /*---------------------------------------------------------*\
        | Fail out if device, zone, or size are out of range        |
        \*---------------------------------------------------------*/
        if((current_device >= static_cast<int>(rgb_controllers.size())) || (current_device < 0))
        {
            std::cout << "Error: Device is out of range" << std::endl;
            return false;
        }
        else if((current_zone >= static_cast<int>(rgb_controllers[current_device]->zones.size())) || (current_zone < 0))
        {
            std::cout << "Error: Zone is out of range" << std::endl;
            return false;
        }
        else if((new_size < rgb_controllers[current_device]->zones[current_zone].leds_min) || (new_size > rgb_controllers[current_device]->zones[current_zone].leds_max))
        {
            std::cout << "Error: New size is out of range" << std::endl;
        }

        /*---------------------------------------------------------*\
        | Resize the zone                                           |
        \*---------------------------------------------------------*/
        rgb_controllers[current_device]->ResizeZone(current_zone, new_size);

        /*---------------------------------------------------------*\
        | Save the profile                                          |
        \*---------------------------------------------------------*/
        ResourceManager::get()->GetProfileManager()->SaveProfile("sizes", true);
    }

    return true;
}

bool OptionProfile(std::string argument, std::vector<RGBController *>& rgb_controllers)
{
    ResourceManager::get()->WaitForDeviceDetection();

    /*---------------------------------------------------------*\
    | Attempt to load profile                                   |
    \*---------------------------------------------------------*/
    if(ResourceManager::get()->GetProfileManager()->LoadProfile(argument))
    {
        /*-----------------------------------------------------*\
        | Change device mode if profile loading was successful  |
        \*-----------------------------------------------------*/
        for(std::size_t controller_idx = 0; controller_idx < rgb_controllers.size(); controller_idx++)
        {
            RGBController* device = rgb_controllers[controller_idx];

            device->DeviceUpdateMode();
            LOG_DEBUG("[CLI] Updating mode for %s to %i", device->GetName().c_str(), device->active_mode);

            if(device->modes[device->active_mode].color_mode == MODE_COLORS_PER_LED)
            {
                device->DeviceUpdateLEDs();
                LOG_DEBUG("[CLI] Mode uses per-LED color, also updating LEDs");
            }
        }

        std::cout << "Profile loaded successfully" << std::endl;
        return true;
    }
    else
    {
        std::cout << "Profile failed to load" << std::endl;
        return false;
    }
}

bool OptionSaveProfile(std::string argument)
{
    /*---------------------------------------------------------*\
    | Set save profile filename                                 |
    \*---------------------------------------------------------*/
    profile_save_filename = argument;
    return(true);
}

int ProcessOptions(Options* options, std::vector<RGBController *>& rgb_controllers)
{
    unsigned int ret_flags  = 0;
    int arg_index           = 1;
    std::vector<DeviceOptions> current_devices;

    options->hasDevice = false;
    options->profile_loaded = false;

#ifdef _WIN32
    int fake_argc;
    wchar_t** argvw = CommandLineToArgvW(GetCommandLineW(), &fake_argc);
#endif

    while(arg_index < preserve_argc)
    {
        std::string option   = preserve_argv[arg_index];
        std::string argument = "";
        filesystem::path arg_path;

        /*---------------------------------------------------------*\
        | Handle options that take an argument                      |
        \*---------------------------------------------------------*/
        if(arg_index + 1 < preserve_argc)
        {
            argument = preserve_argv[arg_index + 1];
#ifdef _WIN32
            arg_path = argvw[arg_index + 1];
#else
            arg_path = argument;
#endif
        }

        /*---------------------------------------------------------*\
        | -l / --list-devices (no arguments)                        |
        \*---------------------------------------------------------*/
        if(option == "--list-devices" || option == "-l")
        {
            OptionListDevices(rgb_controllers);
            exit(0);
        }

        /*---------------------------------------------------------*\
        | -d / --device                                             |
        \*---------------------------------------------------------*/
        else if(option == "--device" || option == "-d")
        {
            while(!current_devices.empty())
            {
                options->devices.push_back(current_devices.back());
                current_devices.pop_back();
            }

            if(!OptionDevice(&current_devices, argument, options, rgb_controllers))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -z / --zone                                               |
        \*---------------------------------------------------------*/
        else if(option == "--zone" || option == "-z")
        {
            if(!OptionZone(&current_devices, argument, options, rgb_controllers))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -c / --color                                              |
        \*---------------------------------------------------------*/
        else if(option == "--color" || option == "-c")
        {
            if(!OptionColor(&current_devices, argument, options))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -m / --mode                                               |
        \*---------------------------------------------------------*/
        else if(option == "--mode" || option == "-m")
        {
            if(!OptionMode(&current_devices, argument, options))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -b / --brightness                                         |
        \*---------------------------------------------------------*/
        else if(option == "--brightness" || option == "-b")
        {
            if(!OptionBrightness(&current_devices, argument, options))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -s / --speed                                         |
        \*---------------------------------------------------------*/
        else if(option == "--speed" || option == "-s")
        {
            if(!OptionSpeed(&current_devices, argument, options))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -sz / --size                                               |
        \*---------------------------------------------------------*/
        else if(option == "--size" || option == "-sz")
        {
            if(!OptionSize(&current_devices, argument, options, rgb_controllers))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -p / --profile                                            |
        \*---------------------------------------------------------*/
        else if(option == "--profile" || option == "-p")
        {
            options->profile_loaded = OptionProfile(arg_path.generic_u8string(), rgb_controllers);

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -sp / --save-profile                                      |
        \*---------------------------------------------------------*/
        else if(option == "--save-profile" || option == "-sp")
        {
            OptionSaveProfile(arg_path.generic_u8string());

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | Invalid option                                            |
        \*---------------------------------------------------------*/
        else
        {
            if((option == "--localconfig")
             ||(option == "--nodetect")
             ||(option == "--noautoconnect")
             ||(option == "--server")
             ||(option == "--gui")
             ||(option == "--i2c-tools" || option == "--yolo")
             ||(option == "--startminimized")
             ||(option == "--print-source")
             ||(option == "--verbose" || option == "-v")
             ||(option == "--very-verbose" || option == "-vv")
             ||(option == "--help" || option == "-h")
             ||(option == "--version" || option == "-V")
             ||(option == "--autostart-check")
             ||(option == "--autostart-disable"))
            {
                /*-------------------------------------------------*\
                | Do nothing, these are pre-detection arguments     |
                | and this parser should ignore them                |
                \*-------------------------------------------------*/
            }
            else if((option == "--server-port")
                  ||(option == "--server-host")
                  ||(option == "--loglevel")
                  ||(option == "--config")
                  ||(option == "--client")
                  ||(option == "--autostart-enable"))
            {
                /*-------------------------------------------------*\
                | Increment index for pre-detection arguments with  |
                | parameter                                         |
                \*-------------------------------------------------*/
                arg_index++;
            }
            else
            {
                /*-------------------------------------------------*\
                | If the argument is not a pre-detection argument,  |
                | throw an error and print help                     |
                \*-------------------------------------------------*/
                std::cout << "Error: Invalid option: " + option << std::endl;
                return RET_FLAG_PRINT_HELP;
            }
        }

        arg_index++;
    }

    /*---------------------------------------------------------*\
    | If a device was specified, check to verify that a         |
    | corresponding option was also specified                   |
    \*---------------------------------------------------------*/
    while(!current_devices.empty())
    {
        options->devices.push_back(current_devices.back());
        current_devices.pop_back();
    }

    if(options->hasDevice)
    {
        for(std::size_t option_idx = 0; option_idx < options->devices.size(); option_idx++)
        {
            if(!options->devices[option_idx].hasOption)
            {
                std::cout << "Error: Device " + std::to_string(option_idx) + " specified, but neither mode nor color given" << std::endl;
                return RET_FLAG_PRINT_HELP;
            }
        }
        return 0;
    }
    else
    {
        return ret_flags;
    }
}

void ApplyOptions(DeviceOptions& options, std::vector<RGBController *>& rgb_controllers)
{
    RGBController* device = rgb_controllers[options.device];

    /*---------------------------------------------------------*\
    | Set mode first, in case it's 'direct' (which affects      |
    | SetLED below)                                             |
    \*---------------------------------------------------------*/
    unsigned int mode = ParseMode(options, rgb_controllers);

    /*---------------------------------------------------------*\
    | If the user has specified random colours and the device   |
    |   supports that colour mode then swich to it before       |
    |   evaluating if a colour needs to be set                  |
    \*---------------------------------------------------------*/
    if(options.random_colors && (device->modes[mode].flags & MODE_FLAG_HAS_RANDOM_COLOR))
    {
        device->modes[mode].color_mode = MODE_COLORS_RANDOM;
    }

    /*---------------------------------------------------------*\
    | If the user has specified random colours and the device   |
    |   supports that colour mode then swich to it before       |
    |   evaluating if a colour needs to be set                  |
    \*---------------------------------------------------------*/
    if((device->modes[mode].flags & MODE_FLAG_HAS_BRIGHTNESS))
    {
        unsigned int new_brightness     = device->modes[mode].brightness_max - device->modes[mode].brightness_min;
        new_brightness                 *= options.brightness;
        new_brightness                 /= brightness_percentage;

        device->modes[mode].brightness  = device->modes[mode].brightness_min + new_brightness;
    }

    if((device->modes[mode].flags & MODE_FLAG_HAS_SPEED))
    {
        unsigned int new_speed     = device->modes[mode].speed_max - device->modes[mode].speed_min;
        new_speed                 *= options.speed;
        new_speed                 /= speed_percentage;

        device->modes[mode].speed  = device->modes[mode].speed_min + new_speed;
    }

    /*---------------------------------------------------------*\
    | Determine which color mode this mode uses and update      |
    | colors accordingly                                        |
    \*---------------------------------------------------------*/
    switch(device->modes[mode].color_mode)
    {
        case MODE_COLORS_NONE:
            break;

        case MODE_COLORS_RANDOM:
            break;

        case MODE_COLORS_PER_LED:
            if(options.colors.size() != 0)
            {
                std::size_t last_set_color = 0;

                RGBColor* start_from;
                unsigned int led_count;
                if(options.zone < 0)
                {
                    start_from  = &device->colors[0];
                    led_count   = (unsigned int)device->leds.size();
                }
                else
                {
                    start_from  = device->zones[options.zone].colors;
                    led_count   = device->zones[options.zone].leds_count;
                }

                for(std::size_t led_idx = 0; led_idx < led_count; led_idx++)
                {
                    if(led_idx < options.colors.size())
                    {
                        last_set_color      = led_idx;
                    }

                    start_from[led_idx]     = ToRGBColor(std::get<0>(options.colors[last_set_color]),
                                                         std::get<1>(options.colors[last_set_color]),
                                                         std::get<2>(options.colors[last_set_color]));
                }
            }
            break;

        case MODE_COLORS_MODE_SPECIFIC:
            if(options.colors.size() >= device->modes[mode].colors_min && options.colors.size() <= device->modes[mode].colors_max)
            {
                device->modes[mode].colors.resize(options.colors.size());

                for(std::size_t color_idx = 0; color_idx < options.colors.size(); color_idx++)
                {
                    device->modes[mode].colors[color_idx] = ToRGBColor(std::get<0>(options.colors[color_idx]),
                                                                       std::get<1>(options.colors[color_idx]),
                                                                       std::get<2>(options.colors[color_idx]));
                }
            }
            else
            {
                std::cout << "Wrong number of colors specified for mode " + device->modes[mode].name << std::endl;
                std::cout << "Please provide between " + std::to_string(device->modes[mode].colors_min) + " and " + std::to_string(device->modes[mode].colors_min) + " colors" << std::endl;
                exit(0);
            }
            break;
    }

    /*---------------------------------------------------------*\
    | Set device mode                                           |
    \*---------------------------------------------------------*/
    device->active_mode = mode;
    device->DeviceUpdateMode();

    /*---------------------------------------------------------*\
    | Set device per-LED colors if necessary                    |
    \*---------------------------------------------------------*/
    if(device->modes[mode].color_mode == MODE_COLORS_PER_LED)
    {
        device->DeviceUpdateLEDs();
    }
}


unsigned int cli_pre_detection(int argc, char* argv[])
{
    /*---------------------------------------------------------*\
    | Process only the arguments that should be performed prior |
    | to detecting devices and/or starting clients              |
    \*---------------------------------------------------------*/
    int             arg_index    = 1;
    unsigned int    cfg_args     = 0;
    unsigned int    ret_flags    = 0;
    std::string     server_host  = OPENRGB_SDK_HOST;
    unsigned short  server_port  = OPENRGB_SDK_PORT;
    bool            server_start = false;
    bool            print_help   = false;

    preserve_argc = argc;
    preserve_argv = argv;

#ifdef _WIN32
    int fake_argc;
    wchar_t** argvw = CommandLineToArgvW(GetCommandLineW(), &fake_argc);
#endif

    while(arg_index < argc)
    {
        std::string option   = argv[arg_index];
        std::string argument = "";

        LOG_DEBUG("[CLI] Parsing CLI option: %s", option.c_str());

        /*---------------------------------------------------------*\
        | Handle options that take an argument                      |
        \*---------------------------------------------------------*/
        if(arg_index + 1 < argc)
        {
            argument = argv[arg_index + 1];
        }

        /*---------------------------------------------------------*\
        | --localconfig                                             |
        \*---------------------------------------------------------*/
        if(option == "--localconfig")
        {
            ResourceManager::get()->SetConfigurationDirectory("./");
            cfg_args++;
        }

        /*---------------------------------------------------------*\
        | --config                                                  |
        \*---------------------------------------------------------*/
        else if(option == "--config")
        {
            cfg_args+= 2;
            arg_index++;
#ifdef _WIN32
            filesystem::path config_path(argvw[arg_index]);
#else
            filesystem::path config_path(argument);
#endif

            if(filesystem::is_directory(config_path))
            {
                ResourceManager::get()->SetConfigurationDirectory(config_path);
                LOG_INFO("[CLI] Setting config directory to %s",argument.c_str()); // TODO: Use config_path in logs somehow
            }
            else
            {
                LOG_ERROR("[CLI] '%s' is not a valid directory",argument.c_str()); // TODO: Use config_path in logs somehow
                print_help = true;
                break;
            }
        }

        /*---------------------------------------------------------*\
        | --nodetect                                                |
        \*---------------------------------------------------------*/
        else if(option == "--nodetect")
        {
            ret_flags |= RET_FLAG_NO_DETECT;
            cfg_args++;
        }

        /*---------------------------------------------------------*\
        | --noautoconnect                                           |
        \*---------------------------------------------------------*/
        else if(option == "--noautoconnect")
        {
            ret_flags |= RET_FLAG_NO_AUTO_CONNECT;
            cfg_args++;
        }

        /*---------------------------------------------------------*\
        | --client                                                  |
        \*---------------------------------------------------------*/
        else if(option == "--client")
        {
            NetworkClient * client = new NetworkClient(ResourceManager::get()->GetRGBControllers());

            std::size_t pos = argument.find(":");
            std::string ip = argument.substr(0, pos);
            unsigned short port_val;

            if(pos == argument.npos)
            {
                port_val = OPENRGB_SDK_PORT;
            }
            else
            {
                std::string port = argument.substr(argument.find(":") + 1);
                port_val = std::stoi(port);
            }

            std::string titleString = "OpenRGB ";
            titleString.append(VERSION_STRING);

            client->SetIP(ip.c_str());
            client->SetName(titleString.c_str());
            client->SetPort(port_val);

            client->StartClient();

            for(int timeout = 0; timeout < 100; timeout++)
            {
                if(client->GetConnected())
                {
                    break;
                }
                std::this_thread::sleep_for(10ms);
            }

            ResourceManager::get()->RegisterNetworkClient(client);

            cfg_args++;
            arg_index++;
        }

        /*---------------------------------------------------------*\
        | --server (no arguments)                                   |
        \*---------------------------------------------------------*/
        else if(option == "--server")
        {
            server_start = true;
        }

        /*---------------------------------------------------------*\
        | --server-port                                             |
        \*---------------------------------------------------------*/
        else if(option == "--server-port")
        {
            if (argument != "")
            {
                try
                {
                    int port = std::stoi(argument);
                    if (port >= 1024 && port <= 65535)
                    {
                        server_port  = port;
                        server_start = true;
                    }
                    else
                    {
                        std::cout << "Error: Port out of range: " << port << " (1024-65535)" << std::endl;
                        print_help = true;
                        break;
                    }
                }
                catch(std::invalid_argument& /*e*/)
                {
                    std::cout << "Error: Invalid data in --server-port argument (expected a number in range 1024-65535)" << std::endl;
                    print_help = true;
                    break;
                }
            }
            else
            {
                std::cout << "Error: Missing argument for --server-port" << std::endl;
                print_help = true;
                break;
            }
            cfg_args++;
            arg_index++;
        }
        /*---------------------------------------------------------*\
        | --server-host                                             |
        \*---------------------------------------------------------*/
        else if(option == "--server-host")
        {
            if (argument != "")
            {
                std::string host = argument;

                server_host  = host;
                server_start = true;
            }
            else
            {
                std::cout << "Error: Missing argument for --server-host" << std::endl;
                print_help = true;
                break;
            }
            cfg_args++;
            arg_index++;
        }

        /*---------------------------------------------------------*\
        | --loglevel                                                |
        \*---------------------------------------------------------*/
        else if(option == "--loglevel")
        {
            if (argument != "")
            {
                try
                {
                    int level = std::stoi(argument);
                    if (level >= 0 && level <= LL_TRACE)
                    {
                        LogManager::get()->setLoglevel(level);
                    }
                    else
                    {
                        LOG_ERROR("[CLI] Loglevel out of range: %d (0-6)", level);
                        print_help = true;
                        break;
                    }
                }
                catch(std::invalid_argument& /*e*/)
                {
                    if(!strcasecmp(argument.c_str(), "fatal"))
                    {
                        LogManager::get()->setLoglevel(LL_FATAL);
                    }
                    else if(!strcasecmp(argument.c_str(), "error"))
                    {
                        LogManager::get()->setLoglevel(LL_ERROR);
                    }
                    else if(!strcasecmp(argument.c_str(), "warning"))
                    {
                        LogManager::get()->setLoglevel(LL_WARNING);
                    }
                    else if(!strcasecmp(argument.c_str(), "info"))
                    {
                        LogManager::get()->setLoglevel(LL_INFO);
                    }
                    else if(!strcasecmp(argument.c_str(), "verbose"))
                    {
                        LogManager::get()->setLoglevel(LL_VERBOSE);
                    }
                    else if(!strcasecmp(argument.c_str(), "debug"))
                    {
                        LogManager::get()->setLoglevel(LL_DEBUG);
                    }
                    else if(!strcasecmp(argument.c_str(), "trace"))
                    {
                        LogManager::get()->setLoglevel(LL_TRACE);
                    }
                    else
                    {
                        LOG_ERROR("[CLI] Invalid loglevel");
                        print_help = true;
                        break;
                    }
                }
            }
            else
            {
                LOG_ERROR("[CLI] Missing argument for --loglevel");
                print_help = true;
                break;
            }
            cfg_args+= 2;
            arg_index++;
        }

        /*---------------------------------------------------------*\
        | --autostart-check (no arguments)                          |
        \*---------------------------------------------------------*/
        else if(option == "--autostart-check")
        {
            AutoStart auto_start("OpenRGB");

            if(auto_start.IsAutoStartEnabled())
            {
                std::cout << "Autostart is enabled." << std::endl;
            }
            else
            {
                std::cout << "Autostart is disabled." << std::endl;
            }
        }

        /*---------------------------------------------------------*\
        | --autostart-disable (no arguments)                        |
        \*---------------------------------------------------------*/
        else if(option == "--autostart-disable")
        {
            AutoStart auto_start("OpenRGB");

            if(auto_start.DisableAutoStart())
            {
                std::cout << "Autostart disabled." << std::endl;
            }
            else
            {
                std::cout << "Autostart failed to disable." << std::endl;
            }
        }

        /*---------------------------------------------------------*\
        | --autostart-enable                                        |
        \*---------------------------------------------------------*/
        else if(option == "--autostart-enable")
        {
            if (argument != "")
            {
                std::string desc = "OpenRGB ";
                desc += VERSION_STRING;
                desc += ", for controlling RGB lighting.";

                AutoStart       auto_start("OpenRGB");
                AutoStartInfo   auto_start_interface;

                auto_start_interface.args        = argument;
                auto_start_interface.category    = "Utility;";
                auto_start_interface.desc        = desc;
                auto_start_interface.icon        = "OpenRGB";
                auto_start_interface.path        = auto_start.GetExePath();

                if(auto_start.EnableAutoStart(auto_start_interface))
                {
                    std::cout << "Autostart enabled." << std::endl;
                }
                else
                {
                    std::cout << "Autostart failed to enable." << std::endl;
                }
            }
            else
            {
                std::cout << "Error: Missing argument for --autostart-enable" << std::endl;
                print_help = true;
                break;
            }

            cfg_args++;
            arg_index++;
        }

        /*---------------------------------------------------------*\
        | --gui (no arguments)                                      |
        \*---------------------------------------------------------*/
        else if(option == "--gui")
        {
            ret_flags |= RET_FLAG_START_GUI;
        }

        /*---------------------------------------------------------*\
        | --i2c-tools / --yolo (no arguments)                       |
        \*---------------------------------------------------------*/
        else if(option == "--i2c-tools" || option == "--yolo")
        {
            ret_flags |= RET_FLAG_START_GUI | RET_FLAG_I2C_TOOLS;
        }

        /*---------------------------------------------------------*\
        | --startminimized (no arguments)                           |
        \*---------------------------------------------------------*/
        else if(option == "--startminimized")
        {
            ret_flags |= RET_FLAG_START_GUI | RET_FLAG_START_MINIMIZED;
        }

        /*---------------------------------------------------------*\
        | -h / --help (no arguments)                                |
        \*---------------------------------------------------------*/
        else if(option == "--help" || option == "-h")
        {
            print_help = true;
            break;
        }

        /*---------------------------------------------------------*\
        | -V / --version (no arguments)                             |
        \*---------------------------------------------------------*/
        else if(option == "--version" || option == "-V")
        {
            OptionVersion();
            exit(0);
        }

        /*---------------------------------------------------------*\
        | -v / --verbose (no arguments)                             |
        \*---------------------------------------------------------*/
        else if(option == "--verbose" || option == "-v")
        {
            LogManager::get()->setVerbosity(LL_VERBOSE);
            cfg_args++;
        }

        /*---------------------------------------------------------*\
        | -vv / --very-verbose (no arguments)                       |
        \*---------------------------------------------------------*/
        else if(option == "--very-verbose" || option == "-vv")
        {
            LogManager::get()->setVerbosity(LL_TRACE);
            cfg_args++;
        }

        /*---------------------------------------------------------*\
        | --print-source (no arguments)                             |
        \*---------------------------------------------------------*/
        else if(option == "--print-source")
        {
            LogManager::get()->setPrintSource(true);
            cfg_args++;
        }

        /*---------------------------------------------------------*\
        | Any unrecognized arguments trigger the post-detection CLI |
        \*---------------------------------------------------------*/
        else
        {
            ret_flags |= RET_FLAG_CLI_POST_DETECTION;
        }

        arg_index++;
    }

    if(print_help)
    {
        OptionHelp();
        exit(0);
    }

    if(server_start)
    {
        NetworkServer * server = ResourceManager::get()->GetServer();
        server->SetHost(server_host);
        server->SetPort(server_port);
        ret_flags |= RET_FLAG_START_SERVER;
    }

    if((argc - cfg_args) <= 1)
    {
        ret_flags |= RET_FLAG_START_GUI;
    }

    return(ret_flags);
}

unsigned int cli_post_detection()
{
    /*---------------------------------------------------------*\
    | Wait for device detection                                 |
    \*---------------------------------------------------------*/
    ResourceManager::get()->WaitForDeviceDetection();

    /*---------------------------------------------------------*\
    | Get controller list from resource manager                 |
    \*---------------------------------------------------------*/
    std::vector<RGBController *> rgb_controllers = ResourceManager::get()->GetRGBControllers();

    /*---------------------------------------------------------*\
    | Process the argument options                              |
    \*---------------------------------------------------------*/
    Options options;
    unsigned int ret_flags = ProcessOptions(&options, rgb_controllers);

    /*---------------------------------------------------------*\
    | If the return flags are set, exit CLI mode without        |
    | processing device updates from CLI input.                 |
    \*---------------------------------------------------------*/
    switch(ret_flags)
    {
        case 0:
            break;

        case RET_FLAG_PRINT_HELP:
            std::cout << "Run `OpenRGB --help` for syntax" << std::endl;
            exit(-1);
            break;

        default:
            return ret_flags;
            break;
    }

    /*---------------------------------------------------------*\
    | If the options has one or more specific devices, loop     |
    | through all of the specific devices and apply settings.   |
    | Otherwise, apply settings to all devices.                 |
    \*---------------------------------------------------------*/
    if (options.hasDevice)
    {
        for(unsigned int device_idx = 0; device_idx < options.devices.size(); device_idx++)
        {
            ApplyOptions(options.devices[device_idx], rgb_controllers);
        }
    }
    else if (!options.profile_loaded)
    {
        for (unsigned int device_idx = 0; device_idx < rgb_controllers.size(); device_idx++)
        {
            options.allDeviceOptions.device = device_idx;
            ApplyOptions(options.allDeviceOptions, rgb_controllers);
        }
    }

    /*---------------------------------------------------------*\
    | If there is a save filename set, save the profile         |
    \*---------------------------------------------------------*/
    if (profile_save_filename != "")
    {
        if(ResourceManager::get()->GetProfileManager()->SaveProfile(profile_save_filename))
        {
            LOG_INFO("[CLI] Profile saved successfully");
        }
        else
        {
            LOG_ERROR("[CLI] Profile saving failed");
        }
    }

    std::this_thread::sleep_for(1s);

    return 0;
}

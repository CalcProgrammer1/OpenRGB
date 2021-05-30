#include <vector>
#include <cstring>
#include <string>
#include <tuple>
#include <iostream>
#include "OpenRGB.h"
#include "ProfileManager.h"
#include "ResourceManager.h"
#include "RGBController.h"
#include "i2c_smbus.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "LogManager.h"

/*-------------------------------------------------------------*\
| Quirk for MSVC; which doesn't support this case-insensitive   |
| function                                                      |
\*-------------------------------------------------------------*/
#ifdef _WIN32
    #define strcasecmp strcmpi
#endif

using namespace std::chrono_literals;

static std::string                 profile_save_filename = "";

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
    std::vector<std::tuple<unsigned char, unsigned char, unsigned char>> colors;
    std::string     mode;
    unsigned int    size;
    bool            hasSize;
    bool            hasOption;
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
    bool                        hasDevice;
    bool                        profile_loaded;
    DeviceOptions               allDeviceOptions;
    ServerOptions               servOpts;
};


/*---------------------------------------------------------------------------------------------------------*\
| Support a common subset of human colors; for easier typing: https://www.w3.org/TR/css-color-3/#svg-color  |
\*---------------------------------------------------------------------------------------------------------*/
struct HumanColors { uint32_t rgb; const char* keyword; } static const human_colors[] =
{
    { 0x000000, "black" },
    { 0x000080, "navy" },
    { 0x00008b, "darkblue" },
    { 0x0000cd, "mediumblue" },
    { 0x0000ff, "blue" },
    { 0x006400, "darkgreen" },
    { 0x008000, "green" },
    { 0x008080, "teal" },
    { 0x008b8b, "darkcyan" },
    { 0x00bfff, "deepskyblue" },
    { 0x00ced1, "darkturquoise" },
    { 0x00fa9a, "mediumspringgreen" },
    { 0x00ff00, "lime" },
    { 0x00ff7f, "springgreen" },
    { 0x00ffff, "aqua" },
    { 0x00ffff, "cyan" },
    { 0x191970, "midnightblue" },
    { 0x1e90ff, "dodgerblue" },
    { 0x20b2aa, "lightseagreen" },
    { 0x228b22, "forestgreen" },
    { 0x2e8b57, "seagreen" },
    { 0x2f4f4f, "darkslategray" },
    { 0x2f4f4f, "darkslategrey" },
    { 0x32cd32, "limegreen" },
    { 0x3cb371, "mediumseagreen" },
    { 0x40e0d0, "turquoise" },
    { 0x4169e1, "royalblue" },
    { 0x4682b4, "steelblue" },
    { 0x483d8b, "darkslateblue" },
    { 0x48d1cc, "mediumturquoise" },
    { 0x4b0082, "indigo" },
    { 0x556b2f, "darkolivegreen" },
    { 0x5f9ea0, "cadetblue" },
    { 0x6495ed, "cornflowerblue" },
    { 0x66cdaa, "mediumaquamarine" },
    { 0x696969, "dimgray" },
    { 0x696969, "dimgrey" },
    { 0x6a5acd, "slateblue" },
    { 0x6b8e23, "olivedrab" },
    { 0x708090, "slategray" },
    { 0x708090, "slategrey" },
    { 0x778899, "lightslategray" },
    { 0x778899, "lightslategrey" },
    { 0x7b68ee, "mediumslateblue" },
    { 0x7cfc00, "lawngreen" },
    { 0x7fff00, "chartreuse" },
    { 0x7fffd4, "aquamarine" },
    { 0x800000, "maroon" },
    { 0x800080, "purple" },
    { 0x808000, "olive" },
    { 0x808080, "gray" },
    { 0x808080, "grey" },
    { 0x87ceeb, "skyblue" },
    { 0x87cefa, "lightskyblue" },
    { 0x8a2be2, "blueviolet" },
    { 0x8b0000, "darkred" },
    { 0x8b008b, "darkmagenta" },
    { 0x8b4513, "saddlebrown" },
    { 0x8fbc8f, "darkseagreen" },
    { 0x90ee90, "lightgreen" },
    { 0x9370db, "mediumpurple" },
    { 0x9400d3, "darkviolet" },
    { 0x98fb98, "palegreen" },
    { 0x9932cc, "darkorchid" },
    { 0x9acd32, "yellowgreen" },
    { 0xa0522d, "sienna" },
    { 0xa52a2a, "brown" },
    { 0xa9a9a9, "darkgray" },
    { 0xa9a9a9, "darkgrey" },
    { 0xadd8e6, "lightblue" },
    { 0xadff2f, "greenyellow" },
    { 0xafeeee, "paleturquoise" },
    { 0xb0c4de, "lightsteelblue" },
    { 0xb0e0e6, "powderblue" },
    { 0xb22222, "firebrick" },
    { 0xb8860b, "darkgoldenrod" },
    { 0xba55d3, "mediumorchid" },
    { 0xbc8f8f, "rosybrown" },
    { 0xbdb76b, "darkkhaki" },
    { 0xc0c0c0, "silver" },
    { 0xc71585, "mediumvioletred" },
    { 0xcd5c5c, "indianred" },
    { 0xcd853f, "peru" },
    { 0xd2691e, "chocolate" },
    { 0xd2b48c, "tan" },
    { 0xd3d3d3, "lightgray" },
    { 0xd3d3d3, "lightgrey" },
    { 0xd8bfd8, "thistle" },
    { 0xda70d6, "orchid" },
    { 0xdaa520, "goldenrod" },
    { 0xdb7093, "palevioletred" },
    { 0xdc143c, "crimson" },
    { 0xdcdcdc, "gainsboro" },
    { 0xdda0dd, "plum" },
    { 0xdeb887, "burlywood" },
    { 0xe0ffff, "lightcyan" },
    { 0xe6e6fa, "lavender" },
    { 0xe9967a, "darksalmon" },
    { 0xee82ee, "violet" },
    { 0xeee8aa, "palegoldenrod" },
    { 0xf08080, "lightcoral" },
    { 0xf0e68c, "khaki" },
    { 0xf0f8ff, "aliceblue" },
    { 0xf0fff0, "honeydew" },
    { 0xf0ffff, "azure" },
    { 0xf4a460, "sandybrown" },
    { 0xf5deb3, "wheat" },
    { 0xf5f5dc, "beige" },
    { 0xf5f5f5, "whitesmoke" },
    { 0xf5fffa, "mintcream" },
    { 0xf8f8ff, "ghostwhite" },
    { 0xfa8072, "salmon" },
    { 0xfaebd7, "antiquewhite" },
    { 0xfaf0e6, "linen" },
    { 0xfafad2, "lightgoldenrodyellow" },
    { 0xfdf5e6, "oldlace" },
    { 0xff0000, "red" },
    { 0xff00ff, "fuchsia" },
    { 0xff00ff, "magenta" },
    { 0xff1493, "deeppink" },
    { 0xff4500, "orangered" },
    { 0xff6347, "tomato" },
    { 0xff69b4, "hotpink" },
    { 0xff7f50, "coral" },
    { 0xff8c00, "darkorange" },
    { 0xffa07a, "lightsalmon" },
    { 0xffa500, "orange" },
    { 0xffb6c1, "lightpink" },
    { 0xffc0cb, "pink" },
    { 0xffd700, "gold" },
    { 0xffdab9, "peachpuff" },
    { 0xffdead, "navajowhite" },
    { 0xffe4b5, "moccasin" },
    { 0xffe4c4, "bisque" },
    { 0xffe4e1, "mistyrose" },
    { 0xffebcd, "blanchedalmond" },
    { 0xffefd5, "papayawhip" },
    { 0xfff0f5, "lavenderblush" },
    { 0xfff5ee, "seashell" },
    { 0xfff8dc, "cornsilk" },
    { 0xfffacd, "lemonchiffon" },
    { 0xfffaf0, "floralwhite" },
    { 0xfffafa, "snow" },
    { 0xffff00, "yellow" },
    { 0xffffe0, "lightyellow" },
    { 0xfffff0, "ivory" },
    { 0xffffff, "white" },
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

        /* swy: (A) try interpreting it as text; as human keywords, otherwise strtoul() will pick up 'darkgreen' as 0xDA */
        for (const struct HumanColors *hc = human_colors; hc->keyword != NULL; hc++)
        {
            if (strcasecmp(hc->keyword, color.c_str()) != 0)
                continue;

            rgb = hc->rgb; parsed = true;

            break;
        }

        /* swy: (B) no luck, try interpreting it as an hexadecimal number instead */
        if (!parsed)
        {
            const char *colorptr = color.c_str(); char *endptr = NULL;

            rgb = strtoul(colorptr, &endptr, 16);

            /* swy: check that strtoul() has advanced the read pointer until the end (NULL terminator);
                    that means it has read the whole thing */
            if (colorptr != endptr && endptr && *endptr == '\0')
                parsed = true;
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

    std::cout << "Error: Mode '" + options.mode + "' not available for device '" + rgb_controllers[options.device]->name + "'" << std::endl;
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
    help_text += "--server-port                            Sets the SDK's server port. Default: 6742 (1024-65535)\n";
    help_text += "-l,  --list-devices                      Lists every compatible device with their number\n";
    help_text += "-d,  --device [0-9]                      Selects device to apply colors and/or effect to, or applies to all devices if omitted\n";
    help_text += "                                           Can be specified multiple times with different modes and colors\n";
    help_text += "-z,  --zone [0-9]                        Selects zone to apply colors and/or sizes to, or applies to all zones in device if omitted\n";
    help_text += "                                           Must be specified after specifying a device\n";
    help_text += "-c,  --color FFFFFF,00AAFF...            Sets colors on each device directly if no effect is specified, and sets the effect color if an effect is specified\n";
    help_text += "                                           If there are more LEDs than colors given, the last color will be applied to the remaining LEDs\n";
    help_text += "-m,  --mode [breathing | static | ...]   Sets the mode to be applied, check --list-devices to see which modes are supported on your device\n";
    help_text += "-s,  --size [0-N]                        Sets the new size of the specified device zone.\n";
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
    help_text += "--loglevel [0-6 | error | warning ...]   Set the log level (0: critical to 6: debug).\n";
    help_text += "--print-source                           Print the source code file and line number for each log entry.\n";
    help_text += "-v,  --verbose                           Print log messages to stdout.\n";
    help_text += "-vv, --very-verbose                      Print debug messages and log messages to stdout.\n";

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

void OptionListDevices(std::vector<RGBController *> &rgb_controllers)
{
    ResourceManager::get()->WaitForDeviceDetection();

    for(std::size_t controller_idx = 0; controller_idx < rgb_controllers.size(); controller_idx++)
    {
        RGBController *controller = rgb_controllers[controller_idx];

        /*---------------------------------------------------------*\
        | Print device name                                         |
        \*---------------------------------------------------------*/
        std::cout << controller_idx << ": " << controller->name << std::endl;

        /*---------------------------------------------------------*\
        | Print device type                                         |
        \*---------------------------------------------------------*/
            std::cout << "  Type:           " << device_type_to_str(controller->type) << std::endl;

        /*---------------------------------------------------------*\
        | Print device description                                  |
        \*---------------------------------------------------------*/
        if(!controller->description.empty())
        {
            std::cout << "  Description:    " << controller->description << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device version                                      |
        \*---------------------------------------------------------*/
        if(!controller->version.empty())
        {
            std::cout << "  Version:        " << controller->version << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device location                                     |
        \*---------------------------------------------------------*/
        if(!controller->location.empty())
        {
            std::cout << "  Location:       " << controller->location << std::endl;
        }

        /*---------------------------------------------------------*\
        | Print device serial                                       |
        \*---------------------------------------------------------*/
        if(!controller->serial.empty())
        {
            std::cout << "  Serial:         " << controller->serial << std::endl;
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

bool OptionDevice(int *current_device, std::string argument, Options *options, std::vector<RGBController *> &rgb_controllers)
{
    ResourceManager::get()->WaitForDeviceDetection();

    try
    {
        *current_device = std::stoi(argument);

        if((*current_device >= static_cast<int>(rgb_controllers.size())) || (*current_device < 0))
        {
            throw nullptr;
        }

        DeviceOptions newDev;
        newDev.device = *current_device;

        if(!options->hasDevice)
        {
            options->hasDevice = true;
        }

        options->devices.push_back(newDev);

        return true;
    }
    catch(...)
    {
        std::cout << "Error: Invalid device ID: " + argument << std::endl;
        return false;
    }
}

bool OptionZone(int *current_device, int *current_zone, std::string argument, Options */*options*/, std::vector<RGBController *> &rgb_controllers)
{
    ResourceManager::get()->WaitForDeviceDetection();

    try
    {
        *current_zone = std::stoi(argument);

        if(*current_device >= static_cast<int>(rgb_controllers.size()))
        {
            if(*current_zone >= static_cast<int>(rgb_controllers[*current_device]->zones.size()))
            {
                throw nullptr;
            }
        }

        return true;
    }
    catch(...)
    {
        std::cout << "Error: Invalid zone ID: " + argument << std::endl;
        return false;
    }
}

bool OptionColor(int *currentDev, int */*current_zone*/, std::string argument, Options *options)
{
    DeviceOptions* currentDevOpts = GetDeviceOptionsForDevID(options, *currentDev);

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

bool OptionMode(int *currentDev, std::string argument, Options *options)
{
    if (argument.size() == 0)
    {
        std::cout << "Error: --mode passed with no argument" << std::endl;
        return false;
    }

    DeviceOptions* currentDevOpts = GetDeviceOptionsForDevID(options, *currentDev);
    currentDevOpts->mode = argument;
    currentDevOpts->hasOption = true;
    return true;
}

bool OptionSize(int *current_device, int *current_zone, std::string argument, Options */*options*/, std::vector<RGBController *> &rgb_controllers)
{
    const unsigned int new_size = std::stoi(argument);

    ResourceManager::get()->WaitForDeviceDetection();

    /*---------------------------------------------------------*\
    | Fail out if device, zone, or size are out of range        |
    \*---------------------------------------------------------*/
    if((*current_device >= static_cast<int>(rgb_controllers.size())) || (*current_device < 0))
    {
        std::cout << "Error: Device is out of range" << std::endl;
        return false;
    }
    else if((*current_zone >= static_cast<int>(rgb_controllers[*current_device]->zones.size())) || (*current_zone < 0))
    {
        std::cout << "Error: Zone is out of range" << std::endl;
        return false;
    }
    else if((new_size < rgb_controllers[*current_device]->zones[*current_zone].leds_min) || (new_size > rgb_controllers[*current_device]->zones[*current_zone].leds_max))
    {
        std::cout << "Error: New size is out of range" << std::endl;
    }

    /*---------------------------------------------------------*\
    | Resize the zone                                           |
    \*---------------------------------------------------------*/
    rgb_controllers[*current_device]->ResizeZone(*current_zone, new_size);

    /*---------------------------------------------------------*\
    | Save the profile                                          |
    \*---------------------------------------------------------*/
    ResourceManager::get()->GetProfileManager()->SaveProfile("sizes", true);

    return true;
}

bool OptionProfile(std::string argument, std::vector<RGBController *> &rgb_controllers)
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
            LOG_DEBUG("Updating mode for %s to %i", device->name.c_str(), device->active_mode);

            if(device->modes[device->active_mode].color_mode == MODE_COLORS_PER_LED)
            {
                device->DeviceUpdateLEDs();
                LOG_DEBUG("Mode uses per-LED color, also updating LEDs");
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

int ProcessOptions(int argc, char *argv[], Options *options, std::vector<RGBController *> &rgb_controllers)
{
    unsigned int ret_flags  = 0;
    int arg_index           = 1;
    int current_device      = -1;
    int current_zone        = -1;

    options->hasDevice = false;
    options->profile_loaded = false;

    while(arg_index < argc)
    {
        std::string option   = argv[arg_index];
        std::string argument = "";

        /*---------------------------------------------------------*\
        | Handle options that take an argument                      |
        \*---------------------------------------------------------*/
        if(arg_index + 1 < argc)
        {
            argument = argv[arg_index + 1];
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
            if(!OptionDevice(&current_device, argument, options, rgb_controllers))
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
            if(!OptionZone(&current_device, &current_zone, argument, options, rgb_controllers))
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
            if(!OptionColor(&current_device, &current_zone, argument, options))
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
            if(!OptionMode(&current_device, argument, options))
            {
                return RET_FLAG_PRINT_HELP;
            }

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -s / --size                                               |
        \*---------------------------------------------------------*/
        else if(option == "--size" || option == "-s")
        {
            if(!OptionSize(&current_device, &current_zone, argument, options, rgb_controllers))
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
            options->profile_loaded = OptionProfile(argument, rgb_controllers);

            arg_index++;
        }

        /*---------------------------------------------------------*\
        | -sp / --save-profile                                      |
        \*---------------------------------------------------------*/
        else if(option == "--save-profile" || option == "-sp")
        {
            OptionSaveProfile(argument);

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
             ||(option == "--client")
             ||(option == "--server")
             ||(option == "--gui")
             ||(option == "--i2c-tools" || option == "--yolo")
             ||(option == "--startminimized")
             ||(option == "--print-source")
             ||(option == "--verbose" || option == "-v")
             ||(option == "--very-verbose" || option == "-vv")
             ||(option == "--help" || option == "-h")
             ||(option == "--version" || option == "-V"))
            {
                /*-------------------------------------------------*\
                | Do nothing, these are pre-detection arguments     |
                | and this parser should ignore them                |
                \*-------------------------------------------------*/
            }
            else if((option == "--server-port")
                  ||(option == "--loglevel")
                  ||(option == "--config"))
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

void ApplyOptions(DeviceOptions& options, std::vector<RGBController *> &rgb_controllers)
{
    RGBController *device = rgb_controllers[options.device];

    /*---------------------------------------------------------*\
    | Set mode first, in case it's 'direct' (which affects      |
    | SetLED below)                                             |
    \*---------------------------------------------------------*/
    unsigned int mode = ParseMode(options, rgb_controllers);

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
                std::size_t last_set_color;

                for(std::size_t led_idx = 0; led_idx < device->leds.size(); led_idx++)
                {
                    if(led_idx < options.colors.size())
                    {
                        last_set_color = led_idx;
                    }

                    device->colors[led_idx] = ToRGBColor(std::get<0>(options.colors[last_set_color]),
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
                std::cout << "Wrong number of colors specified for mode" << std::endl;
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


unsigned int cli_pre_detection(int argc, char *argv[])
{
    /*---------------------------------------------------------*\
    | Process only the arguments that should be performed prior |
    | to detecting devices and/or starting clients              |
    \*---------------------------------------------------------*/
    int             arg_index    = 1;
    unsigned int    cfg_args     = 0;
    unsigned int    ret_flags    = 0;
    unsigned short  server_port  = OPENRGB_SDK_PORT;
    bool            server_start = false;
    bool            print_help   = false;

    while(arg_index < argc)
    {
        std::string option   = argv[arg_index];
        std::string argument = "";

        LOG_DEBUG("Parsing CLI option: %s", option.c_str());

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
            ResourceManager::get()->SetConfigurationDirectory(argument);
            cfg_args++;
            arg_index++;
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

            ResourceManager::get()->GetClients().push_back(client);

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
                catch(std::invalid_argument& e)
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
        | --loglevel                                                |
        \*---------------------------------------------------------*/
        else if(option == "--loglevel")
        {
            if (argument != "")
            {
                try
                {
                    int level = std::stoi(argument);
                    if (level >= 0 && level <= LL_DEBUG)
                    {
                        LogManager::get()->setLoglevel(level);
                    }
                    else
                    {
                        std::cout << "Error: Loglevel out of range: " << level << " (0-6)" << std::endl;
                        print_help = true;
                        break;
                    }
                }
                catch(std::invalid_argument& e)
                {
                    if(!strcasecmp(argument.c_str(), "critical"))
                    {
                        LogManager::get()->setLoglevel(LL_CRITICAL);
                    }
                    else if(!strcasecmp(argument.c_str(), "error"))
                    {
                        LogManager::get()->setLoglevel(LL_ERROR);
                    }
                    else if(!strcasecmp(argument.c_str(), "message"))
                    {
                        LogManager::get()->setLoglevel(LL_MESSAGE);
                    }
                    else if(!strcasecmp(argument.c_str(), "warning"))
                    {
                        LogManager::get()->setLoglevel(LL_WARNING);
                    }
                    else if(!strcasecmp(argument.c_str(), "notice"))
                    {
                        LogManager::get()->setLoglevel(LL_NOTICE);
                    }
                    else if(!strcasecmp(argument.c_str(), "verbose"))
                    {
                        LogManager::get()->setLoglevel(LL_VERBOSE);
                    }
                    else if(!strcasecmp(argument.c_str(), "debug"))
                    {
                        LogManager::get()->setLoglevel(LL_DEBUG);
                    }
                    else
                    {
                        std::cout << "Error: invalid loglevel" << std::endl;
                        print_help = true;
                        break;
                    }
                }
            }
            else
            {
                std::cout << "Error: Missing argument for --loglevel" << std::endl;
                print_help = true;
                break;
            }
            cfg_args+= 2;
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
            LogManager::get()->setVerbosity(LL_DEBUG);
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
        ResourceManager::get()->GetServer()->SetPort(server_port);
        ret_flags |= RET_FLAG_START_SERVER;
    }

    if((argc - cfg_args) <= 1)
    {
        ret_flags |= RET_FLAG_START_GUI;
    }

    return(ret_flags);
}

unsigned int cli_post_detection(int argc, char *argv[])
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
    unsigned int ret_flags = ProcessOptions(argc, argv, &options, rgb_controllers);

    /*---------------------------------------------------------*\
    | If the return flags are set, exit CLI mode without        |
    | processing device updates from CLI input.                 |
    \*---------------------------------------------------------*/
    switch(ret_flags)
    {
        case 0:
            break;

        case RET_FLAG_PRINT_HELP:
            OptionHelp();
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
            std::cout << "Profile saved successfully" << std::endl;
        }
        else
        {
            std::cout << "Profile saving failed" << std::endl;
        }
    }

    std::this_thread::sleep_for(1s);

    return 0;
}

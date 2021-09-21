/*-------------------------------------------------------------------*\
|  LogitechProtocolCommon.cpp                                         |
|                                                                     |
|  Common Logitech RAP and FAP protocol calls                         |
|                                                                     |
|  Chris M (Dr_No)          4th May 2021                              |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <LogitechProtocolCommon.h>

static std::vector<uint16_t> logitech_RGB_pages =
{
    LOGITECH_HIDPP_PAGE_RGB_EFFECTS1,
    LOGITECH_HIDPP_PAGE_RGB_EFFECTS2
};

int getWirelessDevice(usages device_usages, uint16_t pid, wireless_map *wireless_devices)
{
    int result;
    hid_device* dev_use1;
    usages::iterator find_usage = device_usages.find(1);
    if (find_usage == device_usages.end())
    {
        LOG_INFO("Unable get_Wireless_Device due to missing FAP Short Message (0x10) usage");
        LOG_DEBUG("Dumping device usages:");
        for(usages::iterator dev = device_usages.begin(); dev != device_usages.end(); dev++)
        {
            LOG_DEBUG("Usage index:\t%i", dev->first);
        }
    }
    else
    {
        dev_use1 = find_usage->second;
        /*-----------------------------------------------------------------*\
        | Create a buffer for reads                                         |
        \*-----------------------------------------------------------------*/
        blankFAPmessage response;
        response.init();

        shortFAPrequest get_connected_devices;
        get_connected_devices.init(LOGITECH_RECEIVER_DEVICE_INDEX, LOGITECH_GET_REGISTER_REQUEST);

        result = hid_write(dev_use1, get_connected_devices.buffer, get_connected_devices.size());
        result = hid_read_timeout(dev_use1, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        bool wireless_notifications = response.data[1] & 1;  //Connected devices is a flag

        if (!wireless_notifications)
        {
            response.init(); //zero out the response
            get_connected_devices.init(LOGITECH_RECEIVER_DEVICE_INDEX, LOGITECH_SET_REGISTER_REQUEST);
            get_connected_devices.data[1] = 1;
            result = hid_write(dev_use1, get_connected_devices.buffer, get_connected_devices.size());
            result = hid_read_timeout(dev_use1, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);

            if(get_connected_devices.feature_index == 0x8F)
            {
                LOG_ERROR("Logitech Protocol error: %02X %02X %02X %02X %02X %02X %02X", get_connected_devices.report_id, get_connected_devices.device_index, get_connected_devices.feature_index, get_connected_devices.feature_command, get_connected_devices.data[0], get_connected_devices.data[1], get_connected_devices.data[2]);
            }
        }

        response.init(); //zero out the response
        get_connected_devices.init(LOGITECH_RECEIVER_DEVICE_INDEX, LOGITECH_GET_REGISTER_REQUEST);
        get_connected_devices.feature_command       = 0x02;    //0x02 Connection State register. Essentially asking for count of paired devices
        result = hid_write(dev_use1, get_connected_devices.buffer, get_connected_devices.size());
        result = hid_read_timeout(dev_use1, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);

        unsigned int device_count = response.data[1];
        LOG_INFO("Count of connected devices to %4X: %i", pid, device_count);

        if (device_count > 0)
        {
            LOG_INFO("Faking a reconnect to get device list");
            device_count++;     //Add 1 to the device_count to include the receiver

            response.init();
            get_connected_devices.init(LOGITECH_RECEIVER_DEVICE_INDEX, LOGITECH_SET_REGISTER_REQUEST);
            get_connected_devices.feature_index     = LOGITECH_SET_REGISTER_REQUEST;
            get_connected_devices.feature_command   = 0x02;    //0x02 Connection State register
            get_connected_devices.data[0]           = 0x02;    //Writting 0x02 to the connection state register will ask the receiver to fake a reconnect of paired devices
            result = hid_write(dev_use1, get_connected_devices.buffer, get_connected_devices.size());

            for(size_t i = 0; i < device_count; i++)
            {
                blankFAPmessage devices;
                devices.init();

                hid_read_timeout(dev_use1, devices.buffer, devices.size(), LOGITECH_PROTOCOL_TIMEOUT);
                unsigned int wireless_PID = (devices.data[2] << 8) | devices.data[1];
                LOG_INFO("Connected Device Index %i:\tVirtualID=%04X\t\t%02X %02X %02X %02X %02X %02X %02X", i, wireless_PID, devices.buffer[0], devices.buffer[1], devices.buffer[2], devices.buffer[3], devices.buffer[4], devices.buffer[5], devices.buffer[6]);

                /*-----------------------------------------------------------------*\
                | We need to read the receiver from the HID device queue but        |
                |    there is no need to add it as it's own device                  |
                \*-----------------------------------------------------------------*/
                if(devices.device_index != LOGITECH_RECEIVER_DEVICE_INDEX)
                {
                    wireless_devices->emplace(wireless_PID, devices.device_index);
                }
            }
        }
        else
        {
            LOG_WARNING("No devices were found connected to receiver!");
        }
    }

    return wireless_devices->size();
}

logitech_device::logitech_device(char *path, usages _usages, uint8_t _device_index, bool _wireless)
{
    device_index        = _device_index;
    location            = path;
    device_usages       = _usages;
    wireless            = _wireless;
    RGB_feature_index   = 0;
    mutex               = nullptr;

    initialiseDevice();
}

logitech_device::logitech_device(char *path, usages _usages, uint8_t _device_index, bool _wireless, std::shared_ptr<std::mutex> mutex_ptr)
{
    device_index        = _device_index;
    location            = path;
    device_usages       = _usages;
    wireless            = _wireless;
    RGB_feature_index   = 0;
    mutex               = mutex_ptr;

    initialiseDevice();
}

logitech_device::~logitech_device()
{
    for(usages::iterator dev = device_usages.begin(); dev != device_usages.end(); dev++)
    {
        hid_close(dev->second);
    }
}

void logitech_device::initialiseDevice()
{
    bool is_connected = connected();
    flushReadQueue();

    if(is_connected)
    {
        getDeviceName();

        /*-----------------------------------------------------------------*\
        | If this is running with DEBUG or higher loglevel then             |
        |   dump the entire Feature list to log                             |
        \*-----------------------------------------------------------------*/
        if(LogManager::get()->getLoglevel() > 4)
        {
            getDeviceFeatureList();     //This will populate the feature list
        }

        /*-----------------------------------------------------------------*\
        | Check device for known RGB Effects Feature pages & save the index |
        \*-----------------------------------------------------------------*/
        for(std::vector<uint16_t>::iterator page = logitech_RGB_pages.begin(); page != logitech_RGB_pages.end(); page++)
        {
            int feature_index = getFeatureIndex(*page);
            if(feature_index > 0)
            {
                feature_list.emplace(*page, feature_index);
                RGB_feature_index = feature_index;
            }
        }

        /*-----------------------------------------------------------------*\
        | If there was no RGB Effect Feature  page found                    |
        |   dump the entire Feature list to log                             |
        \*-----------------------------------------------------------------*/
        if (RGB_feature_index == 0)
        {
            LOG_INFO("[%s] Unable add this device due to missing RGB Effects Feature", device_name.c_str());
        }
        else
        {
            getRGBconfig();
        }
    }
}

bool logitech_device::is_valid()
{
    bool is_connected = connected();
    bool valid_test = false;

    if(is_connected)
    {
        LOG_DEBUG("[%s] valid_test - type %i   led_count - %i   RGB_index - %i", device_name.c_str(), logitech_device_type, leds.size(), RGB_feature_index);

        valid_test          = !device_name.empty()                                          // Check if device name exists
                            && (logitech_device_type >= 0 && logitech_device_type <= 8)     // Check if device type has a valid index
                            && (device_name[0] >= 32 && device_name[0] < 122)               // Check for non valid characters in device name
                            && device_name.length() > 3                                     // Check for valid device names lenght
                            && leds.size() > 0                                              // Check if a device has at least 1 led
                            && RGB_feature_index > 0;                                       // Check if a feature index is "valid"
    }
    else
    {
        LOG_INFO("Unable add this Logitech device: Not Connected");
    }

    return valid_test;
}

bool logitech_device::connected()
{
    /*-----------------------------------------------------------------*\
    | If this is a wireless device test that it's connected             |
    |   Wired devices will always be connected.                         |
    \*-----------------------------------------------------------------*/
    if(wireless)
    {
        bool test = false;
        hid_device* dev_use1 = getDevice(1);

        if(dev_use1)
        {
            shortFAPrequest get_connected_devices;
            get_connected_devices.init(device_index, LOGITECH_GET_REGISTER_REQUEST);
            get_connected_devices.feature_command = 0x02;    //0x02 Connection State register. Essentially asking for count of paired devices

            hid_write(dev_use1, get_connected_devices.buffer, get_connected_devices.size());
            //This hid_read will not timeout as we need to be sure the wireless device is connected
            hid_read(dev_use1, get_connected_devices.buffer, get_connected_devices.size());
            test = (get_connected_devices.data[1] != 0x09);  //ERR_RESOURCE_ERROR i.e. not currently connected
            LOG_DEBUG("Wireless device index %i is %s - %02X %02X %02X", get_connected_devices.device_index,
                      (test ? "connected" : "disconnected"), get_connected_devices.data[0], get_connected_devices.data[1],  get_connected_devices.data[2]);
        }
        return test;
    }
    else
    {
        return true;
    }
}

uint8_t logitech_device::getLEDinfo()
{
    /*-----------------------------------------------------------------*\
    | Get all info about the LEDs and Zones                             |
    \*-----------------------------------------------------------------*/

    return leds.size();
}

void logitech_device::flushReadQueue()
{
    /*-----------------------------------------------------------------*\
    | Create a buffer for reads                                         |
    \*-----------------------------------------------------------------*/
    blankFAPmessage response;
    response.init();

    for(usages::iterator dev = device_usages.begin(); dev != device_usages.end(); dev++)
    {
        //Flush the buffer
        int flushed     = 0;
        int result      = 1;

        while( result > 0 )
        {
            result = hid_read_timeout(dev->second, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
            if (result > 0)
            {
                flushed++;
            }
        }
        //device_name has not yet been set so can not use it in the log
        LOG_DEBUG("Preparing read queue for device %i - flushed %i packet%s", dev->first, flushed, ((flushed == 1) ? "" : "s"));
    }
}

hid_device* logitech_device::getDevice(uint8_t usage_index)
{
    /*-----------------------------------------------------------------*\
    | Check the usage map for usage_index                               |
    |   Return the associated device if found otherwise a nullptr       |
    \*-----------------------------------------------------------------*/
#ifdef WIN32
    usages::iterator find_usage = device_usages.find(usage_index);
#else
    //Linux does not need bundle the device usages hence .begin()
    usages::iterator find_usage = device_usages.begin();
#endif //WIN32

    if (find_usage == device_usages.end())
    {
        LOG_INFO("Unable add this device due to missing FAP Message usage %i", usage_index);
        return nullptr;
    }
    else
    {
        return find_usage->second;
    }    
}

uint8_t logitech_device::getFeatureIndex(uint16_t feature_page)
{
    /*-----------------------------------------------------------------*\
    | Get the feature index from the Root Index                         |
    |   Return the mapped feature_index of the given feature page       |
    |   for this device or else return 0                                |
    \*-----------------------------------------------------------------*/
    int         result          = 0;
    uint8_t     feature_index   = 0;
    hid_device* dev_use2        = getDevice(2);

    if(dev_use2)
    {
        blankFAPmessage response;
        longFAPrequest get_index;
        get_index.init(device_index, LOGITECH_HIDPP_PAGE_ROOT_IDX, LOGITECH_CMD_ROOT_GET_FEATURE);
        get_index.data[0]       = feature_page >> 8;
        get_index.data[1]       = feature_page & 0xFF;

        result                  = hid_write(dev_use2, get_index.buffer, get_index.size());
        result                  = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        feature_index           = response.data[0];
        LOG_DEBUG("[%s] Feature Page %04X found @ index %02X - %02X %02X %02X %02X %02X %02X %02X %02X", device_name.c_str(), feature_page, feature_index,
                response.data[0], response.data[1],  response.data[2],  response.data[3], response.data[4], response.data[5],  response.data[6],  response.data[7]);
    }

    return feature_index;
}

int logitech_device::getDeviceFeatureList()
{
    int result = 0;

    /*-----------------------------------------------------------------*\
    | Check the usage map for usage2 (0x11 Long FAP Message)            |
    |   then list all features for device                               |
    \*-----------------------------------------------------------------*/
    hid_device* dev_use2 = getDevice(2);

    if(dev_use2)
    {
        /*-----------------------------------------------------------------*\
        | Create a buffer for reads                                         |
        \*-----------------------------------------------------------------*/
        blankFAPmessage response;
        response.init();

        /*-----------------------------------------------------------------*\
        | Query the root index for the index of the feature list            |
        |   This is done for safety as it is generaly at feature index 0x01 |
        \*-----------------------------------------------------------------*/
        int feature_index = getFeatureIndex(LOGITECH_HIDPP_PAGE_FEATURE_SET);

        /*-----------------------------------------------------------------*\
        | Get the count of Features                                         |
        \*-----------------------------------------------------------------*/
        longFAPrequest get_count;
        get_count.init(device_index, feature_index, LOGITECH_CMD_FEATURE_SET_GET_COUNT);

        result = hid_write(dev_use2, get_count.buffer, get_count.size());
        result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        unsigned int feature_count = response.data[0];

        longFAPrequest get_features;
        get_features.init(device_index, feature_index, LOGITECH_CMD_FEATURE_SET_GET_ID);
        for(std::size_t i = 1; feature_list.size() < feature_count; i++ )
        {
            get_features.data[0] = i;
            result = hid_write(dev_use2, get_features.buffer, get_features.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
            LOG_DEBUG("[%s] Feature %04X @ index: %02X", device_name.c_str(), (response.data[0] << 8) | response.data[1], i);
            feature_list.emplace((response.data[0] << 8) | response.data[1], i);
        }
    }
    else
    {
        LOG_INFO("[%s] Unable get the feature index list - missing FAP Long Message (0x11) usage", device_name.c_str());
    }

    return feature_list.size();
}

int logitech_device::getDeviceName()
{
    /*-----------------------------------------------------------------*\
    | Check the usage map for usage2 (0x11 Long FAP Message)            |
    |   Then use it to get the name for this device                     |
    \*-----------------------------------------------------------------*/
    hid_device* dev_use2 = getDevice(2);

    if(dev_use2)
    {
        /*-----------------------------------------------------------------*\
        | Create a buffer for reads                                         |
        \*-----------------------------------------------------------------*/
        blankFAPmessage response;
        response.init();
        int result;

        /*-----------------------------------------------------------------*\
        | Query the root index for the index of the name feature            |
        \*-----------------------------------------------------------------*/
        int feature_index = getFeatureIndex(LOGITECH_HIDPP_PAGE_DEVICE_NAME_TYPE);

        /*-----------------------------------------------------------------*\
        | Get the device name length                                        |
        \*-----------------------------------------------------------------*/
        if(feature_index > 0)
        {
            longFAPrequest get_length;
            get_length.init(device_index, feature_index, LOTITECH_CMD_DEVICE_NAME_TYPE_GET_COUNT);
            result = hid_write(dev_use2, get_length.buffer, get_length.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
            unsigned int name_length = response.data[0];
            LOG_DEBUG("[%s] Name Length %02i - %02X %02X %02X %02X %02X %02X %02X %02X   %02X %02X %02X %02X %02X %02X %02X %02X", device_name.c_str(), name_length,
                response.data[0], response.data[1],  response.data[2],  response.data[3],  response.data[4],  response.data[5],  response.data[6],  response.data[7],
                response.data[8], response.data[9], response.data[10], response.data[11], response.data[12], response.data[13], response.data[14], response.data[15]);

            longFAPrequest get_name;
            get_name.init(device_index, feature_index, LOGITECH_CMD_DEVICE_NAME_TYPE_GET_DEVICE_NAME);
            while(device_name.length() < name_length)
            {
                get_name.data[0] = device_name.length();   //This sets the character index to get from the device
                result = hid_write(dev_use2, get_name.buffer, get_name.size());
                result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
                std::string temp = (char *)&response.data;
                device_name.append(temp);
                LOG_DEBUG("[%s] Get Name %02i - %02X %02X %02X %02X %02X %02X %02X %02X   %02X %02X %02X %02X %02X %02X %02X %02X", device_name.c_str(), device_name.length(),
                    response.data[0], response.data[1],  response.data[2],  response.data[3],  response.data[4],  response.data[5],  response.data[6],  response.data[7],
                    response.data[8], response.data[9], response.data[10], response.data[11], response.data[12], response.data[13], response.data[14], response.data[15]);
            }

            get_name.init(device_index, feature_index, LOGITECH_CMD_DEVICE_NAME_TYPE_GET_TYPE);
            result = hid_write(dev_use2, get_name.buffer, get_name.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
            logitech_device_type = response.data[0];
            LOG_DEBUG("[%s] Get Type %02i - %02X %02X %02X %02X %02X %02X %02X %02X   %02X %02X %02X %02X %02X %02X %02X %02X", device_name.c_str(), logitech_device_type,
                response.data[0], response.data[1],  response.data[2],  response.data[3],  response.data[4],  response.data[5],  response.data[6],  response.data[7],
                response.data[8], response.data[9], response.data[10], response.data[11], response.data[12], response.data[13], response.data[14], response.data[15]);
        }
    }

    return device_name.length();
}

void logitech_device::getRGBconfig()
{
    /*-----------------------------------------------------------------*\
    | Check the usage map for usage2 (0x11 Long FAP Message)            |
    |   Then use it to get the name for this device                     |
    \*-----------------------------------------------------------------*/
    hid_device* dev_use2    = getDevice(2);
    uint8_t led_response    = 0;
    uint8_t led_counter     = 0;

    if(dev_use2)
    {
        /*-----------------------------------------------------------------*\
        | Create a buffer for reads                                         |
        \*-----------------------------------------------------------------*/
        blankFAPmessage response;
        response.init();
        int result;

        longFAPrequest get_count;
        get_count.init(device_index, RGB_feature_index, LOGITECH_CMD_RGB_EFFECTS_GET_COUNT);

        result = hid_write(dev_use2, get_count.buffer, get_count.size());
        do
        {
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
            LOG_DEBUG("[%s] LED Count - %02X :   %04X %04X %04X %04X   %02X %02X %02X %02X %02X %02X %02X", device_name.c_str(),
                response.data[0], (response.data[1] << 8 | response.data[2]), (response.data[3] << 8 | response.data[4]), (response.data[5] << 8 | response.data[6]),
                (response.data[7] << 8 | response.data[8]), response.data[9], response.data[10], response.data[11], response.data[12], response.data[13], response.data[14], response.data[15]);
        } while ((result == 20) && (get_count.feature_index != response.feature_index) && (get_count.feature_command != response.feature_command));

        led_response = response.data[0];

        get_count.feature_command = LOGITECH_CMD_RGB_EFFECTS_GET_INFO;
        do
        {
            //TODO: Push this info into a vector for later enumeration by the RGBController
            get_count.data[0] = leds.size();
            result = hid_write(dev_use2, get_count.buffer, get_count.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
            LOG_DEBUG("[%s] LED %02i - %02X %02X %02X %02X %02X %02X %02X %02X   %02X %02X %02X %02X %02X %02X %02X %02X", device_name.c_str(), led_counter,
                response.data[0], response.data[1],  response.data[2],  response.data[3],  response.data[4],  response.data[5],  response.data[6],  response.data[7],
                response.data[8], response.data[9], response.data[10], response.data[11], response.data[12], response.data[13], response.data[14], response.data[15]);
            if(result == 20 && get_count.feature_index == response.feature_index && get_count.feature_command == response.feature_command && response.data[0] != 0x10 && response.data[1] != 0x02)
            {
                //If the response is the correct length (i.e. no USB error) and is for the RGB_feature_index and LOGITECH_CMD_RGB_EFFECTS_GET_INFO and no error occured with the led_counter then bump the counter
                logitech_led new_led;

                new_led.value   = response.data[0];
                new_led.param1  = response.data[1];
                new_led.param2  = response.data[2];
                new_led.param3  = response.data[3];

                leds.push_back(new_led);
            }
        } while ((result == 20) && (get_count.feature_index == response.feature_index) && (get_count.feature_command == response.feature_command) && (response.data[0] != 0x10) && (response.data[1] != 0x02));

        /*get_count.feature_command = LOGITECH_CMD_RGB_EFFECTS_GET_STATE;
        for(std::size_t i = 0; i < feature_count; i++ )
        {
            get_count.data[0] = i;
            result = hid_write(dev_use2, get_count.buffer, get_count.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        }

        get_count.feature_command = LOGITECH_CMD_RGB_EFFECTS_GET_CONFIG;
        for(std::size_t i = 0; i < feature_count; i++ )
        {
            get_count.data[0] = i;
            result = hid_write(dev_use2, get_count.buffer, get_count.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        }*/
    }

    LOG_DEBUG("[%s] led_response returned %i led_counter returned %i : setting controller to %i LED%s", device_name.c_str(), led_response, led_counter, leds.size(), ((leds.size() == 1) ? "" : "s"));
}

uint8_t logitech_device::setDirectMode(bool direct)
{
    int result = 0;

    /*-----------------------------------------------------------------*\
    | Check the usage map for usage2 (0x11 Long FAP Message)            |
    |   then set the device into direct mode via register 0x80          |
    \*-----------------------------------------------------------------*/
    hid_device* dev_use2 = getDevice(2);

    if(dev_use2)
    {
        /*-----------------------------------------------------------------*\
        | Create a buffer for reads                                         |
        \*-----------------------------------------------------------------*/
        blankFAPmessage response;
        response.init();

        /*-----------------------------------------------------------------*\
        | Turn the direct mode on or off via the RGB_feature_index          |
        \*-----------------------------------------------------------------*/
        longFAPrequest set_direct;
        set_direct.init(device_index, RGB_feature_index, LOGITECH_CMD_RGB_EFFECTS_UNKNOWN);
        set_direct.data[0] = (direct) ? 1 : 0;
        set_direct.data[1] = set_direct.data[0];

        /*-----------------------------------------------------*\
        | Send packet                                           |
        | This code has to be protected to avoid crashes when   |
        | this is called at the same time to change a powerplay |
        | mat and its paired wireless mouse leds. It will       |
        | happen when using effects engines with high framerate |
        \*-----------------------------------------------------*/
        if(mutex)
        {
            std::lock_guard<std::mutex> guard(*mutex);

            result = hid_write(dev_use2, set_direct.buffer, set_direct.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        }
        else
        {
            result = hid_write(dev_use2, set_direct.buffer, set_direct.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        }
    }
    return result;
}

uint8_t logitech_device::setMode(uint8_t mode, uint16_t speed, uint8_t zone, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness, bool bright_cycle_swap)
{
    int result = 0;

    /*-----------------------------------------------------------------*\
    | Check the usage map for usage2 (0x11 Long FAP Message) then       |
    |   set the device mode via LOGITECH_CMD_RGB_EFFECTS_SET_CONTROL    |
    \*-----------------------------------------------------------------*/
    hid_device* dev_use2 = getDevice(2);

    if(dev_use2)
    {
        /*-----------------------------------------------------------------*\
        | Create a buffer for reads                                         |
        \*-----------------------------------------------------------------*/
        blankFAPmessage response;
        response.init();

        /*-----------------------------------------------------------------*\
        | Set the mode via the RGB_feature_index                            |
        \*-----------------------------------------------------------------*/
        longFAPrequest set_mode;
        set_mode.init(device_index, RGB_feature_index, LOGITECH_CMD_RGB_EFFECTS_SET_CONTROL);
        set_mode.data[0] = zone;
        set_mode.data[1] = mode;

        set_mode.data[2] = red;
        set_mode.data[3] = green;
        set_mode.data[4] = blue;

        speed *= 100;
        if(mode == 1)   //Static
        {
            set_mode.data[5]    = zone;
        }
        else if(mode == 2)  //Spectrum Cycle
        {
            if(bright_cycle_swap)
            {
                set_mode.data[1] = 3;
            }
            set_mode.data[7]    = speed >> 8;
            set_mode.data[8]    = speed & 0xFF;
            set_mode.data[9]    = brightness;
        }
        else if(mode == 3)  //Breathing
        {
            if(bright_cycle_swap)
            {
                set_mode.data[1] = 2;
            }
            set_mode.data[5]    = speed >> 8;
            set_mode.data[6]    = speed & 0xFF;
            set_mode.data[8]    = brightness;
        }

        /*-----------------------------------------------------*\
        | Send packet                                           |
        | This code has to be protected to avoid crashes when   |
        | this is called at the same time to change a powerplay |
        | mat and its paired wireless mouse leds. It will       |
        | happen when using effects engines with high framerate |
        \*-----------------------------------------------------*/
        if(mutex)
        {
            std::lock_guard<std::mutex> guard(*mutex);

            result = hid_write(dev_use2, set_mode.buffer, set_mode.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        }
        else
        {
            result = hid_write(dev_use2, set_mode.buffer, set_mode.size());
            result = hid_read_timeout(dev_use2, response.buffer, response.size(), LOGITECH_PROTOCOL_TIMEOUT);
        }
    }
    return result;
}

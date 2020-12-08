#include "LogManager.h"

#include <stdarg.h>
#include "ResourceManager.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;

LogManager::LogManager()
{
}

LogManager* LogManager::get()
{
    static LogManager* _instance = nullptr;
    static std::mutex instance_mutex;
    std::lock_guard<std::mutex> grd(instance_mutex);

    /*-------------------------------------------------*\
    | Create a new instance if one does not exist       |
    \*-------------------------------------------------*/
    if(!_instance)
    {
        _instance = new LogManager();
    }
    
    return _instance;
}

void LogManager::configure(json config, const std::string &defaultDir)
{
    std::lock_guard<std::mutex> grd(entry_mutex);

    /*-------------------------------------------------*\
    | If the log is not open, create a new log file     |
    \*-------------------------------------------------*/
    if(!log_stream.is_open())
    {
        std::string logname = "OpenRGB_#.log";

        /*-------------------------------------------------*\
        | If the logfile is defined in the configuration,   |
        | use the configured name                           |
        \*-------------------------------------------------*/
        if(config.contains("logfile"))
        {
            const json& logfile_obj = config["logfile"];
            if(logfile_obj.is_string())
            {
                std::string tmpname = config["logfile"];
                if(!tmpname.empty())
                {
                    logname = tmpname;
                }
            }
        }

        /*-------------------------------------------------*\
        | If the # symbol is found in the log file name,    |
        | replace it with a timestamp                       |
        \*-------------------------------------------------*/
        size_t oct = logname.find("#");
        if(oct != logname.npos)
        {
            time_t t = time(0);
            struct tm* tmp = localtime(&t);
            char buf[64];
            snprintf(buf, 64, "%04d%02d%02d_%02d%02d%02d", 1900 + tmp->tm_year, tmp->tm_mon, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);
            logname.replace(oct, 1, buf);
        }

        /*-------------------------------------------------*\
        | If the path is relative, use configuration dir    |
        \*-------------------------------------------------*/
        fs::path p = logname;
        if(p.is_relative())
        {
            p = defaultDir;
            p.append(logname);
        }

        /*-------------------------------------------------*\
        | Open the logfile                                  |
        \*-------------------------------------------------*/
        log_stream.open(p);
    }

    /*-------------------------------------------------*\
    | Check loglevel configuration                      |
    \*-------------------------------------------------*/
    if(config.contains("loglevel"))
    {
        const json& loglevel_obj = config["loglevel"];

        /*-------------------------------------------------*\
        | If the log level is configured per section, set   |
        | loglevel for each section                         |
        \*-------------------------------------------------*/
        if(loglevel_obj.is_object())
        {
            for(size_t section = 0; section < sections.size(); ++section)
            {
                if(loglevel_obj.contains(sections[section]))
                {
                    const json& val = loglevel_obj[sections[section]];

                    if(val.is_number_integer())
                    {
                        loglevels[section] = val;
                    }
                }
            }
        }

        /*-------------------------------------------------*\
        | If the log level is configured globally, set same |
        | loglevel for each section                         |
        \*-------------------------------------------------*/
        else if(loglevel_obj.is_number_integer())
        {
            int l = loglevel_obj;
            for(size_t section = 0; section < sections.size(); ++section)
            {
                loglevels[section] = l;
            }
        }
    }

    /*-------------------------------------------------*\
    | Flush the log                                     |
    \*-------------------------------------------------*/
    _flush();
}

void LogManager::_flush()
{
    /*-------------------------------------------------*\
    | If the log is open, write out buffered messages   |
    \*-------------------------------------------------*/
    if(log_stream.is_open())
    {
        for(size_t msg = 0; msg < temp_messages.size(); ++msg)
        {
            int sec = temp_sections[msg];
            if(temp_levels[msg] <= loglevels[sec])
            {
                log_stream << temp_messages[msg] << std::endl;
            }
        }

        /*-------------------------------------------------*\
        | Clear temp message buffers after writing them out |
        \*-------------------------------------------------*/
        temp_messages.clear();
        temp_levels.clear();
        temp_sections.clear();
    }

    /*-------------------------------------------------*\
    | Flush the stream                                  |
    \*-------------------------------------------------*/
    log_stream.flush();
}

void LogManager::flush()
{
    std::lock_guard<std::mutex> grd(entry_mutex);
    _flush();
}

void LogManager::append(int section, int level, const char* fmt, ...)
{
    std::lock_guard<std::mutex> grd(entry_mutex);

    char buf[1024];

    /*-------------------------------------------------*\
    | Start the variable argument list                  |
    \*-------------------------------------------------*/
    va_list va;
    va_start(va, fmt);

    /*-------------------------------------------------*\
    | Return if the log is already open                 |
    \*-------------------------------------------------*/
    if(!log_stream.is_open() && level > loglevels[section])
    {
        return;
    }

    /*-------------------------------------------------*\
    | Print the section to the log entry                |
    \*-------------------------------------------------*/
    int off = sprintf(buf, "[%s]: ", sections[section].c_str());

    /*-------------------------------------------------*\
    | Print the log text to the log entry               |
    \*-------------------------------------------------*/
    vsnprintf(buf + off, 1024 - off, fmt, va);

    /*-------------------------------------------------*\
    | Write the log entry                               |
    \*-------------------------------------------------*/
    if(log_stream.is_open())
    {
        log_stream << buf << std::endl;
    }
    else
    {
        temp_levels.push_back(level);
        temp_messages.push_back(buf);
        temp_sections.push_back(section);
    }

    /*-------------------------------------------------*\
    | End the variable argument list                    |
    \*-------------------------------------------------*/
    va_end(va);
}

int LogManager::registerSection(const char* name, int loglevel)
{
    std::lock_guard<std::mutex> grd(section_mutex);
    size_t section;

    /*-------------------------------------------------*\
    | Check to see if section already exists, if so,    |
    | return the existing section value                 |
    \*-------------------------------------------------*/
    for(section = 0; section < sections.size(); section++)
    {
        if(sections[section] == name)
        {
            return section;
        }
    }

    /*-------------------------------------------------*\
    | If section does not already exist, create it      |
    \*-------------------------------------------------*/
    sections.push_back(name);
    loglevels.push_back(loglevel);

    return section;
}

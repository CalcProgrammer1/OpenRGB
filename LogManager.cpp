/*---------------------------------------------------------*\
| LogManager.cpp                                            |
|                                                           |
|   Manages log file and output to the console              |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include "LogManager.h"

#include <stdarg.h>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "ResourceManager.h"

#include "filesystem.h"

const char* LogManager::log_codes[] = {"FATAL:", "ERROR:", "Warning:", "Info:", "Verbose:", "Debug:", "Trace:", "Dialog:"};

LogManager::LogManager()
{
    base_clock = std::chrono::steady_clock::now();
    log_console_enabled = false;
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

unsigned int LogManager::getLoglevel()
{
    if(log_console_enabled)
    {
        return(LL_TRACE);
    }
    else
    {
        return(loglevel);
    }
}

void LogManager::configure(json config, const filesystem::path& defaultDir)
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
        time_t t = time(0);
        struct tm* tmp = localtime(&t);
        char time_string[64];
        snprintf(time_string, 64, "%04d%02d%02d_%02d%02d%02d", 1900 + tmp->tm_year, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min, tmp->tm_sec);

        size_t oct = logname.find("#");
        if(oct != logname.npos)
        {
            logname.replace(oct, 1, time_string);
        }

        /*-------------------------------------------------*\
        | If the path is relative, use logs dir             |
        \*-------------------------------------------------*/
        filesystem::path p = filesystem::u8path(logname);
        if(p.is_relative())
        {
            p = defaultDir / "logs" / logname;
        }
        filesystem::create_directories(p.parent_path());

        /*-------------------------------------------------*\
        | Open the logfile                                  |
        \*-------------------------------------------------*/
        log_stream.open(p);

        /*-------------------------------------------------*\
        | Print Git Commit info, version, etc.              |
        \*-------------------------------------------------*/
        log_stream << "    OpenRGB v" << VERSION_STRING << std::endl;
        log_stream << "    Commit: " << GIT_COMMIT_ID << " from " << GIT_COMMIT_DATE << std::endl;
        log_stream << "    Launched: " << time_string << std::endl;
        log_stream << "====================================================================================================" << std::endl;
        log_stream << std::endl;
    }

    /*-------------------------------------------------*\
    | Check loglevel configuration                      |
    \*-------------------------------------------------*/
    if(config.contains("loglevel"))
    {
        const json& loglevel_obj = config["loglevel"];

        /*-------------------------------------------------*\
        | Set the log level if configured                   |
        \*-------------------------------------------------*/
        if(loglevel_obj.is_number_integer())
        {
            loglevel = loglevel_obj;
        }
    }

    /*-------------------------------------------------*\
    | Check log console configuration                   |
    \*-------------------------------------------------*/
    if(config.contains("log_console"))
    {
        log_console_enabled = config["log_console"];
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
            if(temp_messages[msg]->level <= loglevel || temp_messages[msg]->level == LL_DIALOG)
            {
                // Put the timestamp here
                std::chrono::milliseconds counter = std::chrono::duration_cast<std::chrono::milliseconds>(temp_messages[msg]->counted_second);
                log_stream << std::left << std::setw(6) << counter.count()  << "|";
                log_stream << std::left << std::setw(9) << log_codes[temp_messages[msg]->level];
                log_stream << temp_messages[msg]->buffer;
         
                if(print_source)
                {
                    log_stream << " [" << temp_messages[msg]->filename << ":" << temp_messages[msg]->line << "]";
                }
                
                log_stream << std::endl;
            }
        }

        /*-------------------------------------------------*\
        | Clear temp message buffers after writing them out |
        \*-------------------------------------------------*/
        temp_messages.clear();
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

void LogManager::_append(const char* filename, int line, unsigned int level, const char* fmt, va_list va)
{
    /*-------------------------------------------------*\
    | If a critical message occurs, enable source       |
    | printing and set loglevel and verbosity to highest|
    \*-------------------------------------------------*/
    if(level == LL_FATAL)
    {
        print_source = true;
        loglevel = LL_DEBUG;
        verbosity = LL_DEBUG;
    }

    /*-------------------------------------------------*\
    | Create a new message                              |
    \*-------------------------------------------------*/
    PLogMessage mes = std::make_shared<LogMessage>();

    /*-------------------------------------------------*\
    | Resize the buffer, then fill in the message text  |
    \*-------------------------------------------------*/
    va_list va2;
    va_copy(va2, va);
    int len = vsnprintf(nullptr, 0, fmt, va);
    mes->buffer.resize(len);
    vsnprintf(&(mes->buffer[0]), len + 1, fmt, va2);
    va_end(va2);

    /*-------------------------------------------------*\
    | Fill in message information                       |
    \*-------------------------------------------------*/
    mes->level          = level;
    mes->filename       = filename;
    mes->line           = line;
    mes->counted_second = std::chrono::steady_clock::now() - base_clock;

    /*-------------------------------------------------*\
    | If this is a dialog message, call the dialog show |
    | callback                                          |
    \*-------------------------------------------------*/
    if(level == LL_DIALOG)
    {
        for(size_t idx = 0; idx < dialog_show_callbacks.size(); idx++)
        {
            dialog_show_callbacks[idx](dialog_show_callback_args[idx], mes);
        }
    }

    /*-------------------------------------------------*\
    | If the message is within the current verbosity,   |
    | print it on the screen                            |
    | TODO: Put the timestamp here                      |
    \*-------------------------------------------------*/
    if(level <= verbosity || level == LL_DIALOG)
    {
        std::cout << mes->buffer;
        if(print_source)
        {
            std::cout << " [" << mes->filename << ":" << mes->line << "]";
        }
        std::cout << std::endl;
    }

    /*-------------------------------------------------*\
    | Add the message to the logfile queue              |
    \*-------------------------------------------------*/
    temp_messages.push_back(mes);

    if(log_console_enabled)
    {
        all_messages.push_back(mes);
    }

    /*-------------------------------------------------*\
    | Flush the queues                                  |
    \*-------------------------------------------------*/
    _flush();
}

std::vector<PLogMessage> LogManager::messages()
{
    return all_messages;
}

void LogManager::clearMessages()
{
    all_messages.clear();
}

void LogManager::append(const char* filename, int line, unsigned int level, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);

    std::lock_guard<std::mutex> grd(entry_mutex);
    _append(filename, line, level, fmt, va);

    va_end(va);
}

void LogManager::setLoglevel(unsigned int level)
{
    /*-------------------------------------------------*\
    | Check that the new log level is valid, otherwise  |
    | set it within the valid range                     |
    \*-------------------------------------------------*/
    if(level > LL_TRACE)
    {
        level = LL_TRACE;
    }

    LOG_DEBUG("Loglevel set to %d", level);

    /*-------------------------------------------------*\
    | Set the new log level                             |
    \*-------------------------------------------------*/
    loglevel = level;
}

void LogManager::setVerbosity(unsigned int level)
{
    /*-------------------------------------------------*\
    | Check that the new verbosity is valid, otherwise  |
    | set it within the valid range                     |
    \*-------------------------------------------------*/

    if(level > LL_TRACE)
    {
        level = LL_TRACE;
    }

    LOG_DEBUG("Verbosity set to %d", level);

    /*-------------------------------------------------*\
    | Set the new verbosity                             |
    \*-------------------------------------------------*/
    verbosity = level;
}

void LogManager::setPrintSource(bool v)
{
    LOG_DEBUG("Source code location printouts were %s", v ? "enabled" : "disabled");
    print_source = v;
}

void LogManager::RegisterDialogShowCallback(LogDialogShowCallback callback, void* receiver)
{
    LOG_DEBUG("dialog show callback registered");
    dialog_show_callbacks.push_back(callback);
    dialog_show_callback_args.push_back(receiver);
}

void LogManager::UnregisterDialogShowCallback(LogDialogShowCallback callback, void* receiver)
{
    for(size_t idx = 0; idx < dialog_show_callbacks.size(); idx++)
    {
        if(dialog_show_callbacks[idx] == callback && dialog_show_callback_args[idx] == receiver)
        {
            dialog_show_callbacks.erase(dialog_show_callbacks.begin() + idx);
            dialog_show_callback_args.erase(dialog_show_callback_args.begin() + idx);
        }
    }
}

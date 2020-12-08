#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <fstream>
#include <mutex>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

enum
{
    LL_CRITICAL,
    LL_ERROR,
    LL_WARNING,
    LL_NOTICE,
    LL_VERBOSE,
    LL_DEBUG
};

class LogManager
{
private:
    LogManager();
    LogManager(const LogManager&) = delete;
    LogManager(LogManager&&) = delete;
    ~LogManager();
    std::mutex entry_mutex;
    std::mutex section_mutex;
    std::vector<std::string> sections;
    std::vector<int> loglevels;
    std::ofstream log_stream;
    std::vector<std::string> temp_messages;
    std::vector<int> temp_levels;
    std::vector<int> temp_sections;
    void _flush();

public:
    static LogManager* get();
    void configure(json config, const std::string& defaultDir);
    void flush();
    void append(int section, int level, const char* fmt, ...);
    int registerSection(const char* name, int loglevel);
};

#define LogSection(name,level) LogManager::get()->registerSection(name, level)
#define LogAppend LogManager::get()->append

#endif // LOGMANAGER_H

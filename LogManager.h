#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <fstream>
#include <mutex>
#include <vector>
#include <queue>
#include <memory>
#include "json.hpp"

using json = nlohmann::json;

enum
{
    LL_CRITICAL,    // Critical unrecoverable errors that cause a generalized crash of a module or of the entire app
    LL_ERROR,       // Local errors that abort an operation
    LL_MESSAGE,     // Things the user should be informed of, not necessarily errors
    LL_WARNING,     // Local errors that may cause an operation to have an undefined behavior or may have dangerous/unforeseen consequences
    LL_NOTICE,      // Initialization messages, significant actions and follow-up information
    LL_VERBOSE,     // Tracing of commands and performed actions, usually for debug purposes, comments on the higher priority messages
    LL_DEBUG,       // Deep tracing, "printf-style debugging" alternative, for debug purposes. Such messages should be put all over the code instead of comments
};

struct LogMessage
{
    std::string buffer;
    unsigned int level;
    const char* filename;
    int line;
    // int timestamp or float time_offset? TBD
};
typedef std::shared_ptr<LogMessage> PLogMessage;
typedef void(*LogErrorCallback)(void*, PLogMessage);
typedef std::pair<LogErrorCallback, void*> LogErrorBlock;

class LogManager
{
private:
    LogManager();
    LogManager(const LogManager&) = delete;
    LogManager(LogManager&&) = delete;
    ~LogManager();
    std::mutex entry_mutex;
    std::mutex section_mutex;
    std::ofstream log_stream;

    std::vector<LogErrorBlock> error_callbacks;

    // A temporary log message storage to hold them until the stream opens
    std::vector<PLogMessage> temp_messages;

    // A flag that marks if the message source file name and line number should be printed on screen
    bool print_source = false;

    // Logfile max level
    unsigned int loglevel = LL_NOTICE;
    
    // Verbosity (stdout) max level
    unsigned int verbosity = LL_WARNING;

    // A non-guarded append()
    void _append(const char* filename, int line, unsigned int level, const char* fmt, va_list va);

    // A non-guarded flush()
    void _flush();

public:
    static LogManager* get();
    void configure(json config, const std::string& defaultDir);
    void flush();
    void append(const char* filename, int line, unsigned int level, const char* fmt, ...);
    void setLoglevel(unsigned int);
    void setVerbosity(unsigned int);
    void setPrintSource(bool);
    void registerErrorCallback(LogErrorCallback callback, void* receiver);
    void unregisterErrorCallback(LogErrorCallback callback, void* receiver);
    unsigned int getLoglevel() {return loglevel;}
    unsigned int getVerbosity() {return verbosity;}
};

#define LogAppend(level, ...)   LogManager::get()->append(__FILE__, __LINE__, level, __VA_ARGS__)
#define LOG_CRITICAL(...)       LogAppend(LL_CRITICAL,  __VA_ARGS__)
#define LOG_ERROR(...)          LogAppend(LL_ERROR,     __VA_ARGS__)
#define LOG_MESSAGE(...)        LogAppend(LL_MESSAGE,   __VA_ARGS__)
#define LOG_WARNING(...)        LogAppend(LL_WARNING,   __VA_ARGS__)
#define LOG_NOTICE(...)         LogAppend(LL_NOTICE,    __VA_ARGS__)
#define LOG_VERBOSE(...)        LogAppend(LL_VERBOSE,   __VA_ARGS__)
#define LOG_DEBUG(...)          LogAppend(LL_DEBUG,     __VA_ARGS__)

#endif // LOGMANAGER_H

#ifndef SERVER_LOG_H
#define SERVER_LOG_H

#include "src/settings.h"
#include <string>
#include <ctime>
#include <iostream>
#include <sstream>

class Log {
public:
    enum Level : std::int8_t {
        DEBUG = 0,
        INFO,
        WARN,
        ERROR,
        //FATAL
    };

    Log(Level level, Level defaultLevel, std::string prefix)
            : _level(level), _defaultLevel(defaultLevel) {
        buffer << "[" << currentTime() << "]" << "[" << prefix << "] ";
    };

    ~Log() {
        buffer << std::endl;
        if (_level >= _defaultLevel) {
            if (_level == Level::DEBUG) {
                std::cout << buffer.str();
            } else if (_level == Level::ERROR) {
                std::cerr << buffer.str();
            } else {
                std::clog << buffer.str();
            }
        }
    };

    template<typename T>
    Log &operator<<(T const &val) {
        if (_level >= _defaultLevel) {
            buffer << val;
        }
        return *this;
    };
private:
    Level _level;
    Level _defaultLevel;
    std::stringstream buffer;

    std::string currentTime() {
        char timeString[32];
        time_t t = time(nullptr);
        tm tm = *localtime(&t);
        strftime(timeString, sizeof(timeString), "%Y/%m/%d %T %z", &tm);
        return std::string(timeString);
    };
};

static Log::Level defaultLevel = static_cast<Log::Level>(LOG_DEFAULT_LEVEL);

#define LOG_DEBUG if (Log::Level::DEBUG >= defaultLevel) Log(Log::Level::DEBUG, defaultLevel, "DEBUG")
#define LOG_INFO  if (Log::Level::INFO  >= defaultLevel) Log(Log::Level::INFO , defaultLevel, "INFO ")
#define LOG_WARN  if (Log::Level::WARN  >= defaultLevel) Log(Log::Level::WARN , defaultLevel, "WARN ")
#define LOG_ERROR if (Log::Level::ERROR >= defaultLevel) Log(Log::Level::ERROR, defaultLevel, "ERROR")
// #define LOG_FATAL if (Log::Level::FATAL >= defaultLevel) Log(Log::Level::FATAL, defaultLevel, "FATAL")

#endif //SERVER_LOG_H

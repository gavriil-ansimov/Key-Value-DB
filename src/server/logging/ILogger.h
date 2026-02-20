#pragma once
#include <string>

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual void log(LogLevel lvl, const std::string& msg) = 0;
};
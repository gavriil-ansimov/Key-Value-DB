#pragma once

#include "ILogger.h"

class Logger : public ILogger {
public:
    Logger(LogLevel minLvl);

    void log(LogLevel lvl, const std::string& msg) override;
};
#include "Logger.h"

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

namespace keywords = boost::log::keywords;

static boost::log::trivial::severity_level toBoostLevel(LogLevel lvl) {
    switch(lvl) {
        case LogLevel::Trace:
            return boost::log::trivial::trace;
        case LogLevel::Debug:
            return boost::log::trivial::debug;
        case LogLevel::Info:
            return boost::log::trivial::info;
        case LogLevel::Warning:
            return boost::log::trivial::warning;
        case LogLevel::Error:
            return boost::log::trivial::error;
        case LogLevel::Fatal:
            return boost::log::trivial::fatal;
    }
    return boost::log::trivial::info;  
}

Logger::Logger(LogLevel minLvl) {
    boost::log::add_common_attributes();

    boost::log::add_file_log(
        keywords::file_name = "logs/server_%N.log",
        keywords::rotation_size = 5 * 1024 * 1024,
        keywords::format = "[%TimeStamp%] [%Severity%] [%ThreadID%]: %Message%"
    );

    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= toBoostLevel(minLvl)
    );
}

void Logger::log(LogLevel lvl, const std::string& msg) {
    switch (lvl)
    {
        case LogLevel::Trace:
            BOOST_LOG_TRIVIAL(trace) << msg;
            break;
        case LogLevel::Debug:
            BOOST_LOG_TRIVIAL(debug) << msg;
            break;
        case LogLevel::Info:
            BOOST_LOG_TRIVIAL(info) << msg;
            break;
        case LogLevel::Warning:
            BOOST_LOG_TRIVIAL(warning) << msg;
            break;
        case LogLevel::Error:
            BOOST_LOG_TRIVIAL(error) << msg;
            break;
        case LogLevel::Fatal:
            BOOST_LOG_TRIVIAL(fatal) << msg;
            break;
    }
}
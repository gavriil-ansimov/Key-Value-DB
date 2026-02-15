#include "Logger.h"

namespace keywords = boost::log::keywords;

void logging::init() {
    boost::log::add_common_attributes();

    boost::log::add_file_log(
        keywords::file_name = "logs/server_%N.log",
        keywords::rotation_size = 5 * 1024 * 1024,
        keywords::format = "[%TimeStamp%] [%Severity%] [%ThreadID%]: %Message%"
    );
#ifndef NDEBUG
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::debug
    );
#else
    boost::log::core::get()->set_filter(
        boost::log::trivial::severity >= boost::log::trivial::info
    );
#endif
}
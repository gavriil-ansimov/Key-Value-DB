cmake_minimum_required(VERSION 3.10)
project(kv_storage)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost REQUIRED COMPONENTS system log log_setup thread)

add_executable(kv_storage
    src/server/main.cpp
    src/server/database/Database.cpp
    src/server/command/Parser.cpp
    src/server/command/commands/GetCommand.cpp
    src/server/command/commands/PutCommand.cpp
    src/server/command/commands/DelCommand.cpp
    src/server/command/commands/DumpCommand.cpp
    src/server/command/commands/LoadCommand.cpp
    src/server/logging/Logger.cpp
    src/server/concurrency/WorkerPool.cpp
    src/server/network/Server.cpp
    src/server/network/Session.cpp
)
add_executable(client src/client/client.cpp)

target_include_directories(kv_storage PRIVATE
    src/server
    src/server/database
    src/server/command
    src/server/command/commands
    src/server/logging
    src/server/concurrency
    src/server/network
)

target_link_libraries(kv_storage 
    PRIVATE
        Boost::log
        Boost::log_setup
        Boost::system
        Boost::thread
)
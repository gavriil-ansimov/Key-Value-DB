cmake_minimum_required(VERSION 3.10)
project(kv_storage)

set(CMAKE_CXX_STANDARD 17)

# Ищем Boost
find_package(Boost REQUIRED COMPONENTS system)

# Главная программа
add_executable(kv_storage
    src/server/main.cpp
    src/server/database/Database.cpp
    src/server/command/Parser.cpp
    src/server/command/commands/GetCommand.cpp
    src/server/network/Server.cpp
    src/server/network/Session.cpp
)

# Инклюд пути
target_include_directories(kv_storage PRIVATE
    src/server
    src/server/database
    src/server/command
    src/server/command/commands
    src/server/network
)

# Линкуем Boost
target_link_libraries(kv_storage PRIVATE Boost::system)
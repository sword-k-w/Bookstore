cmake_minimum_required(VERSION 3.22)
project(BookStore)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_definitions(-Ddebug)

add_definitions(-DFileClear)

add_executable(code
        src/account.cpp
        src/book.cpp
        src/log.cpp
        src/command.cpp
        src/front_end.cpp
        src/main.cpp
)

add_executable(gen
        src/gen.cpp
)

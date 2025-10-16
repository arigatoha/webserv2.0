cmake_minimum_required(VERSION 3.10)

project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRCS "src/*.cpp")

add_executable(webserv ${SRCS})

target_include_directories(webserv PRIVATE include)

target_compile_options(webserv PRIVATE -Wall -Wextra -Werror -Iinclude)
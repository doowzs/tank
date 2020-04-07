cmake_minimum_required(VERSION 3.15)
project(tank)

file(GLOB CMAKE_CXX_SOURCES src/*.cpp)
file(GLOB CMAKE_CXX_HEADERS src/headers/*.h)
set(CMAKE_CXX_INCLUDE_PATH src/headers)
set(CMAKE_CXX_STANDARD 17)

find_library(Curses REQUIRED)
add_executable(tank ${CMAKE_CXX_HEADERS} ${CMAKE_CXX_SOURCES})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    list(APPEND CMAKE_CXX_COMPILE_FLAGS -Wall -Wextra -pedantic -Werror)
endif()

include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(tank PRIVATE ${CURSES_LIBRARIES})
target_compile_options(tank PRIVATE ${CMAKE_CXX_COMPILE_FLAGS})
